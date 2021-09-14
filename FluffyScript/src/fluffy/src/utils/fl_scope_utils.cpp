#include <functional>
#include "ast\fl_ast_decl.h"
#include "scope\fl_scope_manager.h"
#include "utils\fl_ast_utils.h"
#include "utils\fl_scope_utils.h"
namespace fluffy { namespace utils {
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
	 * IncludeItemProcessor
	 */

	class IncludeItemProcessor : public scope::NodeProcessor
	{
	public:
		IncludeItemProcessor(ast::CodeUnit* codeUnit,  ast::IncludeItemDecl* const node)
			: mCodeUnit(codeUnit)
			, mIncludeItem(node)
		{}

		virtual ~IncludeItemProcessor()
		{}

		virtual void
		onProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node) override
		{
			if (node->nodeType == AstNodeType_e::CodeUnit)
			{
				auto codeUnit = ast::safe_cast<ast::CodeUnit>(node);

				auto scope = scope::Scope(scopeManager, nullptr, node);
				auto nextId = mIncludeItem->scopePath.get();

				while (nextId)
				{
					auto findResult = scope.findNodeById(nextId->identifier);

					if (!findResult.foundResult)
					{
						throw exceptions::custom_exception(
							"%s error: Identifier '%s' not found in scope",
							mIncludeItem->line, mIncludeItem->column,
							scopeManager->getCodeUnitName().str(),
							nextId->identifier.str()
						);
					}

					if (findResult.nodeList.size() > 1)
					{
						throw exceptions::custom_exception(
							"%s error: Ambiguous search result",
							mIncludeItem->line, mIncludeItem->column,
							scopeManager->getCodeUnitName().str()
						);
					}

					// Cria uma funcao anonima para verficar se o escopo pode ser importado.
					auto checkExportModifier = [this](ast::AstNode* const node) {
						switch (node->nodeType)
						{
						case AstNodeType_e::NamespaceDecl:
							break;
						case AstNodeType_e::ClassDecl:
						case AstNodeType_e::InterfaceDecl:
						case AstNodeType_e::TraitDecl:
						case AstNodeType_e::StructDecl:
						case AstNodeType_e::EnumDecl:
						case AstNodeType_e::FunctionDecl:
						case AstNodeType_e::VariableDecl:
							{
								auto generalStmt = dynamic_cast<ast::GeneralStmtDecl*>(node);
								if (!generalStmt->isExported) {
									throw exceptions::custom_exception(
										"%s error: When trying to access an unexported element '%s'",
										mIncludeItem->line, mIncludeItem->column,
										mCodeUnit->identifier.str(),
										utils::AstUtils::printIncludeItem(mIncludeItem).c_str()
									);
								}
							}
							break;
						default:
							break;
						}
					};

					// Verifica se resultado pode ser importado.
					checkExportModifier(findResult.nodeList[0]);

					// Atualiza o escopo para o no encontrado.
					scope.assign(findResult.scope, findResult.nodeList[0]);

					if (nextId->scopedChildPath)
					{
						nextId = nextId->scopedChildPath.get();
					}
					else
					{
						findResult = scope.findNodeById(mIncludeItem->identifier);

						if (!findResult.foundResult)
						{
							throw exceptions::custom_exception(
								"%s error: Identifier '%s' not found in scope",
								mIncludeItem->line, mIncludeItem->column,
								scopeManager->getCodeUnitName().str(),
								mIncludeItem->identifier.str()
							);
						}

						// TraitFor nao e considerado uma declaracao, e um complemento para o trait
						// entao deve ser removido da lista.
						Bool hasDeclNode = false;
						for (auto it = findResult.nodeList.begin(); it != findResult.nodeList.end();)
						{
							if ((*it)->nodeType == AstNodeType_e::TraitForDecl)
							{
								auto traitOf = ast::safe_cast<ast::TraitForDecl>(*it);

								if (!traitOf->isExported)
								{
									it = findResult.nodeList.erase(it);
								}
							}
							else
							{
								hasDeclNode = true;

								// Verifica se resultado pode ser importado.
								checkExportModifier(*it);

								it++;
							}
						}

						if (!hasDeclNode)
						{
							throw exceptions::custom_exception(
								"%s error: identifier '%s' not found in scope '%s'",
								mIncludeItem->line, mIncludeItem->column,
								mCodeUnit->identifier.str(),
								mIncludeItem->identifier.str(),
								utils::AstUtils::printIncludeItem(mIncludeItem)
							);
						}

						// Interrompe o processamento.
						scopeManager->interrupt();

						break;
					}
				}
			}
		}
		
