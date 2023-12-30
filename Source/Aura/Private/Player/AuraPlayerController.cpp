// Copyright JYJ


#include "Player/AuraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	//https://docs.unrealengine.com/5.3/en-US/networking-overview-for-unreal-engine/
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	TraceCursor();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//以下设置增强输入系统
	check(AuraContext); //类似于assert

	UEnhancedInputLocalPlayerSubsystem* Subsystem = //单例形式存在
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0); //把IMC添加到子系统中
	}


	//设置输入模式，让鼠标总是显示
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::SlashedCircle;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false); //鼠标点下时不隐藏
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//把输入组件强制转换为增强版, 这个类可以在项目设置->input里设置
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	//在action上绑定函数，每次trigger时都会调用这个函数
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f); //对于前后左右，只取yaw即可
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::TraceCursor()
{
	FHitResult HitResult;
	//获取鼠标上的hit结果
	//GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, HitResult);
	if (!HitResult.bBlockingHit) return;

	LastActor = CurrentActor;
	CurrentActor = Cast<IEnemyInterface>(HitResult.GetActor());

	if (!LastActor && !CurrentActor) return; //两个都为空
	if (!LastActor && CurrentActor)
	{
		CurrentActor->HighlightActor();
	}
	else if (LastActor && !CurrentActor)
	{
		LastActor->UnHighlightActor();
	}
	else if (LastActor != CurrentActor)
	{
		LastActor->UnHighlightActor();
		CurrentActor->HighlightActor();
	}
}
