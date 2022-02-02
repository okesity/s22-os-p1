
#include <stdlib.h>
#include <stdio.h>
#include "extern.h"

void exit(int status) 
{
    panic("***** Game exited, reboot required.");
    while(1) {};
}

int random_int(void) 
{
    return 0;
}

void errx(int a, ...) 
{
    panic("***** Entered errx(), all is lost!");
    while(1) {};
}

void err(int a, char *b) 
{
    panic("***** Entered err(), all is lost!");
    while(1) {};
}

void abort(void) 
{
    panic("***** Entered abort(), all is lost!");
    while(1) {};
}
