// This software may be used in your CECS326 programs

// simple implementation of SEMAPHORE class with some error 
// and signal handling

#include "semaphore.h"

SEMAPHORE::SEMAPHORE(int size) {

	this->size = size;
	semid = semget(IPC_PRIVATE, size, PERMS);
	init();
	}

int SEMAPHORE::remove() {
	semun dummy;
	return semctl(semid, 0 /*not used*/, IPC_RMID, dummy);
	}

int SEMAPHORE::P(int id){
	int retval;
	struct sembuf *p = &((pset+id)->sb);
	while(((retval=semop(semid,p,1))==-1)&&(errno==EINTR));
	return retval;
}

int SEMAPHORE::V(int id){
	int retval;
	struct sembuf *v = &((vset+id)->sb);
	while(((retval=semop(semid,v,1))==-1)&&(errno==EINTR));
	return retval;
}	

void SEMAPHORE::set_sembuf_p(int k, int op, int flg){
	(pset+k)->sb.sem_num = (short) k;
	(pset+k)->sb.sem_op = op;
	(pset+k)->sb.sem_flg = flg;
}

void SEMAPHORE::set_sembuf_v(int k, int op, int flg){
	(vset+k)->sb.sem_num = (short) k;
	(vset+k)->sb.sem_op = op;
	(vset+k)->sb.sem_flg = flg;
}

int SEMAPHORE::init() {
	pset = new mysembuf[size];
	vset = new mysembuf[size];
	for(int k=0; k<size; k++) {
		set_sembuf_p(k, -1, 0 /*suspend*/);
		set_sembuf_v(k, 1, 0 /*suspend*/);
	}
	
	// initialize all to zero
	semun arg;
	ushort initv[size];
	for(int k=0; k<size; k++) {
		initv[k]=0;
	}
	arg.array = initv;
	return semctl(semid, size, SETALL, arg);
}

SEMAPHORE::~SEMAPHORE() {
	delete pset;
	delete vset;
}

