///////////////////////////////////////////////////////////////////////////////
//                                                                             
//  Copyright (C) 2008-2012  Artyom Beilis (Tonkikh) <artyomtnk@yahoo.com>     
//                                                                             
//  See accompanying file COPYING.TXT file for licensing details.
//
///////////////////////////////////////////////////////////////////////////////
#include <cppcms/application.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/http_request.h>
#include <cppcms/http_context.h>
#include <booster/intrusive_ptr.h>
#include <cppcms/json.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

#include <string>
#include <set>
#include <sstream>

#include <cstdio>
#include <ctime>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <uuid/uuid.h>

#include "semaphore.h"
#include "mshm.h"
#include "msgQ.h"
#include "const.h"

const int KEY = 950313;
const int shmKEY = 940716;
const int msgQKEY = 150503;


#ifdef USE_STD_TR1_BIND
  #include <tr1/functional>
  using std::tr1::bind;
#elif defined USE_STD_BIND
  #include <functional>
  using std::bind;
#else
  #include <boost/bind.hpp>
  using boost::bind;
#endif


class search : public cppcms::application {
private:
   mShm *mshm;
   mMsgQ *msgQ;
public:
   search(cppcms::service &srv) : cppcms::application(srv)
   {
      /*
         dispatcher().assign("/post",&search::post,this);
         dispatcher().assign("/get/(\\d+)",&search::get,this,1);
         dispatcher().assign(".*",&search::redirect,this);
       */
      dispatcher().assign("/search", &search::Search, this);

      /*
       * create shm.
       */ 
      mshm = new mShm(shmKEY, 2048 * 2048); 
      msgQ = new mMsgQ(msgQKEY);
   }
   ~search() {
      delete mshm;
   }

     /*
      *@shilei
      *
      */
     void getTime(const char *date, long &dateBegin, long &dateEnd) {
        struct tm stm;
        int iY, iM, iD, iH = 0, iMin = 0, iS = 0;  
        iY = atoi(date);
        iM = atoi(date + 5);
        iD = atoi(date + 8);
        stm.tm_year = iY - 1900;
        stm.tm_mon = iM - 1;
        stm.tm_mday = iD;
        stm.tm_hour = iH;
        stm.tm_min = iMin;
        stm.tm_sec = iS;
        dateBegin = mktime(&stm);
        
        stm.tm_hour = 23;
        stm.tm_min = 59;
        stm.tm_sec = 59;
        dateEnd = mktime(&stm);
     }
     static unsigned long hash(const char *str) {
        unsigned long hash = 5381;
        int c;

        while ((c = *str++) != '\0') {
           hash = ((hash << 5) + hash) + c;
        }
        return hash;
     }
     void Search() {
        std::cout << "in search" << std::endl; 

        cppcms::json::value result[50];
        int result_len = 0;
        //const char *filed[] = {"USERID", "UNIXTIME", "UNIXTIME_END", "LOCATION", "LOCATION_DETAIL", "LATITUDE", "LONGITUDE", "_FROM"};
        //memset(result, 0, 128 * 40);
        if(request().request_method()=="POST") { 
           std::cout << "here" << std::endl; //>>>>>
           std::string userId = request().post("UserID");
           std::string date = request().post("date");
           std::cout << "userId = " << userId << std::endl;//>>>>
           std::cout << "date = " << date << std::endl;//>>>>
           long dateBegin = 0, dateEnd = 0;
           getTime(date.c_str(), dateBegin, dateEnd);
           std::cout << "dateBegin = " << dateBegin << std::endl;//>>>>
           std::cout << "dateEnd = " << dateEnd << std::endl;//>>>>


           char sql[256];
           sprintf(sql, "SELECT * FROM STU_INFO WHERE USER_ID = \"%s\"  and UNIXTIME >= %ld and UNIXTIME_END <=  %ld", userId.c_str(), dateBegin, dateEnd);
           std::cout << "sql = " << sql << std::endl;//>>>>


           /*
            * here run spark-submit [args].
            * method:
            *  fork() a child process to do spark-submit and get return value from child.
            * TODO
            */

           /*
            * we send a signal to scala service and wait for signal to read data from shared memory.
            */ 
           // here we should use uuid_genneral().
           //long long uuid = (long long)getpid();
           boost::uuids::random_generator rgen;
           boost::uuids::uuid uuid = rgen();
           std::string str = boost::lexical_cast<std::string>(uuid); 
           unsigned long long mask = hash(str.c_str());
           printf("DEBUG:mask = %lld\n", mask);

           msgStru msg;
           msg.msgtype = 100;
           char tmp[64];
           sprintf(tmp, "%lld", mask);
           strcpy(msg.msgInfo, tmp);
           strcat(msg.msgInfo, sql);
           printf("msg.msgInfo = %s\n", msg.msgInfo);
           msgQ->sendMsg(&msg);

           //signal(SIGUSR1, mshm->DataFromShm(uuid));
           //semaphore sem(mask);
           //sem.createSem();
           //sem.semaphoreP(); 
           /*
            * get data from shared memory by scala service write.
            */
           // char *_res = mShm.msh->getDataFromShm(uuid);
           //sem.semaphoreV();


           //sprintf(param, "--class search --master spark://114.214.169.173:7077 --executor-memory 2g --num-executors 3 /root/sl/workspace/search/src/search.jar '%s'", sql);
           for (int i = 0; i < result_len; ++i) {
              response().out() << result[i];
           }
        }
     }
};


int main(int argc,char **argv)
{
try {
   cppcms::service service(argc,argv);
   booster::intrusive_ptr<search> c=new search(service);
   service.applications_pool().mount(c);
   service.run();
}
catch(std::exception const &e) {
   std::cerr<<"Catched exception: "<<e.what()<<std::endl;
   return 1;
}
return 0;
}
