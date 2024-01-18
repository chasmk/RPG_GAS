// Copyright JYJ


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (auto Item : AbilityInputActions)
	{
		if (Item.InputAction && InputTag.MatchesTag(Item.InputTag))
		{
			return Item.InputAction;
		}
	}
	return nullptr;
}
