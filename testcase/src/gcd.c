#include "trap.h"

int a = 320,b = 30;

int gcd(int a,int b)
{   
    if (b != 0)
        return gcd(b,a%b);
    else
        return a;
}

int main()
{

    nemu_assert(gcd(a,b) == 10);
    
    HIT_GOOD_TRAP;
    
    return 0;
}
