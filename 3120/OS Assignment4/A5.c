/*
Author: Yansong Li
Banner number: B00755534
Date: 2019-12-06
Courese: CSCI3120
Assignment 5
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXPAGE 20 //maximum number of reference strings

void FirstInFirstOut(int referenceString [], int numberOfFrames, int length, FILE *output);

int main(int argc, char* argv[]){
    //count the amount of reference string in Input.txt
    int count = 0;
    int i;
    char *filename = "Input.txt";
    char *filename1 = "Output.txt";
    int number_frame;
    int reference[MAXPAGE];
    FILE *fp = fopen(filename,"r");
    if(fp == NULL){
        printf("Could not open file: %s\n", filename);
        return 1;
    }
    //read the number of frames to an int variable 
    fscanf(fp,"%d",&number_frame);
    //read the reference strings to an int array same method from my assignment 2
    while(fscanf(fp, "%d*%[^,]", &reference[count]) !=EOF){
        fgetc(fp);
        count++;
    }
    fclose(fp);

    //open a new text file for outputs
    FILE *fp1 = fopen(filename1,"w");
    if(fp1 == NULL){
        printf("Could not open file: %s\n", filename1);
        return 1;
    }
    //run the FIFO algorithm 
    FirstInFirstOut(reference, number_frame, count, fp1);
    fclose(fp1);
    return 0;
}

//FIFO algorithm
void FirstInFirstOut(int referenceString [], int numberOfFrames, int length, FILE *output){
    int i,j,l,k = 0;
    int number_fault = 0;
    int flag = 0;
    //set the vlaue of page frames to -1
    int row[numberOfFrames];
    for(i=0; i<numberOfFrames; i++){
        row[i]=-1;
    }
    for(i=0; i<length; i++){
        //check the reference string is in the page frames or not
        for(j=0; j<numberOfFrames; j++){
            if(row[j] == referenceString[i]){
                flag = 1;
                break;
            }
        }
        //if the reference string is not in the page frames then put it into the page frames 
        //and increment the number of page faults
        if(flag == 0){
            number_fault++;
            row[k] = referenceString[i];
            k = (k+1)%numberOfFrames;
        }
        //print out the page frames
        for (l=0;l<numberOfFrames;l++) {
            if (row[l]==-1&&l!=numberOfFrames-1){
                fprintf(output,"N\t"); 
            }else if(row[l]==-1&&l==numberOfFrames-1){
                fprintf(output,"N\n"); 
            }else if(!(row[l]==-1)&&l==numberOfFrames-1){
                fprintf(output,"%d\n", row[l]);
            }else{
                fprintf(output,"%d\t", row[l]);  
            }
        }
        flag=0;
    }
    //print out the number of page faults
    fprintf(output,"\nNumber of Page Faults : %d",number_fault);
}