#include <functional>
#include "ast\fl_ast.h"
#include "ast\fl_ast_decl.h"
#include "validation\fl_validate.h"
namespace fluffy { namespace validations {
	/**
	 * Validation
	 */

	Validation::Validation()
	{}

	Validation::~Validation()
	{}

	void
	Validation::validate(ast::AstNode* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}

		// Faz a validação no nodo.
		beginValidation(decl);

		// Faz a validação nos filhos.
		switch (decl->nodeType)
		{
		case AstNodeType_e::CodeUnit:
			if (auto n = reinterpret_cast<ast::CodeUnit*>(decl))
			{
				validate_each(n->includeDeclList);
				validate_each(n->namespaceDeclList);
			}
			break;

		case AstNodeType_e::Include:
			if (auto n = reinterpret_cast<ast::IncludeDecl*>(decl))
			{
				validate_single(n->fromNamespace);
			}
			break;

		case AstNodeType_e::Namespace:
			if (auto n = reinterpret_cast<ast::NamespaceDecl*>(decl))
			{
				validate_each(n->namespaceDeclList);
				validate_each(n->generalDeclList);
			}
			break;

		case AstNodeType_e::Class:
			if (auto n = reinterpret_cast<ast::ClassDecl*>(decl))
			{
				validate_single(n->genericDecl);

				validate_single(n->baseClass);
				validate_each(n->interfaceList);

				validate_each(n->constructorList);
				validate_each(n->functionList);
				validate_each(n->variableList);

				validate_single(n->destructorDecl);
			}
			break;

		case AstNodeType_e::ClassConstructor:
			if (auto n = reinterpret_cast<ast::ClassConstructorDecl*>(decl))
			{
				validate_each(n->parameterList);
				validate_single(n->superInitExpr);
				validate_each(n->variableInitDeclList);
				validate_single(n->blockDecl);
			}
			break;

		case AstNodeType_e::ClassDestructor:
			if (auto n = reinterpret_cast<ast::ClassDestructorDecl*>(decl))
			{
				validate_single(n->blockDecl);
			}
			break;

		case AstNodeType_e::ClassFunction:
			if (auto n = reinterpret_cast<ast::ClassFunctionDecl*>(decl))
			{
				validate_single(n->genericDecl);
				validate_single(n->sourceTypeDecl);
				validate_each(n->parameterList);
				validate_single(n->returnType);
				validate_single(n->blockDecl);
				validate_single(n->exprDecl);
			}
			break;

		case AstNodeType_e::ClassVariable:
			if (auto n = reinterpret_cast<ast::ClassVariableDecl*>(decl))
			{
				validate_single(n->typeDecl);
				validate_single(n->initExpression);
			}
			break;

		case AstNodeType_e::ClassVariableInitDecl:
			if (auto n = reinterpret_cast<ast::ClassVariableInitDecl*>(decl))
			{
				validate_single(n->initExpression);
			}
			break;

		case AstNodeType_e::Interface:
			if (auto n = reinterpret_cast<ast::InterfaceDecl*>(decl))
			{
				validate_single(n->genericDecl);
				validate_each(n->functionDeclList);
			}
			break;

		case AstNodeType_e::InterfaceFunction:
			if (auto n = reinterpret_cast<ast::InterfaceFunctionDecl*>(decl))
			{
				validate_single(n->genericDecl);
				validate_each(n->parameterList);
				validate_single(n->returnType);
			}
			break;

		case AstNodeType_e::Struct:
			if (auto n = reinterpret_cast<ast::StructDecl*>(decl))
			{
				validate_single(n->genericDecl);
				validate_each(n->variableList);
			}
			break;

		case AstNodeType_e::StructVariable:
			if (auto n = reinterpret_cast<ast::StructVariableDecl*>(decl))
			{
				validate_single(n->typeDecl);
				validate_single(n->initExpression);
			}
			break;

		case AstNodeType_e::Enum:
			if (auto n = reinterpret_cast<ast::EnumDecl*>(decl))
			{
				validate_single(n->genericDecl);
				validate_each(n->enumItemDeclList);
			}
			break;

		case AstNodeType_e::EnumItem:
			if (auto n = reinterpret_cast<ast::EnumItemDecl*>(decl))
			{
				validate_each(n->dataTypeDeclList);
				validate_single(n->valueExpression);
			}
			break;

		case AstNodeType_e::Trait:
			if (auto n = reinterpret_cast<ast::TraitDecl*>(decl))
			{
				validate_single(n->typeDefinitionDecl);
				validate_single(n->genericDecl);
				validate_each(n->functionDeclList);
			}
			break;

		case AstNodeType_e::TraitFunction:
			if (auto n = reinterpret_cast<ast::TraitFunctionDecl*>(decl))
			{
				validate_single(n->genericDecl);
				validate_each(n->parameterList);
				validate_single(n->returnType);
				validate_single(n->blockDecl);
				validate_single(n->exprDecl);
			}
			break;

		case AstNodeType_e::Function:
			if (auto n = reinterpret_cast<ast::FunctionDecl*>(decl))
			{
				validate_single(n->genericDecl);
				validate_each(n->parameterList);
				validate_single(n->returnType);
				validate_single(n->blockDecl);
				validate_single(n->exprDecl);
			}
			break;

		case AstNodeType_e::Variable:
			if (auto n = reinterpret_cast<ast::VariableDecl*>(decl))
			{
				validate_single(n->typeDecl);
				validate_single(n->initExpression);
			}
			break;

		case AstNodeType_e::FunctionParameter:
			if (auto n = reinterpret_cast<ast::FunctionParameterDecl*>(decl))
			{
				validate_single(n->typeDecl);
				validate_single(n->patternDecl);
			}
			break;

		case AstNodeType_e::Generic:
			if (auto n = reinterpret_cast<ast::GenericDecl*>(decl))
			{
				validate_each(n->genericDeclItemList);
			}
			break;

		case AstNodeType_e::GenericItem:
			if (auto n = reinterpret_cast<ast::GenericItemDecl*>(decl))
			{
				validate_each(n->whereTypeList);
			}
			break;

		case AstNodeType_e::ScopedIdentifier:
			if (auto n = reinterpret_cast<ast::ScopedIdentifierDecl*>(decl))
			{
				validate_single(n->referencedIdentifier);
			}
			break;

		case AstNodeType_e::Block:
			if (auto n = reinterpret_cast<ast::BlockDecl*>(decl))
			{
				validate_each(n->stmtList);
			}
			break;

		case AstNodeType_e::IfStmt:
			if (auto n = reinterpret_cast<ast::stmt::StmtIfDecl*>(decl))
			{
				validate_single(n->conditionExprDecl);
				validate_single(n->ifBlockDecl);
				validate_single(n->elseBlockDecl);
			}
			break;

		case AstNodeType_e::IfLetStmt:
			if (auto n = reinterpret_cast<ast::stmt::StmtIfLetDecl*>(decl))
			{
				validate_single(n->patternDecl);
				validate_single(n->expressionDecl);
				validate_single(n->ifBlockDecl);
				validate_single(n->elseBlockDecl);
			}
			break;

		case AstNodeType_e::ForStmt:
			if (auto n = reinterpret_cast<ast::stmt::StmtForDecl*>(decl))
			{
				validate_single(n->initStmtDecl);
				validate_single(n->initExprDecl);
				validate_single(n->conditionExprDecl);
				validate_single(n->updateExprDecl);
				validate_single(n->blockDecl);
			}
			break;

		case AstNodeType_e::WhileStmt:
			if (auto n = reinterpret_cast<ast::stmt::StmtWhileDecl*>(decl))
			{
				validate_single(n->conditionExprDecl);
				validate_single(n->blockDecl);
			}
			break;

		case AstNodeType_e::DoWhileStmt:
			if (auto n = reinterpret_cast<ast::stmt::StmtDoWhileDecl*>(decl))
			{
				validate_single(n->conditionExprDecl);
				validate_single(n->blockDecl);
			}
			break;

		case AstNodeType_e::MatchStmt:
			if (auto n = reinterpret_cast<ast::stmt::StmtMatchDecl*>(decl))
			{
				validate_single(n->conditionExprDecl);
				validate_each(n->whenDeclList);
			}
			break;

		case AstNodeType_e::ReturnStmt:
			if (auto n = reinterpret_cast<ast::stmt::StmtReturnDecl*>(decl))
			{
				validate_single(n->exprDecl);
			}
			break;

		case AstNodeType_e::TryStmt:
			if (auto n = reinterpret_cast<ast::stmt::StmtTryDecl*>(decl))
			{
				validate_single(n->blockDecl);
				validate_each(n->catchDeclList);
			}
			break;

		case AstNodeType_e::PanicStmt:
			if (auto n = reinterpret_cast<ast::stmt::StmtPanicDecl*>(decl))
			{
				validate_single(n->exprDecl);
			}
			break;

		case AstNodeType_e::VariableStmt:
			if (auto n = reinterpret_cast<ast::stmt::StmtVariableDecl*>(decl))
			{
				validate_single(n->typeDecl);
				validate_single(n->initExpression);
				validate_single(n->patternDecl);
			}
			break;

		case AstNodeType_e::ExprStmt:
			if (auto n = reinterpret_cast<ast::stmt::StmtExprDecl*>(decl))
			{
				validate_single(n->exprDecl);
			}
			break;

		case AstNodeType_e::ForInitDecl:
			if (auto n = reinterpret_cast<ast::stmt::StmtForInitDecl*>(decl))
			{
				validate_single(n->typeDecl);
				validate_single(n->initExpression);
			}
			break;

		case AstNodeType_e::MatchWhenStmtDecl:
			if (auto n = reinterpret_cast<ast::stmt::MatchWhenStmtDecl*>(decl))
			{
				validate_single(n->patternDecl);
				validate_single(n->blockDecl);
			}
			break;

		case AstNodeType_e::CatchBlockStmtDecl:
			if (auto n = reinterpret_cast<ast::stmt::StmtCatchBlockDecl*>(decl))
			{
				validate_single(n->typeDecl);
				validate_single(n->patternDecl);
				validate_single(n->blockDecl);
			}
			break;

		case AstNodeType_e::TernaryExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionTernaryDecl*>(decl))
			{
				validate_single(n->conditionDecl);
				validate_single(n->leftDecl);
				validate_single(n->rightDecl);
			}
			break;

