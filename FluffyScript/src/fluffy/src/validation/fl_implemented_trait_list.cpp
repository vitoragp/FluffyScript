#include "attributes\fl_implemented_trait_list.h"
namespace fluffy { namespace attributes {
	/**
	 * ImplementedTraitList
	 */

	ImplementedTraitList::ImplementedTraitList()
	{}

	ImplementedTraitList::~ImplementedTraitList()
	{}

	void
	ImplementedTraitList::insertTrait(const TString& identifier, ast::TraitForDecl* const traitFor)
	{
		mImplementedTraitList.emplace(identifier, traitFor);
	}

	ast::TraitForDecl* const
	ImplementedTraitList::findTrait(const TString& identifier)
	{
		auto it = mImplementedTraitList.find(identifier);
		return it != mImplementedTraitList.end() ? ast::safe_cast<ast::TraitForDecl>(it->second) : nullptr;
	}
} }