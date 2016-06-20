#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

class semaphore {
private:
   key_t key;
   int semId;

public:
   semaphore(const int _key);
   ~semaphore();
   int createSem();
   int setSem();
   int delSem();

   int semaphoreP();
   int semaphoreV();
};

#endif //_SEMAPHORE_H
