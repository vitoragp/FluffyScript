#include <memory>
#include "gtest/gtest.h"

#include "fl_ast_expr.h"
#include "fl_ast_type.h"
#include "fl_parser.h"
#include "fl_exceptions.h"
#include "parser_objects/fl_parser_objects.h"

namespace {
	using namespace fluffy::ast::expr;
	using namespace fluffy::exceptions;

	template <typename T>
	void __validateExpr(ExpressionDecl* expr, std::function<void(T*)> callback, const fluffy::I8* exprName)
	{
		if (auto exprT = reinterpret_cast<T*>(expr))
		{
			callback(exprT);
		}
		else
		{
			throw custom_exception(
				"Failed to convert expression to '%s'",
				expr->line,
				expr->column,
				exprName
			);
		}
	}

	void validateTerExpr(ExpressionDecl* expr, std::function<void(ExpressionTernaryDecl*)> callback)
	{
		__validateExpr(expr, callback, "ExpressionTernaryDecl");
	}

	void validateBinExpr(ExpressionDecl* expr, std::function<void(ExpressionBinaryDecl*)> callback)
	{
		__validateExpr(expr, callback, "ExpressionBinaryDecl");
	}

	void validateUnaExpr(ExpressionDecl* expr, std::function<void(ExpressionUnaryDecl*)> callback)
	{
		__validateExpr(expr, callback, "ExpressionUnaryDecl");
	}

	void validateIsExpr(ExpressionDecl* expr, std::function<void(ExpressionIsDecl*)> callback)
	{
		__validateExpr(expr, callback, "ExpressionIsDecl");
	}

	void validateAsExpr(ExpressionDecl* expr, std::function<void(ExpressionAsDecl*)> callback)
	{
		__validateExpr(expr, callback, "ExpressionAsDecl");
	}

	void validateMatExpr(ExpressionDecl* expr, std::function<void(ExpressionMatchDecl*)> callback)
	{
		__validateExpr(expr, callback, "ExpressionMatchDecl");
	}

	void validateFDeExpr(ExpressionDecl* expr, std::function<void(ExpressionFunctionDecl*)> callback)
	{
		__validateExpr(expr, callback, "ExpressionFunctionDecl");
	}

	void validateFCaExpr(ExpressionDecl* expr, std::function<void(ExpressionFunctionCall*)> callback)
	{
		__validateExpr(expr, callback, "ExpressionFunctionCall");
	}

	void validateIndExpr(ExpressionDecl* expr, std::function<void(ExpressionIndexAddress*)> callback)
	{
		__validateExpr(expr, callback, "ExpressionIndexAddress");
	}

	void validateThiExpr(ExpressionDecl* expr, std::function<void(ExpressionThisDecl*)> callback)
	{
		__validateExpr(expr, callback, "ExpressionThisDecl");
	}

	void validateSupExpr(ExpressionDecl* expr, std::function<void(ExpressionSuperDecl*)> callback)
	{
		__validateExpr(expr, callback, "ExpressionSuperDecl");
	}

	void validateBooExpr(ExpressionDecl* expr, std::function<void(ExpressionConstantBoolDecl*)> callback)
	{
		__validateExpr(expr, callback, "ExpressionConstantBoolDecl");
	}

	void validateIntExpr(ExpressionDecl* expr, std::function<void(ExpressionConstantIntegerDecl*)> callback)
	{
		__validateExpr(expr, callback, "ExpressionConstantIntegerDecl");
	}

	void validateReaExpr(ExpressionDecl* expr, std::function<void(ExpressionConstantRealDecl*)> callback)
	{
		__validateExpr(expr, callback, "ExpressionConstantRealDecl");
	}

	void validateStrExpr(ExpressionDecl* expr, std::function<void(ExpressionConstantStringDecl*)> callback)
	{
		__validateExpr(expr, callback, "ExpressionConstantStringDecl");
	}

	void validateChaExpr(ExpressionDecl* expr, std::function<void(ExpressionConstantCharDecl*)> callback)
	{
		__validateExpr(expr, callback, "ExpressionConstantCharDecl");
	}

	void validateNulExpr(ExpressionDecl* expr, std::function<void(ExpressionConstantNullDecl*)> callback)
	{
		__validateExpr(expr, callback, "ExpressionConstantNullDecl");
	}

	void validateIdeExpr(ExpressionDecl* expr, std::function<void(ExpressionConstantIdentifierDecl*)> callback)
	{
		__validateExpr(expr, callback, "ExpressionConstantIdentifierDecl");
	}
}

