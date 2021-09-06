#pragma once
#include "fl_defs.h"

namespace fluffy { namespace ast {
	class CodeUnit;
	class NamespaceDecl;
} }

namespace fluffy { namespace parser {
	class Parser;
} }

namespace fluffy { namespace validations {
	/**
	 * DuplicatedValidation
	 */

	class DuplicatedValidation
	{
	public:
		DuplicatedValidation();
		~DuplicatedValidation();

		void
		validate(ast::CodeUnit* const codeUnit);

	private:
		void
		validateNamespace(const I8* sourceFilename, ast::NamespaceDecl* const decl);

		void
		validateClass(const I8* sourceFilename, ast::ClassDecl* const decl);

		void
		validateClassFunction(const I8* sourceFilename, ast::ClassFunctionDecl* const decl);

		void
		validateClassConstructor(const I8* sourceFilename, ast::ClassConstructorDecl* const decl);

		void
		validateInterface(const I8* sourceFilename, ast::InterfaceDecl* const decl);

		void
		validateInterfaceFunction(const I8* sourceFilename, ast::InterfaceFunctionDecl* const decl);

		void
		validateStruct(const I8* sourceFilename, ast::StructDecl* const decl);

		void
		validateTrait(const I8* sourceFilename, ast::TraitDecl* const decl);

		void
		validateTraitFunction(const I8* sourceFilename, ast::TraitFunctionDecl* const decl);

		void
		validateEnum(const I8* sourceFilename, ast::EnumDecl* const decl);

		void
		validateFunction(const I8* sourceFilename, ast::FunctionDecl* const decl);

		void
		validateIfStmt(const I8* sourceFilename, ast::stmt::StmtIfDecl* const decl);

		void
		validateIfLetStmt(const I8* sourceFilename, ast::stmt::StmtIfLetDecl* const decl);

		void
		validateForStmt(const I8* sourceFilename, ast::stmt::StmtForDecl* const decl);

		void
		validateWhileStmt(const I8* sourceFilename, ast::stmt::StmtWhileDecl* const decl);

		void
		validateDoWhileStmt(const I8* sourceFilename, ast::stmt::StmtDoWhileDecl* const decl);

		void
		validateTryStmt(const I8* sourceFilename, ast::stmt::StmtTryDecl* const decl);

		void
		validateMatchStmt(const I8* sourceFilename, ast::stmt::StmtMatchDecl* const decl);

		void
		validateFunctionDeclExpr(const I8* sourceFilename, ast::expr::ExpressionFunctionDecl* const decl);

		void
		validateAnomClassDeclExpr(const I8* sourceFilename, ast::expr::ExpressionAnomClassDecl* const decl);

		std::vector<std::tuple<TString, U32, U32>>
		validateBlockAndExtractIdentifier(const I8* sourceFilename, ast::BlockDecl* const decl);
	};
} }