#coding:utf-8
import urllib2
import re



user_agent = 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.75 Safari/537.36'

#url="https://wx2.qq.com/cgi-bin/mmwebwx-bin/webwxgetcontact"
url="https://wx.qq.com/cgi-bin/mmwebwx-bin/webwxgetcontact?r=1494388346616"
#cooi='wxuin=2411974509; wxsid=pKs0u0N3T79AI2eN; webwx_data_ticket=gSdg0TAFgHkUGKvqN4LHyzzL;'
cooi='wxuin=1624387620; wxsid=rQEXBHhrelRRMiia; webwx_data_ticket=gSdg0TAFgHkUGKvqN4LHyzzL;'


#headers = {'cookie': 'your cookie'}
headers = {'User-Agent' :user_agent,'cookie': cooi}
request = urllib2.Request(url,headers=headers)
response = urllib2.urlopen(request)
print response.read()
#pageCode = response.read().decode('utf-8')
#print pageCode
#pattern = re.compile('feed_list_content\\\\" >(.*?)<\\\\/div',re.S)
#pattern = re.compile('>(.*?)<a',re.S)
#items = re.findall(pattern,pageCode)
#print items
#for item in items:
#    print item
#    print"-----------------------------------------------------------------------------------------------------"
#print response.read()
