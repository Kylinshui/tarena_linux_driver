/*************************************************************************
	> File Name: testchardevicedriver.c
	> Author: bshui
	> Mail: shui6666@126.com 
	> Created Time: 2018年05月28日 星期一 15时15分06秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int fd;
char buf[10];
char ch;

int main(int argc, char **argv){
	fd = open("/dev/chardev", O_RDWR);
	if(fd < 0){
		printf("Failure to open device file /dev/chardev\n");
		return -1;
	}
	printf("Success to open device file: fd=%d\n", fd);

	while(1){
		printf("Starting to test deivce:/dev/chardevicedriver\n");
		printf("\t1. Press key\'q\' to exit...\n");
		printf("\t2. Press key\'o\' to operate_device...\n");

		ch = getchar();
		if('q'==ch){
			break;
		}

		switch(ch){
			case 'o':
				read(fd, buf, 0);
				write(fd, buf,0);
				ioctl(fd,0,0);
				break;
			default:
				break;
		}

		sleep(1);

	}
	close(fd);

	return 0;
}
