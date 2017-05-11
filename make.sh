#!/bin/bash
#g++ -std=c++0x -g -o spider spider.cpp -lcurl
g++ -std=c++0x easy_mysql.cpp -c
g++ -std=c++0x spider.cpp -c 
#g++ -g -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient -I/src/soft/boost_1_63_0/libs/regex/src -L/src/soft/boost_1_63_0/libs/regex/build/gcc -lboost_regex-gcc-1_53  spider.o easy_mysql.o -o spider_mysql -lcurl
g++ -g -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient -I/src/boost/dl.bintray.com/boostorg/release/1.64.0/source/boost_1_64_0/libs/regex/src -L/src/boost/dl.bintray.com/boostorg/release/1.64.0/source/boost_1_64_0/libs/regex/build/gcc -lboost_regex-gcc-1_53  spider.o easy_mysql.o -o spider_mysql -lcurl


#./spider_mysql -u http://blog.csdn.net/sunhuaqiang1/article/details/51811770 -r 'http://blog.csdn.net/([^/]+).*article','http://blog.csdn.net/([^/]+).*detail'
