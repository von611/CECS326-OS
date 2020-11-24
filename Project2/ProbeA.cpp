//
//  ProbeA.cpp
//  
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <limits>
#include <string>
#include <stdlib.h>   
#include <time.h>      
#include <bits/stdc++.h>

using namespace std;


int main() {
    srand( time(0) );

    bool isExecuting = true;
    bool isReturned = false;

    // generates system wide key for the queue
    int qid = msgget(ftok(".",'u'), 0);
    cout << "QID (Probe A) is " << qid << endl;

    // declare my message buffer
    struct buf {
	long mtype; // required
	char greeting[50]; // mesg content
	char probeName[50]; // mesg content
	int id;
	char state[50];
	int validReading;
    };
	

    buf msg;
    int size = sizeof(msg)-sizeof(long);


    while (isExecuting) {

        // generate a valid random number
        int randomValue = rand();

        if (randomValue <= 50) {
                cout << "Current number: (terminates if <50)" << randomValue << endl;

                msg.mtype = 111;
                strcpy(msg.probeName, "Probe A");
	    	strcpy(msg.state, "Valid");
	    	msg.id = getpid();
	    	msg.validReading = randomValue;
                msgsnd(qid, (struct msgbuf *)&msg, size, 0); // send message to queue
                
                isExecuting = false;
                continue;
        } 
        if (randomValue % 997 == 0) {

            cout << "Probe A Sending valid reading: " << randomValue << " to DataHub"<< endl;

            // send to DataHub
            msg.mtype = 111;
            strcpy(msg.probeName, "Probe A");
	    strcpy(msg.state, "Valid");
	    msg.id = getpid();
	    msg.validReading = randomValue;
	    
            msgsnd(qid, (struct msgbuf *)&msg, size, 0); // send message to queue

            
            // wait for acknowledgement from DataHub
            while (!isReturned) {
                msgrcv(qid, (struct msgbuf *)&msg, size, 213, 0); // read incoming message
		string str(msg.probeName);
		string str2(msg.state);

                if ((str.compare("Probe B") == 0) && (str2.compare("Returned") == 0)) {
                	cout << "isReturned" << endl;
                    isReturned = true;
                    cout << "Receive from DataHub\n" << endl;
                }
            }
            isReturned = false;
        }

    }

    return 0;
}
