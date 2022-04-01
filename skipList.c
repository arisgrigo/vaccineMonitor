#include "skipList.h"
#include "citizen.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//allocates memory for a node inside a skip list
node* nodeCreate(citizen* citizenInfo, date* givenDate){
    node* newNode = (node*)malloc(sizeof(node));

    if(newNode != NULL){
        //a date is given only to the nodes that belong in a skip list with vaccinated citizens
        if(givenDate != NULL){
            newNode->date = givenDate;
        }else{
            newNode->date = NULL;
        }
        newNode->citizenID = citizenInfo->id;
        newNode->citizenCheck = citizenInfo;        //citizenCheck contains the data of a citizen, which are stored in the citizen list
        newNode->nextNode = NULL;
        newNode->downNode = NULL;
    }

    return newNode;
}

//initializes a skip list
skipList* skipListInit(char* listName, citizen* citizenInfo, date* date){
    skipList* newSkipList = (skipList*)malloc(sizeof(skipList));

    if(newSkipList != NULL){
        newSkipList->virus = malloc(strlen(listName) + 1);
        strcpy(newSkipList->virus, listName);

        newSkipList->headNode = nodeCreate(citizenInfo, date);
        newSkipList->maxLevel = 23;

        newSkipList->upperList = NULL;
        newSkipList->lowerList = NULL;
        newSkipList->nextList = NULL;
    }

    return newSkipList;
}

skipList* skipListNew(skipList* givenSkipList, char* listName, citizen* citizenInfo, date* date){
    skipList* newSkipList = skipListInit(listName, citizenInfo, date);
    if(newSkipList != NULL){
        newSkipList->nextList = givenSkipList;
    }

    return newSkipList;
}

void skipListAdd(skipList* givenSkipList, citizen* citizenInfo, date* date) {
    skipList* skipListCheck = givenSkipList;

    //first, go to upmost list in the skip list, to start searching there
    while(skipListCheck->upperList != NULL){
        skipListCheck = skipListCheck->upperList;
    }

    node* tmpNode = skipListCheck->headNode;
    node* prevNode = skipListCheck->headNode;
    //if first node of an upper list is bigger than the given id, go lower, until that's not the case
    while(tmpNode->citizenID > citizenInfo->id && skipListCheck != givenSkipList){
        skipListCheck = skipListCheck->lowerList;
        tmpNode = skipListCheck->headNode;
        prevNode = tmpNode;
    }

    if(tmpNode->citizenID > citizenInfo->id && skipListCheck == givenSkipList){
        node *newHead = nodeCreate(citizenInfo, date);
        newHead->nextNode = givenSkipList->headNode;
        givenSkipList->headNode = newHead;
        return;
    }

    node* prevPrevNode = prevNode;
    unsigned int id = citizenInfo->id;
    while(tmpNode != NULL){
        //if the id is bigger than the given id, go a level down(if it exists), and search there
        //downNode is a pointer that points to the same node (same id), one level down
        if(tmpNode->citizenID > id){
            if(skipListCheck == givenSkipList){
                node *newNode = nodeCreate(citizenInfo, date);
                newNode->nextNode = tmpNode;
                prevNode->nextNode = newNode;
                int r = rand();
                if(r % 2 == 0 && givenSkipList->upperList == NULL){          //coin toss
                    //first check to see if max level of skip list has already been reached
                    skipList* tempSkipList = givenSkipList;
                    int count = 0;
                    while(tempSkipList->lowerList != NULL){
                        tempSkipList = tempSkipList->lowerList;
                        if(tempSkipList)
                            count++;
                    }
                    //if not, then add a new level to the skip list
                    if(count < givenSkipList->maxLevel){
                        skipListNewLevel(givenSkipList, givenSkipList->virus, citizenInfo, date);
                        node* tempNode = getNode(givenSkipList->upperList, citizenInfo->id);
                        tempNode->downNode = newNode;
                    }
                }else if(r % 2 == 0){
                    skipListAdd(givenSkipList->upperList, citizenInfo, date);
                    node* tempNode = getNode(givenSkipList->upperList, citizenInfo->id);
                    tempNode->downNode = newNode;
                }
                return;
            }else skipListCheck = skipListCheck->lowerList;
            tmpNode = prevNode->downNode;
            prevNode = prevPrevNode->downNode;
            continue;
        }else{
            //if you reach the end of a level, go a level down and continue
            if(tmpNode->nextNode == NULL && skipListCheck != givenSkipList){
                skipListCheck = skipListCheck->lowerList;
                prevNode = prevNode->downNode;
                tmpNode = tmpNode->downNode;
                continue;
            }else if(tmpNode->nextNode == NULL && skipListCheck == givenSkipList){
                node *newNode = nodeCreate(citizenInfo, date);
                tmpNode->nextNode = newNode;
                int r = rand();
                if(r % 2 == 0 && givenSkipList->upperList == NULL){          //coin toss
                    //first check to see if max level of skip list has already been reached
                    skipList* tempSkipList = givenSkipList;
                    int count = 0;
                    while(tempSkipList->lowerList != NULL){
                        tempSkipList = tempSkipList->lowerList;
                        if(tempSkipList)
                            count++;
                    }
                    //if not, then add a new level to the skip list
                    if(count < givenSkipList->maxLevel){
                        skipListNewLevel(givenSkipList, givenSkipList->virus, citizenInfo, date);
                        node* tempNode = getNode(givenSkipList->upperList, citizenInfo->id);
                        tempNode->downNode = newNode;
                    }
                }else if(r % 2 == 0){
                    skipListAdd(givenSkipList->upperList, citizenInfo, date);
                    node* tempNode = getNode(givenSkipList->upperList, citizenInfo->id);
                    tempNode->downNode = newNode;
                }
                return;
            }else{
                prevPrevNode = prevNode;
                prevNode = tmpNode;
                tmpNode = tmpNode->nextNode;
            }
        }
    }
    return;
}

