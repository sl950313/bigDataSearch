#ifndef _MEMORYMANAGER_H
#define _MEMORYMANAGER_H

class mMemoryManager {
private:
   long long size;
   void *beginPos;
public:
   mMemoryManager(long long _size, char *beginPos);
   ~mMemoryManager();
   void setBeginPos(void *_pos);
   void *getBeginPos();
};

#endif // _
