// Copyright JYJ


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	//Super::BroadcastInitialValues();
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnHealthChangedDelegate.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChangedDelegate.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChangedDelegate.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChangedDelegate.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	//Super::BindCallbacksToDependencies();
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	//获取ASC中Health属性改变时触发的delegate
	//FOnGameplayAttributeValueChange& OnGameplayAttributeValueChange
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddUObject(
		this, &UOverlayWidgetController::UpdateHealth);
	//往这个delegate上绑上我们的callback，注意参数一致
	//OnGameplayAttributeValueChange

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).
	                        AddUObject(this, &UOverlayWidgetController::UpdateMaxHealth);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddUObject(
		this, &UOverlayWidgetController::UpdateMana);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddUObject(
		this, &UOverlayWidgetController::UpdateMaxMana);
}

void UOverlayWidgetController::UpdateHealth(const FOnAttributeChangeData& Data) const
{
	OnHealthChangedDelegate.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::UpdateMaxHealth(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChangedDelegate.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::UpdateMana(const FOnAttributeChangeData& Data) const
{
	OnManaChangedDelegate.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::UpdateMaxMana(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChangedDelegate.Broadcast(Data.NewValue);
}
