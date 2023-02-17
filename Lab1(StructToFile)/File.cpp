#include "File.h"
//#include "fileapi.h"

void File::clear(const char* filename)
{
    FILE* fp;
    fopen_s(&fp, filename, "w");
    fclose(fp);
}
bool File::update(const char* filename, Diagnose* upDiagnose, size_t diagnoseLink, bool _switch)
{
    // відкриваємо файл та готуємось до зчитування
    FILE* fileStream;
    size_t size = sizeof(Diagnose);
    fopen_s(&fileStream, filename, "r+b");
    if (!fileStream)
    {
        printf("Error occured while opening file \n");
        return 0;
    }

    // дістаю стару інфу, щоб не змінювати ключ
    bool suc = fseek(fileStream, (long)diagnoseLink, SEEK_SET) == -1 ? 0 : 1;
    Diagnose* oldDiagnose = new Diagnose;
    fread_s(oldDiagnose, size, size, 1, fileStream);
    //upDiagnose->key = oldDiagnose->key;
    if (_switch)
        upDiagnose->FirstPlanLink = oldDiagnose->FirstPlanLink;
    // оновлюю інформацію
    suc = fseek(fileStream, (long)diagnoseLink, SEEK_SET) == -1 ? 0 : 1;
    int nn;
    if (suc == 1)
        nn = fwrite(upDiagnose, size, 1, fileStream);

    fclose(fileStream);
    return suc == 1 ? 1 : 0;
}

bool File::update(const char* filename, Plan* upPlan, size_t PlanLink, bool _switch)
{
    // відкриваємо файл та готуємось до зчитування
    FILE* fileStream;
    fopen_s(&fileStream, filename, "r+b");
    if (!fileStream)
    {
        printf("Error occured while opening file \n");
        return 0;
    }

    // дістаю стару інфу, щоб не змінювати ключ
    fseek(fileStream, (long)PlanLink, SEEK_SET);
    Plan* oldPlan = new Plan;
    fread_s(oldPlan, sizeof(Plan), sizeof(Plan), 1, fileStream);
    upPlan->key = oldPlan->key;
    if (_switch)
        upPlan->NextPlanLink = oldPlan->NextPlanLink;
    // оновлюю інформацію
    bool suc = fseek(fileStream, (long)PlanLink, SEEK_SET) == -1 ? 0 : 1;
    if (suc == 1)
        fwrite(upPlan, sizeof(Plan), 1, fileStream);

    fclose(fileStream);
    return suc == 1 ? 1 : 0;
}

size_t File::insert_s(const char* filename, Plan* newPlan)
{
    FILE* fileStream;
    fopen_s(&fileStream, filename, "a"); //открываем файл для продолжения бинарной записи
    if (!fileStream)    // если не удалось открыть файл
    {
        printf("Error occured while opening file \n");
        return -1;
    }
    // посимвольно записываем в файл структуру
    //size_t newPlanLink = ftell(fileStream);
    int aa = fwrite(newPlan, sizeof(Plan), 1, fileStream);
    size_t bb = ftell(fileStream);
    size_t newPlanLink = ftell(fileStream) - sizeof(Plan);
    fclose(fileStream);
    return newPlanLink;
}
//size_t insert_s_new(const char* filename, Plan* newPlan)
//{
//    HANDLE SFile = CreateFileA(filename, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
//
//    if (SFile != 0)
//    {
//
//        WriteFile(SFile, newPlan, sizeof(Plan), res, nullptr);
//    }
//
//
//    CloseHandle(SFile);
//}
Diagnose* File::get_m(const char* filename, size_t diagnoseLink)
{
    // відкриваємо файл та готуємось до зчитування
    FILE* fileStream;
    fopen_s(&fileStream, filename, "rb");
    if (!fileStream)
    {
        printf("Error occured while opening file \n");
        return nullptr;
    }

    // зчитуємо та зберігаємо інформацію
    Diagnose* curDiagnose = new Diagnose;
    size_t size = sizeof(Diagnose); 
    bool suc = fseek(fileStream, (long)diagnoseLink, SEEK_SET) == -1 ? 0 : 1;
    if (suc == 1) // ставимо вказівник куди потрібно
        fread_s(curDiagnose, size, size, 1, fileStream);
    

    fclose(fileStream);
    return curDiagnose;
}

