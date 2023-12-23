// Copyright JYJ

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class IEnemyInterface;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	void PlayerTick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	//增强输入系统

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;//IMC

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;//IA

	void Move(const FInputActionValue& InputActionValue);

	//鼠标显示追踪

	void TraceCursor();
	TObjectPtr<IEnemyInterface> CurrentActor;
	TObjectPtr<IEnemyInterface> LastActor;
	
	
public:
};
