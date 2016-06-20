#ifndef _MSG_Q_H
#define _MSG_Q_H
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

struct msgStru {
   long msgtype;
   char msgInfo[1024];
};

class mMsgQ {
private:
   key_t msgKey;
   int msgId;
   bool delMsgQ();
public:
   mMsgQ(int _msgKey);
   ~mMsgQ();
   bool getMsgQ();
   bool createMsgQ();
   bool sendMsg(msgStru *msg);
   bool recvMsg(char *buf);
};

#endif /*_MSG_Q_H */
