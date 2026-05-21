[根目录](../CLAUDE.md) > **06_Process**

# 06_Process -- 进程管理

## 模块职责

学习 Linux 进程管理。演示 `fork()` 创建子进程、`execl()` 替换进程映像、`waitpid()` 等待子进程。包含两个独立程序：基础 fork+exec 演示和看门狗式进程监控（monitor）。

## 入口与启动

本模块包含 3 个独立可执行文件：

| 可执行文件 | 源文件 | 说明 |
|-----------|--------|------|
| `main` | `main.c` | 基础 fork + execl 演示：子进程执行 `ls -l /home` |
| `monitor` | `monitor.c` | 看门狗程序：fork 子进程运行 `./hello`，子进程退出后自动重启 |
| `hello` | `hello.c` | 辅助程序：每 5 秒输出 "hello"，供 monitor 监控 |

### 编译与运行

```bash
mkdir -p build && cd build
cmake ..
make
./main      # 基础 fork 演示
./monitor   # 看门狗演示（需先编译 hello）
```

## 对外接口

本模块为独立演示程序，无对外接口。

### 演示的系统调用

| 函数 | 头文件 | 说明 |
|------|--------|------|
| `fork()` | `<unistd.h>` | 创建子进程，返回 0（子）/ PID（父）/ -1（错误） |
| `execl()` | `<unistd.h>` | 替换当前进程映像（变参列表版本） |
| `getpid()` / `getppid()` | `<unistd.h>` | 获取当前/父进程 PID |
| `waitpid()` | `<sys/wait.h>` | 等待指定子进程（支持 WNOHANG 非阻塞） |
| `usleep()` | `<unistd.h>` | 微秒级休眠 |

## 关键依赖与配置

- **编译器**：gcc
- **构建系统**：CMake >= 3.10
- **系统依赖**：POSIX 进程管理 API

## 数据模型

无。进程间通过 PID 标识，monitor 使用全局变量 `pid_t pid` 跟踪子进程。

## 测试与质量

- `main` 预期输出：父进程打印自身 PID 和子进程 PID，子进程执行 `ls -l /home` 后退出
- `monitor` 预期行为：持续运行 `./hello`，若 hello 被终止则自动重启
- 无自动化测试

## 常见问题 (FAQ)

**Q: monitor 中为什么要用 WNOHANG？**
A: 非阻塞模式下 `waitpid` 立即返回，不会卡住父进程。父进程可以轮询检查子进程状态，实现看门狗循环。

**Q: execl 失败时为什么要 perror + exit？**
A: `execl` 成功时不返回（进程映像已被替换），只有失败时才会执行后续代码。

## 相关文件清单

| 文件 | 类型 | 说明 |
|------|------|------|
| `main.c` | 源码 | 基础 fork + execl + waitpid 演示 |
| `monitor.c` | 源码 | 看门狗式进程监控（含 waitpid WNOHANG 注释） |
| `hello.c` | 源码 | 辅助循环输出程序 |
| `CMakeLists.txt` | 构建文件 | 编译 3 个独立可执行文件 |
| `hello` | 构建产物 | hello 二进制（与源文件同名） |

## 变更记录 (Changelog)

| 日期 | 操作 | 说明 |
|------|------|------|
| 2026-05-20 | 初始化 | 由架构扫描工具自动生成 |
