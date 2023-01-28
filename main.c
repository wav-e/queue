#include <sys/time.h>
#include <stdio.h>
#include "queue.h"


#define NUM 250000000/10	//1G = 250000000
uint32_t buf[NUM];

int main(void)
{
	struct timeval begin, end;
    long microseconds;

	uint32_t val=0xDEADBEEF;
	QUEUE_T q;

	gettimeofday(&begin, 0);
	//test_8bit();
	//test_Nbit(2);
	gettimeofday(&end, 0);

	queue_init(&q, buf, sizeof(buf), 4);

	gettimeofday(&begin, 0);
	for (int i=0; i<NUM; i++)
	{
		queue_put(&q, &val);
	}
	gettimeofday(&end, 0);
	microseconds = end.tv_sec - begin.tv_sec;
    printf("Time measured PUT: %ld us.\n", microseconds);

	gettimeofday(&begin, 0);
	for (int i=0; i<NUM; i++)
	{
		queue_get(&q, &val);
	}
	gettimeofday(&end, 0);
	microseconds = end.tv_sec - begin.tv_sec;
    printf("Time measured GET: %ld us.\n", microseconds);
    printf("val = 0x%08X\n",val);


    return 0;
}