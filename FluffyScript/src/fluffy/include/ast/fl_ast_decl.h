#pragma once
#include <memory>
#include <vector>
#include "fl_ast.h"
#include "fl_ast_expr.h"
#include "fl_ast_type.h"
#include "fl_ast_stmt.h"
#include "fl_defs.h"

namespace fluffy { namespace ast {
	using std::vector;
	using std::unique_ptr;

	using namespace expr;
	using namespace stmt;

	using TStringList						= vector<TString>;

	using IncludeDeclPtr					= unique_ptr<class IncludeDecl>;
	using IncludeDeclPtrList				= vector<IncludeDeclPtr>;

	using IncludeItemDeclPtr				= unique_ptr<class IncludeItemDecl>;
	using IncludeItemDeclPtrList			= vector<IncludeItemDeclPtr>;

	using CodeUnitPtr						= unique_ptr<class CodeUnit>;
	using CodeUnitPtrList					= vector<CodeUnitPtr>;

	using NamespaceDeclPtr					= unique_ptr<class NamespaceDecl>;
	using NamespaceDeclPtrList				= vector<NamespaceDeclPtr>;

	using GeneralStmtDeclPtr				= unique_ptr<class GeneralStmtDecl>;
	using GeneralStmtDeclPtrList			= vector<GeneralStmtDeclPtr>;

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

	using GenericItemDeclPtr				= unique_ptr<class GenericItemDecl>;
	using GenericItemDeclPtrList			= vector<GenericItemDeclPtr>;

	using TypeDeclPtr						= unique_ptr<TypeDecl>;
	using TypeDeclPtrList					= vector<TypeDeclPtr>;

	using TypeDeclNamedPtr					= unique_ptr<TypeDeclNamed>;

	using ExpressionDeclPtr					= unique_ptr<ExpressionDecl>;

	using BlockDeclPtr						= unique_ptr<class BlockDecl>;

	using StmtDeclPtr						= unique_ptr<class StmtDecl>;
	using StmtDeclPtrList					= vector<StmtDeclPtr>;

	using ArrayDeclPtr						= unique_ptr<class ArrayDecl>;
	using ArrayDeclPtrList					= vector<ArrayDeclPtr>;		

	using ScopedIdentifierDeclPtr			= unique_ptr<class ScopedIdentifierDecl>;
	using ScopedIdentifierDeclPtrList		= vector<ScopedIdentifierDeclPtr>;

	/**
	 * CodeUnit
	 */

	class CodeUnit : public AstNode
	{
	public:
		CodeUnit(const String& name);
		virtual ~CodeUnit();

		virtual Bool
		hasChildren() final;

		virtual std::vector<AstNode*>
		getChildren() final;

		const String						name;
		IncludeDeclPtrList					includeDeclList;
		NamespaceDeclPtrList				namespaceDeclList;
	};

	/**
	 * IncludeDecl
	 */

	class IncludeDecl : public AstNode
	{
	public:
		IncludeDecl(U32 line, U32 column);
		virtual ~IncludeDecl();

		virtual Bool
		hasChildren() final;

		virtual std::vector<AstNode*>
		getChildren() final;

		IncludeItemDeclPtrList				includedItemList;
		ScopedIdentifierDeclPtr				inNamespace;
	};

	/**
	 * IncludeItemDecl
	 */

	class IncludeItemDecl : public AstNode
	{
	public:
		IncludeItemDecl(U32 line, U32 column);
		virtual ~IncludeItemDecl();

		TString								referencedId;
	};

	/**
	 * NamespaceDecl
	 */

	class NamespaceDecl : public AstNode
	{
	public:
		NamespaceDecl(U32 line, U32 column);
		virtual ~NamespaceDecl();

		virtual Bool
		hasChildren() final;

		virtual std::vector<AstNode*>
		getChildren() final;

		NamespaceDeclPtrList				namespaceDeclList;
		GeneralStmtDeclPtrList				generalDeclList;
	};

	/**
	 * GeneralStmtDecl
	 */

	class GeneralStmtDecl : public AstNode
	{
	protected:
		GeneralStmtDecl(AstNodeType_e nodeType, GeneralDeclType_e type, U32 line, U32 column);

	public:
		virtual	~GeneralStmtDecl();

		const GeneralDeclType_e				type;

	};

	/**
	 * ClassDecl
	 */

	class ClassDecl : public GeneralStmtDecl
	{
	public:
		ClassDecl(U32 line, U32 column);
		virtual ~ClassDecl();

		virtual Bool
		hasChildren() final;

		virtual std::vector<AstNode*>
		getChildren() final;

