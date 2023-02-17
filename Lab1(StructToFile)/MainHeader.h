#pragma once
#include <string>
#include <vector>
#include <list>
#include <tuple>
#include <iostream>
using namespace std;
#define _null 9999999999
enum name
{
    Diagnoses_file,
    Plans_file,
    Index_file
};
struct Diagnose
{
    int key;
    size_t FirstPlanLink;
    string Description;
};
//#pragma pack(push, 1)
struct Plan
{
    int key;
    size_t NextPlanLink;
    int PatientId;
    string Description;
};
//#pragma pack(pop)
struct DiagnoseIndex
{
    int DiagnoseKey;
    size_t DiagnoseLink;
    DiagnoseIndex()
    {
        DiagnoseKey = 0;
        DiagnoseLink = 0;
    }
    DiagnoseIndex(int key, size_t link)
    {
        DiagnoseKey = key;
        DiagnoseLink = link;
    }
};