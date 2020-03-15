/* Author: Kiana Kabiri
* Student ID: 9871304
* Group: B
* Filename:
* Date: 03/03/2020
*
*/
// add necessary libraries
#include <stdio.h>
#include <stdlib.h>
//add math library for log function
#include <math.h>
#include <string.h>

//start main function
int main() {
    //declare a pointer of the file_pointer
    //pf means pointer of type file
    FILE *pf1;
    FILE *pf2;

    //declare variables used in the trace file
    char RW;
    int address;
    //The ID number of the cache configuration mode (1 … 8)
    int mode_ID;


    int cache[256]={0};
    //Valid bit indicates if the cache block has been filled or not
    //Total number of items in the valid bits array needed is the same as the Number of Cache lines for each mode
    int ValidBit[256]={0};
    //Dirty bit indicates if the cache block has been modified or not
    //Total number of items in the dirty bits array needed is the same as the Number of Cache lines for each mode
    int DirtyBit[256]={0};
        //tracker is a variable used to see where the FIFO algorithm needs to replace an item
    int tracker=0;
        //Total number of read accesses to the external memory
    int NRA = 0;
    //Total number of write accesses to the external memory
    int NWA = 0;
    //Number of cache read hits
    int NCRH = 0;
    //Number of cache read misses
    int NCRM = 0;
    //Number of cache write hits
    int NCWH = 0;
    //Number of cache write misses
    int NCWM = 0;
    //tag is used to assess whether the address exists in cache or not
    int tag;
    //Cache Block Size changes to the power of 2 in each mode
    int CacheBlockSize;
    int NumberofCacheLines;
   //set up if statements for Cache Block Size and Number of Cache Lines for all 8 modes

    //open the files
    pf1 = fopen("bubble_sort_trace_060.trc", "r" );
    pf2 = fopen("cross_correlation_trace_060.trc", "r");

    //check if both files opens successfully
    if(pf1 == NULL || pf2 == NULL){
    //if both files don't open display a message
        printf("unable to open the file\n");
    //if both files open execute the cache simulation
    }else{
        for(mode_ID = 1; mode_ID<9;mode_ID++){
            rewind(pf1);

            if (mode_ID==1){
                CacheBlockSize = 1;
                NumberofCacheLines = 256;
            }else if(mode_ID==2){
                CacheBlockSize = 2;
                NumberofCacheLines = 128;
            }else if(mode_ID==3){
                CacheBlockSize = 4;
                NumberofCacheLines = 64;
            }else if(mode_ID==4){
                CacheBlockSize = 8;
                NumberofCacheLines = 32;
            }else if(mode_ID==5){
                CacheBlockSize = 16;
                NumberofCacheLines = 16;
            }else if(mode_ID==6){
                CacheBlockSize = 32;
                NumberofCacheLines = 8;
            }else if(mode_ID==7){
                CacheBlockSize = 64;
                NumberofCacheLines = 4;
            }else if(mode_ID==8){
                CacheBlockSize = 128;
                NumberofCacheLines = 2;
            }





            //fscanf is used to go through the file opened
            while(fscanf(pf1, "%c %x\n" , &RW, &address)==2){
                //perform bit shifting operation on the address based on which mode it is in
                tag = address>>(int) (log(CacheBlockSize)/log(2));
            //if it is a read memory access

            if(RW == 'R'){
                //run through the loop until all cache lines have been evaluated
                for(int j=0; j<NumberofCacheLines; j++){

                        //if the tag is in the cache and valid bit has been set
                        if(cache[j] == tag&&ValidBit[j]==1){

                            //increment the number of read hits
                            NCRH++;
                            //increase number of read accesses by the cache block size
                            break;
                        }else if(j==NumberofCacheLines-1) {
                                NRA = NRA+CacheBlockSize;

                                //increment number of read misses
                                NCRM++;
                                cache[tracker]=tag;
                                //set the valid bit to 1

                                //and if the dirty bit is not set
                                if(DirtyBit[tracker] == 1){
                                    //increase number of write accesses by cache block size
                                    NWA = NWA + CacheBlockSize;
                                }
                                ValidBit[tracker] = 1;
                                DirtyBit[tracker]=0;

                                tracker++;
                            //if the tracker is less than the number of cache lines
                            if(tracker>NumberofCacheLines-1){
                                //if it reaches the top of the array then reset to 0
                                tracker=0;
                            }
                            break;
                        }
                    }
                }
                else if (RW = 'W'){
                    //run through loop until all cache lines have been evaluated
                    for(int j=0; j<NumberofCacheLines; j++){

                        if (cache[j] == tag&&ValidBit[j]==1){
                            //if tag exists in cache then increment number of cache write hits by 1
                            NCWH++;
                            DirtyBit[j] = 1;
                            //terminate loop and move on to the next loop
                            break;
                        }else if(j==NumberofCacheLines-1) {
                            NRA = NRA+CacheBlockSize;

                            //increment number of write misses
                            NCWM++;

                            //place the tag in cache
                            cache[tracker]=tag;

                            if(DirtyBit[tracker] == 1){
                                //increase the number of write accesses by cache block size
                                NWA = NWA + CacheBlockSize;
                            }
                            DirtyBit[tracker] = 1;
                            ValidBit[tracker] = 1;
                            //move the tracker up the cache array
                            tracker++;
                            if(tracker>NumberofCacheLines-1){
                                //if the tracker reaches the top of the cache array reset tracker to 0 to fulfill FIFO
                                tracker=0;
                            }
                            break;
                        }
                    }
                }
            }
            printf("\nbubble_sort_trace_060.trc, %d, %d, %d, %d, %d, %d, %d", mode_ID, NRA, NWA, NCRH, NCRM, NCWH, NCWM);
            memset (cache,0,sizeof(cache));
            memset (ValidBit,0,sizeof(ValidBit));
            memset (DirtyBit,0,sizeof(DirtyBit));
            tracker=0;
            //Total number of read accesses to the external memory
            NRA = 0;
            //Total number of write accesses to the external memory
            NWA = 0;
            //Number of cache read hits
            NCRH = 0;
            //Number of cache read misses
            NCRM = 0;
            //Number of cache write hits
            NCWH = 0;
            //Number of cache write misses
            NCWM = 0;
        }
    }

    return 0;
}

