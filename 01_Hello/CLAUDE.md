[根目录](../CLAUDE.md) > **01_Hello**

# 01_Hello -- Hello World

## 模块职责

最简 C 程序入门模块。演示多文件编译的基本流程：将程序拆分为 `main.c` 和 `utils.c`，通过头文件 `utils.h` 声明接口，手动使用 gcc 命令分步编译。

## 入口与启动

- **入口文件**：`main.c` -- 调用 `print_message()` 输出 "Hello world!"
- **功能实现**：`utils.c` -- 定义 `print_message()` 函数
- **接口声明**：`utils.h` -- 声明 `void print_message()`

### 编译与运行

```bash
# 分步编译
gcc -c main.c -o main.o
gcc -c utils.c -o utils.o
gcc main.o utils.o -o hello
./hello
```

## 对外接口

| 函数 | 声明文件 | 定义文件 | 说明 |
|------|----------|----------|------|
| `void print_message()` | `utils.h` | `utils.c` | 向 stdout 输出 "Hello world!\n" |

## 关键依赖与配置

- **编译器**：gcc
- **无第三方依赖**
- **无构建系统**：纯手动 gcc 命令

## 数据模型

无。本模块仅演示编译流程。

## 测试与质量

- 预期输出：`Hello world!`
- 无自动化测试

## 相关文件清单

| 文件 | 类型 | 说明 |
|------|------|------|
| `main.c` | 源码 | 入口文件，调用 print_message |
| `utils.c` | 源码 | 功能实现 |
| `utils.h` | 头文件 | 接口声明 |
| `main.o` / `utils.o` | 构建产物 | 目标文件 |
| `hello` | 构建产物 | 可执行文件 |

## 变更记录 (Changelog)

| 日期 | 操作 | 说明 |
|------|------|------|
| 2026-05-20 | 初始化 | 由架构扫描工具自动生成 |
