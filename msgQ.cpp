#include "msgQ.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

mMsgQ::mMsgQ(int _msgKey) {
   msgKey = _msgKey;
   createMsgQ();
}

bool mMsgQ::createMsgQ() {
   msgId = msgget(msgKey, IPC_CREAT | 0666);
   if (msgId < 0) {
      printf("error to create msgQ, errno = %d\n", errno);
      return false;
   }
   printf("msgQ create success\n");
   return true;
}

int mMsgQ::getMsgQ() {
   return 0;
}

bool mMsgQ::recvMsg(char *buf) {
   int msgId = msgget(msgKey, IPC_EXCL);
   if (!msgId) {
      printf("msq not exist\n");
      return false;
   }
   msgStru msg;
   msgrcv(msgId, &msg, sizeof(msgStru), 0, 0);
   strcpy(buf, msg.msgInfo);
   return true;
}

bool mMsgQ::sendMsg(msgStru *msg) {
   msgStru msgTmp;
   msgTmp.msgtype = msg->msgtype;
   strcpy(msgTmp.msgInfo, msg->msgInfo);
   int ret = msgsnd(msgId, msg, sizeof(msgStru), IPC_NOWAIT);
   if (ret < 0) { 
      printf("msgsnd() error\n");
      return false;
   }
   return true;
}

mMsgQ::~mMsgQ() {
   delMsgQ();
}

bool mMsgQ::delMsgQ() {
   int ret = msgctl(msgId, IPC_RMID, 0);
   if (ret) { 
      printf("delMsgQ() error\n");
      return false;
   }
   return true;
}
