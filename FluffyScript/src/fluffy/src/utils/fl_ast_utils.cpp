#include <functional>
#include <sstream>
#include "ast\fl_ast.h"
#include "ast\fl_ast_decl.h"
#include "ast\fl_ast_type.h"
#include "utils\fl_ast_utils.h"
#include "attributes\fl_reference.h"
namespace fluffy { namespace utils {
	/**
	 * AstUtils
	 */

	Bool
	AstUtils::equals(ast::AstNode* nodeA, ast::AstNode* nodeB)
	{
		if (nodeA == nodeB)
		{
			return false;
		}

		if (nodeA->nodeType == nodeB->nodeType)
		{
			switch (nodeA->nodeType)
			{
			case AstNodeType_e::IncludeItemDecl:
				{
					TString idA, idB;

					ast::IncludeItemDecl* incItemA = reinterpret_cast<ast::IncludeItemDecl*>(nodeA);
					ast::IncludeItemDecl* incItemB = reinterpret_cast<ast::IncludeItemDecl*>(nodeB);

					if (incItemA->referencedAlias == TString(nullptr))
					{
						idA = incItemA->identifier;
					}
					else
					{
						idA = incItemA->referencedAlias;
					}

					if (incItemB->referencedAlias == TString(nullptr))
					{
						idB = incItemB->identifier;
					}
					else
					{
						idB = incItemB->referencedAlias;
					}

					if (idA != idB)
					{
						return false;
					}
				}
				return true;

			case AstNodeType_e::TraitDecl:
				{
					ast::TraitDecl* traitA = reinterpret_cast<ast::TraitDecl*>(nodeA);
					ast::TraitDecl* traitB = reinterpret_cast<ast::TraitDecl*>(nodeB);

					if (traitA->identifier != traitB->identifier)
					{
						return false;
					}
					if ((traitA->isDefinition && !traitB->isDefinition) || (!traitA->isDefinition && traitB->isDefinition))
					{
						return false;
					}
					if (traitA->isDefinition && traitB->isDefinition)
					{
						return false;
					}
				}
				return true;

			case AstNodeType_e::TraitForDecl:
				{
					ast::TraitForDecl* traitA = reinterpret_cast<ast::TraitForDecl*>(nodeA);
					ast::TraitForDecl* traitB = reinterpret_cast<ast::TraitForDecl*>(nodeB);

					if (traitA->identifier != traitB->identifier)
					{
						return false;
					}
					if ((traitA->isDefinition && !traitB->isDefinition) || (!traitA->isDefinition && traitB->isDefinition))
					{
						return false;
					}
					if ((traitA->isDefinition && traitB->isDefinition) && !equals(traitA->typeDefinitionDecl.get(), traitB->typeDefinitionDecl.get()))
					{
						return false;
					}
				}
				return true;

			case AstNodeType_e::FunctionDecl:
				{
					ast::FunctionDecl* funcA = reinterpret_cast<ast::FunctionDecl*>(nodeA);
					ast::FunctionDecl* funcB = reinterpret_cast<ast::FunctionDecl*>(nodeB);

					if (funcA && funcB && funcA->identifier == funcB->identifier)
					{
						if (funcA->parameterList.size() == funcB->parameterList.size())
						{
							for (U32 i = 0; i < static_cast<U32>(funcA->parameterList.size()); i++)
							{
								ast::FunctionParameterDecl* paramA = funcA->parameterList[i].get();
								ast::FunctionParameterDecl* paramB = funcB->parameterList[i].get();

								if (!equals(paramA->typeDecl.get(), paramB->typeDecl.get()))
								{
									return false;
								}
							}
						}
						else
						{
							return false;
						}
						return true;
					}
				}
				return false;

			case AstNodeType_e::ClassFunctionDecl:
				{
					ast::ClassFunctionDecl* funcA = reinterpret_cast<ast::ClassFunctionDecl*>(nodeA);
					ast::ClassFunctionDecl* funcB = reinterpret_cast<ast::ClassFunctionDecl*>(nodeB);

					if (funcA && funcB && funcA->identifier == funcB->identifier)
					{
						if (funcA->parameterList.size() == funcB->parameterList.size())
						{
							for (U32 i = 0; i < static_cast<U32>(funcA->parameterList.size()); i++)
							{
								ast::FunctionParameterDecl* paramA = funcA->parameterList[i].get();
								ast::FunctionParameterDecl* paramB = funcB->parameterList[i].get();

								if (!equals(paramA->typeDecl.get(), paramB->typeDecl.get()))
								{
									return false;
								}
							}
						}
						else
						{
							return false;
						}

						if ((funcA->sourceTypeDecl != nullptr && funcB->sourceTypeDecl != nullptr))
						{
							auto referenceA = funcA->getAttribute<attributes::Reference>();
							auto referenceB = funcA->getAttribute<attributes::Reference>();

							if (referenceA && referenceB && referenceA->referencedNode != referenceB->referencedNode)
							{
								return false;
							}
						}
						else if ((funcA->sourceTypeDecl != nullptr && funcB->sourceTypeDecl == nullptr) ||
							(funcA->sourceTypeDecl == nullptr && funcB->sourceTypeDecl != nullptr))
						{
							return false;
						}
						return true;
					}
				}
				return false;

			case AstNodeType_e::ClassConstructorDecl:
				{
					ast::ClassConstructorDecl* funcA = reinterpret_cast<ast::ClassConstructorDecl*>(nodeA);
					ast::ClassConstructorDecl* funcB = reinterpret_cast<ast::ClassConstructorDecl*>(nodeB);

					if (funcA->parameterList.size() == funcB->parameterList.size())
					{
						for (U32 i = 0; i < static_cast<U32>(funcA->parameterList.size()); i++)
						{
							ast::FunctionParameterDecl* paramA = funcA->parameterList[i].get();
							ast::FunctionParameterDecl* paramB = funcB->parameterList[i].get();

							if (!equals(paramA->typeDecl.get(), paramB->typeDecl.get()))
							{
								return false;
							}
						}
					}
					else
					{
						return false;
					}
				}
				return true;

			case AstNodeType_e::FunctionParameterDeclExpr:
				{
					auto paramA = reinterpret_cast<ast::expr::ExpressionFunctionParameterDecl*>(nodeA);
					auto paramB = reinterpret_cast<ast::expr::ExpressionFunctionParameterDecl*>(nodeB);

					if (paramA->identifier == paramB->identifier)
					{
						return true;
					}
				}
				break;

			case AstNodeType_e::PrimitiveType:
				{
					ast::TypeDeclPrimitive* typeA = reinterpret_cast<ast::TypeDeclPrimitive*>(nodeA);
					ast::TypeDeclPrimitive* typeB = reinterpret_cast<ast::TypeDeclPrimitive*>(nodeB);

					if (typeA->primitiveType != typeB->primitiveType)
					{
						return false;
					}
				}
				return true;

			case AstNodeType_e::ArrayType:
				{
					ast::TypeDeclArray* typeA = reinterpret_cast<ast::TypeDeclArray*>(nodeA);
					ast::TypeDeclArray* typeB = reinterpret_cast<ast::TypeDeclArray*>(nodeB);

					if (typeA->nullable != typeB->nullable)
					{
						return false;
					}
					if (!equals(typeA->valueType.get(), typeB->valueType.get()))
					{
						return false;
					}
					if (typeA->arrayDeclList.size() != typeB->arrayDeclList.size())
					{
						return false;
					}
					for (U32 i = 0; i < static_cast<U32>(typeA->arrayDeclList.size()); i++)
					{
						if (!equals(typeA->arrayDeclList[i].get(), typeB->arrayDeclList[i].get()))
						{
							return false;
						}
					}
				}
				return true;

			case AstNodeType_e::FunctionType:
				{
					ast::TypeDeclFunction* typeA = reinterpret_cast<ast::TypeDeclFunction*>(nodeA);
					ast::TypeDeclFunction* typeB = reinterpret_cast<ast::TypeDeclFunction*>(nodeB);

					if (typeA->nullable != typeB->nullable)
					{
						return false;
					}

					if (!equals(typeA->returnType.get(), typeB->returnType.get()))
					{
						return false;
					}

					if (typeA->parameterTypeList.size() == typeB->parameterTypeList.size())
					{
						for (U32 i = 0; i < static_cast<U32>(typeA->parameterTypeList.size()); i++)
						{
							if (!equals(typeA->parameterTypeList[i].get(), typeB->parameterTypeList[i].get()))
							{
								return false;
							}
						}
					}
					else
					{
						return false;
					}
				}
				return true;

			case AstNodeType_e::TupleType:
				{
					ast::TypeDeclTuple* typeA = reinterpret_cast<ast::TypeDeclTuple*>(nodeA);
					ast::TypeDeclTuple* typeB = reinterpret_cast<ast::TypeDeclTuple*>(nodeB);

					if (typeA->nullable != typeB->nullable)
					{
						return false;
					}

					if (typeA->tupleItemList.size() != typeB->tupleItemList.size())
					{
						return false;
					}

					for (U32 i = 0; i < static_cast<U32>(typeA->tupleItemList.size()); i++)
					{
						if (!equals(typeA->tupleItemList[i].get(), typeB->tupleItemList[i].get()))
						{
							return false;
						}
					}
				}
				return true;

			case AstNodeType_e::NamedType:
				{
					ast::TypeDeclNamed* typeA = reinterpret_cast<ast::TypeDeclNamed*>(nodeA);
					ast::TypeDeclNamed* typeB = reinterpret_cast<ast::TypeDeclNamed*>(nodeB);

					if (typeA->nullable != typeB->nullable)
					{
						return false;
					}

					auto referenceA = typeA->getAttribute<attributes::Reference>();
					auto referenceB = typeB->getAttribute<attributes::Reference>();

					if (referenceA != nullptr && referenceB != nullptr)
					{
						if (referenceA->referencedNode != referenceB->referencedNode)
						{
							return false;
						}						
					}
					else
					{
						if (typeA->startFromRoot != typeB->startFromRoot)
						{
							return false;
						}
						if (typeA->identifier != typeB->identifier)
						{
							return false;
						}
					}
					if (typeA->genericDefinitionList.size() != typeB->genericDefinitionList.size())
					{
						return false;
					}
					for (U32 i = 0; i < static_cast<U32>(typeA->genericDefinitionList.size()); i++)
					{
						if (!equals(typeA->genericDefinitionList[i].get(), typeB->genericDefinitionList[i].get()))
						{
							return false;
						}
					}					
				}
				return true;

			case AstNodeType_e::ScopedPathDecl:
				{
					ast::ScopedPathDecl* typeA = reinterpret_cast<ast::ScopedPathDecl*>(nodeA);
					ast::ScopedPathDecl* typeB = reinterpret_cast<ast::ScopedPathDecl*>(nodeB);

					if (typeA->identifier != typeB->identifier)
					{
						return false;
					}
					if (typeA->scopedChildPath && typeB->scopedChildPath)
					{
						if (!equals(typeA->scopedChildPath.get(), typeB->scopedChildPath.get()))
						{
							return false;
						}
					}
					else if (typeA->scopedChildPath || typeB->scopedChildPath)
					{
						return false;
					}
				}
				return true;

			case AstNodeType_e::SizedArray:
				{
					ast::SizedArrayDecl* arrayDeclA = dynamic_cast<ast::SizedArrayDecl*>(nodeA);
					ast::SizedArrayDecl* arrayDeclB = dynamic_cast<ast::SizedArrayDecl*>(nodeB);

					if (arrayDeclA->size != arrayDeclB->size)
					{
						return false;
					}
				}
				return true;

			default:
				{
					ast::AstNode* nodeIdA = dynamic_cast<ast::AstNode*>(nodeA);
					ast::AstNode* nodeIdB = dynamic_cast<ast::AstNode*>(nodeB);

					if (nodeIdA && nodeIdB)
					{
						if (nodeIdA->identifier == nodeIdB->identifier)
						{
							return true;
						}
						return false;
					}
				}
				return true;
			}
		}
		else
		{
			if (nodeA->nodeType == AstNodeType_e::IncludeItemDecl)
			{
				auto includeItemA = ast::safe_cast<ast::IncludeItemDecl>(nodeA);

				if (includeItemA->referencedAlias == TString(nullptr))
				{
					if (includeItemA->identifier == nodeB->identifier)
					{
						return true;
					}
				}
				else
				{
					if (includeItemA->referencedAlias == nodeB->identifier)
					{
						return true;
					}
				}
			}
			else if (nodeB->nodeType == AstNodeType_e::IncludeItemDecl)
			{
				auto includeItemB = ast::safe_cast<ast::IncludeItemDecl>(nodeB);

				if (includeItemB->referencedAlias == TString(nullptr))
				{
					if (nodeA->identifier == includeItemB->identifier)
					{
						return true;
					}
				}
				else
				{
					if (nodeA->identifier == includeItemB->referencedAlias)
					{
						return true;
					}
				}
			}
			else
			{
				if (nodeA->identifier == nodeB->identifier)
				{
					if ((nodeA->nodeType == AstNodeType_e::TraitDecl && nodeB->nodeType == AstNodeType_e::TraitForDecl) ||
						(nodeA->nodeType == AstNodeType_e::TraitForDecl && nodeB->nodeType == AstNodeType_e::TraitDecl)) {
						return false;
					}
					return true;
				}
			}
		}
		return false;
	}

