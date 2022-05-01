#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include "list.h"
#include "cpu.h"

#define QUANTUM 10

int returnId(char*);

struct node *head;
int size = 0;
double avgWait = 0;
double turnTime = 0;
double avgRes = 0;
int currTime = 0;
int compIndTime = 0;
int compTime = 0;
int initSize = 0;
int smlPri = 0;

void add(char* name, int priority, int burst){
        Task *newTask = malloc(sizeof(Task));
        newTask->name = name;
        newTask->tid = size;
        newTask->priority = priority;
        newTask->burst = burst;
        insert(&head, newTask);
        size++;
        initSize++;
}

void schedule(){
        struct node *temp;
        int i, j, k, q, b, currPri, priNum;
        struct node *curr = head;
        struct node *refHead = head;
        q = QUANTUM;
        for(k = 0; k < size - 1; k++){
                for(j = 0; j < size - k - 1; j++){
                        if(curr->task->priority < curr->next->task->priority || (curr->task->priority == curr->next->task->priority && curr->task->tid > curr->next->task->tid)){
                                Task *tempTask = curr->task;
                                curr->task = curr->next->task;
                                curr->next->task = tempTask;
                        }
                        curr = curr->next;
                }
                curr = refHead;
        }
        curr = head;
        while(curr != NULL){
                priNum = 0;
                currPri = curr->task->priority;
                temp = curr;
                if(temp->next != NULL){
                        while(temp->next->task->priority == currPri){
                                priNum++;
                                temp = temp->next;
                        }
                }
                priNum++;
                while(curr != NULL && curr->task->priority == currPri){
                        if(curr->task->burst > q && priNum > 1){
                                if(curr->next != NULL && curr->next->task->priority == curr->task->priority){
                                        Task *newTask = malloc(sizeof(Task));
                                        struct node *newNode = malloc(sizeof(struct node));
                                        newTask->name = curr->task->name;
                                        newTask->tid = size;
                                        newTask->priority = curr->task->priority;
                                        newTask->burst = curr->task->burst - q;
                                        newNode->task = newTask;
                                        newNode->next = temp->next;
                                        temp->next = newNode;
                                        size++;
                                        temp = temp->next;
                                }
                        }
                        curr = curr->next;
                }
        }

        temp = head;
        int compTimeArray[initSize];
        memset(compTimeArray, 0, initSize*sizeof(compTimeArray[0]));
        int compBool[initSize];
        memset(compBool, 0, initSize*sizeof(compBool[0]));
        for(i = 0; i < size; i++){
                if(temp->task->burst <= 10){
                        b = temp->task->burst;
                }
                else if(temp->task->burst > 10 && (temp->next == NULL || temp->task->priority > temp->next->task->priority)){
                        b = temp->task->burst;
                }
                else{
                        b = q;
                }
                run(temp->task, b);
                for(j = 0; j < initSize; j++){
                        if(compBool[j] == 0){
                                compTimeArray[j] += b;
                        }
                }

                if(temp->task->burst <= 10 || temp->next == NULL || temp->task->priority > temp->next->task->priority){
                        int x = returnId(temp->task->name);
                        compBool[x] = 1;
                }

                compIndTime += b;
                if(temp->next != NULL && temp->next->task->tid < initSize){
                        avgRes += compIndTime;
                }
                temp = temp->next;
        }
        temp = head;
        j = 0;
        int ovBurst = 0;
        for(i = 0; i < size; i++){
                if(temp->task->tid < initSize){
                        ovBurst += temp->task->burst;
                        avgWait += compTimeArray[j];
                        turnTime += compTimeArray[j];
                        j++;
                }
                temp = temp->next;
        }

        avgWait = avgWait - ovBurst;
        avgWait /= initSize;
        turnTime /= initSize;
        avgRes /= initSize;

        printf("\nAverage waiting time = %.2f\n", avgWait);
        printf("Average turnaround time = %.2f\n", turnTime);
        printf("Average response time = %.2f\n", avgRes);
}

int returnId(char* name){
        int x;
        int tid = -1;
        struct node *temp = head;
        for(x = 0; x < size && temp->task != NULL; x++){
                if(temp->task->name == name && tid == -1){
                        tid = temp->task->tid;
                }
                temp = temp->next;
        }
        return tid;
}
