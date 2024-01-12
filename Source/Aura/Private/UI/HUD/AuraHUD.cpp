// Copyright JYJ


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"


UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (!OverlayWidgetController)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (!AttributeMenuWidgetController)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
	}
	return AttributeMenuWidgetController;
}

void AAuraHUD::InitHUD(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	InitOverlay(PC, PS, ASC, AS);
	InitAttributeMenu(PC, PS, ASC, AS);
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass,
	       TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"));

	//初始化widget controller
	const FWidgetControllerParams WCParams(PC, PS, ASC, AS);
	OverlayWidgetController = GetOverlayWidgetController(WCParams);
	//初始化widget
	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget->SetWidgetController(OverlayWidgetController);
	//绑定更新属性值所需的callback
	OverlayWidgetController->BindCallbacksToDependencies();
	//初始化属性值
	OverlayWidgetController->BroadcastInitialValues();

	//显示在屏幕上
	OverlayWidget->AddToViewport();
}

void AAuraHUD::InitAttributeMenu(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
	UAttributeSet* AS)
{
	checkf(AttributeMenuWidgetClass, TEXT("AttributeMenu Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(AttributeMenuWidgetControllerClass,
		   TEXT("AttributeMenu Widget Controller Class uninitialized, please fill out BP_AuraHUD"));

	//初始化widget controller
	const FWidgetControllerParams WCParams(PC, PS, ASC, AS);
	AttributeMenuWidgetController = GetAttributeMenuWidgetController(WCParams);
	//初始化widget
	AttributeMenuWidget = CreateWidget<UAuraUserWidget>(GetWorld(), AttributeMenuWidgetClass);
	AttributeMenuWidget->SetWidgetController(AttributeMenuWidgetController);
	//绑定更新属性值所需的callback
	AttributeMenuWidgetController->BindCallbacksToDependencies();
	//初始化属性值
	AttributeMenuWidgetController->BroadcastInitialValues();

	//显示在屏幕上
	//AttributeMenuWidget->AddToViewport();
}