		Bool								isExported;
		Bool								isAbstract;
		GenericDeclPtr						genericDecl;
		TypeDeclPtr							baseClass;
		TypeDeclPtrList						interfaceList;
		ClassFunctionDeclPtrList			functionList;
		ClassVariableDeclPtrList			variableList;
		ClassConstructorDeclPtrList			constructorList;
		ClassDestructorDeclPtr				destructorDecl;
	};

	/**
	 * ClassMemberDecl
	 */

	class ClassMemberDecl : public AstNode
	{
	protected:
		ClassMemberDecl(AstNodeType_e nodeType, ClassMemberType_e type, U32 line, U32 column);

	public:
		virtual ~ClassMemberDecl();

		const ClassMemberType_e				type;
		ClassMemberAccessModifier_e			accessModifier;
	};

	/**
	 * ClassFunctionDecl
	 */

	class ClassFunctionDecl : public ClassMemberDecl
	{
	public:
		ClassFunctionDecl(U32 line, U32 column);
		virtual ~ClassFunctionDecl();

		virtual Bool
		hasChildren() final;

		virtual std::vector<AstNode*>
		getChildren() final;

		Bool								isStatic;
		Bool								isVirtual;
		Bool								isAbstract;
		Bool								isOverride;
		Bool								isFinal;

		GenericDeclPtr						genericDecl;

		TypeDeclPtr							sourceTypeDecl;

		FunctionParameterDeclPtrList		parameterList;
		TypeDeclPtr							returnType;

		BlockDeclPtr						blockDecl;
		ExpressionDeclPtr					exprDecl;
	};

	/**
	 * ClassVariableDecl
	 */

	class ClassVariableDecl : public ClassMemberDecl
	{
	public:
		ClassVariableDecl(U32 line, U32 column);
		virtual ~ClassVariableDecl();

		Bool								isShared;
		Bool								isUnique;
		Bool								isConst;
		Bool								isReference;
		Bool								isStatic;
		TypeDeclPtr							typeDecl;
		ExpressionDeclPtr					initExpr;
	};

	/**
	 * ClassConstructorDecl
	 */

	class ClassConstructorDecl : public ClassMemberDecl
	{
	public:
		ClassConstructorDecl(U32 line, U32 column);
		virtual ~ClassConstructorDecl();

		virtual Bool
		hasChildren() final;

		virtual std::vector<AstNode*>
		getChildren() final;

		FunctionParameterDeclPtrList		parameterList;
		ExpressionDeclPtr					superInitExpr;
		ClassVariableInitDeclPtrList		variableInitDeclList;
		BlockDeclPtr						blockDecl;
	};

	/**
	 * ClassDestructorDecl
	 */

	class ClassDestructorDecl : public ClassMemberDecl
	{
	public:
		ClassDestructorDecl(U32 line, U32 column);
		virtual ~ClassDestructorDecl();

		BlockDeclPtr						blockDecl;
	};

	/**
	 * ClassVariableInitDecl
	 */

	class ClassVariableInitDecl : public AstNode
	{
	public:
		ClassVariableInitDecl(U32 line, U32 column);
		virtual ~ClassVariableInitDecl();

		ExpressionDeclPtr					initExpr;
	};

	/**
	 * InterfaceDecl
	 */

	class InterfaceDecl : public GeneralStmtDecl
	{
	public:
		InterfaceDecl(U32 line, U32 column);
		virtual ~InterfaceDecl();

		virtual Bool
		hasChildren() final;

		virtual std::vector<AstNode*>
		getChildren() final;

		Bool								isExported;
		GenericDeclPtr						genericDecl;
		InterfaceFunctionDeclPtrList		functionDeclList;
	};

	/**
	 * InterfaceFunctionDecl
	 */

	class InterfaceFunctionDecl : public AstNode
	{
	public:
		InterfaceFunctionDecl(U32 line, U32 column);
		virtual ~InterfaceFunctionDecl();

		virtual Bool
		hasChildren() final;

		virtual std::vector<AstNode*>
		getChildren() final;

		GenericDeclPtr						genericDecl;
		FunctionParameterDeclPtrList		parameterList;
		TypeDeclPtr							returnType;
	};

	/**
	 * StructDecl
	 */

	class StructDecl : public GeneralStmtDecl
	{
	public:
		StructDecl(U32 line, U32 column);
		virtual ~StructDecl();

		virtual Bool
		hasChildren() final;

		virtual std::vector<AstNode*>
		getChildren() final;

		Bool								isExported;
		GenericDeclPtr						genericDecl;
		StructVariableDeclPtrList			variableList;
	};

	/**
	 * StructVariableDecl
	 */

	class StructVariableDecl : public AstNode
	{
	public:
		StructVariableDecl(U32 line, U32 column);
		virtual ~StructVariableDecl();

