#include "ringbuffer.h"
#include <string.h>

uint16_t RingBuffer_GetFreeSpace(RingBuffer *buf) {
    if(buf->tail == buf->head)
        return (buf->len) - 1;

    if(buf->head > buf->tail)
        return (buf->len) - ((buf->head - buf->tail) + 1);
    else
        return  (buf->tail - buf->head) - 1;
}

uint16_t RingBuffer_GetDataLength(RingBuffer *buf) {
    return (buf->len) - (RingBuffer_GetFreeSpace(buf) + 1);
}


void RingBuffer_Init(RingBuffer *buf,uint8_t * data,uint16_t len) {
    if(!buf || !data || (len < 2))
        return;
    
    buf->head = buf->tail = 0;
    buf->buf = data;
    buf->len = len;
    memset(buf->buf, 0, len);
}

uint16_t RingBuffer_Read(RingBuffer *buf, uint8_t *data, uint16_t len) {
    uint16_t counter = 0;

    while(buf->tail != buf->head && counter < len) {
        data[counter++] = buf->buf[buf->tail];
        buf->tail = (buf->tail + 1) % (buf->len);
    }
    return counter;
}

uint16_t RingBuffer_Find_Byte(RingBuffer *buf,uint8_t data){
    uint16_t counter = 0;
    uint16_t len = RingBuffer_GetDataLength(buf);

    while(counter < len){
        if(data == buf->buf[(buf->tail + counter)%((buf->len))]){
            return (counter+1);
        }else{
            counter++;
        }
    }   
    return 0;
}

uint8_t RingBuffer_Write(RingBuffer *buf, uint8_t *data, uint16_t len) {
    uint16_t counter = 0;
    uint16_t freeSpace = RingBuffer_GetFreeSpace(buf);

    if(freeSpace == 0)
        return RING_BUFFER_FULL;
    else if (freeSpace < len)
        return RING_BUFFER_NO_SUFFICIENT_SPACE;

    while(counter < len) {
        buf->buf[buf->head] = data[counter++];
        buf->head = (buf->head + 1) % (buf->len);
    }
    return RING_BUFFER_OK;
}
