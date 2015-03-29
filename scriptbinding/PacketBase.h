#ifndef PACKETBASE_HEADER
#define PACKETBASE_HEADER

typedef unsigned char BYTE;

#include <memory>
#include <string>
#include <cstdarg>
#include <string.h>
using std::string;

class PacketBase
{
public:
	enum PACKETVER
	{
		SERVER_PACKET_DEFAULT_VER = 1,
		SERVER_PACKET_DEFAULT_SUBVER = 1
	};

	enum
	{
		PACKET_HEADER_SIZE = 9,
		PACKET_BUFFER_SIZE = 1024 * 12
	};

	PacketBase(void){}
	virtual ~PacketBase(void){}
	char * packet_buf(void)				{	return m_strBuf;}
	const char * getData()const			{	return m_strBuf;}
	int packet_size(void)const			{	return m_nPacketSize;}
	short GetCmdType(void)
	{
		short nCmdType;
		_readHeader((char*)&nCmdType, sizeof(short), 2);
		return nCmdType;
	}
	char GetVersion(void)
	{
		char c;
		_readHeader(&c, sizeof(char), 4);
		return c;
	}
	char GetSubVersion(void)
	{
		char c;
		_readHeader(&c, sizeof(char), 5);
		return c;
	}
	short GetBodyLength(void)
	{
		short nLen;
		_readHeader((char*)&nLen, sizeof(short), 6);
		return nLen;
	}
	BYTE GetcbCheckCode(void)
	{
		BYTE code;
		_readHeader((char*)&code, sizeof(BYTE), 8);
		return code;
	}
	void _reset(void)
	{
		readptr_ = PACKET_HEADER_SIZE;
		m_nPacketSize = PACKET_HEADER_SIZE;
	}

protected:
	bool _copy(const void *pInBuf, int nLen)
	{
		if(nLen > PACKET_BUFFER_SIZE)
		{
			return false;
		}

		_reset();
		memcpy(m_strBuf, pInBuf, nLen);
		m_nPacketSize = nLen;

		return true;
	}
	void _begin(short nCmdType, char cVersion, char cSubVersion)
	{
		_reset();
		const char HEAD_FLAGS[] = "IC";
		_writeHeader(HEAD_FLAGS, sizeof(char)*2, 0);
		_writeHeader((char*)&nCmdType, sizeof(short), 2);
		_writeHeader(&cVersion, sizeof(char), 4);	
		_writeHeader(&cSubVersion, sizeof(char), 5);
	}
	void _SetBegin(short nCmdType)
	{
		_writeHeader((char*)&nCmdType, sizeof(short), 2);
	}
	void _end()
	{
		short nBody = static_cast<short>(m_nPacketSize - PACKET_HEADER_SIZE);
		_writeHeader((char*)&nBody, sizeof(short), 6);
		BYTE code = 0;
		_writeHeader((char*)&code, sizeof(BYTE), 8);
	}
	void _oldend()
	{
		short nBody = static_cast<short>(m_nPacketSize - PACKET_HEADER_SIZE);
		_writeHeader((char*)&nBody, sizeof(short), 6);
	}
	bool _Read(char *pOut, int nLen)
	{
		if((nLen + readptr_) > m_nPacketSize || (nLen + readptr_) > PACKET_BUFFER_SIZE)
		{
			return false ;
		}

		memcpy(pOut, m_strBuf + readptr_, nLen);
		readptr_ += nLen;
		return true;
	}
	bool _ReadDel(char *pOut, int nLen)
	{
		if(!_Read(pOut, nLen))
			return false;
		memmove(m_strBuf + readptr_ - nLen, m_strBuf + readptr_, m_nPacketSize - readptr_);
		readptr_ -= nLen;
		m_nPacketSize -= nLen;
		_end();
		return true;
	}
	void _readundo(int nLen)
	{
		readptr_ -= nLen;
	}
	char *_readpoint(int nLen)
	{
		if((nLen + readptr_) > m_nPacketSize)
			return NULL; 
		char *p = &m_strBuf[readptr_];
		readptr_ += nLen;
		return p;

	}
	bool _Write(const char *pIn, int nLen)
	{
		if((m_nPacketSize < 0) || ((nLen + m_nPacketSize) > PACKET_BUFFER_SIZE))
		{
			return false ;
		}
		memcpy(m_strBuf+m_nPacketSize, pIn, nLen);
		m_nPacketSize += nLen;
		return true;
	}
	bool _Insert(const char *pIn, int nLen)
	{
		if((nLen + m_nPacketSize) > PACKET_BUFFER_SIZE)
		{
			return false;
		}

		memmove(m_strBuf + PACKET_HEADER_SIZE + nLen, m_strBuf + PACKET_HEADER_SIZE, m_nPacketSize - PACKET_HEADER_SIZE);

		memcpy(m_strBuf + PACKET_HEADER_SIZE, pIn, nLen);

		m_nPacketSize += nLen;
		_end();
		return true;
	}
	bool _writezero(void)
	{
		if((m_nPacketSize + 1) > PACKET_BUFFER_SIZE)
		{
			return false ;
		}

		memset(m_strBuf+m_nPacketSize, '\0', sizeof(char)) ;
		m_nPacketSize ++;
		return true;
	}
	void _readHeader(char *pOut, int nLen, int nPos)
	{
		if(nPos > 0 || nPos+nLen < PACKET_HEADER_SIZE)
		{
			memcpy(pOut, m_strBuf+nPos, nLen) ;
		}
	}
	void _writeHeader(const char *pIn, int nLen, int nPos)
	{
		if(nPos > 0 || nPos+nLen < PACKET_HEADER_SIZE)
		{
			memcpy(m_strBuf+nPos, pIn, nLen) ;
		}
	}

private:
	char m_strBuf[PACKET_BUFFER_SIZE];	
	int m_nPacketSize ;	
	int readptr_;
};

