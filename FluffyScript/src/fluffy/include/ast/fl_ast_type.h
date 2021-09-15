#pragma once
#include "fl_ast.h"
#include "fl_defs.h"
#include "fl_collections.h"
#define FLUFFY_PRIMITIVE_TYPE_DECL(type) \
	class TypeDecl##type : public ast::AstNode, public AstSafeCast<AstNodeType_e::type##Type> \
	{ \
	public: \
		TypeDecl##type(); \
		virtual ~TypeDecl##type(); \
	}

namespace fluffy { namespace ast {
	class ScopedPathDecl;
	class TraitDecl;
} }

namespace fluffy { namespace ast {
	using std::unique_ptr;
	using std::vector;

	using TypeDeclPtr						= unique_ptr<class TypeDecl>;
	using TypeDeclPtrList					= vector<TypeDeclPtr>;

	using TypeDeclNamedPtr					= unique_ptr<class TypeDeclNamed>;

	using ScopedPathDeclPtr					= unique_ptr<ast::ScopedPathDecl>;

	using ArrayDeclPtr						= unique_ptr<class ArrayDecl>;
	using ArrayDeclPtrList					= vector<ArrayDeclPtr>;		

	/**
	 * TypeDecl
	 */

	class TypeDecl : public AstNode
	{
	protected:
		TypeDecl(AstNodeType_e nodeType, U32 line, U32 column);

	public:
		virtual ~TypeDecl();

		Bool								nullable;
	};

	/**
	 * Tipos primitivos
	 */

	FLUFFY_PRIMITIVE_TYPE_DECL(Void);
	FLUFFY_PRIMITIVE_TYPE_DECL(Bool);
	FLUFFY_PRIMITIVE_TYPE_DECL(I8);
	FLUFFY_PRIMITIVE_TYPE_DECL(U8);
	FLUFFY_PRIMITIVE_TYPE_DECL(I16);
	FLUFFY_PRIMITIVE_TYPE_DECL(U16);
	FLUFFY_PRIMITIVE_TYPE_DECL(I32);
	FLUFFY_PRIMITIVE_TYPE_DECL(U32);
	FLUFFY_PRIMITIVE_TYPE_DECL(I64);
	FLUFFY_PRIMITIVE_TYPE_DECL(U64);
	FLUFFY_PRIMITIVE_TYPE_DECL(Fp32);
	FLUFFY_PRIMITIVE_TYPE_DECL(Fp64);
	FLUFFY_PRIMITIVE_TYPE_DECL(String);
	FLUFFY_PRIMITIVE_TYPE_DECL(Object);

	/**
	 * TypeDeclPrimitive
	 */

	class TypeDeclPrimitive : public TypeDecl, public AstSafeCast<AstNodeType_e::PrimitiveType>
	{
	public:
		TypeDeclPrimitive(U32 line, U32 column, ast::AstNode* const primitiveTypeRef, const PrimitiveTypeID_e primitiveType);
		virtual ~TypeDeclPrimitive();

		ast::AstNode* const					primitiveTypeRef;
		const PrimitiveTypeID_e				primitiveType;
	};

	/**
	 * TypeDeclArray
	 */

	class TypeDeclArray : public TypeDecl, public AstSafeCast<AstNodeType_e::ArrayType>
	{
	public:
		TypeDeclArray(U32 line, U32 column);
		virtual ~TypeDeclArray();

		ArrayDeclPtrList					arrayDeclList;
		TypeDeclPtr							valueType;
	};

	/**
	 * TypeDeclFunction
	 */

	class TypeDeclFunction : public TypeDecl, public AstSafeCast<AstNodeType_e::FunctionType>
	{
	public:
		TypeDeclFunction(U32 line, U32 column);
		virtual ~TypeDeclFunction();

		TypeDeclPtr							objectOwnerDecl;
		TypeDeclPtrList						parameterTypeList;
		TypeDeclPtr							returnType;
	};

	/**
	 * TypeDeclTuple
	 */

	class TypeDeclTuple : public TypeDecl, public AstSafeCast<AstNodeType_e::TupleType>
	{
	public:
		TypeDeclTuple(U32 line, U32 column);
		virtual ~TypeDeclTuple();

		TypeDeclPtrList						tupleItemList;
	};

	/**
	 * TypeDeclNamed
	 */

	class TypeDeclNamed : public TypeDecl, public AstSafeCast<AstNodeType_e::NamedType>
	{
	public:
		TypeDeclNamed(U32 line, U32 column);
		virtual ~TypeDeclNamed();

		ScopedPathDeclPtr					scopePath;
		TypeDeclPtrList						genericDefinitionList;
		Bool								startFromRoot;
	};

	/**
	 * SelfTypeDecl
	 */

	class SelfTypeDecl : public TypeDecl, public AstSafeCast<AstNodeType_e::SelfType>
	{
	public:
		SelfTypeDecl(U32 line, U32 column);
		virtual ~SelfTypeDecl();

	};

	/**
	 * ArrayDecl
	 */

	class ArrayDecl : public AstNode
	{
	protected:
		ArrayDecl(AstNodeType_e nodeType, ArrayType_e arrayType, U32 line, U32 column);

	public:
		virtual ~ArrayDecl();

		const ArrayType_e					arrayType;
	};

	/**
	 * SizedArrayDecl
	 */

	class SizedArrayDecl : public ArrayDecl, public AstSafeCast<AstNodeType_e::SizedArray>
	{
	public:
		SizedArrayDecl(U32 line, U32 column);
		virtual ~SizedArrayDecl();

		U32									size;
	};

	/**
	 * UnsizedArrayDecl
	 */

	class UnsizedArrayDecl : public ArrayDecl, public AstSafeCast<AstNodeType_e::UnsizedArray>
	{
	public:
		UnsizedArrayDecl(U32 line, U32 column);
		virtual ~UnsizedArrayDecl();
	};

	/**
	 * Funcoes auxiliares
	 */

	std::unique_ptr<ast::TypeDeclPrimitive>
	makePrimitiveType(U32 line, U32 column, PrimitiveTypeID_e typeId);
} }