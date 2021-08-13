#pragma once
#include <memory>
#include <vector>
#include "fl_defs.h"
namespace fluffy { namespace ast {
	using std::vector;
	using std::unique_ptr;

	using StringList = vector<String>;

	using IncludePtr = unique_ptr<class Include>;
	using IncludePtrList = vector<IncludePtr>;

	/**
	 * Program
	 */

	class Program
	{
	public:
								Program();
								~Program();

		IncludePtrList			includeList;
	};

	/**
	 * Include
	 */

	class Include
	{
	public:
								Include();
								~Include();

		StringList				identifierList;
		Bool					allFlag;
		String					from;
	};	
} }
