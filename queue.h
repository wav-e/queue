#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdint.h>

/**
 * Use 1/2/4 bytes to copy instead memset.
 * 
 * @note If queue used 1, 2 or 4 bytes size, it will increase 
 *       performance of data copy
 */
#define QUEUE_OPTIMISE 1


typedef struct
{
	void 		*startPtr;
	void 		*endPtr;
	void 		*buf;
	void 		*getPtr;
	void 		*putPtr;
	uint32_t	numObjs; 	/// added elements in queue
    uint32_t    maxObjs;    /// max objects in queue

	uint32_t    qSize;      /// size of one element
	uint8_t 	isInit;
	uint8_t 	isLocked;   ///prevent "race" flag
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
uint32_t queue_get_num(QUEUE_T *q)

#endif