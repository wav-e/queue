#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdint.h>
/**
 * @file    queue.h
 * @brief   Universal queue module.
 * 
 *   This queue module can contains any size objects (one queue one size)
 *   It can be just uint8_t/uint16_t/uint32_t, or any size struct. 
 *   
 * Using:
 *  @code{.c}
 * void main(void)
 * {
 *    QUEUE_T q;
 *    uint16 buf16[128];
 *    uint16 tmp_var;
 *  
 *    queue_init(&q, buf16, sizeof(buf16), sizeof(uint16_t));
 * 
 *    tmp = 0xDEAD;
 *    queue_put(&q, &tmp);
 *
 *    tmp = 0xBEEF;
 *    queue_put(&q, &tmp);
 * 
 *    tmp=0;
 *    queue_get(&q, &tmp);
 *    printf("tmp = 0x%X\n",tmp); //it prints "tmp = 0xDEAD"
 *    
 *    queue_get(&q, &tmp);
 *    printf("tmp = 0x%X\n",tmp); //it prints "tmp = 0xBEEF" 
 *    
 * }
 * @endcode
 */


/**
 * Use 1/2/4 bytes for copy instead memset.
 * 
 * @note If queue used 1, 2 or 4 bytes size, it will increase 
 *       performance of data copy
 */
#define QUEUE_OPTIMISE 1
#define QUEUE_DEBUG    1




/**
 * @brief   queue instance
 * 
 * @todo buf is needed?
 */
typedef struct
{
    void        *buf;       /// pointer to static memory buf

	void 		*startPtr;  /// start buffer pointer
	void 		*endPtr;    /// last element pointer

	void 		*getPtr;    /// pointer for get from queue
	void 		*putPtr;    /// pointer for put to queue
	uint32_t	numObjs; 	/// added objects into queue
    uint32_t    maxObjs;    /// max objects in queue

	uint32_t    qSize;      /// size of one object
	uint8_t 	isInit;     /// init flag
	uint8_t 	isLocked;   /// prevent "race" flag
}QUEUE_T;




typedef enum
{
	QUEUE_OK = 0,
	QUEUE_NO_MEM_ERR,
	QUEUE_NULL_ERR,
	QUEUE_INIT_ERR,
	QUEUE_EMPTY_ERR
}QUEUE_ERROR;

/**
 * @brief   Put object to queue.
 * @param   q pointer to queue struct.
 * @param   obj Pointer to object you want put, It can be *uint8_t, *uint16_t, 
 *          *uint32_t, or any size struct pointer, but it must match with 
 * 			objSize that was set when queue init.
 *
 * @retval  Error if not 0.
 */
QUEUE_ERROR queue_put(QUEUE_T *q, void *obj);


/**
 * @brief   Put object to head queue.
 * @param   q pointer to queue struct.
 * @param   obj Pointer to object you want put, It can be *uint8_t, *uint16_t, 
 *          *uint32_t, or any size struct pointer, but it must match with 
 *          objSize that was set when queue init.
 * 
 * @note    if queue is not full you can put object in head (like stack)
 *
 * @retval  Error if not 0.
 */
QUEUE_ERROR queue_putToHead(QUEUE_T *q, void *obj);


/**
 * @brief   Get object from queue.
 * @param   q pointer to queue struct.
 * @param   obj pointer to object you want get, It can be *uint8_t, *uint16_t, 
 *          *uint32_t, or any size struct pointer, but it must match with 
 *          objSize that was set when queue init.
 *
 * @retval  Error if not 0.
 */
QUEUE_ERROR queue_get(QUEUE_T *q, void *obj);


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
QUEUE_ERROR queue_init(QUEUE_T *q, void *buf, uint32_t buf_size_bytes, uint32_t objSize);


/**
 * @brief   Get num of putt objects in queue.
 * @retval  Num objects in queue.
 */
uint32_t queue_get_num(QUEUE_T *q);

#endif