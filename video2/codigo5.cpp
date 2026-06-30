#include <stdio.h>

int funB(int n)
{
    if(n>100)
    return n-19;
    return funB(funB(n+11));
}

int main() {
    int r;
    r=funB(101);
    printf("%d\n",r);
    return 0;
}