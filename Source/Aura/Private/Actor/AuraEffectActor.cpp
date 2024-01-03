// Copyright JYJ


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

/*
void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IAbilitySystemInterface* ASInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (ASInterface)
	{
		const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(ASInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));
		UAuraAttributeSet* MutableAuraAttributeSet = const_cast<UAuraAttributeSet*>(AuraAttributeSet);
		MutableAuraAttributeSet->SetHealth(AuraAttributeSet->GetHealth() + 25.f);
		MutableAuraAttributeSet->SetMana(AuraAttributeSet->GetMana()-20);
		Destroy();
	}
}
*/

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	//Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GEClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	
	if (TargetASC == nullptr) return;
	if (GEClass == nullptr) return;

	//以下功能蓝图里也能做
	//handle用来管理访问GEContext
	FGameplayEffectContextHandle GEContextHandle = TargetASC->MakeEffectContext();
	GEContextHandle.AddSourceObject(this);//??不懂它的作用
	//handle用来管理访问GESpec（GE的简化版？）
	const FGameplayEffectSpecHandle GEHandle = TargetASC->MakeOutgoingSpec(GEClass, ActorLevel, GEContextHandle);
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*GEHandle.Data.Get());//应用GE到自己(ASC)上

	//如果是infinite类型
	const bool bIsInfinite = GEHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandlesMap.Add(ActiveGameplayEffectHandle, TargetASC);
	}
	
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGEClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGEClass);
	}
	if(InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToTarget(TargetActor,InfiniteGEClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGEClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGEClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InfiniteGEClass);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		TArray<FActiveGameplayEffectHandle> ActiveGEHandleArray;//记录当前asc上所有对应的ActiveGE
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> Pair : ActiveEffectHandlesMap)
		{
			if (Pair.Value == TargetASC)
			{
				TargetASC->RemoveActiveGameplayEffect(Pair.Key, 1);//移除ASC上该GE,并且只移除一个stack
				ActiveGEHandleArray.Add(Pair.Key);
			}
		}

		for (FActiveGameplayEffectHandle i : ActiveGEHandleArray)
		{
			ActiveEffectHandlesMap.FindAndRemoveChecked(i);//移除缓存的GEHandle和ASC pair
		}
		 
	}
}


