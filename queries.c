#include "queries.h"

void vaccineStatusBloom(bloomFilter* bloomList, int bloomSize) {
    char *argument;

    int vaccinated = 1;         //if vaccinated becomes 0, then the citizen has not been vaccinated, otherwise, he/she could be
    argument = strtok(NULL, " ");
    if(argument==NULL){
        printf("Query requires input!\n");
        return;
    }
    unsigned char *id = argument;
    if(!(atol(id))){
        printf("an id must be given!\n");
        return;
    }

    argument = strtok(NULL, "\n ");
    char *virus = argument;

    argument = strtok(NULL, "\n ");
    if(argument != NULL){
        printf("Incorrect user input!\n");
        return;
    }

    if(virus == NULL){
        printf("a virus must be given!\n");
        return;
    }
    bloomFilter *bloomCheck = getBloom(bloomList, virus);

    unsigned long hash[16];
    for (int i = 0; i < 16; i++) {
        hash[i] = hash_i(id, i);

        //if even one bit is 0, then the citizen has definitely not been vaccinated
        if (getBloomBit(bloomCheck, hash[i] % (bloomSize * 8)) == 0) {
            printf("NOT VACCINATED\n");
            vaccinated = 0;
            break;
        }
    }
    if(vaccinated) {
        printf("MAYBE\n");
    }
}

void vaccineStatus(skipList* vaccinatedSkipList){
    char* argument = strtok(NULL, " ");
    if(argument==NULL){
        printf("Query requires input!\n");
        return;
    }
    if(!(atol(argument))){
        printf("an id must be given!\n");
        return;
    }
    int id = atoi(argument);

    argument = strtok(NULL, "\n ");
    //if specific virus is not given, then check every skip list
    if (argument == NULL) {
        skipList *skipListCheck = vaccinatedSkipList;
        while (skipListCheck != NULL) {
            if (existsInSkipList(skipListCheck, id)) {
                node *tmpNode = getNode(skipListCheck, id);
                printf("%s YES %02d-%02d-%d\n", skipListCheck->virus, tmpNode->date->day, tmpNode->date->month, tmpNode->date->year);
            } else printf("%s NO\n", skipListCheck->virus);
            skipListCheck = skipListCheck->nextList;
        }
    } else {
        char *virus = argument;

        argument = strtok(NULL, "\n ");
        if(argument != NULL){
            printf("Incorrect user input!\n");
            return;
        }

        skipList* skipListCheck = getSkipList(vaccinatedSkipList, virus);

        if (existsInSkipList(skipListCheck, id)) {
            node *tmpNode = getNode(skipListCheck, id);
            printf("VACCINATED ON %02d-%02d-%d\n", tmpNode->date->day, tmpNode->date->month, tmpNode->date->year);
        } else printf("NOT VACCINATED\n");
    }
}

