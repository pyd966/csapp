## Intro

官方下载文件中没有提供 `traces/` 文件夹，不过可以在 github 上[下载](https://github.com/Fanziyang-v/CSAPP-Lab/tree/main/Malloc-Lab/traces)。

## Design

我们先尝试按照教材讲的技术实现，之后再尝试改进。

注意到改进过程中，修改 free list 的结构是困难的，所以我们一定要先想好。

基本上只有 doubly-linked free list 一个选项。

然后再决定要实现教材中的哪个技术。

显然要用 segregated list，好处在于每个 list 管辖的 size 范围可调，这里存在一定调参优化空间。

- find strategy: first-fit
- split strategy: 除非低于一个最小值，否则总是 split
- coalesce strategy: always。
- realloc strategy: 按照 size 变化分讨。变小就原地，split 后 coalesce；变大就先看加上后一个块行不行，如果不行就 free + malloc。

这样应该能实现一个效率还不错的 allocator。

## Implement

注释里写得很清楚了。

## Assign

调参。

我们发现时间完全不是瓶颈，所以可以大胆设置更多的 free lists。不过实践表明优化不显著。

发现对 `binary.bel` util 效率过低，肉眼观察发现这是因为 `binary.bel` 先分配了很多接近 2 的次幂的，中间用 64 隔开，然后 free 掉，最后分配了一堆是 2 的次幂的。这会导致之前声明的完全没办法用。

所以只需要把接近 2 的次幂的调整到 2 的次幂就行了。`binary2` 是类似的。

发现对 `realloc.bel` util 效率过低，肉眼观察模式，发现只要最开始多声明一点空间就行了。

`realloc2` 有点烦，但是整体思路是预留一个 5/10 的位置就可以了。

我没写针对 `realloc2` 的优化，最后是 95pts。加了之后感觉就 98-99 了。