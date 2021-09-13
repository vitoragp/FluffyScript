#include "ast\fl_ast.h"
#include "ast\fl_ast_decl.h"
#include "scope\fl_scope.h"
#include "scope\fl_scope_manager.h"
#include "fl_exceptions.h"

namespace fluffy { namespace scope {
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
} }

namespace fluffy { namespace scope {
	/**
	 * FindResult_t
	 */

	const FindResult_t&
	FindResult_t::operator =(const FindResult_t& fr)
	{
		nodeList = fr.nodeList;
		scope = fr.scope;
		foundResult = fr.foundResult;
		return *this;
	}

	/**
	 * Scope
	 */

	Scope::Scope(ScopeManager* scopeManager, ast::AstNode* const scope, ast::AstNode* const node)
		: mScopeManager(scopeManager)
		, mScope(scope)
		, mNode(node)
		, mFetched(false)
	{}

	Scope::~Scope()
	{}

	ast::AstNode* const
	Scope::getNode()
	{
		return mNode;
	}

	ast::AstNode* const
	Scope::getScope()
	{
		return mScope;
	}

	const NodeMultiMap&
	Scope::toMap()
	{
		if (!mFetched) {
			fetch();
		}
		return mMap;
	}

	FindResult_t
	Scope::findNodeById(const TString& identifier)
	{
		NodeList children;
		if (!mFetched) {
			fetch();
		}
		auto res = mMap.equal_range(identifier);
		while (res.first != res.second) {
			children.emplace_back(res.first->second);
			res.first++;
		}		
		return children.size()
			? FindResult_t { getNode(), std::move(children), true }
			: FindResult_t { nullptr, NodeList(), false};
	}

