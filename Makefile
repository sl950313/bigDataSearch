LIBS=-lcppcms -lbooster

all: chat

chat: chat.o msgQ.o mshm.o semaphore.o mMemoryManager.o
	$(CXX) $(CXXFLAGS) -O2 -Wunused-result -Wall -g chat.o msgQ.o mshm.o semaphore.o mMemoryManager.o -o chat ${LIBS}

chat.o:chat.cpp
	$(CXX) -c chat.cpp

msgQ.o:msgQ.cpp msgQ.h
	$(CXX) -c msgQ.cpp

mshm.o:mshm.cpp mshm.h
	$(CXX) -c mshm.cpp

semaphore.o:semaphore.cpp semaphore.h
	$(CXX) -c semaphore.cpp

mMemoryManager.o:mMemoryManager.cpp mMemoryManager.h
	$(CXX) -c mMemoryManager.cpp

clean:
	rm -fr *.o chat chat.exe cppcms_rundir
