#include "country.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//adds a country to the country list
//if a country already exists in the country list, does nothing
country* addCountry(country* countryList,char* givenCountry){
    if(countryList != NULL){
        if(countryExists(countryList, givenCountry)){
            return countryList;
        }
    }

    //if given country does not exist yet, then allocates memory for it
    country* newCountry = (country*)malloc(sizeof(country));

    if(newCountry != NULL){
        newCountry->country = malloc(strlen(givenCountry) + 1);
        strcpy(newCountry->country, givenCountry);

        //if country list is empty, then the given country becomes the list's head
        if(countryList == NULL){
            newCountry->next = NULL;
            countryList = newCountry;
            return countryList;
        }

        //the next pointer, points to the country that was inserted before the current one
        newCountry->next = countryList;
    }

    return newCountry;
}

int countryExists(country* countryList, char* givenCountry){
    int check = 0;
    if(strcmp(countryList->country, givenCountry) == 0){
        return 1;
    }else if(countryList->next != NULL){
        check = countryExists(countryList->next, givenCountry);
    }
    return check;
}

void countryDelete(country* countryList){
    if(countryList->next != NULL){
        countryDelete(countryList->next);
    }
    free(countryList->country);
    free(countryList);
}

void countryPrint(country* givenCountry){
    printf("%s \n", givenCountry->country);
    return;
}

void countryPrintAll(country* givenCountry){
    if(givenCountry->next != NULL){
        countryPrintAll(givenCountry->next);
    }
    countryPrint(givenCountry);
}

country* getCountry(country* countryList, char* givenCountry){
    while(countryList->next !=NULL){
        if(strcmp(countryList->country, givenCountry) == 0){
            return countryList;
        }
        countryList = countryList->next;
    }
    if(strcmp(countryList->country, givenCountry) == 0){
        return countryList;
    }else{
        printf("no such country found: %s\n",givenCountry);
        return NULL;
    }

}