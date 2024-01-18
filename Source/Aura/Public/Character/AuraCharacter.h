// Copyright JYJ

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CharacterBase.h"
#include "AuraCharacter.generated.h"

class UGameplayAbility;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class AURA_API AAuraCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const override { return AttributeSet; };
	
	/** Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/** end Combat Interface */
	
protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;//server上初始化gas
	virtual void OnRep_PlayerState() override;//client上初始化gas
	virtual void InitAbilityActorInfo() override;//初始化GAS相关变量
	void AddCharacterAbilities() const;//初始化角色初始能力
	

private:
	UPROPERTY(EditAnywhere, Category="Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(EditAnywhere, Category="Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;

public:

};
