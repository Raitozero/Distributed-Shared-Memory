#define TABLESIZE 100

struct node{
    int value;
    struct node* next;
};

struct LinkedList{
    struct node* head;
};

struct LinkedList hashtable[TABLESIZE];


//functions 
int hash(int key);  //returns which node this key belongs to
void initHT();
int putToHT(int key, int value); 
int getFromHT(int key);
