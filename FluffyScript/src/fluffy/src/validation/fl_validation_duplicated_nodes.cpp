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
	 * DuplicatedNodes
	 */

	DuplicatedNodes::DuplicatedNodes()
	{}

	DuplicatedNodes::~DuplicatedNodes()
	{}

	void
	DuplicatedNodes::onProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
	{
		// Faz a validação nos filhos.
		switch (node->nodeType)
		{
		case AstNodeType_e::CodeUnit:
		case AstNodeType_e::NamespaceDecl:
			validateDuplication(scopeManager, node);
			break;
		case AstNodeType_e::ClassDecl:
		case AstNodeType_e::InterfaceDecl:
		case AstNodeType_e::InterfaceFunctionDecl:
		case AstNodeType_e::StructDecl:
		case AstNodeType_e::EnumDecl:
		case AstNodeType_e::TraitDecl:
		case AstNodeType_e::TraitForDecl:
		case AstNodeType_e::FunctionDecl:
		case AstNodeType_e::AnomClassDeclExpr:
		case AstNodeType_e::MatchWhenExpr:
		case AstNodeType_e::NewBlockExpr:
		case AstNodeType_e::ClassConstructorDecl:
		case AstNodeType_e::ClassFunctionDecl:
		case AstNodeType_e::ClassDestructorDecl:
		case AstNodeType_e::TraitFunctionDecl:
		case AstNodeType_e::StmtFor:
		case AstNodeType_e::StmtWhile:
		case AstNodeType_e::StmtDoWhile:
		case AstNodeType_e::StmtTry:
		case AstNodeType_e::StmtCatchBlockDecl:
		case AstNodeType_e::StmtMatchWhenDecl:
		case AstNodeType_e::FunctionDeclExpr:
			validateDuplication(scopeManager, node);
			break;
		default:
			break;
		}
	}

	void
	DuplicatedNodes::validateDuplication(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
	{
		auto scope = scope::Scope(scopeManager, nullptr, node);
				
		NodeList list;
		for_each_ptr(scope.toMap(), [scopeManager, node, &list](ast::AstNode* const nodeA) {
			for_each(list, [scopeManager, node, nodeA](ast::AstNode* const nodeB) {
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
} }