#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

void insert(Queue *queue, Task* task){
    Node* newNode = (Node*) malloc(sizeof(Node));
    if(!newNode){
        printf("Heap allocation failed. Task creating aborted.\n");
        free(task->identifier);
        free(task);
        return;
    }
    newNode->data = task;
    newNode->next = NULL;
    if(!queue->size){
        queue->first = newNode;
        queue->last  = newNode;
    }
    else{
        queue->last->next = newNode;
        queue->last = newNode;
    }
    queue->size++;
}

Task* pop(Queue *queue){
    if(!queue->size){
        return NULL;
    }

    Node* first = queue->first;
    queue->first = first->next;
    queue->size--;
    Task *data = first->data;
    free(first);
    return data;
}

Task* getLastTask(Queue *queue){
    if(!queue->size){
        return NULL;
    }
    return queue->last->data;
}

Task* getFirstTask(Queue *queue){
    if(!queue->size){
        return NULL;
    }
    return queue->first->data;
}

Node* getFirstNode(Queue *queue){
    if(!queue->size){
        return NULL;
    }
    return queue->first;
}

void deleteQueue(Queue *queue){
    if(!queue->size){
        return;
    }

    Node *prev = NULL;
    Node *curr = queue->first;
    for(int i = 0; i < queue->size; i++){
        prev = curr;
        curr = curr->next;
        free(prev->data->identifier);
        free(prev->data);
        free(prev);
    }

    queue->first = NULL;
    queue->last  = NULL;
    queue->size  = 0;
}
