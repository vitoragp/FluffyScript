#pragma once
#include <memory>
#include "ast\fl_ast_decl.h"
namespace fluffy { namespace generics {
	/**
	 * TClone
	 */

	template <typename T>
	struct TClone;

	/**
	 * clone
	 */

	template <typename T>
	T* clone(T* decl)
	{
		return TClone<T>::clone(decl);
	}

	/**
	 * clone_single
	 */
	template <
		typename TUnique
	> TUnique
	clone_single(TUnique& unique)
	{
		if (unique == nullptr)
		{
			return nullptr;
		}
		return TUnique(clone<typename TUnique::element_type>(unique.get()));
	}

	/**
	 * clone_each
	 */
	template <
		typename TList
	> TList
	clone_each(TList& list)
	{
		TList listOut;
		for (auto it = list.begin(); it != list.end(); it++)
		{
			listOut.emplace_back(
				typename TList::value_type(
					clone<typename TList::value_type::element_type>(it->get())
				)
			);
		}
		return listOut;
	}

	/**
	 * TClone -> ClassDecl
	 */

	template <>
	struct TClone<ast::ClassDecl>
	{
		static ast::ClassDecl* const
		clone(ast::ClassDecl* const decl)
		{
			ast::ClassDecl* const newDecl = new ast::ClassDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->isAbstract = decl->isAbstract;
			newDecl->isExported = decl->isExported;

			newDecl->genericDecl = clone_single(decl->genericDecl);
			newDecl->constructorList = clone_each(decl->constructorList);
			newDecl->destructorDecl = clone_single(decl->destructorDecl);			
			newDecl->functionList = clone_each(decl->functionList);
			newDecl->variableList = clone_each(decl->variableList);

			return newDecl;
		}
	};

	/**
	 * TClone -> ClassConstructorDecl
	 */

	template <>
	struct TClone<ast::ClassConstructorDecl>
	{
		static ast::ClassConstructorDecl* const
		clone(ast::ClassConstructorDecl* const decl)
		{
			ast::ClassConstructorDecl* const newDecl = new ast::ClassConstructorDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->accessModifier = decl->accessModifier;

			newDecl->parameterList = clone_each(decl->parameterList);

			return newDecl;
		}
	};

	/**
	 * TClone -> ClassDestructorDecl
	 */

	template <>
	struct TClone<ast::ClassDestructorDecl>
	{
		static ast::ClassDestructorDecl* const
		clone(ast::ClassDestructorDecl* const decl)
		{
			ast::ClassDestructorDecl* const newDecl = new ast::ClassDestructorDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->accessModifier = decl->accessModifier;

			return newDecl;
		}
	};

	/**
	 * TClone -> ClassFunctionDecl
	 */

	template <>
	struct TClone<ast::ClassFunctionDecl>
	{
		static ast::ClassFunctionDecl* const
		clone(ast::ClassFunctionDecl* const decl)
		{
			ast::ClassFunctionDecl* const newDecl = new ast::ClassFunctionDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->accessModifier = decl->accessModifier;
			newDecl->isStatic = decl->isStatic;
			newDecl->isAbstract = decl->isAbstract;
			newDecl->isFinal = decl->isFinal;
			newDecl->isOverride = decl->isOverride;

			newDecl->genericDecl = clone_single(decl->genericDecl);

			newDecl->sourceTypeDecl = clone_single(decl->sourceTypeDecl);

			newDecl->parameterList = clone_each(decl->parameterList);
			newDecl->returnType = clone_single(decl->returnType);

			return newDecl;
		}
	};

	/**
	 * TClone -> ClassVariableDecl
	 */

	template <>
	struct TClone<ast::ClassVariableDecl>
	{
		static ast::ClassVariableDecl* const
		clone(ast::ClassVariableDecl* const decl)
		{
			ast::ClassVariableDecl* const newDecl = new ast::ClassVariableDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->accessModifier = decl->accessModifier;
			newDecl->isConst = decl->isConst;
			newDecl->isReference = decl->isReference;
			newDecl->isShared = decl->isShared;
			newDecl->isStatic = decl->isStatic;
			newDecl->isUnique = decl->isUnique;

			newDecl->typeDecl = clone_single(decl->typeDecl);

			return newDecl;
		}
	};

	/**
	 * TClone -> FunctionParameterDecl
	 */

	template <>
	struct TClone<ast::FunctionParameterDecl>
	{
		static ast::FunctionParameterDecl* const
		clone(ast::FunctionParameterDecl* const decl)
		{
			ast::FunctionParameterDecl* const newDecl = new ast::FunctionParameterDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->isEllipsis = decl->isEllipsis;
			newDecl->isReference = decl->isReference;
			newDecl->isShared = decl->isShared;
			newDecl->isUnique = decl->isUnique;

			newDecl->patternDecl = clone_single(decl->patternDecl);
			newDecl->typeDecl = clone_single(decl->typeDecl);

			return newDecl;
		}
	};

