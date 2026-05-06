// Copyright Ian Good

#pragma once

#include "CoreMinimal.h"

class UDataTable;
class UScriptStruct;

namespace IG::DataTableEx::Private
{

/**
 * Same as `UDataTable::CommonTypeCheck`
 */
IGDATATABLEEX_API bool CommonTypeCheck(const UDataTable& DataTable, const UScriptStruct& Type, const TCHAR* MethodName, const TCHAR* ContextString);

template <typename RowType>
bool CommonTypeCheck(const UDataTable& DataTable, const TCHAR* MethodName, const TCHAR* ContextString)
{
	return CommonTypeCheck(DataTable, *RowType::StaticStruct(), MethodName, ContextString);
}

} // namespace IG::DataTableEx::Private
