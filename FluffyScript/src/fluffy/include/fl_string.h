#pragma once
#include "fl_defs.h"
namespace fluffy {
	/**
	 * TString
	 */

	class TString
	{
	public:
								TString();
								TString(const I8* str);
								TString(std::string& str);
								TString(std::string&& str);

								~TString();

		const TString&			operator=(const TString& str);
		const TString&			operator=(const I8* str);
		const TString&			operator=(const std::string& str);
		const TString&			operator=(const std::string&& str);

		const I8*				str();
		const I8*				str() const;
		const U64				hash();
		const U64				hash() const;

	private:
		I8*						m_data;
		U64						m_hash;
	};

	const Bool					operator==(const TString& str1, const std::string& str2);
	const Bool					operator==(const TString& str1, const TString& str2);
	const Bool					operator==(const TString& str1, const I8* str2);
	const Bool					operator!=(const TString& str1, const std::string& str2);
	const Bool					operator!=(const TString& str1, const TString& str2);
	const Bool					operator!=(const TString& str1, const I8* str2);

	/**
	 * TStringHash
	 */

	class TStringHash {
	public:
		size_t operator() (TString const& key) const
		{
			return key.hash();
		}
	};

	/**
	 * TStringEqual
	 */

	class TStringEqual {
	public:
		size_t operator() (TString const& a, TString const& b) const
		{
			return a.hash() == b.hash();
		}
	};
}