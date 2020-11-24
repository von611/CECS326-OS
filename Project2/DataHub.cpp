#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <bits/stdc++.h> 
#include "force_patch.h"

using namespace std;


int force_patch(pid_t pid);

int main() {

    int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);     // create message queue
    if (qid == -1) {
        qid = msgget(ftok(".",'u'), 0);
    }
    cout << "QID is " << qid << endl;

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
    int messages = 0;

    // sets running state of each probe to true
    bool pA_Running = true;
    bool pB_Running = true;
    bool pC_Running = true;

    while (pA_Running || pB_Running || pC_Running) {	//loops until all probes terminate
        msgrcv(qid, (struct msgbuf *)&msg, size, 111, 0); // read first message in queue
        messages++; // increment message in queue
      


	string str(msg.probeName);
	string str2(msg.state);
       
	cout << "Messages " << messages << endl;
        
         //if probe a is executing, receives message and outputs
        if (pA_Running && (str.compare("Probe A") == 0)) { //if probe a is executing, receives message and outputs
            if (str2.compare("Exit") == 0) {
                cout << msg.greeting << endl;
                pA_Running = false;
            } else {
                msg.mtype = 213;
                strcpy(msg.probeName, "Hub");
                strcpy(msg.state, "Returned");
                msg.id = getpid();
                cout << "Sending Return Message to Probe A: " << endl;
                msgsnd(qid, (struct msgbuf*)&msg, size, 0); // sends return message to probe a
            }
        }

        // executes condition if probe b is executing and running still

        if (pB_Running && (str.compare("Probe B") == 0)) {
        	if (messages >= 10000) {
                cout << msg.greeting << endl;

                pB_Running = false;

                force_patch(msg.id);

            } else {
                cout << "Probe B id: " << msg.id << " Valid Reading: " << msg.validReading << endl;
            }
        }

        // receive message from Probe C
        if (pC_Running && (str.compare("Probe C") == 0)) {
            if (str2.compare("Vaild") == 0) {
                cout << "Probe C id: " << msg.id << " Valid Reading: " << msg.validReading << endl;
            } else if(str2.compare("Exit") == 0) {
                cout << "Probe C Exits " << endl;
                pC_Running = false; // Toggle flag.
            }
        }
    }
    // Remove message queue
    msgctl (qid, IPC_RMID, NULL);
    exit(0);
}
