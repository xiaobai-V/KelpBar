[根目录](../CLAUDE.md) > **05_STDIO**

# 05_STDIO -- 标准 I/O（C 标准库）

## 模块职责

学习 C 标准库文件 I/O。使用 `fopen/fwrite/fread/fseek/fclose` 等 STDIO 函数进行文件操作，与 04 模块的系统调用方式形成对比。包含 fwrite 踩坑记录文档（sizeof vs strlen）和 fwrite/write 缓冲机制对比文档。

## 入口与启动

- **主入口**：`main.c` -- 使用 STDIO 函数的文件读写演示
- **辅助程序**：`test_null_byte.c` -- fwrite 写入 `\0` 字节的验证测试

### 编译与运行

```bash
mkdir -p build && cd build
cmake ..
make
./main
```

## 对外接口

本模块为独立演示程序，无对外接口。

### 演示的 STDIO 函数

| 函数 | 头文件 | 说明 |
|------|--------|------|
| `fopen()` | `<stdio.h>` | 打开文件，返回 FILE* |
| `fwrite()` | `<stdio.h>` | 向文件流写入数据 |
| `fseek()` | `<stdio.h>` | 定位文件指针 |
| `fread()` | `<stdio.h>` | 从文件流读取数据 |
| `fclose()` | `<stdio.h>` | 关闭文件流 |
| `fflush()` | `<stdio.h>` | 刷新缓冲区 |
| `rewind()` | `<stdio.h>` | 重置文件指针到开头 |

## 关键依赖与配置

- **编译器**：gcc（CMake 自动检测）
- **构建系统**：CMake >= 3.1.5
- **无系统依赖限制**：STDIO 为 C 标准库，可跨平台

## 数据模型

程序操作 `example.txt` 文件，写入 `"Hello world!\n"`（使用 `strlen` 而非 `sizeof` 避免写入 `\0`）。

## 测试与质量

- `main.c` 预期输出：
  ```
  成功写入 13 个数据块
  成功读取到 14 字节：Hello world!
  ```
- `test_null_byte.c`：验证 sizeof 版本会多写一个 `\0` 字节
- 无自动化测试

## 常见问题 (FAQ)

**Q: fwrite 的第三个参数用 sizeof 还是 strlen？**
A: 用 `strlen`。`sizeof` 会包含末尾 `\0`，写入文件后文本编辑器可能误判为二进制文件。详见 `fwrite_sizeof_bug.md`。

**Q: fwrite 返回值怎么判断错误？**
A: `fwrite` 返回 `size_t`（无符号），不能用 `== -1` 判断。应与期望写入块数比较：`if (blocks_write < expected_count)`。

**Q: fwrite 和 write 的缓冲机制有什么区别？**
A: `write` 直接写入内核缓冲区（页缓存），可通过 `fsync` 刷到磁盘。`fwrite` 先写入用户空间缓冲区，通过 `fflush` 刷新到内核，再通过 `fsync` 刷到磁盘。详见 `fwrite和write写入的区别.md`。

## 相关文件清单

| 文件 | 类型 | 说明 |
|------|------|------|
| `main.c` | 源码 | STDIO 文件读写演示 |
| `test_null_byte.c` | 源码 | sizeof vs strlen 验证测试 |
| `CMakeLists.txt` | 构建文件 | CMake 配置 |
| `fwrite_sizeof_bug.md` | 文档 | fwrite sizeof 踩坑记录（含面试要点） |
| `fwrite和write写入的区别.md` | 文档 | STDIO vs 系统调用缓冲机制对比 |
| `Linux文件IO.md` | 文档 | Linux 系统架构与 IO API 参考 |
| `Linux一切皆文件.txt` | 文档 | 空文件（占位提示） |

## 变更记录 (Changelog)

| 日期 | 操作 | 说明 |
|------|------|------|
| 2026-05-20 | 初始化 | 由架构扫描工具自动生成 |
