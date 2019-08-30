#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

struct input_event touch;

int move_plate(int x_c)
{
	
	int x,y;
	int lcd = open("/dev/fb0",O_RDWR);
	if(lcd <0){
		perror("OPEN LCD FAILED");
		return -1;
	}
	int * mmap_star = mmap(NULL,800*480*4,PROT_READ | PROT_WRITE,MAP_SHARED,lcd,0);
	if(mmap_star == MAP_FAILED){
		perror("MAP LCD FAILED");
		return -1;
	}
	
	for(y=400; y<430; y++)
	{
		for(x=0; x<800; x++)
		{
			if(x>x_c-50 && x<x_c+50){//画小木板
				*(mmap_star+800*y+x) = 0x0000ff00;
			}
			else{//清除木板轨迹
				*(mmap_star+800*y+x) = 0x00000000;
			}
			
		}
	}
	
	close(lcd);
	return 0;
}

int main()
{
	int x,y;
	int ts = open("/dev/input/event0",O_RDONLY);
	if(ts<0){
		perror("OPEN EVENT0 FAILED");
		return -1;
	}
	
	while(1){
		
		read(ts,&touch,sizeof(touch));
		if(touch.type == EV_ABS && touch.code == ABS_X) x = touch.value;
		if(touch.type == EV_ABS && touch.code == ABS_Y) y = touch.value;
		
		move_plate(x);
	}
	
	close(ts);
	
	return 0;
}