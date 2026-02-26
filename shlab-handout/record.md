## Intro

先读了一下 writeup 和 tsh.c。这个 lab 的难点就在于如何处理各个前台/后台任务的等待，以及如何编写 handler。

我们实现的思路还是照着 trace 的顺序来。

## trace02

简单写一个 `eval` 的流程，调用 `parseline` 解析参数，然后调用 `builtin_cmd` 检查是否是内置函数，不是的话之后再说。

然后，写一下 `builtin_cmd` 里的 `quit` 即可。

## trace03

实现前台任务。

有一个疑问：是否保证任务对应的程序一定存在？似乎不是，这是在 trace14 里考虑的，我们先不管。

那我们只需要在 `eval` 里 `fork` & `execve` 即可，注意改一下儿子的 pgid。

顺便写一下 `waitfg`。writeup 里的 hint 说的很对，我们应该把 reap child 的工作全部交给 SIGCHLD 来做，这里只需要循环等待，并且检查 jobs 就行了。

那我们还需要简单写一下 `sigchld_handler`，顺便建立一下 jobs 机制。

这里要注意，在创建子进程、把它加入到 joblist 之前，需要把 SIGCHLD 屏蔽掉，否则可能会出现 race，某个 job 在加入到 joblist 之前就被 delete。

## trace04

实际上实现前台任务的时候这部分已经实现过了。只需要注意一点：writeup 里没说，但是当后台任务被创建时，需要输出一行信息。

## trace05

实现一下 `builtin_cmd` `jobs`，直接调用 `listjobs` 就行了。

## trace06 & 07

这里要求我们支持发送 SIGINT。那我们需要实现两方面内容：`sigint_handler`，以及在 `sigchld_handler` 里增加输出子进程异常退出信息的机制。

## trace08

要求支持发送 SIGTSTP。

注意这里需要我们把对应 job 的 state 置为 ST，建议在 `sigchld_handler` 中进行这步操作。并且略微修改 `waitfg` 为对应 pid 不存在或者状态不为 FG。

## trace09

要求支持 `bg` 命令。

同样地遵循先前的规范，在 `do_bgfg` 中只发送 SIGCONT 信号，而在 `sigchld_handler` 中处理。

## trace10

要求支持 `fg` 命令。

好吧我们不能在 `sigchld_handler` 中输出某个任务被继续的信息，因为 `fg` 命令不输出这些。

## trace11 & 12 & 13

使用 `kill` 发送信号时向整个 group 发送即可。

## trace14

错误处理。

包括：

- 不存在的程序；
- `fg,bg` 缺少参数，或者参数错误，或者对应进程不存在；

## trace 15 & 16

之前写得好的话这里都不用动即可直接通过。