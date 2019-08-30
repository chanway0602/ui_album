#include "my.h"

g_st p_c;
extern p_st pro_s;

const char *grade_path[10] = {"/IOT/window_bmp/0.bmp", "/IOT/window_bmp/1.bmp", "/IOT/window_bmp/2.bmp", "/IOT/window_bmp/3.bmp",
                                               "/IOT/window_bmp/4.bmp", "/IOT/window_bmp/5.bmp", "/IOT/window_bmp/6.bmp", "/IOT/window_bmp/7.bmp",
                                               "/IOT/window_bmp/8.bmp", "/IOT/window_bmp/9.bmp"};

int game_init()
{
    printf("INTI THE P&C GAME\n");
    //球的圆心坐标（400,240），半径50，单位：像素
    p_c.x0 = 400,p_c.y0 = 240,p_c.r = 50;
    //木板的长为300，高30
    p_c.plate_h = 30, p_c.plate_w = 300;
    //圆的平方公式
    p_c.circle=0;
    //圆球和长条的颜色
    p_c.circle_color = 0x00ff0000,p_c.plate_color = 0x0000ff00;

    p_c.x_mask=0,p_c.y_mask=0;
    //GV__MASK:  0:游戏没输   1：游戏输了  S_S_MASK:0:游戏暂停 1：游戏开始
    p_c.GV_MASK=0,p_c.S_S_MASK=0;

    //项目背景颜色
    //pro_s.black_color = 0x0000FF00;
    
    show_windows("./window_bmp/MI.bmp");
    draw_ball();  //重新画球
    move_plate(400);  //重新画木板

    //创建运动球的线程
    int ret = pthread_create(&p_c.C_ID,NULL,move_ball,NULL);
    if(ret !=0)
    {
        perror("CREATE PTHREAD FOR PUNTION MOVE_BALL FAILED");
        return -1;
    }

    //创建移动木板的线程
    ret = pthread_create(&p_c.P_ID,NULL,ctrl_plate_move,NULL);
    if(ret !=0)
    {
        perror("CREATE PTHREAD FOR PUNTION ctrl_plate_move FAILED");
        return -1;
    }

    return 0;
}

void draw_ball()
{
    int x,y;
    int R = p_c.r*p_c.r;  //大圆半径平方
    int r = (p_c.r-30)*(p_c.r-30);  //小圆半径平方

            for(y=p_c.y0-p_c.r; y<=p_c.y0+p_c.r; y++)
           {
                for(x=p_c.x0-p_c.r; x<=p_c.x0+p_c.r; x++)
              {
                    p_c.circle = (x-p_c.x0) * (x-p_c.x0) + (y-p_c.y0) * (y-p_c.y0);

                    if(p_c.circle <R && p_c.circle>r)
                    {
                       //外圆颜色填充
                       *(pro_s.mmap_star+800*y+x) = p_c.circle_color;
                    }
                    else if(p_c.circle <r)
                    {   
                       //内圆颜色填充
                        *(pro_s.mmap_star+800*y+x) = 0x00000000;
                    }
                    else
                    {
                       //方框其他区域颜色填充
                       *(pro_s.mmap_star+800*y+x) = pro_s.black_color;
                    }
                 }
              }
}


void *move_ball()
{
    printf("MOVE BALL\n");

    p_c.count = 0;  //计算碰撞次数

    while(1)
    {

	   while(p_c.S_S_MASK)
	   {
            if(p_c.count/10 == 0)
            {
                //个位数分数
                show_picture(grade_path[p_c.count], 768, 0);
            }
            else if(p_c.count/100 == 0)
            {   //十位数分数
                show_picture(grade_path[p_c.count%10], 768, 0);
                show_picture(grade_path[p_c.count/10],   736, 0);
            }
            else 
            {   //百位数分数
                show_picture(grade_path[p_c.count%10],      768, 0);
                show_picture(grade_path[p_c.count/10%10], 736, 0);
                show_picture(grade_path[p_c.count/100],      704, 0);
            }

            draw_ball();

		    //撞到屏幕底部
            if(p_c.y0+p_c.r == 478) //p_c.y_mask=0;
            {
                //p_c.S_S_MASK = 0;  //暂停
                show_picture("/IOT/window_bmp/GV.bmp", 250, 165);
                sleep(3);
                p_c.count = 0;  //分数清零
                show_windows("./window_bmp/MI.bmp");  //清屏
                p_c.S_S_MASK = 0;  //球停止运动
                p_c.x0 = 400,p_c.y0 = 240,p_c.r = 50;
                draw_ball();  //重新画球
                move_plate(400);  //重新画木板
            }

            //撞到木板
            if(p_c.y0+p_c.r == 400 && p_c.x0>=p_c.MinLenthOfPlant-50 && p_c.x0<=p_c.MaxLenthOfPlant+50)
        	   {
                    p_c.y_mask=0;
                    p_c.count++;
               }
            if(p_c.x0+p_c.r == 697) p_c.x_mask=0;
            if(p_c.y0-p_c.r == 0)   p_c.y_mask=1;
            if(p_c.x0-p_c.r == 0)   p_c.x_mask=1;

            if(p_c.y_mask==0) p_c.y0--;
            if(p_c.y_mask==1) p_c.y0++;
            if(p_c.x_mask==0) p_c.x0--;
            if(p_c.x_mask==1) p_c.x0++;

            //延时
            usleep(300);

	    }
    }
    return 0;
}


