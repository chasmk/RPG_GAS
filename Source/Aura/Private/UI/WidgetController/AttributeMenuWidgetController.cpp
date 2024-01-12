// Copyright JYJ


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTag.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* As = Cast<UAuraAttributeSet>(AttributeSet);
	check(As);

	for (const auto Pair : As->Tag2AttrMap)
	{//遍历map里所有属性，并把属性值打包在info里发送到ui
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAbilitySystemComponent* Asc = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	UAuraAttributeSet* As = Cast<UAuraAttributeSet>(AttributeSet);
	check(As);
	check(Asc);

	for (auto Pair : As->Tag2AttrMap)
	{//遍历map里所有属性，每个属性都绑到Asc更新的delegate上
		Asc->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			});
	}
	
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(FGameplayTag Tag, const FGameplayAttribute& Attr) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(Tag);
	Info.AttributeValue = Attr.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
