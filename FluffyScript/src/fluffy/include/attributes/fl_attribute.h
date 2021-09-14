#pragma once
#include "fl_defs.h"
namespace fluffy { namespace attributes {

	/**
	 * Attribute
	 */

	class Attribute
	{
	protected:
		Attribute(AttributeType_e type);

	public:
		virtual ~Attribute();

		AttributeType_e
		getAttributeType();

		const AttributeType_e
		type;
	};

	/**
	 * AttributeTemplate
	 */
	template <AttributeType_e typeKey>
	class AttributeTemplate : public Attribute
	{
	protected:
		AttributeTemplate()
			: Attribute(typeKey)
		{}

	public:
		virtual ~AttributeTemplate()
		{}

		static AttributeType_e
		getAttributeTypeKey()
		{
			return typeKey;
		}
	};
} }