#ifndef VACCINEMONITOR_BLOOMFILTER_H
#define VACCINEMONITOR_BLOOMFILTER_H

typedef struct bloomFilter{
    char* virus;
    int* bitArray;
    struct bloomFilter* next;
} bloomFilter;

bloomFilter* bloomInit(int bytes, char* virus);
bloomFilter* addBloom(bloomFilter* bloomList, int bytes, char* virus);
int bloomExists(bloomFilter* bloomList, char* virus);
bloomFilter* getBloom(bloomFilter* bloomList, char* virus);
void deleteBloom(bloomFilter* bloomList);
void setBloomBit(struct bloomFilter* bloom, int bit);
int getBloomBit(struct bloomFilter* bloom, int bit);

unsigned long djb2(unsigned char *str);
unsigned long sdbm(unsigned char *str);
unsigned long hash_i(unsigned char *str, unsigned int i);

#endif //VACCINEMONITOR_BLOOMFILTER_H
