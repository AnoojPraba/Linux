#include<stdio.h>
/*
 *  ip-> 0xDCBA
 *  op -> 0xBACD
 */
void Function1()
{
    int ip=0xdcba;
    int op=((ip<<8 & 0xFF00)|(ip>>12)|(ip>>4 &0xf0));
    printf("0x%4x\n",op);
}
int main(void)
{
    Function1();
    return 0;
}
