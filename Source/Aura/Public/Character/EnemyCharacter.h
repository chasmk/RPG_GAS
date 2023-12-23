// Copyright JYJ

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class AURA_API AEnemyCharacter : public ACharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//高亮显示
	
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	
private:

	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
