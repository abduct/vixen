#ifndef __RESCALE_H__
#define __RESCALE_H__

static inline uint32_t rescale(uint32_t value, uint8_t bits_from, uint8_t bits_to)
{
  return value / ( 1 << (bits_from - bits_to));
}

#endif