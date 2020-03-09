#include "pch.h"
#include "MicroFile.h"

MicroFile::MicroFile(LPCWSTR filename)
{
	name = filename;
	this->m_file = CreateFileW(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

MicroFile::~MicroFile()
{
	if (fileData)
		delete[] fileData;
	CloseHandle(m_file);
}

BOOL MicroFile::Load()
{
	if (this->m_file == INVALID_HANDLE_VALUE)
		return 0;
    size = GetFileSize(this->m_file, NULL);

	if (size == 0) {
		fileData = NULL;
		nPoint = NULL;
		return 1;
	}

	fileData = new BYTE[size];
	nPoint = fileData;
	BOOL result = ReadFile(this->m_file, fileData, size, &this->size, NULL);

	return result;
}

void MicroFile::Clear()
{
	ZeroMemory(fileData, size);
	size = 0;
	delete[] fileData;
	fileData = NULL;
	nPoint = NULL;
}

BOOL MicroFile::Save()
{
	CloseHandle(this->m_file);
	this->m_file = CreateFileW(name.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (size > 0)
		return WriteFile(this->m_file, fileData, size, NULL, NULL);
	return 1;
}

DWORD MicroFile::Size()
{
	return size;
}

void MicroFile::Push(LPCVOID sour, ULONG size)
{

	if (fileData == NULL) {
		fileData= new BYTE[size];

		memcpy_s(fileData, size, sour, size);
		nPoint = fileData;
		this->size = size;

		return;
	}
	int pointOffset = nPoint - fileData;

	BYTE* nNewData = new BYTE[size + this->size];

	memcpy_s(nNewData, this->size, fileData, this->size);
	memcpy_s((nNewData + this->size), size, sour, size);

	delete[] fileData;
	fileData = nNewData;
	nPoint = fileData;
	nPoint += pointOffset;
	this->size += size;

}

void MicroFile::Pop(LPVOID sour, ULONG size)
{
	if (size >= this->size) {
		if (sour)
			memcpy_s(sour, this->size, fileData, this->size);
		this->size = 0;
		nPoint = NULL;
		delete[] fileData;
		fileData = NULL;
		return;
	}
	int pointOffset = nPoint - fileData;
	BYTE* nNewData = new BYTE[this->size - size];

	memcpy_s(nNewData, this->size - size, fileData, this->size - size);
	if (sour)
		memcpy_s(sour, size, fileData + (this->size - size), size);

	delete[] fileData;
	fileData = nNewData;
	this->size -= size;
	if (pointOffset > size) {
		nPoint = fileData + size;
	}
	else {
		nPoint = fileData + pointOffset;
	}
	return;
}

void MicroFile::Sub(PBYTE tart, int size)
{
	memcpy_s(tart, size, nPoint, size);
}

void MicroFile::operator=(DWORD sour)
{
	nPoint = fileData + sour;
}

BYTE& MicroFile::operator*()
{
	return *nPoint;
	// TODO: 在此处插入 return 语句
}

void MicroFile::operator++()
{
	nPoint ++;
}

void MicroFile::operator--()
{
	nPoint--;
}

void MicroFile::operator-=(DWORD count)
{
	nPoint -= count;
}

void MicroFile::operator+=(DWORD count)
{
	nPoint += count;
}

MicroBinary::MicroBinary(LPCWSTR filename) :MicroFile(filename)
{
}

MicroBinary::~MicroBinary()
{
}

BOOL MicroBinary::Get(PBYTE tart)
{
	*tart = *nPoint;

	return 0;
}

BOOL MicroBinary::Get(PWORD tart)
{
	*tart = *(WORD*)nPoint;
	return 0;
}

BOOL MicroBinary::Get(PDWORD tart)
{
	*tart = *(DWORD*)nPoint;
	return 0;
}

BOOL MicroBinary::Set(BYTE sour)
{
	*nPoint = sour;
	return 0;
}

BOOL MicroBinary::Set(WORD sour)
{
	*(WORD*)nPoint = sour;
	return 0;
}

BOOL MicroBinary::Set(DWORD sour)
{
	*(DWORD*)nPoint = sour;
	return 0;
}
