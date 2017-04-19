#ifndef COMMON_H
#  define COMMON_H


#include <map>
#include <fstream>
//#include "armadillo"
using namespace std;
//using namespace arma;

void cmd(string s, char* str[]){
        FILE *in;
        char buff[512];

        if(!(in = popen(s.c_str(), "r")))
        exit(1);

        while(fgets(buff, sizeof(buff), in)!=NULL){
        if(true) 
	{
		char* str_tmp = strstr(buff," ");
		int i = 0;
		while(str_tmp != NULL)
		{
			strcat(str[i++] ,str_tmp);
			str_tmp = strstr(NULL," ");
		}
	} 
        }
        pclose(in);
}
#endif
