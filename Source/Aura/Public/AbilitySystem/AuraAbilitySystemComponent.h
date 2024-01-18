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

	//授予并激活角色初始能力，只在server上执行
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities);

	// PC中input action的callback 触发后调用下面的函数
	
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
protected:
	
	
	// FOnGameplayEffectAppliedDelegate 的 callback，当GE apply到这个Asc上时调用
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* Asc, const FGameplayEffectSpec& GESpec,
	                   FActiveGameplayEffectHandle ActiveGEHandle);

public:
};
