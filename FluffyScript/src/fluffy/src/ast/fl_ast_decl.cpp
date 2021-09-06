#include <memory>
#include <vector>
#include "ast\fl_ast_decl.h"
#include "ast\fl_ast_expr.h"
#include "ast\fl_ast_type.h"
#include "ast\fl_ast_stmt.h"
#include "fl_defs.h"

namespace fluffy { namespace ast {
	/**
	 * CodeUnit
	 */

	CodeUnit::CodeUnit(const String& name)
		: AstNode(AstNodeType_e::CodeUnit, 0, 0)
		, name(name)
	{}

	CodeUnit::~CodeUnit()
	{}

	Bool
	CodeUnit::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	CodeUnit::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->namespaceDeclList)
		{
			children.push_back(it.get());
		}
		return children;
	}

	/**
	 * Include
	 */

	IncludeDecl::IncludeDecl(U32 line, U32 column)
		: AstNode(AstNodeType_e::Include, line, column)
	{}

	IncludeDecl::~IncludeDecl()
	{}

	/**
	 * NamespaceDecl
	 */

	NamespaceDecl::NamespaceDecl(U32 line, U32 column)
		: AstNodeIdentified(AstNodeType_e::Namespace, line, column)
	{}

	NamespaceDecl::~NamespaceDecl()
	{}

	Bool
	NamespaceDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	NamespaceDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->namespaceDeclList)
		{
			children.push_back(it.get());
		}
		for (auto& it : this->generalDeclList)
		{
			children.push_back(it.get());
		}
		return children;
	}

	/**
	 * GeneralStmtDecl
	 */

	GeneralStmtDecl::GeneralStmtDecl(AstNodeType_e nodeType, GeneralDeclType_e type, U32 line, U32 column)
		: AstNodeIdentified(nodeType, line, column)
		, type(type)
	{}

	GeneralStmtDecl::~GeneralStmtDecl()
	{}

	/**
	 * ClassDecl
	 */

	ClassDecl::ClassDecl(U32 line, U32 column)
		: GeneralStmtDecl(AstNodeType_e::Class, GeneralDeclType_e::ClassDecl, line, column)
		, isExported(false)
		, isAbstract(false)
	{}

	ClassDecl::~ClassDecl()
	{}

	Bool
	ClassDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	ClassDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->functionList)
		{
			children.push_back(it.get());
		}
		for (auto& it : this->variableList)
		{
			children.push_back(it.get());
		}
		for (auto& it : this->constructorList)
		{
			children.push_back(it.get());
		}
		if (this->destructorDecl) {
			children.push_back(this->destructorDecl.get());
		}
		return children;
	}

	/**
	 * ClassMemberDecl
	 */

	ClassMemberDecl::ClassMemberDecl(AstNodeType_e nodeType, ClassMemberType_e type, U32 line, U32 column)
		: AstNodeIdentified(nodeType, line, column)
		, type(type)
		, accessModifier(ClassMemberAccessModifier_e::Unknown)

	{}

	ClassMemberDecl::~ClassMemberDecl()
	{}

	/**
	 * ClassFunctionDecl
	 */

	ClassFunctionDecl::ClassFunctionDecl(U32 line, U32 column)
		: ClassMemberDecl(AstNodeType_e::ClassFunction, ClassMemberType_e::Function, line, column)
		, isStatic(false)
		, isVirtual(false)
		, isAbstract(false)
		, isOverride(false)
		, isFinal(false)
	{}

	ClassFunctionDecl::~ClassFunctionDecl()
	{}

	Bool
	ClassFunctionDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	ClassFunctionDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->parameterList)
		{
			children.push_back(it.get());
		}
		std::vector<AstNode*> blockChildren = blockDecl->getChildren();
		children.insert(children.end(), blockChildren.begin(), blockChildren.end());
		return children;
	}

	/**
	 * ClassVariableDecl
	 */

	ClassVariableDecl::ClassVariableDecl(U32 line, U32 column)
		: ClassMemberDecl(AstNodeType_e::ClassVariable, ClassMemberType_e::Variable, line, column)
		, isShared(false)
		, isUnique(false)
		, isConst(false)
		, isReference(false)
		, isStatic(false)
	{}

	ClassVariableDecl::~ClassVariableDecl()
	{}

	/**
	 * ClassConstructorDecl
	 */

	ClassConstructorDecl::ClassConstructorDecl(U32 line, U32 column)
		: ClassMemberDecl(AstNodeType_e::ClassConstructor, ClassMemberType_e::Constructor, line, column)
	{}

	ClassConstructorDecl::~ClassConstructorDecl()
	{}

	Bool
	ClassConstructorDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	ClassConstructorDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->parameterList)
		{
			children.push_back(it.get());
		}
		std::vector<AstNode*> blockChildren = blockDecl->getChildren();
		children.insert(children.end(), blockChildren.begin(), blockChildren.end());
		return children;
	}

	/**
	 * ClassDestructorDecl
	 */

	ClassDestructorDecl::ClassDestructorDecl(U32 line, U32 column)
		: ClassMemberDecl(AstNodeType_e::ClassDestructor, ClassMemberType_e::Destructor, line, column)
	{}

	ClassDestructorDecl::~ClassDestructorDecl()
	{}

	/**
	 * ClassVariableInitDecl
	 */

	ClassVariableInitDecl::ClassVariableInitDecl(U32 line, U32 column)
		: AstNodeIdentified(AstNodeType_e::ClassVariableInitDecl, line, column)
	{}

	ClassVariableInitDecl::~ClassVariableInitDecl()
	{}

	/**
	 * InterfaceDecl
	 */

	InterfaceDecl::InterfaceDecl(U32 line, U32 column)
		: GeneralStmtDecl(AstNodeType_e::Interface, GeneralDeclType_e::InterfaceDecl, line, column)
		, isExported(false)
	{}

	InterfaceDecl::~InterfaceDecl()
	{}

	Bool
	InterfaceDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	InterfaceDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->functionDeclList)
		{
			children.push_back(it.get());
		}
		return children;
	}

	/**
	 * InterfaceFunctionDecl
	 */

	InterfaceFunctionDecl::InterfaceFunctionDecl(U32 line, U32 column)
		: AstNodeIdentified(AstNodeType_e::InterfaceFunction, line, column)
	{}

	InterfaceFunctionDecl::~InterfaceFunctionDecl()
	{}

	Bool
	InterfaceFunctionDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	InterfaceFunctionDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->parameterList)
		{
			children.push_back(it.get());
		}
		return children;
	}

	/**
	 * StructDecl
	 */

	StructDecl::StructDecl(U32 line, U32 column)
		: GeneralStmtDecl(AstNodeType_e::Struct, GeneralDeclType_e::StructDecl, line, column)
		, isExported(false)
	{}

	StructDecl::~StructDecl()
	{}

	Bool
	StructDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	StructDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->variableList)
		{
			children.push_back(it.get());
		}
		return children;
	}

	/**
	 * StructVariableDecl
	 */

	StructVariableDecl::StructVariableDecl(U32 line, U32 column)
		: AstNodeIdentified(AstNodeType_e::StructVariable, line, column)
		, isShared(false)
		, isUnique(false)
		, isConst(false)
		, isReference(false)
	{}

	StructVariableDecl::~StructVariableDecl()
	{}

	/**
	 * EnumDecl
	 */

	EnumDecl::EnumDecl(U32 line, U32 column)
		: GeneralStmtDecl(AstNodeType_e::Enum, GeneralDeclType_e::EnumDecl, line, column)
		, isExported(false)
	{}

	EnumDecl::~EnumDecl()
	{}

	Bool
	EnumDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	EnumDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->enumItemDeclList)
		{
			children.push_back(it.get());
		}
		return children;
	}

	/**
	 * EnumItemDecl 
	 */

	EnumItemDecl::EnumItemDecl(U32 line, U32 column)
		: AstNodeIdentified(AstNodeType_e::EnumItem, line, column)
		, hasData(false)
		, hasValue(false)
	{}

	EnumItemDecl::~EnumItemDecl()
	{}

	/**
	 * TraitDecl
	 */

	TraitDecl::TraitDecl(U32 line, U32 column)
		: GeneralStmtDecl(AstNodeType_e::Trait, GeneralDeclType_e::TraitDecl, line, column)
		, isExported(false)
		, isDefinition(false)
	{}

	TraitDecl::~TraitDecl()
	{}

	Bool
	TraitDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	TraitDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->functionDeclList)
		{
			children.push_back(it.get());
		}
		return children;
	}

	/**
	 * TraitFunctionDecl
	 */

	TraitFunctionDecl::TraitFunctionDecl(U32 line, U32 column)
		: AstNodeIdentified(AstNodeType_e::TraitFunction, line, column)
		, isStatic(true)
	{}

	TraitFunctionDecl::~TraitFunctionDecl()
	{}

	Bool
	TraitFunctionDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	TraitFunctionDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->parameterList)
		{
			children.push_back(it.get());
		}
		std::vector<AstNode*> blockChildren = blockDecl->getChildren();
		children.insert(children.end(), blockChildren.begin(), blockChildren.end());
		return children;
	}

	/**
	 * FunctionDecl
	 */

	FunctionDecl::FunctionDecl(U32 line, U32 column)
		: GeneralStmtDecl(AstNodeType_e::Function, GeneralDeclType_e::FunctionDecl, line, column)
		, isExported(false)
	{}

	FunctionDecl::~FunctionDecl()
	{}

	Bool
	FunctionDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	FunctionDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->parameterList)
		{
			children.push_back(it.get());
		}
		std::vector<AstNode*> blockChildren = blockDecl->getChildren();
		children.insert(children.end(), blockChildren.begin(), blockChildren.end());
		return children;
	}

	/**
	 * VariableDecl
	 */

	VariableDecl::VariableDecl(U32 line, U32 column)
		: GeneralStmtDecl(AstNodeType_e::Variable, GeneralDeclType_e::VariableDecl, line, column)
		, isExported(false)
		, isShared(false)
		, isUnique(false)
		, isConst(false)
		, isReference(false)
	{}

	VariableDecl::~VariableDecl()
	{}

	/**
	 * FunctionParameterDecl
	 */

	FunctionParameterDecl::FunctionParameterDecl(U32 line, U32 column)
		: AstNodeIdentified(AstNodeType_e::FunctionParameter, line, column)
		, isShared(false)
		, isUnique(false)
		, isReference(false)
		, isEllipsis(false)
	{}

	FunctionParameterDecl::~FunctionParameterDecl()
	{}

	Bool
	FunctionParameterDecl::hasChildren()
	{
		return patternDecl != nullptr;
	}

	std::vector<AstNode*>
	FunctionParameterDecl::getChildren()
	{
		if (!this->patternDecl) {
			return std::vector<AstNode*>();
		}
		return patternDecl->getChildren();
	}

	/**
	 * GenericDecl
	 */

	GenericDecl::GenericDecl(U32 line, U32 column)
		: AstNode(AstNodeType_e::Generic, line, column)
	{}

	GenericDecl::~GenericDecl()
	{}

	Bool
	GenericDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	GenericDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->genericDeclItemList)
		{
			children.push_back(it.get());
		}
		return children;
	}

	/**
	 * GenericItemDecl
	 */

	GenericItemDecl::GenericItemDecl(U32 line, U32 column)
		: AstNodeIdentified(AstNodeType_e::GenericItem, line, column)
	{}

	GenericItemDecl::~GenericItemDecl()
	{}

	Bool
	GenericItemDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	GenericItemDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->whereTypeList)
		{
			children.push_back(it.get());
		}
		return children;
	}

	/**
	 * ScopedIdentifierDecl
	 */

	ScopedIdentifierDecl::ScopedIdentifierDecl(U32 line, U32 column)
		: AstNodeIdentified(AstNodeType_e::ScopedIdentifier, line, column)
		, startFromRoot(false)
	{}

	ScopedIdentifierDecl::~ScopedIdentifierDecl()
	{}
} }