	std::vector<std::tuple<TString, U32, U32>>
	AstUtils::extractFunctionIdentifiers(ast::AstNode* node)
	{
		std::vector<std::tuple<TString, U32, U32>> identifiers;
		switch (node->nodeType)
		{
		case AstNodeType_e::FunctionDecl:
			if (auto function = reinterpret_cast<ast::FunctionDecl*>(node))
			{
				for (auto& parameter : function->parameterList)
				{
					if (parameter->patternDecl == nullptr)
					{
						if (parameter->identifier != String("_"))
						{
							identifiers.push_back(
								std::tuple<TString, U32, U32>(
									parameter->identifier,
									parameter->line,
									parameter->column
								)
							);
						}
					}
					else
					{
						std::vector<std::tuple<TString, U32, U32>> patternIdentifiers = extractPatternIdentifiers(parameter->patternDecl.get());
						identifiers.insert(identifiers.end(), patternIdentifiers.begin(), patternIdentifiers.end());
					}
				}
			}
			break;

		case AstNodeType_e::ClassFunctionDecl:
			if (auto function = reinterpret_cast<ast::ClassFunctionDecl*>(node))
			{
				for (auto& parameter : function->parameterList)
				{
					if (parameter->patternDecl == nullptr)
					{
						if (parameter->identifier != String("_"))
						{
							identifiers.push_back(
								std::tuple<TString, U32, U32>(
									parameter->identifier,
									parameter->line,
									parameter->column
								)
							);
						}
					}
					else
					{
						std::vector<std::tuple<TString, U32, U32>> patternIdentifiers = extractPatternIdentifiers(parameter->patternDecl.get());
						identifiers.insert(identifiers.end(), patternIdentifiers.begin(), patternIdentifiers.end());
					}
				}
			}
			break;

		case AstNodeType_e::ClassConstructorDecl:
			if (auto function = reinterpret_cast<ast::ClassConstructorDecl*>(node))
			{
				for (auto& parameter : function->parameterList)
				{
					if (parameter->patternDecl == nullptr)
					{
						if (parameter->identifier != String("_"))
						{
							identifiers.push_back(
								std::tuple<TString, U32, U32>(
									parameter->identifier,
									parameter->line,
									parameter->column
								)
							);
						}
					}
					else
					{
						std::vector<std::tuple<TString, U32, U32>> patternIdentifiers = extractPatternIdentifiers(parameter->patternDecl.get());
						identifiers.insert(identifiers.end(), patternIdentifiers.begin(), patternIdentifiers.end());
					}
				}
			}
			break;

		case AstNodeType_e::InterfaceFunctionDecl:
			if (auto function = reinterpret_cast<ast::InterfaceFunctionDecl*>(node))
			{
				for (auto& parameter : function->parameterList)
				{
					if (parameter->patternDecl == nullptr)
					{
						if (parameter->identifier != String("_"))
						{
							identifiers.push_back(
								std::tuple<TString, U32, U32>(
									parameter->identifier,
									parameter->line,
									parameter->column
								)
							);
						}
					}
					else
					{
						std::vector<std::tuple<TString, U32, U32>> patternIdentifiers = extractPatternIdentifiers(parameter->patternDecl.get());
						identifiers.insert(identifiers.end(), patternIdentifiers.begin(), patternIdentifiers.end());
					}
				}
			}
			break;

		case AstNodeType_e::TraitFunctionDecl:
			if (auto function = reinterpret_cast<ast::TraitFunctionDecl*>(node))
			{
				for (auto& parameter : function->parameterList)
				{
					if (parameter->patternDecl == nullptr)
					{
						if (parameter->identifier != String("_"))
						{
							identifiers.push_back(
								std::tuple<TString, U32, U32>(
									parameter->identifier,
									parameter->line,
									parameter->column
								)
							);
						}
					}
					else
					{
						std::vector<std::tuple<TString, U32, U32>> patternIdentifiers = extractPatternIdentifiers(parameter->patternDecl.get());
						identifiers.insert(identifiers.end(), patternIdentifiers.begin(), patternIdentifiers.end());
					}
				}
			}
			break;

		case AstNodeType_e::FunctionDeclExpr:
			if (auto functionDeclExpr = reinterpret_cast<ast::expr::ExpressionFunctionDecl*>(node))
			{
				for (auto& parameter : functionDeclExpr->parametersDeclList)
				{
					if (!parameter->patternDecl)
					{
						if (parameter->identifier != String("_"))
						{
							identifiers.push_back(
								std::tuple<TString, U32, U32>(
									parameter->identifier,
									parameter->line,
									parameter->column
								)
							);
						}
					}
					else
					{
						std::vector<std::tuple<TString, U32, U32>> patternIdentifiers = extractPatternIdentifiers(parameter->patternDecl.get());
						identifiers.insert(identifiers.end(), patternIdentifiers.begin(), patternIdentifiers.end());
					}
				}
			}
			break;

		default:
			break;
		}
		return identifiers;
	}

