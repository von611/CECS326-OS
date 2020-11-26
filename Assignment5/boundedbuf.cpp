#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaphore.h"

const int MAXCHAR = 10;
const int BUFFSIZE = 3;
enum {PUT_ITEM, TAKE_ITEM}; // set up names of my 2 semaphores

void producer_proc(SEMAPHORE &, char *);
void parent_cleanup(SEMAPHORE &, int);
void consumer_proc(SEMAPHORE &, char *);

int main(){
	int shmid;
	char *shmBUF;

	SEMAPHORE sem(2);
	sem.V(PUT_ITEM);
	sem.V(PUT_ITEM);
	sem.V(PUT_ITEM);

	shmid = shmget(IPC_PRIVATE, BUFFSIZE*sizeof(char), PERMS);
	shmBUF = (char *)shmat(shmid, 0, SHM_RND);

	if(fork()){ /* parent process */

		producer_proc(sem, shmBUF);
		parent_cleanup(sem, shmid);

	} else { // child process
		consumer_proc(sem, shmBUF);
	}

	exit(0);
} // main

void consumer_proc(SEMAPHORE &sem, char *shmBUF) {
	char tmp;

	for(int k=0; k<MAXCHAR; k++){
		sem.P(TAKE_ITEM);
		tmp = *(shmBUF+k%BUFFSIZE);
		sem.V(PUT_ITEM);
		cout << "(" << getpid() << ")  " 
				<< "buf[" << k%BUFFSIZE << "] "
				<< tmp << endl;
	}
} // child_proc

void producer_proc(SEMAPHORE &sem, char *shmBUF) {

	char data[128];
	cout << "(" << getpid() << ")  Please enter a string --> ";
	cin.getline(data, 127);

	char input;
	for(int k=0; k<MAXCHAR; k++){
		input = data[k];
		sem.P(PUT_ITEM);
		*(shmBUF+(k%BUFFSIZE)) = input;
		sem.V(TAKE_ITEM);
	}
} // parent_proc

void parent_cleanup (SEMAPHORE &sem, int shmid) {

	int status;			/* child status */
	wait(0);	/* wait for child to exit */
	shmctl(shmid, IPC_RMID, NULL);	/* cleaning up */
	sem.remove();
} // parent_cleanup

