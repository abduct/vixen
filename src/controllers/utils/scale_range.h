#ifndef __SCALE_RANGE_H__
#define __SCALE_RANGE_H__

static inline uint8_t scale_range(int32_t value, uint16_t min, uint16_t max)
{

  uint32_t range = max - min;

  if (value < min) value = min;
  if (value > max) value = max;

  uint8_t mval = value - min;//(range / 2);

  if (mval < 0) mval = 0;
  if (mval > range) mval = range;

  float percent = (float)mval / ((float)range / 100.f);

  return percent * 2.55f;
}

#endif