	/**
	 * TClone -> InterfaceDecl
	 */

	template <>
	struct TClone<ast::InterfaceDecl>
	{
		static ast::InterfaceDecl* const
		clone(ast::InterfaceDecl* const decl)
		{
			ast::InterfaceDecl* const newDecl = new ast::InterfaceDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->isExported = decl->isExported;

			newDecl->genericDecl = clone_single(decl->genericDecl);
			newDecl->functionDeclList = clone_each(decl->functionDeclList);

			return newDecl;
		}
	};

	/**
	 * TClone -> InterfaceFunctionDecl
	 */

	template <>
	struct TClone<ast::InterfaceFunctionDecl>
	{
		static ast::InterfaceFunctionDecl* const
		clone(ast::InterfaceFunctionDecl* const decl)
		{
			ast::InterfaceFunctionDecl* const newDecl = new ast::InterfaceFunctionDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;

			newDecl->genericDecl = clone_single(decl->genericDecl);

			newDecl->parameterList = clone_each(decl->parameterList);
			newDecl->returnType = clone_single(decl->returnType);

			return newDecl;
		}
	};

	/**
	 * TClone -> TraitDecl
	 */

	template <>
	struct TClone<ast::TraitDecl>
	{
		static ast::TraitDecl* const
		clone(ast::TraitDecl* const decl)
		{
			ast::TraitDecl* const newDecl = new ast::TraitDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->isExported = decl->isExported;

			newDecl->genericDecl = clone_single(decl->genericDecl);

			newDecl->functionDeclList = clone_each(decl->functionDeclList);

			return newDecl;
		}
	};

	/**
	 * TClone -> TraitFunctionDecl
	 */

	template <>
	struct TClone<ast::TraitFunctionDecl>
	{
		static ast::TraitFunctionDecl* const
		clone(ast::TraitFunctionDecl* const decl)
		{
			ast::TraitFunctionDecl* const newDecl = new ast::TraitFunctionDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->isStatic = decl->isStatic;

			newDecl->genericDecl = clone_single(decl->genericDecl);

			newDecl->parameterList = clone_each(decl->parameterList);
			newDecl->returnType = clone_single(decl->returnType);

			return newDecl;
		}
	};

	/**
	 * TClone -> EnumDecl
	 */

	template <>
	struct TClone<ast::EnumDecl>
	{
		static ast::EnumDecl* const
		clone(ast::EnumDecl* const decl)
		{
			ast::EnumDecl* const newDecl = new ast::EnumDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->isExported = decl->isExported;

			newDecl->genericDecl = clone_single(decl->genericDecl);

			newDecl->enumItemDeclList = clone_each(decl->enumItemDeclList);

			return newDecl;
		}
	};

	/**
	 * TClone -> EnumItemDecl
	 */

	template <>
	struct TClone<ast::EnumItemDecl>
	{
		static ast::EnumItemDecl* const
		clone(ast::EnumItemDecl* const decl)
		{
			ast::EnumItemDecl* const newDecl = new ast::EnumItemDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->hasData = decl->hasData;
			newDecl->hasValue = decl->hasValue;

			newDecl->dataTypeDeclList = clone_each(decl->dataTypeDeclList);

			return newDecl;
		}
	};

	/**
	 * TClone -> GenericDecl
	 */

	template <>
	struct TClone<ast::GenericDecl>
	{
		static ast::GenericDecl* const
		clone(ast::GenericDecl* const decl)
		{
			ast::GenericDecl* const newDecl = new ast::GenericDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;

			newDecl->genericDeclItemList = clone_each(decl->genericDeclItemList);

			return newDecl;
		}
	};

	/**
	 * TClone -> GenericItemDecl
	 */

	template <>
	struct TClone<ast::GenericItemDecl>
	{
		static ast::GenericItemDecl* const
		clone(ast::GenericItemDecl* const decl)
		{
			ast::GenericItemDecl* const newDecl = new ast::GenericItemDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;

			newDecl->whereTypeList = clone_each(decl->whereTypeList);

			return newDecl;
		}
	};

	/**
	 * TClone -> TypeDeclPrimitive
	 */

	template <>
	struct TClone<ast::TypeDeclPrimitive>
	{
		static ast::TypeDeclPrimitive* const
		clone(ast::TypeDeclPrimitive* const decl)
		{
			ast::TypeDeclPrimitive* const newDecl = new ast::TypeDeclPrimitive(
				decl->line, decl->column,
				decl->primitiveTypeRef,
				decl->primitiveType
			);

			newDecl->identifier = decl->identifier;
			newDecl->nullable = decl->nullable;

			return newDecl;
		}
	};

