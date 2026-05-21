# Linux一切皆文件

./hello &
& 让程序后台运行

    PID TTY          TIME CMD
   9053 pts/4    00:00:00 bash
  11466 pts/4    00:00:40 hello
  11652 pts/4    00:00:00 ps

在路径 /proc下面可以找到进程的状态
说明**进程也是一个文件**
hjy@hjy:~/work/Embedded-Linux-Dev-Notes/KelpBar/04_FileIO/build$ cat /proc/11466/status
Name:   hello
Umask:  0022
State:  R (running)
Tgid:   11466

我们运行
cat /dev/tty > output.txt
然后通过键盘输入内容，按下回车后，就可以看到output.txt中写入了输入内容。


## Linux系统架构

### 用户空间 (User Space)
所有的应用程序，如浏览器、文档编辑器或音乐播放器都运行在这个空间。
- 安全性：用户空间的程序运行在受限的环境中，它们只能访问分配给它们的资源，不能直接访问硬件或其他程序的数据。
- 稳定性：如果一个应用程序崩溃，它不会影响其他应用程序或系统的核心功能。
### 内核空间 (Kernel Space)
内核空间是操作系统的核心。
- 权限：内核可以直接访问硬件，并有权执行任何命令。
- 安全性：虽然内核拥有广泛的权限，但只有那些已知且经过严格测试和验证的代码才被允许在内核空间执行。
- 稳定性：如果内核遇到问题，整个系统可能会崩溃。

## Linux 系统IO
官网api手册：https://www.man7.org/linux/man-pages/index.html

第三方中文api手册：[Linux API速查手册](https://www.bookstack.cn/read/linuxapi/POSIX-IO)

```c
头文件：
#include <fcntl.h>
int open(const char *pathname, int flags, mode_t mode);

参数解析：
pathname：需要打开文件的路径名

flags：打开文件的标志位
必选其一：O_RDONLY（只读）、O_WRONLY（只写）、O_RDWR（读写）
可选组合：O_CREAT（不存在则创建）、O_TRUNC（清空）、O_APPEND（追加）
高级选项：O_NONBLOCK（非阻塞）、O_SYNC（同步写入）

mode（仅O_CREAT时有效）：权限
如0644，对应权限-rw-r--r--

返回值：
成功：返回 文件描述符
失败：返回 -1
```

这里不再一一列举文件io的api，需要的时候查询
[小智学长Linux文件IO](https://x509p6c8to.feishu.cn/docx/GanSd5WxyoqU7gxhSM3cOb7hngc)

