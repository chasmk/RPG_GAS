// Copyright JYJ


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTag.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Player/AuraPlayerState.h"

AAuraPlayerController::AAuraPlayerController()
{
	//https://docs.unrealengine.com/5.3/en-US/networking-overview-for-unreal-engine/
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	TraceCursor();
	AutoRun();
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

	//把输入组件强制转换为aura版, 这个类可以在项目设置->input里设置
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	//在action上绑定函数，每次trigger时都会调用这个函数
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);

	//自定义的绑定函数
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
	                                       &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
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
	//获取鼠标上的hit结果
	//GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, CursorHitResult);
	if (!CursorHitResult.bBlockingHit) return;

	LastActor = CurrentActor;
	CurrentActor = Cast<IEnemyInterface>(CursorHitResult.GetActor());

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

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		//用现成函数获取direction (应该是切线方向)
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);

		DrawDebugDirectionalArrow(GetWorld(), ControlledPawn->GetActorLocation(),
		                          ControlledPawn->GetActorLocation() + Direction * 10, 20.f, FColor::Red, false, 2.f);

		ControlledPawn->AddMovementInput(Direction);

		//阈值范围内停止自动移动
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = CurrentActor ? true : false; //鼠标在敌人身上时为true
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Blue, *InputTag.ToString());
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB)) //非鼠标左键
	{
		if (GetAsc())
		{
			GetAsc()->AbilityInputTagReleased(InputTag);
		}
		return;
	}
	if (GetAsc()) GetAsc()->AbilityInputTagReleased(InputTag);

	if (!bTargeting && !bShiftKeyDown)
	{
		APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn) //小于阈值，说明是点按
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				//调用寻路算法找到一条路径
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					//把路径上的点加到spline中
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
				}
				if (NavPath->PathPoints.Num() > 0)
				{
					//更新目标点为生成路径的最后一个点，避免原始目标点不可达导致乱跑的问题
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				}
				bAutoRunning = true; //让角色自动走到目的地
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, *InputTag.ToString());
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB)) //非鼠标左键
	{
		if (GetAsc())
		{
			GetAsc()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	if (bTargeting || bShiftKeyDown)
	{
		if (GetAsc())
		{
			GetAsc()->AbilityInputTagHeld(InputTag);
		}
	}
	else //点击移动的逻辑
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		if (CursorHitResult.bBlockingHit)
		{
			CachedDestination = CursorHitResult.ImpactPoint;
		}
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAsc()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
			GetPlayerState<AAuraPlayerState>()->GetAbilitySystemComponent());
	}
	return AuraAbilitySystemComponent;
}
