#include "my.h"
extern p_st pro_s;

int main_ctrl()
{
    int x,y;

    while(1)
    {
        touch_data();
        x= pro_s.touch_x;
        y= pro_s.touch_y;
        if(pro_s.touch.type == EV_KEY && pro_s.touch.code == BTN_TOUCH && pro_s.touch.value == 0)
        {
            if(x>30 && x<190 && y>300 && y<440)  
                pic_fun();//������ᰴť

            if(x>310 && x<414 && y>300 && y<440) 
                music_fun();//���ְ�ť

            if(x>580 && x<770 && y>300 && y<440) 
                game_fun();//��Ϸ��ť
        }

    }

    return 0;
}



int main()
{
    pro_init();

    main_ctrl();
    //game_fun();
 
    pro_free();

    return 0;
}
