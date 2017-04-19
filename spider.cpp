#include <stdio.h>    
 #include <string.h>    
 #include </usr/include/curl/curl.h>    
 #include <vector> 
#include <string>
#include <algorithm>
#include <deque>
#include <assert.h>
#include "QueueManager.h"
#include <sys/syscall.h>
#include <map>
#include <thread>
#include <unistd.h>
//#include <regex.h>
#include <iostream>
#include <fstream>
#include "common.h"
#include "boost/regex.hpp"
#include "csdn_persioninfo.h"
using namespace std;  
#define MAX_BUF (1024 * 1024 * 10)
#define MAX_THREAD 10

 
extern QueueManager Surl_queue;
extern QueueManager Our_queue;

map<int , surl> thread_url;

map<int , size_t > thread_size;

map<int , size_t > thread_sleep_num;

vector<string> reg_string;

pthread_mutex_t pt_mutex_2;

string g_host;

int all_url = 0;

char* tag = "href=\"";

void get_personinfo(char* url, int threadid);

long GetOneField(char * sTotal, const char* nPosBegin,const char * sSpan,char ** sOut) {
	char * sPos = NULL;
	long nPosEnd = 0;
	long nLen = 0;

	nLen = strlen(sTotal);
	char * B_Pos = strstr(sTotal, nPosBegin);
	if(B_Pos == NULL)
		return 0;
	
	B_Pos += strlen(nPosBegin);		
	

	sPos = strstr(B_Pos, sSpan);
	if (sPos == NULL) {
		nPosEnd = nLen;
	} else {
		nPosEnd = sPos - B_Pos;
	}
	memcpy(*sOut, B_Pos, nPosEnd);
	(*sOut)[nPosEnd] = 0;


	return nPosEnd;
}

long GetOneField(char * sTotal, const char* nPosBegin,const char * sSpan,int& iOut) {
	char * sPos = NULL;
	long nPosEnd = 0;
	long nLen = 0;

	nLen = strlen(sTotal);
	char * B_Pos = strstr(sTotal, nPosBegin);
	if(B_Pos == NULL)
		return 0;
	
	B_Pos += strlen(nPosBegin);		
	

	sPos = strstr(B_Pos, sSpan);
	if (sPos == NULL) {
		nPosEnd = nLen;
	} else {
		nPosEnd = sPos - B_Pos;
	}
	char sOut[10] = {0};
	memcpy(sOut, B_Pos, nPosEnd);
	sOut[nPosEnd] = 0;
	iOut = atoi(sOut);	

	return nPosEnd;
}
//char* reg_str = NULL; 
char* cut_url = NULL;
size_t get_url_frompage(char* wr_buf, int threadid, ostream& fout)
{
	char url[1024]={0};
	char* pos = NULL;
	char* pos_urlend = NULL;
	char* p=NULL;
	if(strlen(wr_buf) <= 0)
		return 0;
	
	char my_csdn_url[100] = {0};
	char* ptr = my_csdn_url;
	GetOneField(wr_buf, "href=\"http://my.csdn.net/", "\"", &ptr);
	if(!strstr(my_csdn_url,"class") && strlen(my_csdn_url) > 2)
	{
		
		memset(url,0,1024);
		memcpy(url,"http://my.csdn.net/",21);
		strncat(url,my_csdn_url, (strlen(my_csdn_url) > 72)?72:strlen(my_csdn_url));
		struct surl URL;
		URL.url = (string)url; 		
		URL.deep_level = 1;
		if(Our_queue.find(URL) == -1)
		{
			Our_queue.Add(URL);				
			if(Our_queue.size() > 10240)
				Our_queue.pop();
			
			get_personinfo(url,threadid);
		}
	}
		
	//cout<<"ok1"<<endl;
	pos = strstr(wr_buf, tag);
	while(pos)
	{
		pos += strlen(tag);
		//printf("content: %s\n",pos);
		pos_urlend = strstr(pos, "\"");
		
		if(pos_urlend)
		{
			//cout<<"ok2"<<endl;
			struct surl child_url;
			

			memset(url,0,1024);
			strncat(url, pos, (pos_urlend - pos)>1024?1024:(pos_urlend - pos));
			/*if(!strstr(url,"http://") || strlen(url) < 9)
			{
				pos = strstr(pos,tag);
				continue;
			}*/
		
			p =const_cast<char*>(thread_url[threadid].url.c_str());	
			//cout<<"ok3"<<endl;
			if(strstr(p,url))
			{
				pos = strstr(pos,tag);
				continue;
			}
			
			if(!strstr(url,".com") && !strstr(url,".cn"))
				child_url.url = g_host + (string)url;
			else
				child_url.url = (string)url;
			//cout<<"ok4"<<endl;
			child_url.deep_level = thread_url[threadid].deep_level + 1;
                        //std::smatch m;
		 	    pthread_mutex_lock( &pt_mutex_2 );
			    all_url++;
			    pthread_mutex_unlock( &pt_mutex_2 );
                        for(int i = 0; i < reg_string.size(); ++i)
                        {
			     //cout<<"ok5"<<endl;
                            
                            //c reg
			    /*regmatch_t pmatch[10];
			    const char* reg_str = reg_string[i].c_str(); 
                            regex_t reg;
			    const size_t nmatch= 1;
			    
			    regcomp(&reg, reg_str, REG_EXTENDED);//regex expression(rs); // 字符串传递给构造函数，建立正则表达式
                            int status = regexec(&reg, url, nmatch, pmatch,0 );
			    
			   			    
	
			    int status = regcomp(&reg, reg_str, REG_NOSUB);
			    status = regexec(&reg, url, 10, pmatch, 0);*/

			    //boost reg
			    boost::regex oRegex(reg_string[i].c_str());    
			    boost::smatch oResults;
			    std::string strRes;
                            if(boost::regex_search((string)url,oResults,oRegex))   
			    //if(strstr(url,reg_string[i].c_str()))
			    {
				if(strlen(url) > 85)
				{
					cut_url = strstr(url,"?");
					if(cut_url != NULL && (cut_url - url + 1) < 86)
					{
						url[cut_url - url + 1] = 0;
						child_url.url = (string)url;
					}
					else
					{
						continue;	
					}
				}

				int flag = Our_queue.find(child_url);
				if(flag == -1)
				{
					//printf("newurl: %s, level: %d, flag:%d\n", child_url.url.c_str(),child_url.deep_level,flag);
					/*if(Surl_queue.size() > 2000)
						cout<<"can't save:"<<child_url.url.c_str()<<","<<child_url.deep_level<<endl;
					else
					{*/		
						//Surl_queue.Add(child_url);
					Surl_queue.Save(child_url,threadid);
					//}
					Our_queue.Add(child_url);
						
					
					if(Our_queue.size() > 10240)
						Our_queue.pop();
				}	
                                	break;  
                            }
			    //regfree(&reg);
                        }
                        
                        	
                        pos = pos_urlend;
				
		}
		if(pos != NULL)	
		pos = strstr(pos,tag);
			
	}

	return 1;
	
}  
 
 /*   
 * Write data callback function (called within the context of  
 * curl_easy_perform.   
 */    

 char** dstbuf = NULL; 
