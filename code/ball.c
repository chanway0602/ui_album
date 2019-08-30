#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>


int move_ball()
{
	
	int x0=400,y0=240,r=50,circle=0;
	int x,y;
	
	int lcd = open("/dev/fb0",O_RDWR);//打开LCD
	if(lcd <0){
		perror("OPEN LCD FAILED");
		return -1;
	}
	
	int *mmap_star = mmap(NULL,800*480*4,PROT_READ | PROT_WRITE,MAP_SHARED,lcd,0);
	if(mmap_star == MAP_FAILED){
		perror("MMAP FOR LCD FIALED");
		return -1;
	}
	
	while(1)
	{
		for(y=0; y<480; y++)
		{
			for(x=0; x<800; x++)
			{
				circle = (x-x0) * (x-x0) + (y-y0) * (y-y0);
				if( circle<= r*r){
					*(mmap_star+800*y+x) = 0x00ff0000;
				}
				else{
					*(mmap_star+800*y+x) = 0x00000000;
				}
			}
		}
		
		x0++;
		y0++;
		if()
	
	}
	
	int ret = munmap(mmap_star,800*480*4);
	if(ret<0){
		perror("MUNMAP LCD FAILED");
		return -1;
	}
	
	close(lcd);
	return 0;
}


int main()
{
	move_ball();
	
	return 0;
}