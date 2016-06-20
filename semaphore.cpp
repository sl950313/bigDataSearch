#include "semaphore.h"
#include <stdio.h>
#include <stdlib.h>

union semun {
   int val;
   struct semid_ds *buf;
   unsigned short *array;
};

semaphore::semaphore(int _key) {
   key = _key;
}

int semaphore::createSem() {
   semId = semget(key, 1, 0666 | IPC_CREAT);
   return semId;
}

int semaphore::setSem() {
   union semun semUnion;
   semUnion.val = 1;
   if (semctl(semId, 0, SETVAL, semUnion)) {
      return 0;
   }
   return 1;
}

int semaphore::delSem() {
   union semun semUnion;
   if (semctl(semId, 0, IPC_RMID, semUnion)) {
      fprintf(stderr, "fail to del sem\n");
   }
   return 0;
}

int semaphore::semaphoreP() {
   struct sembuf semb;
   semb.sem_num = 0;
   semb.sem_op = -1;
   semb.sem_flg = SEM_UNDO;

   if (semop(semId, &semb, 1) == -1) {
      fprintf(stderr, "semaphoreP failed\n");
      return 0;
   }
   return 1;
}

int semaphore::semaphoreV() {
   struct sembuf semb;
   semb.sem_num = 0;
   semb.sem_op = 1;
   semb.sem_flg = SEM_UNDO;

   if (semop(semId, &semb, 1) == -1) {
      fprintf(stderr, "semaphoreV failed\n");
      return 0;
   }
   return 1;
}
