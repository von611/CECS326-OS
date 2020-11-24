#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>      
#include <iostream>
#include "semaphore.h"
using namespace std;

// header to be included with your implementation
// shipment is an array containing 5 integer elements

// return value contains the new shipment of:
// 	overgrips, strings, shoes, balls, accessories, 
//	respectively (to be added to current stock)
int *shipment_arrival(int shipment[]);

enum {MUTEX};
int main() {
    srand(time(0));

    int shmid;
	char *shmBUF;
    long childPID;
    key_t key = ftok(".", 69);

    SEMAPHORE sem(1); //initialize MUTEX

    //increment each semaphore to 1 to allow the child processes to use
    /*sem.V(Sem1);
    sem.V(Sem2);
    sem.V(Sem3);
    sem.V(Sem4);
    sem.V(Sem5);*/
    sem.V(MUTEX);

    
    shmid = shmget(key, sizeof(int), 0600 | IPC_CREAT);
    cout << "Shmid: " << shmid << endl;

    int *shipment;
    shipment = (int*)shmat(shmid, NULL, 0); //pointer to shared memory and cast to int pointer

    shipment[0] = 1;
    shipment[1] = 1;
    shipment[2] = 1;
    shipment[3] = 1;

    for(int i = 0; i < 10; i++) {   //create 10 child processes for each customer
        childPID = fork();
        if(childPID == 0) { //execute child process/ customer

            for(int j = 0; j < 100; j++) {  //iterate 100 times for each order
                cout << "childPID: " << childPID << " PID: " << getpid() << " Parent PID: " << getppid() << endl;
                //place order for each item
                int og = 1;     //1 item for over grip
                int rs = rand() % 2;    //zero to one for racket strings
                int ts = rand() % 2;    //zero to one for tennis shoes
                int tb = rand() % 2; //zero to one for tennis balls
                int ta = rand() % 2; //zero to one for tennis accessories
                cout << "Placing order \n";
                
                //sem.P(MUTEX);  //decrement the semaphore and block any process trying to order an overgrip
                sem.P(MUTEX);    //halts iorder if inventory is not stocked 
                shipment[0] = shipment[0] - og; //decrements 1 order of overgrip in shared inventory
                if(rs == 1) {    //if customer is trying to place an order for any racket strings
                    //sem.P(Sem2);    //halts if there are none
                    shipment[1] = shipment[1] - rs; //decrement 1 order of racket string in shared inventory
                }
                if(ts == 1) {    //if customer is trying to place an order for any tennis shoes
                    //sem.P(Sem3);    //halts if there are none
                    shipment[2] = shipment[2] - ts; //decrement 1 order of tennis shoes in shared inventory
                }
                if(tb == 1) {    //if customer is trying to place an order for any tennis balls
                    //sem.P(Sem4);    //halts if there are none
                    shipment[3] = shipment[3] - tb; //decrement 1 order of tennis balls in shared inventory
                }
                if(ta == 1) {    //if customer is trying to place an order for any tennis accessories
                    //sem.P(Sem5);    //halts if there are none
                    shipment[4] = shipment[4] - ta; //decrement 1 order of tennis accessories in shared inventory
                }
                cout << "Order Shipped! Shipped: " << og << " Over grip, " << rs << " Racket String, " << ts << " Tennis Shoe, " << tb << " Tennis Ball, " << ta << " Tennis Accessory \n"; 
                sem.V(MUTEX);
            }

        }

    }
    childPID = fork();  //create child process for shipment receiver to restock inventory
    if(childPID == 0) {
        while(true) {
            cout << "Shipment Arriving... \n";
            sleep(500); //delay each shipment
            cout << "childPID: " << childPID << " PID: " << getpid() << " Parent: " << getppid() << endl;

            cout << "Restocking Inventory \n";
            shipment_arrival((int*)shipment);
            sem.V(MUTEX);   //resume a customers order since the inventory has
        }
    }
    exit(0);
}

/**
*creates temporary array to stock each item
**/
int *shipment_arrival(int shipment[]) {
    int *temp = new int[5];
    temp[0] = shipment[0] + 1;
    temp[1] = shipment[1] + 1;
    temp[2] = shipment[2] + 1;
    temp[3] = shipment[3] + 1;
    return temp;
}