namespace fluffy { namespace testing {
	using parser::Parser;
	using lexer::Lexer;
	using namespace ast::expr;

	/**
	 * ParserExpressionTest
	 */

	struct ParserExpressionTest : public ::testing::Test
	{
		std::unique_ptr<Lexer> lexer;
		std::unique_ptr<Parser> parser;

		CompilationContext_t ctx;

		// Sets up the test fixture.
		virtual void SetUp()
		{
			parser = std::make_unique<Parser>(
				new Lexer(
					new DirectBuffer()
				)
				);

			ctx.parser = parser.get();
		}
	};

	/**
	 * Tests
	 */

	TEST_F(ParserExpressionTest, TestExpression)
	{
		ctx.parser->loadSource("2 + 2 * 3");
		ctx.parser->nextToken();

		auto exprDecl = parser_objects::ParserObjectExpressionDecl::parse(&ctx);

		// [2 + 2 * 3]
		validateBinExpr(exprDecl.get(), [](ExpressionBinaryDecl* binExpr) {
			EXPECT_EQ(binExpr->op, TokenSubType_e::Plus);
			EXPECT_EQ(binExpr->line, 1);
			EXPECT_EQ(binExpr->column, 1);

			validateIntExpr(binExpr->leftDecl.get(), [](ExpressionConstantIntegerDecl* val) {
				EXPECT_EQ(val->valueDecl, 2);
				EXPECT_EQ(val->valueType, TypeDeclID_e::I32);
				EXPECT_EQ(val->line, 1);
				EXPECT_EQ(val->column, 1);
			});

			// 2 + [2 * 3]
			validateBinExpr(binExpr->rightDecl.get(), [](ExpressionBinaryDecl* binExpr) {
				EXPECT_EQ(binExpr->op, TokenSubType_e::Multiplication);
				EXPECT_EQ(binExpr->line, 1);
				EXPECT_EQ(binExpr->column, 5);

				validateIntExpr(binExpr->leftDecl.get(), [](ExpressionConstantIntegerDecl* val) {
					EXPECT_EQ(val->valueDecl, 2);
					EXPECT_EQ(val->valueType, TypeDeclID_e::I32);
					EXPECT_EQ(val->line, 1);
					EXPECT_EQ(val->column, 5);
				});

				validateIntExpr(binExpr->rightDecl.get(), [](ExpressionConstantIntegerDecl* val) {
					EXPECT_EQ(val->valueDecl, 3);
					EXPECT_EQ(val->valueType, TypeDeclID_e::I32);
					EXPECT_EQ(val->line, 1);
					EXPECT_EQ(val->column, 9);
				});
			});
		});
	}

	TEST_F(ParserExpressionTest, TestExpressionWithParent)
	{
		ctx.parser->loadSource("(2 + 5) * 3");
		ctx.parser->nextToken();

		auto exprDecl = parser_objects::ParserObjectExpressionDecl::parse(&ctx);

		// [(2 + 5) * 3]
		validateBinExpr(exprDecl.get(), [](ExpressionBinaryDecl* binExpr) {
			EXPECT_EQ(binExpr->op, TokenSubType_e::Multiplication);
			EXPECT_EQ(binExpr->line, 1);
			EXPECT_EQ(binExpr->column, 1);

			// [(2 + 5)] * 3
			validateBinExpr(binExpr->leftDecl.get(), [](ExpressionBinaryDecl* binExpr) {
				EXPECT_EQ(binExpr->op, TokenSubType_e::Plus);
				EXPECT_EQ(binExpr->line, 1);
				EXPECT_EQ(binExpr->column, 1);

				validateIntExpr(binExpr->leftDecl.get(), [](ExpressionConstantIntegerDecl* val) {
					EXPECT_EQ(val->valueDecl, 2);
					EXPECT_EQ(val->valueType, TypeDeclID_e::I32);
					EXPECT_EQ(val->line, 1);
					EXPECT_EQ(val->column, 2);
				});

				validateIntExpr(binExpr->rightDecl.get(), [](ExpressionConstantIntegerDecl* val) {
					EXPECT_EQ(val->valueDecl, 5);
					EXPECT_EQ(val->valueType, TypeDeclID_e::I32);
					EXPECT_EQ(val->line, 1);
					EXPECT_EQ(val->column, 6);
				});
			});

			// 3
			validateIntExpr(binExpr->rightDecl.get(), [](ExpressionConstantIntegerDecl* val) {
				EXPECT_EQ(val->valueDecl, 3);
				EXPECT_EQ(val->valueType, TypeDeclID_e::I32);
				EXPECT_EQ(val->line, 1);
				EXPECT_EQ(val->column, 11);
			});
		});
	}

