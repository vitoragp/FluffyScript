#include "ast\fl_ast_pattern.h"
#include "ast\fl_ast_expr.h"
#include "ast\fl_ast_block.h"
#include "ast\fl_ast.h"
namespace fluffy { namespace ast { namespace pattern {
	/**
	 * PatternDecl
	 */

	PatternDecl::PatternDecl(AstNodeType_e nodeType, U32 line, U32 column)
		: AstNode(nodeType, line, column)
	{}

	PatternDecl::~PatternDecl()
	{}


	/**
	 * LiteralPatternDecl
	 */

	LiteralPatternDecl::LiteralPatternDecl(U32 line, U32 column)
		: PatternDecl(AstNodeType_e::LiteralPattern, line, column)
	{}

	LiteralPatternDecl::~LiteralPatternDecl()
	{}

	Bool
	LiteralPatternDecl::hasChildren()
	{
		return false;
	}

	std::vector<AstNode*>
	LiteralPatternDecl::getChildren()
	{
		return std::vector<AstNode*>();
	}

	/**
	 * TuplePatternDecl
	 */

	TuplePatternDecl::TuplePatternDecl(U32 line, U32 column)
		: PatternDecl(AstNodeType_e::TuplePattern, line, column)
	{}

	TuplePatternDecl::~TuplePatternDecl()
	{}

	Bool
	TuplePatternDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	TuplePatternDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->patternItemDeclList)
		{
			if (!it->hasChildren())
			{
				children.push_back(it.get());
			}
			else
			{
				auto itChildren = it->getChildren();
				children.insert(children.end(), itChildren.begin(), itChildren.end());
			}
		}
		return children;
	}

	/**
	 * StructurePatternDecl
	 */

	StructurePatternDecl::StructurePatternDecl(U32 line, U32 column)
		: PatternDecl(AstNodeType_e::StructurePattern, line, column)
	{}

	StructurePatternDecl::~StructurePatternDecl()
	{}

	Bool
	StructurePatternDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	StructurePatternDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->structureItemDeclList)
		{
			if (!it->hasChildren())
			{
				children.push_back(it.get());
			}
			else
			{
				auto itChildren = it->getChildren();
				children.insert(children.end(), itChildren.begin(), itChildren.end());
			}
		}
		return children;
	}

	/**
	 * EnumerablePatternDecl
	 */

	EnumerablePatternDecl::EnumerablePatternDecl(U32 line, U32 column)
		: PatternDecl(AstNodeType_e::EnumerablePattern, line, column)
	{}

	EnumerablePatternDecl::~EnumerablePatternDecl()
	{}

	Bool
	EnumerablePatternDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	EnumerablePatternDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->patternDeclItemList)
		{
			if (!it->hasChildren())
			{
				children.push_back(it.get());
			}
			else
			{
				auto itChildren = it->getChildren();
				children.insert(children.end(), itChildren.begin(), itChildren.end());
			}
		}
		return children;
	}

	/**
	 * StructureItemPatternDecl
	 */

	StructureItemPatternDecl::StructureItemPatternDecl(U32 line, U32 column)
		: AstNodeIdentified(AstNodeType_e::StructureItemPatternDecl, line, column)
	{}

	StructureItemPatternDecl::~StructureItemPatternDecl()
	{}

} } }