void populationStatus(country* countryList, skipList* vaccinatedSkipList, skipList* not_vaccinatedSkipList){
    char* argument = strtok(NULL, " ");
    if(argument==NULL){
        printf("Query requires input!\n");
        return;
    }
    char *country = argument;

    //if country given actually exists, then print the stats for given country
    if (countryExists(countryList, country)) {
        argument = strtok(NULL, " \n");
        if(argument==NULL){
            printf("Query requires virus to be given!\n");
            return;
        }
        char *virus = argument;

        int isValid = 1;
        int day, month, year;

        argument = strtok(NULL, "-\n");
        if(argument == NULL){
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 31 || atoi(argument) < 1) {
            isValid = 0;
        }
        day = atoi(argument);

        argument = strtok(NULL, "-\n");
        if(argument == NULL){
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 12 || atoi(argument) < 1) {
            isValid = 0;
        }
        month = atoi(argument);

        argument = strtok(NULL, " \n");
        if(argument == NULL){
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 2021 || atoi(argument) < 1900) {
            isValid = 0;
        }
        year = atoi(argument);
        date *date1 = setDate(day, month, year);

        argument = strtok(NULL, "-\n");

        if(argument == NULL){
            printf("Date range must be given!\n");
            free(date1);
            return;
        }

        if (atoi(argument) > 31 || atoi(argument) < 1) {
            isValid = 0;
        }

        day = atoi(argument);

        argument = strtok(NULL, "-\n");
        if(argument == NULL){
            free(date1);
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 12 || atoi(argument) < 1) {
            isValid = 0;
        }
        month = atoi(argument);

        argument = strtok(NULL, " \n");
        if(argument == NULL){
            free(date1);
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 2021 || atoi(argument) < 1900) {
            isValid = 0;
        }
        year = atoi(argument);

        date *date2 = setDate(day, month, year);

        argument = strtok(NULL, " \n");
        if(argument != NULL){
            free(date1);
            free(date2);
            printf("Incorrect user input!\n");
            return;
        }

        if(!isValid){
            printf("Invalid date format\n");
            free(date1);
            free(date2);
            return;
        }

        int vacPopRange = 0;            //number of vaccinated citizens in given date range for given virus
        int vacPop = 0;                 //number of total vaccinated citizens for given virus
        int notVacPop = 0;              //number of total non vaccinated citizens for given virus
        float percent;

        skipList* skipListCheck = getSkipList(vaccinatedSkipList, virus);
        vacPopRange = skipListGetCountry(skipListCheck, country, date1, date2);
        vacPop = skipListGetCountry(skipListCheck, country, NULL, NULL);
        skipListCheck = getSkipList(not_vaccinatedSkipList, virus);
        notVacPop = skipListGetCountry(skipListCheck, country, NULL, NULL);
        percent = ((float) vacPopRange / (float) (vacPop + notVacPop)) * 100;
        if (vacPop + notVacPop == 0) {
            printf("No one has been tested for %s in %s\n", virus, country);
        } else printf("%s %s %d %f %%\n", country, virus, vacPopRange, percent);
        free(date1);
        free(date2);
    } else {
        char *virus = country;
        if(virus==NULL){
            printf("Query requires virus to be given!\n");
            return;
        }
        int isValid = 1;
        int day, month, year;

        argument = strtok(NULL, "-\n");
        if(argument == NULL){
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 31 || atoi(argument) < 1) {
            isValid = 0;
        }
        day = atoi(argument);

        argument = strtok(NULL, "-\n");
        if(argument == NULL){
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 12 || atoi(argument) < 1) {
            isValid = 0;
        }
        month = atoi(argument);

        argument = strtok(NULL, " \n");
        if(argument == NULL){
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 2021 || atoi(argument) < 1900) {
            isValid = 0;
        }
        year = atoi(argument);
        date *date1 = setDate(day, month, year);

        argument = strtok(NULL, "-\n");

        if(argument == NULL){
            printf("Date range must be given!\n");
            free(date1);
            return;
        }

        if (atoi(argument) > 31 || atoi(argument) < 1) {
            isValid = 0;
        }

        day = atoi(argument);

        argument = strtok(NULL, "-\n");
        if(argument == NULL){
            free(date1);
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 12 || atoi(argument) < 1) {
            isValid = 0;
        }
        month = atoi(argument);

        argument = strtok(NULL, " \n");
        if(argument == NULL){
            free(date1);
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 2021 || atoi(argument) < 1900) {
            isValid = 0;
        }
        year = atoi(argument);

        date *date2 = setDate(day, month, year);

        argument = strtok(NULL, " \n");
        if(argument != NULL){
            free(date1);
            free(date2);
            printf("Incorrect user input!\n");
            return;
        }

        if(!isValid){
            printf("Invalid date format!\n");
            free(date1);
            free(date2);
            return;
        }

        //same as above, but for every country inside the country list
        int vacPopRange = 0;
        int vacPop = 0;
        int notVacPop = 0;
        float percent;

        struct country *countryListHead = countryList;
        while (countryListHead != NULL) {
            country = countryListHead->country;
            skipList* skipListCheck = getSkipList(vaccinatedSkipList, virus);
            vacPopRange = skipListGetCountry(skipListCheck, country, date1, date2);
            vacPop = skipListGetCountry(skipListCheck, country, NULL, NULL);
            skipListCheck = getSkipList(not_vaccinatedSkipList, virus);
            notVacPop = skipListGetCountry(skipListCheck, country, NULL, NULL);
            percent = (float) vacPopRange / (float) (vacPop + notVacPop) * 100;
            if (vacPop + notVacPop == 0) {
                printf("No one has been tested for %s in %s\n", virus, country);
            } else printf("%s %s %d %f %%\n", country, virus, vacPopRange, percent);

            countryListHead = countryListHead->next;
        }
        free(date1);
        free(date2);
    }
}

