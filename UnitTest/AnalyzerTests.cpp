#include <iostream>
using namespace std;

#include "Framework\UnitTest.h"
using namespace Hush;
using namespace Hush::UnitTest;

#include "..\Engine\QueryProcessor\Analyzer.h"
using namespace HushDB;

TESTCLASS(AnalyzerTests)
{
    TESTMETHOD(TestBasic)
    {
        MemoryTableDef tableDef;
        tableDef.Name = T("t1");
        tableDef.Schema->AddColumn(T("c1"), SqlType::Int);
        tableDef.Schema->AddColumn(T("c2"), SqlType::String);

        Catalog::Ptr catalog = make_shared<Catalog>();
        catalog->AddTable(tableDef);

        String query(T("select *, c1, c2 from t1"));
        SqlToken::List tokens = Tokenizer::Parse(query);
        auto current = tokens.begin();

        SelectStmt::Ptr selectStmt = SelectStmt::Parse(current, tokens.end());

        Analyzer analyzer(catalog.get());
        LogicalPlan::Ptr plan = analyzer.Analyze(selectStmt);
    }
};