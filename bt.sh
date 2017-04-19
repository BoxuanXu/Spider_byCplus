#!/bin/bash
strA=`ps -aux | grep spider_mysql`
strB="./spider_mysql -r"
result=$(echo $str | grep "${strB}")
if [[ "$result" == "" ]]
then
    #echo "包含"
    date
#else
    #echo "不包含"
    /src/spider/spider_mysql -r 'http://blog.csdn.net/([^/]+).*article','http://blog.csdn.net/([^/]+).*detail' 
fi
