/* CSC 541 - assignment 1 - hramach2 */
// in-memory versus disk-based searching

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>

// custom headers
#include "assn_1.h"

// Variables to track execution time
struct timeval start;
struct timeval end; 
struct timeval exec_tm;

/* retruns the size of the input file
   returns type long int  */
long int getFileSize(FILE *temp) {

    long int sizeBytes = 0;
    long int sizeInt = 0;

    fseek(temp, 0, SEEK_END);
    sizeBytes = ftell(temp);
    sizeInt = sizeBytes/sizeof(int);
    //printf("Size(in bytes) after ftell = %ld \n", sizeBytes);
    //printf("Number of integers in the file = %ld \n", sizeInt);
    fseek(temp, 0, SEEK_SET); //set fp to start of file
    return sizeInt;
}

/* Read the file contents into the array */
void readFile(int *tempElements, FILE *temp) {

    // read all the elements in seek into array
    long int j = 0;
    int i;
    while(1) {
        if( !(fread( &i, sizeof(int), 1, temp))) {
            //printf("Reached eof \n");
            break; // out of while loop
        }
        tempElements[j] = i;
        j = j + 1;
    }
}

/* IN-MEMORY SEQUENTIAL SEARCH */
void in_mem_seq(int *seekElements, long int sizeSeek, FILE *key) {

    int ki, si;
    long int sizeKey = 0;

    // initialize hit array
    int *hit;
    hit = (int *) malloc(sizeSeek * sizeof(int));

    // ***setting start timer***
    gettimeofday(&start, NULL); 
    //printf("Seconds: %d\n", start.tv_sec); 
    //printf("Microseconds: %d\n", start.tv_usec);

    // get num of integers in key.db
    sizeKey = getFileSize(key);    

    // intialize integer array to hold key.db 
    int *keyElements;
    keyElements = (int *) malloc(sizeKey * sizeof(int));

    // read the file contents into the array in MEMORY
    readFile(keyElements, key);

    // loop over each element in seek, s[i]
    for(long int i=0; i < sizeSeek; i++) {
        // for each element in key, k[i]
        for(long int j=0; j < sizeKey; j++) {
            // is found set hit[i]
            if(seekElements[i] == keyElements[j]) {
                hit[i] = 1;
            }
        }
    }

    // ***setting end timer***
    gettimeofday(&end, NULL); 

    // print S[i] as per hit[i] 
    for(long int k=0; k < sizeSeek; k++) {
        if(hit[k]==1) {
            printf( "%12d: Yes\n", seekElements[k]);
        }
        if( hit[k]==0) {    
            printf( "%12d: No\n", seekElements[k]);
        }
    }

    // get execution time 
    timersub(&end, &start, &exec_tm);
    printf( "Time: %ld.%06ld\n", exec_tm.tv_sec, exec_tm.tv_usec );    
}

/* IN-MEMORY BINARY SEARCH */
void in_mem_bin(int *seekElements, long int sizeSeek, FILE *key) {

    int ki, si;
    long int sizeKey = 0;

    // initialize hit array
    int *hit;
    hit = (int *) malloc(sizeSeek * sizeof(int));

    // ***setting start timer***
    gettimeofday(&start, NULL); 
    //printf("Seconds: %d\n", start.tv_sec); 
    //printf("Microseconds: %d\n", start.tv_usec);

    // get num of integers in key.db
    sizeKey = getFileSize(key);    

    // intialize integer array to hold key.db 
    int *keyElements;
    keyElements = (int *) malloc(sizeKey * sizeof(int));

    // read the file contents into the array in MEMORY
    readFile(keyElements, key);
    
    long int lokey=0;
    long int midkey=0; 
    long int hikey=0; 

    // loop over each element in seek, s[i]
    for(long int i=0; i < sizeSeek; i++) {

        // Binary search part
        lokey = 0;
        hikey = sizeKey;
        //printf("lokey=%ld; hikey=%ld",lokey, hikey);
        while( lokey <= hikey) {
            midkey = (lokey+hikey)/2;
            if(seekElements[i] == keyElements[midkey]) {
                hit[i] = 1;
                break; // out of while
            }
            else if(seekElements[i] < keyElements[midkey]) {
                hikey = midkey - 1;
            }
            else if(seekElements[i] > keyElements[midkey]) {
                lokey = midkey + 1;
            }
        }
    }

    // ***setting end timer***
    gettimeofday(&end, NULL); 

    // print S[i] as per hit[i] 
    for(long int k=0; k < sizeSeek; k++) {
        if(hit[k]==1) {
            printf( "%12d: Yes\n", seekElements[k]);
        }
        if( hit[k]==0) {    
            printf( "%12d: No\n", seekElements[k]);
        }
    }

    // get execution time 
    timersub(&end, &start, &exec_tm);
    printf( "Time: %ld.%06ld\n", exec_tm.tv_sec, exec_tm.tv_usec );    
}

