#pragma once
#include <memory>
#include <vector>
#include "fl_defs.h"
namespace fluffy { namespace ast {
	using std::vector;
	using std::unique_ptr;

	using StringList						= vector<String>;

	using IncludePtr						= unique_ptr<class Include>;
	using IncludePtrList					= vector<IncludePtr>;

	using NamespacePtr						= unique_ptr<class Namespace>;
	using NamespacePtrList					= vector<NamespacePtr>;

	using GeneralStmtPtr					= unique_ptr<class GeneralStmt>;
	using GeneralStmtPtrList				= vector<GeneralStmtPtr>;

	using GenericTemplateDeclPtr			= unique_ptr<class GenericTemplateDecl>;
	using GenericTemplateDeclPtrList		= vector<GenericTemplateDeclPtr>;

	using ScopedIdentifierDeclPtr			= unique_ptr<class ScopedIdentifierDecl>;
	using ScopedIdentifierDeclPtrList		= vector<ScopedIdentifierDeclPtr>;

	/**
	 * Program
	 */

	class Program
	{
	public:
								Program();
								~Program();

		IncludePtrList			includeList;
		NamespacePtrList		namespaceList;
	};

	/**
	 * Include
	 */

	class Include
	{
	public:
								Include();
								~Include();

		StringList				includedItemList;
		Bool					allFlag;
		ScopedIdentifierDeclPtr fromNamespace;
	};

	/**
	 * Namespace
	 */

	class Namespace
	{
	public:
								Namespace();
								~Namespace();

		String					name;
		NamespacePtrList		namespaceList;
		GeneralStmtPtrList		generalDeclList;
	};

	/**
	 * GeneralStmt
	 */

	class GeneralStmt
	{
	public:
		enum GeneralStmtType
		{
			eGST_Unknown,
			eGST_ClassDecl,
			eGST_InterfaceDecl,
			eGST_StructDecl,
			eGST_EnumDecl,
			eGST_TraitDecl,
			eGST_VariableDecl,
			eGST_FunctionDecl
		};

	protected:
								GeneralStmt(GeneralStmtType const type);

	public:
		virtual					~GeneralStmt();

		GeneralStmtType			getType();

	private:
		GeneralStmtType			m_type;
	};

	/**
	 * ClassDecl
	 */

	class ClassDecl : public GeneralStmt
	{
	public:
								ClassDecl();
		virtual					~ClassDecl();

		Bool					isExported;
		Bool					isAbstract;
		String					name;
		GenericTemplateDeclPtr	genericTemplateDecl;
		// ImplementsDecl		implementsDecl;
		// ExtendsDecl			extendsDecl;
		// StaticVariableList	staticVariableList;
		// VariableList			variableList;
		// StaticFunctionList	staticFunctionList;
		// FunctionList			functionList;
		// ConstructorList		constructorList;
		// DestructorDecl		destructorDecl;
	};

	/**
	 * GenericTemplateDecl
	 */

	class GenericTemplateDecl
	{
	public:
								GenericTemplateDecl();
								~GenericTemplateDecl();

		StringList				templateItemList;
	};

	/**
	 * ScopedIdentifierDecl
	 */

	class ScopedIdentifierDecl
	{
	public:
								ScopedIdentifierDecl();
								~ScopedIdentifierDecl();

		String					identifier;
		ScopedIdentifierDeclPtr	tailDecl;
	};
} }
