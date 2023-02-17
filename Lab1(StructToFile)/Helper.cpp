#include "Helper.h"



size_t Helper::FindDiagnoseLink(int key)
{
    for (int i = 0; i < *diagnoseCount; i++)
    {
        if (indexArr[i].DiagnoseKey == key)
        {
            return indexArr[i].DiagnoseLink;
        }
    }
    return _null;
}


void Helper::AddDiagnose(string desc)
{
    string ans;
    Diagnose* newDiagnose = new Diagnose;
    newDiagnose->key = *diagnoseCount != 0 ? indexArr[*diagnoseCount - 1].DiagnoseKey + 1 : 0;
    //cout << "Enter the name of the Diagnose: ";
    //cin >> newDiagnose->Name;
    //cout << "Enter the adress of the Diagnose: ";
    //cin >> newDiagnose->Address;
    newDiagnose->Description = desc;
    newDiagnose->FirstPlanLink = _null;

    indexArr[(*diagnoseCount)++] = *new DiagnoseIndex(newDiagnose->key, File::insert_m(fileName[Diagnoses_file], newDiagnose));
    return;
    //cout << "Does it have any Plans? (No\\Yes)";
    //tryagain:
    //cin >> ans;
    //if (ans == "No")
    //{
    //    return;
    //}
    ////else if (ans == "Yes")
    ////{

    ////    AddPlan(newDiagnose->key);
    ////    cout << "Do you wanna add another Plan? (No\\Yes)\n";
    ////}
    //else
    //    cout << "I don't understand you, please write again\n";
    //goto tryagain;
}

void Helper::AddPlan(int key, int patientId, string description)
{
    size_t diagnoseLink = FindDiagnoseLink(key);
    if (diagnoseLink != _null)
    {
        Plan* newPlan = new Plan;
        //cout << "Enter Plan number: ";
        //cin >> newPlan->PlanNumber;
        newPlan->PatientId = patientId;
        //cout << "Enter Plan type: ";
        //cin >> newPlan->PlanType;
        newPlan->Description = description;
        newPlan->NextPlanLink = _null;


        Diagnose* myDiagnose = (Diagnose*)File::get_m(fileName[Diagnoses_file], diagnoseLink);
        Plan* lastPlan = new Plan;
        size_t lastPlanLink;
        if (myDiagnose->FirstPlanLink != _null)
        {
            auto lastPlanPair = File::getLastPlan(fileName[Plans_file], myDiagnose->FirstPlanLink, 1);
            lastPlan = get<0>(lastPlanPair);
            lastPlanLink = get<1>(lastPlanPair);
            int planCount = get<2>(lastPlanPair);
            newPlan->key = ++planCount;
            size_t newPlanLink = File::insert_s(fileName[Plans_file], newPlan);
            lastPlan->NextPlanLink = newPlanLink;
            File::update(fileName[Plans_file], lastPlan, lastPlanLink, 0);
        }
        else
        {
            newPlan->key = 0;
            size_t newPlanLink = File::insert_s(fileName[Plans_file], newPlan);
            myDiagnose->FirstPlanLink = newPlanLink;
            File::update(fileName[Diagnoses_file], myDiagnose, diagnoseLink,0);
        }

    }
}

void Helper::UpdateDiagnose(int key, string descr)
{
    Diagnose upDiagnose;
    //cout << "New name: ";
    //cin >> upDiagnose.Name;
    upDiagnose.Description = descr;
    //cout << "New address: ";
    //cin >> upDiagnose.Address;
    upDiagnose.FirstPlanLink = _null;
    size_t diagnoseLink = FindDiagnoseLink(key);
    Diagnose* oldDiagnose = File::get_m(fileName[Diagnoses_file], diagnoseLink);
    upDiagnose.key = oldDiagnose->key;
    if (diagnoseLink != _null)
    {
        File::update(fileName[Diagnoses_file], &upDiagnose, diagnoseLink,1);
    }
}

