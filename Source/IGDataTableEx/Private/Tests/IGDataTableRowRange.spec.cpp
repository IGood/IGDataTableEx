// Copyright Ian Good

#include "IGDataTableEx/IGDataTableRowRange.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestMeRow.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FIGDataTableRowRangeSpec, "IG.DataTableEx.range-for", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

void FIGDataTableRowRangeSpec::Define()
{
	It("foreach", [this]() {
		UDataTable* DataTable = NewObject<UDataTable>();
		DataTable->RowStruct = FTestMeRow::StaticStruct();

		DataTable->AddRow("row0", FTestMeRow(TEXT("foo"), 123));
		DataTable->AddRow("row1", FTestMeRow(TEXT("bar"), 456));
		DataTable->AddRow("row2", FTestMeRow(TEXT("baz"), 789));

		TArray<FTestMeRow*> Expected;
		DataTable->GetAllRows(TEXT(__FUNCTION__), Expected);

		TArray<FTestMeRow*> Actual;
		for (auto& Row : TIGDataTableRowRange<FTestMeRow, /*bWithNames=*/false, /*bIsConst=*/false>(*DataTable))
		{
			Actual.Emplace(&Row);
		}

		TestEqual(TEXT("visited rows"), Actual, Expected);
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