		Bool								isShared;
		Bool								isUnique;
		Bool								isConst;
		Bool								isReference;
		TypeDeclPtr							typeDecl;
		ExpressionDeclPtr					initExpr;
	};

	/**
	 * EnumDecl
	 */

	class EnumDecl : public GeneralStmtDecl
	{
	public:
		EnumDecl(U32 line, U32 column);
		virtual ~EnumDecl();

		virtual Bool
		hasChildren() final;

		virtual std::vector<AstNode*>
		getChildren() final;

		Bool								isExported;
		GenericDeclPtr						genericDecl;
		EnumItemDeclPtrList					enumItemDeclList;
	};

	/**
	 * EnumItemDecl 
	 */

	class EnumItemDecl : public AstNode
	{
	public:
		EnumItemDecl(U32 line, U32 column);
		virtual ~EnumItemDecl();

		Bool								hasData;
		Bool								hasValue;
		TypeDeclPtrList						dataTypeDeclList;
		ExpressionDeclPtr					valueExpression;
	};

	/**
	 * TraitDecl
	 */

	class TraitDecl : public GeneralStmtDecl
	{
	public:
		TraitDecl(U32 line, U32 column);
		virtual ~TraitDecl();

		virtual Bool
		hasChildren() final;

		virtual std::vector<AstNode*>
		getChildren() final;

		Bool								isExported;
		Bool								isDefinition;
		TypeDeclPtr							typeDefinitionDecl;
		GenericDeclPtr						genericDecl;
		TraitFunctionDeclPtrList			functionDeclList;
	};

	/**
	 * TraitFunctionDecl
	 */

	class TraitFunctionDecl : public AstNode
	{
	public:
		TraitFunctionDecl(U32 line, U32 column);
		virtual ~TraitFunctionDecl();

		virtual Bool
		hasChildren() final;

		virtual std::vector<AstNode*>
		getChildren() final;

		GenericDeclPtr						genericDecl;
		FunctionParameterDeclPtrList		parameterList;
		TypeDeclPtr							returnType;
		BlockDeclPtr						blockDecl;
		ExpressionDeclPtr					exprDecl;
		Bool								isStatic;
	};

	/**
	 * FunctionDecl
	 */

	class FunctionDecl : public GeneralStmtDecl
	{
	public:
		FunctionDecl(U32 line, U32 column);
		virtual ~FunctionDecl();

		virtual Bool
		hasChildren() final;

		virtual std::vector<AstNode*>
		getChildren() final;

		Bool								isExported;
		GenericDeclPtr						genericDecl;
		FunctionParameterDeclPtrList		parameterList;
		TypeDeclPtr							returnType;
		BlockDeclPtr						blockDecl;
		ExpressionDeclPtr					exprDecl;
	};

	/**
	 * VariableDecl
	 */

	class VariableDecl : public GeneralStmtDecl
	{
	public:
		VariableDecl(U32 line, U32 column);
		virtual ~VariableDecl();

		Bool								isExported;
		Bool								isShared;
		Bool								isUnique;
		Bool								isConst;
		Bool								isReference;
		TypeDeclPtr							typeDecl;
		ExpressionDeclPtr					initExpr;
	};

	/**
	 * FunctionParameterDecl
	 */

	class FunctionParameterDecl : public AstNode
	{
	public:
		FunctionParameterDecl(U32 line, U32 column);
		virtual ~FunctionParameterDecl();

		virtual Bool
		hasChildren() final;

		virtual std::vector<AstNode*>
		getChildren() final;

		Bool								isShared;
		Bool								isUnique;
		Bool								isReference;
		Bool								isEllipsis;
		TypeDeclPtr							typeDecl;
		PatternDeclPtr						patternDecl;
	};

	/**
	 * GenericDecl
	 */

	class GenericDecl : public AstNode
	{
	public:
		GenericDecl(U32 line, U32 column);
		virtual ~GenericDecl();

		virtual Bool
		hasChildren() final;

		virtual std::vector<AstNode*>
		getChildren() final;

		GenericItemDeclPtrList				genericDeclItemList;
	};

	/**
	 * GenericItemDecl
	 */

	class GenericItemDecl : public AstNode
	{
	public:
		GenericItemDecl(U32 line, U32 column);
		virtual ~GenericItemDecl();

		virtual Bool
		hasChildren() final;

		virtual std::vector<AstNode*>
		getChildren() final;

		TypeDeclPtrList						whereTypeList;
	};

	/**
	 * ScopedIdentifierDecl
	 */

	class ScopedIdentifierDecl : public AstNode
	{
	public:
		ScopedIdentifierDecl(U32 line, U32 column);
		virtual ~ScopedIdentifierDecl();

		ScopedIdentifierDeclPtr				referencedIdentifier;
		Bool								startFromRoot;
	};
} }