void skipListPrintAll(skipList* givenSkipList){
    if(givenSkipList == NULL){
        printf("this list does not exist yet\n");
        return;
    }else{
        node* tmpNode = givenSkipList->headNode;
        if(tmpNode != NULL){
            while(tmpNode->nextNode != NULL){
                printf("%d %s %s %s %d\n",tmpNode->citizenID, tmpNode->citizenCheck->firstName, tmpNode->citizenCheck->lastName, tmpNode->citizenCheck->country->country, tmpNode->citizenCheck->age);
                tmpNode = tmpNode->nextNode;
            }
            printf("%d %s %s %s %d\n",tmpNode->citizenID, tmpNode->citizenCheck->firstName, tmpNode->citizenCheck->lastName, tmpNode->citizenCheck->country->country, tmpNode->citizenCheck->age);
        }else printf("There are none not-vaccinated citizens for %s\n",givenSkipList->virus);
    }


}

//go through a given skip list and return 1 if citizen with the given id is found
int existsInSkipList(skipList* givenSkipList, int givenID){
    if(givenSkipList == NULL){
        printf("this list does not exist yet!\n");
        return 0;
    }
    skipList* skipListCheck = givenSkipList;

    //first, go to upmost list in the skip list, to start searching there
    while(skipListCheck->upperList != NULL){
        skipListCheck = skipListCheck->upperList;
    }

    node* tmpNode = skipListCheck->headNode;
    node* prevNode = skipListCheck->headNode;
    //if first node of an upper list is bigger than the given id, go lower, until that's not the case
    while(tmpNode->citizenID > givenID && skipListCheck->lowerList != NULL){
        skipListCheck = skipListCheck->lowerList;
        tmpNode= skipListCheck->headNode;
        prevNode = tmpNode;

        //if at every level, the head node has a bigger id than the one given, then the list does not contain it
        if(tmpNode->citizenID > givenID && skipListCheck->lowerList == NULL){
            return 0;
        }
    }

    while(tmpNode != NULL){
        if(tmpNode->citizenID == givenID){
            return 1;
        }

        //if the id is bigger than the given id, go a level down(if it exists), and search there
        //downNode is a pointer that points to the same node (same id), one level down
        if(tmpNode->citizenID > givenID){
            if(skipListCheck->lowerList == NULL){
                return 0;
            }else skipListCheck = skipListCheck->lowerList;
            tmpNode = prevNode->downNode;
        }else{
            //if you reach the end of a level, go a level down and continue
            if(tmpNode->nextNode == NULL && skipListCheck->lowerList != NULL){
                skipListCheck = skipListCheck->lowerList;
                tmpNode = tmpNode->downNode;
            }else{
                prevNode = tmpNode;
                tmpNode = tmpNode->nextNode;
            }
        }
    }
    return 0;
}

