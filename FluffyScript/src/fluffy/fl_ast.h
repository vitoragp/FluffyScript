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

namespace fluffy {
	namespace ast {
		using std::vector;
		using std::unique_ptr;

		using StringList						= vector<String>;

		using IncludeDeclPtr					= unique_ptr<class IncludeDecl>;
		using IncludeDeclPtrList				= vector<IncludeDeclPtr>;

		using NamespaceDeclPtr					= unique_ptr<class NamespaceDecl>;
		using NamespaceDeclPtrList				= vector<NamespaceDeclPtr>;

		using GeneralStmtPtr					= unique_ptr<class GeneralStmt>;
		using GeneralStmtPtrList				= vector<GeneralStmtPtr>;

		using ClassFunctionDeclPtr				= unique_ptr<class ClassFunctionDecl>;
		using ClassFunctionDeclPtrList			= vector<ClassFunctionDeclPtr>;

		using ClassFunctionParameterDeclPtr		= unique_ptr<class ClassFunctionParameterDecl>;
		using ClassFunctionParameterDeclPtrList = vector<ClassFunctionParameterDeclPtr>;		

		using GenericDeclPtr					= unique_ptr<class GenericDecl>;
		using GenericDeclPtrList				= vector<GenericDeclPtr>;

		using TypeDeclPtr						= unique_ptr<class TypeDecl>;
		using TypeDeclPtrList					= vector<TypeDeclPtr>;

		using TypeDeclNamedPtr					= unique_ptr<class TypeDeclNamed>;

		using ArrayDeclPtr						= unique_ptr<class ArrayDecl>;
		using ArrayDeclPtrList					= vector<ArrayDeclPtr>;		

		using ScopedIdentifierDeclPtr			= unique_ptr<class ScopedIdentifierDecl>;
		using ScopedIdentifierDeclPtrList		= vector<ScopedIdentifierDeclPtr>;

		/**
		 * Enumerations
		 */

		enum class GeneralStmtType_e
		{
			Unknown,
			ClassDecl,
			InterfaceDecl,
			StructDecl,
			EnumDecl,
			TraitDecl,
			VariableDecl,
			FunctionDecl
		};

		enum class ClassMemberType_e
		{
			Unknown,

			Constructor,
			Destructor,

			StaticFunction,
			Function,

			StaticVariable,
			Variable,

			StaticConst,
			Const
		};

		enum class ClassMemberAccessModifier_e
		{
			Unknown,

			Public,
			Protected,
			Private
		};


		enum class TypeDeclID_e
		{
			Unknown,

			Void,
			Bool,
			I8,
			U8,
			I16,
			U16,
			I32,
			U32,
			I64,
			U64,
			Fp32,
			Fp64,
			String,
			Object,

			Array,

			Vector,
			Set,
			Map,

			Function,

			Named			// Classes, interfaces, trait, enum...
		};

		/**
		 * Program
		 */

		class Program
		{
		public:
			Program() {}
			~Program() {}

			IncludeDeclPtrList				includeDeclList;
			NamespaceDeclPtrList			namespaceDeclList;
		};

		/**
		 * Include
		 */

		class IncludeDecl
		{
		public:
			IncludeDecl() {}
			~IncludeDecl() {}

			StringList						includedItemList;
			ScopedIdentifierDeclPtr			fromNamespace;
		};

		/**
		 * NamespaceDecl
		 */

		class NamespaceDecl
		{
		public:
			NamespaceDecl() {}
			~NamespaceDecl() {}

			String							name;
			NamespaceDeclPtrList			namespaceDeclList;
			GeneralStmtPtrList				generalDeclList;
		};

		/**
		 * GeneralStmt
		 */

		class GeneralStmt
		{
		protected:
			GeneralStmt(GeneralStmtType_e type)
				: type(type)
			{}

		public:
			virtual	~GeneralStmt()
			{}

			GeneralStmtType_e				type;
	};

	/**
	 * ClassDecl
	 */

	class ClassDecl : public GeneralStmt
	{
	public:
		ClassDecl()
			: GeneralStmt(GeneralStmtType_e::ClassDecl)
			, isExported(false)
			, isAbstract(false)
		{}

		virtual ~ClassDecl()
		{}

		Bool								isExported;
		Bool								isAbstract;
		String								name;
		GenericDeclPtrList					genericTemplateList;
		TypeDeclPtr							baseClass;
		TypeDeclPtrList						interfaceList;
		ClassFunctionDeclPtrList			staticFunctionList;
		ClassFunctionDeclPtrList			functionList;
		// StaticVariableList				staticVariableList;
		// VariableList						variableList;
		// ConstructorList					constructorList;
		// DestructorDecl					destructorDecl;
	};

	/**
	 * ClassMemberDecl
	 */

	class ClassMemberDecl
	{
	protected:
		ClassMemberDecl(ClassMemberType_e type)
			: type(type)
			, accessModifier(ClassMemberAccessModifier_e::Unknown)
		{}

	public:
		virtual ~ClassMemberDecl()
		{}

		const ClassMemberType_e				type;

		ClassMemberAccessModifier_e			accessModifier;
	};

	/**
	 * ClassFunctionDecl
	 */

	class ClassFunctionDecl : public ClassMemberDecl
	{
	public:
		ClassFunctionDecl()
			: ClassMemberDecl(ClassMemberType_e::Function)
			, isStatic(false)
			, isVirtual(false)
			, isAbstract(false)
			, isOverride(false)
			, isFinal(false)
		{}

		virtual ~ClassFunctionDecl()
		{}

		String								identifier;

		Bool								isStatic;
		Bool								isVirtual;
		Bool								isAbstract;
		Bool								isOverride;
		Bool								isFinal;

		GenericDeclPtrList					genericTemplateList;

		ClassFunctionParameterDeclPtrList	parameterList;
		TypeDeclPtr							returnType;

		// BlockDeclPtr						blockDecl;
	};

	/**
	 * ClassFunctionParameterDecl
	 */

	class ClassFunctionParameterDecl
	{
	public:
		ClassFunctionParameterDecl()
		{}

		~ClassFunctionParameterDecl()
		{}

		TypeDeclPtr							typeDecl;
		String								identifier;
		// ExpressionPtr					defaultValueDecl;
	};

	/**
	 * GenericDecl
	 */

	class GenericDecl
	{
	public:
		GenericDecl()
		{}

		~GenericDecl()
		{}

		String								identifier;
		TypeDeclPtrList						whereTypeList;
	};

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
	 * Array
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
	 * Vector
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
	 * Set
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
	 * Map
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
	 * Function
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
	 * Named
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
		enum class ArrayType_e
		{
			Sized,
			Unsized
		};

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
			: ArrayDecl(ArrayDecl::ArrayType_e::Sized)
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
			: ArrayDecl(ArrayDecl::ArrayType_e::Unsized)
		{}

		virtual ~UnsizedArrayDecl()
		{}
	};

	/**
	 * ScopedIdentifierDecl
	 */

	class ScopedIdentifierDecl
	{
	public:
		ScopedIdentifierDecl()
			: startFromRoot(false)
		{}

		~ScopedIdentifierDecl()
		{}

		String								identifier;
		ScopedIdentifierDeclPtr				tailIdentifier;
		Bool								startFromRoot;
	};
} }