	std::vector<std::tuple<TString, U32, U32>>
	AstUtils::extractPatternIdentifiers(ast::pattern::PatternDecl* pattern)
	{
		std::vector<std::tuple<TString, U32, U32>> identifiers;

		std::function<void(ast::pattern::PatternDecl*)> _extractPatternIdentifiers = [&identifiers, &_extractPatternIdentifiers](ast::pattern::PatternDecl* pattern) {
			switch (pattern->nodeType)
			{
			case AstNodeType_e::LiteralPattern:
				if (auto literalPattern = reinterpret_cast<ast::pattern::LiteralPatternDecl*>(pattern))
				{
					if (literalPattern->literalExpr == nullptr)
					{
						identifiers.push_back(std::tuple<TString, U32, U32>(
							literalPattern->identifier,
							literalPattern->line,
							literalPattern->column
						));
					}
				}
				break;

			case AstNodeType_e::TuplePatternDecl:
				if (auto tuplePattern = reinterpret_cast<ast::pattern::TuplePatternDecl*>(pattern))
				{
					for (auto& subPattern : tuplePattern->patternItemDeclList)
					{
						_extractPatternIdentifiers(subPattern.get());
					}
				}
				break;

			case AstNodeType_e::StructurePatternDecl:
				if (auto structurePattern = reinterpret_cast<ast::pattern::StructurePatternDecl*>(pattern))
				{
					for (auto& structureItem : structurePattern->structureItemDeclList)
					{
						if (!structureItem->referencedPattern)
						{
							if (structureItem->identifier != String("_"))
							{
								identifiers.push_back(std::tuple<TString, U32, U32>(
									structureItem->identifier,
									structureItem->line,
									structureItem->column
								));
							}
						}
						else
						{
							_extractPatternIdentifiers(structureItem->referencedPattern.get());
						}
					}
				}
				break;

			case AstNodeType_e::EnumerablePatternDecl:
				if (auto enumerablePattern = reinterpret_cast<ast::pattern::EnumerablePatternDecl*>(pattern))
				{
					for (auto& enumerableItem : enumerablePattern->patternDeclItemList)
					{
						_extractPatternIdentifiers(enumerableItem.get());
					}
				}
				break;

			default:
				break;

			}
		};

		_extractPatternIdentifiers(pattern);
		return identifiers;
	}

