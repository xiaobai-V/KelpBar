[根目录](../CLAUDE.md) > **03_CMake**

# 03_CMake -- CMake 构建

## 模块职责

学习 CMake 跨平台构建系统。代码与 01/02 模块相同（多文件 Hello World），但使用 `CMakeLists.txt` 替代 Makefile。包含三种编译方式的对比文档和 CMake 语法参考。

## 入口与启动

- **入口文件**：`main.c` -- 调用 `print_message()` 输出 "Hello world!"
- **功能实现**：`utils.c` -- 定义 `print_message()` 函数
- **接口声明**：`utils.h` -- 声明 `void print_message()`

### 编译与运行

```bash
mkdir -p build && cd build
cmake ..
make
./hello
```

## 对外接口

| 函数 | 声明文件 | 定义文件 | 说明 |
|------|----------|----------|------|
| `void print_message()` | `utils.h` | `utils.c` | 向 stdout 输出 "Hello world!\n" |

## 关键依赖与配置

- **编译器**：gcc（CMake 自动检测）
- **构建系统**：CMake >= 3.1.5
- **CMakeLists.txt 核心指令**：
  - `cmake_minimum_required(VERSION 3.1.5)` -- 最低版本要求
  - `project(hello C)` -- 项目名和语言
  - `aux_source_directory(. SRCS)` -- 自动查找源文件
  - `add_executable(hello ${SRCS})` -- 生成可执行文件

## 数据模型

无。

## 测试与质量

- 预期输出：`Hello world!`
- 无自动化测试

## 常见问题 (FAQ)

**Q: CMake 相比 Makefile 有什么优势？**
A: 跨平台、自动检测环境、可生成不同构建系统的文件。详见 `三种编译方式的对比.md`。

## 相关文件清单

| 文件 | 类型 | 说明 |
|------|------|------|
| `main.c` | 源码 | 入口文件 |
| `utils.c` | 源码 | 功能实现 |
| `utils.h` | 头文件 | 接口声明 |
| `CMakeLists.txt` | 构建文件 | CMake 配置（含常用语法注释） |
| `三种编译方式的对比.md` | 文档 | 手动编译 / Makefile / CMake 对比表 |
| `不需要特意背CMake语法.txt` | 文档 | 空文件（占位提示） |
| `build/` | 构建产物 | CMake 生成目录 |

## 变更记录 (Changelog)

| 日期 | 操作 | 说明 |
|------|------|------|
| 2026-05-20 | 初始化 | 由架构扫描工具自动生成 |
