#include "pch.h"
#include "MicroFile.h"

MicroFile::MicroFile(LPCWSTR filename)
{
	*name = filename;
	this->m_file = CreateFileW(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

MicroFile::~MicroFile()
{
	if (fileData)
		delete[] fileData;
	CloseHandle(m_file);
	delete name;
}

BOOL MicroFile::Load()
{
	if (this->m_file == INVALID_HANDLE_VALUE)
		return 0;
   int size = GetFileSize(this->m_file, NULL);

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
	this->m_file = CreateFileW((*name).c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

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

void MicroFile::Sub(LPVOID tart, int size)
{
	memcpy_s(tart, size, nPoint, size);
}

MicroFile& MicroFile::operator=(int sour)
{
	if (sour > this->size)
	{
		nPoint = fileData + (size-1);
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
	if (nPoint + 1 >= fileData + size) {
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
	if (nPoint + count >= fileData + size) {
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

MicroBinary& MicroBinary::operator=(int sour)
{
	if (sour >= this->size)
	{
		nPoint = fileData + (size-1);
		return *this;
	}
	nPoint = fileData + sour;
	return *this;
}

MicroText::MicroText(LPCWSTR filename, UINT code) :MicroFile(filename)
{
	switch (code) {
	case ENCODE_BYTE:
		m_code = 1;
		break;
	case ENCODE_WORD:
		m_code = 2;
		break;
	case ENCODE_DWORD:
		m_code = 4;
		break;
	default:
		throw "undefined code";
	}
}

MicroText::~MicroText()
{
	delete Data;
	delete wData;
}

BOOL MicroText::Get(LPWSTR tart)
{
	lstrcpyW(tart, wData->c_str());

	return 0;
}

BOOL MicroText::Get(LPSTR tart)
{
	lstrcpyA(tart, Data->c_str());
	return 0;
}

BOOL MicroText::Load()
{
	if (this->m_file == INVALID_HANDLE_VALUE)
		return 0;
	int size = GetFileSize(this->m_file, NULL);

	if (size == 0) {
		if (m_code == 1)
			*Data = "";
		else
			*wData = L"";
		fileData = NULL;
		nPoint = NULL;
		return 1;
	}
	if (fileData)
		delete[] fileData;

	size += (m_code == 1 ? 1 : 2);

	fileData = new BYTE[size];
	nPoint = fileData;

	BOOL result = ReadFile(this->m_file, fileData, GetFileSize(this->m_file, NULL), &this->size, NULL);
	this->size += (m_code == 1 ? 1 : 2);
	if (!result) {
		this->Clear();
		return 0;
	}
	if (m_code == 1) {
		((LPSTR)fileData)[this->size-1] = '\0';
		*Data = (LPSTR)fileData;
		*Data += "\0";
	}
	else {
		((LPWSTR)fileData)[(this->size /2)-1] = L'\0';
		*wData = (LPWSTR)fileData;
		*wData += L"\0";
	}
	return 1;
}

BOOL MicroText::Set(BYTE tart)
{
	*nPoint = tart;
	return 0;
}

BOOL MicroText::Get(LPBYTE tart)
{
	*tart = *nPoint;
	return 0;
}

BOOL MicroText::Set(LPCWSTR tart)
{
	this->Clear();
	this->Push(tart);

	*wData = tart;
	return 0;
}

BOOL MicroText::Set(LPCSTR tart)
{
	this->Clear();
	this->Push(tart);

	*Data = tart;
	return 0;
}

void  MicroText::Push(LPCSTR sour)
{
	*Data += sour;
	this->MicroFile::Pop(NULL, 1);
	this->MicroFile::Push(sour, lstrlenA(sour)+1);
}

void  MicroText::Push(LPCWSTR sour)
{
	*wData += sour;
	this->MicroFile::Pop(NULL, 2);
	this->MicroFile::Push(sour, (lstrlenW(sour)+1) * 2);
}

void MicroText::Pop(LPWSTR tart,int snbize)
{
	int locSize = snbize*2;
	wData->erase(wData->end() - snbize, wData->end());

	this->MicroFile::Pop(tart,2+ locSize);
	wchar_t loc0 = L'\0';
	this->MicroFile::Push(&loc0, 2);
}
void MicroText::Pop(LPSTR tart, int snbize)
{
	int locSize = snbize;
	Data->erase(Data->end() - locSize, Data->end());
	this->MicroFile::Pop(tart, locSize+1);
	char loc0 = '\0';
	this->MicroFile::Push(&loc0, 1);
}
void MicroText::Clear()
{
	this ->MicroFile::Clear();
	if (m_code == 1)
		*Data = "";
	else
		*wData = L"";
}

MicroText& MicroText::operator=(int sour)
{
	if (m_code == 1)
		nPoint = fileData + sour;
	else
		nPoint = fileData + (sour*2);

	return *this;
	// TODO: 在此处插入 return 语句
}

char& MicroText::operator*()
{
		return *(CHAR*)nPoint;
	// TODO: 在此处插入 return 语句
}

WCHAR& MicroText::operator&()
{
	return *(WCHAR*)nPoint;
	// TODO: 在此处插入 return 语句
}

BOOL MicroText::operator++(int)
{
	if (m_code == 1)
	{
		if (nPoint + 1 >= fileData + size) {
			return 0;
		}
		nPoint++;
		return 1;
	}
	else 
	{
		if (nPoint + 2 >= fileData + size) {
			return 0;
		}
		nPoint+=2;
		return 1;
	}
}

BOOL MicroText::operator--(int)
{
	

	if (m_code == 1)
	{
		if (nPoint - 1 < fileData) {
			return 0;
		}
		nPoint--;
		return 1;
	}
	else
	{
		if (nPoint - 2 < fileData) {
			return 0;
		}
		nPoint -= 2;
		return 1;
	}
	return 0;
}


MicroData::MicroData(LPCWSTR filename, DWORD nsize) :MicroFile(filename)
{
	structure = nsize;
}


MicroData::~MicroData()
{
}




BOOL MicroData::operator++(int)
{
	if (nPoint + structure >= fileData + size) {
		return 0;
	}
	nPoint+= structure;
	return 1;
}


BOOL MicroData::operator--(int)
{
	if (nPoint - structure < fileData) {
		return 0;
	}
	nPoint-=structure;
	return 1;
}


void MicroData::operator=(int sour)
{
	if (sour*structure >= this->size)
	{
		nPoint = fileData + (size-1);
		return;
	}
	nPoint = fileData + sour*structure;
	// TODO: 在此处插入 return 语句
}


void  MicroData::Push(LPCVOID sour)
{
	this->MicroFile::Push(sour, structure);
}


void MicroData::Pop(LPVOID tart)
{
	this->MicroFile::Pop(&tart, structure);
}


BOOL MicroData::operator-=(DWORD count)
{
	if (nPoint - (count*structure) < fileData) {
		return 0;
	}
	nPoint -= (count * structure);
	return 1;
}


BOOL  MicroData::operator+=(DWORD count)
{
	if (nPoint + (count * structure) >= fileData + size) {
		return 0;
	}
	nPoint += (count * structure);
	return 1;
}


BOOL MicroData::Set(BYTE sour)
{
	*nPoint = sour;
	return 0;
}


BOOL MicroData::Get(LPBYTE tart)
{
	*tart = *nPoint;

	return 0;
}


BOOL MicroData::Set(LPCVOID sour)
{
	memcpy_s(nPoint, structure, sour, structure);
	return 0;
}


BOOL MicroData::Get(LPVOID tart)
{
	memcpy_s(tart,structure,nPoint, structure);

	return 0;
}

void TEST() {

}

BOOL MicroText::Save()
{
	CloseHandle(this->m_file);
	this->m_file = CreateFileW((*name).c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (this->m_file == INVALID_HANDLE_VALUE)
		return 0;
	if (size > 0)
		return WriteFile(this->m_file, fileData, size - m_code, NULL, NULL);
	return 0;
}

DWORD MicroText::Size()
{

	return size/m_code;
}

DWORD MicroData::Size()
{

	return size / structure;
}
