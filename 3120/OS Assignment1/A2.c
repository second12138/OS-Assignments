/*
Author: Yansong Li
Banner number: B00755534
Date: 2019-10-22
Courese: CSCI3120
Assignment2
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXINT 500 //There could be at most 500 integers in the list
#define MAXRANGE 1000 //The range of the integers is [1,999]

void *sorter(void *params); /*thread that performs basic sorting algorithm*/
void *merger(void *params); /*thread that performs merging of results*/

/*structure for passing data to threads*/
typedef struct{
  int starting_index;
  int ending_index;
}parameters;

int before[MAXINT];
int after[MAXINT];

int main (int argc, const char *argv[]){
  int i=0;
  int listlength=0;
  char ch;

  /*Open the "IntegerList.txt"
  reference: https://stackoverflow.com/questions/26443492/read-comma-eparated-values-from-a-text-file-in-c*/
  char *filename = "IntegerList.txt";
  FILE *fp = fopen(filename,"r");
  if(fp == NULL){
    printf("Could not open file: %s\n", filename);
    return 1;
  }

  /*read the integers in the file to a array
  reference：https://www.geeksforgeeks.org/fgetc-fputc-c/*/
  while(fscanf(fp, "%d*%[^,]", &before[listlength],&ch) !=EOF){
    fgetc(fp);
    listlength++;
  }
  fclose(fp);

  pthread_t threads[3];
  /*create the first sorting thread*/
  parameters *first = (parameters *)malloc(sizeof(parameters));
  first->starting_index = 0;
  first->ending_index = (listlength/2)-1;
  pthread_create(&threads[0], 0, sorter, first);

  /*create the second sorting thread*/
  parameters *second = (parameters *)malloc(sizeof(parameters));
  second->starting_index = (listlength/2);
  second->ending_index = listlength-1;
  pthread_create(&threads[1], 0, sorter, second);

  /*now wait for the 2 sorting threads to finish
   reference: https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_join.html*/
  for(i=0;i<2;i++){
    pthread_join(threads[i],NULL);
  }

  /*establish the merge thread*/
  parameters *third = (parameters *)malloc(sizeof(parameters));
  third->starting_index = listlength/2;
  third->ending_index = listlength-1;
  pthread_create(&threads[2], 0, merger, third);

  /*wait for the merge thread to finish*/
  pthread_join(threads[2],NULL);

  /*output the sorted array
  reference: https://stackoverflow.com/questions/2229377/writing-an-integer-to-a-file-with-fputs*/
  char *filename1 = "SortedIntegerList.txt";
  FILE *fp1 = fopen(filename1,"w");
  if(fp1 == NULL){
    printf("Could not open file: %s\n", filename1);
    return 1;
  }
  fprintf(fp1,"%d",after[0]);
  for(i=1;i<listlength;i++){
    fprintf(fp1,",%d",after[i]);
  }
  fclose(fp1);
  free(first);
  free(second);
  free(third);
  return 0;
}
/*sorting algorithm implementation*/
/*Sorting algorithm is bubble sort
reference: https://www.geeksforgeeks.org/bubble-sort/*/
void *sorter(void *params){
  parameters *p = (parameters *)params;
  int temp;
  int a = p->starting_index;
  int b = p->ending_index+1;
  int i, j, k, m=0;
  for(i=a; i<b; i++ ){
    for(j=a; j<b-m-1; j++){
      if(before[j] > before[j+1]){
        temp = before[j];
        before[j] = before[j+1];
        before[j+1] = temp;
      }
    }
    m++;
  }
  pthread_exit(0);
}

/* merging algorithm implementation*/
void *merger(void *params){
  parameters *p=(parameters *)params;
  int a = p->starting_index;
  int b = p->ending_index+1;
  int i, j,k;
  i = 0, j = a, k = 0;
  while(i<a && j<b){
    if(before[i] < before[j]){
      after[k] = before[i];
      k++;
      i++;
    }else{
      after[k] = before[j];
      k++;
      j++;
    }
  }
  for(; i<a; i++){
    after[k++] = before[i];
  }
  for(; j<b; j++){
    after[k++] = before[j];
  }
  pthread_exit(0);
}