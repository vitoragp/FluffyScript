#pragma once
#include <memory>
#include <vector>
#include "fl_defs.h"

namespace fluffy { namespace ast {
	class TypeDecl;
	class TypeDeclFunction;
	class BlockDecl;
} }

namespace fluffy { namespace ast { namespace expr {
	using TypeDeclPtr								= std::unique_ptr<TypeDecl>;
	using TypeDeclFunctionPtr						= std::unique_ptr<TypeDeclFunction>;

	using BlockDeclPtr								= std::unique_ptr<BlockDecl>;

	using ExpressionDeclPtr							= std::unique_ptr<class ExpressionDecl>;

	using ExpressionFunctionParameterDeclPtr		= std::unique_ptr<class ExpressionFunctionParameterDecl>;
	using ExpressionFunctionParameterDeclPtrList	= std::vector<ExpressionFunctionParameterDeclPtr>;

	using ExpressionMatchWhenDeclPtr				= std::unique_ptr<class ExpressionMatchWhenDecl>;
	using ExpressionMatchWhenDeclPtrList			= std::vector<ExpressionMatchWhenDeclPtr>;

	using PatternDeclPtr							= std::unique_ptr<class PatternDecl>;

	/**
	 * Enums
	 */

	enum class ExpressionDeclType_e
	{
		Unknown,

		Ternary,
		Binary,
		Unary,
		As,
		Is,
		Function,
		Match,
		ConstantBool,
		ConstantInteger,
		ConstantReal,
		ConstantString,
		ConstantNull,
		ConstantIdentifier
	};

	/**
	 * ExpressionUnaryType_e
	 */

	enum class ExpressionUnaryType_e
	{
		Unknown,

		Prefix,
		Posfix
	};

	/**
	 * PatternType_e
	 */

	enum class PatterType_e
	{
		Unknown
	};

	/**
	 * ExpressionDecl
	 */

	class ExpressionDecl
	{
	protected:
		ExpressionDecl(ExpressionDeclType_e type, const U32 line, const U32 column)
			: type(type)
			, line(line)
			, column(column)
		{}

	public:

		virtual ~ExpressionDecl()
		{}

		const ExpressionDeclType_e				type;
		const U32								line;
		const U32								column;
	};

	/**
	 * ExpressionTernaryDecl
	 */

	class ExpressionTernaryDecl : public ExpressionDecl
	{
	public:
		ExpressionTernaryDecl(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::Ternary, line, column)
		{}

		virtual ~ExpressionTernaryDecl()
		{}

		ExpressionDeclPtr						conditionDecl;
		ExpressionDeclPtr						leftDecl;
		ExpressionDeclPtr						rightDecl;
	};

	/**
	 * ExpressionBinaryDecl
	 */

	class ExpressionBinaryDecl : public ExpressionDecl
	{
	public:
		ExpressionBinaryDecl(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::Binary, line, column)
		{}

		virtual ~ExpressionBinaryDecl()
		{}

		ExpressionDeclPtr						leftDecl;
		ExpressionDeclPtr						rightDecl;
		TokenSubType_e							op;
	};

	/**
	 * ExpressionUnaryDecl
	 */

	class ExpressionUnaryDecl : public ExpressionDecl
	{
	public:
		ExpressionUnaryDecl(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::Unary, line, column)
			, unaryType(ExpressionUnaryType_e::Unknown)
		{}

		virtual ~ExpressionUnaryDecl()
		{}

		ExpressionDeclPtr						exprDecl;
		ExpressionUnaryType_e					unaryType;
		TokenSubType_e							op;
	};

	/**
	 * ExpressionAsDecl
	 */

	class ExpressionAsDecl : public ExpressionDecl
	{
	public:
		ExpressionAsDecl(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::As, line, column)
		{}

		virtual ~ExpressionAsDecl()
		{}

		ExpressionDeclPtr						exprDecl;
		TypeDeclPtr								typeDecl;

	};

