#ifndef RING_BUFFER_H__
#define RING_BUFFER_H__

#include <stdint.h>


typedef struct {
    uint8_t  *buf;
    uint16_t head, tail,len;
} RingBuffer;

typedef enum {
    RING_BUFFER_OK = 0x0,
    RING_BUFFER_FULL,
    RING_BUFFER_NO_SUFFICIENT_SPACE
} RingBuffer_Status;

uint16_t RingBuffer_GetDataLength(RingBuffer *buf);
uint16_t RingBuffer_GetFreeSpace(RingBuffer *buf);
void RingBuffer_Init(RingBuffer *buf,uint8_t * data,uint16_t len);
uint16_t RingBuffer_Read(RingBuffer *buf, uint8_t *data, uint16_t len);
uint8_t RingBuffer_Write(RingBuffer *buf, uint8_t *data, uint16_t len);
uint16_t RingBuffer_Find_Byte(RingBuffer *buf,uint8_t data);
#endif //#ifndef RING_BUFFER_H__
