// Copyright JYJ


#include "AbilitySystem/MMC/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	//初始化Intelligence
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters AggregatorEvalParam;
	AggregatorEvalParam.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	AggregatorEvalParam.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//获取Intelligence值
	float IntelligenceMagnitude = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, AggregatorEvalParam, IntelligenceMagnitude);
	IntelligenceMagnitude = FMath::Max(0.f, IntelligenceMagnitude);

	//获取level值
	ICombatInterface* Ici = CastChecked<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int PlayerLevel = Ici->GetPlayerLevel();

	//返回计算结果
	return 10.f * IntelligenceMagnitude + 10.f * PlayerLevel;
}
