#pragma once
#include <memory>
#include <vector>
#include "fl_ast.h"
#include "fl_ast_expr.h"
#include "fl_ast_type.h"
#include "..\\fl_defs.h"

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

	using GeneralStmtPtr					= unique_ptr<class GeneralDecl>;
	using GeneralStmtPtrList				= vector<GeneralStmtPtr>;

	using ClassFunctionDeclPtr				= unique_ptr<class ClassFunctionDecl>;
	using ClassFunctionDeclPtrList			= vector<ClassFunctionDeclPtr>;

	using ClassVariableDeclPtr				= unique_ptr<class ClassVariableDecl>;
	using ClassVariableDeclPtrList			= vector<ClassVariableDeclPtr>;

	using ClassConstructorDeclPtr			= unique_ptr<class ClassConstructorDecl>;
	using ClassConstructorDeclPtrList		= vector<ClassConstructorDeclPtr>;

	using ClassDestructorDeclPtr			= unique_ptr<class ClassDestructorDecl>;
	using ClassDestructorDeclPtrList		= vector<ClassDestructorDeclPtr>;

	using ClassVariableInitDeclPtr			= unique_ptr<class ClassVariableInitDecl>;
	using ClassVariableInitDeclPtrList		= vector<ClassVariableInitDeclPtr>;

	using InterfaceFunctionDeclPtr			= unique_ptr<class InterfaceFunctionDecl>;
	using InterfaceFunctionDeclPtrList		= vector<InterfaceFunctionDeclPtr>;

	using StructVariableDeclPtr				= unique_ptr<class StructVariableDecl>;
	using StructVariableDeclPtrList			= vector<StructVariableDeclPtr>;

	using EnumItemDeclPtr					= unique_ptr<class EnumItemDecl>;
	using EnumItemDeclPtrList				= vector<EnumItemDeclPtr>;

	using TraitFunctionDeclPtr				= unique_ptr<class TraitFunctionDecl>;
	using TraitFunctionDeclPtrList			= vector<TraitFunctionDeclPtr>;

	using FunctionParameterDeclPtr			= unique_ptr<class FunctionParameterDecl>;
	using FunctionParameterDeclPtrList		= vector<FunctionParameterDeclPtr>;		

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

		IncludeDeclPtrList					includeDeclList;
		CodeUnitPtrList						codeUnitList;
	};

	/**
		* Include
		*/

	class IncludeDecl
	{
	public:
		IncludeDecl(U32 line, U32 column)
			: line(line)
			, column(column)
		{}

		~IncludeDecl()
		{}

		StringList							includedItemList;
		ScopedIdentifierDeclPtr				fromNamespace;
		U32									line;
		U32									column;
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

		const String						name;
		NamespaceDeclPtrList				namespaceDeclList;
	};

	/**
		* NamespaceDecl
		*/

	class NamespaceDecl : public AstNodeIdentified
	{
	public:
		NamespaceDecl(U32 line, U32 column)
			: AstNodeIdentified(line, column)
		{}

		~NamespaceDecl()
		{}

		NamespaceDeclPtrList				namespaceDeclList;
		GeneralStmtPtrList					generalDeclList;
	};

	/**
		* GeneralDecl
		*/

	class GeneralDecl : public AstNodeIdentified
	{
	protected:
		GeneralDecl(GeneralDeclType_e type, U32 line, U32 column)
			: AstNodeIdentified(line, column)
			, type(type)
		{}

	public:
		virtual	~GeneralDecl()
		{}

		const GeneralDeclType_e				type;

	};

	/**
	 * ClassDecl
	 */

	class ClassDecl : public GeneralDecl
	{
	public:
		ClassDecl(U32 line, U32 column)
			: GeneralDecl(GeneralDeclType_e::ClassDecl, line, column)
			, isExported(false)
			, isAbstract(false)
		{}

		virtual ~ClassDecl()
		{}

		Bool								isExported;
		Bool								isAbstract;
		GenericDeclPtrList					genericDeclList;
		TypeDeclPtr							baseClass;
		TypeDeclPtrList						interfaceList;
		ClassFunctionDeclPtrList			staticFunctionList;
		ClassFunctionDeclPtrList			functionList;
		ClassVariableDeclPtrList			staticVariableList;
		ClassVariableDeclPtrList			variableList;
		ClassConstructorDeclPtrList			constructorList;
		ClassDestructorDeclPtr				destructorDecl;
	};

	/**
	 * ClassMemberDecl
	 */

	class ClassMemberDecl : public AstNodeIdentified
	{
	protected:
		ClassMemberDecl(ClassMemberType_e type, U32 line, U32 column)
			: AstNodeIdentified(line, column)
			, type(type)
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
		ClassFunctionDecl(U32 line, U32 column)
			: ClassMemberDecl(ClassMemberType_e::Function, line, column)
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

		GenericDeclPtrList					genericDeclList;

		FunctionParameterDeclPtrList		parameterList;
		TypeDeclPtr							returnType;

		BlockDeclPtr						blockDecl;
	};

	/**
	 * ClassVariableDecl
	 */

	class ClassVariableDecl : public ClassMemberDecl
	{
	public:
		ClassVariableDecl(U32 line, U32 column)
			: ClassMemberDecl(ClassMemberType_e::Variable, line, column)
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
	 * ClassConstructorDecl
	 */

	class ClassConstructorDecl : public ClassMemberDecl
	{
	public:
		ClassConstructorDecl(U32 line, U32 column)
			: ClassMemberDecl(ClassMemberType_e::Constructor, line, column)
		{}

		virtual ~ClassConstructorDecl()
		{}

		FunctionParameterDeclPtrList		parameterList;
		ExpressionDeclPtr					superParameters;
		ClassVariableInitDeclPtrList		variableInitDeclList;
		BlockDeclPtr						blockDecl;
	};

	/**
	 * ClassDestructorDecl
	 */

	class ClassDestructorDecl : public ClassMemberDecl
	{
	public:
		ClassDestructorDecl(U32 line, U32 column)
			: ClassMemberDecl(ClassMemberType_e::Destructor, line, column)
		{}

		virtual ~ClassDestructorDecl()
		{}

		BlockDeclPtr						blockDecl;
	};

	/**
	 * ClassVariableInitDecl
	 */

	class ClassVariableInitDecl : public AstNodeIdentified
	{
	public:
		ClassVariableInitDecl(U32 line, U32 column)
			: AstNodeIdentified(line, column)
		{}

		~ClassVariableInitDecl()
		{}

		ExpressionDeclPtr					initExpression;
	};

	/**
	 * InterfaceDecl
	 */

	class InterfaceDecl : public GeneralDecl
	{
	public:
		InterfaceDecl(U32 line, U32 column)
			: GeneralDecl(GeneralDeclType_e::InterfaceDecl, line, column)
			, isExported(false)
		{}

		virtual ~InterfaceDecl()
		{}

		Bool								isExported;
		String								identifier;
		GenericDeclPtrList					genericDeclList;
		InterfaceFunctionDeclPtrList		functionDeclList;
	};

	/**
	 * InterfaceFunctionDecl
	 */

	class InterfaceFunctionDecl : public AstNodeIdentified
	{
	public:
		InterfaceFunctionDecl(U32 line, U32 column)
			: AstNodeIdentified(line, column)
		{}

		~InterfaceFunctionDecl()
		{}

		GenericDeclPtrList					genericDeclList;
		FunctionParameterDeclPtrList		parameterList;
		TypeDeclPtr							returnType;
	};

	/**
	 * StructDecl
	 */

	class StructDecl : public GeneralDecl
	{
	public:
		StructDecl(U32 line, U32 column)
			: GeneralDecl(GeneralDeclType_e::StructDecl, line, column)
			, isExported(false)
		{}

		virtual ~StructDecl()
		{}

		Bool								isExported;
		String								identifier;
		GenericDeclPtrList					genericDeclList;
		StructVariableDeclPtrList			variableList;
	};

	/**
	 * StructVariableDecl
	 */

	class StructVariableDecl : public AstNodeIdentified
	{
	public:
		StructVariableDecl(U32 line, U32 column)
			: AstNodeIdentified(line, column)
			, isConst(false)
			, isReference(false)
		{}

		~StructVariableDecl()
		{}

		Bool								isConst;
		Bool								isReference;
		TypeDeclPtr							typeDecl;
		ExpressionDeclPtr					initExpression;
	};

	/**
	 * EnumDecl
	 */

	class EnumDecl : public GeneralDecl
	{
	public:
		EnumDecl(U32 line, U32 column)
			: GeneralDecl(GeneralDeclType_e::EnumDecl, line, column)
			, isExported(false)
		{}

		virtual ~EnumDecl()
		{}

		Bool								isExported;
		GenericDeclPtrList					genericDeclList;
		EnumItemDeclPtrList					enumItemDeclList;
	};

	/**
	 * EnumItemDecl 
	 */

	class EnumItemDecl : public AstNodeIdentified
	{
	public:
		EnumItemDecl(U32 line, U32 column)
			: AstNodeIdentified(line, column)
			, hasData(false)
			, hasValue(false)
		{}

		~EnumItemDecl()
		{}

		Bool								hasData;
		Bool								hasValue;
		TypeDeclPtrList						dataTypeDeclList;
		ExpressionDeclPtr					valueExpression;
	};

	/**
	 * TraitDecl
	 */

	class TraitDecl : public GeneralDecl
	{
	public:
		TraitDecl(U32 line, U32 column)
			: GeneralDecl(GeneralDeclType_e::TraitDecl, line, column)
			, isExported(false)
			, isDefinition(false)
		{}

		virtual ~TraitDecl()
		{}

		Bool								isExported;
		Bool								isDefinition;
		TypeDeclPtr							typeDefinitionDecl;
		GenericDeclPtrList					genericDeclList;
		TraitFunctionDeclPtrList			functionDeclList;
	};

	/**
	 * TraitFunctionDecl
	 */

	class TraitFunctionDecl : public AstNodeIdentified
	{
	public:
		TraitFunctionDecl(U32 line, U32 column)
			: AstNodeIdentified(line, column)
		{}

		~TraitFunctionDecl()
		{}

		GenericDeclPtrList					genericDeclList;
		FunctionParameterDeclPtrList		parameterList;
		TypeDeclPtr							returnType;
	};

	/**
	 * TraitSelfTypeDecl
	 */

	class TraitSelfTypeDecl : public TypeDecl
	{
	public:
		TraitSelfTypeDecl(U32 line, U32 column)
			: TypeDecl(TypeDeclID_e::Self, line, column)
		{}

		virtual ~TraitSelfTypeDecl()
		{}
	};

	/**
	 * FunctionDecl
	 */

	class FunctionDecl : public GeneralDecl
	{
	public:
		FunctionDecl(U32 line, U32 column)
			: GeneralDecl(GeneralDeclType_e::FunctionDecl, line, column)
			, isExported(false)
		{}

		virtual ~FunctionDecl()
		{}

		Bool								isExported;
		GenericDeclPtrList					genericDeclList;
		FunctionParameterDeclPtrList		parameterList;
		TypeDeclPtr							returnType;
		BlockDeclPtr						blockDecl;
	};

	/**
	 * VariableDecl
	 */

	class VariableDecl : public GeneralDecl
	{
	public:
		VariableDecl(U32 line, U32 column)
			: GeneralDecl(GeneralDeclType_e::VariableDecl, line, column)
			, isExported(false)
			, isConst(false)
			, isReference(false)
		{}

		virtual ~VariableDecl()
		{}

		Bool								isExported;
		Bool								isConst;
		Bool								isReference;
		TypeDeclPtr							typeDecl;
		ExpressionDeclPtr					initExpression;
	};

	/**
	 * FunctionParameterDecl
	 */

	class FunctionParameterDecl : public AstNodeIdentified
	{
	public:
		FunctionParameterDecl(U32 line, U32 column)
			: AstNodeIdentified(line, column)
		{}

		~FunctionParameterDecl()
		{}

		TypeDeclPtr							typeDecl;
	};

	/**
	 * GenericDecl
	 */

	class GenericDecl : public AstNodeIdentified
	{
	public:
		GenericDecl(U32 line, U32 column)
			: AstNodeIdentified(line, column)
		{}

		~GenericDecl()
		{}

		TypeDeclPtrList						whereTypeList;
	};

	/**
	 * ScopedIdentifierDecl
	 */

	class ScopedIdentifierDecl : public AstNodeIdentified
	{
	public:
		ScopedIdentifierDecl(U32 line, U32 column)
			: AstNodeIdentified(line, column)
			, startFromRoot(false)
		{}

		~ScopedIdentifierDecl()
		{}

		ScopedIdentifierDeclPtr				tailIdentifier;
		Bool								startFromRoot;
	};

	/**
	 * BlockDecl
	 */

	class BlockDecl : public AstNode
	{
	protected:
		BlockDecl(BlockType_e type, U32 line, U32 column)
			: AstNode(line, column)
			, type(type)
		{}

	public:
		virtual ~BlockDecl()
		{}

		const BlockType_e					type;
	};
} }
