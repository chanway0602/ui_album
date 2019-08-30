#include <stdio.h>
#include <pthread.h>

void *A()
{
	while(1){
		sleep(1);
		printf("HELLO\n");
	}
	return;
}

void *B()
{
	while(1){
		sleep(1);
		printf("WORLD\n");
	}
	return;
}

int main()
{
	pthread_t A_ID,B_ID;
	int ret = pthread_create(&A_ID,NULL,A,NULL);
	if(ret !=0){
		printf("CREATE PTHREAD FOR FUNTION A FILED\n");
		return -1;
	}

	ret = pthread_create(&B_ID,NULL,B,NULL);
	if(ret !=0){
		printf("CREATE PTHREAD FOR FUNTION B FILED\n");
		return -1;
	}
	while(1);
	return 0;
}	

