#pragma once
#include "fl_string.h"
#include "fl_defs.h"
#include "fl_exceptions.h"

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

		const AstNodeType_e		nodeType;
		TString					identifier;
		U32						line;
		U32						column;
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

} }