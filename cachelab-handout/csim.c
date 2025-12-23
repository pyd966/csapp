#include "cachelab.h"
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef long long ll;

int hitCnt,missCnt,evictCnt;
int s,S,E,b,B,v,h;
char *fileAddr;
ll setMask,tagMask,blockMask;
int timeCnt;
int **memToTag,**lstUse;
bool **used;

void praseParameters() ;
int myAccess(ll addr) ;

int main(int argc,char *argv[]){

    praseParameters(argc,argv);

    blockMask = B - 1;
    setMask = (S - 1) << b;
    tagMask = (~0ll) - blockMask - setMask;

    memToTag = (int**)malloc(S * sizeof(int*));
    for (int i = 0;i < S;i++){
        memToTag[i] = (int*)malloc(E * sizeof(int));
        for (int j = 0;j < E;j++) memToTag[i][j] = 0;
    }

    lstUse = (int**)malloc(S * sizeof(int*));
    for (int i = 0;i < S;i++){
        lstUse[i] = (int*)malloc(E * sizeof(int));
        for (int j = 0;j < E;j++) lstUse[i][j] = 0;
    }

    used = (bool**)malloc(S * sizeof(bool*));
    for (int i = 0;i < S;i++){
        used[i] = (bool*)malloc(E * sizeof(bool));
        for (int j = 0;j < E;j++) used[i][j] = 0;
    }

    if (h){
        printf("-h detected.\n");
    }

    if (v){
        printf("-v detected.\n");
        printf("set number S=%d\n",S);
        printf("line number E=%d\n",E);
        printf("block size B=%d\n",B);
        printf("opening file %s\n",fileAddr);
    }

    freopen(fileAddr,"r",stdin);
    char *sline = (char *)malloc(sizeof(char) * 1000);
    while (fgets(sline,sizeof(char) * 1000,stdin) != NULL){
        if (sline[0] != ' ') continue;
        sline[(int)(strlen(sline)) - 1] = '\0';
        ll addr = 0;
        sscanf(sline + 2," %llx ",&addr);
        int flag = 0;
        switch (sline[1]){
            case 'M':
                flag |= myAccess(addr);
            case 'L':
            case 'S':
                flag |= myAccess(addr);
                break;
        }
        if (v){
            printf("%s ",sline);
            if (flag & 1) printf("miss ");
            if (flag & 2) printf("eviction ");
            if (flag & 4) printf("hit");
            puts("");
        }
    }

    fclose(stdin);

    printSummary(hitCnt, missCnt, evictCnt);
    return 0;
}

void praseParameters(int argc,char *argv[]){
    int opt;
    while ((opt = getopt(argc,argv,"vhs:E:b:t:")) != -1){
        switch (opt){
            case 'h':
                h = 1;
                break;
            case 'v':
                v = 1;
                break;
            case 's':
                {
                    int tmp;
                    sscanf(optarg,"%d",&tmp);
                    s = tmp,S = 1 << s;
                    break;
                }
            case 'E':
                {
                    int tmp;
                    sscanf(optarg,"%d",&tmp);
                    E = tmp;
                    break;
                }
            case 'b':
                {
                    int tmp;
                    sscanf(optarg,"%d",&tmp);
                    b = tmp,B = 1 << b;
                    break;
                }
            case 't':
                {
                    int len = strlen(optarg);
                    fileAddr = malloc(sizeof(char) * len);
                    strcpy(fileAddr,optarg);
                    break;
                }
            case '?':
                printf("Unknown argument: %c\n",optopt);
                exit(0);
                break;
        }
    }
}

int myAccess(ll addr){
    ++timeCnt;
    int ret = 0;

    int nTag = (addr & tagMask) >> (s + b);
    int nSet = (addr & setMask) >> b;
    
    int tLine = -1;

    for (int line = 0;line < E;line++){
        if (used[nSet][line] && memToTag[nSet][line] == nTag){
            tLine = line;
            break;
        }
    }
    
    if (tLine != -1){
        ret |= 4;
        ++hitCnt;
        lstUse[nSet][tLine] = timeCnt;
        return ret;
    }

    ++missCnt;
    ret |= 1;

    tLine = -1;
    for (int line = 0;line < E;line++){
        if (!used[nSet][line]) {tLine = line;break;}
    }
    
    if (tLine != -1){
        used[nSet][tLine] = 1;
        memToTag[nSet][tLine] = nTag;
        lstUse[nSet][tLine] = timeCnt;
        return ret;
    }

    ++evictCnt;
    ret |= 2;

    int mnUse = lstUse[nSet][0];
    tLine = 0;
    for (int line = 0;line < E;line++){
        if (lstUse[nSet][line] < mnUse){
            mnUse = lstUse[nSet][line];
            tLine = line;
        }
    }

    lstUse[nSet][tLine] = timeCnt;
    memToTag[nSet][tLine] = nTag;

    return ret;
}