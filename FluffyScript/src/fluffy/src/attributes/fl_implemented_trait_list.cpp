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
	ImplementedTraitList::insertTrait(const TString& identifier, ast::TraitDecl* const traitFor)
	{
		mImplementedTraitList.emplace(identifier, traitFor);
	}

	ast::TraitDecl* const
	ImplementedTraitList::findTrait(const TString& identifier)
	{
		auto it = mImplementedTraitList.find(identifier);
		return it != mImplementedTraitList.end() ? ast::safe_cast<ast::TraitDecl>(it->second) : nullptr;
	}

	/**
	 * ImplementedTraitForList
	 */

	ImplementedTraitForList::ImplementedTraitForList()
	{}

	ImplementedTraitForList::~ImplementedTraitForList()
	{}

	void
	ImplementedTraitForList::insertTraitFor(ast::TraitForDecl* const traitFor)
	{
		mImplementedTraitForList.emplace_back(traitFor);
	}

	Bool
	ImplementedTraitForList::findTraitFor(ast::TraitForDecl* const findedTraitFor)
	{
		for (auto& it : mImplementedTraitForList)
		{
			auto traitFor = it->to<ast::TraitForDecl>();

			if (findedTraitFor == traitFor)
			{
				return true;
			}
		}
		return false;
	}
} }