	/**
	 * TClone -> TypeDeclArray
	 */

	template <>
	struct TClone<ast::TypeDeclArray>
	{
		static ast::TypeDeclArray* const
		clone(ast::TypeDeclArray* const decl)
		{
			ast::TypeDeclArray* const newDecl = new ast::TypeDeclArray(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->nullable = decl->nullable;

			newDecl->arrayDeclList = clone_each(decl->arrayDeclList);
			newDecl->valueType = clone_single(decl->valueType);

			return newDecl;
		}
	};

	/**
	 * TClone -> TypeDeclFunction
	 */

	template <>
	struct TClone<ast::TypeDeclFunction>
	{
		static ast::TypeDeclFunction* const
		clone(ast::TypeDeclFunction* const decl)
		{
			ast::TypeDeclFunction* const newDecl = new ast::TypeDeclFunction(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->nullable = decl->nullable;

			newDecl->objectOwnerDecl = clone_single(decl->objectOwnerDecl);

			newDecl->parameterTypeList = clone_each(decl->parameterTypeList);

			newDecl->returnType = clone_single(decl->returnType);

			return newDecl;
		}
	};

	/**
	 * TClone -> TypeDeclTuple
	 */

	template <>
	struct TClone<ast::TypeDeclTuple>
	{
		static ast::TypeDeclTuple* const
		clone(ast::TypeDeclTuple* const decl)
		{
			ast::TypeDeclTuple* const newDecl = new ast::TypeDeclTuple(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->nullable = decl->nullable;

			newDecl->tupleItemList = clone_each(decl->tupleItemList);

			return newDecl;
		}
	};

	/**
	 * TClone -> TypeDeclTuple
	 */

	template <>
	struct TClone<ast::TypeDeclNamed>
	{
		static ast::TypeDeclNamed* const
		clone(ast::TypeDeclNamed* const decl)
		{
			ast::TypeDeclNamed* const newDecl = new ast::TypeDeclNamed(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->nullable = decl->nullable;
			newDecl->startFromRoot = decl->startFromRoot;

			newDecl->genericDefinitionList = clone_each(decl->genericDefinitionList);
			newDecl->scopePath = clone_single(decl->scopePath);

			return newDecl;
		}
	};

	/**
	 * TClone -> SelfTypeDecl
	 */

	template <>
	struct TClone<ast::SelfTypeDecl>
	{
		static ast::SelfTypeDecl* const
		clone(ast::SelfTypeDecl* const decl)
		{
			ast::SelfTypeDecl* const newDecl = new ast::SelfTypeDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->nullable = decl->nullable;

			return newDecl;
		}
	};

	/**
	 * TClone -> TypeDecl
	 */

	template <>
	struct TClone<ast::TypeDecl>
	{
		static ast::TypeDecl* const
		clone(ast::TypeDecl* const decl)
		{
			switch (decl->nodeType)
			{
			case AstNodeType_e::PrimitiveType:
				return TClone<ast::TypeDeclPrimitive>::clone(decl->to<ast::TypeDeclPrimitive>());

			case AstNodeType_e::ArrayType:
				return TClone<ast::TypeDeclArray>::clone(decl->to<ast::TypeDeclArray>());

			case AstNodeType_e::FunctionType:
				return TClone<ast::TypeDeclFunction>::clone(decl->to<ast::TypeDeclFunction>());

			case AstNodeType_e::TupleType:
				return TClone<ast::TypeDeclTuple>::clone(decl->to<ast::TypeDeclTuple>());

			case AstNodeType_e::NamedType:
				return TClone<ast::TypeDeclNamed>::clone(decl->to<ast::TypeDeclNamed>());

			case AstNodeType_e::SelfType:
				return TClone<ast::SelfTypeDecl>::clone(decl->to<ast::SelfTypeDecl>());

			default:
				break;
			}
			return nullptr;
		}
	};

	/**
	 * TClone -> SizedArrayDecl
	 */

	template <>
	struct TClone<ast::SizedArrayDecl>
	{
		static ast::SizedArrayDecl* const
		clone(ast::SizedArrayDecl* const decl)
		{
			ast::SizedArrayDecl* const newDecl = new ast::SizedArrayDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->size = decl->size;

			return newDecl;
		}
	};

	/**
	 * TClone -> UnsizedArrayDecl
	 */

	template <>
	struct TClone<ast::UnsizedArrayDecl>
	{
		static ast::UnsizedArrayDecl* const
		clone(ast::UnsizedArrayDecl* const decl)
		{
			ast::UnsizedArrayDecl* const newDecl = new ast::UnsizedArrayDecl(decl->line, decl->column);
			return newDecl;
		}
	};

