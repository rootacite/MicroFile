#pragma once
#include <Windows.h>

#include <string>
using namespace std;


#define DLLAPI __declspec(dllexport)



 class DLLAPI MicroFile
{
public:
	//���ļ�����������ļ��������ܲ���4G�����ļ�
	MicroFile(LPCWSTR filename);
	//�ر��ļ�����������ڴ�
	~MicroFile();
	//���ļ�����װ���ڴ�
	BOOL Load();
	//����ļ�
	void Clear();
    //�����޸�
	BOOL Save();
	//��ȡ�ļ���С
	DWORD Size();
	///��һ�����������ļ�ĩβ
	void Push(LPCVOID sour, ULONG size);
	///��һ�����ݴ��ļ�ĩβ����
	void Pop(LPVOID sour, ULONG size);
	//�ӵ�ǰ��ȡλ�ý�ȡ����
	void Sub(LPBYTE tart,int size);

	//���������ļ�����
	BOOL Gate(LPVOID tart);

	virtual BOOL Get(LPBYTE tart) = 0;
	virtual BOOL Set(BYTE sour) = 0;

	MicroFile& operator=(int sour);

	BYTE &operator*();
	BOOL operator++(int);
	BOOL operator--(int);
	BOOL operator-=(DWORD count);
	BOOL operator+=(DWORD count);



protected:
	HANDLE m_file = NULL;
	BYTE* fileData = NULL;
	DWORD size;
	wstring name;
	BYTE* nPoint = NULL;
};

class DLLAPI  MicroBinary :public MicroFile
{
public:
	MicroBinary(LPCWSTR filename);
	~MicroBinary();

	BOOL Get(LPBYTE tart);
	BOOL Get(LPWORD tart);
	BOOL Get(LPDWORD tart);

	BOOL Set(BYTE sour);
	BOOL Set(WORD sour);
	BOOL Set(DWORD sour);

	MicroBinary& operator=(int sour);
private:

};

class MicroText:public MicroFile
{
public:
	MicroText(LPCWSTR filename);
	~MicroText();

	BOOL Get(LPWSTR tart);
	BOOL Get(LPSTR tart);

	//MicroText& operator=(int sour);
	//void Push(LPCWSTR sour);
	//void Push(LPCSTR sour);

	//void Pop(LPWSTR tart);
	//void Pop(LPSTR tart);

	//void Sub(LPBYTE tart, int size);
private:

};