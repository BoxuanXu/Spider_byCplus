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
    nohup ./run.sh &
fi
