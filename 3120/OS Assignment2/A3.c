/*
Author: Yansong Li
Banner number: B00755534
Date: 2019-11-11
Courese: CSCI3120
Assignment3
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXROW 50
//a sturct that keep one task
typedef struct{
    char *name;
    int arrival_time;
    int burst_time;
}task;

int String_Split(char *argv[], char command[]);
void FirstComeFirstServe(task before[], int taskNumber, FILE *output);
void RoundRobin(task before[], int taskNumber, FILE *output);
void NonPreemptiveSJF(task before[], int taskNumber, FILE *output);
void PreemptiveSJF(task before[], int taskNumber, FILE *output);

/*These two gobal variables calculate the total waiting time in psjf algorithm.
*It declared as gobal variables because when I declare a array inside the PSJF() function, the output changes.
*I cannot figure out the reason, so I have to use gobal varibles to prevent it.
*/
int waiting[MAXROW];
int total=0;

int main(int argc, char* argv[]){
    //record the number of tasks
    int count=0;
    int i;
    task input[MAXROW];
    char temp[MAXROW][64];
    char *part[32];
    char *filename = "TaskSpec.txt";
    FILE *fp = fopen(filename,"r");
    if(fp == NULL){
        printf("Could not open file: %s\n", filename);
        return 1;
    }
    //open the input file and read the content to a string array
    while(fscanf(fp, "%s", &temp[count]) !=EOF){
        fgetc(fp);
        count++;
    }
    fclose(fp);

    //split every string in the string array into three parts and set the value to tasks
    for(i=0;i<count;i++){
        String_Split(part,temp[i]);
        input[i].name = part[0];
        input[i].arrival_time = atoi(part[1]);
        input[i].burst_time = atoi(part[2]);
    }

    //create a txt file for ouptput
    char *filename1 = "Output.txt";
    FILE *fp1 = fopen(filename1,"w");
    if(fp1 == NULL){
        printf("Could not open file: %s\n", filename1);
        return 1;
    }

    //run algorithms
    FirstComeFirstServe(input, count, fp1);
    RoundRobin(input, count, fp1);
    NonPreemptiveSJF(input, count, fp1);
    PreemptiveSJF(input, count, fp1);
    fclose(fp1);
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

//FCFS algorithm
void FirstComeFirstServe(task before[], int taskNumber, FILE *output){
    int totalTime=0;
    int totalBurstTime=0;
    int i;
    int startTime = 0;
    int waitingTime = 0;
    fprintf(output,"FCFS:\n");
    //print the starting time and endind time
    for(i=0; i<taskNumber; i++){
        fprintf(output,"%s\t%d\t%d\n", before[i].name,startTime,startTime+before[i].burst_time);
        startTime = startTime+before[i].burst_time;
    }
    //calculate the total waiting time
    for(i=1; i<taskNumber; i++){
        totalBurstTime = totalBurstTime+before[i-1].burst_time;
        waitingTime = totalBurstTime-before[i].arrival_time;
        totalTime = totalTime+waitingTime;       
    }
    fprintf(output, "Average Waiting Time: %.2f\n\n", (float)totalTime/(float)taskNumber);
}

//RR algorithm
void RoundRobin(task before[], int taskNumber, FILE *output){
    task rr[MAXROW];
    int timeQuantum = 4;
    int startTime=0;
    int endTime=0;
    int totalEndTime=0;
    int i,longestTask=0;
    int longestBurstTime=0;
    int waitingTime[taskNumber];
    int totalWaitingTime=0;
    for(i=0;i<taskNumber;i++){
        waitingTime[i]=0;
    }
    //put the content of orginal task array into a new one in case we change the content during calculation
    for(i=0;i<taskNumber;i++){
        rr[i]=before[i];
    }
    //find out the longest burst time in the array and its index
    for(i=0;i<taskNumber;i++){
        if(longestBurstTime<=rr[i].burst_time){
            longestBurstTime=rr[i].burst_time;
            longestTask=i;
        }
    }
    fprintf(output,"RR:\n");
    //as long as the longest burst time is not equal to zero, keep running
    while(rr[longestTask].burst_time!=0){
        for(i=0; i<taskNumber; i++){
            //if the burst time is less than the time quantum and it's not equal to zero
            if(rr[i].burst_time<timeQuantum&&rr[i].burst_time!=0){
                endTime=startTime+rr[i].burst_time;
                rr[i].burst_time=0;
                fprintf(output,"%s\t%d\t%d\n", rr[i].name,startTime,endTime);
                //record the waiting time
                waitingTime[i]=waitingTime[i]+(startTime-rr[i].arrival_time);
                startTime = endTime;  
                rr[i].arrival_time = startTime;    
            //if the burst time is equal or more than the time quantum and it's not equal to zero        
            }else if(rr[i].burst_time!=0){
                endTime=timeQuantum;
                totalEndTime = totalEndTime+endTime;
                rr[i].burst_time = rr[i].burst_time-timeQuantum;
                fprintf(output,"%s\t%d\t%d\n", rr[i].name,startTime,totalEndTime);
                //record the waiting time
                waitingTime[i]=waitingTime[i]+(startTime-rr[i].arrival_time);
                startTime = startTime+endTime;
                rr[i].arrival_time = startTime;
            }           
        }
    }
    //calculate the total waiting time
    for(i=0;i<taskNumber;i++){
        totalWaitingTime = totalWaitingTime+waitingTime[i];
    }
    fprintf(output, "Average Waiting Time: %.2f\n\n", (float)totalWaitingTime/(float)taskNumber);
    
}

// Non-preemptive Shortest-Job-First algorithm
void NonPreemptiveSJF(task before[], int taskNumber, FILE *output){
    int totalTime=0;
    int totalBurstTime=0;
    int i,j,m=0;
    int startTime = 0;
    int waitingTime = 0;
    task npsjf[MAXROW];
    task temp;
    //put the content of orginal task array into a new one in case we change the content during calculation
    for(i=0;i<taskNumber;i++){
        npsjf[i]=before[i];
    }
    fprintf(output,"NSJF:\n");
    //sort the tasks in the array
    for(i=0; i<taskNumber; i++ ){
        for(j=0; j<taskNumber-m-1; j++){
            if(npsjf[j+1].burst_time<npsjf[j].burst_time&&(npsjf[j+1].arrival_time<=npsjf[j].arrival_time||
            npsjf[j+1].arrival_time<npsjf[0].burst_time&&npsjf[j].arrival_time!=0)){
                temp = npsjf[j];
                npsjf[j] = npsjf[j+1];
                npsjf[j+1] = temp;
            }
        }
        m++;
    }
    //print the starting time and ending time
    for(i=0; i<taskNumber; i++){
        fprintf(output,"%s\t%d\t%d\n", npsjf[i].name,startTime,startTime+npsjf[i].burst_time);
        startTime = startTime+npsjf[i].burst_time;
    }
    //calculate the total waiting time
    for(i=1; i<taskNumber; i++){
        totalBurstTime = totalBurstTime+npsjf[i-1].burst_time;
        waitingTime = totalBurstTime-npsjf[i].arrival_time;
        totalTime = totalTime+waitingTime;       
    }
    fprintf(output, "Average Waiting Time: %.2f\n\n", (float)totalTime/(float)taskNumber);
}

//Preemptive Shortest-Job-First algorithm
void PreemptiveSJF(task before[], int taskNumber, FILE *output){
    task temp1;
    task psjf[MAXROW];
    int i,j,m=0;
    int startTime=0;
    int longestBurstTime=0;
    int shortestBurstTime=before[0].burst_time;
    int longestTask=0;
    int shorestTask=0;
    total=0;
    for(i=0;i<taskNumber;i++){
        waiting[i]=0;
    }
    //put the content of orginal task array into a new one in case we change the content during calculation
    for(i=0;i<taskNumber;i++){
        psjf[i]=before[i];
    }
    fprintf(output,"PSJF:\n");
    //sort the tasks
    for(i=0; i<taskNumber; i++ ){
        for(j=0; j<taskNumber-m-1; j++){
            if(psjf[j+1].burst_time<psjf[j].burst_time&&(psjf[j+1].arrival_time<=psjf[j].arrival_time||
            psjf[j+1].arrival_time<psjf[0].burst_time&&psjf[j].arrival_time!=0)){
                temp1 = psjf[j];
                psjf[j] = psjf[j+1];
                psjf[j+1] = temp1;
            }
        }
        m++;
    }
    //find the longest burst time and its index
    for(i=0;i<taskNumber;i++){
        if(longestBurstTime<=psjf[i].burst_time){
            longestBurstTime=psjf[i].burst_time;
            longestTask=i;
        }
    }
    while(psjf[longestTask].burst_time!=0){    
        //find the current shortest burst time and its index  
        for(i=0; i<taskNumber; i++){
            for(j=0;j<taskNumber;j++){
                if(shortestBurstTime>=psjf[j].burst_time&&psjf[j].burst_time!=0){
                   shortestBurstTime=psjf[j].burst_time;
                   shorestTask=j;                   
                }
            }
            //if the next task has a shorter burst time than current task
            if(psjf[i+1].burst_time<psjf[i].burst_time&&psjf[i+1].arrival_time>psjf[i].arrival_time){
                fprintf(output,"%s\t%d\t%d\n", psjf[i].name,startTime,startTime+psjf[i+1].arrival_time);
                waiting[i]=waiting[i]+(startTime-psjf[i].arrival_time);
                startTime = startTime+psjf[i+1].arrival_time;
                psjf[i].arrival_time = startTime;
                psjf[i].burst_time = psjf[i].burst_time-psjf[i+1].arrival_time;
            //if the next task has a longer or euqal burst time than current task
            }else if(psjf[i].burst_time!=0){
                //if the next burst time is no the current shorest burst time change to the shorst task
                if(psjf[i+1].burst_time>shortestBurstTime){
                    i=shorestTask;                    
                }
                fprintf(output,"%s\t%d\t%d\n", psjf[i].name,startTime,startTime+psjf[i].burst_time);
                waiting[i]=waiting[i]+(startTime-psjf[i].arrival_time);
                startTime = startTime+psjf[i].burst_time;
                psjf[i].arrival_time = startTime;
                psjf[i].burst_time=0;
                shortestBurstTime=longestBurstTime;
            }
        }
    }
    //calculate the total waiting time
    for(i=0;i<taskNumber;i++){
        total = total+waiting[i];
    }
    fprintf(output, "Average Waiting Time: %.2f\n\n", (float)total/(float)taskNumber);
}