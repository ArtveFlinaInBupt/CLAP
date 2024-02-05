# CLAP

> C Lexer *And Parser*
 
## 概述 

这是一份北京邮电大学计算机科学与技术专业 2021 级大三上（2023–2024 学年度秋季学期）《编译原理与技术》课程实验。

这是一个 C99 (ISO/IEC 9899:1999) 的词法分析器。名字里有个 Parser 只是因为当时开项目的时候不知道后面不需要写语法分析了，就凑了个单词 acronym 出来。

这个 lexer 没有实现以下的 C99 特性：

- 预处理器指令
- 行尾的反斜杠续行
- 三标符
-  Unicode 转义标识符
- 十六进制浮点常量

此外，它在处理不常见的整数常量和浮点常量时会有一系列错误，糊弄作业应该问题不大。

运行方式不详述。构建后直接运行会向 stdout 打印帮助信息。以下的命令能够对所有 C99 标头经过预处理器展开后的代码进行词法分析：

```shell
./clap ./testbench/huge_test_out.c
```

词法分析结果包含 14555 个 token，不包含错误，肉眼看上去没啥问题。

更多信息参见[报告](doc.pdf)。