	void
	Scope::fetch()
	{
		// Faz a validação nos filhos.
		switch (mNode->nodeType)
		{
		case AstNodeType_e::CodeUnit:
			if (auto n = reinterpret_cast<ast::CodeUnit*>(mNode))
			{
				filterMap(mMap, n->includeDeclList, [](NodeMultiMap& map, ast::IncludeDecl* const includeDecl) {
					filterMap(map, includeDecl->includedItemList, [](NodeMultiMap& map, ast::BaseIncludeItemDecl* const baseIncludeItemDecl) {
						switch (baseIncludeItemDecl->nodeType)
						{
						case AstNodeType_e::IncludeItemDecl:
							{
								auto includeItemDecl = ast::safe_cast<ast::IncludeItemDecl>(baseIncludeItemDecl);

								if (includeItemDecl->includeAll)
								{
									break;
								}

								if (includeItemDecl->referencedAlias == TString(nullptr))
								{
									map.emplace(includeItemDecl->identifier, includeItemDecl);
								}
								else
								{
									map.emplace(includeItemDecl->referencedAlias, includeItemDecl);
								}
							}
							break;

						case AstNodeType_e::WeakIncludeItemDecl:
							{
								auto weakIncludeItemDecl = ast::safe_cast<ast::WeakIncludeItemDecl>(baseIncludeItemDecl);
								map.emplace(weakIncludeItemDecl->identifier, weakIncludeItemDecl);
							}
							break;

						default:
							break;

						}
					});
				});

				appendMap(mMap, n->namespaceDeclList);
			}
			break;

		case AstNodeType_e::NamespaceDecl:
			if (auto n = reinterpret_cast<ast::NamespaceDecl*>(mNode))
			{
				appendMap(mMap, n->namespaceDeclList);
				appendMap(mMap, n->generalDeclList);
			}
			break;

		case AstNodeType_e::ClassDecl:
			if (auto n = reinterpret_cast<ast::ClassDecl*>(mNode))
			{
				if (n->genericDecl) {
					appendMap(mMap, n->genericDecl->genericDeclItemList);
				}

				appendMap(mMap, n->constructorList);
				appendMap(mMap, n->functionList);
				appendMap(mMap, n->variableList);

				appendSingle(mMap, n->destructorDecl);
			}
			break;

		case AstNodeType_e::InterfaceDecl:
			if (auto n = reinterpret_cast<ast::InterfaceDecl*>(mNode))
			{
				if (n->genericDecl) {
					appendMap(mMap, n->genericDecl->genericDeclItemList);
				}

				appendMap(mMap, n->functionDeclList);
			}
			break;

		case AstNodeType_e::InterfaceFunctionDecl:
			if (auto n = reinterpret_cast<ast::InterfaceFunctionDecl*>(mNode))
			{
				if (n->genericDecl) {
					appendMap(mMap, n->genericDecl->genericDeclItemList);
				}

				filterMap(mMap, n->parameterList, [](NodeMultiMap& mMap, ast::FunctionParameterDecl* const param) {
					if (param->patternDecl == nullptr)
					{
						mMap.emplace(param->identifier, param);
					}
					else
					{
						filterSingle(mMap, param->patternDecl, appendPatternToScope);
					}
				});
			}
			break;

		case AstNodeType_e::StructDecl:
			if (auto n = reinterpret_cast<ast::StructDecl*>(mNode))
			{
				if (n->genericDecl) {
					appendMap(mMap, n->genericDecl->genericDeclItemList);
				}

				appendMap(mMap, n->variableList);
			}
			break;

		case AstNodeType_e::EnumDecl:
			if (auto n = reinterpret_cast<ast::EnumDecl*>(mNode))
			{
				if (n->genericDecl) {
					appendMap(mMap, n->genericDecl->genericDeclItemList);
				}

				appendMap(mMap, n->enumItemDeclList);
			}
			break;

		case AstNodeType_e::TraitDecl:
			if (auto n = reinterpret_cast<ast::TraitDecl*>(mNode))
			{
				if (n->genericDecl) {
					appendMap(mMap, n->genericDecl->genericDeclItemList);
				}

				appendMap(mMap, n->functionDeclList);
			}
			break;

		case AstNodeType_e::TraitForDecl:
			if (auto n = reinterpret_cast<ast::TraitForDecl*>(mNode))
			{
				if (n->genericDecl) {
					appendMap(mMap, n->genericDecl->genericDeclItemList);
				}

				appendMap(mMap, n->functionDeclList);
			}
			break;

		case AstNodeType_e::NewExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewDecl*>(mNode))
			{
				appendMap(mMap, n->objInitBlockDecl->itemDeclList);
			}
			break;

		case AstNodeType_e::AnomClassDeclExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionAnomClassDecl*>(mNode))
			{
				appendMap(mMap, n->functionList);
				appendMap(mMap, n->variableList);
				appendMap(mMap, n->constructorList);
				appendSingle(mMap, n->destructorDecl);
			}
			break;

		case AstNodeType_e::MatchWhenExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionMatchWhenDecl*>(mNode))
			{
				appendSingle(mMap, n->patternDecl);
			}
			break;

