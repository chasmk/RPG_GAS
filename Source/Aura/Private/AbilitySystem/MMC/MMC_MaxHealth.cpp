// Copyright JYJ


#include "AbilitySystem/MMC/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	//初始化vigor
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters AggregatorEvalParam;
	AggregatorEvalParam.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	AggregatorEvalParam.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//获取vigor值
	float VigorMagnitude = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, AggregatorEvalParam, VigorMagnitude);
	VigorMagnitude = FMath::Max(0.f, VigorMagnitude);

	//获取level值
	ICombatInterface* Ici = CastChecked<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int PlayerLevel = Ici->GetPlayerLevel();

	//返回计算结果
	return 10.f * VigorMagnitude + 10.f * PlayerLevel;
}
