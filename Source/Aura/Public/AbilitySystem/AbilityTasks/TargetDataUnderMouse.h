// Copyright JYJ

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

//广播鼠标命中地点的delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataDelegate, const FGameplayAbilityTargetDataHandle&, DataHandle);
/**
 * 
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
public:
	//用于蓝图中调用，获取该类的实例
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="TargetDataUnderMouse",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataDelegate ValidData;//delegate，蓝图中再assign

private:
	virtual void Activate() override;//用于广播delegate信息
	void SendMouseCursorData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
