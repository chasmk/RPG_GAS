// Copyright JYJ

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

struct FWidgetControllerParams;
class UOverlayWidgetController;
class UAuraWidgetController;
class UAuraUserWidget;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	//单例模式
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	//初始化Overlay的Widget和widgetController
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:

private:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
public:
};
