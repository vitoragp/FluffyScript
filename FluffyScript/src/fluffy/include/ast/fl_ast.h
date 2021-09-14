#pragma once
#include "fl_string.h"
#include "fl_defs.h"
#include "fl_exceptions.h"
#include "fl_collections.h"
#include "attributes\fl_attribute.h"

namespace fluffy { namespace ast {

	/**
	 * AstNode
	 */

	class AstNode
	{
	protected:
		AstNode(const AstNodeType_e	nodeType, U32 line, U32 column);

	public:
		virtual ~AstNode();

		virtual Bool
		hasChildren();

		virtual std::vector<AstNode*>
		getChildren();

		virtual const I8*
		getIdentifier();

		template <typename TAttribute>
		TAttribute* const
		getAttribute();

		template <typename TAttribute, typename ...TArgs>
		TAttribute* const
		getOrCreateAttribute(TArgs...args);

		void
		insertAttribute(attributes::Attribute* const attribute);

		AttributeMap
		attributeMap;

		const AstNodeType_e
		nodeType;

		TString
		identifier;

		U32
		line;

		U32
		column;
	};

	template <AstNodeType_e Type>
	class AstSafeCast
	{
	public:
		static AstNodeType_e
		getReferenceType()
		{
			return Type;
		}
	};

	/**
	 * Safe astNode cast
	 */

	template <typename T>
	T* safe_cast(ast::AstNode* const node)
	{
		if (node == nullptr || node->nodeType != T::getReferenceType())
		{
			throw exceptions::custom_exception("Invalid AstNode cast");
		}
		return reinterpret_cast<T*>(node);
	}

	/**
	 * Funcoes auxiliares
	 */

	template <
		typename TList,
		typename TFunc,
		typename ...TArgs
	> void
	for_each(TList& list, TFunc func, TArgs...args)
	{
		auto itBegin = list.begin();
		auto itEnd = list.end();

		while (itBegin != itEnd)
		{
			func(itBegin->get(), args...);			
			itBegin++;
		}
	}

	template <
		typename TClass,
		typename TList,
		typename TFunc,
		typename ...TArgs
	> void
	for_each(TClass owner, TFunc func, TList& list, TArgs...args)
	{
		auto itBegin = list.begin();
		auto itEnd = list.end();

		while (itBegin != itEnd)
		{
			(owner->* func)(itBegin->get(), args...);
			itBegin++;
		}
	}

	template <
		typename TUnique,
		typename TFunc,
		typename ...TArgs
	> void
	for_single(TUnique& unique, TFunc func, TArgs...args)
	{
		if (unique != nullptr) {
			func(unique.get(), args...);
		}
	}

	template <
		typename TClass,
		typename TUnique,
		typename TFunc,
		typename ...TArgs
	> void
	for_single(TClass owner, TFunc func, TUnique& unique, TArgs...args)
	{
		if (unique != nullptr) {
			(owner->* func)(unique.get(), args...);
		}
	}

	/**
	 * AstNode Impl
	 */

	template <typename TAttribute>
	TAttribute* const
	AstNode::getAttribute()
	{
		auto it = attributeMap.find(TAttribute::getAttributeTypeKey());
		return it != attributeMap.end() ? reinterpret_cast<TAttribute* const>(it->second.get()) : nullptr;
	}

	template <typename TAttribute, typename ...TArgs>
	TAttribute* const
	AstNode::getOrCreateAttribute(TArgs...args)
	{
		auto it = attributeMap.find(TAttribute::getAttributeTypeKey());

		if (it == attributeMap.end())
		{
			auto newAttrib = new TAttribute(args...);
			insertAttribute(newAttrib);
			return newAttrib;
		}
		return reinterpret_cast<TAttribute* const>(it->second.get());
	}

} }
