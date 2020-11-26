// This software may be used in your CECS326 programs

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define PERMS S_IWUSR|S_IRUSR

class mysembuf {
public:
	struct sembuf sb;
};

// need to be checking for existing definition
union semun { 		// truncated definition
	int val; 	// value used with SETVAL
	ushort *array; 	// array of values: GETALL and SETALL
};

class SEMAPHORE {
private:
	int size;
	pid_t semid;
	mysembuf *pset, *vset;

	int init();
	void set_sembuf_p(int, int, int);
	void set_sembuf_v(int, int, int);
public:
	// create a number of semaphores denoted by size
	// precondition: size is larger than zero
	// postcondition: all semaphores are initialized to zero
	SEMAPHORE(int size);
	~SEMAPHORE();

	// deallocates all semaphores created by constructor
	// precondition: existence of SEMAPHORE object
	// postcondition: all semaphores are removed
	int remove();

	// semaphore P operation on semaphore semname
	// precondition: existence of SEMAPHORE object
	// postcondition: semaphore decrements and process may be blocked
	// return value: -1 denotes an error
	int P(int semname);

	// semaphore V operation on semaphore semname
	// precondition: existence of SEMAPHORE object
	// postcondition: semaphore increments and process may be resumed
	// return value: -1 denotes an error
	int V(int semname);
};
