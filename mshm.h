#ifndef _MSHM_H
#define _MSHM_H
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "mMemoryManager.h"
#include "const.h"

class mShm {
private:
   key_t key;
   int shmId;
   int bufferSize;
   mMemoryManager *msh;
public:
   mShm(int _key, long long _bufferSize);
   ~mShm();
   mMemoryManager *getMManager();
   int createShm();
   int deleteShm();
   int dtShm();
   mMemoryManager *getSelf(int key);
   //void *getBeginPos();
   static void *getBeginPos(int _key) {
      int shmid = shmget(_key, 2048 * 2048, 0666);
      char *mShmPos = (char *)shmat(shmid, 0, 0);
      return mShmPos;
   }
};

#endif // _MSHM_H
