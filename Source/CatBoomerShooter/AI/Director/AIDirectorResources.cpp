// Fill out your copyright notice in the Description page of Project Settings.


#include "AIDirectorResources.h"


FTokenCollection* FActorTokensCollection::GetCollectionOfType(ETokenType TokenType)
{
	switch (TokenType)
	{
		case ETokenType::Normal:
			return &NormalTokens;
		case ETokenType::Heavy:
			return &HeavyTokens;
		default:
			UE_LOG(LogTemp, Error, TEXT("FActorTokensCollection::GetCollectionOfType: Invalid Token Type provided."));
			return nullptr;
	}
}