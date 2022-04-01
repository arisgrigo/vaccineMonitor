#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#include "bloomFilter.h"
#include "citizen.h"
#include "skipList.h"
#include "queries.h"

int main( int argc, char *argv[]) {
    citizen *citizenList = NULL;        //used to store the data of each separate (different id) citizen
    country *countryList = NULL;                //used to store the data of each separate country
    citizen *citizen0 = NULL;
    country *country0 = NULL;
    skipList *vaccinatedSkipList = NULL;        //list that contains every SkipList which contains vaccinated citizens
    skipList *not_vaccinatedSkipList = NULL;    //list that contains every SkipList which contains non vaccinated citizens
    skipList *skipListCheck = NULL;             //used in various functions, to not mess the other two SkipLists
    bloomFilter *bloomList = NULL;              //list that contains a bloom filter for every virus
    srand(time(NULL));              //to produce random numbers used to determine if a Skip List will grow one level

    char *fileToRead = NULL;
    int bloomSize=0;                              //size of the bloom filters (in bytes)
    int c;

    while ((c = getopt(argc, argv, "b:c:")) != -1)
        switch (c) {
            case 'b':
                if(!atol(optarg)){
                    printf("-b must be followed by integer!\n");
                    return 1;
                }
                bloomSize = atol(optarg);
                break;
            case 'c':
                fileToRead = optarg;
                break;
            case '?':
                if(optopt == 'c')
                    printf("Option -%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    printf("Unknown option.\n");
                else
                    printf("Unknown option character. \n");
                return 1;
        default:
            abort();
        }

    if (argc == 5) {

        FILE *fp;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        char *token;        //used to temporarily store each argument in a line
        char *file = fileToRead;
        int citizenID;
        char *firstName;
        char *lastName;
        char *country;
        int age;
        char *virusName;
        char *vaccinated;       //can be YES or NO

        fp = fopen(file, "r");
        if (fp == NULL) {
            exit(EXIT_FAILURE);
        }

        printf("Parsing %s...\n", fileToRead);
        while ((read = getline(&line, &len, fp)) != -1) {
            int isValid = 1;        //used to determine if a record contains errors
            date *datePntr = NULL;
            int day = 0;
            int month = 0;
            int year = 0;

            token = strtok(line, " \n");
            if(token==NULL){
                continue;
            }
            if(!atol(token)){
                continue;
            }

            unsigned char* bloomStr = (unsigned char*)token;      //string used in hash functions for the bloom filters
            citizenID = atoi(token);

            token = strtok(NULL, " \n");
            if(token==NULL){
                continue;
            }
            firstName = token;

            token = strtok(NULL, " \n");
            if(token==NULL){
                continue;
            }
            lastName = token;

            token = strtok(NULL, " \n");
            if(token==NULL){
                continue;
            }
            country = token;

            token = strtok(NULL, " \n");
            if(token==NULL){
                continue;
            }
            if(!atol(token)){
                continue;
            }
            age = atoi(token);

            token = strtok(NULL, " \n");
            if(token==NULL){
                continue;
            }
            virusName = token;

            token = strtok(NULL, " \n");
            if(token==NULL){
                continue;
            }
            vaccinated = token;

            //if record contains YES, a date must follow
            //format of the date is strictly dd-mm-yyyy or else a record is considered invalid
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
                    printf("Invalid date form it record with id %d\n", citizenID);
                }
                year = atoi(token);
            }

            //check if a record is valid by comparing it to previous records
            //(if previous records with same id exist)
            //if inconsistencies exist, the record is considered invalid
            if (citizenExists(citizenList, citizenID)) {
                citizen *tempCitizen = getCitizen(citizenList, citizenID);

                if (strcmp(tempCitizen->country->country, country) == 0) {
                    if (strcmp(tempCitizen->firstName, firstName) == 0) {
                        if (strcmp(tempCitizen->lastName, lastName) == 0) {
                            if (tempCitizen->age == age) {
                                //if everything lines up, check to see if the record refers to a different virus
                                if (skipListExists(vaccinatedSkipList, virusName)) {
                                    skipListCheck = getSkipList(vaccinatedSkipList, virusName);
                                    if (existsInSkipList(skipListCheck, citizenID)) {
                                        isValid = 0;
                                    }
                                }
                                if (skipListExists(not_vaccinatedSkipList, virusName)) {
                                    skipListCheck = getSkipList(not_vaccinatedSkipList, virusName);
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
                    //setDate allocates memory for the date
                    //the memory is deallocated when the skip list that contains it, is freed
                    datePntr = setDate(day, month, year);

                    //if it's the first record, initialize every struct that is needed
                    if (citizenList == NULL) {
                        countryList = addCountry(countryList, country);
                        citizenList = citizenInit(citizenID, firstName, lastName, country, age, countryList);
                        vaccinatedSkipList = skipListInit(virusName, citizenList, datePntr);
                        bloomList = bloomInit(bloomSize, virusName);
                        unsigned long hash[16];
                        for (int i = 0; i < 16; i++) {
                            hash[i] = hash_i(bloomStr, i);
                            //since the size of the bloom filter is given in bytes, multiply it by 8 to get the bits
                            setBloomBit(bloomList, hash[i] % (bloomSize * 8));
                        }
                    } else {
                        countryList = addCountry(countryList, country);
                        citizenList = addCitizen(citizenList, citizenID, firstName, lastName, country, age, countryList);
                        //citizen0 is used to pass the required citizen info to a skip list
                        if (citizenExists(citizenList, citizenID)) {
                            citizen0 = getCitizen(citizenList, citizenID);
                        } else citizen0 = citizenList;
                        //if skip list already exists for the virus, use it, or else make a new one for it
                        if (skipListExists(vaccinatedSkipList, virusName)) {
                            skipListAdd(getSkipList(vaccinatedSkipList, virusName), citizen0, datePntr);
                        } else {
                            vaccinatedSkipList = skipListNew(vaccinatedSkipList, virusName, citizen0, datePntr);
                        }
                        if (bloomExists(bloomList, virusName)) {
                            unsigned long hash[16];
                            for (int i = 0; i < 16; i++) {
                                hash[i] = hash_i(bloomStr, i);
                                setBloomBit(getBloom(bloomList, virusName), hash[i] % (bloomSize * 8));
                            }
                        } else {
                            bloomList = addBloom(bloomList, bloomSize, virusName);
                            unsigned long hash[16];
                            for (int i = 0; i < 16; i++) {
                                hash[i] = hash_i(bloomStr, i);
                                setBloomBit(bloomList, hash[i] % (bloomSize * 8));
                            }
                        }
                    }
                }
                //same process for NO, but without the date and the bloom filter
                if (strcmp(vaccinated, "NO") == 0) {
                    if (citizenList == NULL) {
                        countryList = addCountry(countryList, country);
                        citizenList = citizenInit(citizenID, firstName, lastName, country, age, countryList);
                        not_vaccinatedSkipList = skipListInit(virusName, citizenList, NULL);
                    } else {
                        countryList = addCountry(countryList, country);
                        citizenList = addCitizen(citizenList, citizenID, firstName, lastName, country, age, countryList);
                        if (citizenExists(citizenList, citizenID)) {
                            citizen0 = getCitizen(citizenList, citizenID);
                        } else citizen0 = citizenList;
                        if (skipListExists(not_vaccinatedSkipList, virusName)) {
                            skipListAdd(getSkipList(not_vaccinatedSkipList, virusName), citizen0, NULL);
                        } else {
                            not_vaccinatedSkipList = skipListNew(not_vaccinatedSkipList, virusName, citizen0, NULL);
                        }
                    }
                }
            } else {
                printf("ERROR IN RECORD WITH ID: %d\n", citizenID);
            }
        }
        fclose(fp);
        if (line) {
            free(line);
        }
        printf("Parsed %s with success!\n", fileToRead);
    } else {
        printf("Invalid number of arguments!\n");
        return 0;
    }
    //parsing of the given file has stopped

    //here begins the queries
    printf("Now awaiting user's input...\n");

    char input[100];    //input is used to temporarily store the line that the user gives

    //while, is always true, so the program only stops if user types in "/exit"
    while(1){
        char *argument = NULL;

        //gets input from user, and chops it down using strtok, to fulfill each query
        fgets(input,100,stdin);

        argument = strtok(input, " \n");

        if (argument == NULL) {
            continue;
        }

        if (strcmp(argument, "/vaccineStatusBloom") == 0) {
            vaccineStatusBloom(bloomList, bloomSize);
            continue;
        }

        if (strcmp(argument, "/vaccineStatus") == 0) {
            vaccineStatus(vaccinatedSkipList);
            continue;
        }

        if (strcmp(argument, "/populationStatus") == 0) {
            populationStatus(countryList,vaccinatedSkipList,not_vaccinatedSkipList);
            continue;
        }

        if (strcmp(argument, "/popStatusByAge") == 0) {
            popStatusByAge(countryList,vaccinatedSkipList,not_vaccinatedSkipList);
            continue;
        }

        if (strcmp(argument, "/insertCitizenRecord") == 0) {
            insertRecord(&vaccinatedSkipList, &not_vaccinatedSkipList, &citizenList, &countryList, &bloomList, bloomSize);
            continue;
        }

        if(strcmp(argument, "/vaccinateNow") == 0){
            vaccinateNow(&vaccinatedSkipList, &not_vaccinatedSkipList, &citizenList, &countryList, &bloomList, bloomSize);
            continue;
        }

        if (strcmp(argument, "/list-nonVaccinated-Persons") == 0){
            listNonVaccinatedPersons(not_vaccinatedSkipList);
            continue;
        }

        if (strcmp(argument, "/exit") == 0){
            break;
        }

        printf("Unknown command.\n");

    }

    if (citizenList != NULL)
        citizenDelete(citizenList);
    if (countryList != NULL)
        countryDelete(countryList);
    if (vaccinatedSkipList != NULL)
        deleteSkipList(vaccinatedSkipList);
    if (not_vaccinatedSkipList != NULL)
        deleteSkipList(not_vaccinatedSkipList);
    if (bloomList != NULL)
        deleteBloom(bloomList);

    return 0;
}
