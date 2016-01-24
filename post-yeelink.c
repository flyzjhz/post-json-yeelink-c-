//用于通过post方式发送json格式数据到yeelink。
//Raspberry Pi 2 B
//Raspbian jessie 
//linux 4.1.*
//by Luke

#include <curl/curl.h>//curl库，重点
#include <stdio.h>
#include <stdlib.h>

int gettemp();
int main (void)
{
	CURL *curl=NULL;
	char data[32]="";
	int i;
	i=gettemp();
	if(i==-1)
	{
		printf("get temp data error!");
		return -1;
	}

	sprintf(data,"{\"value\":%d}",i);	
	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);
	curl=curl_easy_init();//初始化
	if(curl != NULL)
	{
		curl_easy_setopt(curl,CURLOPT_URL,"http://api.yeelink.net/v1.0/device/343257/sensor/382812/datapoints");//前一个数字为设备号，后一个数字为传感器号。
		struct curl_slist *plist = curl_slist_append(NULL,"U-ApiKey:225b4a9d7cfa28197eba45c23a920728");//api-key换成自己的。
		curl_slist_append(plist,"Content-Type:application/json");
		curl_easy_setopt(curl,CURLOPT_HTTPHEADER,plist);
		curl_easy_setopt(curl,CURLOPT_POSTFIELDS,data);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
		{
			printf("%s\n\n",curl_easy_strerror(res));
		}
		curl_easy_cleanup(curl);	
			
			
	}
	curl_global_cleanup();//清空，结束。
	return 0;

}

//用于获取cpu温度。
int gettemp(void)
{
	char temp[16];
	int cputemp;
	FILE *fp;
	fp=fopen("/sys/class/thermal/thermal_zone0/temp","r");
	if(fp==NULL)
	{
		return -1;
	}	
	fgets(temp,8,fp);
	cputemp=atoi(temp)/1000;//这里对温度进行了取整，把小数强制换成了整数。
	fclose(fp);
	return cputemp;

}