void popStatusByAge(country* countryList, skipList* vaccinatedSkipList, skipList* not_vaccinatedSkipList) {
    char *argument = strtok(NULL, " ");
    if(argument==NULL){
        printf("Query requires input!\n");
        return;
    }
    char *country = argument;

    //same as with populationStatus but using a slightly different function to get the specific age range
    if (countryExists(countryList, country)) {
        argument = strtok(NULL, " \n");
        if(argument==NULL){
            printf("Query requires virus to be given!\n");
            return;
        }

        char *virus = argument;
        int isValid = 1;
        int day, month, year;

        argument = strtok(NULL, "-\n");
        if(argument == NULL){
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 31 || atoi(argument) < 1) {
            isValid = 0;
        }
        day = atoi(argument);

        argument = strtok(NULL, "-\n");
        if(argument == NULL){
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 12 || atoi(argument) < 1) {
            isValid = 0;
        }
        month = atoi(argument);

        argument = strtok(NULL, " \n");
        if(argument == NULL){
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 2021 || atoi(argument) < 1900) {
            isValid = 0;
        }
        year = atoi(argument);
        date *date1 = setDate(day, month, year);

        argument = strtok(NULL, "-\n");

        if(argument == NULL){
            printf("Date range must be given!\n");
            free(date1);
            return;
        }

        if (atoi(argument) > 31 || atoi(argument) < 1) {
            isValid = 0;
        }

        day = atoi(argument);

        argument = strtok(NULL, "-\n");
        if(argument == NULL){
            free(date1);
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 12 || atoi(argument) < 1) {
            isValid = 0;
        }
        month = atoi(argument);

        argument = strtok(NULL, " \n");
        if(argument == NULL){
            free(date1);
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 2021 || atoi(argument) < 1900) {
            isValid = 0;
        }
        year = atoi(argument);

        date *date2 = setDate(day, month, year);

        argument = strtok(NULL, " \n");
        if(argument != NULL){
            free(date1);
            free(date2);
            printf("Incorrect user input!\n");
            return;
        }

        if(!isValid){
            printf("Invalid date format!\n");
            free(date1);
            free(date2);
            return;
        }

        int vacPopRange = 0;
        int vacPop = 0;
        int notVacPop = 0;
        float percent;
        int lowAge = 0;
        int highAge = 20;
        skipList *skipListCheck = NULL;

        printf("%s\n", country);

        for(int i = 0; i < 4; i++){
            if(i == 1){
                lowAge = 20;
                highAge = 40;
            }
            if(i == 2){
                lowAge = 40;
                highAge = 60;
            }
            if(i == 3){
                lowAge = 60;
                highAge = 140;
            }
            skipListCheck = getSkipList(vaccinatedSkipList, virus);
            vacPopRange = skipListGetCountryAge(skipListCheck, country, date1, date2, lowAge, highAge);
            vacPop = skipListGetCountryAge(skipListCheck, country, NULL, NULL, lowAge, highAge);
            skipListCheck = getSkipList(not_vaccinatedSkipList, virus);
            notVacPop = skipListGetCountryAge(skipListCheck, country, NULL, NULL, lowAge, highAge);
            percent = (float) vacPopRange / (float) (vacPop + notVacPop) * 100;
            if (vacPop + notVacPop == 0) {
                printf("No one has been tested for %s in %s, for age range:%d-%d\n", virus, country, lowAge, highAge);
            }else printf("%d-%d %d %f %%\n", lowAge, highAge, vacPopRange, percent);
        }

        free(date1);
        free(date2);
    } else {
        char *virus = country;
        if(virus==NULL){
            printf("Query requires virus to be given!\n");
            return;
        }
        int isValid = 1;
        int day, month, year;

        argument = strtok(NULL, "-\n");
        if(argument == NULL){
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 31 || atoi(argument) < 1) {
            isValid = 0;
        }
        day = atoi(argument);

        argument = strtok(NULL, "-\n");
        if(argument == NULL){
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 12 || atoi(argument) < 1) {
            isValid = 0;
        }
        month = atoi(argument);

        argument = strtok(NULL, " \n");
        if(argument == NULL){
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 2021 || atoi(argument) < 1900) {
            isValid = 0;
        }
        year = atoi(argument);
        date *date1 = setDate(day, month, year);

        argument = strtok(NULL, "-\n");

        if(argument == NULL){
            printf("Date range must be given!\n");
            free(date1);
            return;
        }

        if (atoi(argument) > 31 || atoi(argument) < 1) {
            isValid = 0;
        }

        day = atoi(argument);

        argument = strtok(NULL, "-\n");
        if(argument == NULL){
            free(date1);
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 12 || atoi(argument) < 1) {
            isValid = 0;
        }
        month = atoi(argument);

        argument = strtok(NULL, " \n");
        if(argument == NULL){
            free(date1);
            printf("Date range must be given!\n");
            return;
        }
        if (atoi(argument) > 2021 || atoi(argument) < 1900) {
            isValid = 0;
        }
        year = atoi(argument);

        date *date2 = setDate(day, month, year);

        argument = strtok(NULL, " \n");
        if(argument != NULL){
            free(date1);
            free(date2);
            printf("Incorrect user input!\n");
            return;
        }

        if(!isValid){
            printf("Invalid date format!\n");
            free(date1);
            free(date2);
            return;
        }

        int vacPopRange = 0;
        int vacPop = 0;
        int notVacPop = 0;
        float percent;

        struct country *countryListHead = countryList;
        while (countryListHead != NULL) {
            country = countryListHead->country;

            int lowAge = 0;
            int highAge = 20;
            skipList *skipListCheck = NULL;

            printf("%s\n", country);

            for(int i = 0; i < 4; i++){
                if(i == 1){
                    lowAge = 20;
                    highAge = 40;
                }
                if(i == 2){
                    lowAge = 40;
                    highAge = 60;
                }
                if(i == 3){
                    lowAge = 60;
                    highAge = 140;
                }
                skipListCheck = getSkipList(vaccinatedSkipList, virus);
                vacPopRange = skipListGetCountryAge(skipListCheck, country, date1, date2, lowAge, highAge);
                vacPop = skipListGetCountryAge(skipListCheck, country, NULL, NULL, lowAge, highAge);
                skipListCheck = getSkipList(not_vaccinatedSkipList, virus);
                notVacPop = skipListGetCountryAge(skipListCheck, country, NULL, NULL, lowAge, highAge);
                percent = (float) vacPopRange / (float) (vacPop + notVacPop) * 100;
                if (vacPop + notVacPop == 0) {
                    printf("No one has been tested for %s in %s, for age range:%d-%d\n", virus, country, lowAge, highAge);
                }else if(i == 3){
                    printf("%d+ %d %f %%\n", lowAge, vacPopRange, percent);
                }else
                    printf("%d-%d %d %f %%\n", lowAge, highAge, vacPopRange, percent);
            }
            printf("\n");
            countryListHead = countryListHead->next;
        }
        free(date1);
        free(date2);
    }
}

