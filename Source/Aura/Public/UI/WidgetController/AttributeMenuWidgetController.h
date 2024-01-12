// Copyright JYJ

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FAuraAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeInfoDelegate, const FAuraAttributeInfo&, NewAttributeInfo);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	void BroadcastAttributeInfo(FGameplayTag Tag, const FGameplayAttribute& Attr) const;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeInfoDelegate AttributeInfoDelegate;
protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;//存放所有attribute信息的data asset
	
};
