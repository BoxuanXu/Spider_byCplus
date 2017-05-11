#pragma once

#include"BitMap.h"
#include"HashFun.h"
#include <algorithm>
#include <ctype.h>
#include <string>
template<class T>
struct __HashFun1          //5ÖÖ¹þÏ£º¯Êý¶ÔÓ¦µÄ·Âº¯Êý
{
	size_t operator()(const T& key)
	{
		return BKDRHash<T>(key.c_str());
	}
};

template<class T>
struct __HashFun2
{
	size_t operator()(const T& key)
	{
		return SDBMHash<T>(key.c_str());
	}
};

template<class T>
struct __HashFun3
{
	size_t operator()(const T& key)
	{
		return RSHash<T>(key.c_str());
	}
};

template<class T>
struct __HashFun4
{
	size_t operator()(const T& key)
	{
		return APHash<T>(key.c_str());
	}
};

template<class T>
struct __HashFun5
{
	size_t operator()(const T& key)
	{
		return JSHash<T>(key.c_str());
	}
};


template<class K = string,
class HashFun1 = __HashFun1<K>/*,
class HashFun2 = __HashFun2<K>,
class HashFun3 = __HashFun3<K>,
class HashFun4 = __HashFun4<K>,
class HashFun5 = __HashFun5<K>*/>
class Bloom_Filter
{
public:
	Bloom_Filter(size_t size)
		:_capacity(size)
	{
		pthread_mutex_init( &pt_mutex_bloom, NULL );
		_bitmap._array.resize((size >> 5) + 1);
	}
	
	~Bloom_Filter()
	{

		pthread_mutex_destroy( &pt_mutex_bloom ); 
	}
	
	void _Set(const K& key)
	{
		 pthread_mutex_lock( &pt_mutex_bloom );
		string S_r = (string)key;
  	         if(typeid(K) == typeid(string))
			transform(S_r.begin(),S_r.end(), S_r.begin(), ::tolower);  	 
		_bitmap.Set(HashFun1()(S_r) % _capacity);
		/*_bitmap.Set(HashFun2()(key) % _capacity);
		_bitmap.Set(HashFun3()(key) % _capacity);
		_bitmap.Set(HashFun4()(key) % _capacity);
		_bitmap.Set(HashFun5()(key) % _capacity);*/
		pthread_mutex_unlock( &pt_mutex_bloom );
	}

	void _add_datafromsql()
	{
		CEncapMysql *con2;	
		con2 = new CEncapMysql;		
    		con2->Connect("127.0.0.1", "root", "676892","Spider_Quene");

		char szSQL[100] = {0};
		sprintf(szSQL, "select * from Persioninfo;");
		con2->SelectQuery(szSQL);
			
					
		char sz_dSQL[100] = {0};
		int i=0;
		while (char** r = con2->FetchRow())
		{
				memset(sz_dSQL,0,100);
				sprintf(sz_dSQL,"http://my.csdn.net/%s",r[2]);		
				_bitmap.Set(HashFun1()(sz_dSQL) % _capacity);
				++i;
		}		
		delete con2;
	}	

	bool _IsIn(const K& key)
	{
		 pthread_mutex_lock( &pt_mutex_bloom );
		string S_r = (string)key;
  		  if(typeid(K) == typeid(string))
			transform(S_r.begin(),S_r.end(), S_r.begin(), ::tolower);  	 

		bool gg = _bitmap.Get(HashFun1()(S_r) % _capacity);
		
		if (!gg)
		{
			//cout<<"URL:"<<key<<","<<HashFun1()(key)<<","<<gg<<endl;
			pthread_mutex_unlock( &pt_mutex_bloom );
			return true;
		
		}
		/*if (!_bitmap.Test(HashFun1()(key) % _capacity))
			return false;
		if (!_bitmap.Test(HashFun1()(key) % _capacity))
			return false;
		if (!_bitmap.Test(HashFun1()(key) % _capacity))
			return false;
		if (!_bitmap.Test(HashFun1()(key) % _capacity))
			return false;*/
		pthread_mutex_unlock( &pt_mutex_bloom );
		return false;
	}
private:
	BitMap _bitmap;
	size_t _capacity;
	pthread_mutex_t pt_mutex_bloom;
};
//¿¿¿¿¿¿,csdn¿url¿¿¿¿¿¿¿
char *strupr(char *str){
    char *orign=str;
    for (; *str!='\0 '; str++)
        *str = tolower(*str);
    return orign;
}
