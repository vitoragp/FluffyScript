#include <functional>
#include "ast\fl_ast_decl.h"
#include "utils\fl_ast_utils.h"
#include "validation\fl_validation_duplicated_nodes.h"
namespace fluffy { namespace validations {
	template <
		typename TListDst,
		typename TListSrc
	> void
	appendMap(TListDst& dest, const TListSrc& source)
	{
		auto itBegin = source.begin();
		auto itEnd = source.end();
		while (itBegin != itEnd) {
			dest.emplace(itBegin->get()->identifier, itBegin->get());
			itBegin++;
		}
	}

	template <
		typename TListDst,
		typename TUnique
	> void
	appendSingle(TListDst& dest, const TUnique& source)
	{
		if (source != nullptr) {
			dest.emplace(source->identifier, source.get());
		}
	}

	template <
		typename TListDst,
		typename TListSrc,
		typename TFunc
	> void
	filterMap(TListDst& dest, const TListSrc& source, TFunc func)
	{
		auto itBegin = source.begin();
		auto itEnd = source.end();
		while (itBegin != itEnd) {
			func(dest, itBegin->get());
			itBegin++;
		}
	}

	template <
		typename TListDst,
		typename TUnique,
		typename TFunc
	> void
	filterSingle(TListDst& dest, const TUnique& source, TFunc func)
	{
		if (source != nullptr) {
			func(dest, source.get());
		}
	}

	template <
		typename TList,
		typename TFunc
	> void
	for_each_ptr(TList& list, TFunc func)
	{
		auto itBegin = list.begin();
		auto itEnd = list.end();
		while (itBegin != itEnd) {
			func(itBegin->second);
			itBegin++;
		}
	}

	template <
		typename TList,
		typename TFunc
	> void
	for_each(TList& list, TFunc func)
	{
		auto itBegin = list.begin();
		auto itEnd = list.end();
		while (itBegin != itEnd) {
			func(*itBegin);
			itBegin++;
		}
	}

	/**
	 * appendPatternToScope
	 */

	void
	appendPatternToScope(NodeMultiMap& children, ast::pattern::PatternDecl* const pattern)
	{
		switch (pattern->nodeType)
		{
		case AstNodeType_e::LiteralPattern:
			{
				auto literalPattern = ast::safe_cast<ast::pattern::LiteralPatternDecl>(pattern);

				if (literalPattern->literalExpr == nullptr)
				{
					children.emplace(literalPattern->identifier, literalPattern);
				}
			}
			break;

		case AstNodeType_e::EnumerablePatternDecl:
			{
				auto enumerablePattern = ast::safe_cast<ast::pattern::EnumerablePatternDecl>(pattern);

				filterMap(children, enumerablePattern->patternDeclItemList, appendPatternToScope);
			}
			break;

		case AstNodeType_e::StructurePatternDecl:
			{
				auto structurePattern = ast::safe_cast<ast::pattern::StructurePatternDecl>(pattern);

				filterMap(children, structurePattern->structureItemDeclList, [](NodeMultiMap& children, ast::pattern::StructureItemPatternDecl* const node) {
					if (node->referencedPattern == nullptr)
					{
						children.emplace(node->identifier, node);
					}
					else
					{
						filterSingle(children, node->referencedPattern, appendPatternToScope);
					}
				});
			}
			break;

		case AstNodeType_e::StructureItemPatternDecl:
			{
				auto structureItemPattern = ast::safe_cast<ast::pattern::StructureItemPatternDecl>(pattern);

				if (structureItemPattern->referencedPattern == nullptr)
				{
					children.emplace(structureItemPattern->identifier, structureItemPattern);
				}
				else
				{
					filterSingle(children, structureItemPattern->referencedPattern, appendPatternToScope);
				}
			}
			break;

		case AstNodeType_e::TuplePatternDecl:
			{
				auto tuplePattern = ast::safe_cast<ast::pattern::TuplePatternDecl>(pattern);

				filterMap(children, tuplePattern->patternItemDeclList, appendPatternToScope);
			}
			break;

		default:
			break;
		}
	}

	/**
	 * ValidationDuplicatedNodes
	 */

	ValidationDuplicatedNodes::ValidationDuplicatedNodes()
	{}

	ValidationDuplicatedNodes::~ValidationDuplicatedNodes()
	{}