tuple<Plan*, size_t, int> File::getLastPlan(const char* filename, size_t planLink, int planNum)
{// відкриваємо файл та готуємось до зчитування
    FILE* fileStream;
    fopen_s(&fileStream, filename, "rb");
    if (!fileStream)
    {
        printf("Error occured while opening file \n");
        return make_tuple(nullptr, _null, -1);
    }

    // зчитуємо та зберігаємо інформацію
    Plan* curPlan = new Plan;
    size_t size = sizeof(Plan);
    tuple<Plan*, size_t, int> curPair;
    bool suc = fseek(fileStream, (long)planLink, SEEK_SET) == -1 ? 0 : 1;
    if (suc == 1) // ставимо вказівник куди потрібно
    {
        fread_s(curPlan, size, size, 1, fileStream);
        curPair = make_tuple(curPlan, planLink, planNum);
        if (curPlan->NextPlanLink != _null)
        {
            curPair = getLastPlan(filename, curPlan->NextPlanLink, ++planNum);
        }
    }

    fclose(fileStream);
    return curPair;
}

Plan* File::get_s(const char* filename, size_t planLink)
{
    FILE* fileStream;
    fopen_s(&fileStream, filename, "rb");
    if (!fileStream)
    {
        printf("Error occured while opening file \n");
        return nullptr;
    }

    // зчитуємо та зберігаємо інформацію
    Plan* curPlan = new Plan;
    size_t size = sizeof(Plan);
    bool suc = fseek(fileStream, (long)planLink, SEEK_SET) == -1 ? 0 : 1;
    if (suc == 1) // ставимо вказівник куди потрібно
    {
        fread_s(curPlan, size, size, 1, fileStream);
    }

    fclose(fileStream);
    return curPlan;
}


size_t File::insert_m(const char* filename, Diagnose* myDiagnose)
{
    FILE* fileStream;
    size_t size = sizeof(Diagnose); // количество записываемых байтов
    fopen_s(&fileStream, filename, "a"); //открываем файл для продолжения бинарной записи
    if (!fileStream)    // если не удалось открыть файл
    {
        printf("Error occured while opening file \n");
        return -1;
    }
    // посимвольно записываем в файл структуру
    fwrite(myDiagnose, size, 1, fileStream);
    size_t diagnoseLink = ftell(fileStream) - sizeof(Diagnose);
    fclose(fileStream);
    return diagnoseLink;
}
bool File::update(const char* filename, DiagnoseIndex* indexArr, int diagnoseCount)
{
    FILE* fileStream;
    fopen_s(&fileStream, filename, "wb"); //открываем файл для продолжения бинарной записи
    if (!fileStream)    // если не удалось открыть файл
    {
        printf("Error occured while opening file \n");
        return 0;
    }
    // посимвольно записываем в файл структуру
    fwrite(&diagnoseCount, sizeof(int), 1, fileStream); // перші вісім байт - число готелей
    fwrite(indexArr, sizeof(DiagnoseIndex), diagnoseCount, fileStream); // інші - готелі
    fclose(fileStream);
    return 1;
}
size_t File::del_m(const char* filename, size_t diagnoseCount)
{
    //FILE* fileStream;
    //fopen_s(&fileStream, filename, "wb"); //открываем файл для продолжения бинарной записи
    //if (!fileStream)    // если не удалось открыть файл
    //{
    //    printf("Error occured while opening file \n");
    //    return 0;
    //}
    //// посимвольно записываем в файл структуру
    //fwrite(&DiagnoseCount, sizeof(int), 1, fileStream); // перші вісім байт - число готелей
    //fwrite(indexArr, sizeof(DiagnoseIndex), DiagnoseCount, fileStream); // інші - готелі
    //fclose(fileStream);
    return 1;

}
pair<int,DiagnoseIndex*> File::load_i(const char* filename)
{
    // відкриваємо файл та готуємось до зчитування
    FILE* fileStream;
    fopen_s(&fileStream, filename, "rb");
    if (!fileStream)
    {
        printf("Error occured while opening file \n");
        return make_pair(0,nullptr);
    }

    // зчитуємо та зберігаємо інформацію
    int* diagnoseCount = new int;
    fread_s(diagnoseCount, sizeof(int), sizeof(int), 1, fileStream);
    DiagnoseIndex* indexArr = new DiagnoseIndex[*diagnoseCount];
    for (int i = 0; !feof(fileStream); i++)
    {
        fread_s(&indexArr[i], sizeof(DiagnoseIndex)*(*diagnoseCount), sizeof(DiagnoseIndex), 1, fileStream);
    }
    fclose(fileStream);
    return make_pair(*diagnoseCount, indexArr);
}