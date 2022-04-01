#ifndef VACCINEMONITOR_QUERIES_H
#define VACCINEMONITOR_QUERIES_H
#include "skipList.h"
#include "citizen.h"
#include "country.h"
#include "bloomFilter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void vaccineStatusBloom(bloomFilter* bloomList, int bloomSize);
void vaccineStatus(skipList* vaccinatedSkipList);
void populationStatus(country* countryList, skipList* vaccinatedSkipList, skipList* not_vaccinatedSkipList);
void popStatusByAge(country* countryList, skipList* vaccinatedSkipList, skipList* not_vaccinatedSkipList);
void listNonVaccinatedPersons(skipList* not_vaccinatedSkipList);
void insertRecord(skipList** vaccinatedSkipList, skipList** not_vaccinatedSkipList, citizen** citizenList, country** countryList, bloomFilter** bloomList, int bloomSize);
void vaccinateNow(skipList** vaccinatedSkipList, skipList** not_vaccinatedSkipList, citizen** citizenList, country** countryList, bloomFilter** bloomList, int bloomSize);

#endif //VACCINEMONITOR_QUERIES_H
