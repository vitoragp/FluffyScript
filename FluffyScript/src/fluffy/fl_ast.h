#pragma once
#include <memory>
#include <vector>
#include "fl_defs.h"
namespace fluffy {
	namespace ast {
		using std::vector;
		using std::unique_ptr;

		using StringList					= vector<String>;

		using IncludeDeclPtr				= unique_ptr<class IncludeDecl>;
		using IncludeDeclPtrList			= vector<IncludeDeclPtr>;

		using NamespaceDeclPtr				= unique_ptr<class NamespaceDecl>;
		using NamespaceDeclPtrList			= vector<NamespaceDeclPtr>;

		using GeneralStmtPtr				= unique_ptr<class GeneralStmt>;
		using GeneralStmtPtrList			= vector<GeneralStmtPtr>;

		using ScopedIdentifierDeclPtr		= unique_ptr<class ScopedIdentifierDecl>;
		using ScopedIdentifierDeclPtrList	= vector<ScopedIdentifierDeclPtr>;

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
			IncludeDecl() : allFlag(false) {}
			~IncludeDecl() {}

			StringList						includedItemList;
			Bool							allFlag;
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
		public:
			enum class GeneralStmtType
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
			GeneralStmt(GeneralStmtType const type)
				: m_type(type)
			{}

		public:
			virtual	~GeneralStmt()
			{}

		GeneralStmtType						getType();

	private:
		GeneralStmtType						m_type;
	};

	/**
	 * ClassDecl
	 */

	class ClassDecl : public GeneralStmt
	{
	public:
		ClassDecl()
			: GeneralStmt(GeneralStmt::GeneralStmtType::eGST_ClassDecl)
			, isExported(false)
			, isAbstract(false)
		{}

		virtual ~ClassDecl()
		{}

		Bool								isExported;
		Bool								isAbstract;
		String								name;
		StringList							genericTemplateList;
		ScopedIdentifierDeclPtr				baseClass;
		ScopedIdentifierDeclPtrList			interfaceList;
		// StaticVariableList				staticVariableList;
		// VariableList						variableList;
		// StaticFunctionList				staticFunctionList;
		// FunctionList						functionList;
		// ConstructorList					constructorList;
		// DestructorDecl					destructorDecl;
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
		StringList							genericTemplateList;
		ScopedIdentifierDeclPtr				tailIdentifier;
		Bool								startFromRoot;
	};
} }
