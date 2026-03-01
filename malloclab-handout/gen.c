#include <stdio.h>

int MAX = 61000, STEP = 16, TO = 2048;

int main(){
    int p = 0,flag = 0,cnt = 0;
    printf("const int LIST_DIVIDER[LIST_SIZE] = {");
    while (p < TO){
        if (flag) printf(", ");
        printf("%d",p + 1);
        p += STEP;
        ++cnt,flag = 1;
    }
    while (p <= MAX){
        if (flag) printf(", ");
        printf("%d",p + 1);
        p *= 2;
        ++cnt,flag = 1;
    }
    printf("};\n");
    printf("LIST_SIZE = %d\n",cnt);
    return 0;
}