#include "mMemoryManager.h"
#include "stdlib.h"

mMemoryManager::mMemoryManager(long long _size, char *_beginPos) {
   size = _size;
   beginPos = _beginPos;
}

mMemoryManager::~mMemoryManager() {
}

void mMemoryManager::setBeginPos(void *_pos) {
   beginPos = _pos;
}

void *mMemoryManager::getBeginPos() {
   return beginPos;
}
