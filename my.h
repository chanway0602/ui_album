#ifndef   MY_H_
#define  MY_H_

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>
#include <strings.h>
#include <stdlib.h>
#include <pthread.h>

#define PIC_PAHT   100


//���ݶ���
typedef struct pro_st//�ṹ��
{
    struct input_event touch;
    int lcd;//��lcd���ļ���������
    int ts;//�򿪴��������ļ�������
    unsigned long * mmap_star;//�����ڴ�ӳ��ָ��
    unsigned long black_color;//��Ŀ������ɫ
    int touch_x,touch_y;
    int windows_w,windows_h;
    int wah;

}p_st;


typedef struct photo_st
{
    char *pic_path;
    int pic_w,pic_h;
    struct photo_st * next;
    struct photo_st * prev;

}ph_st;

typedef struct game_st
{
    int x0,y0,r,plate_w,plate_h,circle;
    unsigned long circle_color,plate_color;
    int x_mask,y_mask;//0�� x0-- y0-- 1��x0++ y0++
    pthread_t C_ID, P_ID;
    int GV_MASK, S_S_MASK;
    int count;  //������ײ����
    int MinLenthOfPlant, MaxLenthOfPlant;
}g_st;



//����������
//========pro_set.c
int pro_init();
int touch_data();
int show_windows(char *pic_path);
int pro_free();


//========music.c
int music_fun();


//========pic.c
int pic_find();
int pic_fun();
bool is_bmp(char *name);
ph_st* init_list(void);
ph_st* new_node(char *name);
ph_st* switchover(ph_st* head, char dire);
void list_add_tail(ph_st* new, ph_st* head);

//========game.c
int game_init();
int game_fun();
int game_free();
void *move_ball();
void *ctrl_plate_move();
int move_plate(int x_c);
int show_picture(const char * pic_path, int lseek_x, int lseek_y);
void draw_ball();


#endif // MY_H_
