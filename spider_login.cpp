#include <curl/curl.h>
#include <stdlib.h>
//#define POSTURL "https://login.xiami.com/member/login"
#define GETURL "https://wx.qq.com/cgi-bin/mmwebwx-bin/webwxgetcontact?r=1494388346616"
#define POSTFIELDS "_xiamitoken=819938e694aa2024c49f2a0c362831e3&done=http://www.xiami.com&type=&email=112121212@qq.com&password=passwd&submit=登录"
#define FILENAME "curlposttest.log"

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);

int main(int argc, char *argv[]) {
	CURL *curl;
	CURLcode res;
	FILE *fptr;
	struct curl_slist *http_header = NULL;
	
	if ((fptr = fopen(FILENAME, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", FILENAME);
		return 0;
	}
	
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, GETURL);
	//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, POSTFIELDS);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);
	//curl_easy_setopt(curl, CURLOPT_POST, 1);
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
	//curl_easy_setopt(curl, CURLOPT_HEADER, 1);
	//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

//	curl_easy_setopt(curl,CURLOPT_COOKIEJAR,"cookie_login.txt");//保存登陆后的cookie

	curl_easy_setopt(curl,CURLOPT_COOKIE,"wxuin=1624387620; wxsid=rQEXBHhrelRRMiia; webwx_data_ticket=gSdHFiEDXbh8EnQxGmYsCdiG;");//提交第一步保存的cookie

	curl_easy_setopt( curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.75 Safari/537.36");  
//	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); 
//	curl_easy_setopt(curl, CURLOPT_CAINFO, "ca-bundle.crt");	//设置证书路径
//	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST,  1L);

	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	//system("pause");
	return 1;
}

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
	FILE *fptr = (FILE*)userp;
	return fwrite(buffer, size, nmemb, fptr);
}