size_t write_data( void *buffer, size_t size, size_t nmemb, void *userp )    
 {    
  
  if(userp == NULL) return -1;
  /* Check to see if this data exceeds the size of our buffer. If so,  
   * set the user-defined context value and return 0 to indicate a  
   * problem to curl.   
   */    
  int segsize = size * nmemb;   
  
  
  int thread_pid = syscall(SYS_gettid);
   
  if (size ==0 || nmemb == 0 || segsize + thread_size[thread_pid] > MAX_BUF ) {    
    return 0;    
  }    
 
  //printf("Threadid find: %d,%d,%d\n",syscall(SYS_gettid), getpid(),pthread_self()); 
  dstbuf = (char**)userp; 
  
  /* Copy the data from the curl buffer into our buffer */   
  memcpy(*dstbuf , buffer, (size_t)segsize );    
 
  *dstbuf += segsize;
   thread_size[thread_pid] += segsize;  
   /* Return the number of bytes received, indicating to curl that all is okay */    
  
  return segsize;    
 }    

void get_personinfo(char* url, int threadid)
{

	Persioninfo Persion;
  	CURL *curl;   
  	curl = curl_easy_init();    
	 
  	/* First step, init curl */    
  	if (!curl) {    
    		printf("couldn't init curl\n");    
    		return;   
	}


	char*  wr_buf = NULL;    
	if(wr_buf == NULL)
		wr_buf = (char*)malloc(MAX_BUF+1);
  	/* Tell curl the URL of the file we're going to retrieve */    
	 curl_easy_setopt( curl, CURLOPT_URL, url );    
  
  	/* Tell curl that we'll receive data to the function write_data, and  
   	* also provide it with a context pointer for our error return.  
   	*/    
 	
	if(wr_buf == NULL)
		return;
	memset(wr_buf, 0 ,MAX_BUF+1);
   	
  	char* ptr = wr_buf; 
  	curl_easy_setopt( curl, CURLOPT_WRITEDATA, &ptr );    
  	curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_data );    
 
	curl_easy_setopt( curl, CURLOPT_USERAGENT, "Mozilla/5.0");  
  	/* Allow curl to perform the action */    
  	int ret = curl_easy_perform( curl );

	if(strlen(wr_buf) > 0)
	{
		char* persion_t = strstr(wr_buf,"class=\"persional_property\"");
		if(persion_t != NULL)
		{
			GetOneField(persion_t, "class=\"focus_num\"><b>", "</b>", Persion.focus_num);
			
			GetOneField(persion_t, "class=\"fans_num\"><b>", "</b>", Persion.fans_num);
			char* ptr = Persion.nick_name;
			GetOneField(persion_t, "class=\"person-nick-name\"><span>", "</span>", &ptr);
			ptr = Persion.address;	
			GetOneField(persion_t, "class=\"person-detail\"> ", "</dd>", &ptr);
			ptr = Persion.person_sign; 	
			GetOneField(persion_t, "<dd class=\"person-sign\">", "</dd>", &ptr);	
			
			char* persion_contact = strstr(persion_t,"class=\"mod_contact\">");	
			if(persion_contact != NULL)
			{
			ptr = Persion.Email;
			GetOneField(persion_t, "class=\"email\">", "</span>", &ptr);
			ptr = Persion.phone;  	
			GetOneField(persion_t, "class=\"modile\">", "</span>", &ptr);
			ptr = Persion.QQ; 	
			GetOneField(persion_t, "class=\"qq\">", "</span>", &ptr);
			ptr = Persion.weixin;	
			GetOneField(persion_t, "class=\"weixin\">", "</span>", &ptr);	
			}
			//Persion.PrintInfo();	
			Surl_queue.Save_Persioninfo(Persion);	
		}
	}	   
	
	
	char * username_url = strstr(wr_buf,"class=\"mod_relations\"");
	if(username_url != NULL)
	{
		char* username_id = strstr(username_url, "username=\'");
		char username_id_c[1024*3] = {0};
		if(username_id != NULL)
		{
                strncpy(username_id_c, username_id, (strlen(username_id)>3071)?3071:strlen(username_id));
		char* username_id_cpy = username_id_c;
		while(username_id_cpy != NULL)
                {
			username_id_cpy += 10;
			char * username = strstr(username_id_cpy,"\'");
			if(username != NULL && (username - username_id_cpy) > 2)
			{
					
				memset(url,0,1024);
				memcpy(url,"http://my.csdn.net/",21);
				strncat(url,username_id_cpy, ((username - username_id_cpy) > 72)?72:(username - username_id_cpy));
				
				struct surl URL;
				URL.url = (string)url; 		
				URL.deep_level = 1;
				if(Our_queue.find(URL) == -1)
				{
					if(wr_buf != NULL)
					{
						free(wr_buf);
						wr_buf = NULL;
					}
					Our_queue.Add(URL);			
		
					if(Our_queue.size() > 10240 )
						Our_queue.pop();
					if(threadid != 3)
						get_personinfo(url, threadid);
					else
						Surl_queue.Save(URL,threadid);
						
				}
				
			}
				username_id_cpy = strstr(username, "username=\'");
		}	
		}
	}
	if(wr_buf != NULL)
		free(wr_buf);
	
	if(curl != NULL)	
  		curl_easy_cleanup( curl );  
	
}
 
