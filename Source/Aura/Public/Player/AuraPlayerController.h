// Copyright JYJ

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraInputConfig;
struct FGameplayTag;
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
	FHitResult CursorHitResult;

	// GAS输入系统绑定的callback
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;//里面存放input action和tag的数值

	//获取关联的ASC
	 
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	UAuraAbilitySystemComponent* GetAsc();

	// 点击移动相关

	FVector CachedDestination = FVector::ZeroVector;//缓存的目标位置
	float FollowTime = 0.f;//记录按键按下的时间，判断是长按还是点按
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;//为true时每帧都移动
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;//这个范围内可以停止auto run

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;//我们到目标点的曲线

	void AutoRun();
public:
};
