#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "queue.h"

/* can be modified with semaphore */
void queue_lock(QUEUE_T *q)
{
	q->isLocked = 1;
}
void queue_unlock(QUEUE_T *q)
{
	q->isLocked = 0;
}



/**
 * @brief   Put object to queue.
 * @param   q pointer to queue struct.
 * @param   obj Pointer to object you want put, It can be *uint8_t, *uint16_t, 
 *          *uint32_t, or any size struct pointer, but it must match with 
 * 			objSize that was set when queue init.
 *
 * @retval  Error if not 0.
 */
QUEUE_ERROR queue_put(QUEUE_T *q, void *obj)
{
    if (!q->isInit)
        return QUEUE_INIT_ERR;

    if (q->numObjs == q->maxObjs)
        return QUEUE_NO_MEM_ERR;

	while(q->isLocked)
	{;}

	queue_lock(q);


#if QUEUE_OPTIMISE	
	if(1 == q->qSize)
	{
		*((uint8_t*)(q->putPtr)) = *((uint8_t*)(obj));
	}
	else if (2 == q->qSize)
	{
		*((uint16_t*)(q->putPtr))=*((uint16_t*)(obj));
	}
	else if (4 == q->qSize)
	{
		*((uint32_t*)(q->putPtr))=*((uint32_t*)(obj));
	}
	else
	{
		memcpy(q->putPtr, obj, q->qSize);
	}
#else
		memcpy(q->putPtr, obj, q->qSize);
#endif

	q->numObjs++;
	
	//loop queue
	q->putPtr = (q->endPtr==q->putPtr)? (q->startPtr) : (q->putPtr+q->qSize);

	queue_unlock(q);  

	return QUEUE_OK;
}

/**
 * @brief   Put object to head queue.
 * @param   q pointer to queue struct.
 * @param   obj Pointer to object you want put, It can be *uint8_t, *uint16_t, 
 *          *uint32_t, or any size struct pointer, but it must match with 
 * 			objSize that was set when queue init.
 * 
 * @note	if queue is not full you can put object in head (like stack)
 *
 * @retval  Error if not 0.
 */
QUEUE_ERROR queue_putToHead(QUEUE_T *q, void *obj)
{
    if (!q->isInit)
        return QUEUE_INIT_ERR;

    if (q->numObjs == q->maxObjs)
        return QUEUE_NO_MEM_ERR;

	while(q->isLocked)
	{;}

	queue_lock(q);

	/* shift getPtr back and write data into *(getPtr) */
    q->getPtr = (q->getPtr==q->startPtr)? (q->endPtr) : (q->getPtr-q->qSize);

#if QUEUE_OPTIMISE	
	if(1 == q->qSize)
	{
		*((uint8_t*)(q->getPtr)) = *((uint8_t*)(obj));
	}
	else if (2 == q->qSize)
	{
		*((uint16_t*)(q->getPtr))=*((uint16_t*)(obj));
	}
	else if (4 == q->qSize)
	{
		*((uint32_t*)(q->getPtr))=*((uint32_t*)(obj));
	}
	else
	{
		memcpy(q->getPtr, obj, q->qSize);
	}
#else
		memcpy(q->getPtr, obj, q->qSize);
#endif

	q->numObjs++;
	
	/* put ptr is not shifting */
	//q->putPtr = (q->endPtr==q->putPtr)? (q->startPtr) : (q->putPtr+q->qSize);

	queue_unlock(q);  

	return QUEUE_OK;
}

/**
 * @brief   Get object from queue.
 * @param   q pointer to queue struct.
 * @param   obj pointer to object you want get, It can be *uint8_t, *uint16_t, 
 *          *uint32_t, or any size struct pointer, but it must match with 
 *          objSize that was set when queue init.
 *
 * @retval  Error if not 0.
 */
QUEUE_ERROR queue_get(QUEUE_T *q, void *obj)
{
    if (!q->isInit) 
        return QUEUE_INIT_ERR;

    if (0 == q->numObjs)
        return QUEUE_EMPTY_ERR;

    while(q->isLocked)
    {;}

    queue_lock(q);

#if QUEUE_OPTIMISE
	if(1 == q->qSize)
	{
	 	*((uint8_t*)(obj)) = *((uint8_t*)(q->getPtr));
	}
	else if (2 == q->qSize)
	{
		*((uint16_t*)(obj)) = *((uint16_t*)(q->getPtr));
	}
	else if (4 == q->qSize)
	{
		*((uint32_t*)(obj)) = *((uint32_t*)(q->getPtr));
	}
	else
	{
		memcpy(q->getPtr, obj, q->qSize);
	}
#else
    memcpy(obj, q->getPtr, q->qSize);
#endif

#if QUEUE_DEBUG
    memset(q->getPtr, 0xAA , q->qSize);
#endif

    q->getPtr = (q->getPtr==q->endPtr)? (q->startPtr) : (q->getPtr+q->qSize);
    q->numObjs--;

    queue_unlock(q);  

    return QUEUE_OK;
}


/**
 * @brief   Get num of putt objects in queue.
 * @retval  Num objects in queue.
 */
uint32_t queue_get_num(QUEUE_T *q)
{
	if (NULL==q)
        return 0;

    return q->numObjs;
}

/**
 * @brief   Init queue.
 * @param   q pointer to queue struct
 * @param   buf pointer to queue buffer
 * @param   buf_size_bytes size of buf
 * @param   objSize size of one object in queue in bytes
 * @retval  Error if not 0.
 * 
 * @todo align end pointer 
 */
QUEUE_ERROR queue_init(QUEUE_T *q, void *buf, uint32_t buf_size_bytes, uint32_t objSize)
{

	uint8_t endPtr;
	if ((NULL==q) || (NULL == buf))
		return QUEUE_NULL_ERR;

	if (objSize > buf_size_bytes)
		return QUEUE_NO_MEM_ERR;

	q->qSize = objSize;
	q->buf = buf;

	q->getPtr = buf;
	q->putPtr = buf;

	q->startPtr = buf;

	q->numObjs = 0;
	q->maxObjs = buf_size_bytes/objSize;

	//todo need to fix
	q->endPtr = ((uint8_t*)buf) + (buf_size_bytes-objSize);

	if(buf_size_bytes%objSize)
		printf("buf size not equal to objects\r\n");
	
	q->isInit = 1;
	q->isLocked = 0;
	return QUEUE_OK;
}