class NETInputPacket: public PacketBase
{
public:
	typedef PacketBase base;
	int ReadInt(void)		{int nValue = -1; base::_Read((char*)&nValue, sizeof(int)); return nValue;} 
	unsigned int ReadUInt(void)	{unsigned int nValue = 0; base::_Read((char*)&nValue, sizeof(unsigned int)); return nValue; }
	unsigned long ReadULong(void) {unsigned long nValue = 0; base::_Read((char*)&nValue, sizeof(unsigned long)); return nValue;}
	int ReadIntDel(void)	{int nValue = -1; base::_ReadDel((char*)&nValue, sizeof(int)); return nValue;} 
	short ReadShort(void)	{short nValue = -1; base::_Read((char*)&nValue, sizeof(short)); return nValue;}
	BYTE ReadByte(void)		{BYTE nValue = 0; base::_Read((char*)&nValue, sizeof(BYTE)); return nValue;}
	bool ReadString(char *pOutString, int nMaxLen)
	{
		int nLen = ReadInt();
		if(nLen <= 0) 
			return false;
		if(nLen > nMaxLen)
		{
			base::_readundo(sizeof(short));
			return false;
		}
		return base::_Read(pOutString, nLen);
	}
	char *ReadChar(void)
	{
		int nLen = ReadInt();
		if(nLen <= 0 || nLen >= PACKET_BUFFER_SIZE)
			return NULL;
		return base::_readpoint(nLen);
	}
	string ReadString(void)
	{
		char *p = ReadChar();
		return (p == NULL ? "" : p);
	}
	int ReadBinary(char *pBuf, int nMaxLen)
	{
		int nLen = ReadInt();
		if(nLen <= 0)
			return -1;
		if(nLen > nMaxLen)
		{
			base::_readundo(sizeof(int));
			return -1;
		}
		if(base::_Read(pBuf, nLen))
			return nLen ;
		return 0;
	}
	void Reset(void)
	{
		base::_reset();
	}
	bool Copy(const void *pInBuf, int nLen)
	{
		return base::_copy(pInBuf, nLen);
	}
	bool WriteBody(const char *pIn, int nLen)
	{
		return base::_Write(pIn, nLen);
	}
	void Begin(short nCommand, char cVersion = SERVER_PACKET_DEFAULT_VER, char cSubVersion = SERVER_PACKET_DEFAULT_SUBVER)
	{
		base::_begin(nCommand, cVersion, cSubVersion);
	}
	void End(void)
	{
		base::_end();
	}
};

