#ifndef _QUEUEMANAGER_H_
#define _QUEUEMANAGER_H_

#include <string>
#include <pthread.h>
#include <deque>
#include "easy_mysql.h"
#include "csdn_persioninfo.h"

using EncapMysql::CEncapMysql;
using namespace std;

typedef struct surl{
	string url;
	size_t deep_level;
}Surl;


class QueueManager
{
public:
	QueueManager()
	{
		pthread_mutex_init( &pt_mutex, NULL );
		con = new CEncapMysql;		
    		con->Connect("127.0.0.1", "root", "676892","Spider_Quene");
	}
		
	~QueueManager()
	{
		Clear();
		delete con;
		pthread_mutex_destroy( &pt_mutex ); 
	}
	
	int find(surl t_surl)
	{
		
		 pthread_mutex_lock( &pt_mutex );
		 deque<surl>::iterator iter = url_queue.begin(); 	
		 int flag = -1;
		 while(iter != url_queue.end())
		 {
			++flag;
			//cout<<"surl:"<<((surl)(*iter)).url<<","<<(string)t_surl.url<<","<<flag<<endl;
			if(((surl)(*iter)).url == (string)t_surl.url)
			{
				//return flag;
				break;
			}	
			++iter;
		 }		  
		
		if(iter == url_queue.end()) 
			flag = -1;
		pthread_mutex_unlock( &pt_mutex );
		 
		
		 return flag; 		
	}
	
	bool Clear()
	{
		pthread_mutex_lock( &pt_mutex );
		while(url_queue.size() > 10)
			url_queue.pop_front();
		
		pthread_mutex_unlock( &pt_mutex );
		return true;
	}	

	size_t size()
	{
		return url_queue.size();
	}
	
	bool empty()
	{
		 pthread_mutex_lock( &pt_mutex ); 		
		 bool temp = url_queue.size() > 0 ? false: true;
		 pthread_mutex_unlock( &pt_mutex ); 		
		 return temp;	
	}
	
	void Save(surl t_surl, int thread_id)
	{
		pthread_mutex_lock( &pt_mutex ); 		
		char szSQL[100] = {0};
		sprintf(szSQL, "insert into Quene values(NULL,%d,'%s',%d);", thread_id,t_surl.url.c_str(), t_surl.deep_level);
		if(con->ModifyQuery(szSQL))
		{
			cout<<"insert false!"<<endl;
		}
		 pthread_mutex_unlock( &pt_mutex ); 		
	}

	void Add(surl t_surl)
	{
		 pthread_mutex_lock( &pt_mutex ); 		
		 url_queue.push_back(t_surl);
		 pthread_mutex_unlock( &pt_mutex ); 		
		
	}
	void update_queue()
	{
		pthread_mutex_lock( &pt_mutex ); 		
		if(url_queue.size() < 2000)
		{
			char szSQL[100] = {0};
			sprintf(szSQL, "select * from Quene category limit 2000;");
			con->SelectQuery(szSQL);
			
			char sz_dSQL[100] = {0};
				
			while (char** r = con->FetchRow())
			{
				struct surl t_surl;
				t_surl.url = (string)r[2];
						
				t_surl.deep_level =atoi(r[3]);	
		 		
				url_queue.push_back(t_surl);
	
				memset(sz_dSQL,0,100);		
				sprintf(sz_dSQL,"delete from Quene where id=%s;",r[0]);		
				if(con->ModifyQuery(sz_dSQL))
				{
					cout<<"delete false!"<<endl;
				}
			}		
		}
		pthread_mutex_unlock( &pt_mutex ); 		
	}	
	
	
	void Save_Persioninfo(Persioninfo &P)
	{
   		pthread_mutex_lock( &pt_mutex );
		char szSQL[1024 * 10] = {0};
		if((strlen(P.nick_name) + strlen(P.address) + strlen(P.person_sign)) > 10000)
			return;
        	sprintf(szSQL, "insert into Persioninfo values(NULL,'%s','%s','%s','%s','%s',%d,'%s',%d, %d, '%s','%s','%s','%s');", P.nick_name, P.true_name, P.sex, P.occupation, P.address,P.age,P.person_sign,P.focus_num,P.fans_num,P.Email,P.phone,P.QQ,P.weixin);
        	if( con->ModifyQuery(szSQL))
        	{
                        cout<<"insert false!"<<endl;
        	}

		pthread_mutex_unlock( &pt_mutex );

	}	

	surl pop()
	{
		pthread_mutex_lock( &pt_mutex ); 		
		surl tmp_surl;
		if(url_queue.size() > 0)
		{
			tmp_surl = url_queue.front();
			url_queue.pop_front();
		}
		 
		pthread_mutex_unlock( &pt_mutex ); 
		return tmp_surl;		

	}
private:
	deque<Surl> url_queue;
	pthread_mutex_t pt_mutex;
	CEncapMysql *con;	
};

QueueManager Surl_queue;
QueueManager Our_queue;


#endif
