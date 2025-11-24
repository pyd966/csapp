#include <stdio.h>

int n = 2;

int main(){
    printf("n=%d\n",n);
    --n;
    if (n != 0) main();
    return 0;
}