void deleteSkipList(skipList* givenSkipList){
    if(givenSkipList->nextList != NULL){
        deleteSkipList(givenSkipList->nextList);
    }

    if(givenSkipList->upperList != NULL){
        deleteSkipList(givenSkipList->upperList);
    }
    node* tmpNode = givenSkipList->headNode;
    node* myNode = NULL;
    free(givenSkipList->virus);
    while(tmpNode !=NULL){
        myNode = tmpNode->nextNode;
        //we free space from dates only in the lower level, so we don't free the same space twice
        if(givenSkipList->lowerList == NULL){
            if(tmpNode->date != NULL){
                free(tmpNode->date);
            }
        }
        free(tmpNode);
        tmpNode = myNode;
    }
    free(givenSkipList);
}

skipList* getSkipList(skipList* givenSkipList, char* virus){
    if(givenSkipList == NULL){
        return NULL;
    }

    while(givenSkipList->nextList != NULL){
        if(strcmp(givenSkipList->virus, virus) == 0){
            return givenSkipList;
        }
        givenSkipList = givenSkipList->nextList;
    }
    if(strcmp(givenSkipList->virus, virus) == 0){
        return givenSkipList;
    }else{
        return NULL;
    }
}

//go through a given list of skip lists, and return 1 if skip list for a certain virus has been created
int skipListExists(skipList* givenSkipList, char* virus){
    int check = 0;
    if(givenSkipList == NULL){
        return 0;
    }

    if(strcmp(givenSkipList->virus, virus) == 0){
        return 1;
    }else if(givenSkipList->nextList != NULL){
        check = skipListExists(givenSkipList->nextList, virus);
    }
    return check;
}

//creates an upper level for the given skipList
skipList* skipListNewLevel(skipList* skipListLowerLevel, char* listName, citizen* citizenInfo, date* date){
    skipList* newSkipList = skipListInit(listName, citizenInfo, date);
    if(newSkipList != NULL){
        newSkipList->lowerList = skipListLowerLevel;
        node* tmpNode = getNode(newSkipList, citizenInfo->id);
        tmpNode->downNode = getNode(skipListLowerLevel, citizenInfo->id);
        skipListLowerLevel->upperList = newSkipList;
    }

    return newSkipList;
}

node* getNode(skipList* givenSkipList, int givenID){
    if(givenSkipList == NULL){
        printf("this list does not exist yet\n");
        return 0;
    }
    node* tmpNode = givenSkipList->headNode;

    while(tmpNode !=NULL){
        if(tmpNode->citizenID == givenID){
            return tmpNode;
        }
        tmpNode = tmpNode->nextNode;
    }

    return NULL;
}

date* setDate(int day, int month, int year){
    date* givenDate = (date*)malloc(sizeof(date));
    if(givenDate != NULL){
        givenDate->day = day;
        givenDate->month = month;
        givenDate->year = year;
    }
    return givenDate;
}

int isEarlierDate(date* givenDate, date* date){
    int isEarlier = 0;
    if(givenDate->year <= date->year){
        if(givenDate->year == date->year){
            if(givenDate->month <= date->month){
                if(givenDate->month == date->month){
                    if(givenDate->day <= date->day){
                        return 1;
                    }else return 0;
                }else isEarlier = 1;
            }else return 0;
        }else isEarlier = 1;
    }
    return isEarlier;
}

int isLaterDate(date* givenDate, date* date){
    int isLater = 0;
    if(givenDate->year >= date->year){
        if(givenDate->year == date->year){
            if(givenDate->month >= date->month){
                if(givenDate->month == date->month){
                    if(givenDate->day >= date->day){
                        return 1;
                    }else return 0;
                }else isLater = 1;
            }else return 0;
        }else isLater = 1;
    }
    return isLater;
}

int skipListGetCountry(skipList* givenSkipList, char* country, date* date1, date* date2){
    if(givenSkipList == NULL){
        printf("this list does not exist yet\n");
        return 0;
    }else{
        int vacPop = 0;
        node* tmpNode = givenSkipList->headNode;
        while(tmpNode->nextNode != NULL){
            if(strcmp(tmpNode->citizenCheck->country->country, country) == 0 ){
                if(date1 != NULL && date2 != NULL){
                    if(isLaterDate(tmpNode->date, date1) && isEarlierDate(tmpNode->date, date2)){
                        vacPop++;
                    }
                }else vacPop++;
            }
            tmpNode = tmpNode->nextNode;
        }
        if(strcmp(tmpNode->citizenCheck->country->country, country) == 0 ){
            if(date1 != NULL && date2 != NULL){
                if(isLaterDate(tmpNode->date, date1) && isEarlierDate(tmpNode->date, date2)){
                    vacPop++;
                }
            }else vacPop++;
        }
        return vacPop;
    }

}

