# MicroFile
A easy and special way to rw files
基类:MicroFile 抽象类
方法:
Load()把文件加载入内存
Clear()清空文件
Save()保存修改到文件
Size()返回文件大小(字节数)

Push(LPCVOID,ULONG)把一段数据推入文件末尾
Pop(LPVOID,ULONG)把一段数据从文件末尾弹出

void Sub(LPVOID,int) 从当前指针(这个库有一套自己的文件指针机制)位置截取一段数据
BOOL Gate(LPVOID) 复制整个文件数据

virtual BOOL Get(LPBYTE tart) = 0; 从当前位置获取一个字节
virtual BOOL Set(BYTE sour) = 0; //设置当前位置字节

重载运算符:
++文件指针位置自增1，如果超出文件边界返回0，反之1，下列运算符以此类推
-- 文件指针位置自减1
*返回当前字节的引用
+=n文件指针位置自增n
-=n 文件指针位置自减n
=设置文件指针位置

除了运算符以外，任何操作不会影响文件指针位置。
派生类MicroBinary
重载 Get,Set方法。
Get(LPBYTE) 
Get(LPWORD)
Get(LPDWORD)
Set(LPCBYTE)
Set(LPCWORD)
Set(LPCDWORD)
派生类 MicroText
重载:
Get(LPWSTR) 
Get(LPSTR)
Set(LPCWSTR)
Set(LPCSTR)
MicroText的Get与Set方法与指针位置无关，直接设置整个文件。
BOOL Load()
void Push(LPCSTR,int) 向文件末尾推入字符串
void Push(LPCWSTR,int)
void Pop(LPWSTR,int) 从文件末尾弹出字符串
void Pop(LPSTR,int)
MicroText类的构造函数的第二个参数是字符格式。
define ENCODE_BYTE 1
define ENCODE_WORD 2

DWORD Size()返回字符个数
运算符以每个字符占的字节数为移动距离

派生类 MicroData
构造函数的第二个参数是结构体的大小。
void Push(LPCVOID) 将数据推入文件
void Pop(LPVOID) 从文件末尾弹出数据

Get Set以及所有运算符根据对象初始化时定义的结合体大小进行读写和移动指针。