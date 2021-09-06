#include <unordered_map>
#include <mutex>
#include "fl_string.h"

namespace fluffy {
	/**
	 * TStringBuffer
	 */

	class TStringBuffer
	{
	private:
		TStringBuffer(const U32 bufferSize = 2097152) // 2MB
			: m_buffer(nullptr)
			, m_cursor(0)
			, m_bufferSize(bufferSize)
		{
			m_buffer = static_cast<I8*>(malloc(bufferSize));
			m_buffer[0] = '\0';
			m_buffer[bufferSize - 1] = '\0';
		}

	public:
		~TStringBuffer()
		{
			if (m_buffer != nullptr) {
				delete m_buffer;
				m_buffer = nullptr;
			}
		}

		static
		TStringBuffer* getSingleton()
		{
			static TStringBuffer* buffer = nullptr;
			if (buffer == nullptr) {
				buffer = new TStringBuffer(32768);
			}
			return buffer;
		}

		const I8* find(const I8* str)
		{
			I8* bufferBegin = m_buffer;

			U32 bufferIdx = 0;
			U32 strIdx = 0;

			if (m_cursor == 0)
			{
				return nullptr;
			}

			// Busca string no buffer.
			while (true)
			{
				const I8* bufferCur = bufferBegin + bufferIdx;
				const I8* strCur = str + strIdx;

				if (*bufferCur == *strCur)
				{
					if (compare_string(bufferCur, str))
					{
						return bufferCur;
					}
					bufferIdx += getOffsetToNextString(bufferCur);

					if (bufferIdx >= m_cursor)
					{
						return nullptr;
					}
					continue;
				}
				bufferIdx += getOffsetToNextString(bufferCur);

				if (bufferIdx >= m_cursor)
				{
					return nullptr;
				}
			}
			return nullptr;
		}

		const I8* put(const I8* str)
		{
			const I8* strCur = str;

			// Verifica se string ja existe.
			if (auto strFinded = find(str)) {
				return strFinded;
			}

			std::lock_guard<std::mutex> guard(m_mutex);
			{
				// Verifica se e necesseario aumentar o tamanho do buffer.
				if (m_bufferSize - m_cursor < 256)
				{
					m_bufferSize += 8192; // 8K
					m_buffer = static_cast<I8*>(realloc(m_buffer, m_bufferSize));
				}

				I8* bufferStrBeg = m_buffer + m_cursor;

				// Adiciona string ao buffer.
				if (I8* bufferCur = bufferStrBeg)
				{
					while (*strCur != 0)
					{
						*bufferCur++ = *strCur++;
					}
					*bufferCur++ = '\0';

					// Atualiza posicao do pcursor para a proxima
					m_cursor += static_cast<U32>(bufferCur - bufferStrBeg);

					// Verifica se e necesseario aumentar o tamanho do buffer.
					if (m_cursor >= m_bufferSize)
					{
						m_bufferSize += 8192; // 16K
						m_buffer = static_cast<I8*>(realloc(m_buffer, m_bufferSize));
					}

					return bufferStrBeg;
				}
			}
			return nullptr;
		}

	private:
		Bool compare_string(const I8* bufferStr, const I8* str)
		{
			const I8* str1 = bufferStr;
			const I8* str2 = str;

			while (true)
			{
				if (*str1 == 0 && *str2 == 0)
				{
					return true;
				}
				if ((*str1 == 0 && *str2 != 0) || (*str1 != 0 && *str2 == 0))
				{
					return false;
				}
				if (*str1 != *str2)
				{
					return false;
				}
				str1++;
				str2++;
			}
			return false;
		}

		U32 getOffsetToNextString(const I8* buffer)
		{
			const I8* bufferStr = buffer;
			while (true)
			{
				if (*bufferStr == 0) {
					return static_cast<U32>(bufferStr - buffer + 1);
				}
				bufferStr++;
			}
			return 0;
		}

	private:
		std::mutex	m_mutex;
		I8*			m_buffer;
		U32			m_cursor;
		U32			m_bufferSize;
	};
}

namespace fluffy {
	/**
	 * TString
	 */

	TString::TString()
		: m_data(nullptr)
		, m_hash(0)
	{}

	TString::TString(std::string& str)
		: m_data(const_cast<I8*>(TStringBuffer::getSingleton()->put(str.c_str())))
		, m_hash(std::hash<std::string>{}(str))
	{}

	TString::TString(std::string&& str)
		: m_data(const_cast<I8*>(TStringBuffer::getSingleton()->put(str.c_str())))
		, m_hash(std::hash<std::string>{}(str))
	{}

	TString::~TString()
	{}

	const TString& TString::operator=(const TString& str)
	{
		m_data = str.m_data;
		m_hash = str.m_hash;
		return *this;
	}

	const TString& TString::operator=(const I8* str)
	{
		m_data = const_cast<I8*>(TStringBuffer::getSingleton()->put(str));
		m_hash = std::hash<std::string>{}(str);
		return *this;
	}

	const TString& TString::operator=(const std::string& str)
	{
		m_data = const_cast<I8*>(TStringBuffer::getSingleton()->put(str.c_str()));
		m_hash = std::hash<std::string>{}(str);
		return *this;
	}

	const TString& TString::operator=(const std::string&& str)
	{
		m_data = const_cast<I8*>(TStringBuffer::getSingleton()->put(str.c_str()));
		m_hash = std::hash<std::string>{}(str);
		return *this;
	}

	const I8* TString::str()
	{
		return m_data;
	}

	const I8* TString::str() const
	{
		return m_data;
	}

	const U64 TString::hash()
	{
		return m_hash;
	}

	const U64 TString::hash() const
	{
		return m_hash;
	}

	const Bool operator==(const TString& str1, const std::string& str2)
	{
		return str1.str() == str2;
	}

	const Bool operator==(const TString& str1, const TString& str2)
	{
		return str1.hash() == str2.hash();
	}

	const Bool operator!=(const TString& str1, const std::string& str2)
	{
		return str1.str() != str2;
	}

	const Bool operator!=(const TString& str1, const TString& str2)
	{
		return str1.hash() != str2.hash();
	}
}