// Copyright Ian Good

#pragma once

#include "Engine/DataTable.h"
#include "IGDataTableEx/Impl/Common.h"

/**
 * Represents an object that enables type safe range-for support for a Data Table.
 *
 * When iterating one of these, the iterator element type is `TPair<FName, RowType*>`.
 *
 * See example usage below in `DoStuff`.
 */
template <typename RowType>
struct TIGDataTableEx final
{
	using RowMapType = const TMap<FName, RowType*>;

	[[nodiscard]] FORCEINLINE TIGDataTableEx(const UDataTable& DataTable, const TCHAR* ContextString = TEXT("unspecified context"))
		: bIsValid(::IG::DataTableEx::Private::CommonTypeCheck<RowType>(DataTable, ContextString, TEXT(__FUNCTION__)))
		, RowMap(reinterpret_cast<RowMapType&>(DataTable.GetRowMap()))
	{
		// The `reinterpret_cast` for `RowMap` may look a little fishy but this is how `UCurveTable` does things too & we
		// know how C++ works, so it's fine.
	}

	/**
	 * Returns whether this instance is valid, meaning that `RowType` is compatible with the Data Table's row struct.
	 * You do *not* need to check this before calling other functions.
	 */
	[[nodiscard]] FORCEINLINE bool IsValid() const { return bIsValid; }

	[[nodiscard]] FORCEINLINE int32 Num() const { return bIsValid ? RowMap.Num() : 0; }

	[[nodiscard]] FORCEINLINE bool ContainsRow(const FName& RowName) const { return bIsValid && RowMap.Contains(RowName); }

	[[nodiscard]] FORCEINLINE const RowType* FindRow(const FName& RowName) const { return bIsValid ? RowMap.FindRef(RowName) : nullptr; }

	[[nodiscard]] FORCEINLINE const RowType* FindRowByPredicate(TFunctionRef<bool(const FName& RowName, const RowType& Row)> Predicate) const
	{
		for (const auto& [RowName, Row] : *this)
		{
			if (Predicate(RowName, *Row))
			{
				return Row;
			}
		}

		return nullptr;
	}

	[[nodiscard]] FORCEINLINE auto begin() const { return bIsValid ? RowMap.begin() : RowMap.end(); }

	[[nodiscard]] FORCEINLINE auto end() const { return RowMap.end(); }

private:
	const bool bIsValid;
	RowMapType& RowMap;
};

/**
 * Creates a `TIGDataTableEx` with the given row type and Data Table, with the implicit "context" for the usage
 * being the name of the current function.
 *
 * See example usage below in `DoStuff`.
 */
#define MakeIGDataTableEx(RowType, DataTable) TIGDataTableEx<RowType>(DataTable, TEXT(__FUNCTION__))

// Example code demonstrating intended use of `TIGDataTableEx`:
#if 0

void DoStuff(const UDataTable& MyDataTable)
{
	for (const auto& [RowName, Row] : MakeIGDataTableEx(FMyRow, MyDataTable))
	{
		// These are the types & names of the structured binding variables:
		// const FName& RowName
		// FMyRow* const Row
		UE_LOG(LogTemp, Display, TEXT("Row '%s' has value %d"), *RowName.ToString(), Row->SomeInt);
	}
}

#endif // 0
