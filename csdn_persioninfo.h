#ifndef _CSDN_PERSIONINFO_H_
#define _CSDN_PERSIONINFO_H_

#include <string>
#include <pthread.h>
#include "easy_mysql.h"

using EncapMysql::CEncapMysql;
using namespace std;

class Persioninfo
{
public:
	 Persioninfo();
	 ~Persioninfo();
    	 Persioninfo(char nick_name[50],char true_name[30], char sex[4], int age, char occupation[30], char address[100], char person_sign[1024 * 5], int focus_num, int fans_num, char Email[30], char phone[20], char QQ[15],char weixin[15]);
    	 void SetTrueName(char true_name[30]);
    	 void SetSex(char sex[4]);
    	 void SetAge(int age);
    	 char *GetTrueName();
    	 char *GetSex();
    	 int GetAge();
    	 void PrintInfo();
	 void Save_Persioninfo();
public:	
	char nick_name[50];
	char true_name[30];
	char sex[4];
	char occupation[30];
	char address[100];
	int age;

	char person_sign[1024 * 5];
	int focus_num;
	int fans_num;
	char Email[30];
	char phone[20];
	char QQ[15];
	char weixin[15];
	
	
};

Persioninfo::Persioninfo()
{

	memset( nick_name,0,50);
	memset(true_name,0,30);
	memset(sex,0,4);
	memset(occupation,0,30);
	memset(address,0,100);
	age = 0;

	memset(person_sign,0,1024 * 5);
	focus_num = 0;
	fans_num = 0;
	memset(Email,0,30);
	memset(phone,0,20);
	memset(QQ,0,15);
	memset(weixin,0,15);
		
}

Persioninfo::~Persioninfo()
{
}

Persioninfo::Persioninfo(char nick_name[50],char true_name[30], char sex[4], int age, char occupation[30], char address[100], char person_sign[1024 * 5], int focus_num, int fans_num, char Email[30], char phone[20], char QQ[15],char weixin[15])
{

	memcpy(this->nick_name, nick_name,50);
	memcpy(this->true_name, true_name,30);
	memcpy(this->sex,sex,4);
	memcpy(this->occupation,occupation,30);
	memcpy(this->address, address,100);
	this->age = age;

	memcpy(this->person_sign, person_sign,1024 * 5);
	this->focus_num = focus_num;
	this->fans_num = fans_num;
	memcpy(this->Email,Email,30);
	memcpy(this->phone,phone,20);
	memcpy(this->QQ,QQ,15);
	memcpy(this->weixin,weixin,15);

}


void Persioninfo::PrintInfo()
{
	cout<<"PersionInfo:"<<this->nick_name<<","<<this->true_name<<","<<this->focus_num<<","<<this->fans_num<<","<<this->sex<<","<<this->occupation<<","<<this->address<<","<<this->person_sign<<endl;
}

#endif