void *Curl_Config(void* param)
{

 	int *id = (int*)param;
 	int threadid = *id;
 	int thread_pid = syscall(SYS_gettid);
	delete id;
	

	char filename[1024] = {0};
	sprintf(filename,"/tmp/thread_mem_%d", thread_pid);	
	ofstream fout(filename);
	
			
  	CURL *curl;    
	struct surl t_surl;
  	curl = curl_easy_init();    
  	/* First step, init curl */    
  	if (!curl) {    
    		printf("couldn't init curl\n");    
    		return 0;   
	}
	 
 	
	if(param == NULL)
		pthread_exit(NULL);

  
        
        thread_sleep_num[thread_pid] = 0;
  	
	char*  wr_buf = NULL;    
	if(wr_buf == NULL)
		wr_buf = (char*)malloc(MAX_BUF+1);
        
	while(1){	
	
	while(1){ 
		Surl_queue.update_queue();			
	
		if(!Surl_queue.empty())
		{
			t_surl = Surl_queue.pop();
			thread_sleep_num[thread_pid] = 0;	
			break;
  		}
                else 
		{
			char file_str[20][1024] = {0};
			//cmd("find . -type f -size +100M",file_str);
			
				
						
			ifstream ifile();
			
			
			if(thread_sleep_num[thread_pid] < 10)
                        {
                        printf("Thread-%d find that url quene is empty!sleep 5 minutes\n", threadid);
                        ++thread_sleep_num[thread_pid];
                        sleep(5);
                        }
                        else
                        {
                        printf("Thread-%d has been wait for a long time,exit\n", threadid);
                        pthread_exit(NULL);
                       
                        }
		
		}
	}
	
	thread_size[thread_pid] = 0;
	
	thread_url[thread_pid] = t_surl;

  	//printf("Thread-%d begin URL: %s,%d\n",threadid, t_surl.url.c_str(),Surl_queue.size());
  	//Our_queue.Add(t_surl);
  
  	/* Tell curl the URL of the file we're going to retrieve */    
	 curl_easy_setopt( curl, CURLOPT_URL, t_surl.url.c_str() );    
  
  	/* Tell curl that we'll receive data to the function write_data, and  
   	* also provide it with a context pointer for our error return.  
   	*/    
 	
	if(wr_buf == NULL)
		continue;
	memset(wr_buf, 0 ,MAX_BUF+1);
   	
  	char* ptr = wr_buf; 
  	curl_easy_setopt( curl, CURLOPT_WRITEDATA, &ptr );    
  	curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_data );    
 
	curl_easy_setopt( curl, CURLOPT_USERAGENT, "Mozilla/5.0");  
  	/* Allow curl to perform the action */    
  	int ret = curl_easy_perform( curl );    
//	exit(0);
	if(strlen(wr_buf) > 0) 
  		get_url_frompage(wr_buf, threadid , fout);
	/*if(Surl_queue.size() > 8000)
	{
		Surl_queue.Clear();
		free(wr_buf);	
		wr_buf = (char*)malloc(MAX_BUF+1);
	}*/
  		
	//printf( "Thread-%d 's ret = %d\n",threadid, ret );  
	
	ptr=NULL;
	}  
	free(wr_buf);
	
  	curl_easy_cleanup( curl );    
	pthread_exit(NULL);
} 
  
 /*   
 * Simple curl application to read the index.html file from a Web site.  
 */    
 int main( int argc, char* argv[])    
 {    
  CURLcode curl_ret;    
  
 
  struct surl first_surl; 

     if(argc<=1)
     {
	printf("please print your url!\n");
	return 0;
     }
  for(int i = 1; i < argc; ++i)
  {
     

     if(strcmp(argv[i],"-h") == 0 || strcmp(argv[i],"-help") == 0){
          cout << "Usage: "<<endl;
          cout << " -u, the first url"<<endl;
          cout << " -r, the regular expression can be used by the spider"<<endl;
          return -1;
     }

     if(strcmp(argv[i],"-u") == 0)
     {
         if(++i >= argc){
              cout << "ERROR: The argvment -f without param"<<endl;
              break;
         }
         first_surl.url = (string)argv[i];
         first_surl.deep_level = 0;
         Surl_queue.Add(first_surl);	
         //Our_queue.Add(first_surl);	
         
         continue;
        
     }
     
     if(strcmp(argv[i],"-r") == 0)
     {
         if(++i >= argc){
              cout << "ERROR: The argvment -r without param"<<endl;
              break;
         }
         char* reg_buf = argv[i]; 
         char* reg_tmp;
	 while((reg_tmp = strtok(reg_buf,",")) != NULL)
         {
		reg_string.push_back((string)reg_tmp);
             reg_buf = NULL;
         }
          
         continue;
        
     }
     
     if(strcmp(argv[i],"-host") == 0)
     {
         if(++i >= argc){
              cout << "ERROR: The argvment -r without param"<<endl;
              break;
         }
         
          
         g_host = (string)argv[i];
         continue;
        
     }
  }
  
  
 
  curl_ret = curl_global_init(CURL_GLOBAL_SSL);
  if(curl_ret != CURLE_OK) 
	return -1; 

  pthread_t spider_threads[MAX_THREAD];
 
pthread_mutex_init( &pt_mutex_2, NULL );
 for(unsigned int i = 0; i < MAX_THREAD;++i)
  { 
  	int *id = new int(i);
	
        pthread_create(&spider_threads[i] , NULL, Curl_Config, (void*)id);
  }

  for(unsigned int i = 0; i < MAX_THREAD; ++i )
	pthread_join(spider_threads[i], NULL);


  /* Emit the page if curl indicates that no errors occurred */    
  curl_global_cleanup;  
  
  return 0;    
 }

  
