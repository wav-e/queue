#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "queue.h"


print_mem(uint8_t *mem, uint32_t size)
{
    for (int i=0; i<size; i++){
        printf("mem[%d] = 0x%02X\r\n", i, mem[i]);
    }

    printf("-------OVERMEM--------\r\n");

    for (int i=size; i<size+4; i++){
        printf("mem[%d] = 0x%02X\r\n", i, mem[i]);
    }
}



void print_queue_info(QUEUE_T *q)
{
	int i=0; 
	uint8_t *ptr = (uint8_t*)q->buf;

	printf("----START MEMORY-------\r\n");
    for (i=0; i<((q->qSize)*(q->maxObjs)); i++)
    {
    	if (q->startPtr == &ptr[i])
    		printf("start->");
    	else if (q->endPtr == &ptr[i])
    		printf("  end->");
    	else 
    		printf("       ");

        printf("mem[%d] = 0x%02X", i, ptr[i]);
        if (q->putPtr == &ptr[i])
       		printf("<=put");
       	if (q->getPtr == &ptr[i])
       		printf("<=get");
       	printf("\r\n");
    }
    printf("------END MEMORY-------\r\n");

    for (; i<((q->qSize)*(q->maxObjs))+((q->qSize)*(q->maxObjs)); i++)
    {
    	if (q->startPtr == &ptr[i])
    		printf("start->");
    	else if (q->endPtr == &ptr[i])
    		printf("  end->");
    	else 
    		printf("       ");

        printf("mem[%d] = 0x%02X", i, ptr[i]);
        if (q->putPtr == &ptr[i])
       		printf("<=put");
       	if (q->getPtr == &ptr[i])
       		printf("<=get");
       	printf("\r\n");
    }

}



void test_Nbit(uint8_t size_byte)
{
	int i=0;
	uint32_t tmp;
#define TEST_OBJS_NUM 10

	uint8_t bBuf[TEST_OBJS_NUM];
	uint16_t wBuf[TEST_OBJS_NUM];
	uint32_t dwBuf[TEST_OBJS_NUM];
 	void *buf;
 	
	QUEUE_T Queue;



	memset(bBuf,0xAA, sizeof(bBuf));
	memset(wBuf,0xAA, sizeof(wBuf));
	memset(dwBuf,0xAA, sizeof(dwBuf));


	switch(size_byte)
	{
		case(1): buf = (void*)bBuf; break;
		case(2): buf = (void*)wBuf; break;
		case(4): buf = (void*)dwBuf; break;
		default: {
			printf("QUEUE TEST: unsupporting size\r\n"); 
			return;
		}

	}
	queue_init(&Queue, buf, size_byte*TEST_OBJS_NUM, size_byte);



	printf("init\r\n");
	printf("num elements = %d\r\n", queue_get_num(&Queue));
	print_queue_info(&Queue);

	printf("---------------------------------\r\n");
	printf("put 10 elements\r\n");
	for (i=0; i<TEST_OBJS_NUM; i++)
		queue_put(&Queue, &i);
	printf("num elements = %d\r\n", queue_get_num(&Queue));
	print_queue_info(&Queue);
	printf("---------------------------------\r\n");


	printf ("trying overrun put. ret=%d\r\n", queue_put(&Queue, &i));
	print_queue_info(&Queue);


	printf("---------------------------------\r\n");
	printf("get 5 elements:\r\n");
	for (int i=0; i<TEST_OBJS_NUM/2; i++)
	{ 	
		queue_get(&Queue, &tmp);
		printf("getval = %d\r\n", tmp);
	}
	print_queue_info(&Queue);

	printf ("get 5 elements again\r\n");
	printf("---------------------------------\r\n");
	printf("get 5 elements:\r\n");
	for (int i=0; i<TEST_OBJS_NUM/2; i++)
	{ 	

		queue_get(&Queue, &tmp);
		printf("getval = %d\r\n", tmp);
	}
	print_queue_info(&Queue);


	printf("---------------------------------\r\n");

	printf ("tmp=%d\r\n",  tmp);
	printf ("get 1 element overrun. ret=%d\r\n", queue_get(&Queue, &tmp));
	printf ("tmp=%d\r\n",  tmp);

	printf("---------------------------------\r\n");
	printf("get 5 elements:\r\n");
	for (int i=0; i<TEST_OBJS_NUM/2; i++)
	{ 	
		queue_get(&Queue, &tmp);
		printf("getval = %d\r\n", tmp);
	}
	print_queue_info(&Queue);
}