	TEST_F(ParserExpressionTest, TestExpressionTernary)
	{
		ctx.parser->loadSource("a > 5 ? 5 : 3 * 4");
		ctx.parser->nextToken();

		auto exprDecl = parser_objects::ParserObjectExpressionDecl::parse(&ctx);

		// [a > 5 ? 5 : 3 * 4]
		validateTerExpr(exprDecl.get(), [](ExpressionTernaryDecl* terExpr) {
			EXPECT_EQ(terExpr->line, 1);
			EXPECT_EQ(terExpr->column, 1);

			// [a > 5] ? 5 : 3 * 4
			validateBinExpr(terExpr->conditionDecl.get(), [](ExpressionBinaryDecl* binExpr) {
				EXPECT_EQ(binExpr->op, TokenSubType_e::GreaterThan);
				EXPECT_EQ(binExpr->line, 1);
				EXPECT_EQ(binExpr->column, 1);

				// [a] > 5 ? 5 : 3 * 4
				validateIdeExpr(binExpr->leftDecl.get(), [](ExpressionConstantIdentifierDecl* ident) {
					EXPECT_EQ(ident->identifierDecl, "a");
					EXPECT_EQ(ident->startFromRoot, false);
					EXPECT_EQ(ident->line, 1);
					EXPECT_EQ(ident->column, 1);
				});

				// a > [5] ? 5 : 3 * 4
				validateIntExpr(binExpr->rightDecl.get(), [](ExpressionConstantIntegerDecl* val) {
					EXPECT_EQ(val->valueDecl, 5);
					EXPECT_EQ(val->valueType, TypeDeclID_e::I32);
					EXPECT_EQ(val->line, 1);
					EXPECT_EQ(val->column, 5);
				});
			});

			// a > 5 ? [5] : 3 * 4
			validateIntExpr(terExpr->leftDecl.get(), [](ExpressionConstantIntegerDecl* val) {
				EXPECT_EQ(val->valueDecl, 5);
				EXPECT_EQ(val->valueType, TypeDeclID_e::I32);
				EXPECT_EQ(val->line, 1);
				EXPECT_EQ(val->column, 9);
			});

			// a > 5 ? 5 : [3 * 4]
			validateBinExpr(terExpr->rightDecl.get(), [](ExpressionBinaryDecl* binExpr) {
				EXPECT_EQ(binExpr->op, TokenSubType_e::Multiplication);
				EXPECT_EQ(binExpr->line, 1);
				EXPECT_EQ(binExpr->column, 13);

				// a > 5 ? 5 : [3] * 4
				validateIntExpr(binExpr->leftDecl.get(), [](ExpressionConstantIntegerDecl* val) {
					EXPECT_EQ(val->valueDecl, 3);
					EXPECT_EQ(val->valueType, TypeDeclID_e::I32);
					EXPECT_EQ(val->line, 1);
					EXPECT_EQ(val->column, 13);
				});

				// a > 5 ? 5 : 3 * [4]
				validateIntExpr(binExpr->rightDecl.get(), [](ExpressionConstantIntegerDecl* val) {
					EXPECT_EQ(val->valueDecl, 4);
					EXPECT_EQ(val->valueType, TypeDeclID_e::I32);
					EXPECT_EQ(val->line, 1);
					EXPECT_EQ(val->column, 17);
				});
			});
		});

	}

	TEST_F(ParserExpressionTest, TestExpressionPass)
	{
		ctx.parser->loadSourceFromFile(".\\files\\parser\\source_3.txt");
		ctx.parser->nextToken();

		int exprCount = 0;
		while (true)
		{
			if (ctx.parser->isEof())
			{
				break;
			}
			parser_objects::ParserObjectExpressionDecl::parse(&ctx);
			ctx.parser->nextToken();
			exprCount++;
		}
		ASSERT_EQ(exprCount, 20);
	}

	TEST_F(ParserExpressionTest, TestExpressionGeneric)
	{
		ctx.parser->loadSource("t.foo<T>(2)");
		ctx.parser->nextToken();

		parser_objects::ParserObjectExpressionDecl::parse(&ctx);

		if (!ctx.parser->isEof())
		{
			throw std::exception();
		}
	}
} }