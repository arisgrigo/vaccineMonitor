#ifndef VACCINEMONITOR_COUNTRY_H
#define VACCINEMONITOR_COUNTRY_H

typedef struct country{
    char* country;
    struct country* next;
}country;

country* addCountry(country* countryList, char* country);
int countryExists(country* countryList, char* country);
void countryDelete(country* countryList);
void countryPrintAll(country* givenCountry);
void countryPrint(country* givenCountry);
country* getCountry(country* countryList, char* givenCountry);

#endif //VACCINEMONITOR_COUNTRY_H
