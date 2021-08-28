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
			: AstNode(AstNodeType_e::Block, line, column)
			, beginPosition(0)
			, endPosition(0)
		{}

		BlockDecl(U32 beginPosition, U32 endPosition, U32 line, U32 column)
			: AstNode(AstNodeType_e::Block, line, column)
			, beginPosition(beginPosition)
			, endPosition(endPosition)
		{}

		virtual ~BlockDecl()
		{}

		StmtDeclPtrList						stmtList;

		const U32							beginPosition;
		const U32							endPosition;
	};
} }