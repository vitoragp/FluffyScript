#pragma once
#include <memory>
#include <vector>
#include "fl_ast_expr.h"
#include "fl_ast_type.h"
#include "fl_defs.h"

namespace fluffy { namespace ast {
	using std::vector;
	using std::unique_ptr;

	using namespace expr;

	using StringList						= vector<String>;

	using IncludeDeclPtr					= unique_ptr<class IncludeDecl>;
	using IncludeDeclPtrList				= vector<IncludeDeclPtr>;

	using CodeUnitPtr						= unique_ptr<class CodeUnit>;
	using CodeUnitPtrList					= vector<CodeUnitPtr>;

	using NamespaceDeclPtr					= unique_ptr<class NamespaceDecl>;
	using NamespaceDeclPtrList				= vector<NamespaceDeclPtr>;

	using GeneralStmtPtr					= unique_ptr<class GeneralStmt>;
	using GeneralStmtPtrList				= vector<GeneralStmtPtr>;

	using ClassFunctionDeclPtr				= unique_ptr<class ClassFunctionDecl>;
	using ClassFunctionDeclPtrList			= vector<ClassFunctionDeclPtr>;

	using ClassVariableDeclPtr				= unique_ptr<class ClassVariableDecl>;
	using ClassVariableDeclPtrList			= vector<ClassVariableDeclPtr>;

	using ClassFunctionParameterDeclPtr		= unique_ptr<class ClassFunctionParameterDecl>;
	using ClassFunctionParameterDeclPtrList = vector<ClassFunctionParameterDeclPtr>;		

	using GenericDeclPtr					= unique_ptr<class GenericDecl>;
	using GenericDeclPtrList				= vector<GenericDeclPtr>;

	using TypeDeclPtr						= unique_ptr<TypeDecl>;
	using TypeDeclPtrList					= vector<TypeDeclPtr>;

	using TypeDeclNamedPtr					= unique_ptr<TypeDeclNamed>;

	using ExpressionDeclPtr					= unique_ptr<ExpressionDecl>;

	using BlockDeclPtr						= unique_ptr<class BlockDecl>;

	using ArrayDeclPtr						= unique_ptr<class ArrayDecl>;
	using ArrayDeclPtrList					= vector<ArrayDeclPtr>;		

	using ScopedIdentifierDeclPtr			= unique_ptr<class ScopedIdentifierDecl>;
	using ScopedIdentifierDeclPtrList		= vector<ScopedIdentifierDeclPtr>;

	/**
		* Program
		*/

	class Program
	{
	public:
		Program() {}
		~Program() {}

		IncludeDeclPtrList				includeDeclList;
		CodeUnitPtrList					codeUnits;			
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
		* CodeUnit
		*/

	class CodeUnit
	{
	public:
		CodeUnit(const String& name)
			: name(name)
		{}

		~CodeUnit()
		{}

		const String					name;
		NamespaceDeclPtrList			namespaceDeclList;
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
		ClassVariableDeclPtrList			staticVariableList;
		ClassVariableDeclPtrList			variableList;
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

		BlockDeclPtr						blockDecl;
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
		// ExpressionDeclPtr					defaultValueDecl;
	};

	/**
	 * ClassVariableDecl
	 */

	class ClassVariableDecl : public ClassMemberDecl
	{
	public:
		ClassVariableDecl()
			: ClassMemberDecl(ClassMemberType_e::Variable)
			, isConst(false)
			, isReference(false)
			, isStatic(false)
		{}

		virtual ~ClassVariableDecl()
		{}

		String								identifier;
		Bool								isConst;
		Bool								isReference;
		Bool								isStatic;
		TypeDeclPtr							typeDecl;
		ExpressionDeclPtr					initExpression;
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

	/**
	 * BlockDecl
	 */

	class BlockDecl
	{
	protected:
		BlockDecl(BlockType_e type)
			: type(type)
		{}

	public:
		virtual ~BlockDecl()
		{}

		const BlockType_e					type;
	};
} }
