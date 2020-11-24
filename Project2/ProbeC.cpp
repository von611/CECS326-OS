
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <string>
#include "kill_patch.h"

using namespace std;
const int MTYPE = 111;

void kill_patch(int, struct msgbuf *, int, long);

int main() {

    srand(time(0)); // Seed for better random values

    // Message buffer struct
    struct buf {
	long mtype; // required
	char greeting[50]; // mesg content
	char probeName[50]; // mesg content
	int id;
	char state[50];
	int validReading;
	};

    // retrieve key id of message queue
    int qid = msgget(ftok(".",'u'), 0);
    cout << "QID (Probe C) is " << qid << endl;


    buf msg;

    int size = sizeof(msg)-sizeof(long);

    // Set parameters for kill patch
    msg.mtype = 111; // Set to mtype for exit



    strcpy(msg.probeName, "Probe C");
    strcpy(msg.state, "Valid");
    msg.id = getpid();
    // Call kill_patch.h function.
    kill_patch(qid, (struct msgbuf *)&msg, size, MTYPE);

    cout << "Probe C: Generating valid reading" << endl;



    while (true) {
        // Valid Reading
        int randomValue = rand();
        msg.validReading = randomValue;
     
        if (randomValue % 251 == 0) { // random integer is divisible by seed.
        strcpy(msg.probeName, "Probe C");
	strcpy(msg.state, "Valid");
	msg.id = getpid();
	msg.validReading = randomValue;
            cout << "Probe C: " << getpid() << " Sending valid reading: " << random << " to DataHub"<< endl;
            
            // send to DataHub
            msg.mtype = 111; // global mtype

            msgsnd(qid, (struct msgbuf*)&msg, size, 0); // Send message to queue
        }
    }
    
    
    exit(0);
}
