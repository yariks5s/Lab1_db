#include "Helper.h"

int diagnoseCount;
DiagnoseIndex* indexArr;
const char* fileName[3] = { "DB\\Diagnoses.fl","DB\\Plans.fl", "DB\\DiagnoseIndex.fl" };

void sort()
{
    for (int startIndex = 0; startIndex < diagnoseCount - 1; ++startIndex)
    {
        // В переменной smallestIndex хранится индекс наименьшего значения, которое мы нашли в этой итерации.
        // Начинаем с того, что наименьший элемент в этой итерации - это первый элемент (индекс 0)
        int smallestIndex = startIndex;

        // Затем ищем элемент поменьше в остальной части массива
        for (int currentIndex = startIndex + 1; currentIndex < diagnoseCount; ++currentIndex)
        {
            // Если мы нашли элемент, который меньше нашего наименьшего элемента,
            if (indexArr[currentIndex].DiagnoseKey < indexArr[smallestIndex].DiagnoseKey)
                // то запоминаем его
                smallestIndex = currentIndex;
        }

        // smallestIndex теперь наименьший элемент. 
        // Меняем местами наше начальное наименьшее число с тем, которое мы обнаружили
        std::swap(indexArr[startIndex], indexArr[smallestIndex]);
    }
}
void start(bool _swith)
{
    indexArr = new DiagnoseIndex[100]{};
    if (_swith)
    {
        File::clear(fileName[Diagnoses_file]);
        File::clear(fileName[Plans_file]);
        File::clear(fileName[Index_file]);
        diagnoseCount = 0;
    }
    else
    {
        auto myPair = File::load_i(fileName[Index_file]);
        diagnoseCount = myPair.first;
        indexArr = myPair.second;
    }
}

vector<string> slim(string ans)
{
    string word = "";
    vector<string> *vec = new vector<string>;
    int size = ans.size();
    for (int i = 0; i < size; i++)
    {
        if (ans[i] != ' ')
        {
            word += ans[i];
        }
        else
        {
            vec->push_back(word);
            word = "";
        }
    }
    vec->push_back(word);
    return *vec;
}
int main()
{
    size_t aaa = sizeof(Plan);
    size_t aaaa = sizeof(int) + sizeof(size_t) + sizeof(int) + sizeof(char[100]);
    // Витягнути інфу про розтошування готелей
    start(1);
    Helper myHelper(&diagnoseCount, indexArr);
    // Дії за вибором
    int key, subkey;
    bool suc = 1;
    string ans;
    vector<string> vec;
    do
    {
        getline(cin, ans);
        vec = slim(ans);
        if (vec[0] == "del-m") // delete diagnose
        {
            myHelper.DeleteDiagnose(stoi(vec[1])); // key
        }
        else if (vec[0] == "del-s") // delete plan
        {
            myHelper.DeletePlan(stoi(vec[1]), stoi(vec[2]));// key, subkey
        }
        else if (vec[0] == "insert-m") // add diagnose
        {
            myHelper.AddDiagnose(vec[1]); // name
        }
        else if (vec[0] == "insert-s") // add plan
        {
            myHelper.AddPlan(stoi(vec[1]),stoi(vec[2]),vec[3]); // key, plannum, plantype
        }
        else if (vec[0] == "update-m") // update diagnose
        {
            myHelper.UpdateDiagnose(stoi(vec[1]), vec[2]); // key, diagnosedescr
        }
        else if (vec[0] == "update-s") // update plan
        {
            myHelper.UpdatePlan(stoi(vec[1]), stoi(vec[2]), stoi(vec[3]), vec[4]);// key, patientId, num, descr
        }
        else if (vec[0] == "ut-m") // display i-diagnose info
        {
            myHelper.DisplayIDiagnose(stoi(vec[1]));// key
        }
        else if (vec[0] == "ut-all") // display all info
        {
            for (int i = 0; i < diagnoseCount; i++)
            {
                myHelper.DisplayIDiagnose(indexArr[i].DiagnoseKey);
            }
        }
        else if (vec[0] == "count-m") // count the quantity of Diagnoses
        {
            cout << "Quantity: " << diagnoseCount << endl;
        }
        else if (vec[0] == "count-s") // count the quantity of plans for specific diagnose
        {
            myHelper.CountSlaves(stoi(vec[1]));
        }
        if (!suc)
            cout << "Something went wrong try again please";
    } while (ans != "1");

    sort();
    File::update(fileName[Index_file], indexArr, diagnoseCount);
    return 1;
}
//Test::
//insert-m Diagnose0 Diagnose0_descr
//insert-m Diagnose1 Diagnose1_descr
//insert-m Diagnose2 Diagnose2_descr
//insert-m Diagnose3 Diagnose3_descr
//insert-m Diagnose4 Diagnose4_descr
//insert-s 0 1 One
//insert-s 1 2 Two
//insert-s 1 3 Three
//insert-s 2 4 One
//insert-s 2 5 Two
//insert-s 2 6 Three
//insert-s 3 7 Two
//insert-s 3 8 One
//insert-s 4 9 Three
//ut-all
//del-m 3
//del-s 2 2
//ut-all
//insert-m Diagnose6 Diagnose6_descr
//insert-s 5 11 One
//ut-all
//update-m 5 Diagnose6_new Diagnose6_new_descr
//update-s 5 0 15 Two
//ut-all