	private:
		ast::CodeUnit* const
		mCodeUnit;

		ast::IncludeItemDecl* const
		mIncludeItem;
	};

	/**
	 * ScopeUtils
	 */

	NodeMultiMap
	ScopeUtils::getPositionalSimplifiedNodeChildrenMap(scope::ScopeManager* const scopeManager, ast::AstNode* const parent, ast::AstNode* const node)
	{
		NodeMultiMap children;

		// Faz a validação nos filhos.
		switch (node->nodeType)
		{
		case AstNodeType_e::CodeUnit:
			if (auto n = reinterpret_cast<ast::CodeUnit*>(node))
			{
				ast::for_each(n->includeDeclList, [n, scopeManager, &children](ast::IncludeDecl* const node) {
					auto includeCodeUnit = scopeManager->findCodeUnitByName(node->inFile);

					ast::for_each(node->includedItemList, [n, scopeManager, includeCodeUnit, &children](ast::IncludeItemDecl* const includeNode) {

					});
				});

				appendMap(children, n->namespaceDeclList);
			}
			break;

		case AstNodeType_e::NamespaceDecl:
			if (auto n = reinterpret_cast<ast::NamespaceDecl*>(node))
			{
				appendMap(children, n->namespaceDeclList);
				appendMap(children, n->generalDeclList);
			}
			break;

		case AstNodeType_e::ClassDecl:
			if (auto n = reinterpret_cast<ast::ClassDecl*>(node))
			{
				if (n->genericDecl) {
					appendMap(children, n->genericDecl->genericDeclItemList);
				}

				appendMap(children, n->constructorList);
				appendMap(children, n->functionList);
				appendMap(children, n->variableList);

				appendSingle(children, n->destructorDecl);
			}
			break;

		case AstNodeType_e::InterfaceDecl:
			if (auto n = reinterpret_cast<ast::InterfaceDecl*>(node))
			{
				if (n->genericDecl) {
					appendMap(children, n->genericDecl->genericDeclItemList);
				}

				appendMap(children, n->functionDeclList);
			}
			break;

		case AstNodeType_e::InterfaceFunctionDecl:
			if (auto n = reinterpret_cast<ast::InterfaceFunctionDecl*>(node))
			{
				if (n->genericDecl) {
					appendMap(children, n->genericDecl->genericDeclItemList);
				}

				filterMap(children, n->parameterList, [](NodeMultiMap& children, ast::FunctionParameterDecl* const param) {
					if (param->patternDecl == nullptr)
					{
						children.emplace(param->identifier, param);
					}
					else
					{
						filterSingle(children, param->patternDecl, appendPatternToScope);
					}
				});
			}
			break;

		case AstNodeType_e::StructDecl:
			if (auto n = reinterpret_cast<ast::StructDecl*>(node))
			{
				if (n->genericDecl) {
					appendMap(children, n->genericDecl->genericDeclItemList);
				}

				appendMap(children, n->variableList);
			}
			break;

		case AstNodeType_e::EnumDecl:
			if (auto n = reinterpret_cast<ast::EnumDecl*>(node))
			{
				if (n->genericDecl) {
					appendMap(children, n->genericDecl->genericDeclItemList);
				}

				appendMap(children, n->enumItemDeclList);
			}
			break;

		case AstNodeType_e::TraitDecl:
			if (auto n = reinterpret_cast<ast::TraitDecl*>(node))
			{
				if (n->genericDecl) {
					appendMap(children, n->genericDecl->genericDeclItemList);
				}

				appendMap(children, n->functionDeclList);
			}
			break;

		case AstNodeType_e::TraitForDecl:
			if (auto n = reinterpret_cast<ast::TraitForDecl*>(node))
			{
				if (n->genericDecl) {
					appendMap(children, n->genericDecl->genericDeclItemList);
				}

				appendMap(children, n->functionDeclList);
			}
			break;

		case AstNodeType_e::NewExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewDecl*>(node))
			{
				appendMap(children, n->objInitBlockDecl->itemDeclList);
			}
			break;

		case AstNodeType_e::AnomClassDeclExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionAnomClassDecl*>(node))
			{
				appendMap(children, n->functionList);
				appendMap(children, n->variableList);
				appendMap(children, n->constructorList);
				appendSingle(children, n->destructorDecl);
			}
			break;

