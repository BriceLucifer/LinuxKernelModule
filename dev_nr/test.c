#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(){
	int ret = open("/dev/mydevice",O_RDONLY);
	if(ret == -1){
		printf("can not open sorry\n");
		return -1;
	}else{
		printf("open is successful\n");
	}
	close(ret);
	return 0;
}
