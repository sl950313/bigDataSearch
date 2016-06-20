#include "excute.h"
#include <ctype.h>
#include "mshm.h"
#include "msgQ.h"
#include "string.h"
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

const int msgQKey = 150503;
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
   char buff[2048];
   mMsgQ mq(msgQKey);
   mq.recvMsg(buff);
   jstring result = stoJstring(env, (const char*)buff);
   return result;
}

JNIEXPORT void JNICALL Java_excute_writeToShmAndRecall(JNIEnv *, jobject, jint pid) {
   //mShm msh(mshKey);
   //msh.getMManager()->writeData();
   
   void *mshPos = mShm::getBeginPos(940716);
   printf("mShmPos = %ld", (long)mshPos);
   kill(pid, SIGUSR1);
}
