#include<cstdio>

#include <iostream>
using namespace std;

#include "Framework\UnitTest.h"
using namespace Hush;
using namespace Hush::UnitTest;

#include "..\Engine\StorageEngine\PageFile.h"
using namespace HushDB;

TESTCLASS(PageFileTests)
{
public:
    virtual void TestInitialize() override
    {
        remove("test.df");
    }

    TESTMETHOD(TestUpdateFile)
    {
        PageFile file(STR("test.df"));        
        file.Open();

        DataPage page(0);
        Int16 row1 = 1;
        Int32 row2 = 2;        

        RowId rowId1 = page.InsertRowValue(row1);
        RowId rowId2 = page.InsertRowValue(row2); 
                
        file.AppendPage(&page);
        file.Flush();

        DataPage* pageFromFile = file.ReadPageAs<DataPage>(0);
        Assert::AreEqual(page, *pageFromFile);       
        
        Int64 row3 = 3;
        RowId rowId3 = page.InsertRowValue(row3);

        file.WritePage(&page);
        file.Flush();

        pageFromFile = file.ReadPageAs<DataPage>(0);
        Assert::AreEqual(page, *pageFromFile);
        
        file.Flush();
        file.Close();
    }

    TESTMETHOD(TestAppendFile)
    {
        PageFile file(STR("test.df"));
        file.Open();

        DataPage page(0);
        Int16 row1 = 1;
        Int32 row2 = 2;

        RowId rowId1 = page.InsertRowValue(row1);
        RowId rowId2 = page.InsertRowValue(row2);

        file.WritePage(&page);
        file.Flush();
        file.Close();

        file.Open();

        DataPage* pageFromFile = file.ReadPageAs<DataPage>(0);
        Assert::AreEqual(page, *pageFromFile);

        file.Flush();
        file.Close();
    }
};