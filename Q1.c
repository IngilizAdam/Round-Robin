#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

//#define DEBUG

#define ARG_ROW 5
#define ARG_COL 100

void readInputLine(char*);
int  parseInput(char*, char*);
void printArgs(int, char*);
void processCommand(int, char*);
void startNewTask(char*, int);
void simulateK(int);
void displaySystemTime();
void displayTotalIdleTime();
void displayMostRecentlyCompletedTask();
void dumpCompletedToFile(char*);
void deleteCompletedQueue();
void displayFirstTask();

Queue waitingQueue, completedQueue;
long systemTime = 0, idleTime = 0;

int main(){
    printf("Creating queues...\n");
    waitingQueue.size = 0;
    completedQueue.size = 0;
    printf("Done.\n");
    printf("Allocating heap memory for command strings...\n");
    char *input = (char*) malloc(ARG_COL*ARG_ROW*sizeof(char));
    char *arguments = (char*) malloc(ARG_ROW*ARG_COL*sizeof(char));
    if(!input || !arguments){
        printf("Couldn't allocate heap memory. Exiting...\n");
        free(input);
        free(arguments);
        exit(1);
    }
    printf("Done.\n");
    printf("Welcome!\n");
    while(1){
        printf("\nroot@the-pearl:~$ ");
        readInputLine(input);

        #ifdef DEBUG
        printf("Input: %s\n", input);
        #endif

        int argCount = parseInput(arguments, input);

        #ifdef DEBUG
        printArgs(argCount, arguments);
        #endif

        if(!strcmp(arguments, "QUIT") && argCount == 1){
            printf("Quitting...\n");
            free(input);
            free(arguments);
            deleteQueue(&waitingQueue);
            deleteQueue(&completedQueue);
            exit(0);
        }

        processCommand(argCount, arguments);
    }
}

void readInputLine(char *ptr){
    fgets(ptr, ARG_COL*ARG_ROW, stdin);
    *(ptr + strlen(ptr) - 1) = '\0';
}

int parseInput(char *arguments, char *input){
    int argCount = 0;
    int readingWord = 0;
    int currentLetter = 0;
    for(int i = 0; i < strlen(input); i++){
        char c = *(input+i);
        if(readingWord){
            if(c == ' '){
                readingWord = 0;
                *(arguments+(ARG_COL*(argCount-1))+currentLetter) = '\0';
            }
            else{
                *(arguments+(ARG_COL*(argCount-1))+currentLetter) = c;
                currentLetter++;

                if(i == (strlen(input)-1)){
                    *(arguments+(ARG_COL*(argCount-1))+currentLetter) = '\0';
                }
            }
        }
        else{
            if(c != ' '){
                readingWord = 1;
                argCount++;
                *(arguments+(ARG_COL*(argCount-1))) = c;
                currentLetter = 1;

                if(i == (strlen(input)-1)){
                    *(arguments+(ARG_COL*(argCount-1))+currentLetter) = '\0';
                }
            }
        }
    }

    return argCount;
}

// used for debug purposes
void printArgs(int argCount, char *arguments){
    for(int i = 0; i < argCount; i++){
        for(int j = 0; j < ARG_COL; j++){
            if(*(arguments+(ARG_COL*i)+j) == '\0'){
                break;
            }
            printf("%c", *(arguments+(ARG_COL*i)+j));
        }
        printf("\n");
    }
}