	/**
	 * ExpressionIsDecl
	 */

	class ExpressionIsDecl : public ExpressionDecl
	{
	public:
		ExpressionIsDecl(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::Is, line, column)
		{}

		virtual ~ExpressionIsDecl()
		{}

		ExpressionDeclPtr						exprDecl;
		TypeDeclPtr								typeDecl;
	};

	/**
	 * ExpressionMatchDecl
	 */

	class ExpressionMatchDecl : public ExpressionDecl
	{
	public:
		ExpressionMatchDecl(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::Match, line, column)
		{}

		virtual ~ExpressionMatchDecl()
		{}

		ExpressionDeclPtr						exprDecl;
		ExpressionMatchWhenDeclPtrList			whenDeclList;
	};

	/**
	 * ExpressionFunctionDecl
	 */

	class ExpressionFunctionDecl : public ExpressionDecl
	{
	public:
		ExpressionFunctionDecl(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::Function, line, column)
		{}

		virtual ~ExpressionFunctionDecl()
		{}

		TypeDeclPtr								returnTypeDecl;
		ExpressionFunctionParameterDeclPtrList	parametersDecl;
	};

	/**
	 * ExpressionConstantBoolDecl
	 */

	class ExpressionConstantBoolDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantBoolDecl(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::ConstantBool, line, column)
		{}

		virtual ~ExpressionConstantBoolDecl()
		{}

		Bool									valueDecl;
	};

	/**
	 * ExpressionConstantIntegerDecl
	 */

	class ExpressionConstantIntegerDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantIntegerDecl(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::ConstantInteger, line, column)
		{}

		virtual ~ExpressionConstantIntegerDecl()
		{}

		long long int							valueDecl;
		TokenSubType_e							valueType;
	};

	/**
	 * ExpressionConstantRealDecl
	 */

	class ExpressionConstantRealDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantRealDecl(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::ConstantReal, line, column)
		{}

		virtual ~ExpressionConstantRealDecl()
		{}

		double									valueDecl;
		TokenSubType_e							valueType;
	};

	/**
	 * ExpressionConstantStringDecl
	 */

	class ExpressionConstantStringDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantStringDecl(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::ConstantString, line, column)
		{}

		virtual ~ExpressionConstantStringDecl()
		{}

		String									valueDecl;
	};

	/**
	 * ExpressionConstantNullDecl
	 */

	class ExpressionConstantNullDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantNullDecl(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::ConstantNull, line, column)
		{}

		virtual ~ExpressionConstantNullDecl()
		{}
	};

	/**
	 * ExpressionConstantIdentifierDecl
	 */

	class ExpressionConstantIdentifierDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantIdentifierDecl(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::ConstantIdentifier, line, column)
		{}

		virtual ~ExpressionConstantIdentifierDecl()
		{}

		String									identifierDecl;
	};

	/**
	 * ExpressionMatchWhenDecl
	 */

	class ExpressionMatchWhenDecl
	{
	public:
		ExpressionMatchWhenDecl()
		{}

		~ExpressionMatchWhenDecl()
		{}

		PatternDeclPtr							patternDecl;
		BlockDeclPtr							blockDecl;
	};

	/**
	 * PatternDecl
	 */

	class PatternDecl
	{
	protected:
		PatternDecl(PatterType_e type)
			: type(type)
		{}

	public:
		virtual ~PatternDecl()
		{}

		const PatterType_e						type;
	};

	/**
	 * ExpressionFunctionParameterDecl
	 */

	class ExpressionFunctionParameterDecl
	{
	public:
		ExpressionFunctionParameterDecl(const U32 line, const U32 column)
			: line(line)
			, column(column)
		{}

		~ExpressionFunctionParameterDecl()
		{}

		String									identifierDecl;
		TypeDeclPtr								typeDecl;
		const U32								line;
		const U32								column;
	};
} } }