		case AstNodeType_e::MatchWhenExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionMatchWhenDecl*>(node))
			{
				appendSingle(children, n->patternDecl);
			}
			break;

		case AstNodeType_e::NewBlockExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewBlockDecl*>(node))
			{
				appendMap(children, n->itemDeclList);
			}
			break;

		case AstNodeType_e::ClassConstructorDecl:
			{
				auto n = ast::safe_cast<ast::ClassConstructorDecl>(node);

				filterMap(children, n->parameterList, [](NodeMultiMap& children, ast::FunctionParameterDecl* const param) {
					if (param->patternDecl == nullptr)
					{
						children.emplace(param->identifier, param);
					}
					else
					{
						filterSingle(children, param->patternDecl, appendPatternToScope);
					}
				});

				filterMap(children, n->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							children.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});
			}
			break;

		case AstNodeType_e::ClassFunctionDecl:
			{
				auto n = ast::safe_cast<ast::ClassFunctionDecl>(node);

				filterMap(children, n->parameterList, [](NodeMultiMap& children, ast::FunctionParameterDecl* const param) {
					if (param->patternDecl == nullptr)
					{
						children.emplace(param->identifier, param);
					}
					else
					{
						filterSingle(children, param->patternDecl, appendPatternToScope);
					}
				});

				filterMap(children, n->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							children.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});
			}
			break;

		case AstNodeType_e::ClassDestructorDecl:
			{
				auto n = ast::safe_cast<ast::ClassDestructorDecl>(node);

				filterMap(children, n->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							children.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});
			}
			break;

		case AstNodeType_e::TraitFunctionDecl:
			{
				auto n = ast::safe_cast<ast::TraitFunctionDecl>(node);

				filterMap(children, n->parameterList, [](NodeMultiMap& children, ast::FunctionParameterDecl* const param) {
					if (param->patternDecl == nullptr)
					{
						children.emplace(param->identifier, param);
					}
					else
					{
						filterSingle(children, param->patternDecl, appendPatternToScope);
					}
				});

				filterMap(children, n->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							children.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});
			}
			break;

		case AstNodeType_e::FunctionDecl:
			{
				auto n = ast::safe_cast<ast::FunctionDecl>(node);

				filterMap(children, n->parameterList, [](NodeMultiMap& children, ast::FunctionParameterDecl* const param) {
					if (param->patternDecl == nullptr)
					{
						children.emplace(param->identifier, param);
					}
					else
					{
						filterSingle(children, param->patternDecl, appendPatternToScope);
					}
				});

				filterMap(children, n->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							children.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});
			}
			break;

		case AstNodeType_e::StmtFor:
			{
				auto n = ast::safe_cast<ast::stmt::StmtForDecl>(node);

				appendSingle(children, n->initStmtDecl);

				filterMap(children, n->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							children.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});						
			}
			break;

		case AstNodeType_e::StmtWhile:
			{
				auto n = ast::safe_cast<ast::stmt::StmtWhileDecl>(node);

				filterMap(children, n->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							children.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});						
			}
			break;

		case AstNodeType_e::StmtDoWhile:
			{
				auto n = ast::safe_cast<ast::stmt::StmtDoWhileDecl>(node);

				filterMap(children, n->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							children.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});						
			}
			break;

		case AstNodeType_e::StmtTry:
			{
				auto n = ast::safe_cast<ast::stmt::StmtTryDecl>(node);

				filterMap(children, n->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							children.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});						
			}
			break;

		case AstNodeType_e::StmtCatchBlockDecl:
			{
				auto n = ast::safe_cast<ast::stmt::StmtCatchBlockDecl>(node);

				filterMap(children, n->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							children.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});						
			}
			break;

		case AstNodeType_e::StmtMatchWhenDecl:
			{
				auto n = ast::safe_cast<ast::stmt::StmtMatchWhenDecl>(node);

				filterMap(children, n->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							children.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});						
			}
			break;

		case AstNodeType_e::FunctionDeclExpr:
			{
				auto n = ast::safe_cast<ast::expr::ExpressionFunctionDecl>(node);

				filterMap(children, n->parametersDeclList, [](NodeMultiMap& children, ast::ExpressionFunctionParameterDecl* const param) {
					if (param->patternDecl == nullptr)
					{
						children.emplace(param->identifier, param);
					}
					else
					{
						filterSingle(children, param->patternDecl, appendPatternToScope);
					}
				});

				filterMap(children, n->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							children.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});						
			}
			break;

		case AstNodeType_e::Block:
			if (auto n = reinterpret_cast<ast::BlockDecl*>(node))
			{
				switch (parent->nodeType)
				{
				case AstNodeType_e::ClassConstructorDecl:
					{
						auto s = ast::safe_cast<ast::ClassConstructorDecl>(parent);

						filterMap(children, s->parameterList, [](NodeMultiMap& children, ast::FunctionParameterDecl* const param) {
							if (param->patternDecl == nullptr)
							{
								children.emplace(param->identifier, param);
							}
							else
							{
								filterSingle(children, param->patternDecl, appendPatternToScope);
							}
						});

						filterMap(children, s->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									children.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});
					}
					break;

				case AstNodeType_e::ClassFunctionDecl:
					{
						auto s = ast::safe_cast<ast::ClassFunctionDecl>(parent);

						filterMap(children, s->parameterList, [](NodeMultiMap& children, ast::FunctionParameterDecl* const param) {
							if (param->patternDecl == nullptr)
							{
								children.emplace(param->identifier, param);
							}
							else
							{
								filterSingle(children, param->patternDecl, appendPatternToScope);
							}
						});

						filterMap(children, s->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									children.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});
					}
					break;

				case AstNodeType_e::ClassDestructorDecl:
					{
						auto s = ast::safe_cast<ast::ClassDestructorDecl>(parent);

						filterMap(children, s->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									children.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});
					}
					break;

				case AstNodeType_e::TraitFunctionDecl:
					{
						auto s = ast::safe_cast<ast::TraitFunctionDecl>(parent);

						filterMap(children, s->parameterList, [](NodeMultiMap& children, ast::FunctionParameterDecl* const param) {
							if (param->patternDecl == nullptr)
							{
								children.emplace(param->identifier, param);
							}
							else
							{
								filterSingle(children, param->patternDecl, appendPatternToScope);
							}
						});

						filterMap(children, s->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									children.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});
					}
					break;

				case AstNodeType_e::FunctionDecl:
					{
						auto s = ast::safe_cast<ast::FunctionDecl>(parent);

						filterMap(children, s->parameterList, [](NodeMultiMap& children, ast::FunctionParameterDecl* const param) {
							if (param->patternDecl == nullptr)
							{
								children.emplace(param->identifier, param);
							}
							else
							{
								filterSingle(children, param->patternDecl, appendPatternToScope);
							}
						});

						filterMap(children, s->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									children.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});
					}
					break;

				case AstNodeType_e::StmtIf:
					{
						auto s = ast::safe_cast<ast::stmt::StmtIfDecl>(parent);

						if (n == s->ifBlockDecl.get())
						{
							filterMap(children, s->ifBlockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
								if (stmt->nodeType == AstNodeType_e::StmtVariable)
								{
									auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

									if (stmtVar->patternDecl == nullptr)
									{
										children.emplace(stmt->identifier, stmt);
									}
									else
									{
										filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
									}
								}
							});
						}
						else
						{
							filterMap(children, s->elseBlockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
								if (stmt->nodeType == AstNodeType_e::StmtVariable)
								{
									auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

									if (stmtVar->patternDecl == nullptr)
									{
										children.emplace(stmt->identifier, stmt);
									}
									else
									{
										filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
									}
								}
							});
						}						
					}
					break;

				case AstNodeType_e::StmtIfLet:
					{
						auto s = ast::safe_cast<ast::stmt::StmtIfLetDecl>(parent);

						if (n == s->ifBlockDecl.get())
						{
							filterMap(children, s->ifBlockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
								if (stmt->nodeType == AstNodeType_e::StmtVariable)
								{
									auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

									if (stmtVar->patternDecl == nullptr)
									{
										children.emplace(stmt->identifier, stmt);
									}
									else
									{
										filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
									}
								}
							});
						}
						else
						{
							filterMap(children, s->elseBlockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
								if (stmt->nodeType == AstNodeType_e::StmtVariable)
								{
									auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

									if (stmtVar->patternDecl == nullptr)
									{
										children.emplace(stmt->identifier, stmt);
									}
									else
									{
										filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
									}
								}
							});
						}						
					}
					break;

				case AstNodeType_e::StmtFor:
					{
						auto s = ast::safe_cast<ast::stmt::StmtForDecl>(parent);

						appendSingle(children, s->initStmtDecl);

						filterMap(children, s->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									children.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});						
					}
					break;

				case AstNodeType_e::StmtWhile:
					{
						auto s = ast::safe_cast<ast::stmt::StmtWhileDecl>(parent);

						filterMap(children, s->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									children.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});						
					}
					break;

				case AstNodeType_e::StmtDoWhile:
					{
						auto s = ast::safe_cast<ast::stmt::StmtDoWhileDecl>(parent);

						filterMap(children, s->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									children.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});						
					}
					break;

				case AstNodeType_e::StmtTry:
					{
						auto s = ast::safe_cast<ast::stmt::StmtTryDecl>(parent);

						filterMap(children, s->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									children.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});						
					}
					break;

				case AstNodeType_e::StmtCatchBlockDecl:
					{
						auto s = ast::safe_cast<ast::stmt::StmtCatchBlockDecl>(parent);

						filterMap(children, s->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									children.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});						
					}
					break;

				case AstNodeType_e::StmtMatchWhenDecl:
					{
						auto s = ast::safe_cast<ast::stmt::StmtMatchWhenDecl>(parent);

						filterMap(children, s->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									children.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});						
					}
					break;

				case AstNodeType_e::FunctionDeclExpr:
					{
						auto s = ast::safe_cast<ast::expr::ExpressionFunctionDecl>(parent);

						filterMap(children, s->parametersDeclList, [](NodeMultiMap& children, ast::ExpressionFunctionParameterDecl* const param) {
							if (param->patternDecl == nullptr)
							{
								children.emplace(param->identifier, param);
							}
							else
							{
								filterSingle(children, param->patternDecl, appendPatternToScope);
							}
						});

						filterMap(children, s->blockDecl->stmtList, [](NodeMultiMap& children, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									children.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(children, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});						
					}
					break;

				default:
					break;
				}
			}
			break;

		default:
			break;
		}
		return children;
	}

	class ExtractExtendsDecl : public scope::NodeProcessor
	{
	public:
		ExtractExtendsDecl(ast::AstNode* const target)
			: mTarget(target)
			, mExtendsObject(nullptr)
		{}

		virtual ~ExtractExtendsDecl()
		{}

		virtual void
		onProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node) override
		{
			if (node == mTarget)
			{
			}
		}

	private:
		ast::AstNode* const
		mTarget;

		ast::AstNode*
		mExtendsObject;
	};

	ast::AstNode*
	ScopeUtils::resolveExtendsClass(scope::ScopeManager* scopeManager, ast::AstNode* const node)
	{
		auto classDecl = ast::safe_cast<ast::ClassDecl>(node);

		if (classDecl->baseClass == nullptr)
		{
			return nullptr;
		}

		scope::ScopeManager scopeManagerInt;
		scopeManagerInt.copyReferenceTree(scopeManager);

		ExtractExtendsDecl extractExtendsDecl(node);

		return nullptr;
	}
	
} }