// Copyright Ian Good

#include "IGDataTableEx/Impl/Common.h"

namespace IG::DataTableEx::Private
{
/**
 * Same as `UDataTable::CommonTypeCheck`
 */
bool CommonTypeCheck(const UDataTable& DataTable, const UScriptStruct& Type, const TCHAR* MethodName, const TCHAR* ContextString)
{
	if (DataTable.RowStruct == nullptr)
	{
		UE_LOG(LogDataTable, Error, TEXT("%s : DataTable '%s' has no RowStruct specified (%s)."), MethodName, *DataTable.GetPathName(), ContextString);
		return false;
	}

	if (!DataTable.RowStruct->IsChildOf(&Type))
	{
		UE_LOG(LogDataTable, Error, TEXT("%s : Incorrect type specified for DataTable '%s' (%s)."), MethodName, *DataTable.GetPathName(), ContextString);
		return false;
	}

	return true;
}

} // namespace IG::DataTableEx::Private