void test_NbitInverse(uint8_t size_byte)
{
	int i=0;
	uint32_t 	tmp;


	uint8_t 	tmp8;
	uint16_t 	tmp16;
	uint32_t	tmp32;

	#define TEST_OBJS_NUM_2 4

	uint8_t bBuf[TEST_OBJS_NUM_2];
	uint16_t wBuf[TEST_OBJS_NUM_2];
	uint32_t dwBuf[TEST_OBJS_NUM_2];
 	void *buf;
 	
	QUEUE_T Queue;



	memset(bBuf,0xAA, sizeof(bBuf));
	memset(wBuf,0xAA, sizeof(wBuf));
	memset(dwBuf,0xAA, sizeof(dwBuf));


	switch(size_byte)
	{
		case(1): buf = (void*)bBuf;  break;
		case(2): buf = (void*)wBuf; break;
		case(4): buf = (void*)dwBuf; break;
		default: {
			printf("QUEUE TEST: unsupporting size\r\n"); 
			return;
		}

	}
	queue_init(&Queue, buf, size_byte*TEST_OBJS_NUM_2, size_byte);



	printf("init\r\n");
	printf("num elements = %d\r\n", queue_get_num(&Queue));
	print_queue_info(&Queue);

	printf("---------------------------------\r\n");
	printf("put 1 elements\r\n");
	for (i=0; i<TEST_OBJS_NUM_2-1; i++)
		queue_put(&Queue, &i);
	printf("num elements = %d\r\n", queue_get_num(&Queue));
	print_queue_info(&Queue);
	printf("---------------------------------\r\n");

	printf("\r\ntrying put to head\r\n");
	queue_putToHead(&Queue, &i);
	print_queue_info(&Queue);
	printf("---------------------------------\r\n");

	printf("\r\ntrying put to head overrun\r\n");
	queue_putToHead(&Queue, &i);
	print_queue_info(&Queue);
	printf("---------------------------------\r\n");




	printf("\r\ntrying to get 1 element\r\n");
	tmp = 0;
	printf("tmp init = 0x%X\r\n", tmp);	
	queue_get(&Queue, &tmp);
	printf("getval = 0x%X\r\n", tmp);	
	print_queue_info(&Queue);
	printf("num elements = %d\r\n", queue_get_num(&Queue));
	printf("---------------------------------\r\n");


	printf("\r\ntrying to get 1 element\r\n");
	tmp = 0;
	printf("tmp init = 0x%X\r\n", tmp);	
	queue_get(&Queue, &tmp);
	printf("getval = 0x%X\r\n", tmp);	
	print_queue_info(&Queue);
	printf("num elements = %d\r\n", queue_get_num(&Queue));
	printf("---------------------------------\r\n");


	printf("\r\ntrying put to head\r\n");
	tmp = 0xDC;
	queue_putToHead(&Queue, &tmp);
	print_queue_info(&Queue);
	printf("---------------------------------\r\n");	

	tmp = 0xDD;
	printf("\r\ntrying put to head\r\n");
	queue_putToHead(&Queue, &tmp);
	print_queue_info(&Queue);
	printf("num elements = %d\r\n", queue_get_num(&Queue));
	printf("---------------------------------\r\n");	

	
	{
	uint32_t tmp[TEST_OBJS_NUM_2];
	memset(&tmp, 0x0, sizeof(tmp));
	printf("\r\ntrying to get all elements\r\n");
	for (i=0; i<TEST_OBJS_NUM_2; i++)
	{	
		queue_get(&Queue, &tmp[i]);
		printf ("tmp[%d]=0x%X\r\n", i, tmp[i]);
	}
	print_queue_info(&Queue);
	printf("num elements = %d\r\n", queue_get_num(&Queue));
	printf("---------------------------------\r\n");

	}
#if 0

	printf ("trying overrun put. ret=%d\r\n", queue_put(&Queue, &i));
	print_queue_info(&Queue);


	printf("---------------------------------\r\n");
	printf("get 5 elements:\r\n");
	for (int i=0; i<TEST_OBJS_NUM_2/2; i++)
	{ 	
		queue_get(&Queue, &tmp);
		printf("getval = %d\r\n", tmp);
	}
	print_queue_info(&Queue);

	printf ("get 5 elements again\r\n");
	printf("---------------------------------\r\n");
	printf("get 5 elements:\r\n");
	for (int i=0; i<TEST_OBJS_NUM_2/2; i++)
	{ 	

		queue_get(&Queue, &tmp);
		printf("getval = %d\r\n", tmp);
	}
	print_queue_info(&Queue);


	printf("---------------------------------\r\n");

	printf ("tmp=%d\r\n",  tmp);
	printf ("get 1 element overrun. ret=%d\r\n", queue_get(&Queue, &tmp));
	printf ("tmp=%d\r\n",  tmp);

	printf("---------------------------------\r\n");
	printf("get 5 elements:\r\n");
	for (int i=0; i<TEST_OBJS_NUM_2/2; i++)
	{ 	
		queue_get(&Queue, &tmp);
		printf("getval = %d\r\n", tmp);
	}
	print_queue_info(&Queue);

	#endif
}












