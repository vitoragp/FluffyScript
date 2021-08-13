#include "fl_ast.h"
namespace fluffy { namespace ast {
	/**
	 * Program
	 */

	Program::Program()
	{}

	Program::~Program()
	{}

	/**
	 * Include
	 */

	Include::Include()
		: allFlag(false)
	{}

	Include::~Include()
	{}

	/**
	 * Namespace
	 */

	Namespace::Namespace()
	{}

	Namespace::~Namespace()
	{}

	/**
	 * GeneralStmt
	 */

	GeneralStmt::GeneralStmt(GeneralStmtType const type)
		: m_type(type)
	{}

	GeneralStmt::~GeneralStmt()
	{}

	GeneralStmt::GeneralStmtType GeneralStmt::getType()
	{
		return m_type;
	}

	/**
	 * ClassDecl
	 */

	ClassDecl::ClassDecl()
		: GeneralStmt(GeneralStmt::eGST_ClassDecl)
		, isExported(false)
	{}

	ClassDecl::~ClassDecl()
	{}

	/**
	 * GenericTemplateDecl
	 */

	GenericTemplateDecl::GenericTemplateDecl()
	{}

	GenericTemplateDecl::~GenericTemplateDecl()
	{}

	/**
	 * NamedDecl
	 */

	NamedDecl::NamedDecl()
	{}

	NamedDecl::~NamedDecl()
	{}
} }