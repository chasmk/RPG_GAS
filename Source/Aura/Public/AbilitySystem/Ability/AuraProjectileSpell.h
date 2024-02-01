// Copyright JYJ

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/**
	 * @brief 生成一个朝向target的projectile
	 * @param TargetLocation 要命中的目标target的位置
	 */
	UFUNCTION(BlueprintCallable)
	void SpawnActor(const FVector& TargetLocation);
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AAuraProjectile> ProjectileSpellClass;

};