class NETOutputPacket: public PacketBase
{
public:
	typedef PacketBase base;
	NETOutputPacket(void){}
	bool WriteInt(int nValue)		{return base::_Write((char*)&nValue, sizeof(int));}
	bool WriteUInt(unsigned int nValue)	{return base::_Write((char*)&nValue, sizeof(unsigned int));}
	bool WriteULong(unsigned long nValue) {return base::_Write((char*)&nValue, sizeof(unsigned long));}
	bool WriteByte(BYTE nValue)		{return base::_Write((char*)&nValue, sizeof(BYTE));}
	bool WriteShort(short nValue)	{return base::_Write((char*)&nValue, sizeof(short));}
	bool InsertInt(int nValue)		{return base::_Insert((char*)&nValue, sizeof(int));}
	bool InsertByte(BYTE nValue)	{return base::_Insert((char*)&nValue, sizeof(BYTE));}
	bool WriteString(const char *pString)
	{
		int nLen = (int)strlen(pString) ;
		WriteInt(nLen + 1) ;
		return base::_Write(pString, nLen) && base::_writezero();
	}
	bool WriteString(const string & strDate)
	{
		int nLen = (int)strDate.size();	

		WriteInt(nLen + 1);
		
		return base::_Write(strDate.c_str(), nLen) && base::_writezero();
	}
	bool WriteBinary(const char *pBuf, int nLen)
	{
		WriteInt(nLen) ;
		return base::_Write(pBuf, nLen) ;
	}
	bool Copy(const void *pInBuf, int nLen)
	{
		return base::_copy(pInBuf, nLen);
	}
	bool CopyData(const char* pInBuf, int nBufLen)
	{
		return base::_Write(pInBuf, nBufLen);
	}
	void Begin(short nCommand, char cVersion = SERVER_PACKET_DEFAULT_VER, char cSubVersion = SERVER_PACKET_DEFAULT_SUBVER)
	{
		base::_begin(nCommand, cVersion, cSubVersion);
	}
	void End(void)
	{
		base::_end();
	}
	void oldEnd(void)
	{
		base::_oldend();
	}
	void SetBegin(short nCommand)
	{
		base::_SetBegin(nCommand);
	}
	void WritecbCheckCode(BYTE nValue)
	{
		base::_writeHeader((char*)&nValue, sizeof(BYTE), 8);
	}
	void BuildPackage(short nCmdType, const char* pszFmt, ...)
	{
		Begin(nCmdType);

		if (pszFmt == NULL)
		{
			End();
			return;
		}

		va_list ap; 
		va_start (ap, pszFmt); 
		const char* p = NULL;

		for (p= pszFmt; *p; p++) 
		{ 
			if (*p != '%') 
			{
				continue; 
			}

			switch (*++p) 
			{ 
			case 'd':	//int
				{
					int nVal= va_arg(ap, int);
					WriteInt(nVal);
					break;
				}
			case 'h':	//short
				{
					const int shVal = va_arg(ap, int);
					WriteShort(static_cast<short>(shVal));
					break;
				}
			case 'u':	//unsigned long
				{
					unsigned long dwVal = va_arg(ap, unsigned long);
					WriteULong(dwVal);
					break;
				}
			case 's':	//char*
				{
					char* pVal = va_arg(ap, char*);
					WriteString(pVal);
					break;
				}
			}
		}
		End();
	}
};

#endif 

