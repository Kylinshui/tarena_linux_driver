/*************************************************************************
	> File Name: testchardevicedriver.c
	> Author: bshui
	> Mail: shui6666@126.com 
	> Created Time: 2018年05月28日 星期一 15时15分06秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
int fd;
char buf[100];
char ch;
#define MISC_IOC_MAGIC	'M'
#define LED_ON  _IO(MISC_IOC_MAGIC,0x01)
#define LED_OFF _IO(MISC_IOC_MAGIC,0x02)

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
		printf("\t2. Press key\'r\' to read string from kernel...\n");
		printf("\t3. Press key\'w\' to write string to kernel...\n");
		printf("\t4. Press key\'i\' use ioctl func led on...\n");
		printf("\t5. Press key\'o\' use ioctl func led off...\n");

		ch = getchar();
		getchar();

		if('q'==ch){
			break;
		}

		switch(ch){
			case 'r':
				read(fd, buf, 0);
				break;
			case 'w':
				write(fd,buf,0);
				break;
			case 'i':
				ioctl(fd,LED_ON,0);
				printf("LED_ON:%d\n",LED_ON);
				break;
			case 'o':
				ioctl(fd,LED_OFF,0);
				break;
			default:
				break;
		}

		sleep(1);

	}
	close(fd);

	return 0;
}