void listNonVaccinatedPersons(skipList* not_vaccinatedSkipList){
    skipList* skipListCheck = not_vaccinatedSkipList;

    char* token = strtok(NULL, "\n ");
    char* virusName = token;

    if(virusName == NULL){
        printf("A specific virus must be given!\n");
        return;
    }

    char* argument = strtok(NULL, "\n ");
    if(argument!=NULL){
        printf("Incorrect user input!\n");
        return;
    }

    skipListCheck = getSkipList(not_vaccinatedSkipList, virusName);
    skipListPrintAll(skipListCheck);
}

//almost exactly same as the block of code used for the initial parsing of the file
void insertRecord(skipList** vaccinatedSkipList, skipList** not_vaccinatedSkipList, citizen** citizenList, country** countryList, bloomFilter** bloomList, int bloomSize) {
    int citizenID;
    char *firstName;
    char *lastName;
    char *country;
    int age;
    char *virusName;
    char *vaccinated;
    unsigned char* bloomStr;
    skipList* skipListCheck;
    struct citizen* citizen0;
    struct country* country0;

    char* token;

    char *affirm = "YES";
    int isValid = 1;

    date *datePntr = NULL;
    int day = 0;
    int month = 0;
    int year = 0;

    token = strtok(NULL, "  \n");
    if(token==NULL){
        printf("Query requires id to be given!\n");
        return;
    }
    if(!atol(token)){
        printf("id must be an integer!\n");
        return;
    }
    bloomStr = (unsigned char*)token;
    citizenID = atoi(token);

    token = strtok(NULL, " \n");
    if(token==NULL){
        printf("Query requires first name to be given!\n");
        return;
    }
    firstName = token;

    token = strtok(NULL, " \n");
    if(token==NULL){
        printf("Query requires last name to be given!\n");
        return;
    }
    lastName = token;

    token = strtok(NULL, " \n");
    if(token==NULL){
        printf("Query requires country to be given!\n");
        return;
    }
    country = token;

    token = strtok(NULL, " \n");
    if(token==NULL){
        printf("Query requires age to be given!\n");
        return;
    }
    if(!atol(token)){
        printf("Age must be an integer!\n");
        return;
    }
    age = atoi(token);

    token = strtok(NULL, " \n");
    if(token==NULL){
        printf("Query requires virus to be given!\n");
        return;
    }
    virusName = token;

    token = strtok(NULL, " \n");
    if(token==NULL){
        printf("Query requires YES or NO input to be given after virus!\n");
        return;
    }
    vaccinated = token;

    if (strcmp(vaccinated, "YES") == 0) {
        token = strtok(NULL, "-");
        if (atoi(token) > 31 || atoi(token) < 1) {
            isValid = 0;
            printf("Invalid date form it record with id %d\n", citizenID);
        }
        day = atoi(token);

        token = strtok(NULL, "-");
        if (atoi(token) > 12 || atoi(token) < 1) {
            isValid = 0;
            printf("Invalid date form it record with id %d\n", citizenID);
        }
        month = atoi(token);

        token = strtok(NULL, "-");
        if (atoi(token) > 2021 || atoi(token) < 1900) {
            isValid = 0;
        }
        year = atoi(token);
    }

    if (citizenExists(*citizenList, citizenID)) {
        citizen *tempCitizen = getCitizen(*citizenList, citizenID);

        if (strcmp(tempCitizen->country->country, country) == 0) {
            if (strcmp(tempCitizen->firstName, firstName) == 0) {
                if (strcmp(tempCitizen->lastName, lastName) == 0) {
                    if (tempCitizen->age == age) {
                        if (skipListExists(*vaccinatedSkipList, virusName)) {
                            skipListCheck = getSkipList(*vaccinatedSkipList, virusName);
                            if (existsInSkipList(skipListCheck, citizenID)) {
                                node *tmpNode = getNode(skipListCheck, citizenID);
                                printf("ERROR: CITIZEN %d ALREADY VACCINATED ON %02d-%02d-%d\n",tmpNode->citizenID, tmpNode->date->day, tmpNode->date->month, tmpNode->date->year);
                                isValid = 0;
                            }
                        }
                        if (skipListExists(*not_vaccinatedSkipList, virusName)) {
                            skipListCheck = getSkipList(*not_vaccinatedSkipList, virusName);
                            if (existsInSkipList(skipListCheck, citizenID)) {
                                isValid = 0;
                            }
                        }
                    } else isValid = 0;
                } else isValid = 0;
            } else isValid = 0;
        } else isValid = 0;
    }
    if (isValid) {
        if (strcmp(vaccinated, "YES") == 0) {
            datePntr = setDate(day, month, year);
            if (citizenList == NULL) {
                *countryList = addCountry(*countryList, country);
                *citizenList = citizenInit(citizenID, firstName, lastName, country, age, *countryList);
                *vaccinatedSkipList = skipListInit(virusName, *citizenList, datePntr);
                *bloomList = bloomInit(bloomSize, virusName);
                unsigned long hash[16];
                for (int i = 0; i < 16; i++) {
                    hash[i] = hash_i(bloomStr, i);
                    setBloomBit(*bloomList, hash[i] % (bloomSize * 8));
                }
            } else {
                *countryList = addCountry(*countryList, country);
                *citizenList = addCitizen(*citizenList, citizenID, firstName, lastName, country, age, *countryList);
                if (citizenExists(*citizenList, citizenID)) {
                    citizen0 = getCitizen(*citizenList, citizenID);
                } else citizen0 = *citizenList;
                if (skipListExists(*vaccinatedSkipList, virusName)) {
                    skipListAdd(getSkipList(*vaccinatedSkipList, virusName), citizen0, datePntr);
                } else {
                    *vaccinatedSkipList = skipListNew(*vaccinatedSkipList, virusName, citizen0, datePntr);
                }
                if (bloomExists(*bloomList, virusName)) {
                    unsigned long hash[16];
                    for (int i = 0; i < 16; i++) {
                        hash[i] = hash_i(bloomStr, i);
                        setBloomBit(getBloom(*bloomList, virusName), hash[i] % (bloomSize * 8));
                    }
                } else {
                    *bloomList = addBloom(*bloomList, bloomSize, virusName);
                    unsigned long hash[16];
                    for (int i = 0; i < 16; i++) {
                        hash[i] = hash_i(bloomStr, i);
                        setBloomBit(*bloomList, hash[i] % (bloomSize * 8));
                    }
                }
            }
        }
        if (strcmp(vaccinated, "NO") == 0) {
            if (citizenList == NULL) {
                *countryList = addCountry(*countryList, country);
                *citizenList = citizenInit(citizenID, firstName, lastName, country, age, *countryList);
                *not_vaccinatedSkipList = skipListInit(virusName, *citizenList, NULL);
            } else {
                *countryList = addCountry(*countryList, country);
                *citizenList = addCitizen(*citizenList, citizenID, firstName, lastName, country, age, *countryList);
                if (citizenExists(*citizenList, citizenID)) {
                    citizen0 = getCitizen(*citizenList, citizenID);
                } else citizen0 = *citizenList;
                if (skipListExists(*not_vaccinatedSkipList, virusName)) {
                    skipListAdd(getSkipList(*not_vaccinatedSkipList, virusName), citizen0, NULL);
                } else {
                    *not_vaccinatedSkipList = skipListNew(*not_vaccinatedSkipList, virusName, citizen0, NULL);
                }
            }
        }
    } else {
        printf("ERROR IN RECORD WITH ID: %d\n", citizenID);
    }
}

