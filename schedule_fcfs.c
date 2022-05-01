#include  <stdio.h>
#include  <stdlib.h>
#include "list.h"
#include "cpu.h"

struct node *head;
int size = 0;
double avgWait = 0;
double turnTime = 0;
double avgRes = 0;
int currTime = 0;
int compTime = 0;

void add(char* name, int priority, int burst){
        Task *newTask = malloc(sizeof(Task));
        newTask->name = name;
        newTask->tid = size;
        newTask->priority = priority;
        newTask->burst = burst;
        insert(&head, newTask);
        size++;
}

void schedule(){
        struct node *temp;
        int i;
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
        temp = head;
        for(i = 0; i < size; i++){
                run(temp->task, temp->task->burst);
                compTime += temp->task->burst;
                currTime = compTime - temp->task->burst;
                avgWait += currTime;
                turnTime += compTime;
                temp = temp->next;
                if(i != size -1)
                        avgRes += compTime;
        }
        avgWait /= size;
        turnTime /= size;
        avgRes /= size;
        printf("\nAverage waiting time = %.2f\n", avgWait);
        printf("Average turnaround time = %.2f\n", turnTime);
        printf("Average response time = %.2f\n", avgRes);
}
