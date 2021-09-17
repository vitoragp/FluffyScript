#include "ast\fl_ast_decl.h"
#include "attributes\fl_reference.h"
#include "attributes\fl_implemented_trait_list.h"
#include "utils\fl_polymorphic_utils.h"

namespace fluffy { namespace ast {
	class AstNode;
} }

namespace fluffy { namespace utils {
	/**
	 * PolymorphicUtils
	 */

	Bool
	PolymorphicUtils::canBe(ast::AstNode* nodeA, ast::AstNode* nodeB)
	{
		if (nodeA == nodeB)
		{
			return true;
		}
		else if (nodeA->nodeType == AstNodeType_e::ClassDecl)
		{
			auto classDeclA = nodeA->to<ast::ClassDecl>();

			if (nodeB->nodeType == AstNodeType_e::ClassDecl)
			{
				auto classDeclB = nodeB->to<ast::ClassDecl>();

				if (classDeclA->baseClass)
				{
					auto referenceBaseClass = classDeclA->baseClass->getAttribute<attributes::Reference>();

					if (canBe(referenceBaseClass->get(), nodeB))
					{
						return true;
					}
				}
			}
			else if (nodeB->nodeType == AstNodeType_e::InterfaceDecl)
			{
				for (auto& interfaceOfA : classDeclA->interfaceList)
				{
					auto referenceInterface = interfaceOfA->getAttribute<attributes::Reference>();

					if (referenceInterface->get() == nodeB)
					{
						return true;
					}
				}
				if (classDeclA->baseClass)
				{
					auto referenceBaseClass = classDeclA->baseClass->getAttribute<attributes::Reference>();

					if (canBe(referenceBaseClass->get(), nodeB))
					{
						return true;
					}
				}
			}
			else if (nodeB->nodeType == AstNodeType_e::TraitDecl)
			{
				auto implementedTraitList = classDeclA->getAttribute<attributes::ImplementedTraitList>();
				
				if (implementedTraitList != nullptr)
				{
					auto traitRef = implementedTraitList->findTrait(nodeB->identifier);

					if (traitRef == nodeB)
					{
						return true;
					}
				}
				if (classDeclA->baseClass)
				{
					auto referenceBaseClass = classDeclA->baseClass->getAttribute<attributes::Reference>();

					if (canBe(referenceBaseClass->get(), nodeB))
					{
						return true;
					}
				}
			}
		}
		return false;
	}
} }