//almost exactly same as insertCitizenRecord, with the following changes:
//1) if the given citizen record is found in the not_vaccinatedSkipList it is removed from there
//2) instead of taking the date from the record, it takes it from today's date
void vaccinateNow(skipList** vaccinatedSkipList, skipList** not_vaccinatedSkipList, citizen** citizenList, country** countryList, bloomFilter** bloomList, int bloomSize){
    int citizenID;
    char *firstName;
    char *lastName;
    char *country;
    int age;
    char *virusName;
    char *vaccinated;
    unsigned char* bloomStr;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char* token;

    skipList* skipListCheck;
    struct citizen* citizen0;
    struct country* country0;
    int isValid = 1;            //used to determine if a record contains errors
    date *datePntr = NULL;
    int day = 0;
    int month = 0;
    int year = 0;

    token = strtok(NULL, " \n");
    if(token==NULL){
        printf("Query requires id to be given!\n");
        return;
    }
    if(!atol(token)){
        printf("id must be an integer!\n");
        return;
    }
    bloomStr = (unsigned char*)token;
    citizenID = atoi(token);

    token = strtok(NULL, " \n");
    if(token==NULL){
        printf("Query requires first name to be given!\n");
        return;
    }
    firstName = token;

    token = strtok(NULL, " \n");
    if(token==NULL){
        printf("Query requires last name to be given!\n");
        return;
    }
    lastName = token;

    token = strtok(NULL, " \n");
    if(token==NULL){
        printf("Query requires country to be given!\n");
        return;
    }
    country = token;

    token = strtok(NULL, " \n");
    if(token==NULL){
        printf("Query requires age to be given!\n");
        return;
    }
    if(!atol(token)){
        printf("age must be an integer!\n");
        return;
    }
    age = atoi(token);

    token = strtok(NULL, "\n");
    if(token==NULL){
        printf("Query requires virus to be given!\n");
        return;
    }
    virusName = token;

    if (citizenExists(*citizenList, citizenID)) {
        citizen *tempCitizen = getCitizen(*citizenList, citizenID);

        if (strcmp(tempCitizen->country->country, country) == 0) {
            if (strcmp(tempCitizen->firstName, firstName) == 0) {
                if (strcmp(tempCitizen->lastName, lastName) == 0) {
                    if (tempCitizen->age == age) {
                        if (skipListExists(*vaccinatedSkipList, virusName)) {
                            skipListCheck = getSkipList(*vaccinatedSkipList, virusName);
                            if (existsInSkipList(skipListCheck, citizenID)) {
                                node *tmpNode = getNode(skipListCheck, citizenID);
                                printf("ERROR: CITIZEN %d ALREADY VACCINATED ON %02d-%02d-%d\n",tmpNode->citizenID, tmpNode->date->day, tmpNode->date->month, tmpNode->date->year);
                                isValid = 0;
                            }
                        }
                    } else isValid = 0;
                } else isValid = 0;
            } else isValid = 0;
        } else isValid = 0;
    }
    if (isValid) {
        datePntr = setDate(tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
        if (citizenList == NULL) {
            *countryList = addCountry(*countryList, country);
            *citizenList = citizenInit(citizenID, firstName, lastName, country, age, *countryList);
            *vaccinatedSkipList = skipListInit(virusName, *citizenList, datePntr);
            *bloomList = bloomInit(bloomSize, virusName);
            unsigned long hash[16];
            for (int i = 0; i < 16; i++) {
                hash[i] = hash_i(bloomStr, i);
                setBloomBit(*bloomList, hash[i] % (bloomSize * 8));
            }
        } else {
            *countryList = addCountry(*countryList, country);
            *citizenList = addCitizen(*citizenList, citizenID, firstName, lastName, country, age, *countryList);
            if (citizenExists(*citizenList, citizenID)) {
                citizen0 = getCitizen(*citizenList, citizenID);
            } else citizen0 = *citizenList;
            if (skipListExists(*vaccinatedSkipList, virusName)) {
                skipListAdd(getSkipList(*vaccinatedSkipList, virusName), citizen0, datePntr);
            } else {
                *vaccinatedSkipList = skipListNew(*vaccinatedSkipList, virusName, citizen0, datePntr);
            }
            if (bloomExists(*bloomList, virusName)) {
                unsigned long hash[16];
                for (int i = 0; i < 16; i++) {
                    hash[i] = hash_i(bloomStr, i);
                    setBloomBit(getBloom(*bloomList, virusName), hash[i] % (bloomSize * 8));
                }
            } else {
                *bloomList = addBloom(*bloomList, bloomSize, virusName);
                unsigned long hash[16];
                for (int i = 0; i < 16; i++) {
                    hash[i] = hash_i(bloomStr, i);
                    setBloomBit(*bloomList, hash[i] % (bloomSize * 8));
                }
            }
        }
        //get the not_vaccinatedSkipList for the virus, and check if the given citizen exists there
        //if yes, delete the node, since citizen is now vaccinated for that virus
        skipListCheck = getSkipList(*not_vaccinatedSkipList, virusName);
        if(existsInSkipList(skipListCheck,citizenID)){
            skipListDeleteNode(skipListCheck, citizenID);
        }
    } else {
        printf("ERROR IN RECORD WITH ID: %d\n", citizenID);
    }
}
