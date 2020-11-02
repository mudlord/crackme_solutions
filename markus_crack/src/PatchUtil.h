#pragma once
#include "Common.h"

uAddr ReadRelativeAddress(uAddr address);
void WriteRelativeAddress(uAddr address, uAddr content);
void WriteMemBytes(LPVOID lpAddress, LPVOID lpBuffer, DWORD nSize);
void ReadMem(LPVOID lpAddress, LPVOID lpBuffer, DWORD nSize);

//returns: offset to data when found, -1 when not found
size_t patternfind(
	unsigned char* data, //data
	size_t datasize, //size of data
	const char* pattern //pattern to search
);

//returns: offset to data when found, -1 when not found
size_t patternfind(
	unsigned char* data, //data
	size_t datasize, //size of data
	unsigned char* pattern, //bytes to search
	size_t patternsize //size of bytes to search
);

//returns: nothing
void patternwrite(
	unsigned char* data, //data
	size_t datasize, //size of data
	const char* pattern //pattern to write
);

//returns: true on success, false on failure
bool patternsnr(
	unsigned char* data, //data
	size_t datasize, //size of data
	const char* searchpattern, //pattern to search
	const char* replacepattern //pattern to write
);

bool patternsnr_mem(
	unsigned char* data, //data
	size_t datasize, //size of data
	const char* searchpattern, //pattern to search
	const char* replacepattern //pattern to write
);

template<typename T>
void WriteMem(uAddr address, T value)
{
	DWORD oldProtect;
	VirtualProtect(LPVOID(address), sizeof T, PAGE_EXECUTE_READWRITE, &oldProtect);
	*reinterpret_cast<T *>(address) = value;
	VirtualProtect(LPVOID(address), sizeof T, oldProtect, &oldProtect);
}

template<typename T>
void WriteMemRaw(uAddr address, T value)
{
	*reinterpret_cast<T *>(address) = value;
}


#define WriteInt(address, value)  WriteMem(address, int(value))
#define WriteUInt(address, value)  WriteMem(address, uint(value))
#define WriteUAddr(address, value)  WriteMem(address, uAddr(value))

#define WriteShort(address, value)   WriteMem(address, short(value))
#define WriteUShort(address, value)  WriteMem(address, ushort(value))

#define WriteByte(address, value)  WriteMem(address, byte(value))


#define WriteIntRaw(address, value)  WriteMemRaw(address, int(value))
#define WriteUIntRaw(address, value)  WriteMemRaw(address, uint(value))
#define WriteUAddrRaw(address, value)  WriteMemRaw(address, uAddr(value))

#define WriteShortRaw(address, value)   WriteMemRaw(address, short(value))
#define WriteUShortRaw(address, value)  WriteMemRaw(address, ushort(value))

#define WriteByteRaw(address, value)  WriteMemRaw(address, byte(value))
