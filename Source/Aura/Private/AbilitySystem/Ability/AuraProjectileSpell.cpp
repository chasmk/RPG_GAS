// Copyright JYJ


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	
}

void UAuraProjectileSpell::SpawnActor(const FVector& TargetLocation)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());//这里avatar是角色类
	if (CombatInterface)
	{
		const FVector SourceLocation = CombatInterface->GetCombatSocketLocation();
		const FRotator TargetRotator = (TargetLocation - SourceLocation).Rotation();
		/*notify触发后再生成*/
		FTransform Transform;
		Transform.SetLocation(SourceLocation);
		Transform.SetRotation(TargetRotator.Quaternion());
		//DrawDebugBox(GetWorld(), Transform.GetLocation(), FVector(100), FColor::Red, true);

		
		 AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileSpellClass,
			Transform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),//这里own actor是player state类
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
			ESpawnActorScaleMethod::MultiplyWithRoot);

		//TODO: 应用GE，造成伤害
		
		Projectile->FinishSpawning(Transform);
	}
}