void processCommand(int argCount, char *arguments){
    char *command = arguments;
    if(!strcmp(command, "SNT")){
        if(argCount == 3){
            char *fileName = arguments+(ARG_COL*1);
            char *runtime = arguments+(ARG_COL*2);
            startNewTask(fileName, strToInt(runtime));
        }
        else{
            printf("Invalid arguments for command SNT. Usage: SNT <task_name> <runtime>\n");
        }
    }
    else if(!strcmp(command, "S")){
        if(argCount == 2){
            char *k = arguments+(ARG_COL*1);
            simulateK(strToInt(k));
        }
        else{
            printf("Invalid arguments for command S. Usage: S <k>\n");
        }
    }
    else if(!strcmp(command, "DST")){
        if(argCount == 1){
            displaySystemTime();
        }
        else{
            printf("Invalid arguments for command DST. Usage: DST\n");
        }
    }
    else if(!strcmp(command, "DTIT")){
        if(argCount == 1){
            displayTotalIdleTime();
        }
        else{
            printf("Invalid arguments for command DTIT. Usage: DTIT\n");
        }
    }
    else if(!strcmp(command, "DIMRCT")){
        if(argCount == 1){
            displayMostRecentlyCompletedTask();
        }
        else{
            printf("Invalid arguments for command DIMRCT. Usage: DIMRCT\n");
        }
    }
    else if(!strcmp(command, "DCTQ")){
        if(argCount == 2){
            char *fileName = arguments+(ARG_COL*1);
            dumpCompletedToFile(fileName);
        }
        else{
            printf("Invalid arguments for command DCTQ. Usage: DCTQ <file_name>\n");
        }
    }
    else if(!strcmp(command, "ECTS")){
        if(argCount == 1){
            deleteCompletedQueue();
        }
        else{
            printf("Invalid arguments for command ECTS. Usage: ECTS\n");
        }
    }
    else{
        printf("Invalid command.\n");
    }
}

void startNewTask(char *taskName, int runtime){
    printf("Creating new task...\n");
    Task *newTask = (Task*) malloc(sizeof(Task));
    if(!newTask){
        printf("Heap allocation failed.\n");
        return;
    }
    newTask->identifier = (char*) malloc((strlen(taskName)+1)*sizeof(char));
    if(!newTask->identifier){
        printf("Heap allocation failed.\n");
        free(newTask);
        return;
    }
    for(int i = 0; i < strlen(taskName)+1; i++){
        *(newTask->identifier+i) = *(taskName+i);
    }
    newTask->runtime = runtime;
    newTask->spentTime = 0;
    newTask->initTime = systemTime;
    newTask->completionTime = -1;

    insert(&waitingQueue, newTask);
    printf("Task created:\n");
    printf("Task Name: %s, Runtime: %d\n", taskName, runtime);
}

void simulateK(int k){
    for(int i = 0; i < k; i++){
        Task* task = pop(&waitingQueue);
        if(task == NULL){
            printf("Waiting task list is empty.\n");
            idleTime++;
            return;
        }
        systemTime++;

        task->spentTime++;
        if(task->spentTime >= task->runtime){
            task->completionTime = systemTime;
            insert(&completedQueue, task);
        }
        else{
            insert(&waitingQueue, task);
        }
    }
}

void displaySystemTime(){
    printf("System Time: %ld\n", systemTime);
}

void displayTotalIdleTime(){
    printf("Total Idle Time: %ld\n", idleTime);
}

void displayMostRecentlyCompletedTask(){
    Task* task = getLastTask(&completedQueue);
    if(task != NULL){
        printf("Most Recently Completed Task:\n");
        printf("Task Name:       %s\n", task->identifier);
        printf("Runtime:         %d\n", task->runtime);
        printf("Elapsed Time:    %ld\n", task->completionTime-task->initTime);
        printf("Completion Time: %ld\n", task->completionTime);
    }
    else{
        printf("There are no completed tasks.\n");
    }
}

void dumpCompletedToFile(char *fileName){
    FILE *out = fopen(fileName, "w");
    if(!out){
        printf("Failed to create file %s\n", fileName);
        return;
    }
    fprintf(out, "identifier, runtime, elapsedTime, completionTime:\n");

    Node *curr = getFirstNode(&completedQueue);
    while(curr){
        fprintf(out, "%s, %d, %ld, %ld\n", curr->data->identifier, curr->data->runtime, curr->data->completionTime-curr->data->initTime, curr->data->completionTime);
        curr = curr->next;
    }

    fclose(out);

    printf("Completed tasks queue dumped into %s\n", fileName);
}

void deleteCompletedQueue(){
    deleteQueue(&completedQueue);
    printf("Completed tasks queue has been deleted.\n");
}

// used for debug purposes
void displayFirstTask(){
    Task* task = getFirstTask(&waitingQueue);
    if(task){
        printf("%s, %d/%d", task->identifier, task->spentTime, task->runtime);
    }
    else{
        printf("Queue empty.\n");
    }
}
