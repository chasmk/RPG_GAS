// Copyright JYJ

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "AuraCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class AURA_API AAuraCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();

protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category="Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(EditAnywhere, Category="Camera")
	TObjectPtr<UCameraComponent> FollowCamera;
public:
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
