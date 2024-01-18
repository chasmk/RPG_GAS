// Copyright JYJ


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"


AAuraCharacter::AAuraCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Camera");
	FollowCamera->SetupAttachment(SpringArmComponent, SpringArmComponent->SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//设置角色视角
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

UAbilitySystemComponent* AAuraCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

int32 AAuraCharacter::GetPlayerLevel()
{
	return CastChecked<AAuraPlayerState>(GetPlayerState())->GetPlayerLevel();
}

void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
	AddCharacterAbilities();//只在server上调用
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	//获取player state
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();

	if (AuraPlayerState)//初始化PlayerState里GAS相关变量
	{
		//赋值
		AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
		AttributeSet = AuraPlayerState->GetAttributeSet();
		//初始化Asc
		AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
		//初始化AuraAsc
		Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GAS Initialize Failed, 因为AuraPlayerState获取失败！"))
	}

	//初始化AS中的属性值
	InitializeDefaultAttributes();

	//通过HUD 初始化Widget和WidgetController
	if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			HUD->InitHUD(PC, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

void AAuraCharacter::AddCharacterAbilities() const
{
	UAuraAbilitySystemComponent* Asc = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent());

	//保证Ga只在server上操作
	if (!HasAuthority()) return;
	
	Asc->AddCharacterAbilities(StartUpAbilities);
}


