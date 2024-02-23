// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamsProjectSettings.h"

UTeamsProjectSettings::UTeamsProjectSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	typedef ETeamAttitude::Type EA;
	TeamAttitudes = {
			// Neutral		// Aliens		// Players
		{	EA::Friendly,	EA::Neutral,	EA::Neutral },	// Neutral
		{	EA::Neutral,	EA::Friendly,	EA::Hostile },	// Aliens
		{	EA::Neutral,	EA::Hostile,	EA::Friendly }	// Players
	};
}

const UTeamsProjectSettings* UTeamsProjectSettings::Get()
{
	return GetDefault<UTeamsProjectSettings>();
}

ETeamAttitude::Type UTeamsProjectSettings::GetAttitude(FGenericTeamId Of, FGenericTeamId Towards)
{
	TArray<FTeamAttitude> TeamAttitudes = UTeamsProjectSettings::Get()->TeamAttitudes;

	bool OfValid = TeamAttitudes.IsValidIndex(Of.GetId());
	bool TowardsValid = TeamAttitudes.IsValidIndex(Towards.GetId());

	if (OfValid && TowardsValid)
	{
		auto& Attitudes = TeamAttitudes[Of.GetId()].Attitude;

		if (Attitudes.IsValidIndex(Towards.GetId()))
		{
			return Attitudes[Towards.GetId()];
		}
	}
	return ETeamAttitude::Neutral;
}