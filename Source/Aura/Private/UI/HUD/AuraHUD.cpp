// Copyright JYJ


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"
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
