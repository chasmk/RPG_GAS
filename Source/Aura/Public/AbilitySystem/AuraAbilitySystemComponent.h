// Copyright JYJ

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagDelegate, const FGameplayTagContainer&/*asset tags*/);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	//绑定call back的地方
	void AbilityActorInfoSet();
	FEffectAssetTagDelegate EffectAssetTagDelegate;//用于广播GE的tags

protected:
	
	
	// FOnGameplayEffectAppliedDelegate 的 callback，当GE apply到这个Asc上时调用
	void EffectApplied(UAbilitySystemComponent* Asc, const FGameplayEffectSpec& GESpec,
	                   FActiveGameplayEffectHandle ActiveGEHandle);

public:
};
