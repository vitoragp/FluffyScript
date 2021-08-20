#pragma once
#include "fl_ast.h"

namespace fluffy { namespace ast { namespace stmt {
	class StmtDecl;
} } }

namespace fluffy { namespace ast {
	using StmtDeclPtr						= std::unique_ptr<stmt::StmtDecl>;
	using StmtDeclPtrList					= std::vector<StmtDeclPtr>;

	/**
	 * BlockDecl
	 */

	class BlockDecl : public AstNode
	{
	public:
		BlockDecl(U32 line, U32 column)
			: AstNode(line, column)
		{}

		virtual ~BlockDecl()
		{}

		StmtDeclPtrList						stmtList;
	};
} }