		case AstNodeType_e::BinaryExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionBinaryDecl*>(decl))
			{
				validate_single(n->leftDecl);
				validate_single(n->rightDecl);
			}
			break;

		case AstNodeType_e::UnaryExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionUnaryDecl*>(decl))
			{
				validate_single(n->exprDecl);
			}
			break;

		case AstNodeType_e::AsExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionAsDecl*>(decl))
			{
				validate_single(n->exprDecl);
				validate_single(n->typeDecl);
			}
			break;

		case AstNodeType_e::IsExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionIsDecl*>(decl))
			{
				validate_single(n->exprDecl);
				validate_single(n->typeDecl);
			}
			break;

		case AstNodeType_e::MatchExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionMatchDecl*>(decl))
			{
				validate_single(n->exprDecl);
				validate_each(n->whenDeclList);
			}
			break;

		case AstNodeType_e::FunctionDeclExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionFunctionDecl*>(decl))
			{
				validate_single(n->returnTypeDecl);
				validate_each(n->parametersDeclList);
				validate_single(n->blockDecl);
				validate_single(n->exprDecl);
			}
			break;

		case AstNodeType_e::FunctionCallExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionFunctionCall*>(decl))
			{
				validate_single(n->lhsDecl);
				validate_single(n->rhsDecl);
			}
			break;

		case AstNodeType_e::GenericCallExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionGenericCallDecl*>(decl))
			{
				validate_each(n->genericTypeList);
				validate_single(n->lhsDecl);
				validate_single(n->rhsDecl);
			}
			break;

		case AstNodeType_e::IndexExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionIndexDecl*>(decl))
			{
				validate_single(n->lhsDecl);
				validate_single(n->rhsDecl);
			}
			break;

		case AstNodeType_e::NewExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewDecl*>(decl))
			{
				validate_single(n->objTypeDecl);
				validate_single(n->exprDecl);
				validate_single(n->objInitBlockDecl);
			}
			break;

		case AstNodeType_e::AnomClassDeclExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionAnomClassDecl*>(decl))
			{
				validate_single(n->baseReferencedDecl);
				validate_each(n->functionList);
				validate_each(n->variableList);
				validate_each(n->constructorList);
				validate_single(n->destructorDecl);
			}
			break;

		case AstNodeType_e::PrimitiveTypeExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionPrimitiveTypeDecl*>(decl))
			{
				validate_single(n->typeDecl);
			}
			break;

		case AstNodeType_e::ArrayInitExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionArrayInitDecl*>(decl))
			{
				validate_each(n->arrayElementDeclList);
			}
			break;

		case AstNodeType_e::MatchWhenExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionMatchWhenDecl*>(decl))
			{
				validate_single(n->patternDecl);
				validate_single(n->exprDecl);
			}
			break;

		case AstNodeType_e::ExprFunctionParameterDecl:
			if (auto n = reinterpret_cast<ast::expr::ExpressionFunctionParameterDecl*>(decl))
			{
				validate_single(n->typeDecl);
				validate_single(n->patternDecl);
			}
			break;

		case AstNodeType_e::NewBlockExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewBlockDecl*>(decl))
			{
				validate_each(n->itemDeclList);
			}
			break;

		case AstNodeType_e::NewItemExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewItemDecl*>(decl))
			{
				validate_single(n->exprDecl);
			}
			break;

		case AstNodeType_e::LiteralPattern:
			if (auto n = reinterpret_cast<ast::pattern::LiteralPatternDecl*>(decl))
			{
				validate_single(n->literalExpr);
			}
			break;

		case AstNodeType_e::TuplePattern:
			if (auto n = reinterpret_cast<ast::pattern::TuplePatternDecl*>(decl))
			{
				validate_each(n->patternItemDeclList);
			}
			break;

		case AstNodeType_e::StructurePattern:
			if (auto n = reinterpret_cast<ast::pattern::StructurePatternDecl*>(decl))
			{
				validate_each(n->structureItemDeclList);
			}
			break;

		case AstNodeType_e::EnumerablePattern:
			if (auto n = reinterpret_cast<ast::pattern::EnumerablePatternDecl*>(decl))
			{
				validate_single(n->enumReferenced);
				validate_each(n->patternDeclItemList);
			}
			break;

		case AstNodeType_e::StructureItemPatternDecl:
			if (auto n = reinterpret_cast<ast::pattern::StructureItemPatternDecl*>(decl))
			{
				validate_single(n->referencedPattern);
			}
			break;

		case AstNodeType_e::ArrayType:
			if (auto n = reinterpret_cast<ast::TypeDeclArray*>(decl))
			{
				validate_each(n->arrayDeclList);
				validate_single(n->valueType);
			}
			break;

		case AstNodeType_e::FunctionType:
			if (auto n = reinterpret_cast<ast::TypeDeclFunction*>(decl))
			{
				validate_each(n->parameterTypeList);
				validate_single(n->returnType);
			}
			break;

		case AstNodeType_e::TupleType:
			if (auto n = reinterpret_cast<ast::TypeDeclTuple*>(decl))
			{
				validate_each(n->tupleItemList);
			}
			break;

		case AstNodeType_e::NamedType:
			if (auto n = reinterpret_cast<ast::TypeDeclNamed*>(decl))
			{
				validate_single(n->internalIdentifier);
				validate_each(n->genericDefinitionList);
			}
			break;

		default:
			break;
		}

		// Faz a validação no nodo.
		endValidation(decl);
	}
} }