#pragma once
#include "fl_defs.h"
#include "fl_collections.h"
#include "ast\fl_ast_decl.h"
#include "attributes\fl_attribute.h"
namespace fluffy { namespace attributes {
	/**
	 * ImplementedTraitList
	 */

	class ImplementedTraitList : public AttributeTemplate<AttributeType_e::ImplementedTraitList>
	{
	public:
		ImplementedTraitList();
		virtual ~ImplementedTraitList();

		void
		insertTrait(const TString& identifier, ast::TraitDecl* const traitFor);

		ast::TraitDecl* const
		findTrait(const TString& identifier);

	private:
		NodeMultiMap
		mImplementedTraitList;
	};

	/**
	 * ImplementedTraitForList
	 */

	class ImplementedTraitForList : public AttributeTemplate<AttributeType_e::ImplementedTraitForList>
	{
	public:
		ImplementedTraitForList();
		virtual ~ImplementedTraitForList();

		void
		insertTraitFor(ast::TraitForDecl* const traitFor);

		Bool
		findTraitFor(ast::TraitForDecl* const findedTraitFor);

	private:
		NodeList
		mImplementedTraitForList;
	};
} }