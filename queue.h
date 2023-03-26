#ifndef QUEUE
#define QUEUE

typedef struct{
    char *identifier;
    int runtime;
    int spentTime;
    long initTime;
    long completionTime;
} Task;

typedef struct node{
    Task *data;
    struct node *next;
} Node;

typedef struct{
    Node *first;
    Node *last;
    int size;
} Queue;

Task* pop(Queue*);
void insert(Queue*, Task*);
Task* getLastTask(Queue*);
Task* getFirstTask(Queue*);
Node* getFirstNode(Queue*);
void deleteQueue(Queue*);

#endif
