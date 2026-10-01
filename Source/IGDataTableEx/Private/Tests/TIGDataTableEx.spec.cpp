// Copyright Ian Good

#include "IGDataTableEx/TIGDataTableEx.h"
#include "Misc/AutomationTest.h"
#include "Tests/IGTestMeRowA.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FTIGDataTableExSpec, "IG.TIGDataTableEx", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

static const UDataTable* MakeTestTable()
{
	UDataTable* DataTable = NewObject<UDataTable>();
	DataTable->RowStruct = FIGTestMeRowA::StaticStruct();

	DataTable->AddRow("row0", FIGTestMeRowA(TEXT("foo"), 123));
	DataTable->AddRow("row1", FIGTestMeRowA(TEXT("bar"), 456));
	DataTable->AddRow("row2", FIGTestMeRowA(TEXT("baz"), 789));

	return DataTable;
}

void FTIGDataTableExSpec::Define()
{
	It("range-for", [this]() {
		const UDataTable* DataTable = MakeTestTable();

		TArray<FIGTestMeRowA*> Expected;
		DataTable->GetAllRows(TEXT(__FUNCTION__), Expected);

		TArray<FIGTestMeRowA*> Actual;
		for (auto& [RowName, Row] : MakeIGDataTableEx(FIGTestMeRowA, *DataTable))
		{
			Actual.Emplace(Row);
		}

		TestEqual(TEXT("visited rows"), Actual, Expected);
	});

	It("ContainsRow", [this]() {
		const UDataTable* DataTable = MakeTestTable();
		const TIGDataTableEx DataTableEx = MakeIGDataTableEx(FIGTestMeRowA, *DataTable);

		UTEST_TRUE_EXPR(DataTableEx.ContainsRow("row0"));
		UTEST_FALSE_EXPR(DataTableEx.ContainsRow("unknown"));

		return true;
	});

	It("FindRow", [this]() {
		const UDataTable* DataTable = MakeTestTable();
		const TIGDataTableEx DataTableEx = MakeIGDataTableEx(FIGTestMeRowA, *DataTable);

		UTEST_NOT_NULL_EXPR(DataTableEx.FindRow("row0"));
		UTEST_NULL_EXPR(DataTableEx.FindRow("unknown"));

		return true;
	});

	It("FindRowByPredicate", [this]() {
		const UDataTable* DataTable = MakeTestTable();
		const TIGDataTableEx DataTableEx = MakeIGDataTableEx(FIGTestMeRowA, *DataTable);

		const FIGTestMeRowA* FoundRow = DataTableEx.FindRowByPredicate([](const FName& RowName, const FIGTestMeRowA& Row) {
			return Row.MyInt32 == 456;
		});
		UTEST_NOT_NULL(TEXT("row with 456"), FoundRow);
		UTEST_EQUAL_EXPR(FoundRow->MyInt32, 456);

		FoundRow = DataTableEx.FindRowByPredicate([](const FName& RowName, const FIGTestMeRowA& Row) {
			return Row.MyInt32 == -999;
		});
		UTEST_NULL(TEXT("row with -999"), FoundRow);

		return true;
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
