#include <stdio.h>

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(void)
{
  // The entire range of pressures covered by our dial
  for (float p = 931.26; p <= 1066.71; p+= 0.1) {
    long s = map(p * 100, 93126, 106671, 0, 756);
    printf("In: %f out: %lu\n", p, s);
  }
  return 0;
}

