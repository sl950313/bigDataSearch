#include <stdlib.h>
#include <stdio.h>
#include "mshm.h"

mShm::mShm(int _key, long long _bufferSize) {
   key = _key;
   bufferSize = _bufferSize;
   printf("key = %d\n", key);
   int ret = createShm();
   if (ret != 0) {
      printf("error createShm\n");
   }
   //msh = (mMemoryManager *)malloc(sizeof(mMemoryManager) );
}

mShm::~mShm() {
   delete msh;
}

mMemoryManager *mShm::getMManager() {
   return msh;
}

int mShm::createShm() {
   shmId = shmget(key, bufferSize, 0666 | IPC_CREAT);
   if (shmId < 0)  {
      perror("shmget");
      return -1;
   }
   printf("create Shm success\n");
   char *mShmPos;
   if ((mShmPos = (char *)shmat(shmId, 0, 0)) != (void *)-1) {
      perror("shmat");
      return -1;
   }
   msh = new mMemoryManager(bufferSize, mShmPos);
   return 0;
}


int mShm::deleteShm() {
   shmctl(shmId, IPC_RMID, 0);
   return 0;
}

int mShm::dtShm() {
   shmdt(msh->getBeginPos());
   return 0;
}

/*
static void *mShm::getBeginPos(int _key) {
   int shmid = shmget(_key, 2048 * 2048, 0666);
   char *mShmPos = (char *)shmat(shmid, 0, 0);
   return mShmPos;
}
*/
