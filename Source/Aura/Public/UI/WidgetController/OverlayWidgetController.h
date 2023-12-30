// Copyright JYJ

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedDelegate, float, NewHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedDelegate, float, NewMaxHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedDelegate, float, NewMana);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedDelegate, float, NewMaxMana);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnHealthChangedDelegate OnHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxHealthChangedDelegate OnMaxHealthChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnManaChangedDelegate OnManaChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxManaChangedDelegate OnMaxManaChangedDelegate;

protected:
	void UpdateHealth(const FOnAttributeChangeData& Data) const;
	void UpdateMaxHealth(const FOnAttributeChangeData& Data) const;
	void UpdateMana(const FOnAttributeChangeData& Data) const;
	void UpdateMaxMana(const FOnAttributeChangeData& Data) const;
};