int move_plate(int x_c)
{

	int x,y;
    p_c.MinLenthOfPlant  = x_c-100;
    p_c.MaxLenthOfPlant = x_c+100;

	for(y=400; y<430; y++)
	{
		for(x=0; x<697; x++)
		{
			if(x>p_c.MinLenthOfPlant && x<p_c.MaxLenthOfPlant) //木板长度
			{
				//木板颜色
				*(pro_s.mmap_star+800*y+x) = p_c.plate_color;
			}
			else
			{
				//其他区域颜色
				*(pro_s.mmap_star+800*y+x) = pro_s.black_color;
			}

		}
	}

	return 0;
}


void *ctrl_plate_move()
{
    int x,y;
	while(1)
    {
        touch_data();
        x = pro_s.touch_x;
        y = pro_s.touch_y;
        if(y>400 && y<430 && x>100 && x<597)
        {
                if(x>p_c.MinLenthOfPlant && x<p_c.MaxLenthOfPlant)
                {
                    move_plate(x);
                }
                
        }

        if(pro_s.touch.type == EV_KEY && pro_s.touch.code == BTN_TOUCH && pro_s.touch.value == 0)
        {
            //开始OR暂停
            if(x>700 && x<800 && y>50 && y<150)
            {
                if(p_c.S_S_MASK == 0)
                {
                    p_c.S_S_MASK = 1;
                }
                else
                {
                    p_c.S_S_MASK = 0;
                }
            }

            //重新开始
            if(x>700 && x<800 && y>150 && y<300)
            {
                p_c.count = 0;  //分数清零
                show_windows("./window_bmp/MI.bmp");  //清屏
                p_c.S_S_MASK = 0;  //球停止运动
                p_c.x0 = 400,p_c.y0 = 240,p_c.r = 50;
                draw_ball();  //重新画球
                move_plate(400);  //重新画木板
            }

            //退出
            if(x>700 && x<800 && y>300 && y<480)
            {
                p_c.GV_MASK = 1;
            }

        }

	}

    return 0;
}

int game_free()
{
    printf("FREE THE GAME \n");
    pthread_cancel(p_c.C_ID);
    pthread_cancel(p_c.P_ID);
    show_windows("./window_bmp/main.bmp");//显示主界面
    return 0;
}

int show_picture(const char * pic_path, int lseek_x, int lseek_y)
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
    //printf("WIDTH IS %d\n",pro_s.windows_w);
    read(f_bmp,&pro_s.windows_h,4);
    //printf("height IS %d\n",pro_s.windows_h);
    pro_s.wah = pro_s.windows_h*pro_s.windows_w;

    char rgb[pro_s.wah*3];//定义一个数组，存放图片的像素点  rgb[0] = B   rgb[1] = G   rgb[2] = R
    int new_rgb[pro_s.wah];
    //定义一个新的整形数组，存放24位转成32位像素点的数组 new_rgb[0] = R | G |B new_rgb[1]  = ?? new_rgb[2]

    lseek(f_bmp,54,SEEK_SET);//跳过54个字节
    read(f_bmp,rgb,pro_s.wah*3);//需要把24的转成32位


    for(loop_num=0; loop_num<pro_s.wah; loop_num++)
        new_rgb[loop_num] = rgb[3*loop_num]<<0 | rgb[(3*loop_num)+1]<<8 | rgb[(3*loop_num)+2]<<16;

    unsigned long *mmap_new = pro_s.mmap_star;
    mmap_new +=  800*lseek_y + lseek_x;

    for(y=0; y<pro_s.windows_h; y++)
    {
        for(x=0; x<pro_s.windows_w; x++)
        {
            *(mmap_new + 800*(pro_s.windows_h-1-y)+x) = new_rgb[pro_s.windows_w*y+x];
            // lcd                              纵轴反转
        }
    }

    close(f_bmp);
    return 0;
}

int game_fun()
{
    printf("STAR THE P&C GAME\n");
    game_init();

    while(1)
    {

        if(p_c.GV_MASK == 1)
        {
            printf("----------\n");
            break;
        }
    }
    game_free();

    return 0;
}
