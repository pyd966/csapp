int inp[6];(rsp)

r14=rsp
cnt(r12d)=0

while (rbx <= 5){
    assert(1<=*r13<=6)
    cnt++
    if cnt==6 break;
    for (int rbx = cnt;rbx <= 5;rbx++){
        int tmp = inp[rbx];
        assert(inp[rbx] != *r13)
    }
    r13++
}//input 构成一个基环树，要求不能够有自环。
for (end=inp+6,cur=inp,rcx=7;cur!=end;cur++){
    *cur=7-*cur;
}
esi=0
while (rsi!=24){
    rdx=0x6032d0
    for (int i = 1;i < inp[rsi];i++) rdx=*(rdx+8)
    inp[rsi*2+32]=rdx
    rsi+=4
}

rbx=*inp+32    rbx pt1
rax=*inp+40    rax pt2
rsi=*inp+80    rsi ptend
rcx=rbx        rcx pt1

rcx lst rax cur
do{
    rdx=*rax    Node cur,*lst
    rcx->to = rdx
    rax+=8
    rcx=rdx
}while(rsi!=rax)
*rdx+8=0

ebp=5
do{
    rax=*(rbx+8)
    rax=*rax
    assert(*rbx>=rax)
    rbx=*(rbx+8)
    ebp--
}while(ebp!=0)