	void
	ValidationDuplicatedNodes::onProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
	{
		// Faz a validação nos filhos.
		switch (node->nodeType)
		{
		case AstNodeType_e::CodeUnit:
			if (auto n = reinterpret_cast<ast::CodeUnit*>(node))
			{
				auto scope = scope::Scope(scopeManager, nullptr, n);
				
				NodeList list;
				for_each_ptr(scope.toMap(), [scopeManager, n, &list](ast::AstNode* const nodeA) {
					for_each(list, [scopeManager, n, nodeA](ast::AstNode* const nodeB) {
						if (utils::AstUtils::equals(nodeA, nodeB)) {
							ast::AstNode* duplicatedNode = nullptr;
							if (nodeA->line > nodeB->line)
							{
								duplicatedNode = nodeA;
							}
							else if (nodeA->line == nodeB->line && nodeA->column > nodeB->column)
							{
								duplicatedNode = nodeA;
							}
							else
							{
								duplicatedNode = nodeB;
							}

							throw exceptions::custom_exception(
								"%s error: Duplicated identifier '%s'",
								duplicatedNode->line, duplicatedNode->column,
								n->identifier.str(),
								duplicatedNode->identifier.str()
							);
						}
					});
					list.emplace_back(nodeA);
				});
			}
			break;

		case AstNodeType_e::NamespaceDecl:
			if (auto n = reinterpret_cast<ast::NamespaceDecl*>(node))
			{
				auto scope = scopeManager->getParentScope();

				NodeList list;
				for_each_ptr(scope.toMap(), [scopeManager, n, &list](ast::AstNode* const nodeA) {
					for_each(list, [scopeManager, n, nodeA](ast::AstNode* const nodeB) {
						if (utils::AstUtils::equals(nodeA, nodeB)) {
							ast::AstNode* duplicatedNode = nullptr;
							if (nodeA->line > nodeB->line)
							{
								duplicatedNode = nodeA;
							}
							else if (nodeA->line == nodeB->line && nodeA->column > nodeB->column)
							{
								duplicatedNode = nodeA;
							}
							else
							{
								duplicatedNode = nodeB;
							}

							throw exceptions::custom_exception(
								"%s error: Duplicated identifier '%s'",
								duplicatedNode->line, duplicatedNode->column,
								scopeManager->getCodeUnitName().str(),
								duplicatedNode->identifier.str()
							);
						}
					});
					list.emplace_back(nodeA);
				});
			}
			break;

		case AstNodeType_e::ClassDecl:
			if (auto n = reinterpret_cast<ast::ClassDecl*>(node))
			{
			}
			break;

		case AstNodeType_e::InterfaceDecl:
			if (auto n = reinterpret_cast<ast::InterfaceDecl*>(node))
			{
			}
			break;

		case AstNodeType_e::InterfaceFunctionDecl:
			if (auto n = reinterpret_cast<ast::InterfaceFunctionDecl*>(node))
			{
			}
			break;

		case AstNodeType_e::StructDecl:
			if (auto n = reinterpret_cast<ast::StructDecl*>(node))
			{
			}
			break;

		case AstNodeType_e::EnumDecl:
			if (auto n = reinterpret_cast<ast::EnumDecl*>(node))
			{
			}
			break;

		case AstNodeType_e::TraitDecl:
			if (auto n = reinterpret_cast<ast::TraitDecl*>(node))
			{
			}
			break;

		case AstNodeType_e::TraitForDecl:
			if (auto n = reinterpret_cast<ast::TraitForDecl*>(node))
			{
			}
			break;

		case AstNodeType_e::NewExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewDecl*>(node))
			{
			}
			break;

		case AstNodeType_e::AnomClassDeclExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionAnomClassDecl*>(node))
			{
			}
			break;

		case AstNodeType_e::MatchWhenExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionMatchWhenDecl*>(node))
			{
			}
			break;

		case AstNodeType_e::NewBlockExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewBlockDecl*>(node))
			{
			}
			break;

		case AstNodeType_e::ClassConstructorDecl:
			{
			}
			break;

		case AstNodeType_e::ClassFunctionDecl:
			{
			}
			break;

		case AstNodeType_e::ClassDestructorDecl:
			{
			}
			break;

		case AstNodeType_e::TraitFunctionDecl:
			{
			}
			break;

		case AstNodeType_e::FunctionDecl:
			{
			}
			break;

		case AstNodeType_e::StmtFor:
			{
			}
			break;

		case AstNodeType_e::StmtWhile:
			{
			}
			break;

		case AstNodeType_e::StmtDoWhile:
			{				
			}
			break;

		case AstNodeType_e::StmtTry:
			{
			}
			break;

		case AstNodeType_e::StmtCatchBlockDecl:
			{
			}
			break;

		case AstNodeType_e::StmtMatchWhenDecl:
			{
			}
			break;

		case AstNodeType_e::FunctionDeclExpr:
			{
			}
			break;

		default:
			break;
		}
	}
} }