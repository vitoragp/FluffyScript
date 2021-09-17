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
			if (auto newBuffer = malloc(bufferSize))
			{
				m_buffer = static_cast<I8*>(newBuffer);
			}
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

		std::tuple<U64, const I8*>
		put(const I8* str)
		{
			if (str == nullptr)
			{
				return std::tuple<U64, const I8*>(0, nullptr);
			}
			const U64 strHash = std::hash<std::string>{}(str);

			// Verifica se string ja existe.
			auto strFinded = mStringLibrary.find(strHash);
			if (strFinded != mStringLibrary.end()) {
				return std::tuple<U64, const I8*>(strFinded->first, strFinded->second);
			}

			std::lock_guard<std::mutex> guard(m_mutex);
			{
				// Verifica se e necesseario aumentar o tamanho do buffer.
				if (m_bufferSize - m_cursor < 256)
				{
					m_bufferSize += 8192; // 8K
					if (auto newBuffer = realloc(m_buffer, m_bufferSize))
					{
						m_buffer = static_cast<I8*>(newBuffer);
					}
				}

				I8* bufferStrBeg = m_buffer + m_cursor;
				I8* strCur = const_cast<I8*>(str);

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
						if (auto newBuffer = realloc(m_buffer, m_bufferSize))
						{
							m_buffer = static_cast<I8*>(newBuffer);
						}
					}

					mStringLibrary.emplace(strHash, bufferStrBeg);

					return std::tuple<U64, const I8*>(strHash, bufferStrBeg);
				}
			}
			return std::tuple<U64, const I8*>(0, nullptr);
		}

	private:
		std::unordered_map<U64, const I8*>
		mStringLibrary;

		std::mutex
		m_mutex;

		I8*
		m_buffer;

		U32
		m_cursor;

		U32
		m_bufferSize;
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

	TString::TString(const I8* str)
		: m_data(nullptr)
		, m_hash(0)
	{
		auto t = TStringBuffer::getSingleton()->put(str);
		m_hash = std::get<0>(t);
		m_data = const_cast<I8*>(std::get<1>(t));
	}

	TString::TString(std::string& str)
		: m_data(nullptr)
		, m_hash(0)
	{
		auto t = TStringBuffer::getSingleton()->put(str.c_str());
		m_hash = std::get<0>(t);
		m_data = const_cast<I8*>(std::get<1>(t));
	}

	TString::TString(std::string&& str)
		: m_data(nullptr)
		, m_hash(0)
	{
		auto t = TStringBuffer::getSingleton()->put(str.c_str());
		m_hash = std::get<0>(t);
		m_data = const_cast<I8*>(std::get<1>(t));
	}

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
		auto t = TStringBuffer::getSingleton()->put(str);
		m_hash = std::get<0>(t);
		m_data = const_cast<I8*>(std::get<1>(t));
		return *this;
	}

	const TString& TString::operator=(const std::string& str)
	{
		auto t = TStringBuffer::getSingleton()->put(str.c_str());
		m_hash = std::get<0>(t);
		m_data = const_cast<I8*>(std::get<1>(t));
		return *this;
	}

	const TString& TString::operator=(const std::string&& str)
	{
		auto t = TStringBuffer::getSingleton()->put(str.c_str());
		m_hash = std::get<0>(t);
		m_data = const_cast<I8*>(std::get<1>(t));
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
		if (str1.str() == nullptr) {
			return str1.str() == str2.c_str();
		}
		return str1.str() == str2;
	}

	const Bool operator==(const TString& str1, const TString& str2)
	{
		return str1.hash() == str2.hash();
	}

	const Bool operator==(const TString& str1, const I8* str2)
	{
		return str1.hash() == TString(str2).hash();
	}

	const Bool operator!=(const TString& str1, const std::string& str2)
	{
		if (str1.str() == nullptr) {
			return str1.str() != str2.c_str();
		}
		return str1.str() != str2;
	}

	const Bool operator!=(const TString& str1, const TString& str2)
	{
		return str1.hash() != str2.hash();
	}

	const Bool operator!=(const TString& str1, const I8* str2)
	{
		return str1.hash() != TString(str2).hash();
	}
}