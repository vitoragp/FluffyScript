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
		insertTrait(const TString& identifier, ast::TraitForDecl* const traitFor);

		ast::TraitForDecl* const
		findTrait(const TString& identifier);

	private:
		NodeMultiMap
		mImplementedTraitList;
	};
} }