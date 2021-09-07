#include <cstring>
#include "test.h"
namespace fluffy { namespace testing {
	String
	getProjectFilePath(const I8* file)
	{
		String url;
		url.resize(1024);
		sprintf_s(const_cast<I8*>(url.c_str()), 1024, "FluffyScriptTest\\%s", file);
		return url;
	}
} }