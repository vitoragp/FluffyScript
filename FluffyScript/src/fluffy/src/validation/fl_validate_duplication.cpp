#include <typeinfo>
#include <tuple>
#include "ast\fl_ast.h"
#include "ast\fl_ast_decl.h"
#include "utils\fl_ast_utils.h"
#include "validation\fl_validate_duplication.h"
#include "fl_exceptions.h"
namespace fluffy { namespace validations {
	/**
	 * DuplicatedValidation
	 */

	DuplicatedValidation::DuplicatedValidation()
	{}

	DuplicatedValidation::~DuplicatedValidation()
	{}

	void
	DuplicatedValidation::beginValidation(ast::AstNode* const decl)
	{
		switch (decl->nodeType)
		{
		case AstNodeType_e::CodeUnit:
			if (auto n = reinterpret_cast<ast::CodeUnit*>(decl))
			{
				m_scopeList.push_back(ScopeHashMap());
				m_sourceFile = n->name;
			}
			break;

		case AstNodeType_e::IncludeItemDecl:
		case AstNodeType_e::ClassVariableDecl:
		case AstNodeType_e::VariableDecl:
		case AstNodeType_e::StmtVariable:
		case AstNodeType_e::GenericItemDecl:
			if (auto n = reinterpret_cast<ast::AstNode*>(decl))
			{
				checkDuplicationAndPutInScope(n);
			}
			break;

		case AstNodeType_e::NamespaceDecl:
		case AstNodeType_e::ClassDecl:
		case AstNodeType_e::ClassConstructorDecl:
		case AstNodeType_e::ClassDestructorDecl:
		case AstNodeType_e::ClassFunctionDecl:
		case AstNodeType_e::InterfaceDecl:
		case AstNodeType_e::InterfaceFunctionDecl:
		case AstNodeType_e::TraitDecl:
		case AstNodeType_e::TraitFunctionDecl:
		case AstNodeType_e::EnumDecl:
		case AstNodeType_e::FunctionDecl:
			if (auto n = reinterpret_cast<ast::AstNode*>(decl))
			{
				checkDuplicationAndPutInScope(n);

				// Cria novo escopo.
				m_scopeList.push_back(ScopeHashMap());
			}
			break;

		case AstNodeType_e::FunctionParameterDecl:
			if (auto n = reinterpret_cast<ast::FunctionParameterDecl*>(decl))
			{
				if (n->patternDecl == nullptr)
				{
					checkDuplicationAndPutInScope(n);
				}
			}
			break;

		case AstNodeType_e::FunctionParameterDeclExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionFunctionParameterDecl*>(decl))
			{
				if (n->patternDecl == nullptr)
				{
					checkDuplicationAndPutInScope(n);
				}
			}
			break;

		case AstNodeType_e::LiteralPattern:
			if (auto n = reinterpret_cast<ast::pattern::LiteralPatternDecl*>(decl))
			{
				if (n->literalExpr->nodeType == AstNodeType_e::IdentifierExpr)
				{
					if (auto id = reinterpret_cast<ast::expr::ExpressionConstantIdentifierDecl*>(n->literalExpr.get()))
					{
						checkDuplicationAndPutInScope(id);
					}
				}
			}
			break;

		case AstNodeType_e::StructureItemPatternDecl:
			if (auto n = reinterpret_cast<ast::pattern::StructureItemPatternDecl*>(decl))
			{
				if (n->referencedPattern == nullptr)
				{
					checkDuplicationAndPutInScope(n);
				}
			}
			break;

		case AstNodeType_e::StmtForInitDecl:
			if (auto n = reinterpret_cast<ast::stmt::StmtForInitDecl*>(decl))
			{
				checkDuplicationAndPutInScope(n);
			}
			break;

		case AstNodeType_e::NewItemExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewItemDecl*>(decl))
			{
				checkDuplicationAndPutInScope(n);
			}
			break;

		case AstNodeType_e::GenericDecl:
		case AstNodeType_e::StmtIfLet:
		case AstNodeType_e::StmtFor:
		case AstNodeType_e::StmtWhile:
		case AstNodeType_e::StmtDoWhile:
		case AstNodeType_e::StmtTry:
		case AstNodeType_e::StmtMatchWhenDecl:
		case AstNodeType_e::StmtCatchBlockDecl:
		case AstNodeType_e::FunctionDeclExpr:
		case AstNodeType_e::MatchWhenExpr:
		case AstNodeType_e::NewBlockExpr:
			{
				// Cria novo escopo.
				m_scopeList.push_back(ScopeHashMap());
			}
			break;
		default:
			break;
		}
	}

	void
	DuplicatedValidation::endValidation(ast::AstNode* const decl)
	{
		switch (decl->nodeType)
		{
		case AstNodeType_e::CodeUnit:
		case AstNodeType_e::NamespaceDecl:
		case AstNodeType_e::ClassDecl:
		case AstNodeType_e::ClassConstructorDecl:
		case AstNodeType_e::ClassDestructorDecl:
		case AstNodeType_e::ClassFunctionDecl:
		case AstNodeType_e::InterfaceDecl:
		case AstNodeType_e::InterfaceFunctionDecl:
		case AstNodeType_e::TraitDecl:
		case AstNodeType_e::TraitFunctionDecl:
		case AstNodeType_e::EnumDecl:
		case AstNodeType_e::FunctionDecl:
		case AstNodeType_e::GenericDecl:
		case AstNodeType_e::StmtIfLet:
		case AstNodeType_e::StmtFor:
		case AstNodeType_e::StmtWhile:
		case AstNodeType_e::StmtDoWhile:
		case AstNodeType_e::StmtTry:
		case AstNodeType_e::StmtMatchWhenDecl:
		case AstNodeType_e::StmtCatchBlockDecl:
		case AstNodeType_e::FunctionDeclExpr:
		case AstNodeType_e::MatchWhenExpr:
		case AstNodeType_e::NewBlockExpr:
			m_scopeList.pop_back();
			break;
		default:
			break;
		}
	}

	void
	DuplicatedValidation::checkDuplicationAndPutInScope(ast::AstNode* const node)
	{
		// Pega o escopo atual.
		auto& scope = m_scopeList.back();

		// Verifica se o identificador foi descartado.
		if (node->identifier == "_")
		{
			return;
		}

		// Verifica se ja existe um item com o mesmo identificador.
		auto it = scope.find(node->identifier.str());
		while (it != scope.end())
		{
			if (it != scope.end() && utils::AstUtils::equals(node, it->second))
			{
				throw exceptions::custom_exception(
					"%s: error duplicated identifier '%s' at: line %d, column %d",
					m_sourceFile.c_str(),
					node->identifier.str(),
					node->line,
					node->column
				);
			}
			it++;
		}

		// Inclui o item no escopo.
		scope.insert(ScopedPair(node->identifier.str(), node));
	}
} }