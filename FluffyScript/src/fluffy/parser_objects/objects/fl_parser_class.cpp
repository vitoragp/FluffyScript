#include "../../fl_ast.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectClass
	 */

	ClassDeclPtr ParserObjectClass::parse(Parser* parser, Bool hasExport, Bool hasAbstract)
	{
		auto classDecl = std::make_unique<ast::ClassDecl>();

		classDecl->isExported = hasExport;
		classDecl->isAbstract = hasAbstract;

		// Consome 'class'.
		parser->expectToken([parser]() { return parser->isClass(); });

		// Consome o nome da classe.
		classDecl->name = parser->expectIdentifier();

		// Verifica se a declaracao de generic.
		if (parser->isLessThan())
		{
			throw exceptions::not_implemented_feature_exception("generic template parser");
		}

		// Verifica se a declaracao de extends.
		if (parser->isExtends())
		{
			throw exceptions::not_implemented_feature_exception("class extends declaration parser");
		}

		// Verifica se a declaracao de generic.
		if (parser->isImplements())
		{
			throw exceptions::not_implemented_feature_exception("class implements declarion parser");
		}

		throw exceptions::not_implemented_feature_exception("class block declaration");

		return classDecl;
	}
} }
