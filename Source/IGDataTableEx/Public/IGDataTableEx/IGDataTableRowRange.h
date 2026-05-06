// Copyright Ian Good

#pragma once

#include "Engine/DataTable.h"
#include "IGDataTableEx/Impl/Common.h"

template <typename T, bool bWithNames = true, bool bIsConst = true>
struct TIGDataTableRowRange final
{
	using RowType = std::conditional_t<bIsConst, const T, T>;
	using DataTableType = std::conditional_t<bIsConst, const UDataTable, UDataTable>;
	using RowMapType = std::conditional_t<bIsConst, const TMap<FName, RowType*>, TMap<FName, RowType*>>;
	using RowMapIteratorType = decltype(RowMapType().begin());

	[[nodiscard]] UE_FORCEINLINE_HINT explicit TIGDataTableRowRange(DataTableType& DataTable, const TCHAR* ContextString = TEXT("unspecified context"))
		: bIsValid(IG::DataTableEx::Private::CommonTypeCheck<RowType>(DataTable, TEXT(__FUNCTION__), ContextString))
		, RowMap(const_cast<RowMapType&>(reinterpret_cast<const RowMapType&>(DataTable.GetRowMap())))
	{
	}

	[[nodiscard]] UE_FORCEINLINE_HINT auto begin() const { return FIterator(bIsValid ? RowMap.begin() : RowMap.end()); }

	[[nodiscard]] UE_FORCEINLINE_HINT auto end() const { return FIterator(RowMap.end()); }

private:
	struct FIterator final : public RowMapIteratorType
	{
		using PairType = TPair<const FName&, RowType&>;
		using IteratorElementType = std::conditional_t<bWithNames, PairType, RowType&>;

		[[nodiscard]] UE_FORCEINLINE_HINT explicit FIterator(RowMapIteratorType&& Iter)
			: RowMapIteratorType(MoveTemp(Iter))
		{
		}

		[[nodiscard]] UE_FORCEINLINE_HINT IteratorElementType operator*() const
		{
			const auto& SuperPair = RowMapIteratorType::operator*();
			if constexpr (bWithNames)
			{
				return PairType(SuperPair.Key, *SuperPair.Value);
			}
			else
			{
				return *SuperPair.Value;
			}
		}
	};

	const bool bIsValid;
	RowMapType& RowMap;
};

namespace IG::DataTableEx::Private
{
template <typename T, typename DataTableType>
auto MakeRowRangeImpl(DataTableType& DataTable, const TCHAR* ContextString)
{
	return TIGDataTableRowRange<T, /*bWithNames=*/false, /*bIsConst=*/std::is_const_v<DataTableType>>(DataTable, ContextString);
}

template <typename T, typename DataTableType>
auto MakeRowRangeImpl(DataTableType* DataTable, const TCHAR* ContextString)
{
	check(DataTable);
	return MakeRowRangeImpl<T>(*DataTable, ContextString);
}

} // namespace IG::DataTableEx::Private

#define MakeRowRange(RowType, DataTable) ::IG::DataTableEx::Private::MakeRowRangeImpl<RowType>(DataTable, TEXT(__FUNCTION__))

#define IG_FOREACH_ROW(RowType, Row, DataTable) for (auto& Row : MakeRowRange(RowType, DataTable))
