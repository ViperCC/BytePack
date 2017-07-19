#ifndef _BYTEPACK_
#define _BYTEPACK_

#include <string>

namespace BytePack
{
	struct Bytes
	{
		char* data;
		unsigned int size;
		Bytes(){}
		Bytes(char* d, unsigned int s) : data(d), size(s){}
	};

	template<class T>
	unsigned int _ToSize(T& p) { return sizeof(p); }
	unsigned int _ToSize(const char* p) { return strlen(p) + 1; }
	unsigned int _ToSize(char* p) { return strlen(p) + 1; }

	char* _WriteBytes(char* bytes, char* data, unsigned int size)
	{
		memcpy(bytes, data, size);
		bytes += size;
		return bytes;
	}

	char* _ReadBytes(char* bytes, unsigned int size, char* param)
	{
		memcpy(param, bytes, size);
		bytes += size;
		return bytes;
	}

	template<class T>
	Bytes _ParseData(T& param)
	{
		Bytes b;
		unsigned int size = _ToSize(param);
		b.data = (char*)&param;
		b.size = size;

		return b;
	}

	Bytes _ParseData(Bytes& param)
	{
		return param;
	}

	Bytes _ParseData(const char* param)
	{
		Bytes b;
		unsigned int size = _ToSize(param);
		b.data = (char*)param;
		b.size = size;

		return b;
	}

	Bytes _ParseData(char* param)
	{
		return _ParseData((const char*)param);
	}

	template<class... Args>
	char* ToBytes(Args&... args)
	{
		auto l = {_ParseData(args)...};

		unsigned int size = 0;
		unsigned int paramCount = l.size();
		unsigned int sizeBytesSize = sizeof(unsigned int) * (2 + paramCount);

		char* sizeBytes = new char[sizeBytesSize];
		char* tSizeBytes = sizeBytes;
		tSizeBytes = _WriteBytes(tSizeBytes, (char*)&paramCount, sizeof(unsigned int));
		tSizeBytes += sizeof(unsigned int);

		for (auto x : l)
			tSizeBytes = _WriteBytes(tSizeBytes, (char*)&x.size, sizeof(unsigned int)),
			size += x.size;
	
		memcpy(sizeBytes + sizeof(unsigned int), &size, sizeof(unsigned int));

		char* dataBytes = new char[size];
		char* tDataBytes = dataBytes;
		for (auto x : l)
			tDataBytes = _WriteBytes(tDataBytes, x.data, x.size);

		char* data = new char[sizeBytesSize + size];
		char* tData = data;

		tData = _WriteBytes(tData, sizeBytes, sizeBytesSize);
		tData = _WriteBytes(tData, dataBytes, size);

		delete[] sizeBytes;
		delete[] dataBytes;

		return data;
	}

	template<class T>
	void _UnpackParse(char** sizePack, char** dataPack,T* param)
	{
		unsigned int size = 0;
		*sizePack = _ReadBytes(*sizePack, sizeof(unsigned int), (char*)&size);
		*dataPack = _ReadBytes(*dataPack, size, (char*)param);
	}

	template<class... Args>
	bool Unpack(char* pack, Args*... args)
	{
		unsigned int countInPack = pack[0];
		if (countInPack >= sizeof...(args))
		{
			char* sizeBeginPack = pack + sizeof(unsigned int) * 2;
			char* dataBeginPack = pack + sizeof(unsigned int) * (2 + countInPack);
			auto l = { (_UnpackParse(&sizeBeginPack, &dataBeginPack, args), 0)... };

			return true;
		}

		return false;
	}
}

#endif