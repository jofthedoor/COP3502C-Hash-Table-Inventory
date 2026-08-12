/* COP 3502C Assignment 6
This program is written by: Juan de Lapuerta */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTLEN 29
#define TABLESIZE 300007

typedef struct merchandise {
    char name[MAXSTLEN + 1]; // e.g., "braziljersey"
    int quantity; // stock available
    int price; // sale price per item
} merchandise;

typedef struct inventoryNode {
    merchandise* merchPtr;
    struct inventoryNode* next;
} inventoryNode;

typedef struct inventoryTable { //Hash table
    inventoryNode** lists;
    int size; // number of buckets (table size)
} inventoryTable;


//all the cash and complexity
int totalcash = 100000;
int totalcomplexity = 0;

//hash function
int hashfunction(char* word, int size) {
    int len = strlen(word);
    int hvalue = 0;
    for (int i=0; i<len; i++)
        hvalue = (1151*hvalue + (word[i]-'a'))%size;
    return hvalue;
}

// this will insert a node at the front of the linked list
inventoryNode* insert_front(inventoryNode* front, merchandise* item) {
	inventoryNode* temp = malloc(sizeof(inventoryNode));
	temp->merchPtr = item;
	temp->next = front;
	return temp;
}
//looks for node who has an item in it that you are looking for while also updating complexity 
inventoryNode* search(inventoryNode* front, char name[], int* complexity) {
	*complexity = 0;
	while (front) {
		(*complexity)++;
		if (strcmp(front->merchPtr->name, name) == 0)
			return front;
		front = front->next;
	}
	return NULL;
}
//adding quanity to item or create item if not exist
void buy(inventoryTable* table, char* name, int quantity, int totalprice) {
	int i = hashfunction(name, table->size);
	int complexity = 0;
    //find node
	inventoryNode* found = search(table->lists[i], name, &complexity);
    totalcash -= totalprice;
    //if exist then update quantity and complezity
	if (found) {
		found->merchPtr->quantity += quantity;
		totalcomplexity += complexity;
		printf("%s %d %d\n", name, found->merchPtr->quantity, totalcash);
	}// if it doesnt exist then create a new node and add it to the linked list
    else {
		merchandise* temp2 = malloc(sizeof(merchandise));
		strcpy(temp2->name, name);
		temp2->quantity = quantity;
		temp2->price = 0;
		table->lists[i] = insert_front(table->lists[i], temp2);
        totalcomplexity += complexity +1;
        printf("%s %d %d\n", name, temp2->quantity, totalcash);
	}

}
//selling an item
void sell(inventoryTable* table, char* name, int quantity) {
	int complexity;
	int i = hashfunction(name, table->size);
    //find the node
	inventoryNode* found = search(table->lists[i], name, &complexity);
	totalcomplexity += complexity;
	int amount = 0;
    //if we are trying to sell more than we sell everything
	if (quantity > found->merchPtr->quantity) {
		quantity = found->merchPtr->quantity;
	}
    //update uanity and total cash
	found->merchPtr->quantity -= quantity;
	totalcash += quantity * found->merchPtr->price;
    //print 
	printf("%s %d %d\n", name, found->merchPtr->quantity, totalcash);

}
// changing the price of an item
void change(inventoryTable* table, char* name, int newprice) {
	int complexity;
	int i = hashfunction(name, table->size);
    //fuind the node of the tiem
	inventoryNode* found = search(table->lists[i], name, &complexity);
	totalcomplexity += complexity;
    //update the price 
	if (found) {
		found->merchPtr->price = newprice;
	}
}
//freeing a nodes liked list
void dellist(struct inventoryNode *p) {
	if (p != NULL) {
		dellist(p->next);
        free(p->merchPtr);
		free(p);
	}
}
//freeing a tables nodes
void killTable(struct inventoryTable *h) {
	for (int i = 0; i < TABLESIZE; i++) {
		dellist(h->lists[i]);
	}
}
// initializing a table
void initTable(struct inventoryTable *h) {
    for (int i = 0; i < TABLESIZE; i++) {
        h->lists[i] = NULL;
    }
}
// main
int main() {
	int n;
	scanf("%d", &n);
    //creates the table
	inventoryTable* table = malloc(sizeof(inventoryTable));
	table->size = TABLESIZE;
	table->lists = malloc(sizeof(inventoryNode*) * TABLESIZE);
	initTable(table);
    //runs comands
	for (int i = 0; i < n; i++) {
		char command[20];
		char item[MAXSTLEN + 1];
		int quantity;
		int totalprice;
		int newprice;

		scanf("%s", command);
        // if you want to buy
		if (strcmp(command, "buy") == 0) {
			scanf("%s %d %d", item, &quantity, &totalprice);
			buy(table, item, quantity, totalprice);
        //if you want to sell
		} else if (strcmp(command, "sell") == 0) {
			scanf("%s %d", item, &quantity);
			sell(table, item, quantity);
        //if you want to change price 
		} else if (strcmp(command, "change_price") == 0) {
			scanf("%s %d", item, &newprice);
			change(table, item, newprice);
		}
	}

	printf("%d\n", totalcash);
	printf("%d\n", totalcomplexity);

	killTable(table);
    free(table->lists);
	free(table);
	return 0;
}
