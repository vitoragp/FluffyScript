#include <functional>
#include "ast\fl_ast_decl.h"
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
	 * ScopeUtils
	 */

	NodeMultiMap
	ScopeUtils::getPositionalSimplifiedNodeChildrenMap(ast::AstNode* const parent, ast::AstNode* const node)
	{
		NodeMultiMap children;

		// Faz a validação nos filhos.
		switch (node->nodeType)
		{
		case AstNodeType_e::CodeUnit:
			if (auto n = reinterpret_cast<ast::CodeUnit*>(node))
			{
				filterMap(children, n->includeDeclList, [](NodeMultiMap& children, ast::IncludeDecl* const node) {
					for (auto& it : node->includedItemList)
					{
						if (it->referencedAlias == TString(nullptr))
						{
							children.emplace(it->identifier, it.get());
						}
						else
						{
							children.emplace(it->referencedAlias, it.get());
						}
					}
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

	/*
	NodeMultiMap
	ScopeUtils::getNodeChildrenMap(ast::AstNode* const node)
	{
		NodeMultiMap children;

		// Faz a validação nos filhos.
		switch (node->nodeType)
		{
		case AstNodeType_e::CodeUnit:
			if (auto n = reinterpret_cast<ast::CodeUnit*>(node))
			{
				appendMap(children, n->includeDeclList);
				appendMap(children, n->namespaceDeclList);
			}
			break;

		case AstNodeType_e::IncludeDecl:
			if (auto n = reinterpret_cast<ast::IncludeDecl*>(node))
			{
				appendMap(children, n->includedItemList);
			}
			break;

		case AstNodeType_e::IncludeItemDecl:
			if (auto n = reinterpret_cast<ast::IncludeItemDecl*>(node))
			{
				appendSingle(children, n->referencedPath);
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
				appendMap(children, n->genericDecl->genericDeclItemList);

				appendSingle(children, n->baseClass);
				appendMap(children, n->interfaceList);

				appendMap(children, n->constructorList);
				appendMap(children, n->functionList);
				appendMap(children, n->variableList);

				appendSingle(children, n->destructorDecl);
			}
			break;

		case AstNodeType_e::ClassConstructorDecl:
			if (auto n = reinterpret_cast<ast::ClassConstructorDecl*>(node))
			{
				appendMap(children, n->parameterList);
				appendSingle(children, n->superInitExpr);
				appendMap(children, n->variableInitDeclList);
				appendSingle(children, n->blockDecl);
			}
			break;

		case AstNodeType_e::ClassDestructorDecl:
			if (auto n = reinterpret_cast<ast::ClassDestructorDecl*>(node))
			{
				appendSingle(children, n->blockDecl);
			}
			break;

		case AstNodeType_e::ClassFunctionDecl:
			if (auto n = reinterpret_cast<ast::ClassFunctionDecl*>(node))
			{
				appendMap(children, n->genericDecl->genericDeclItemList);
				appendSingle(children, n->sourceTypeDecl);
				appendMap(children, n->parameterList);
				appendSingle(children, n->returnType);
				appendSingle(children, n->blockDecl);
				appendSingle(children, n->exprDecl);
			}
			break;

		case AstNodeType_e::ClassVariableDecl:
			if (auto n = reinterpret_cast<ast::ClassVariableDecl*>(node))
			{
				appendSingle(children, n->typeDecl);
				appendSingle(children, n->initExpr);
			}
			break;

		case AstNodeType_e::ClassVariableInitDecl:
			if (auto n = reinterpret_cast<ast::ClassVariableInitDecl*>(node))
			{
				appendSingle(children, n->initExpr);
			}
			break;

		case AstNodeType_e::InterfaceDecl:
			if (auto n = reinterpret_cast<ast::InterfaceDecl*>(node))
			{
				appendMap(children, n->genericDecl->genericDeclItemList);
				appendMap(children, n->functionDeclList);
			}
			break;

		case AstNodeType_e::InterfaceFunctionDecl:
			if (auto n = reinterpret_cast<ast::InterfaceFunctionDecl*>(node))
			{
				appendMap(children, n->genericDecl->genericDeclItemList);
				appendMap(children, n->parameterList);
				appendSingle(children, n->returnType);
			}
			break;

		case AstNodeType_e::StructDecl:
			if (auto n = reinterpret_cast<ast::StructDecl*>(node))
			{
				appendMap(children, n->genericDecl->genericDeclItemList);
				appendMap(children, n->variableList);
			}
			break;

		case AstNodeType_e::StructVariableDecl:
			if (auto n = reinterpret_cast<ast::StructVariableDecl*>(node))
			{
				appendSingle(children, n->typeDecl);
				appendSingle(children, n->initExpr);
			}
			break;

		case AstNodeType_e::EnumDecl:
			if (auto n = reinterpret_cast<ast::EnumDecl*>(node))
			{
				appendMap(children, n->genericDecl->genericDeclItemList);
				appendMap(children, n->enumItemDeclList);
			}
			break;

		case AstNodeType_e::EnumItemDecl:
			if (auto n = reinterpret_cast<ast::EnumItemDecl*>(node))
			{
				appendMap(children, n->dataTypeDeclList);
				appendSingle(children, n->valueExpression);
			}
			break;

		case AstNodeType_e::TraitDecl:
			if (auto n = reinterpret_cast<ast::TraitDecl*>(node))
			{
				appendMap(children, n->genericDecl->genericDeclItemList);
				appendMap(children, n->functionDeclList);
			}
			break;

		case AstNodeType_e::TraitForDecl:
			if (auto n = reinterpret_cast<ast::TraitForDecl*>(node))
			{
				appendSingle(children, n->typeDefinitionDecl);
				appendMap(children, n->genericDecl->genericDeclItemList);
				appendMap(children, n->functionDeclList);
			}
			break;

		case AstNodeType_e::TraitFunctionDecl:
			if (auto n = reinterpret_cast<ast::TraitFunctionDecl*>(node))
			{
				appendMap(children, n->genericDecl->genericDeclItemList);
				appendMap(children, n->parameterList);
				appendSingle(children, n->returnType);
				appendSingle(children, n->blockDecl);
				appendSingle(children, n->exprDecl);
			}
			break;

		case AstNodeType_e::FunctionDecl:
			if (auto n = reinterpret_cast<ast::FunctionDecl*>(node))
			{
				appendMap(children, n->genericDecl->genericDeclItemList);
				appendMap(children, n->parameterList);
				appendSingle(children, n->returnType);
				appendSingle(children, n->blockDecl);
				appendSingle(children, n->exprDecl);
			}
			break;

		case AstNodeType_e::VariableDecl:
			if (auto n = reinterpret_cast<ast::VariableDecl*>(node))
			{
				appendSingle(children, n->typeDecl);
				appendSingle(children, n->initExpr);
			}
			break;

		case AstNodeType_e::FunctionParameterDecl:
			if (auto n = reinterpret_cast<ast::FunctionParameterDecl*>(node))
			{
				appendSingle(children, n->typeDecl);
				appendSingle(children, n->patternDecl);
			}
			break;

		case AstNodeType_e::GenericDecl:
			if (auto n = reinterpret_cast<ast::GenericDecl*>(node))
			{
				appendMap(children, n->genericDeclItemList);
			}
			break;

		case AstNodeType_e::GenericItemDecl:
			if (auto n = reinterpret_cast<ast::GenericItemDecl*>(node))
			{
				appendMap(children, n->whereTypeList);
			}
			break;

		case AstNodeType_e::ScopedIdentifierDecl:
			if (auto n = reinterpret_cast<ast::ScopedIdentifierDecl*>(node))
			{
				appendSingle(children, n->referencedIdentifier);
			}
			break;

		case AstNodeType_e::Block:
			if (auto n = reinterpret_cast<ast::BlockDecl*>(node))
			{
				appendMap(children, n->stmtList);
			}
			break;

		case AstNodeType_e::StmtIf:
			if (auto n = reinterpret_cast<ast::stmt::StmtIfDecl*>(node))
			{
				appendSingle(children, n->conditionExprDecl);
				appendSingle(children, n->ifBlockDecl);
				appendSingle(children, n->elseBlockDecl);
			}
			break;

		case AstNodeType_e::StmtIfLet:
			if (auto n = reinterpret_cast<ast::stmt::StmtIfLetDecl*>(node))
			{
				appendSingle(children, n->patternDecl);
				appendSingle(children, n->expressionDecl);
				appendSingle(children, n->ifBlockDecl);
				appendSingle(children, n->elseBlockDecl);
			}
			break;

		case AstNodeType_e::StmtFor:
			if (auto n = reinterpret_cast<ast::stmt::StmtForDecl*>(node))
			{
				appendSingle(children, n->initStmtDecl);
				appendSingle(children, n->initExprDecl);
				appendSingle(children, n->conditionExprDecl);
				appendSingle(children, n->updateExprDecl);
				appendSingle(children, n->blockDecl);
			}
			break;

		case AstNodeType_e::StmtWhile:
			if (auto n = reinterpret_cast<ast::stmt::StmtWhileDecl*>(node))
			{
				appendSingle(children, n->conditionExprDecl);
				appendSingle(children, n->blockDecl);
			}
			break;

		case AstNodeType_e::StmtDoWhile:
			if (auto n = reinterpret_cast<ast::stmt::StmtDoWhileDecl*>(node))
			{
				appendSingle(children, n->conditionExprDecl);
				appendSingle(children, n->blockDecl);
			}
			break;

		case AstNodeType_e::StmtMatch:
			if (auto n = reinterpret_cast<ast::stmt::StmtMatchDecl*>(node))
			{
				appendSingle(children, n->conditionExprDecl);
				appendMap(children, n->whenDeclList);
			}
			break;

		case AstNodeType_e::StmtReturn:
			if (auto n = reinterpret_cast<ast::stmt::StmtReturnDecl*>(node))
			{
				appendSingle(children, n->exprDecl);
			}
			break;

		case AstNodeType_e::StmtTry:
			if (auto n = reinterpret_cast<ast::stmt::StmtTryDecl*>(node))
			{
				appendSingle(children, n->blockDecl);
				appendMap(children, n->catchDeclList);
			}
			break;

		case AstNodeType_e::StmtPanic:
			if (auto n = reinterpret_cast<ast::stmt::StmtPanicDecl*>(node))
			{
				appendSingle(children, n->exprDecl);
			}
			break;

		case AstNodeType_e::StmtVariable:
			if (auto n = reinterpret_cast<ast::stmt::StmtVariableDecl*>(node))
			{
				appendSingle(children, n->typeDecl);
				appendSingle(children, n->initExpr);
				appendSingle(children, n->patternDecl);
			}
			break;

		case AstNodeType_e::StmtExpr:
			if (auto n = reinterpret_cast<ast::stmt::StmtExprDecl*>(node))
			{
				appendSingle(children, n->exprDecl);
			}
			break;

		case AstNodeType_e::StmtForInitDecl:
			if (auto n = reinterpret_cast<ast::stmt::StmtForInitDecl*>(node))
			{
				appendSingle(children, n->typeDecl);
				appendSingle(children, n->initExpr);
			}
			break;

		case AstNodeType_e::StmtMatchWhenDecl:
			if (auto n = reinterpret_cast<ast::stmt::StmtMatchWhenDecl*>(node))
			{
				appendSingle(children, n->patternDecl);
				appendSingle(children, n->blockDecl);
			}
			break;

		case AstNodeType_e::StmtCatchBlockDecl:
			if (auto n = reinterpret_cast<ast::stmt::StmtCatchBlockDecl*>(node))
			{
				appendSingle(children, n->patternDecl);
				appendSingle(children, n->typeDecl);
				appendSingle(children, n->blockDecl);
			}
			break;

		case AstNodeType_e::TernaryExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionTernaryDecl*>(node))
			{
				appendSingle(children, n->conditionDecl);
				appendSingle(children, n->leftDecl);
				appendSingle(children, n->rightDecl);
			}
			break;

		case AstNodeType_e::BinaryExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionBinaryDecl*>(node))
			{
				appendSingle(children, n->leftDecl);
				appendSingle(children, n->rightDecl);
			}
			break;

		case AstNodeType_e::UnaryExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionUnaryDecl*>(node))
			{
				appendSingle(children, n->exprDecl);
			}
			break;

		case AstNodeType_e::AsExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionAsDecl*>(node))
			{
				appendSingle(children, n->exprDecl);
				appendSingle(children, n->typeDecl);
			}
			break;

		case AstNodeType_e::IsExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionIsDecl*>(node))
			{
				appendSingle(children, n->exprDecl);
				appendSingle(children, n->typeDecl);
			}
			break;

		case AstNodeType_e::MatchExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionMatchDecl*>(node))
			{
				appendSingle(children, n->exprDecl);
				appendMap(children, n->whenDeclList);
			}
			break;

		case AstNodeType_e::FunctionDeclExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionFunctionDecl*>(node))
			{
				appendMap(children, n->parametersDeclList);
				appendSingle(children, n->returnTypeDecl);
				appendSingle(children, n->blockDecl);
				appendSingle(children, n->exprDecl);
			}
			break;

		case AstNodeType_e::FunctionCallExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionFunctionCall*>(node))
			{
				appendSingle(children, n->lhsDecl);
				appendSingle(children, n->rhsDecl);
			}
			break;

		case AstNodeType_e::GenericCallExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionGenericCallDecl*>(node))
			{
				appendMap(children, n->genericTypeList);
				appendSingle(children, n->lhsDecl);
				appendSingle(children, n->rhsDecl);
			}
			break;

		case AstNodeType_e::IndexExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionIndexDecl*>(node))
			{
				appendSingle(children, n->lhsDecl);
				appendSingle(children, n->rhsDecl);
			}
			break;

		case AstNodeType_e::NewExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewDecl*>(node))
			{
				appendSingle(children, n->objTypeDecl);
				appendSingle(children, n->exprDecl);
				appendSingle(children, n->objInitBlockDecl);
			}
			break;

		case AstNodeType_e::AnomClassDeclExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionAnomClassDecl*>(node))
			{
				appendSingle(children, n->baseReferencedDecl);
				appendMap(children, n->functionList);
				appendMap(children, n->variableList);
				appendMap(children, n->constructorList);
				appendSingle(children, n->destructorDecl);
			}
			break;

		case AstNodeType_e::PrimitiveTypeExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionPrimitiveTypeDecl*>(node))
			{
				appendSingle(children, n->typeDecl);
			}
			break;

		case AstNodeType_e::ArrayInitExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionArrayInitDecl*>(node))
			{
				appendMap(children, n->arrayElementDeclList);
			}
			break;

		case AstNodeType_e::MatchWhenExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionMatchWhenDecl*>(node))
			{
				appendSingle(children, n->patternDecl);
				appendSingle(children, n->exprDecl);
			}
			break;

		case AstNodeType_e::FunctionParameterDeclExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionFunctionParameterDecl*>(node))
			{
				appendSingle(children, n->typeDecl);
				appendSingle(children, n->patternDecl);
			}
			break;

		case AstNodeType_e::NewBlockExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewBlockDecl*>(node))
			{
				appendMap(children, n->itemDeclList);
			}
			break;

		case AstNodeType_e::NewItemExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewItemDecl*>(node))
			{
				appendSingle(children, n->exprDecl);
			}
			break;

		case AstNodeType_e::LiteralPattern:
			if (auto n = reinterpret_cast<ast::pattern::LiteralPatternDecl*>(node))
			{
				appendSingle(children, n->literalExpr);
			}
			break;

		case AstNodeType_e::TuplePatternDecl:
			if (auto n = reinterpret_cast<ast::pattern::TuplePatternDecl*>(node))
			{
				appendMap(children, n->patternItemDeclList);
			}
			break;

		case AstNodeType_e::StructurePatternDecl:
			if (auto n = reinterpret_cast<ast::pattern::StructurePatternDecl*>(node))
			{
				appendMap(children, n->structureItemDeclList);
			}
			break;

		case AstNodeType_e::EnumerablePatternDecl:
			if (auto n = reinterpret_cast<ast::pattern::EnumerablePatternDecl*>(node))
			{
				appendSingle(children, n->enumReferenced);
				appendMap(children, n->patternDeclItemList);
			}
			break;

		case AstNodeType_e::StructureItemPatternDecl:
			if (auto n = reinterpret_cast<ast::pattern::StructureItemPatternDecl*>(node))
			{
				appendSingle(children, n->referencedPattern);
			}
			break;

		case AstNodeType_e::ArrayType:
			if (auto n = reinterpret_cast<ast::TypeDeclArray*>(node))
			{
				appendMap(children, n->arrayDeclList);
				appendSingle(children, n->valueType);
			}
			break;

		case AstNodeType_e::FunctionType:
			if (auto n = reinterpret_cast<ast::TypeDeclFunction*>(node))
			{
				appendSingle(children, n->objectOwnerDecl);
				appendMap(children, n->parameterTypeList);
				appendSingle(children, n->returnType);
			}
			break;

		case AstNodeType_e::TupleType:
			if (auto n = reinterpret_cast<ast::TypeDeclTuple*>(node))
			{
				appendMap(children, n->tupleItemList);
			}
			break;

		case AstNodeType_e::NamedType:
			if (auto n = reinterpret_cast<ast::TypeDeclNamed*>(node))
			{
				appendSingle(children, n->scopedReferenceDecl);
				appendMap(children, n->genericDefinitionList);
			}
			break;

		default:
			break;
		}
		return children;
	}

	NodeMultiMapList
	ScopeUtils::getSimplifiedScopeChildrenMap(ast::AstNode* const node)
	{
		NodeMultiMapList scopes;

		// Faz a validação nos filhos.
		switch (node->nodeType)
		{
		case AstNodeType_e::CodeUnit:
			if (auto n = reinterpret_cast<ast::CodeUnit*>(node))
			{
				NodeMultiMap children;

				filterMap(children, n->includeDeclList, [](NodeMultiMap& children, ast::IncludeDecl* const node) {
					for (auto& it : node->includedItemList)
					{
						if (it->referencedAlias == TString(nullptr))
						{
							children.emplace(it->identifier, it.get());
						}
						else
						{
							children.emplace(it->referencedAlias, it.get());
						}
					}
				});

				appendMap(children, n->namespaceDeclList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::NamespaceDecl:
			if (auto n = reinterpret_cast<ast::NamespaceDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->namespaceDeclList);
				appendMap(children, n->generalDeclList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::ClassDecl:
			if (auto n = reinterpret_cast<ast::ClassDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->genericDecl->genericDeclItemList);

				appendMap(children, n->constructorList);
				appendMap(children, n->functionList);
				appendMap(children, n->variableList);

				appendSingle(children, n->destructorDecl);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::ClassConstructorDecl:
			if (auto n = reinterpret_cast<ast::ClassConstructorDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->parameterList);
				appendMap(children, n->blockDecl->stmtList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::ClassDestructorDecl:
			if (auto n = reinterpret_cast<ast::ClassDestructorDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->blockDecl->stmtList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::ClassFunctionDecl:
			if (auto n = reinterpret_cast<ast::ClassFunctionDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->genericDecl->genericDeclItemList);
				appendMap(children, n->parameterList);
				appendMap(children, n->blockDecl->stmtList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::InterfaceDecl:
			if (auto n = reinterpret_cast<ast::InterfaceDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->genericDecl->genericDeclItemList);
				appendMap(children, n->functionDeclList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::InterfaceFunctionDecl:
			if (auto n = reinterpret_cast<ast::InterfaceFunctionDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->genericDecl->genericDeclItemList);
				appendMap(children, n->parameterList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::StructDecl:
			if (auto n = reinterpret_cast<ast::StructDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->genericDecl->genericDeclItemList);
				appendMap(children, n->variableList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::EnumDecl:
			if (auto n = reinterpret_cast<ast::EnumDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->genericDecl->genericDeclItemList);
				appendMap(children, n->enumItemDeclList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::TraitDecl:
			if (auto n = reinterpret_cast<ast::TraitDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->genericDecl->genericDeclItemList);
				appendMap(children, n->functionDeclList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::TraitForDecl:
			if (auto n = reinterpret_cast<ast::TraitForDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->genericDecl->genericDeclItemList);
				appendMap(children, n->functionDeclList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::TraitFunctionDecl:
			if (auto n = reinterpret_cast<ast::TraitFunctionDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->genericDecl->genericDeclItemList);
				appendMap(children, n->parameterList);
				appendMap(children, n->blockDecl->stmtList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::FunctionDecl:
			if (auto n = reinterpret_cast<ast::FunctionDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->genericDecl->genericDeclItemList);
				appendMap(children, n->parameterList);
				appendMap(children, n->blockDecl->stmtList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::FunctionParameterDecl:
			if (auto n = reinterpret_cast<ast::FunctionParameterDecl*>(node))
			{
				NodeMultiMap children;

				appendSingle(children, n->typeDecl);
				appendSingle(children, n->patternDecl);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::GenericDecl:
			if (auto n = reinterpret_cast<ast::GenericDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->genericDeclItemList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::Block:
			if (auto n = reinterpret_cast<ast::BlockDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->stmtList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::StmtIf:
			if (auto n = reinterpret_cast<ast::stmt::StmtIfDecl*>(node))
			{
				NodeMultiMap childrenIf, childrenElse;

				appendSingle(childrenIf, n->ifBlockDecl);
				appendSingle(childrenElse, n->elseBlockDecl);

				scopes.emplace_back(childrenIf);
				scopes.emplace_back(childrenElse);
			}
			break;

		case AstNodeType_e::StmtIfLet:
			if (auto n = reinterpret_cast<ast::stmt::StmtIfLetDecl*>(node))
			{
				NodeMultiMap childrenIf, childrenElse;

				appendSingle(childrenIf, n->patternDecl);
				appendSingle(childrenIf, n->ifBlockDecl);

				appendSingle(childrenElse, n->patternDecl);
				appendSingle(childrenElse, n->elseBlockDecl);

				scopes.emplace_back(childrenIf);
				scopes.emplace_back(childrenElse);
			}
			break;

		case AstNodeType_e::StmtFor:
			if (auto n = reinterpret_cast<ast::stmt::StmtForDecl*>(node))
			{
				NodeMultiMap children;

				appendSingle(children, n->initStmtDecl);
				appendMap(children, n->blockDecl->stmtList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::StmtWhile:
			if (auto n = reinterpret_cast<ast::stmt::StmtWhileDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->blockDecl->stmtList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::StmtDoWhile:
			if (auto n = reinterpret_cast<ast::stmt::StmtDoWhileDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->blockDecl->stmtList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::StmtTry:
			if (auto n = reinterpret_cast<ast::stmt::StmtTryDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->blockDecl->stmtList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::StmtMatchWhenDecl:
			if (auto n = reinterpret_cast<ast::stmt::StmtMatchWhenDecl*>(node))
			{
				NodeMultiMap children;

				appendSingle(children, n->patternDecl);
				appendMap(children, n->blockDecl->stmtList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::StmtCatchBlockDecl:
			if (auto n = reinterpret_cast<ast::stmt::StmtCatchBlockDecl*>(node))
			{
				NodeMultiMap children;

				appendSingle(children, n->patternDecl);
				appendMap(children, n->blockDecl->stmtList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::FunctionDeclExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionFunctionDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->parametersDeclList);
				appendMap(children, n->blockDecl->stmtList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::NewExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->objInitBlockDecl->itemDeclList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::AnomClassDeclExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionAnomClassDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->functionList);
				appendMap(children, n->variableList);
				appendMap(children, n->constructorList);
				appendSingle(children, n->destructorDecl);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::MatchWhenExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionMatchWhenDecl*>(node))
			{
				NodeMultiMap children;

				appendSingle(children, n->patternDecl);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::FunctionParameterDeclExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionFunctionParameterDecl*>(node))
			{
				NodeMultiMap children;

				appendSingle(children, n->typeDecl);
				appendSingle(children, n->patternDecl);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::NewBlockExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewBlockDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->itemDeclList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::LiteralPattern:
			if (auto n = reinterpret_cast<ast::pattern::LiteralPatternDecl*>(node))
			{
				NodeMultiMap children;

				appendSingle(children, n->literalExpr);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::TuplePatternDecl:
			if (auto n = reinterpret_cast<ast::pattern::TuplePatternDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->patternItemDeclList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::StructurePatternDecl:
			if (auto n = reinterpret_cast<ast::pattern::StructurePatternDecl*>(node))
			{
				NodeMultiMap children;

				appendMap(children, n->structureItemDeclList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::EnumerablePatternDecl:
			if (auto n = reinterpret_cast<ast::pattern::EnumerablePatternDecl*>(node))
			{
				NodeMultiMap children;

				appendSingle(children, n->enumReferenced);
				appendMap(children, n->patternDeclItemList);

				scopes.emplace_back(children);
			}
			break;

		case AstNodeType_e::StructureItemPatternDecl:
			if (auto n = reinterpret_cast<ast::pattern::StructureItemPatternDecl*>(node))
			{
				NodeMultiMap children;

				appendSingle(children, n->referencedPattern);

				scopes.emplace_back(children);
			}
			break;

		default:
			break;
		}
		return scopes;
	}
	*/

	
} }