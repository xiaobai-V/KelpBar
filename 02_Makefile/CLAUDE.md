[根目录](../CLAUDE.md) > **02_Makefile**

# 02_Makefile -- Makefile 构建

## 模块职责

学习 Makefile 自动化编译。代码与 01_Hello 相同（多文件 Hello World），但引入了 Makefile 来管理编译规则。提供了两版 Makefile：初版（逐文件写规则）和优化版（使用通配符和自动变量）。

## 入口与启动

- **入口文件**：`main.c` -- 调用 `print_message()` 输出 "Hello world!"
- **功能实现**：`utils.c` -- 定义 `print_message()` 函数
- **接口声明**：`utils.h` -- 声明 `void print_message()`

### 编译与运行

```bash
make        # 使用 Makefile 编译
./hello     # 运行
make clean  # 清理构建产物
```

## 对外接口

| 函数 | 声明文件 | 定义文件 | 说明 |
|------|----------|----------|------|
| `void print_message()` | `utils.h` | `utils.c` | 向 stdout 输出 "Hello world!\n" |

## 关键依赖与配置

- **编译器**：gcc（通过 `CC = gcc` 变量指定）
- **构建系统**：GNU Make
- **当前使用的 Makefile**：`Makefile`（优化版，含详细注释）

### Makefile 对比

| 文件 | 特点 |
|------|------|
| `Makefile` | 优化版：使用 `wildcard` 自动查找源文件、模式规则 `%.o: %.c`、自动变量 `$@ $^ $<` |
| `Makefile.backup` | 初版：逐文件手写编译规则，显式列出每个 .o 依赖 |

### 自动变量速查

| 变量 | 含义 |
|------|------|
| `$@` | 目标文件完整名称 |
| `$^` | 所有不重复的依赖文件 |
| `$<` | 第一个依赖文件名称 |
| `$?` | 比目标新的依赖文件 |
| `$*` | 不含扩展名的目标名称 |

## 数据模型

无。

## 测试与质量

- 预期输出：`Hello world!`
- 无自动化测试

## 常见问题 (FAQ)

**Q: 为什么不直接用 gcc 手动编译？**
A: Makefile 的优势在于自动化——只重新编译修改过的文件，节省时间。详见 `Makefile的劣势.md` 中的分析。

## 相关文件清单

| 文件 | 类型 | 说明 |
|------|------|------|
| `main.c` | 源码 | 入口文件 |
| `utils.c` | 源码 | 功能实现 |
| `utils.h` | 头文件 | 接口声明 |
| `Makefile` | 构建文件 | 优化版 Makefile（含详细注释） |
| `Makefile.backup` | 构建文件 | 初版 Makefile |
| `Makefile的劣势.md` | 文档 | Makefile 的局限性分析 |

## 变更记录 (Changelog)

| 日期 | 操作 | 说明 |
|------|------|------|
| 2026-05-20 | 初始化 | 由架构扫描工具自动生成 |
