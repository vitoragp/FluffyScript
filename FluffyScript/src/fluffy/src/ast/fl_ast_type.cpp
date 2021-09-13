#include "ast\fl_ast_decl.h"
#include "ast\fl_ast_type.h"
#define FLUFFY_PRIMITIVE_TYPE_DECL_IMP(type) \
		TypeDecl##type::TypeDecl##type(U32 line, U32 column) \
			: TypeDecl(AstNodeType_e::type##Type, TypeDeclID_e::type, line, column) \
		{} \
		TypeDecl##type::~TypeDecl##type() \
		{}

namespace fluffy { namespace ast {
	/**
	 * TypeDecl
	 */

	TypeDecl::TypeDecl(AstNodeType_e nodeType, TypeDeclID_e typeID, U32 line, U32 column)
		: AstNode(nodeType, line, column)
		, typeID(typeID)
		, nullable(false)
	{}

	TypeDecl::~TypeDecl()
	{}

	/**
	 * Tipos primitivos
	 */

	FLUFFY_PRIMITIVE_TYPE_DECL_IMP(Void);
	FLUFFY_PRIMITIVE_TYPE_DECL_IMP(Bool);
	FLUFFY_PRIMITIVE_TYPE_DECL_IMP(I8);
	FLUFFY_PRIMITIVE_TYPE_DECL_IMP(U8);
	FLUFFY_PRIMITIVE_TYPE_DECL_IMP(I16);
	FLUFFY_PRIMITIVE_TYPE_DECL_IMP(U16);
	FLUFFY_PRIMITIVE_TYPE_DECL_IMP(I32);
	FLUFFY_PRIMITIVE_TYPE_DECL_IMP(U32);
	FLUFFY_PRIMITIVE_TYPE_DECL_IMP(I64);
	FLUFFY_PRIMITIVE_TYPE_DECL_IMP(U64);
	FLUFFY_PRIMITIVE_TYPE_DECL_IMP(Fp32);
	FLUFFY_PRIMITIVE_TYPE_DECL_IMP(Fp64);
	FLUFFY_PRIMITIVE_TYPE_DECL_IMP(String);
	FLUFFY_PRIMITIVE_TYPE_DECL_IMP(Object);

	/**
	 * TypeDeclArray
	 */


	TypeDeclArray::TypeDeclArray(U32 line, U32 column)
		: TypeDecl(AstNodeType_e::ArrayType, TypeDeclID_e::Array, line, column)
	{}

	TypeDeclArray::~TypeDeclArray()
	{}

	/**
	 * TypeDeclFunction
	 */

	TypeDeclFunction::TypeDeclFunction(U32 line, U32 column)
		: TypeDecl(AstNodeType_e::FunctionType, TypeDeclID_e::Function, line, column)
	{}

	TypeDeclFunction::~TypeDeclFunction()
	{}

	/**
	 * TypeDeclTuple
	 */

	TypeDeclTuple::TypeDeclTuple(U32 line, U32 column)
		: TypeDecl(AstNodeType_e::TupleType, TypeDeclID_e::Tuple, line, column)
	{}

	TypeDeclTuple::~TypeDeclTuple()
	{}

	/**
	 * TypeDeclNamed
	 */

	TypeDeclNamed::TypeDeclNamed(U32 line, U32 column)
		: TypeDecl(AstNodeType_e::NamedType, TypeDeclID_e::Named, line, column)
		, startFromRoot(false)
		, referencedScope(nullptr)
		, referencedNode(nullptr)
		, hasBeenResolved(false)
	{}

	TypeDeclNamed::~TypeDeclNamed()
	{}

	/**
	 * SelfTypeDecl
	 */

	SelfTypeDecl::SelfTypeDecl(U32 line, U32 column)
		: TypeDecl(AstNodeType_e::SelfType, TypeDeclID_e::Self, line, column)
	{}

	SelfTypeDecl::~SelfTypeDecl()
	{}

	/**
	 * ArrayDecl
	 */

	ArrayDecl::ArrayDecl(AstNodeType_e nodeType, ArrayType_e arrayType, U32 line, U32 column)
		: AstNode(nodeType, line, column)
		, arrayType(arrayType)
	{}

	ArrayDecl::~ArrayDecl()
	{}

	/**
	 * SizedArrayDecl
	 */

	SizedArrayDecl::SizedArrayDecl(U32 line, U32 column)
		: ArrayDecl(AstNodeType_e::SizedArray, ArrayType_e::Sized, line, column)
		, size(0)
	{}

	SizedArrayDecl::~SizedArrayDecl()
	{}

	/**
	 * UnsizedArrayDecl
	 */

	UnsizedArrayDecl::UnsizedArrayDecl(U32 line, U32 column)
		: ArrayDecl(AstNodeType_e::UnsizedArray, ArrayType_e::Unsized, line, column)
	{}

	UnsizedArrayDecl::~UnsizedArrayDecl()
	{}
} }