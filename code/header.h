#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED


SDL_Rect findNextPixel();

int SIDE=640;
int SIDESQUARED = SIDE * SIDE;

void findPrimes(int *primes, int *numbers);

static int step = 2, sideLength = 3, stage = 0;

#endif // HEADER_H_INCLUDED
