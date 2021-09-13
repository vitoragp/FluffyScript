#pragma once
#include <memory>
#include "fl_defs.h"
#include "ast\fl_ast_decl.h"
#include "ast\fl_ast_type.h"
#include "ast\fl_ast_block.h"
#include "ast\fl_ast_expr.h"
#include "ast\fl_ast_pattern.h"

namespace fluffy {
	class BufferBase;
}

namespace fluffy { namespace lexer {
	class Lexer;
} }

namespace fluffy { namespace parser {
	struct ParserContext_s
	{
		Bool insideTrait;
		Bool insideExpr;
	};

	///
	/// Parser
	///
	
	class Parser
	{
	public:
		Parser(BufferBase* const buffer);
		virtual ~Parser();

		void
		loadSource(const I8* sourceCode);

		void
		loadSource(const I8* sourceFilename, const I8* sourceCode);

		void
		loadSourceFromFile(const I8* sourceFilename);

		Bool
		finished();

		void
		skipToken();

		std::unique_ptr<ast::CodeUnit>
		parseCodeUnit(ParserContext_s& ctx);

		std::unique_ptr<ast::IncludeDecl>
		parseInclude(ParserContext_s& ctx);

		std::unique_ptr<ast::NamespaceDecl>
		parseNamespace(ParserContext_s& ctx);

		std::unique_ptr<ast::ClassDecl>
		parseClass(ParserContext_s& ctx, Bool hasExport);

		std::unique_ptr<ast::InterfaceDecl>
		parseInterface(ParserContext_s& ctx, Bool hasExport);

		std::unique_ptr<ast::StructDecl>
		parseStruct(ParserContext_s& ctx, Bool hasExport);

		std::unique_ptr<ast::GeneralStmtDecl>
		parseTrait(ParserContext_s& ctx, Bool hasExport);

		std::unique_ptr<ast::EnumDecl>
		parseEnum(ParserContext_s& ctx, Bool hasExport);

		std::unique_ptr<ast::FunctionDecl>
		parseFunction(ParserContext_s& ctx, Bool hasExport);

		std::unique_ptr<ast::VariableDecl>
		parseVariable(ParserContext_s& ctx, Bool hasExport);

		std::unique_ptr<ast::GenericDecl>
		parseGenericDecl(ParserContext_s& ctx);

		/// 
		/// Types
		/// 

		std::unique_ptr<ast::TypeDecl>
		parseType(ParserContext_s& ctx);

		/// 
		/// Block
		/// 
		
		std::unique_ptr<ast::BlockDecl>
		parseBlock(ParserContext_s& ctx);

		/// 
		/// Stmt
		/// 
		
		std::unique_ptr<ast::stmt::StmtDecl>
		parseStmtDecl(ParserContext_s& ctx);

		///
		/// Expression 
		///
		
		std::unique_ptr<ast::expr::ExpressionDecl>
		parseExpression(ParserContext_s& ctx, U32 prec);

		///
		/// Pattern
		/// 

		std::unique_ptr<ast::pattern::PatternDecl>
		parsePattern(ParserContext_s& ctx);

		///
		/// Misc 
		///
		
		std::vector<std::unique_ptr<ast::FunctionParameterDecl>>
		parseFunctionParameters(ParserContext_s& ctx);

		std::unique_ptr<ast::ScopedPathDecl>
		parseScopedPath(ParserContext_s& ctx);

	private:
		std::unique_ptr<ast::GeneralStmtDecl>
		parseGeneralStmt(ParserContext_s& ctx);

		std::unique_ptr<ast::ClassFunctionDecl>
		parseClassFunction(ParserContext_s& ctx, TokenType_e accessModifier, Bool staticModifier);

		std::unique_ptr<ast::ClassVariableDecl>
		parseClassVariable(ParserContext_s& ctx, TokenType_e accessModifier, Bool staticModifier);

		std::unique_ptr<ast::ClassConstructorDecl>
		parseClassConstructor(ParserContext_s& ctx, TokenType_e accessModifier);

