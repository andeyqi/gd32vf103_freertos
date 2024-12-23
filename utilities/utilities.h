#ifndef __UTILITIES_H__
#define __UTILITIES_H__
#include <stdint.h>

#define UNUSED(x) (void)x 

/* 32bit memory read macor */
#define UTILITIES_REG_READ32(address)     (*(volatile uint32_t*)(address))

#define UTILITIES_REG_BIT_READ32(address,mask)     ((*(volatile uint32_t*)(address))&((uint32_t)(mask)))

/* error code definitions */
#define RET_EOK                          0               /**< There is no error */
#define RET_ERROR                        1               /**< A generic error happens */
#define RET_ETIMEOUT                     2               /**< Timed out */
#define RET_EFULL                        3               /**< The resource is full */
#define RET_EEMPTY                       4               /**< The resource is empty */
#define RET_ENOMEM                       5               /**< No memory */
#define RET_ENOSYS                       6               /**< No system */
#define RET_EBUSY                        7               /**< Busy */
#define RET_EIO                          8               /**< IO error */
#define RET_EINTR                        9               /**< Interrupted system call */
#define RET_EINVAL                       10              /**< Invalid argument */

int myatoi(const char* s);
int myatohex(const char* s);
unsigned int myatohexuint(const char* s);
long long int myatol(const char* s);

#endif /* end of __UTILITIES_H__ */