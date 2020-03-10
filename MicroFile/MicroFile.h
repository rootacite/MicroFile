#pragma once


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

	virtual BOOL Get(LPBYTE tart) = 0;
	virtual BOOL Set(BYTE sour) = 0;

	void operator=(DWORD sour);
	BYTE &operator*();
	BOOL operator++();
	BOOL operator--();
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

private:

};