	/**
	 * TClone -> ArrayDecl
	 */

	template <>
	struct TClone<ast::ArrayDecl>
	{
		static ast::ArrayDecl* const
		clone(ast::ArrayDecl* const decl)
		{
			switch (decl->nodeType)
			{
			case AstNodeType_e::SizedArray:
				return TClone<ast::SizedArrayDecl>::clone(decl->to<ast::SizedArrayDecl>());

			case AstNodeType_e::UnsizedArray:
				return TClone<ast::UnsizedArrayDecl>::clone(decl->to<ast::UnsizedArrayDecl>());

			default:
				break;
			}
			return nullptr;
		}
	};

	/**
	 * TClone -> ScopedPathDecl
	 */

	template <>
	struct TClone<ast::ScopedPathDecl>
	{
		static ast::ScopedPathDecl* const
		clone(ast::ScopedPathDecl* const decl)
		{
			ast::ScopedPathDecl* const newDecl = new ast::ScopedPathDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;

			newDecl->scopedChildPath = clone_single(decl->scopedChildPath);

			return newDecl;
		}
	};

	/**
	 * TClone -> LiteralPatternDecl
	 */

	template <>
	struct TClone<ast::pattern::LiteralPatternDecl>
	{
		static ast::pattern::LiteralPatternDecl* const
		clone(ast::pattern::LiteralPatternDecl* const decl)
		{
			ast::pattern::LiteralPatternDecl* const newDecl = new ast::pattern::LiteralPatternDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;

			return newDecl;
		}
	};

	/**
	 * TClone -> EnumerablePatternDecl
	 */

	template <>
	struct TClone<ast::pattern::EnumerablePatternDecl>
	{
		static ast::pattern::EnumerablePatternDecl* const
		clone(ast::pattern::EnumerablePatternDecl* const decl)
		{
			ast::pattern::EnumerablePatternDecl* const newDecl = new ast::pattern::EnumerablePatternDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->patternDeclItemList = clone_each(decl->patternDeclItemList);

			return newDecl;
		}
	};

	/**
	 * TClone -> StructurePatternDecl
	 */

	template <>
	struct TClone<ast::pattern::StructurePatternDecl>
	{
		static ast::pattern::StructurePatternDecl* const
		clone(ast::pattern::StructurePatternDecl* const decl)
		{
			ast::pattern::StructurePatternDecl* const newDecl = new ast::pattern::StructurePatternDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->structureItemDeclList = clone_each(decl->structureItemDeclList);

			return newDecl;
		}
	};

	/**
	 * TClone -> TuplePatternDecl
	 */

	template <>
	struct TClone<ast::pattern::TuplePatternDecl>
	{
		static ast::pattern::TuplePatternDecl* const
		clone(ast::pattern::TuplePatternDecl* const decl)
		{
			ast::pattern::TuplePatternDecl* const newDecl = new ast::pattern::TuplePatternDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->patternItemDeclList = clone_each(decl->patternItemDeclList);

			return newDecl;
		}
	};

	/**
	 * TClone -> PatternDecl
	 */

	template <>
	struct TClone<ast::pattern::PatternDecl>
	{
		static ast::pattern::PatternDecl* const
		clone(ast::pattern::PatternDecl* const decl)
		{
			switch (decl->nodeType)
			{
			case AstNodeType_e::LiteralPattern:
				return TClone<ast::pattern::LiteralPatternDecl>::clone(decl->to<ast::pattern::LiteralPatternDecl>());

			case AstNodeType_e::EnumerablePatternDecl:
				return TClone<ast::pattern::EnumerablePatternDecl>::clone(decl->to<ast::pattern::EnumerablePatternDecl>());

			case AstNodeType_e::StructurePatternDecl:
				return TClone<ast::pattern::StructurePatternDecl>::clone(decl->to<ast::pattern::StructurePatternDecl>());

			case AstNodeType_e::TuplePatternDecl:
				return TClone<ast::pattern::TuplePatternDecl>::clone(decl->to<ast::pattern::TuplePatternDecl>());

			default:
				break;
			}
			return nullptr;
		}
	};

	/**
	 * TClone -> StructureItemPatternDecl
	 */

	template <>
	struct TClone<ast::pattern::StructureItemPatternDecl>
	{
		static ast::pattern::StructureItemPatternDecl* const
		clone(ast::pattern::StructureItemPatternDecl* const decl)
		{
			ast::pattern::StructureItemPatternDecl* const newDecl = new ast::pattern::StructureItemPatternDecl(decl->line, decl->column);

			newDecl->identifier = decl->identifier;
			newDecl->referencedPattern = clone_single(decl->referencedPattern);

			return newDecl;
		}
	};
} }