		std::unique_ptr<ast::ClassDestructorDecl>
		parseClassDestructor(ParserContext_s& ctx);

		std::unique_ptr<ast::InterfaceFunctionDecl>
		parserInterfaceFunction(ParserContext_s& ctx);

		std::unique_ptr<ast::StructVariableDecl>
		parseStructVariable(ParserContext_s& ctx);

		std::unique_ptr<ast::TraitFunctionDecl>
		parseTraitFunction(ParserContext_s& ctx, Bool isDefinition, Bool isStatic);

		std::unique_ptr<ast::EnumItemDecl>
		parseEnumItem(ParserContext_s& ctx);

		std::unique_ptr<ast::stmt::StmtDecl>
		parseIf(ParserContext_s& ctx);

		std::unique_ptr<ast::stmt::StmtDecl>
		parseIfLet(ParserContext_s& ctx);

		std::unique_ptr<ast::stmt::StmtDecl>
		parseFor(ParserContext_s& ctx);

		std::unique_ptr<ast::stmt::StmtDecl>
		parseWhile(ParserContext_s& ctx);

		std::unique_ptr<ast::stmt::StmtDecl>
		parseDoWhile(ParserContext_s& ctx);

		std::unique_ptr<ast::stmt::StmtDecl>
		parseMatch(ParserContext_s& ctx);

		std::unique_ptr<ast::stmt::StmtDecl>
		parseReturn(ParserContext_s& ctx);

		std::unique_ptr<ast::stmt::StmtDecl>
		parseContinue(ParserContext_s& ctx);

		std::unique_ptr<ast::stmt::StmtDecl>
		parseBreak(ParserContext_s& ctx);

		std::unique_ptr<ast::stmt::StmtDecl>
		parseGoto(ParserContext_s& ctx);

		std::unique_ptr<ast::stmt::StmtDecl>
		parseTry(ParserContext_s& ctx);

		std::unique_ptr<ast::stmt::StmtDecl>
		parsePanic(ParserContext_s& ctx);

		std::unique_ptr<ast::stmt::StmtDecl>
		parseVariable(ParserContext_s& ctx);

		std::unique_ptr<ast::stmt::StmtDecl>
		parseLabel(ParserContext_s& ctx);

		std::unique_ptr<ast::stmt::StmtDecl>
		parseExprStmt(ParserContext_s& ctx);

		std::unique_ptr<ast::expr::ExpressionDecl>
		parseExpressionImp(ParserContext_s& ctx, U32 prec, Bool* requiredGenericValidation);

		std::unique_ptr<ast::expr::ExpressionDecl>
		parseAtom(ParserContext_s& ctx, Bool* requiredGenericValidation);

		std::unique_ptr<ast::pattern::PatternDecl>
		parseLiteralPattern(ParserContext_s& ctx);

		std::unique_ptr<ast::pattern::PatternDecl>
		parseTuplePattern(ParserContext_s& ctx);

		std::unique_ptr<ast::pattern::PatternDecl>
		parseStructurePattern(ParserContext_s& ctx);

		std::unique_ptr<ast::pattern::PatternDecl>
		parseEnumerablePattern(ParserContext_s& ctx);

		std::unique_ptr<ast::TypeDecl>
		parseFunctionType(ParserContext_s& ctx);

		std::unique_ptr<ast::TypeDecl>
		parseTupleType(ParserContext_s& ctx);

		std::unique_ptr<ast::TypeDecl>
		parseNamedType(ParserContext_s& ctx);

		std::unique_ptr<ast::TypeDeclNamed>
		parseInternalNamedType(ParserContext_s& ctx);

		std::unique_ptr<ast::ArrayDecl>
		parseArrayDecl(ParserContext_s& ctx);

		std::unique_ptr<ast::ScopedPathDecl>
		parseChildScopedPaths(ParserContext_s& ctx);

		void
		validateIdentifier(TString& id);

	private:
		std::unique_ptr<lexer::Lexer>
		m_lexer;

		String
		m_filename;
	};
} }