void test_Nbit_check(uint8_t size_byte)
{
	int i=0;
	uint32_t tmp;
	uint32_t num_check = 0;
	uint32_t errors = 0;
	uint32_t res = 0;


#define TEST_OBJS_NUM 10

	uint8_t bBuf[TEST_OBJS_NUM];
	uint16_t wBuf[TEST_OBJS_NUM];
	uint32_t dwBuf[TEST_OBJS_NUM];
 	void *buf;
 	
	QUEUE_T Queue;



	memset(bBuf,0xAA, sizeof(bBuf));
	memset(wBuf,0xAA, sizeof(wBuf));
	memset(dwBuf,0xAA, sizeof(dwBuf));


	switch(size_byte)
	{
		case(1): buf = (void*)bBuf; break;
		case(2): buf = (void*)wBuf; break;
		case(4): buf = (void*)dwBuf; break;
		default: {
			printf("QUEUE TEST: unsupporting size\r\n"); 
			return;
		}

	}
	queue_init(&Queue, buf, size_byte*TEST_OBJS_NUM, size_byte);

	/*
		1) test fill put with constant values
		2) test overrun put
		3) test get and check constant values
		4) test overrun get 
	*/
	printf("---------------------------------\r\n");

	printf("Testing queue module\r\n");
	printf("---------------------------------\r\n");
	printf("* Test fill put with constant values\r\n");
	printf("     num elements at start = %d\r", queue_get_num(&Queue));
	num_check = 0;
	if (num_check != queue_get_num(&Queue))
		printf("ERR\r\n");
	else
		printf("OK\r\n");

	printf("     trying to put %d obj\r\n", TEST_OBJS_NUM);
	for (i=0; i<TEST_OBJS_NUM; i++)
	{
		num_check++;

		if (queue_put(&Queue, &i)){
			errors++;
			printf("    ERR put %d\r\n",i);
		}
		else{
			printf("    OK  put %d \r\n",i);

		}
		
		num_check++;
		if (queue_get_num(&Queue) != num_check){
			printf("    ERR put: size queue not match\r\n");

		}
	}
	printf("     trying to overrun %d obj\r", TEST_OBJS_NUM+1);
	res = queue_put(&Queue, &i);
	if (!res)	
		printf("ERR\r\n");
	else
	{
		if (TEST_OBJS_NUM == queue_get_num(&Queue))
			printf("OK \r\n");
		else 
		{
			printf("ERR\r\n");
			printf("      NOT EQUAL SIZE = %d\r\n", queue_get_num(&Queue));
		}
	}


	printf("     test get from queue  obj\r");




	for (i=0; i<TEST_OBJS_NUM; i++)
		queue_put(&Queue, &i);
	printf("num elements = %d\r\n", queue_get_num(&Queue));
	print_queue_info(&Queue);
	printf("---------------------------------\r\n");


	printf ("trying overrun put. ret=%d\r\n", queue_put(&Queue, &i));
	print_queue_info(&Queue);


	printf("---------------------------------\r\n");
	printf("get 5 elements:\r\n");
	for (int i=0; i<TEST_OBJS_NUM/2; i++)
	{ 	
		queue_get(&Queue, &tmp);
		printf("getval = %d\r\n", tmp);
	}
	print_queue_info(&Queue);

	printf ("get 5 elements again\r\n");
	printf("---------------------------------\r\n");
	printf("get 5 elements:\r\n");
	for (int i=0; i<TEST_OBJS_NUM/2; i++)
	{ 	

		queue_get(&Queue, &tmp);
		printf("getval = %d\r\n", tmp);
	}
	print_queue_info(&Queue);


	printf("---------------------------------\r\n");

	printf ("tmp=%d\r\n",  tmp);
	printf ("get 1 element overrun. ret=%d\r\n", queue_get(&Queue, &tmp));
	printf ("tmp=%d\r\n",  tmp);

	printf("---------------------------------\r\n");
	printf("get 5 elements:\r\n");
	for (int i=0; i<TEST_OBJS_NUM/2; i++)
	{ 	
		queue_get(&Queue, &tmp);
		printf("getval = %d\r\n", tmp);
	}
	print_queue_info(&Queue);
}
