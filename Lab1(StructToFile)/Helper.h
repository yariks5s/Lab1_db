#pragma once
#include "File.h"
class Helper
{
private:
	int* diagnoseCount;
	DiagnoseIndex* indexArr;
	size_t FindDiagnoseLink(int);
	const char* fileName[3] = {"DB\\Diagnoses.fl","DB\\Plans.fl", "DB\\DiagnoseIndex.fl"};
public:
	Helper()
	{

	}
	Helper(int* DiagnoseCount, DiagnoseIndex* indexArr)
	{
		this->diagnoseCount = DiagnoseCount;
		this->indexArr = indexArr;
	}
	void AddDiagnose(string);
	void AddPlan(int,int,string);
	void UpdateDiagnose(int,string);
	void DisplayIDiagnose(int);
	void UpdatePlan(int, int, int, string);
	void DeleteDiagnose(int);
	void DeletePlan(int, int);
	void CountSlaves(int);
};

