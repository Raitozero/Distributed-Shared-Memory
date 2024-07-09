#include "hashtable.h"
#include "util.h"
#include <unistd.h>

//hash table
int hash(int key){
    return key % NUM_Machine;
}

void initHT(){
    for(int i = 0; i < TABLESIZE; i++){
        hashtable[i].head = NULL;
    }
}

int putToHT(int key, int value){
    if(key < 0 || key >= TABLESIZE || hash(key) % NUM_Machine != curr){
        printf("put to hashtable failed.\n");
        return 0;
    }
    int index = key/NUM_Machine - 1;
    if(hashtable[index].head == NULL){
        hashtable[index].head = (struct node*) malloc(sizeof(struct node));
        hashtable[index].head->value = value;
    }
    else{
        struct node* tmp = hashtable[index].head;
        while(tmp->next){
            tmp = tmp->next;
            tmp->next = (struct node*) malloc(sizeof(struct node));
            tmp->next->value = value;
        }
    }
    return 1;
}

int getFromHT(int key){
	int index = key/NUM_Machine - 1;
	if(key < 0 || key > TABLESIZE || hashtable[index].head == NULL){
        printf("get from hashtable failed.\n");
    }
	else {
        struct node* tmp = hashtable[index].head;
        while(tmp->next) tmp = tmp->next;
        return tmp->value;
   	}   
}

