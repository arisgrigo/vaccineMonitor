#ifndef VACCINEMONITOR_CITIZEN_H
#define VACCINEMONITOR_CITIZEN_H
#include "country.h"

typedef struct citizen{
    unsigned int id;
    char* firstName;
    char* lastName;
    struct country* country;
    unsigned int age;

    struct citizen* next;
}citizen;

citizen* citizenInit(int id, char* fName, char* lName, char* country, int age,struct country* countryList);
citizen* addCitizen(citizen* citizenList, int id, char* fName, char* lName, char* country, int age, struct country* countryList);

void citizenDelete(citizen* citizenList);
void citizenPrint(citizen* givenCitizen);
void citizenPrintAll(citizen* givenCitizen);
void citizenPrintSpecific(citizen* givenCitizen, int givenID);
int citizenExists(citizen* citizenList, int givenID);
citizen* getCitizen(citizen* citizenList, int givenID);

#endif //VACCINEMONITOR_CITIZEN_H
