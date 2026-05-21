[根目录](../CLAUDE.md) > **04_FileIO**

# 04_FileIO -- Linux 文件 I/O（系统调用）

## 模块职责

学习 Linux 系统级文件 I/O。使用 POSIX 系统调用（`open/read/write/lseek/close`）完成文件的创建、写入、指针定位和读取操作。包含 "Linux 一切皆文件" 概念和系统架构说明文档。

## 入口与启动

- **入口文件**：`main.c` -- 完整的文件读写演示

### 编译与运行

```bash
mkdir -p build && cd build
cmake ..
make
./main
```

运行后在 `build/` 目录生成 `example.txt` 文件。

## 对外接口

本模块为独立演示程序，无对外接口。

### 演示的系统调用

| 函数 | 头文件 | 说明 |
|------|--------|------|
| `open()` | `<fcntl.h>` | 打开/创建文件，返回文件描述符 |
| `write()` | `<unistd.h>` | 向文件描述符写入数据 |
| `lseek()` | `<unistd.h>` | 移动文件指针（SEEK_SET/CUR/END） |
| `read()` | `<unistd.h>` | 从文件描述符读取数据 |
| `close()` | `<unistd.h>` | 关闭文件描述符 |

## 关键依赖与配置

- **编译器**：gcc（CMake 自动检测）
- **构建系统**：CMake >= 3.1.5
- **系统依赖**：POSIX API（Linux 专属）

## 数据模型

程序操作 `example.txt` 文件，写入 `"Hello, World!\n"` 后读取并打印。

## 测试与质量

- 预期输出：
  ```
  写入 14 字节到文件 example.txt
  读取 14 字节从文件 example.txt
  读取到的内容：Hello, World!
  ```
- 可检查 `build/example.txt` 文件内容验证
- 无自动化测试

## 常见问题 (FAQ)

**Q: write 后为什么要 lseek 回文件开头？**
A: `write()` 后文件指针会自动移动到写入位置之后，直接 `read()` 会读到空内容。

## 相关文件清单

| 文件 | 类型 | 说明 |
|------|------|------|
| `main.c` | 源码 | 完整的文件 I/O 演示，含详细 API 注释 |
| `CMakeLists.txt` | 构建文件 | CMake 配置 |
| `Linux文件IO.md` | 文档 | Linux 系统架构 + IO API 参考手册 |
| `Linux一切皆文件.txt` | 文档 | 空文件（占位提示） |

## 变更记录 (Changelog)

| 日期 | 操作 | 说明 |
|------|------|------|
| 2026-05-20 | 初始化 | 由架构扫描工具自动生成 |
