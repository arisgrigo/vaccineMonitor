#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bloomFilter.h"

//initializes a bloom filter
bloomFilter* bloomInit(int bytes, char* vir){
    //bytes, is the size of the bloom filter, in bytes
    //since the bloom filter is made up of an array of ints, we divide these bytes by 4, to get the spaces we need
    int spaces = bytes/4;
    bloomFilter* bloom = (bloomFilter*) malloc(sizeof(bloomFilter));
    if(bloom != NULL) {
        bloom->virus = malloc(strlen(vir) + 1);
        strcpy(bloom->virus, vir);

        bloom->bitArray = (int *) malloc(sizeof(int) * spaces);

        //initialize the bit array, so each it's filled with zeroes
        for (int i = 0; i < spaces; i++) {
            bloom->bitArray[i] = 0;
        }

        //next pointer, points to the bloom filter that was created before the current one
        bloom->next = NULL;
    }
    return bloom;
}

//adds a bloom filter to the bloom list
bloomFilter* addBloom(bloomFilter* bloomList, int bytes, char* virus){
    bloomFilter* bloom = bloomInit(bytes, virus);
    if(bloom != NULL){
        bloom->next = bloomList;
    }
    return bloom;
}

//returns 1 if a bloom filter for a specific virus has been made
int bloomExists(bloomFilter* bloomList, char* virus){
    int check = 0;
    if(bloomList == NULL){
        return 0;
    }

    if(strcmp(bloomList->virus, virus) == 0){
        return 1;
    }else if(bloomList->next != NULL){
        check = bloomExists(bloomList->next, virus);
    }
    return check;
}

//returns the bloom filter with the given virus name (if it exists)
bloomFilter* getBloom(bloomFilter* bloomList, char* virus){
    if(bloomList == NULL){
        return NULL;
    }

    while(bloomList != NULL){
        if(strcmp(bloomList->virus, virus) == 0){
            return bloomList;
        }
        bloomList = bloomList->next;
    }
    return NULL;
}

//frees the memory allocated for the bloom filter list
void deleteBloom(bloomFilter* bloomList){
    if(bloomList->next != NULL){
        deleteBloom(bloomList->next);
    }
    free(bloomList->bitArray);
    free(bloomList->virus);
    free(bloomList);
}

//sets the given bit of a bloom filter to 1
void setBloomBit(bloomFilter* bloom, int bit){
    if(bloom == NULL){
        return;
    }

    int arrayIndex = bit/32;        //use arrayIndex to get the correct index in the bit array
    int bitPos = bit%32;            //use bitPos to get the specific bit place in that index
    unsigned int bitFlag = 1;

    bitFlag = bitFlag << bitPos;    //bit flag is used to set the correct bit, and only that, to 1
    //if the bit in the bitPos of the arrayIndex was 0, now it's 1
    bloom->bitArray[arrayIndex] = bloom->bitArray[arrayIndex] | bitFlag;
}

//returns 1 if the specific bit that was given is 1, in the given bloom filter
int getBloomBit(bloomFilter* bloom, int bit){
    if(bloom == NULL){
        printf("this Bloom Filter does not exist yet\n");
        return 0;
    }

    int arrayIndex = bit/32;
    int bitPos = bit%32;
    unsigned int bitFlag = 1;

    bitFlag = bitFlag << bitPos;

    if(bloom->bitArray[arrayIndex] & bitFlag){
        return 1;
    } else{
        return 0;
    }
}

unsigned long djb2(unsigned char* str){
    unsigned long hash = 5381;
    int c;
    while(c = *str++){
        hash = ((hash<<5) + hash) + c;
    }
    return hash;
}

unsigned long sdbm(unsigned char *str) {
    unsigned long hash = 0;
    int c;

    while (c = *str++) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

unsigned long hash_i(unsigned char *str, unsigned int i) {
    return djb2(str) + i*sdbm(str) + i*i;
}