//
// ProbeB.cpp
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
#include "force_patch.h"

using namespace std;



int main() {
	srand(time(0));
	
	bool isExecuting = true;


	// generates system wide key for the queue
	int qid = msgget(ftok(".",'u'), 0);
	cout << "QID (Probe B) is: " << qid << endl;

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

	//cout << "Magic seed for Probe B: " << PROBE_B_SEED << endl;
	cout << "Starting Probe B: " << endl;

	int count = 0;//messages sent to the queue

	while (true) {// generate a valid random number

		int random = rand();

		if(random % 257 != 0){
			
           		cout << "Probe B Sending valid reading: " << random << " to DataHub"<< endl;
			count++;
			
			// send to DataHub
			msg.mtype = 111;
			strcpy(msg.probeName, "Probe B");
			strcpy(msg.state, "Valid");
			msg.id = getpid();
			msg.validReading = random;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0); // send message to queue
		}
	}
}
