#include "my.h"

p_st pro_s;

int pro_init()
{
    printf("PRO INIT\n");

    //打开LCD
    pro_s.lcd = open("/dev/fb0",O_RDWR);
    if(pro_s.lcd == -1)
    {
        perror("OPEN LCD FAILED");
        return -1;
    }

    //打开触摸屏
    pro_s.ts = open("/dev/input/event0",O_RDONLY);
    if(pro_s.ts == -1)
    {
        perror("OPEN EVENT0 FAILED");
        return -1;
    }

    //LCD内存映射
    pro_s.mmap_star = mmap(NULL, 800*480*4, PROT_READ | PROT_WRITE, MAP_SHARED, pro_s.lcd, 0);
    if(pro_s.mmap_star == MAP_FAILED)
    {
        perror("MMAP FOR LCD FAILED");
        return -1;
    }

    pro_s.black_color = 0x00ffffff;
    pro_s.windows_h =0,pro_s.windows_w=0;

    show_windows("./window_bmp/main.bmp");//显示主界面

    return 0;
}

//读取触摸屏坐标
int touch_data()
{

    read(pro_s.ts,&pro_s.touch,sizeof(pro_s.touch));

    if(pro_s.touch.type == EV_ABS && pro_s.touch.code == ABS_X)  
        pro_s.touch_x = pro_s.touch.value;
    if(pro_s.touch.type == EV_ABS && pro_s.touch.code == ABS_Y)  
        pro_s.touch_y = pro_s.touch.value;

    return 0;
}


//显示主界面
int show_windows(char * pic_path)
{
	int loop_num,x,y;

	int f_bmp = open(pic_path, O_RDONLY);
	if(f_bmp ==-1)
    {
		perror("OPEN PICTURE FAILED");
		printf("OPEN %s FAILED", pic_path);
		return -1;
	}

	lseek(f_bmp,18,SEEK_SET);//获取图片的长 宽
	read(f_bmp,&pro_s.windows_w,4);
	printf("WIDTH IS %d\n",pro_s.windows_w);
	read(f_bmp,&pro_s.windows_h,4);
	printf("height IS %d\n",pro_s.windows_h);
	pro_s.wah = pro_s.windows_h*pro_s.windows_w;

	char rgb[pro_s.wah*3];//定义一个数组，存放图片的像素点  rgb[0] = B   rgb[1] = G   rgb[2] = R
	int new_rgb[pro_s.wah];
	//定义一个新的整形数组，存放24位转成32位像素点的数组 new_rgb[0] = R | G |B new_rgb[1]  = ?? new_rgb[2]

	lseek(f_bmp,54,SEEK_SET);//跳过54个字节
	read(f_bmp,rgb,pro_s.wah*3);//需要把24的转成32位


	for(loop_num=0; loop_num<pro_s.wah; loop_num++)

		new_rgb[loop_num] = rgb[3*loop_num]<<0 | rgb[(3*loop_num)+1]<<8 | rgb[(3*loop_num)+2]<<16;


	for(y=0; y<pro_s.windows_h; y++)
	{
		for(x=0; x<pro_s.windows_w; x++)
		{
			*(pro_s.mmap_star + 800*(pro_s.windows_h-1-y)+x) = new_rgb[pro_s.windows_w*y+x];
			// lcd                              数组
		}
	}

    close(f_bmp);
	return 0;
}



int pro_free()
{
    printf("PRO FREE\n");

    int ret = munmap(pro_s.mmap_star,800*480*4);
    if(ret == -1){
        perror("MUNMAP LCD FAILED");
        return -1;
    }

    close(pro_s.lcd);
    close(pro_s.ts);

    return 0;
}
