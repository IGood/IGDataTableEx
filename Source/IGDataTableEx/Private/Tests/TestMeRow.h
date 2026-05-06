// Copyright Ian Good

#pragma once

#include "Engine/DataTable.h"
#include "TestMeRow.generated.h"

USTRUCT()
struct FTestMeRow : public FTableRowBase
{
	GENERATED_BODY()

	FTestMeRow() = default;

	FTestMeRow(FString InString, int32 InInt32)
		: MyString(MoveTemp(InString))
		, MyInt32(InInt32)
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString MyString;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MyInt32 = 0;
};
