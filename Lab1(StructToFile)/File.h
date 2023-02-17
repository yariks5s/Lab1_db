#pragma once
#include "MainHeader.h"
class File
{
public:
	static size_t insert_m(const char*, Diagnose*); // add new diagnose to the file
	static size_t insert_s(const char*, Plan*); // add new plan to file
	static Diagnose* get_m(const char*, size_t); // get diagnose with a link
	static Plan* get_s(const char*, size_t); // get plan with a link
	static tuple<Plan*, size_t, int> getLastPlan(const char*, size_t, int); // get all information about the last plan
	static bool update(const char*, Diagnose*, size_t, bool); // update diagnose
	static bool update(const char*, Plan*, size_t, bool); // update plan
	static bool update(const char*, DiagnoseIndex*, int); // update diagnose index list
	static size_t del_m(const char*, size_t);
	static pair<int, DiagnoseIndex*>  load_i(const char*); // load diagnose index list
	static void clear(const char*); // clear file
};

