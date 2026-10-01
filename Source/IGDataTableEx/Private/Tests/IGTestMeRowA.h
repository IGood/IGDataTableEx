// Copyright Ian Good

#pragma once

#include "Engine/DataTable.h"
#include "IGTestMeRowA.generated.h"

/**
 * Represents a row in a data table for testing purposes.
 */
USTRUCT()
struct FIGTestMeRowA : public FTableRowBase
{
	GENERATED_BODY()

	FIGTestMeRowA() = default;

	FIGTestMeRowA(FString InString, int32 InInt32)
		: MyString(MoveTemp(InString))
		, MyInt32(InInt32)
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString MyString;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MyInt32 = 0;
};
