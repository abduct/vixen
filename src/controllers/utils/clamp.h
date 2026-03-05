#ifndef __CLAMP_H__
#define __CLAMP_H__

static inline int clamp(int value, int min, int max)
{
  if (value <= min)
    return min;
  if (value >= max)
    return max;
  return value;
}

#endif