int skipListGetCountryAge(skipList* givenSkipList, char* country, date* date1, date* date2, int lowAge, int highAge){
    if(givenSkipList == NULL){
        printf("this list does not exist yet\n");
        return 0;
    }else{
        int vacPop = 0;
        node* tmpNode = givenSkipList->headNode;
        while(tmpNode->nextNode != NULL){
            if(strcmp(tmpNode->citizenCheck->country->country, country) == 0 ){
                if(date1 != NULL && date2 != NULL){
                    if(isLaterDate(tmpNode->date, date1) && isEarlierDate(tmpNode->date, date2)){
                        if(tmpNode->citizenCheck->age >= lowAge && tmpNode->citizenCheck->age <= highAge){
                            vacPop++;
                        }
                    }
                }else if(tmpNode->citizenCheck->age >= lowAge && tmpNode->citizenCheck->age <= highAge){
                    vacPop++;
                }
            }
            tmpNode = tmpNode->nextNode;
        }
        if(strcmp(tmpNode->citizenCheck->country->country, country) == 0 ){
            if(date1 != NULL && date2 != NULL){
                if(isLaterDate(tmpNode->date, date1) && isEarlierDate(tmpNode->date, date2)){
                    if(tmpNode->citizenCheck->age >= lowAge && tmpNode->citizenCheck->age <= highAge){
                        vacPop++;
                    }
                }
            }else if(tmpNode->citizenCheck->age >= lowAge && tmpNode->citizenCheck->age <= highAge){
                vacPop++;
            }
        }
        return vacPop;
    }

}

//deletes a specific entry from the given skip list
void skipListDeleteNode(skipList* givenSkipList, int givenID){
    if(givenSkipList == NULL){
        printf("this list does not exist yet\n");
        return;
    }

    skipList* skipListCheck = givenSkipList;
    //get to the top list
    while(skipListCheck->upperList != NULL){
        skipListCheck = skipListCheck->upperList;
    }

    node* tmpNode = skipListCheck->headNode;
    node* prevNode = skipListCheck->headNode;
    //if first element of the list is bigger than the one we search, go down one level, and repeat
    while(tmpNode->citizenID > givenID && skipListCheck->lowerList != NULL){
        skipListCheck = skipListCheck->lowerList;
        tmpNode= skipListCheck->headNode;
        prevNode = tmpNode;

        //if in every list, the first element has bigger id than the one we search, then the element does not exist
        if(tmpNode->citizenID > givenID && skipListCheck->lowerList == NULL){
            return;
        }
    }

    while(tmpNode != NULL){
        //if it finds the id it searched for
        if(tmpNode->citizenID == givenID){
            if(tmpNode == skipListCheck->headNode){
                skipListCheck->headNode = tmpNode->nextNode;
            }
            if(prevNode != tmpNode){
                prevNode->nextNode = tmpNode->nextNode;
            }
            if(skipListCheck->lowerList == NULL){
                if(tmpNode->date != NULL){
                    free(tmpNode->date);
                }
                free(tmpNode);
                return;
            }
            skipList* tempSkipList = skipListCheck->lowerList;
            free(tmpNode);
            if(skipListCheck->headNode == NULL){
                free(skipListCheck->virus);
                free(skipListCheck);
            }
            skipListCheck = tempSkipList;
            tmpNode = skipListCheck->headNode;
            prevNode = tmpNode;
        }else{
            if(tmpNode->citizenID > givenID){
                if(skipListCheck->lowerList == NULL){
                    return;
                }
                skipListCheck = skipListCheck->lowerList;
                tmpNode = skipListCheck->headNode;
                prevNode = tmpNode;
            }else{
                if(tmpNode->nextNode == NULL){
                    if(skipListCheck->lowerList == NULL){
                        return;
                    }
                    skipListCheck = skipListCheck->lowerList;
                    tmpNode = skipListCheck->headNode;
                    prevNode = tmpNode;
                }else{
                    prevNode = tmpNode;
                    tmpNode = tmpNode->nextNode;
                }
            }
        }
    }
}
