#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include "list.h"
#include "cpu.h"

#define QUANTUM 10

int returnId(char*);
struct node *head;
int initSize = 0;
int size = 0;
double avgWait = 0;
double turnTime = 0;
double avgRes = 0;
int currTime = 0;
int compTime = 0;
int compIndTime = 0;

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
        int i, b, j;
        int q = QUANTUM;
        struct node *curr = head;
        struct node *prev = NULL;
        struct node *next = NULL;
        while(curr != NULL){
                next = curr->next;
                curr->next = prev;
                prev = curr;
                curr = next;
        }
        head = prev;
        curr = head;
        struct node *lastNode = head;
        while(lastNode->next != NULL){
                lastNode = lastNode->next;
        }
        while(curr != NULL){
                if(curr->task->burst > q){
                        Task *newTask = malloc(sizeof(Task));
                        struct node *newNode = malloc(sizeof(struct node));
                        newTask->name = curr->task->name;
                        newTask->tid = size;
                        newTask->priority = curr->task->priority;
                        newTask->burst = curr->task->burst - q;
                        newNode->task = newTask;
                        lastNode->next = newNode;
                        size++;
                        lastNode = lastNode->next;
                }
                curr = curr->next;
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
                else{
                        b = q;
                }
                run(temp->task, b);
                for(j = 0; j < initSize; j++){
                        if(compBool[j] == 0){
                                compTimeArray[j] += b;
                        }
                }

                if(temp->task->burst <= 10){
                        int x = returnId(temp->task->name);
                        compBool[x] = 1;
                }
                if(i < initSize){
                        compIndTime += b;
                }
                if(i < initSize -1){
                        avgRes += compIndTime;
                }
                temp = temp->next;
        }
        temp = head;
        for(i = 0; i < initSize; i++){
                avgWait += compTimeArray[i] - temp->task->burst;
                turnTime += compTimeArray[i];
                temp = temp->next;
        }
        avgWait /= initSize;
        turnTime /= initSize;
        avgRes /= initSize;
        printf("\nAverage waiting time = %.2f\n", avgWait);
        printf("Average turnaround time = %.2f\n", turnTime);
        printf("Average response time = %.2f\n", avgRes);
}

int returnId(char* name){
        int x;
        int tid;
        struct node *temp = head;
        for(x = 0; x < initSize && temp->task != NULL; x++){
                if(temp->task->name == name){
                        tid = temp->task->tid;
                }
                temp = temp->next;
        }
        return tid;
}
