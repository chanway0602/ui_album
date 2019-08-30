#include "my.h"

extern p_st pro_s;

int pic_find()
{
    printf("PIC IS FINDING...\n");
    return 0;
}


//判断文件是否是.bmp格式
bool is_bmp(char *name)
{
	char *tmp = name;
	int i = 0;
	while(tmp[i])
	{
		if(tmp[i] == '.')
			return tmp[i+1] == 'b';
		i++;
	}
	
}

int pic_fun()//电子相册的总入口
{
    printf("STAR THE  PIC PLATYER\n");
    pic_find();

	struct stat info;
	bzero(&info, sizeof(info));
	char *target = "pic_bmp";
	lstat(target, &info);
	
	ph_st *head = init_list();

	printf("[LINE:%d]\n", __LINE__);
// 要浏览的是一个目录
    printf("ox%x\n",info.st_mode);
    
	if(S_ISDIR(info.st_mode))
	{
	printf("[LINE:%d]\n", __LINE__);
		// 打开并进入目录
		DIR *dp = opendir(target);
		chdir(target); // cd ..

		struct dirent *ep;
		while(1)
		{
			//读取目录内容，读取结束后退出循环
			ep = readdir(dp);	
			if(ep == NULL)
				break;

			// 跳过所有的隐藏文件
			if(ep->d_name[0] == '.')
				continue;

			bzero(&info, sizeof(info));
			lstat(ep->d_name, &info);
			
			//图片加入链表
			if(is_bmp(ep->d_name))
			{
				//printf("%s\n", ep->d_name);
				ph_st* new = new_node(ep->d_name);
				list_add_tail(new, head);
				printf("%s\n", new->pic_path);
			}
		}
        closedir(dp);
	}

//滑动显示图片
ph_st* pic = head->next;
show_windows(pic->pic_path);
while(1)
{
	touch_data();
	if(pro_s.touch.type == EV_KEY && pro_s.touch.code == BTN_TOUCH && pro_s.touch.value == 0)
	{
		        //向左切换
   	         if(pro_s.touch_x < 340 )
   	         {
   					pic = pic->prev;
					if(pic->pic_path == NULL)
						pic = pic->prev;
					show_windows(pic->pic_path);
	            }

    			//向右切换
            	if(pro_s.touch_x > 440 )
   				{
            		pic = pic->next;
					if(pic->pic_path == NULL)
						pic = pic->next;
					show_windows(pic->pic_path);
   				}

            	//退出 回到主界面
            	if(pro_s.touch_x > 340 && pro_s.touch_x < 440 && pro_s.touch_y > 180 && pro_s.touch_y < 270)
            	{
            		show_windows("../window_bmp/main.bmp");
            		printf("EXIT PIC PLAYER\n");
            		chdir("/IOT");   //切换系统工作路径
            		break;
            	}
	}	
}

    return 0;
}


//初始化一个带头结点的空链表
ph_st* init_list(void)
{
	ph_st* head = calloc(1, sizeof(ph_st));
	if(head != NULL)
	{
		head->pic_path = NULL;
		head->prev = head;
		head->next = head;
	}

	return head;
}

//建立新的节点
ph_st* new_node(char *name)
{
	ph_st* new = calloc(1, sizeof(ph_st));
	if(new != NULL)
	{
		new->pic_path = name;

		new->prev = new;
		new->next = new;
	}

	return new;
}

// 将节点new插入到以head为首的链表的末尾
void list_add_tail(ph_st* new, ph_st* head)
{
	new->prev = head->prev;
	new->next = head;

	head->prev->next = new;
	head->prev = new;
}

ph_st* switchover(ph_st* head, char dire)
{
	ph_st* p = head;
	
	if(dire == 'R')
	{	
		p = p->next;
		if(p->pic_path == NULL)
			p = p->next;
	}
	
	if(dire == 'L')
	{	
		p = p->prev;
		if(p->pic_path == NULL)
			p = p->prev;
	}
	
	return p;
}

/*
设计思路：
1.读取目录，获取图片文件名称信息 *
2.将图片名称信息加入到链表当中 *
3.滑动显示
 */


