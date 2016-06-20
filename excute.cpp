#include "excute.h"
#include <ctype.h>
#include "mshm.h"
#include "msgQ.h"
#include "string.h"
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

//const int msgQKey = 150503;
const int mshKey = 950313;

jstring stoJstring(JNIEnv* env, const char* pat) {
   jclass strClass = env->FindClass("Ljava/lang/String;");
   jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
   jbyteArray bytes = env->NewByteArray(strlen(pat));
   env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
   jstring encoding = env->NewStringUTF("utf-8");
   return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}

JNIEXPORT jstring JNICALL Java_excute_readMsgFromMsgQ(JNIEnv *env, jobject, jint msgQKey) { 
   printf("in debug:\tin Java_excute_readMsgFromMsgQ\n");
   char *buff = (char*)malloc(sizeof(char) *2048);
   mMsgQ mq(msgQKey);
   if (mq.getMsgQ()) {
      printf("begin recvMsg\n");
      bool ret = mq.recvMsg(buff);
      /*
       * TODO:
       * wtf! why here would not go on excute! axib! the following phras must
       * be a fuck!
       *
       * How! FIX! IT!
       */
      printf("ret = %d\nbuff = %s\n", ret, buff);
      printf("recvMsg buff = %s\n", buff);
   } else {
      printf("msq is empty. It's time to sleep\n");
   }
   jstring result = stoJstring(env, (const char*)buff);
   //free(buff);
   return result;
}

JNIEXPORT void JNICALL Java_excute_writeToShmAndRecall(JNIEnv *, jobject, jint pid) {
   //mShm msh(mshKey);
   //msh.getMManager()->writeData();
   
   void *mshPos = mShm::getBeginPos(940716);
   printf("mShmPos = %ld", (long)mshPos);
   kill(pid, SIGUSR1);
}
