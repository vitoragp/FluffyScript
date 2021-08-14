#include "../../fl_ast.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectClassDecl
	 */

	ClassDeclPtr ParserObjectClassDecl::parse(Parser* parser, Bool hasExport, Bool hasAbstract)
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
			classDecl->genericTemplateList = ParserObjectGenericTemplateDecl::parse(parser);
		}

		// Verifica se a declaracao de extends.
		if (parser->isExtends())
		{
			classDecl->baseClass = ParserObjectClassExtendsDecl::parse(parser);
		}

		// Verifica se a declaracao de interfaces.
		if (parser->isImplements())
		{
			classDecl->interfaceList = ParserObjectClassImplementsDecl::parse(parser);
		}

		// Consome '{'.
		parser->expectToken([parser]() { return parser->isLeftBracket(); });

		// Consome '}'.
		parser->expectToken([parser]() { return parser->isRightBracket(); });

		return classDecl;
	}
} }