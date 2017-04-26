#!/bin/bash
#/src/spider/spider_mysql -u http://my.csdn.net/wireless_com -r 'http://blog.csdn.net/([^/]+).*article','http://blog.csdn.net/([^/]+).*detail'
./spider_mysql -u http://blog.csdn.net/sunhuaqiang1/article/details/51811770 -r 'http://blog.csdn.net/([^/]+).*article','http://blog.csdn.net/([^/]+).*detail' 
