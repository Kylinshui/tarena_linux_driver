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
#include <signal.h>

int fd;
char buf[100];
char ch;
int oldflags;

#define MISC_IOC_MAGIC	'M'
#define LED_ON  _IO(MISC_IOC_MAGIC,0x01)
#define LED_OFF _IO(MISC_IOC_MAGIC,0x02)

void chardevicedriver_signal_handler(int signum){
	printf("received signal from device signal number:0x%4x\n",signum);

}

int main(int argc, char **argv){
	fd = open("/dev/chardev", O_RDWR);
	if(fd < 0){
		printf("Failure to open device file /dev/chardev\n");
		return -1;
	}
	printf("Success to open device file: fd=%d\n", fd);

	//在应用程序中捕捉SIGIO信号(由驱动程序发送)
	signal(SIGIO, chardevicedriver_signal_handler);
	//将当前进程pid设置为fd文件所对应驱动程序将要发送SIGIO,SIGUSR信号进程pid
	fcntl(fd, F_SETOWN, getpid());
	//获取fd的打开方式
	oldflags = fcntl(fd, F_GETFL);
	//将fd的打开方式设置为FASYNC,即支持异步通知
	fcntl(fd, F_SETFL, oldflags | FASYNC);


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
				read(fd, buf, 100);
				printf("User space read string from kernel:%s\n", buf);
				//ioctl(fd,0,0);
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
