#include "attributes\fl_attribute.h"
namespace fluffy { namespace attributes {
	/**
	 * Attribute
	 */
	
	Attribute::Attribute(AttributeType_e type)
		: type(type)
	{}

	Attribute::~Attribute()
	{}

	AttributeType_e
	Attribute::getAttributeType()
	{
		return type;
	}
} }