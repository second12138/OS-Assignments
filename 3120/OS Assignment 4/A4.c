/*
Author: Yansong Li
Banner number: B00755534
Date: 2019-11-24
Courese: CSCI3120
Assignment 4
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//assignment description indicates there are at most 10 rows in “LogicalAddress.txt”
#define MAXROW 10
//assignment description indicates the size of page/frame is 4 bytes
#define SIZE 4

//a sturct for page table
typedef struct{
    int paging;
    int frame;
}pageTable;

//a function that can split a line into chars from my assignment 1
int String_Split(char *argv[], char command[]);

int main(int argc, char* argv[]){
    //count the amount of numbers in LogicalAddress.txt
    int count = 0;
    //count the amount of numbers in PageTable.txt
    int count1 = 0;
    int i,j,k;
    //temporarily store the content of LogicalAddress.txt
    char temp[MAXROW][32];
    //temporarily store the content of PageTable.txt
    char temp1[MAXROW][32];
    char *part[32];
    char *part1[32];
    char *filename = "LogicalAddress.txt";
    char *filename1 = "PageTable.txt";
    char *filename2 = "PhysicalAddresses.txt";
    //dynamically allocate the space for page table array
    pageTable *pagetable = (pageTable *)malloc(MAXROW*sizeof(pageTable));
    //dynamically allocate the space for logical address array
    int *logicalAddress = (int *)malloc(MAXROW*sizeof(int));
    //dynamically allocate the space for physical address array
    int *physicalAddress = (int *)malloc(MAXROW*sizeof(int));
    int binary[MAXROW][SIZE];
    int twoDecimal[MAXROW][2];

    //open the input file of logical address and read it into a string array
    FILE *fp = fopen(filename,"r");
    if(fp == NULL){
        printf("Could not open file: %s\n", filename);
        return 1;
    }
    while(fscanf(fp, "%s", &temp[count]) !=EOF){
        fgetc(fp);
        count++;
    }
    fclose(fp);

    //turn the string array into a int array
    for(i=0;i<count;i++){
        String_Split(part1,temp[i]);
        logicalAddress[i] = atoi(part1[0]);
    }

    //convert decimal numbers into binary
    for(i=0;i<count;i++){
        for(j=0;j<SIZE;j++){
            binary[i][j] = 0;
        }
    }
    for(i=0;i<count;i++){
        for(j=SIZE-1;logicalAddress[i]>0;j--){
            binary[i][j]=logicalAddress[i]%2;    
            logicalAddress[i]=logicalAddress[i]/2;    
        }  
    }

    //convert four digits binary numbers to two decimal numbers
    for(i=0;i<count;i++){
        twoDecimal[i][0]=0;   
        twoDecimal[i][1]=0;
    }
    for(i=0;i<count;i++){
        twoDecimal[i][0] = binary[i][0]*2 + binary[i][1];
        twoDecimal[i][1] = binary[i][2]*2 + binary[i][3];
    }

    //open the input file of page table and read it into a string array
    FILE *fp1 = fopen(filename1,"r");
    if(fp1 == NULL){
        printf("Could not open file: %s\n", filename1);
        return 1;
    }
    while(fscanf(fp1, "%s", &temp1[count1]) !=EOF){
        fgetc(fp1);
        count1++;
    }
    fclose(fp1);

    //split every string in the string array of  pageTable into two int and set the values to pageTable struct
    for(i=0;i<count1;i++){
        String_Split(part,temp1[i]);
        pagetable[i].paging = atoi(part[0]);
        pagetable[i].frame = atoi(part[1]);
    }

    //calculate the physical addresses 
    for(i=0; i<count; i++){
        physicalAddress[i] = pagetable[twoDecimal[i][0]].frame*SIZE + twoDecimal[i][1];
    }

    //open a new text file for outputs
    FILE *fp2 = fopen(filename2,"w");
    if(fp2 == NULL){
        printf("Could not open file: %s\n", filename2);
        return 1;
    }
    for(i=0;i<count;i++){
        fprintf(fp2,"%d\n",physicalAddress[i]);
    }
    
    //free the dynamically allocated spaces
    free(pagetable);
    free(logicalAddress);
    free(physicalAddress);

    return 0;
}

//split one string into three parts
int String_Split(char *argv[], char command[]){
    char *delim  = ",";
    int index =0;
    char *token;
    token = strtok(command, delim);
    argv[index] = token;
    while(token){
        token  = strtok(NULL, delim);
        argv[++index] = token;
    }
    return 0;
}