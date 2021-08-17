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
		U32										line;
		U32										column;
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
			, op(TokenSubType_e::Unknown)
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
			, op(TokenSubType_e::Unknown)
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
			: ExpressionDecl(ExpressionDeclType_e::FunctionDecl, line, column)
		{}

		virtual ~ExpressionFunctionDecl()
		{}

		TypeDeclPtr								returnTypeDecl;
		ExpressionFunctionParameterDeclPtrList	parametersDecl;
		BlockDeclPtr							blockDecl;
	};

	/**
	 * ExpressionFunctionCall
	 */

	class ExpressionFunctionCall : public ExpressionDecl
	{
	public:
		ExpressionFunctionCall(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::FunctionCall, line, column)
		{}

		virtual ~ExpressionFunctionCall()
		{}

		ExpressionDeclPtr						leftDecl;
		ExpressionDeclPtr						rightDecl;
	};

	/**
	 * ExpressionIndexAddress
	 */

	class ExpressionIndexAddress : public ExpressionDecl
	{
	public:
		ExpressionIndexAddress(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::Index, line, column)
		{}

		virtual ~ExpressionIndexAddress()
		{}

		ExpressionDeclPtr						leftDecl;
		ExpressionDeclPtr						rightDecl;
	};

	/**
	 * ExpressionConstantBoolDecl
	 */

	class ExpressionConstantBoolDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantBoolDecl(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::ConstantBool, line, column)
			, valueDecl(false)
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
			, valueDecl(0)
			, valueType(TypeDeclID_e::Unknown)
		{}

		virtual ~ExpressionConstantIntegerDecl()
		{}

		long long int							valueDecl;
		TypeDeclID_e							valueType;
	};

	/**
	 * ExpressionConstantRealDecl
	 */

	class ExpressionConstantRealDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantRealDecl(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::ConstantReal, line, column)
			, valueDecl(0.0)
			, valueType(TypeDeclID_e::Unknown)
		{}

		virtual ~ExpressionConstantRealDecl()
		{}

		double									valueDecl;
		TypeDeclID_e							valueType;
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
	 * ExpressionConstantCharDecl
	 */

	class ExpressionConstantCharDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantCharDecl(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::ConstantChar, line, column)
			, valueDecl(0)
		{}

		virtual ~ExpressionConstantCharDecl()
		{}

		I8										valueDecl;
	};
	
	/**
	 * ExpressionThisDecl
	 */

	class ExpressionThisDecl : public ExpressionDecl
	{
	public:
		ExpressionThisDecl(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::This, line, column)
		{}

		virtual ~ExpressionThisDecl()
		{}
	};

	/**
	 * ExpressionSuperDecl
	 */

	class ExpressionSuperDecl : public ExpressionDecl
	{
	public:
		ExpressionSuperDecl(const U32 line, const U32 column)
			: ExpressionDecl(ExpressionDeclType_e::Super, line, column)
		{}

		virtual ~ExpressionSuperDecl()
		{}
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
			, startFromRoot(false)
		{}

		virtual ~ExpressionConstantIdentifierDecl()
		{}

		String									identifierDecl;
		Bool									startFromRoot;
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