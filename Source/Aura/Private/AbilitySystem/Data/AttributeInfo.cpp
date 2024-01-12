// Copyright JYJ


#include "AbilitySystem/Data/AttributeInfo.h"

#include "AuraGameplayTag.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag& AttributeTag) const
{
	for (const FAuraAttributeInfo& Info : AttributeInfos)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}
	
	UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."),
	       *AttributeTag.ToString(), *GetNameSafe(this));
	return FAuraAttributeInfo();
}
