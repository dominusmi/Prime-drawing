#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <SDL/SDL.h>
#include "header.h"

#define NO_STDIO_REDIRECT

int main ( int argc, char** argv )
{
    freopen( "CON", "w", stdout );
    freopen( "CON", "w", stderr );
    printf("\n");
    switch(argc)
    {
    case 1 :
        SIDE = 640;
        break;

    case 2 :
        SIDE = atoi(argv[1]);
        if(SIDE == 0)
        {
            printf("Invalid argument : %s\n", argv[1]);
            return -1;
        }
        break;

    default :
        printf("Usage : \"primeDrawing\" [side length]\n");
        printf("            (if no arguments, default side length is 640)\n");
        return -1;
    }
    /*if(argc == 2)
        SIDE = atoi(argv[1]);

    if(argc == 1)
        SIDE = 640;

    else(argc != 1 && argc != 2)
    {
        printf("Usage : \"primeDrawing\" [side length]\n");
        printf("    if no arguments, default side length is 640\n");
        return -1;
    }*/

    printf("Side length set to %d\n", SIDE);
    SIDESQUARED = SIDE * SIDE;

    int *primes = (int*) malloc(SIDESQUARED * sizeof(int));
    int *numbers = (int*) malloc(sizeof(int) * SIDESQUARED);
    if(primes == NULL || numbers == NULL)
    {
        printf("Error when reserving memory for arrays");
        return -1;
    }

    // finds the prime numbers and sets up the two arrays
    findPrimes(primes, numbers);

    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // create a new window
    SDL_Surface* screen = SDL_SetVideoMode(SIDE, SIDE, 16,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        char dimensions[20] = "";
        sprintf(dimensions, "%dx%d", SIDE, SIDE);
        printf("Unable to set %s video: %s\n", dimensions, SDL_GetError());
        return 1;
    }

    // a black pixel surface to print on the screen
    SDL_Surface* pixelSurface = SDL_LoadBMP("pixel.bmp");

    // load an image
    if (!pixelSurface)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }


    // make sure SDL cleans up before exit
    atexit(SDL_Quit);


        // fills the screen with white
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 255, 255, 255));

        SDL_Rect dstrect;
        // set up first coordinate
        dstrect.x = SIDE/2-1;
        dstrect.y = SIDE/2;

        int i;
        //main loop
        for(i=1; i<SIDESQUARED; i++)
        {

            // "paints" the pixel if it's a prime
            if(numbers[i]==1)
                SDL_BlitSurface(pixelSurface, 0, screen, &dstrect);

            dstrect = findNextPixel();
            //SDL_Delay(10);
        }
        printf("Finished\n");



    // finally, update the screen :)
    SDL_Flip(screen);
    // Save the screen
    printf("Screenshot : %s\n", ( SDL_SaveBMP(screen, "Prime drawing.bmp") == 0 ) ? "Succesful" : "Unsuccesful");
    // free loaded bitmap
    SDL_FreeSurface(pixelSurface);
    //free other arrays
    free(primes);
    free(numbers);
    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}


//Sets the position of the next pixel to draw
SDL_Rect findNextPixel()
{

    static Sint16 posX = SIDE/2-1, posY = SIDE/2;

    if(step == sideLength)
    {
        step = 2;
        switch(stage)
        {
            case 0:
                posX++;
                break;
            case 1:
                posY++;
                break;
            case 2:
                posX--;
                break;
            case 3:
                posX--;
                step = 2;
                break;
        }
        stage++;
        stage = stage%4;
        if(stage == 0)
            sideLength+=2;
    }
    else
    {
        step++;
        switch(stage)
        {
            case 0:
                posY--;
                break;
            case 1:
                posX++;
                break;
            case 2:
                posY++;
                break;
            case 3:
                posX--;
                break;
        }
    }

    SDL_Rect returnRect;
    returnRect.x = posX;
    returnRect.y = posY;

    return returnRect;
}

//finds all the prime numbers and puts them in an array
void findPrimes(int *primes, int *numbers)
{
    primes[0] = 2;
    primes[1] = 3;
    int num = 5;
    int j = 0;
    int i = 2;
    int k = 1;
    int prime = 1;
    int decimal = (int)((double)(SIDESQUARED))/10;

    //file to output prime numbers
    FILE *primesFile = fopen("Prime numbers.nano", "w+");

    printf("Calculating prime numbers up to %d\n", SIDESQUARED);

    //setting up first few primes

    numbers[0]=0;
    numbers[1]=1;
    numbers[2]=1;
    numbers[3]=0;

    while(num<SIDESQUARED)
    {
        if(num == decimal*k || num == decimal*k-1)
        {
            printf("=");
            k++;
        }

        prime = 1;
        for(j=0; j<i; j++)
        {
            if(num % primes[j] == 0)
            {
                prime = 0;
                break;
            }
        }
        if(prime)
        {
            primes[i] = num;
            i++;
            fprintf(primesFile, "%d\n", num);
            numbers[num-1] = 1;
        }
        else
            numbers[num-1] = 0;

        num+=2;
    }
    fclose(primesFile);
    printf("\n");
}
