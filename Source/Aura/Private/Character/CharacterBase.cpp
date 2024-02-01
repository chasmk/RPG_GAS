// Copyright JYJ


#include "Character/CharacterBase.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//不与相机碰撞
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), "WeaponHandSocket");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::InitAbilityActorInfo()
{
}

FVector ACharacterBase::GetCombatSocketLocation()
{
	if (!IsValid(Weapon)) return FVector();
	return Weapon->GetSocketLocation(HipSocketName);
}

void ACharacterBase::InitializeDefaultAttributes() const
{
	check(IsValid(DefaultVitalAttributeGE));
	check(IsValid(DefaultPrimaryAttributeGE));
	check(IsValid(DefaultSecondaryAttributeGE));
	ApplyInitialEffect(DefaultPrimaryAttributeGE, 1.f);
	ApplyInitialEffect(DefaultSecondaryAttributeGE, 1.f);
	ApplyInitialEffect(DefaultVitalAttributeGE, 1.f);//放到第一个时有bug，不知道为什么
}

void ACharacterBase::ApplyInitialEffect(const TSubclassOf<UGameplayEffect> GameplayEffectClass, const float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}
