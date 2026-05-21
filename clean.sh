#!/bin/bash
# 清理所有子目录下的 build 目录内容
# 用法: ./clean.sh

script_dir="$(cd "$(dirname "$0")" && pwd)"
count=0

for dir in "$script_dir"/*/build; do
    if [ -d "$dir" ]; then
        rm -rf "${dir:?}"/*
        echo "已清理: $dir"
        ((count++))
    fi
done

echo "完成，共清理 $count 个 build 目录"