void Helper::CountSlaves(int key)
{    
    size_t diagnoseLink = FindDiagnoseLink(key);
    if (diagnoseLink != _null)
    {
        Diagnose* iDiagnose = (Diagnose*)File::get_m(fileName[Diagnoses_file], diagnoseLink);

        cout << "Master key: " << (iDiagnose)->key << endl;
        if (iDiagnose->FirstPlanLink != _null)
        {
            Plan* nextPlan;
            size_t link = iDiagnose->FirstPlanLink;
            int count = 0;
            do
            {
                count++;
                nextPlan = File::get_s(fileName[Plans_file], link);
                link = nextPlan->NextPlanLink;
            } while (link != _null);
            cout << "Quantity: " << count << endl;
        }
        else cout << 0 << endl;
    }
}

void Helper::DisplayIDiagnose(int key)
{
    size_t diagnoseLink = FindDiagnoseLink(key);
    if (diagnoseLink != _null)
    {
        Diagnose* iDiagnose = (Diagnose*)File::get_m(fileName[Diagnoses_file], diagnoseLink);

        cout << "Key: " << (iDiagnose)->key << endl;
        cout << "Description: " << (iDiagnose)->Description << endl;
        if (iDiagnose->FirstPlanLink != _null)
        {
            Plan* nextPlan;
            size_t link = iDiagnose->FirstPlanLink;
            do
            {
                nextPlan = File::get_s(fileName[Plans_file], link);
                cout << "\tKey " << nextPlan->key << endl;
                cout << "\tPatient id: " << nextPlan->PatientId << endl;
                cout << "\tDescription: " << nextPlan->Description << endl << endl;
                link = nextPlan->NextPlanLink;
            } while (link != _null);
        }
    }
}

void Helper::UpdatePlan(int key, int subkey, int patientId, string descr)
{
    Plan upPlan;
    //cout << "Plan number: ";
    //cin >> upPlan.PlanNumber;
    upPlan.PatientId = patientId;
    //cout << "Plan type: ";
    //cin >> upPlan.PlanType;
    upPlan.Description = descr;

    size_t diagnoseLink = FindDiagnoseLink(key);
    if (diagnoseLink != _null)
    {
        Diagnose* myDiagnose = File::get_m(fileName[Diagnoses_file], diagnoseLink);
        size_t nextLink = myDiagnose->FirstPlanLink, myLink;
        bool suc = 0;
        Plan* myPlan;
        while (nextLink != _null)
        {
            myPlan = File::get_s(fileName[Plans_file], nextLink);
            myLink = nextLink;
            if (myPlan->key == subkey)
            {
                suc = 1;
                break;
            }
            nextLink = myPlan->NextPlanLink;
        }
        if (suc)
        {
            File::update(fileName[Plans_file], &upPlan, myLink,1);
        }

    }

}

void Helper::DeleteDiagnose(int key)
{
    for (int i = 0; i < *diagnoseCount; i++)
    {
        if (key == indexArr[i].DiagnoseKey)
        {
            for (int j = i; j < *diagnoseCount; j++)
            {
                indexArr[j] = indexArr[j + 1];
            }
            --(*diagnoseCount);
            break;
        }
    }
}

void Helper::DeletePlan(int key, int subkey)
{
    size_t diagnoseLink = FindDiagnoseLink(key);
    if (diagnoseLink != _null)
    {
        Diagnose* myDiagnose = File::get_m(fileName[Diagnoses_file], diagnoseLink);
        size_t nextLink = myDiagnose->FirstPlanLink, myLink, firstLink = nextLink, prevLink;
        bool suc = 0;
        Plan* myPlan;
        while (nextLink != _null)
        {
            myPlan = File::get_s(fileName[Plans_file], nextLink);
            myLink = nextLink;
            if (myPlan->key == subkey)
            {
                suc = 1;
                nextLink = myPlan->NextPlanLink;
                break;
            }
            nextLink = myPlan->NextPlanLink;
        }
        if (suc)
        {
            if (myDiagnose->FirstPlanLink == myLink)
            {
                myDiagnose->FirstPlanLink = nextLink;

                File::update(fileName[Plans_file], myDiagnose, diagnoseLink, 0);
                return;
            }
            while (firstLink != _null)
            {
                myPlan = File::get_s(fileName[Plans_file], firstLink);
                prevLink = firstLink;
                if (myPlan->NextPlanLink == myLink)
                {
                    myPlan->NextPlanLink = nextLink;
                    File::update(fileName[Plans_file], myPlan, prevLink,0);
                    return;
                }
                firstLink = myPlan->NextPlanLink;
            }
        }

    }
}
