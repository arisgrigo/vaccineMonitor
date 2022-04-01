//
// Created by aris on 03/03/2021.
//

#ifndef VACCINEMONITOR_SKIPLIST_H
#define VACCINEMONITOR_SKIPLIST_H

#include "citizen.h"

typedef struct date{
    int day;
    int month;
    int year;
}date;

typedef struct node{
    unsigned int citizenID;
    struct date* date;
    citizen* citizenCheck;
    struct node* nextNode;
    struct node* downNode;
}node;

typedef struct skipList{
    char* virus;
    node* headNode;

    int maxLevel; //will be 23 (~ log(7000000))

    struct skipList* upperList;
    struct skipList* lowerList;

    struct skipList* nextList;
}skipList;

node* nodeCreate(citizen* citizenInfo, date* date);
skipList* skipListInit(char* listName, citizen* citizenInfo, date* date);
skipList* skipListNew(skipList* givenSkipList, char* listName, citizen* citizenInfo, date* date);
skipList* skipListNewLevel(skipList* skipListLowerLevel, char* listName, citizen* citizenInfo, date* date);
void skipListAdd(skipList* listName, citizen* citizenInfo, date* date);
void skipListPrintAll(skipList* givenSkipList);
int existsInSkipList(skipList* givenSkipList, int givenID);
void deleteSkipList(skipList* givenSkipList);
skipList* getSkipList(skipList* givenSkipList, char* virus);
int skipListExists(skipList* givenSkipList, char* virus);
node* getNode(skipList* givenSkipList, int givenID);
date* setDate(int day, int month, int year);
int isEarlierDate(date* givenDate, date* date);
int isLaterDate(date* givenDate, date* date);
int skipListGetCountry(skipList* givenSkipList, char* country, date* date1, date* date2);
int skipListGetCountryAge(skipList* givenSkipList, char* country, date* date1, date* date2, int lowAge, int highAge);
void skipListDeleteNode(skipList* givenSkipList, int givenID);

#endif //VACCINEMONITOR_SKIPLIST_H