/* ON_DISK SEQUENTIAL SEARCH */
void on_disk_seq(int *seekElements, long int sizeSeek, char keyfile[15]) {

    int ki, si;
    //long int sizeKey = 0;
    FILE *key;

    // initialize hit array
    int *hit;
    hit = (int *) malloc(sizeSeek * sizeof(int));

    // ***setting start timer***
    gettimeofday(&start, NULL); 

    // loop over each element in seek, s[i]
    for(long int i=0; i < sizeSeek; i++) {
        // open keyfile for each seek element
        key = fopen(keyfile, "rb");
        while(1) {    
            // check with each element in keyfile till eof
            if( !(fread( &ki, sizeof(int), 1, key))) {
                fclose(key);
                break; //exit while loop
            }        
            // if found set hit[i]
            if(seekElements[i] == ki) {
                hit[i] = 1;
            }
        }
    }

    // ***setting end timer***
    gettimeofday(&end, NULL); 

    // print S[i] as per hit[i] 
    for(long int k=0; k < sizeSeek; k++) {
        if(hit[k]==1) {
            printf( "%12d: Yes\n", seekElements[k]);
        }
        if( hit[k]==0) {    
            printf( "%12d: No\n", seekElements[k]);
        }
    }

    // get execution time 
    timersub(&end, &start, &exec_tm);
    printf( "Time: %ld.%06ld\n", exec_tm.tv_sec, exec_tm.tv_usec );    
}

/* ON_DISK BINARY SEARCH */
void on_disk_bin(int *seekElements, long int sizeSeek, char keyfile[15]) {

    int si, ki;
    long int lokey = 0, hikey=0, midkey;
    long int sizeKey, j = 0;
    FILE *key;

    // initialize hit array
    int *hit;
    hit = (int *) malloc(sizeSeek * sizeof(int));

    // open keyfile for each seek element
    key = fopen(keyfile, "rb");
    // exit if fail to open the file 
    if( (!key) ) {
        printf("could not open input files \n");
        exit(1);
    }    
    // get num of integers in key.db
    sizeKey = getFileSize(key);    

    // ***setting start timer***
    gettimeofday(&start, NULL); 

    // loop over each element in seek, s[i]
    for(long int i=0; i < sizeSeek; i++) {

        // set filepointer to beginning and clear errors
        fseek(key, 0, SEEK_SET);
        clearerr(key);

        lokey = 0;
        hikey = sizeKey;        

        // Binary Search Part
        while(lokey <= hikey) {
            
            midkey = lokey + ((hikey-lokey)/2);
            fseek( key, midkey * sizeof( int ), SEEK_SET);

            if( !fread (&ki, sizeof( int ), 1, key)) {
                clearerr(key);
                break; // exit while loop - hit[i]=0 condition
            }
            // check with each element in keyfile till eof
            if( seekElements[i] == ki) {
                hit[i]=1;
                break; //exit while loop
            } 
            else if(seekElements[i] < ki) {
                hikey = midkey - 1;
            }
            else if(seekElements[i] > ki) {
                lokey = midkey + 1;
            }
        }
    }


    // ***setting end timer***
    gettimeofday(&end, NULL); 

    // print S[i] as per hit[i] 
    for(long int k=0; k < sizeSeek; k++) {
        if(hit[k]==1) {
            printf( "%12d: Yes\n", seekElements[k]);
        }
        if( hit[k]==0) {    
            printf( "%12d: No\n", seekElements[k]);
        }
    }

    // get execution time 
    timersub(&end, &start, &exec_tm);
    printf( "Time: %ld.%06ld\n", exec_tm.tv_sec, exec_tm.tv_usec );    
}





// MAIN FUNCTION
int main(int argc, char *argv[]){

    // checking if arguments are proper
    if(argc < 4){
        printf("Insufficient arguments, need 3 arguments...\n assn_1 search-mode keyfile-name seekfile-name \n");
    exit(1);
    }

/*    for(int i=1; i <  argc; i++){
        printf("argv[%d]=%s \n", i, argv[i]);
    }
*/

    char sMode[15];
    char keyfile[15];
    char seekfile[15];
    char tempc[15];

    strcpy(sMode, argv[1]);
    strcpy(keyfile, argv[2]);
    strcpy(seekfile, argv[3]);

    FILE *key;
    FILE *seek;
    FILE *out;
    int ki, si;
    long int sizeSeek = 0;

    // seek-file and key-file opened
    seek = fopen(seekfile, "rb");
    key = fopen(keyfile, "rb");

    // exit if fail to open the file 
    if( (!seek) || (!key) ) {
        printf("could not open input files \n");
        return 1;
    }    

    // get num of integers in seek.db
    sizeSeek = getFileSize(seek);    

    // intialize integer array to hold seek.db
    int *seekElements;
    seekElements = (int *) malloc(sizeSeek * sizeof(int));

    // read the file contents into the array
    readFile(seekElements, seek);
    //printf("Seek.db read into an array....\n");

   
    if( !strcmp(sMode, IN_MEMORY_SEQUENTIAL)) {
        //printf("Calling In-Memory Sequential Search on the input files \n");
        in_mem_seq(seekElements, sizeSeek, key);
    } 
    else if( !strcmp(sMode, IN_MEMORY_BINARY)) {
        //printf("Calling In-Memory Binary Search on the input files \n");
        in_mem_bin(seekElements, sizeSeek, key);
    } 
    else if( !strcmp(sMode, ON_DISK_SEQUENTIAL)) {
        // closing the keyfile file pointer
        fclose(key);
        on_disk_seq(seekElements, sizeSeek, keyfile);
    } 
    else if( !strcmp(sMode, ON_DISK_BINARY)) {
        on_disk_bin(seekElements, sizeSeek, keyfile);
    } 

    
}
