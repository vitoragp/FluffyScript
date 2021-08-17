#pragma once
#include <memory>
#include <vector>
#include "fl_defs.h"
#define FLUFFY_PRIMITIVE_TYPE_DECL(type) \
	class TypeDecl##type : public TypeDecl \
	{ \
	public: \
		TypeDecl##type() \
			: TypeDecl(TypeDeclID_e::type) \
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

	class TypeDecl
	{
	protected:
		TypeDecl(TypeDeclID_e typeID)
			: typeID(typeID)
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
		TypeDeclArray()
			: TypeDecl(TypeDeclID_e::Array)
		{}

		virtual ~TypeDeclArray()
		{}

		ArrayDeclPtrList					arrayDeclList;
		TypeDeclPtr							valueType;
	};

	/**
	 * TypeDeclVector
	 */

	class TypeDeclVector : public TypeDecl
	{
	public:
		TypeDeclVector()
			: TypeDecl(TypeDeclID_e::Vector)
		{}

		virtual ~TypeDeclVector()
		{}

		TypeDeclPtr							valueType;
	};

	/**
	 * TypeDeclSet
	 */

	class TypeDeclSet : public TypeDecl
	{
	public:
		TypeDeclSet()
			: TypeDecl(TypeDeclID_e::Set)
		{}

		virtual ~TypeDeclSet()
		{}

		TypeDeclPtr							valueType;
	};

	/**
	 * TypeDeclMap
	 */

	class TypeDeclMap : public TypeDecl
	{
	public:
		TypeDeclMap()
			: TypeDecl(TypeDeclID_e::Map)
		{}

		virtual ~TypeDeclMap()
		{}

		TypeDeclPtr							keyType;
		TypeDeclPtr							valueType;
	};

	/**
	 * TypeDeclFunction
	 */

	class TypeDeclFunction : public TypeDecl
	{
	public:
		TypeDeclFunction()
			: TypeDecl(TypeDeclID_e::Function)
		{}

		virtual ~TypeDeclFunction()
		{}

		TypeDeclPtr							returnType;
		TypeDeclPtrList						parameterTypeList;
	};

	/**
	 * TypeDeclNamed
	 */

	class TypeDeclNamed : public TypeDecl
	{
	public:
		TypeDeclNamed()
			: TypeDecl(TypeDeclID_e::Named)
			, startFromRoot(false)
		{}

		virtual ~TypeDeclNamed()
		{}

		String								identifier;
		TypeDeclNamedPtr					internalIdentifier;
		TypeDeclPtrList						genericDefinitionList;
		Bool								startFromRoot;
	};

	/**
	 * ArrayDecl
	 */

	class ArrayDecl
	{
	public:
		ArrayDecl(ArrayType_e arrayType)
			: arrayType(arrayType)
		{}

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
		SizedArrayDecl()
			: ArrayDecl(ArrayType_e::Sized)
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
		UnsizedArrayDecl()
			: ArrayDecl(ArrayType_e::Unsized)
		{}

		virtual ~UnsizedArrayDecl()
		{}
	};
} }