	String
	AstUtils::printScopedPath(ast::AstNode* const scopedIdentifier)
	{
		std::stringstream ss;

		if (scopedIdentifier->nodeType == AstNodeType_e::ScopedPathDecl)
		{
			if (auto n = reinterpret_cast<ast::ScopedPathDecl*>(scopedIdentifier))
			{
				if (n->scopedChildPath)
				{
					ast::ScopedPathDecl* nextId = n->scopedChildPath.get();
					while (nextId)
					{
						ss << nextId->identifier.str() << "::";
						nextId = nextId->scopedChildPath.get();
					}
				}
				ss << n->identifier.str();
			}
		}
		return ss.str();
	}

	String
	AstUtils::printIncludeItem(ast::AstNode* const includeItem)
	{
		std::stringstream ss;

		if (includeItem->nodeType == AstNodeType_e::IncludeItemDecl)
		{
			if (auto n = reinterpret_cast<ast::IncludeItemDecl*>(includeItem))
			{
				if (n->scopePath)
				{
					ast::ScopedPathDecl* nextId = n->scopePath.get();
					while (nextId)
					{
						ss << nextId->identifier.str() << "::";
						nextId = nextId->scopedChildPath.get();
					}
				}
				ss << n->identifier.str();
			}
		}
		return ss.str();
	}

	Bool
	AstUtils::getExtendsFromClass(ast::AstNode* const decl, ast::AstNode** outDecl)
	{
		if (auto classDecl = reinterpret_cast<ast::ClassDecl*>(decl))
		{
			if (classDecl->baseClass)
			{
				if (classDecl->baseClass->nodeType == AstNodeType_e::NamedType)
				{
					if (auto baseClassType = reinterpret_cast<ast::TypeDeclNamed*>(classDecl->baseClass.get()))
					{
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}
		return true;
	}
} }