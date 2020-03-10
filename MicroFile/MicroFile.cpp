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
	if (!result) {
		this->Clear();
		return 0;
	}
	return 1;
}

void MicroFile::Clear()
{
	size = 0;

	if (fileData)
		delete[] fileData;
	fileData = NULL;
	nPoint = NULL;
}

BOOL MicroFile::Save()
{
	CloseHandle(this->m_file);
	this->m_file = CreateFileW(name.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (this->m_file == INVALID_HANDLE_VALUE)
		return 0;
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


	if (fileData)
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

		if (fileData)
			delete[] fileData;
		fileData = NULL;
		return;
	}
	int pointOffset = nPoint - fileData;
	BYTE* nNewData = new BYTE[this->size - size];

	memcpy_s(nNewData, this->size - size, fileData, this->size - size);
	if (sour)
		memcpy_s(sour, size, fileData + (this->size - size), size);


	if (fileData)
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

void MicroFile::Sub(LPBYTE tart, int size)
{
	memcpy_s(tart, size, nPoint, size);
}

MicroFile& MicroFile::operator=(int sour)
{
	if (sour > this->size)
	{
		nPoint = fileData + size;
		return *this;
	}
	nPoint = fileData + sour;
	return *this;
}

BYTE& MicroFile::operator*()
{
	return *nPoint;
	// TODO: 在此处插入 return 语句
}

BOOL MicroFile::operator++(int)
{
	if (nPoint + 1 > fileData + size) {
		return 0;
	}
	nPoint ++;
	return 1;
}

BOOL MicroFile::operator--(int)
{
	if (nPoint - 1 < fileData) {
		return 0;
	}
	nPoint--;
	return 1;
}

BOOL MicroFile::operator-=(DWORD count)
{
	if (nPoint - count < fileData) {
		return 0;
	}
	nPoint -= count;
	return 1;
}

BOOL MicroFile::operator+=(DWORD count)
{
	if (nPoint + count > fileData + size) {
		return 0;
	}
	nPoint += count;
	return 1;
}

MicroBinary::MicroBinary(LPCWSTR filename) :MicroFile(filename)
{
}

MicroBinary::~MicroBinary()
{
}

BOOL MicroBinary::Get(LPBYTE tart)
{
	*tart = *nPoint;

	return 0;
}

BOOL MicroBinary::Get(LPWORD tart)
{
	*tart = *(WORD*)nPoint;
	return 0;
}

BOOL MicroBinary::Get(LPDWORD tart)
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

BOOL MicroFile::Gate(LPVOID tart)
{
	if (size > 0) {
		memcpy_s(tart, size, fileData, size);
		return 1;
	}
	return 0;
}

MicroText::MicroText(LPCWSTR filename) :MicroFile(filename)
{
}

MicroText::~MicroText()
{
}

BOOL MicroText::Get(LPWSTR tart)
{
	lstrcpyW(tart, (LPCWSTR)nPoint);

	return 0;
}

BOOL MicroText::Get(LPSTR tart)
{
	lstrcpyA(tart, (LPSTR)nPoint);
	return 0;
}

MicroBinary& MicroBinary::operator=(int sour)
{
	if (sour > this->size)
	{
		nPoint = fileData + size;
		return *this;
	}
	nPoint = fileData + sour;
	return *this;
}
