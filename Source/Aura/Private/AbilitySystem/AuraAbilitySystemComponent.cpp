// Copyright JYJ


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTag.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
	
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	GEngine->AddOnScreenDebugMessage(
		-1,
		10.f,
		FColor::Orange,
		FString::Printf(TEXT("Tag: %s"), *GameplayTags.Tag_Secondary_Armor.ToString())
		);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* Asc, const FGameplayEffectSpec& GESpec,
                                                FActiveGameplayEffectHandle ActiveGEHandle)
{
	FGameplayTagContainer TagContainer;
	GESpec.GetAllAssetTags(TagContainer);
	EffectAssetTagDelegate.Broadcast(TagContainer);
	
}
