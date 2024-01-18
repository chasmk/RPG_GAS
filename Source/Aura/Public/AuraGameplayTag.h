// Copyright JYJ

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTag
 * 单例struct，包含原生的gameplay tag
 */
struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Tag_Primary_Strength;
	FGameplayTag Tag_Primary_Intelligence;
	FGameplayTag Tag_Primary_Resilience;
	FGameplayTag Tag_Primary_Vigor;
	
	FGameplayTag Tag_Secondary_Armor;
	FGameplayTag Tag_Secondary_ArmorPenetration;
	FGameplayTag Tag_Secondary_BlockChance;
	FGameplayTag Tag_Secondary_CriticalHitChance;
	FGameplayTag Tag_Secondary_CriticalHitDamage;
	FGameplayTag Tag_Secondary_CriticalHitResistance;
	FGameplayTag Tag_Secondary_HealthRegeneration;
	FGameplayTag Tag_Secondary_ManaRegeneration;
	FGameplayTag Tag_Secondary_MaxHealth;
	FGameplayTag Tag_Secondary_MaxMana;

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

private:
	static FAuraGameplayTags GameplayTags;
};