		case AstNodeType_e::NewBlockExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewBlockDecl*>(mNode))
			{
				appendMap(mMap, n->itemDeclList);
			}
			break;

		case AstNodeType_e::ClassConstructorDecl:
			{
				auto n = ast::safe_cast<ast::ClassConstructorDecl>(mNode);

				filterMap(mMap, n->parameterList, [](NodeMultiMap& mMap, ast::FunctionParameterDecl* const param) {
					if (param->patternDecl == nullptr)
					{
						mMap.emplace(param->identifier, param);
					}
					else
					{
						filterSingle(mMap, param->patternDecl, appendPatternToScope);
					}
				});

				filterMap(mMap, n->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							mMap.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});
			}
			break;

		case AstNodeType_e::ClassFunctionDecl:
			{
				auto n = ast::safe_cast<ast::ClassFunctionDecl>(mNode);

				if (n->genericDecl) {
					appendMap(mMap, n->genericDecl->genericDeclItemList);
				}

				filterMap(mMap, n->parameterList, [](NodeMultiMap& mMap, ast::FunctionParameterDecl* const param) {
					if (param->patternDecl == nullptr)
					{
						mMap.emplace(param->identifier, param);
					}
					else
					{
						filterSingle(mMap, param->patternDecl, appendPatternToScope);
					}
				});

				filterMap(mMap, n->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							mMap.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});
			}
			break;

		case AstNodeType_e::ClassDestructorDecl:
			{
				auto n = ast::safe_cast<ast::ClassDestructorDecl>(mNode);

				filterMap(mMap, n->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							mMap.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});
			}
			break;

		case AstNodeType_e::TraitFunctionDecl:
			{
				auto n = ast::safe_cast<ast::TraitFunctionDecl>(mNode);

				if (n->genericDecl) {
					appendMap(mMap, n->genericDecl->genericDeclItemList);
				}

				filterMap(mMap, n->parameterList, [](NodeMultiMap& mMap, ast::FunctionParameterDecl* const param) {
					if (param->patternDecl == nullptr)
					{
						mMap.emplace(param->identifier, param);
					}
					else
					{
						filterSingle(mMap, param->patternDecl, appendPatternToScope);
					}
				});

				filterMap(mMap, n->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							mMap.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});
			}
			break;

		case AstNodeType_e::FunctionDecl:
			{
				auto n = ast::safe_cast<ast::FunctionDecl>(mNode);

				if (n->genericDecl) {
					appendMap(mMap, n->genericDecl->genericDeclItemList);
				}

				filterMap(mMap, n->parameterList, [](NodeMultiMap& mMap, ast::FunctionParameterDecl* const param) {
					if (param->patternDecl == nullptr)
					{
						mMap.emplace(param->identifier, param);
					}
					else
					{
						filterSingle(mMap, param->patternDecl, appendPatternToScope);
					}
				});

				filterMap(mMap, n->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							mMap.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});
			}
			break;

		case AstNodeType_e::StmtFor:
			{
				auto n = ast::safe_cast<ast::stmt::StmtForDecl>(mNode);

				appendSingle(mMap, n->initStmtDecl);

				filterMap(mMap, n->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							mMap.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});						
			}
			break;

		case AstNodeType_e::StmtWhile:
			{
				auto n = ast::safe_cast<ast::stmt::StmtWhileDecl>(mNode);

				filterMap(mMap, n->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							mMap.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});						
			}
			break;

		case AstNodeType_e::StmtDoWhile:
			{
				auto n = ast::safe_cast<ast::stmt::StmtDoWhileDecl>(mNode);

				filterMap(mMap, n->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							mMap.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});						
			}
			break;

		case AstNodeType_e::StmtTry:
			{
				auto n = ast::safe_cast<ast::stmt::StmtTryDecl>(mNode);

				filterMap(mMap, n->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							mMap.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});						
			}
			break;

		case AstNodeType_e::StmtCatchBlockDecl:
			{
				auto n = ast::safe_cast<ast::stmt::StmtCatchBlockDecl>(mNode);

				filterMap(mMap, n->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							mMap.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});						
			}
			break;

		case AstNodeType_e::StmtMatchWhenDecl:
			{
				auto n = ast::safe_cast<ast::stmt::StmtMatchWhenDecl>(mNode);

				filterMap(mMap, n->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							mMap.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});						
			}
			break;

		case AstNodeType_e::FunctionDeclExpr:
			{
				auto n = ast::safe_cast<ast::expr::ExpressionFunctionDecl>(mNode);

				filterMap(mMap, n->parametersDeclList, [](NodeMultiMap& mMap, ast::ExpressionFunctionParameterDecl* const param) {
					if (param->patternDecl == nullptr)
					{
						mMap.emplace(param->identifier, param);
					}
					else
					{
						filterSingle(mMap, param->patternDecl, appendPatternToScope);
					}
				});

				filterMap(mMap, n->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
					if (stmt->nodeType == AstNodeType_e::StmtVariable)
					{
						auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

						if (stmtVar->patternDecl == nullptr)
						{
							mMap.emplace(stmt->identifier, stmt);
						}
						else
						{
							filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
						}
					}
				});						
			}
			break;

		case AstNodeType_e::Block:
			if (auto n = reinterpret_cast<ast::BlockDecl*>(mNode))
			{
				switch (mScope->nodeType)
				{
				case AstNodeType_e::ClassConstructorDecl:
					{
						auto s = ast::safe_cast<ast::ClassConstructorDecl>(mScope);

						filterMap(mMap, s->parameterList, [](NodeMultiMap& mMap, ast::FunctionParameterDecl* const param) {
							if (param->patternDecl == nullptr)
							{
								mMap.emplace(param->identifier, param);
							}
							else
							{
								filterSingle(mMap, param->patternDecl, appendPatternToScope);
							}
						});

						filterMap(mMap, s->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									mMap.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});
					}
					break;

				case AstNodeType_e::ClassFunctionDecl:
					{
						auto s = ast::safe_cast<ast::ClassFunctionDecl>(mScope);

						if (s->genericDecl) {
							appendMap(mMap, s->genericDecl->genericDeclItemList);
						}

						filterMap(mMap, s->parameterList, [](NodeMultiMap& mMap, ast::FunctionParameterDecl* const param) {
							if (param->patternDecl == nullptr)
							{
								mMap.emplace(param->identifier, param);
							}
							else
							{
								filterSingle(mMap, param->patternDecl, appendPatternToScope);
							}
						});

						filterMap(mMap, s->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									mMap.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});
					}
					break;

				case AstNodeType_e::ClassDestructorDecl:
					{
						auto s = ast::safe_cast<ast::ClassDestructorDecl>(mScope);

						filterMap(mMap, s->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									mMap.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});
					}
					break;

				case AstNodeType_e::TraitFunctionDecl:
					{
						auto s = ast::safe_cast<ast::TraitFunctionDecl>(mScope);

						if (s->genericDecl) {
							appendMap(mMap, s->genericDecl->genericDeclItemList);
						}

						filterMap(mMap, s->parameterList, [](NodeMultiMap& mMap, ast::FunctionParameterDecl* const param) {
							if (param->patternDecl == nullptr)
							{
								mMap.emplace(param->identifier, param);
							}
							else
							{
								filterSingle(mMap, param->patternDecl, appendPatternToScope);
							}
						});

						filterMap(mMap, s->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									mMap.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});
					}
					break;

				case AstNodeType_e::FunctionDecl:
					{
						auto s = ast::safe_cast<ast::FunctionDecl>(mScope);

						if (s->genericDecl) {
							appendMap(mMap, s->genericDecl->genericDeclItemList);
						}

						filterMap(mMap, s->parameterList, [](NodeMultiMap& mMap, ast::FunctionParameterDecl* const param) {
							if (param->patternDecl == nullptr)
							{
								mMap.emplace(param->identifier, param);
							}
							else
							{
								filterSingle(mMap, param->patternDecl, appendPatternToScope);
							}
						});

						filterMap(mMap, s->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									mMap.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});
					}
					break;

				case AstNodeType_e::StmtIf:
					{
						auto s = ast::safe_cast<ast::stmt::StmtIfDecl>(mScope);

						if (n == s->ifBlockDecl.get())
						{
							filterMap(mMap, s->ifBlockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
								if (stmt->nodeType == AstNodeType_e::StmtVariable)
								{
									auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

									if (stmtVar->patternDecl == nullptr)
									{
										mMap.emplace(stmt->identifier, stmt);
									}
									else
									{
										filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
									}
								}
							});
						}
						else
						{
							filterMap(mMap, s->elseBlockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
								if (stmt->nodeType == AstNodeType_e::StmtVariable)
								{
									auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

									if (stmtVar->patternDecl == nullptr)
									{
										mMap.emplace(stmt->identifier, stmt);
									}
									else
									{
										filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
									}
								}
							});
						}						
					}
					break;

				case AstNodeType_e::StmtIfLet:
					{
						auto s = ast::safe_cast<ast::stmt::StmtIfLetDecl>(mScope);

						if (n == s->ifBlockDecl.get())
						{
							filterMap(mMap, s->ifBlockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
								if (stmt->nodeType == AstNodeType_e::StmtVariable)
								{
									auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

									if (stmtVar->patternDecl == nullptr)
									{
										mMap.emplace(stmt->identifier, stmt);
									}
									else
									{
										filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
									}
								}
							});
						}
						else
						{
							filterMap(mMap, s->elseBlockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
								if (stmt->nodeType == AstNodeType_e::StmtVariable)
								{
									auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

									if (stmtVar->patternDecl == nullptr)
									{
										mMap.emplace(stmt->identifier, stmt);
									}
									else
									{
										filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
									}
								}
							});
						}						
					}
					break;

				case AstNodeType_e::StmtFor:
					{
						auto s = ast::safe_cast<ast::stmt::StmtForDecl>(mScope);

						appendSingle(mMap, s->initStmtDecl);

						filterMap(mMap, s->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									mMap.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});						
					}
					break;

				case AstNodeType_e::StmtWhile:
					{
						auto s = ast::safe_cast<ast::stmt::StmtWhileDecl>(mScope);

						filterMap(mMap, s->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									mMap.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});						
					}
					break;

				case AstNodeType_e::StmtDoWhile:
					{
						auto s = ast::safe_cast<ast::stmt::StmtDoWhileDecl>(mScope);

						filterMap(mMap, s->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									mMap.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});						
					}
					break;

				case AstNodeType_e::StmtTry:
					{
						auto s = ast::safe_cast<ast::stmt::StmtTryDecl>(mScope);

						filterMap(mMap, s->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									mMap.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});						
					}
					break;

				case AstNodeType_e::StmtCatchBlockDecl:
					{
						auto s = ast::safe_cast<ast::stmt::StmtCatchBlockDecl>(mScope);

						filterMap(mMap, s->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									mMap.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});						
					}
					break;

				case AstNodeType_e::StmtMatchWhenDecl:
					{
						auto s = ast::safe_cast<ast::stmt::StmtMatchWhenDecl>(mScope);

						filterMap(mMap, s->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									mMap.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
								}
							}
						});						
					}
					break;

				case AstNodeType_e::FunctionDeclExpr:
					{
						auto s = ast::safe_cast<ast::expr::ExpressionFunctionDecl>(mScope);

						filterMap(mMap, s->parametersDeclList, [](NodeMultiMap& mMap, ast::ExpressionFunctionParameterDecl* const param) {
							if (param->patternDecl == nullptr)
							{
								mMap.emplace(param->identifier, param);
							}
							else
							{
								filterSingle(mMap, param->patternDecl, appendPatternToScope);
							}
						});

						filterMap(mMap, s->blockDecl->stmtList, [](NodeMultiMap& mMap, ast::AstNode* const stmt) {
							if (stmt->nodeType == AstNodeType_e::StmtVariable)
							{
								auto stmtVar = ast::safe_cast<ast::stmt::StmtVariableDecl>(stmt);

								if (stmtVar->patternDecl == nullptr)
								{
									mMap.emplace(stmt->identifier, stmt);
								}
								else
								{
									filterSingle(mMap, stmtVar->patternDecl, appendPatternToScope);
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
		mFetched = true;
	}

	void
	Scope::assign(ast::AstNode* const scope, ast::AstNode* const node)
	{
		mFetched = false;
		mNode = node;
		mScope = scope;
		mMap.clear();
	}
} }