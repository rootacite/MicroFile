#pragma once


#include <string>
using namespace std;


#define DLLAPI __declspec(dllexport)

 class DLLAPI MicroFile
{
public:
	MicroFile(LPCWSTR filename);
	~MicroFile();

	BOOL Load();
	void Clear();
	BOOL Save();
	DWORD Size();
	void Push(LPCVOID sour, ULONG size);
	void Pop(LPVOID sour, ULONG size);
	void Sub(PBYTE tart,int size);

	virtual BOOL Get(PBYTE tart) = 0;
	virtual BOOL Set(BYTE sour) = 0;

	void operator=(DWORD sour);
	BYTE &operator*();
	void operator++();
	void operator--();
	void operator-=(DWORD count);
	void operator+=(DWORD count);

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

	BOOL Get(PBYTE tart);
	BOOL Get(PWORD tart);
	BOOL Get(PDWORD tart);

	BOOL Set(BYTE sour);
	BOOL Set(WORD sour);
	BOOL Set(DWORD sour);

private:

};