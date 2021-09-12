#pragma once
#include "fl_ast_expr.h"
#include "fl_ast_block.h"
#include "fl_ast_type.h"
#include "fl_ast.h"

namespace fluffy { namespace ast {
	class TypeDecl;
} }

namespace fluffy { namespace ast { namespace pattern {
	using BlockDeclPtr						= std::unique_ptr<ast::BlockDecl>;
	using ExpressionDeclPtr					= std::unique_ptr<ast::expr::ExpressionDecl>;

	using TypeDeclPtr						= std::unique_ptr<ast::TypeDecl>;

	using PatternDeclPtr					= std::unique_ptr<class PatternDecl>;
	using PatternDeclPtrList				= std::vector<PatternDeclPtr>;

	using StructureItemPatternDeclPtr		= std::unique_ptr<class StructureItemPatternDecl>;
	using StructureItemPatternDeclPtrList	= std::vector<StructureItemPatternDeclPtr>;

	/**
	 * PatternDecl
	 */

	class PatternDecl : public AstNode
	{
	protected:
		PatternDecl(AstNodeType_e nodeType, U32 line, U32 column);

	public:
		virtual ~PatternDecl();
	};

	/**
	 * LiteralPatternDecl
	 */

	class LiteralPatternDecl : public PatternDecl, public AstSafeCast<AstNodeType_e::LiteralPattern>
	{
	public:
		LiteralPatternDecl(U32 line, U32 column);
		virtual ~LiteralPatternDecl();

		virtual Bool
		hasChildren();

		virtual std::vector<AstNode*>
		getChildren();

		ExpressionDeclPtr					literalExpr;
	};

	/**
	 * TuplePatternDecl
	 */

	class TuplePatternDecl : public PatternDecl, public AstSafeCast<AstNodeType_e::TuplePatternDecl>
	{	
	public:
		TuplePatternDecl(U32 line, U32 column);
		virtual ~TuplePatternDecl();

		virtual Bool
		hasChildren();

		virtual std::vector<AstNode*>
		getChildren();

		PatternDeclPtrList					patternItemDeclList;
	};

	/**
	 * StructurePatternDecl
	 */

	class StructurePatternDecl : public PatternDecl, public AstSafeCast<AstNodeType_e::StructurePatternDecl>
	{
	public:
		StructurePatternDecl(U32 line, U32 column);
		virtual ~StructurePatternDecl();

		virtual Bool
		hasChildren();

		virtual std::vector<AstNode*>
		getChildren();

		StructureItemPatternDeclPtrList		structureItemDeclList;
	};

	/**
	 * EnumerablePatternDecl
	 */

	class EnumerablePatternDecl : public PatternDecl, public AstSafeCast<AstNodeType_e::EnumerablePatternDecl>
	{
	public:
		EnumerablePatternDecl(U32 line, U32 column);
		virtual ~EnumerablePatternDecl();

		virtual Bool
		hasChildren();

		virtual std::vector<AstNode*>
		getChildren();

		PatternDeclPtrList					patternDeclItemList;
	};

	/**
	 * StructureItemPatternDecl
	 */

	class StructureItemPatternDecl : public AstNode, public AstSafeCast<AstNodeType_e::StructureItemPatternDecl>
	{
	public:
		StructureItemPatternDecl(U32 line, U32 column);
		virtual ~StructureItemPatternDecl();

		PatternDeclPtr						referencedPattern;
	};

} } }