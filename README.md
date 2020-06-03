## 起步
供学习的脚本语言`origin`，属于弱类型编程语言，想法来自学习《编译原理》的时候想开发一门编程语言。现在开发一门语言门槛确实没那么高了。

另外这个版本比较粗糙，后面会慢慢完善的。

## 环境需求
 - Linux
 - bison
 - flex
 - gcc

gcc一般是linux系统自带的，没有bsion和flex可以通过`apt-get install bison`和`apt-get install flex`进行安装

## 学习
`cale` 文件夹下是一个计算器案例, 包含使用flex版本和手工版本
`simple_for` 文件夹是一个简易的仅支持for和if和简单的四则运算的解释器, 刚开始学习可试着来完善它.

## 编译
```
$ git clone https://github.com/hongweipeng/origin.git
$ make
$ ./origin tests/hello.org
```
如果这时有输出`hello world`，就说明可以使用了。

## 语法
### 保留字
保留字在语法分析后会被替换掉，目前有`for while if elseif else return function true false`

### 数据类型
 基本数据类型有`整型，浮点型，字符串型，布尔型`，由于是弱类型语言，变量声明时候不需要指定类型。
```
a = 10;
b = 9.0;
c = "hello";
```
### 操作符
支持双目运算符`+ - * /` 和比较运算符：`< > <= >= == !=` 和位运算符: `<< >> | & ^`
```
c = a + b;
c = a || b;
c = a - 6;
```

### 跳转语句
```
if (4 < 5) {
    print(true);
}

sum = 0;
for(i = 0; i <= 100; i = i + 1) {
    sum = sum + i;
}
print(sum);

while(a > 0) {
    a = a - 1;
}
```
### 注释
以`#`开头的行认为是注释

### 函数
用户自己声明的函数：
```
function fun(a) {
    return a + 1;
}
b = fun(2);
```
系统提供的函数：
这个我不是很想提，目前只提供的`print()`函数而已。
