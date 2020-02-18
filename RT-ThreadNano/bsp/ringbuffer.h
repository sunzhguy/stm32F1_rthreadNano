#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <rtthread.h>
#include <string.h>

struct rt_ringbuffer
{
    rt_uint8_t *buffer_ptr;
    rt_uint16_t read_mirror : 1;
    rt_uint16_t read_index : 15;
    rt_uint16_t write_mirror : 1;
    rt_uint16_t write_index : 15;
    rt_int16_t buffer_size;
};


void rt_ringbuffer_init(struct rt_ringbuffer *rb,
                        rt_uint8_t           *pool,
                        rt_int16_t            size);
rt_size_t rt_ringbuffer_putchar(struct rt_ringbuffer *rb, const rt_uint8_t ch);
rt_size_t rt_ringbuffer_getchar(struct rt_ringbuffer *rb, rt_uint8_t *ch);
#endif
