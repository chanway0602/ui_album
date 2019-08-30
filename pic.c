#include "my.h"

extern p_st pro_s;

int pic_find()
{
    printf("PIC IS FINDING...\n");
    return 0;
}


//�ж��ļ��Ƿ���.bmp��ʽ
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

int pic_fun()//�������������
{
    printf("STAR THE  PIC PLATYER\n");
    pic_find();

	struct stat info;
	bzero(&info, sizeof(info));
	char *target = "pic_bmp";
	lstat(target, &info);
	
	ph_st *head = init_list();

	printf("[LINE:%d]\n", __LINE__);
// Ҫ�������һ��Ŀ¼
    printf("ox%x\n",info.st_mode);
    
	if(S_ISDIR(info.st_mode))
	{
	printf("[LINE:%d]\n", __LINE__);
		// �򿪲�����Ŀ¼
		DIR *dp = opendir(target);
		chdir(target); // cd ..

		struct dirent *ep;
		while(1)
		{
			//��ȡĿ¼���ݣ���ȡ�������˳�ѭ��
			ep = readdir(dp);	
			if(ep == NULL)
				break;

			// �������е������ļ�
			if(ep->d_name[0] == '.')
				continue;

			bzero(&info, sizeof(info));
			lstat(ep->d_name, &info);
			
			//ͼƬ��������
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

//������ʾͼƬ
ph_st* pic = head->next;
show_windows(pic->pic_path);
while(1)
{
	touch_data();
	if(pro_s.touch.type == EV_KEY && pro_s.touch.code == BTN_TOUCH && pro_s.touch.value == 0)
	{
		        //�����л�
   	         if(pro_s.touch_x < 340 )
   	         {
   					pic = pic->prev;
					if(pic->pic_path == NULL)
						pic = pic->prev;
					show_windows(pic->pic_path);
	            }

    			//�����л�
            	if(pro_s.touch_x > 440 )
   				{
            		pic = pic->next;
					if(pic->pic_path == NULL)
						pic = pic->next;
					show_windows(pic->pic_path);
   				}

            	//�˳� �ص�������
            	if(pro_s.touch_x > 340 && pro_s.touch_x < 440 && pro_s.touch_y > 180 && pro_s.touch_y < 270)
            	{
            		show_windows("../window_bmp/main.bmp");
            		printf("EXIT PIC PLAYER\n");
            		chdir("/IOT");   //�л�ϵͳ����·��
            		break;
            	}
	}	
}

    return 0;
}


//��ʼ��һ����ͷ���Ŀ�����
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

//�����µĽڵ�
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

// ���ڵ�new���뵽��headΪ�׵������ĩβ
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
���˼·��
1.��ȡĿ¼����ȡͼƬ�ļ�������Ϣ *
2.��ͼƬ������Ϣ���뵽������ *
3.������ʾ
 */


