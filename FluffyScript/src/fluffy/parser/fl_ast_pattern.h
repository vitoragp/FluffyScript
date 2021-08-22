#pragma once
#include "fl_ast_expr.h"
#include "fl_ast_block.h"
#include "fl_ast.h"
namespace fluffy { namespace ast { namespace pattern {
	using BlockDeclPtr						= std::unique_ptr<ast::BlockDecl>;
	using ExpressionDeclPtr					= std::unique_ptr<ast::expr::ExpressionDecl>;

	using PatternDeclPtr					= std::unique_ptr<class PatternDecl>;

	using DestructuringPatternDeclPtr		= std::unique_ptr<class DestructuringPatternDecl>;

	using DestructuringItemDeclPtr			= std::unique_ptr<class DestructuringItemDecl>;
	using DestructuringItemDeclPtrList		= std::vector<DestructuringItemDeclPtr>;

	/**
	 * PatternDecl
	 */

	class PatternDecl : public AstNode
	{
	protected:
		PatternDecl(PatterType_e type, U32 line, U32 column)
			: AstNode(line, column)
			, type(type)
		{}

	public:
		virtual ~PatternDecl()
		{}

		const PatterType_e						type;
	};

	/**
	 * LiteralPatternDecl
	 */

	class LiteralPatternDecl : public PatternDecl
	{
	public:
		LiteralPatternDecl(U32 line, U32 column)
			: PatternDecl(PatterType_e::Literal, line, column)
		{}

		virtual ~LiteralPatternDecl()
		{}

		ExpressionDeclPtr					patternExpr;
	};

	/**
	 * DestructuringPatternDecl
	 */

	class DestructuringPatternDecl : public PatternDecl
	{	
	public:
		DestructuringPatternDecl(U32 line, U32 column)
			: PatternDecl(PatterType_e::Destructuring, line, column)
			, destructuringType(DestructuringType_e::Unknown)
		{}

		virtual ~DestructuringPatternDecl()
		{}

		DestructuringType_e					destructuringType;
		DestructuringItemDeclPtrList		destructuringItemDeclList;
		ExpressionDeclPtr					enumExpr;
	};

	/**
	 * DestructuringItemDecl
	 */

	class DestructuringItemDecl : public AstNodeIdentified
	{
	public:
		DestructuringItemDecl(U32 line, U32 column)
			: AstNodeIdentified(line, column)
			, destructuringItemType(DestructuringItemType_e::Unknown)
		{}

		virtual ~DestructuringItemDecl()
		{}

		DestructuringItemType_e				destructuringItemType;
		ExpressionDeclPtr					indirectRefOrMatchingDecl;
		PatternDeclPtr						subDestructuringPattern;

	};
} } }