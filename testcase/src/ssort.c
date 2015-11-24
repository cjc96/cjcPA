#include "trap.h"

int a[] = {7,295,3,1,2,8,2,5};
int b[] = {1,2,2,3,5,7,8,295};

void swap(int *x,int *y)
{
    int temp;
    
    temp = *x; *x = *y; *y = temp;
}

int main()
{
    int i , j;
    for (i = 0; i < 7; i++)
        for (j = i+1; j < 8; j++)
            if (a[i] > a[j])
                swap(&a[i],&a[j]);
    for (i = 0; i < 8; i++)
        nemu_assert(a[i] == b[i]);
       
    HIT_GOOD_TRAP;
    
    return 0;
}
