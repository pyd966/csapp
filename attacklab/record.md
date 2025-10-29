## 题外话

如果你不小心没有读网站上的 note 的话，那你会得到 `Initialization error: Running on an illegal host`。

只需要使用 `-q` 运行即可。

## phase1

我们先看一下 `BUFFER_SIZE` 是多少。

通过检查 `getbuf`，可以发现我们有 0x28=40 的空间。

找到 `touch1` 的地址 `0x004017c0` 通过 overflow 的方式注入到 ret addr 的位置即可。

注意小端序的神秘顺序。

## phase2

阅读文档，我们需要调用 `touch2` 并且把自己的 cookie 作为第一个参数传进去。

那说明我们需要在调用 `touch2` 之前把 `%rdi` 设置成 cookie。

所以，我们在栈里注入代码

```asm
mov [cookie],%rdi
push [touch2addr]
ret
```

这样就可以了，然后把 `getbuf` 的 `ret addr` 改成我们注入代码的地址。

## phase3

跑一下程序，发现 phase3 我们的 cookie 是 `0x59b997fa`

如果直接按照 `[code][string]` 的方式注入 buf，由于 `strncmp,hexmatch` 都会往栈里 `push`，会把字符串覆写。

如果按照 `[string][code]` 方式注入，仍然会覆写。

考虑在周围找一块没被用过的地方。

我们有两个思路。第一个思路是往前，把 `%rsp` 直接提前减很多；另一个思路是往后，在后面找一个空的地方放东西。

实现第一个思路的时候，不知道为什么 `hexmatch` 里面调用 `sprintf` 会报错。估计是用到了映射的内存区域。

实现第二个思路成功了。

## phase4

仍然实现 phase2 的任务，只不过是改用 return-oriented 的方式。

我们只能把 cookie 写到栈里，所以势必要有一个指令从栈里读取数据存到寄存器，那只有 `popq`。

通过查找，我们发现可以实现 `popq %rax` 和 `movq %rax,%rdi`。那已经赢了。

最后的结构是 `[ret addr to popq][cookie][ret addr to movq][ret addr to touch2]`。

## phase5

还是 phase3 的任务。

因为我们只能把东西写到栈里面，所以我们的字符串肯定是要存到栈里的。

问题出在，由于栈的开始位置是随机化的，所以我们没办法知道字符串存放的位置。

但是 `%rsp` 知道，我们的想法就是获得 `%rsp` 然后进行一些加减操作，就能算出字符串的位置。

观察代码，它送了一个 `addxy`，我们利用这个能进行加减。

最后一个问题是 `movl,movq` 这些指令是不全的，但是你找找有哪些能用的，拼一拼就能拼出来想要的效果，最后我们的结构是:

`[movq %rsp,%rax][movq %rax,%rdi][popq %rax][bias(=72)][movl %eax,%edx][movl %edx,%ecx][movl %ecx,%esi][addxy][movq %rax,%rdi][touch3][mystring]`