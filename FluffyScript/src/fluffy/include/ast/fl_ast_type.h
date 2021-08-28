#pragma once
#include <memory>
#include <vector>
#include "fl_ast.h"
#include "fl_defs.h"
#define FLUFFY_PRIMITIVE_TYPE_DECL(type) \
	class TypeDecl##type : public TypeDecl \
	{ \
	public: \
		TypeDecl##type(U32 line, U32 column) \
			: TypeDecl(AstNodeType_e::type##Type, TypeDeclID_e::type, line, column) \
		{} \
		virtual ~TypeDecl##type() \
		{} \
	}

namespace fluffy { namespace ast {
	using std::unique_ptr;
	using std::vector;

	using TypeDeclPtr						= unique_ptr<class TypeDecl>;
	using TypeDeclPtrList					= vector<TypeDeclPtr>;

	using TypeDeclNamedPtr					= unique_ptr<class TypeDeclNamed>;

	using ArrayDeclPtr						= unique_ptr<class ArrayDecl>;
	using ArrayDeclPtrList					= vector<ArrayDeclPtr>;		

	/**
	 * TypeDecl
	 */

	class TypeDecl : public AstNode
	{
	protected:
		TypeDecl(AstNodeType_e nodeType, TypeDeclID_e typeID, U32 line, U32 column)
			: AstNode(nodeType, line, column)
			, typeID(typeID)
			, nullable(false)
		{}

	public:
		virtual ~TypeDecl()
		{}

		const TypeDeclID_e					typeID;
		Bool								nullable;
	};

	/**
	 * Tipos primitivos
	 */

	FLUFFY_PRIMITIVE_TYPE_DECL(Void);
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
	 * TypeDeclArray
	 */

	class TypeDeclArray : public TypeDecl
	{
	public:
		TypeDeclArray(U32 line, U32 column)
			: TypeDecl(AstNodeType_e::ArrayType, TypeDeclID_e::Array, line, column)
		{}

		virtual ~TypeDeclArray()
		{}

		ArrayDeclPtrList					arrayDeclList;
		TypeDeclPtr							valueType;
	};

	/**
	 * TypeDeclFunction
	 */

	class TypeDeclFunction : public TypeDecl
	{
	public:
		TypeDeclFunction(U32 line, U32 column)
			: TypeDecl(AstNodeType_e::FunctionType, TypeDeclID_e::Function, line, column)
		{}

		virtual ~TypeDeclFunction()
		{}

		TypeDeclPtr							returnType;
		TypeDeclPtrList						parameterTypeList;
	};

	/**
	 * TypeDeclTuple
	 */

	class TypeDeclTuple : public TypeDecl
	{
	public:
		TypeDeclTuple(U32 line, U32 column)
			: TypeDecl(AstNodeType_e::TupleType, TypeDeclID_e::Tuple, line, column)
		{}

		virtual ~TypeDeclTuple()
		{}

		TypeDeclPtrList						tupleItemList;
	};

	/**
	 * TypeDeclNamed
	 */

	class TypeDeclNamed : public TypeDecl
	{
	public:
		TypeDeclNamed(U32 line, U32 column)
			: TypeDecl(AstNodeType_e::NamedType, TypeDeclID_e::Named, line, column)
			, startFromRoot(false)
			, reference(nullptr)
		{}

		virtual ~TypeDeclNamed()
		{}

		TString								identifier;
		TypeDeclNamedPtr					internalIdentifier;
		TypeDeclPtrList						genericDefinitionList;
		Bool								startFromRoot;
		AstNode*							reference;
	};

	/**
	 * SelfTypeDecl
	 */

	class SelfTypeDecl : public TypeDecl
	{
	public:
		SelfTypeDecl(U32 line, U32 column)
			: TypeDecl(AstNodeType_e::SelfType, TypeDeclID_e::Self, line, column)
		{}

		virtual ~SelfTypeDecl()
		{}
	};

	/**
	 * ArrayDecl
	 */

	class ArrayDecl : public AstNode
	{
	protected:
		ArrayDecl(AstNodeType_e nodeType, ArrayType_e arrayType, U32 line, U32 column)
			: AstNode(nodeType, line, column)
			, arrayType(arrayType)
		{}

	public:
		virtual ~ArrayDecl()
		{}

		const ArrayType_e					arrayType;
	};

	/**
	 * SizedArrayDecl
	 */

	class SizedArrayDecl : public ArrayDecl
	{
	public:
		SizedArrayDecl(U32 line, U32 column)
			: ArrayDecl(AstNodeType_e::SizedArray, ArrayType_e::Sized, line, column)
			, size(0)
		{}

		virtual ~SizedArrayDecl()
		{}

		U32									size;
	};

	/**
	 * UnsizedArrayDecl
	 */

	class UnsizedArrayDecl : public ArrayDecl
	{
	public:
		UnsizedArrayDecl(U32 line, U32 column)
			: ArrayDecl(AstNodeType_e::UnsizedArray, ArrayType_e::Unsized, line, column)
		{}

		virtual ~UnsizedArrayDecl()
		{}
	};
} }