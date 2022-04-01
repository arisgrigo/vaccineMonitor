#include "citizen.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//initializes the list that contains each citizen
citizen* citizenInit(int id, char* fName, char* lName, char* country, int age,struct country* countryList){
    citizen* newCitizen = (citizen*)malloc(sizeof(citizen));

    if(newCitizen != NULL){
        newCitizen->id = id;

        newCitizen->firstName = malloc(strlen(fName) + 1);
        strcpy(newCitizen->firstName, fName);

        newCitizen->lastName = malloc(strlen(lName) + 1);
        strcpy(newCitizen->lastName, lName);

        //country points to the correct country node in the country list, to avoid data duplication
        newCitizen->country = getCountry(countryList,country);

        newCitizen->age = age;

        //the next pointer, points to the citizen that was inserted before the current one
        newCitizen->next = NULL;
    }

    return newCitizen;
}

citizen* addCitizen(citizen* citizenList, int id, char* fName, char* lName, char* country, int age,struct country* countryList){
    if(citizenList != NULL){
    //if citizen already exists in the list, return that citizen
        if(citizenExists(citizenList, id)){
            return citizenList;
        }
    }

    citizen* newCitizen = NULL;
    newCitizen = citizenInit(id, fName, lName, country, age, countryList);

    if(newCitizen != NULL){
        newCitizen->next = citizenList;
    }

    return newCitizen;
}

void citizenDelete(citizen* citizenList){
    if(citizenList->next != NULL){
      citizenDelete(citizenList->next);
    }
    free(citizenList->firstName);
    free(citizenList->lastName);
    free(citizenList);
}

void citizenPrint(citizen* givenCitizen){
    printf("%d, %s %s, %s, age %d\n", givenCitizen->id, givenCitizen->firstName, givenCitizen->lastName, givenCitizen->country->country, givenCitizen->age);
    return;
}

void citizenPrintSpecific(citizen* givenCitizen, int givenID){
    if(givenCitizen->id == givenID){
        printf("%d, %s %s, %s, age %d\n", givenCitizen->id, givenCitizen->firstName, givenCitizen->lastName, givenCitizen->country->country, givenCitizen->age);
        return;
    }else if(givenCitizen->next != NULL){
        citizenPrintSpecific(givenCitizen->next, givenID);
    }
    return;
}

void citizenPrintAll(citizen* givenCitizen){
    if(givenCitizen->next != NULL){
        citizenPrintAll(givenCitizen->next);
    }
    citizenPrint(givenCitizen);
}

//searches the citizen list and returns 1 if the given id is found
int citizenExists(citizen* citizenList, int givenID){
    int check = 0;

    if(citizenList == NULL){
        return 0;
    }

    if(citizenList->id == givenID){
        return 1;
    }else if(citizenList->next != NULL){
        check = citizenExists(citizenList->next, givenID);
    }
    return check;
}

citizen* getCitizen(citizen* citizenList, int givenID){
    while(citizenList->next !=NULL){
        if(citizenList->id == givenID){
            return citizenList;
        }
        citizenList = citizenList->next;
    }
    if(citizenList->id == givenID){
        return citizenList;
    }else{
        printf("no such citizen found: %d\n",givenID);
        return NULL;
    }

}
