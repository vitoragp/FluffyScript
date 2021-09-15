#include "ast\fl_ast_decl.h"
#include "ast\fl_ast_type.h"
#define FLUFFY_PRIMITIVE_TYPE_DECL_IMP(type) \
		TypeDecl##type::TypeDecl##type() \
			: ast::AstNode(AstNodeType_e::type##Type, 0, 0) \
		{} \
		TypeDecl##type::~TypeDecl##type() \
		{}

namespace fluffy { namespace ast { namespace types { namespace refs {
	static auto g_typeDeclVoidRef			= std::make_unique<ast::TypeDeclVoid>();

	static auto g_typeDeclBoolRef			= std::make_unique<ast::TypeDeclBool>();

	static auto g_typeDeclI8Ref				= std::make_unique<ast::TypeDeclI8>();
	static auto g_typeDeclU8Ref				= std::make_unique<ast::TypeDeclU8>();

	static auto g_typeDeclI16Ref			= std::make_unique<ast::TypeDeclI16>();
	static auto g_typeDeclU16Ref			= std::make_unique<ast::TypeDeclU16>();

	static auto g_typeDeclI32Ref			= std::make_unique<ast::TypeDeclI32>();
	static auto g_typeDeclU32Ref			= std::make_unique<ast::TypeDeclU32>();

	static auto g_typeDeclI64Ref			= std::make_unique<ast::TypeDeclI64>();
	static auto g_typeDeclU64Ref			= std::make_unique<ast::TypeDeclU64>();

	static auto g_typeDeclFp32Ref			= std::make_unique<ast::TypeDeclFp32>();
	static auto g_typeDeclFp64Ref			= std::make_unique<ast::TypeDeclFp64>();

	static auto g_typeDeclStringRef			= std::make_unique<ast::TypeDeclObject>();

	static auto g_typeDeclObjectRef			= std::make_unique<ast::TypeDeclString>();
} } } }

namespace fluffy { namespace ast {
	/**
	 * TypeDecl
	 */

	TypeDecl::TypeDecl(AstNodeType_e nodeType, U32 line, U32 column)
		: AstNode(nodeType, line, column)
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
	 * TypeDeclPrimitive
	 */

	TypeDeclPrimitive::TypeDeclPrimitive(U32 line, U32 column, ast::AstNode* const primitiveTypeRef, const PrimitiveTypeID_e primitiveType)
		: TypeDecl(AstNodeType_e::PrimitiveType, line, column)
		, primitiveTypeRef(primitiveTypeRef)
		, primitiveType(primitiveType)
	{}

	TypeDeclPrimitive::~TypeDeclPrimitive()
	{}

	/**
	 * TypeDeclArray
	 */


	TypeDeclArray::TypeDeclArray(U32 line, U32 column)
		: TypeDecl(AstNodeType_e::ArrayType, line, column)
	{}

	TypeDeclArray::~TypeDeclArray()
	{}

	/**
	 * TypeDeclFunction
	 */

	TypeDeclFunction::TypeDeclFunction(U32 line, U32 column)
		: TypeDecl(AstNodeType_e::FunctionType, line, column)
	{}

	TypeDeclFunction::~TypeDeclFunction()
	{}

	/**
	 * TypeDeclTuple
	 */

	TypeDeclTuple::TypeDeclTuple(U32 line, U32 column)
		: TypeDecl(AstNodeType_e::TupleType, line, column)
	{}

	TypeDeclTuple::~TypeDeclTuple()
	{}

	/**
	 * TypeDeclNamed
	 */

	TypeDeclNamed::TypeDeclNamed(U32 line, U32 column)
		: TypeDecl(AstNodeType_e::NamedType, line, column)
		, startFromRoot(false)
	{}

	TypeDeclNamed::~TypeDeclNamed()
	{}

	/**
	 * SelfTypeDecl
	 */

	SelfTypeDecl::SelfTypeDecl(U32 line, U32 column)
		: TypeDecl(AstNodeType_e::SelfType, line, column)
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

	/**
	 * Funcoes auxiliares
	 */

	std::unique_ptr<ast::TypeDeclPrimitive>
	makePrimitiveType(U32 line, U32 column, PrimitiveTypeID_e typeId)
	{
		switch (typeId)
		{
		case PrimitiveTypeID_e::Void:
			return std::make_unique<ast::TypeDeclPrimitive>(line, column, types::refs::g_typeDeclVoidRef.get(), PrimitiveTypeID_e::Void);
		case PrimitiveTypeID_e::Bool:
			return std::make_unique<ast::TypeDeclPrimitive>(line, column, types::refs::g_typeDeclBoolRef.get(), PrimitiveTypeID_e::Bool);
		case PrimitiveTypeID_e::I8:
			return std::make_unique<ast::TypeDeclPrimitive>(line, column, types::refs::g_typeDeclI8Ref.get(), PrimitiveTypeID_e::I8);
		case PrimitiveTypeID_e::U8:
			return std::make_unique<ast::TypeDeclPrimitive>(line, column, types::refs::g_typeDeclU8Ref.get(), PrimitiveTypeID_e::U8);
		case PrimitiveTypeID_e::I16:
			return std::make_unique<ast::TypeDeclPrimitive>(line, column, types::refs::g_typeDeclI16Ref.get(), PrimitiveTypeID_e::I16);
		case PrimitiveTypeID_e::U16:
			return std::make_unique<ast::TypeDeclPrimitive>(line, column, types::refs::g_typeDeclU16Ref.get(), PrimitiveTypeID_e::U16);
		case PrimitiveTypeID_e::I32:
			return std::make_unique<ast::TypeDeclPrimitive>(line, column, types::refs::g_typeDeclI32Ref.get(), PrimitiveTypeID_e::I32);
		case PrimitiveTypeID_e::U32:
			return std::make_unique<ast::TypeDeclPrimitive>(line, column, types::refs::g_typeDeclU32Ref.get(), PrimitiveTypeID_e::U32);
		case PrimitiveTypeID_e::I64:
			return std::make_unique<ast::TypeDeclPrimitive>(line, column, types::refs::g_typeDeclI64Ref.get(), PrimitiveTypeID_e::I64);
		case PrimitiveTypeID_e::U64:
			return std::make_unique<ast::TypeDeclPrimitive>(line, column, types::refs::g_typeDeclU64Ref.get(), PrimitiveTypeID_e::U64);
		case PrimitiveTypeID_e::Fp32:
			return std::make_unique<ast::TypeDeclPrimitive>(line, column, types::refs::g_typeDeclFp32Ref.get(), PrimitiveTypeID_e::Fp32);
		case PrimitiveTypeID_e::Fp64:
			return std::make_unique<ast::TypeDeclPrimitive>(line, column, types::refs::g_typeDeclFp64Ref.get(), PrimitiveTypeID_e::Fp64);
		case PrimitiveTypeID_e::String:
			return std::make_unique<ast::TypeDeclPrimitive>(line, column, types::refs::g_typeDeclStringRef.get(), PrimitiveTypeID_e::String);
		case PrimitiveTypeID_e::Object:
			return std::make_unique<ast::TypeDeclPrimitive>(line, column, types::refs::g_typeDeclObjectRef.get(), PrimitiveTypeID_e::Object);
		default:
			return nullptr;
		}
	}
} }