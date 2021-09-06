#include <typeinfo>
#include "ast\fl_ast.h"
#include "ast\fl_ast_decl.h"
#include "utils\fl_ast_utils.h"
#include "validation\fl_validate_duplication.h"
#include "fl_exceptions.h"
namespace fluffy { namespace validations {
	/**
	 * StructureValidation
	 */

	DuplicatedValidation::DuplicatedValidation()
	{}

	DuplicatedValidation::~DuplicatedValidation()
	{}

	void
	DuplicatedValidation::validate(ast::CodeUnit* const codeUnit)
	{
		if (codeUnit == nullptr)
		{
			throw std::bad_alloc();
		}

		for (auto& nsDecl : codeUnit->namespaceDeclList)
		{
			for (auto& nsDeclCmp : codeUnit->namespaceDeclList)
			{
				if (utils::AstUtils::equals(nsDecl.get(), nsDeclCmp.get()))
				{
					throw exceptions::custom_exception(
						"%s: error duplicated identifier '%s' at: line %d, column %d",
						codeUnit->name.c_str(),
						nsDecl->identifier.str(),
						nsDecl->line,
						nsDecl->column
					);
				}
			}

			// Verifica se ha duplicacao no namespace.
			validateNamespace(codeUnit->name.c_str(), nsDecl.get());
		}
	}

	void
	DuplicatedValidation::validateNamespace(const I8* sourceFilename, ast::NamespaceDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}
		
		// Verifica se ha namespaces filhos duplicados.
		for (auto& nsDeclChild : decl->namespaceDeclList)
		{
			for (auto& nsDeclChildCmp : decl->namespaceDeclList)
			{
				if (utils::AstUtils::equals(nsDeclChild.get(), nsDeclChildCmp.get()))
				{
					throw exceptions::custom_exception(
						"%s: error duplicated identifier '%s' at: line %d, column %d",
						sourceFilename,
						nsDeclChildCmp->identifier.str(),
						nsDeclChildCmp->line,
						nsDeclChildCmp->column
					);
				}
			}

			// Verifica se ha duplicacao nos filhos.
			validateNamespace(sourceFilename, nsDeclChild.get());
		}

		for (auto& geDeclChild : decl->generalDeclList)
		{
			// Verifica a lista de filhos.
			for (auto& geDeclChildCmp : decl->generalDeclList)
			{
				if (utils::AstUtils::equals(geDeclChild.get(), geDeclChildCmp.get()))
				{
					throw exceptions::custom_exception(
						"%s: error duplicated identifier '%s' at: line %d, column %d",
						sourceFilename,
						geDeclChildCmp->identifier.str(),
						geDeclChildCmp->line,
						geDeclChildCmp->column
					);
				}
			}

			// Verifica nos filhos se ha duplicacoes.
			switch (geDeclChild->nodeType)
			{
			case AstNodeType_e::Class:
				validateClass(sourceFilename, dynamic_cast<ast::ClassDecl*>(geDeclChild.get()));
				break;
			case AstNodeType_e::Interface:
				validateInterface(sourceFilename, dynamic_cast<ast::InterfaceDecl*>(geDeclChild.get()));
				break;
			case AstNodeType_e::Struct:
				validateStruct(sourceFilename, dynamic_cast<ast::StructDecl*>(geDeclChild.get()));
				break;
			case AstNodeType_e::Trait:
				validateTrait(sourceFilename, dynamic_cast<ast::TraitDecl*>(geDeclChild.get()));
				break;
			case AstNodeType_e::Enum:
				validateEnum(sourceFilename, dynamic_cast<ast::EnumDecl*>(geDeclChild.get()));
				break;
			case AstNodeType_e::Function:
				validateFunction(sourceFilename, dynamic_cast<ast::FunctionDecl*>(geDeclChild.get()));
				break;
			default:
				break;
			}
		}
	}

	void
	DuplicatedValidation::validateClass(const I8* sourceFilename, ast::ClassDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}

		// Verificar a lista de interface.
		for (auto& interfaceDeclA : decl->interfaceList)
		{
			for (auto& interfaceDeclB : decl->interfaceList)
			{
				if (utils::AstUtils::equals(interfaceDeclA.get(), interfaceDeclB.get()))
				{
					ast::AstNodeIdentified* astNodeId = dynamic_cast<ast::AstNodeIdentified*>(interfaceDeclA.get());

					throw exceptions::custom_exception(
						"%s: error duplicated interface '%s' at class '%s' at: line %d, column %d",
						sourceFilename,
						astNodeId->identifier.str(), decl->identifier.str(),
						interfaceDeclB->line,
						interfaceDeclB->column
					);
				}
			}
		}

		// Verificar a lista de variaveis.
		for (auto& varA : decl->variableList)
		{
			// Verificar se ha duplicacoes nas variaveis.
			for (auto& varB : decl->variableList)
			{
				if (utils::AstUtils::equals(varA.get(), varB.get()))
				{
					throw exceptions::custom_exception(
						"%s: error duplicated identifier '%s' at class '%s' at: line %d, column %d",
						sourceFilename,
						varB->identifier.str(), decl->identifier.str(),
						varB->line,
						varB->column
					);
				}
			}

			// Verificar se ha duplicacoes nas funcoes.
			for (auto& funcB : decl->functionList)
			{
				if (varA->identifier == funcB->identifier)
				{
					throw exceptions::custom_exception(
						"%s: error duplicated identifier '%s' at class '%s' at: line %d, column %d",
						sourceFilename,
						funcB->identifier.str(), decl->identifier.str(),
						funcB->line,
						funcB->column
					);
				}
			}
		}

		// Verificar se ha duplicacoes nas funcoes.
		for (auto& funcA : decl->functionList)
		{
			for (auto& funcB : decl->functionList)
			{
				if (utils::AstUtils::equals(funcA.get(), funcB.get()))
				{
					throw exceptions::custom_exception(
						"%s: error duplicated identifier '%s' at class '%s' at: line %d, column %d",
						sourceFilename,
						funcB->identifier.str(), decl->identifier.str(),
						funcB->line,
						funcB->column
					);
				}
			}
			validateClassFunction(sourceFilename, funcA.get());
		}

		// Verificar se ha duplicacoes nas constructores.
		for (auto& constA : decl->constructorList)
		{
			for (auto& constB : decl->constructorList)
			{
				if (utils::AstUtils::equals(constA.get(), constB.get()))
				{
					throw exceptions::custom_exception(
						"%s: error duplicated constructors at class '%s' at: line %d, column %d",
						sourceFilename,
						decl->identifier.str(),
						constB->line,
						constB->column
					);
				}
			}
			validateClassConstructor(sourceFilename, constA.get());
		}
	}

	void
	DuplicatedValidation::validateClassFunction(const I8* sourceFilename, ast::ClassFunctionDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}
		auto identifiers = utils::AstUtils::extractFunctionIdentifiers(decl);

		// Validar bloco e validar parametros.
		if (decl->blockDecl)
		{
			auto blockIdentifiers = validateBlockAndExtractIdentifier(sourceFilename, decl->blockDecl.get());
			identifiers.insert(identifiers.end(), blockIdentifiers.begin(), blockIdentifiers.end());
		}

		// Valida os identificadores.
		for (auto& identifierA : identifiers)
		{
			for (auto& identifierB : identifiers)
			{
				auto idA = std::get<0>(identifierA);
				auto idB = std::get<0>(identifierB);

				auto lineA = std::get<1>(identifierA);
				auto lineB = std::get<1>(identifierB);

				auto columnA = std::get<2>(identifierA);
				auto columnB = std::get<2>(identifierB);

				if (idA == idB && (lineA != lineB || columnA != columnB))
				{
					throw exceptions::custom_exception(
						"%s: error duplicated identifier '%s' at class function '%s' at: line %d, column %d",
						sourceFilename,
						idA.str(), decl->identifier.str(),
						lineB,
						columnB
					);
				}
			}
		}
	}

	void
	DuplicatedValidation::validateClassConstructor(const I8* sourceFilename, ast::ClassConstructorDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}
		auto identifiers = utils::AstUtils::extractFunctionIdentifiers(decl);

		// Validar bloco e validar parametros.
		if (decl->blockDecl)
		{
			auto blockIdentifiers = validateBlockAndExtractIdentifier(sourceFilename, decl->blockDecl.get());
			identifiers.insert(identifiers.end(), blockIdentifiers.begin(), blockIdentifiers.end());
		}

		// Valida os identificadores.
		for (auto& identifierA : identifiers)
		{
			for (auto& identifierB : identifiers)
			{
				auto idA = std::get<0>(identifierA);
				auto idB = std::get<0>(identifierB);

				auto lineA = std::get<1>(identifierA);
				auto lineB = std::get<1>(identifierB);

				auto columnA = std::get<2>(identifierA);
				auto columnB = std::get<2>(identifierB);

				if (idA == idB && (lineA != lineB || columnA != columnB))
				{
					throw exceptions::custom_exception(
						"%s: error duplicated identifier '%s' at class constructor '%s' at: line %d, column %d",
						sourceFilename,
						idA.str(), decl->identifier.str(),
						lineB,
						columnB
					);
				}
			}
		}
	}

	void
	DuplicatedValidation::validateInterface(const I8* sourceFilename, ast::InterfaceDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}

		for (auto& funcA : decl->functionDeclList)
		{
			for (auto& funcB : decl->functionDeclList)
			{
				if (utils::AstUtils::equals(funcA.get(), funcB.get()))
				{
					throw exceptions::custom_exception(
						"%s: error duplicated identifier '%s' at interface '%s' at: line %d, column %d",
						sourceFilename,
						funcB->identifier.str(), decl->identifier.str(),
						funcB->line,
						funcB->column
					);
				}
			}
			validateInterfaceFunction(sourceFilename, funcA.get());
		}
	}

	void
	DuplicatedValidation::validateInterfaceFunction(const I8* sourceFilename, ast::InterfaceFunctionDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}
		auto parameters = utils::AstUtils::extractFunctionIdentifiers(decl);

		for (auto& paramA : parameters)
		{
			for (auto& paramB : parameters)
			{
				auto idA = std::get<0>(paramA);
				auto idB = std::get<0>(paramB);

				auto lineA = std::get<1>(paramA);
				auto lineB = std::get<1>(paramB);

				auto columnA = std::get<2>(paramA);
				auto columnB = std::get<2>(paramB);

				if (idA == idB && (lineA != lineB || columnA != columnB))
				{
					throw exceptions::custom_exception(
						"%s: error duplicated identifier '%s' at interface function '%s' at: line %d, column %d",
						sourceFilename,
						idA.str(), decl->identifier.str(),
						lineB,
						columnB
					);
				}
			}
		}
	}

	void
	DuplicatedValidation::validateStruct(const I8* sourceFilename, ast::StructDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}

		for (auto& varA : decl->variableList)
		{
			for (auto& varB : decl->variableList)
			{
				if (utils::AstUtils::equals(varA.get(), varB.get()))
				{
					throw exceptions::custom_exception(
						"%s: error duplicated identifier '%s' at struct '%s' at: line %d, column %d",
						sourceFilename,
						varB->identifier.str(), decl->identifier.str(),
						varB->line,
						varB->column
					);
				}
			}
		}
	}

	void
	DuplicatedValidation::validateTrait(const I8* sourceFilename, ast::TraitDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}

		for (auto& funcA : decl->functionDeclList)
		{
			for (auto& funcB : decl->functionDeclList)
			{
				if (utils::AstUtils::equals(funcA.get(), funcB.get()))
				{
					throw exceptions::custom_exception(
						"%s: error duplicated identifier '%s' at trait '%s' at: line %d, column %d",
						sourceFilename,
						funcB->identifier.str(), decl->identifier.str(),
						funcB->line,
						funcB->column
					);
				}
			}
			validateTraitFunction(sourceFilename, funcA.get());
		}
	}

	void
	DuplicatedValidation::validateTraitFunction(const I8* sourceFilename, ast::TraitFunctionDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}
		auto identifiers = utils::AstUtils::extractFunctionIdentifiers(decl);

		// Validar bloco e validar parametros.
		if (decl->blockDecl)
		{
			auto blockIdentifiers = validateBlockAndExtractIdentifier(sourceFilename, decl->blockDecl.get());
			identifiers.insert(identifiers.end(), blockIdentifiers.begin(), blockIdentifiers.end());
		}

		// Valida os identificadores.
		for (auto& identifierA : identifiers)
		{
			for (auto& identifierB : identifiers)
			{
				auto idA = std::get<0>(identifierA);
				auto idB = std::get<0>(identifierB);

				auto lineA = std::get<1>(identifierA);
				auto lineB = std::get<1>(identifierB);

				auto columnA = std::get<2>(identifierA);
				auto columnB = std::get<2>(identifierB);

				if (idA == idB && (lineA != lineB || columnA != columnB))
				{
					throw exceptions::custom_exception(
						"%s: error duplicated identifier '%s' at trait function '%s' at: line %d, column %d",
						sourceFilename,
						idA.str(), decl->identifier.str(),
						lineB,
						columnB
					);
				}
			}
		}
	}

	void
	DuplicatedValidation::validateEnum(const I8* sourceFilename, ast::EnumDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}

		for (auto& enumItemA : decl->enumItemDeclList)
		{
			for (auto& enumItemB : decl->enumItemDeclList)
			{
				if (enumItemA.get() != enumItemB.get() && enumItemA->identifier == enumItemB->identifier)
				{
					throw exceptions::custom_exception(
						"%s: error duplicated identifier '%s' at enum '%s' at: line %d, column %d",
						sourceFilename,
						enumItemB->identifier.str(), decl->identifier.str(),
						enumItemB->line,
						enumItemB->column
					);
				}
			}
		}
	}

	void
	DuplicatedValidation::validateFunction(const I8* sourceFilename, ast::FunctionDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}
		auto identifiers = utils::AstUtils::extractFunctionIdentifiers(decl);

		// Validar bloco e validar parametros.
		if (decl->blockDecl)
		{
			auto blockIdentifiers = validateBlockAndExtractIdentifier(sourceFilename, decl->blockDecl.get());
			identifiers.insert(identifiers.end(), blockIdentifiers.begin(), blockIdentifiers.end());
		}

		// Valida os identificadores.
		for (auto& identifierA : identifiers)
		{
			for (auto& identifierB : identifiers)
			{
				auto idA = std::get<0>(identifierA);
				auto idB = std::get<0>(identifierB);

				auto lineA = std::get<1>(identifierA);
				auto lineB = std::get<1>(identifierB);

				auto columnA = std::get<2>(identifierA);
				auto columnB = std::get<2>(identifierB);

				if (idA == idB && (lineA != lineB || columnA != columnB))
				{
					if (idA != "_")
					{
						throw exceptions::custom_exception(
							"%s: error duplicated identifier '%s' at function '%s' at: line %d, column %d",
							sourceFilename,
							idA.str(), decl->identifier.str(),
							lineB,
							columnB
						);
					}
				}
			}
		}
	}

	void
	DuplicatedValidation::validateIfStmt(const I8* sourceFilename, ast::stmt::StmtIfDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}
		std::vector<std::tuple<TString, U32, U32>> identifiers;

		// Validar bloco if.
		if (decl->ifBlockDecl)
		{
			auto blockIdentifiers = validateBlockAndExtractIdentifier(sourceFilename, decl->ifBlockDecl.get());
			identifiers.insert(identifiers.end(), blockIdentifiers.begin(), blockIdentifiers.end());

			// Valida os identificadores.
			for (auto& identifierA : identifiers)
			{
				for (auto& identifierB : identifiers)
				{
					auto idA = std::get<0>(identifierA);
					auto idB = std::get<0>(identifierB);

					auto lineA = std::get<1>(identifierA);
					auto lineB = std::get<1>(identifierB);

					auto columnA = std::get<2>(identifierA);
					auto columnB = std::get<2>(identifierB);

					if (idA == idB && (lineA != lineB || columnA != columnB))
					{
						throw exceptions::custom_exception(
							"%s: error duplicated identifier '%s'  at: line %d, column %d",
							sourceFilename,
							idB.str(),
							lineB,
							columnB
						);
					}
				}
			}
		}

		// Validar bloco e validar parametros.
		if (decl->elseBlockDecl)
		{
			identifiers = validateBlockAndExtractIdentifier(sourceFilename, decl->elseBlockDecl.get());

			// Valida os identificadores.
			for (auto& identifierA : identifiers)
			{
				for (auto& identifierB : identifiers)
				{
					auto idA = std::get<0>(identifierA);
					auto idB = std::get<0>(identifierB);

					auto lineA = std::get<1>(identifierA);
					auto lineB = std::get<1>(identifierB);

					auto columnA = std::get<2>(identifierA);
					auto columnB = std::get<2>(identifierB);

					if (idA == idB && (lineA != lineB || columnA != columnB))
					{
						throw exceptions::custom_exception(
							"%s: error duplicated identifier '%s' at: line %d, column %d",
							sourceFilename,
							idB.str(),
							lineB,
							columnB
						);
					}
				}
			}
		}
	}

	void
	DuplicatedValidation::validateIfLetStmt(const I8* sourceFilename, ast::stmt::StmtIfLetDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}
		std::vector<std::tuple<TString, U32, U32>> pattIdentifiers = utils::AstUtils::extractPatternIdentifiers(decl->patternDecl.get());

		// Validar bloco if.
		if (decl->ifBlockDecl)
		{
			std::vector<std::tuple<TString, U32, U32>> identifiers(pattIdentifiers.begin(), pattIdentifiers.end());

			auto blockIdentifiers = validateBlockAndExtractIdentifier(sourceFilename, decl->ifBlockDecl.get());
			identifiers.insert(identifiers.end(), blockIdentifiers.begin(), blockIdentifiers.end());

			// Valida os identificadores.
			for (auto& identifierA : identifiers)
			{
				for (auto& identifierB : identifiers)
				{
					auto idA = std::get<0>(identifierA);
					auto idB = std::get<0>(identifierB);

					auto lineA = std::get<1>(identifierA);
					auto lineB = std::get<1>(identifierB);

					auto columnA = std::get<2>(identifierA);
					auto columnB = std::get<2>(identifierB);

					if (idA == idB && (lineA != lineB || columnA != columnB))
					{
						throw exceptions::custom_exception(
							"%s: error duplicated identifier '%s' at: line %d, column %d",
							sourceFilename,
							idB.str(),
							lineB,
							columnB
						);
					}
				}
			}
		}

		// Validar bloco e validar parametros.
		if (decl->elseBlockDecl)
		{
			std::vector<std::tuple<TString, U32, U32>> identifiers(pattIdentifiers.begin(), pattIdentifiers.end());

			auto blockIdentifiers = validateBlockAndExtractIdentifier(sourceFilename, decl->elseBlockDecl.get());
			identifiers.insert(identifiers.end(), blockIdentifiers.begin(), blockIdentifiers.end());

			// Valida os identificadores.
			for (auto& identifierA : identifiers)
			{
				for (auto& identifierB : identifiers)
				{
					auto idA = std::get<0>(identifierA);
					auto idB = std::get<0>(identifierB);

					auto lineA = std::get<1>(identifierA);
					auto lineB = std::get<1>(identifierB);

					auto columnA = std::get<2>(identifierA);
					auto columnB = std::get<2>(identifierB);

					if (idA == idB && (lineA != lineB || columnA != columnB))
					{
						throw exceptions::custom_exception(
							"%s: error duplicated identifier '%s' at: line %d, column %d",
							sourceFilename,
							idB.str(),
							lineB,
							columnB
						);
					}
				}
			}
		}
	}

	void
	DuplicatedValidation::validateForStmt(const I8* sourceFilename, ast::stmt::StmtForDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}
		std::vector<std::tuple<TString, U32, U32>> identifiers;

		if (decl->initStmtDecl)
		{
			identifiers.push_back(std::tuple<TString, U32, U32>(
				decl->initStmtDecl->identifier,
				decl->initStmtDecl->line,
				decl->initStmtDecl->column
			));
		}

		// Validar bloco.
		if (decl->blockDecl)
		{
			auto blockIdentifiers = validateBlockAndExtractIdentifier(sourceFilename, decl->blockDecl.get());
			identifiers.insert(identifiers.end(), blockIdentifiers.begin(), blockIdentifiers.end());

			// Valida os identificadores.
			for (auto& identifierA : identifiers)
			{
				for (auto& identifierB : identifiers)
				{
					auto idA = std::get<0>(identifierA);
					auto idB = std::get<0>(identifierB);

					auto lineA = std::get<1>(identifierA);
					auto lineB = std::get<1>(identifierB);

					auto columnA = std::get<2>(identifierA);
					auto columnB = std::get<2>(identifierB);

					if (idA == idB && (lineA != lineB || columnA != columnB))
					{
						throw exceptions::custom_exception(
							"%s: error duplicated identifier '%s' at: line %d, column %d",
							sourceFilename,
							idB.str(),
							lineB,
							columnB
						);
					}
				}
			}
		}
	}

	void
	DuplicatedValidation::validateWhileStmt(const I8* sourceFilename, ast::stmt::StmtWhileDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}
		std::vector<std::tuple<TString, U32, U32>> identifiers;

		// Validar bloco.
		if (decl->blockDecl)
		{
			identifiers = validateBlockAndExtractIdentifier(sourceFilename, decl->blockDecl.get());

			// Valida os identificadores.
			for (auto& identifierA : identifiers)
			{
				for (auto& identifierB : identifiers)
				{
					auto idA = std::get<0>(identifierA);
					auto idB = std::get<0>(identifierB);

					auto lineA = std::get<1>(identifierA);
					auto lineB = std::get<1>(identifierB);

					auto columnA = std::get<2>(identifierA);
					auto columnB = std::get<2>(identifierB);

					if (idA == idB && (lineA != lineB || columnA != columnB))
					{
						throw exceptions::custom_exception(
							"%s: error duplicated identifier '%s' at: line %d, column %d",
							sourceFilename,
							idB.str(),
							lineB,
							columnB
						);
					}
				}
			}
		}
	}

	void
	DuplicatedValidation::validateDoWhileStmt(const I8* sourceFilename, ast::stmt::StmtDoWhileDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}
		std::vector<std::tuple<TString, U32, U32>> identifiers;

		// Validar bloco.
		if (decl->blockDecl)
		{
			identifiers = validateBlockAndExtractIdentifier(sourceFilename, decl->blockDecl.get());

			// Valida os identificadores.
			for (auto& identifierA : identifiers)
			{
				for (auto& identifierB : identifiers)
				{
					auto idA = std::get<0>(identifierA);
					auto idB = std::get<0>(identifierB);

					auto lineA = std::get<1>(identifierA);
					auto lineB = std::get<1>(identifierB);

					auto columnA = std::get<2>(identifierA);
					auto columnB = std::get<2>(identifierB);

					if (idA == idB && (lineA != lineB || columnA != columnB))
					{
						throw exceptions::custom_exception(
							"%s: error duplicated identifier '%s' at: line %d, column %d",
							sourceFilename,
							idB.str(),
							lineB,
							columnB
						);
					}
				}
			}
		}
	}

	void
	DuplicatedValidation::validateTryStmt(const I8* sourceFilename, ast::stmt::StmtTryDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}
		std::vector<std::tuple<TString, U32, U32>> identifiers;

		// Validar bloco.
		if (decl->blockDecl)
		{
			identifiers = validateBlockAndExtractIdentifier(sourceFilename, decl->blockDecl.get());

			// Valida os identificadores.
			for (auto& identifierA : identifiers)
			{
				for (auto& identifierB : identifiers)
				{
					auto idA = std::get<0>(identifierA);
					auto idB = std::get<0>(identifierB);

					auto lineA = std::get<1>(identifierA);
					auto lineB = std::get<1>(identifierB);

					auto columnA = std::get<2>(identifierA);
					auto columnB = std::get<2>(identifierB);

					if (idA == idB && (lineA != lineB || columnA != columnB))
					{
						throw exceptions::custom_exception(
							"%s: error duplicated identifier '%s' at: line %d, column %d",
							sourceFilename,
							idB.str(),
							lineB,
							columnB
						);
					}
				}
			}
		}
	}

	void
	DuplicatedValidation::validateMatchStmt(const I8* sourceFilename, ast::stmt::StmtMatchDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}

		for (auto& whenClause : decl->whenDeclList)
		{
			std::vector<std::tuple<TString, U32, U32>> identifiers = utils::AstUtils::extractPatternIdentifiers(whenClause->patternDecl.get());

			if (whenClause->blockDecl)
			{
				auto blockIdentifiers = validateBlockAndExtractIdentifier(sourceFilename, whenClause->blockDecl.get());
				identifiers.insert(identifiers.end(), blockIdentifiers.begin(), blockIdentifiers.end());
			}

			// Valida os identificadores.
			for (auto& identifierA : identifiers)
			{
				for (auto& identifierB : identifiers)
				{
					auto idA = std::get<0>(identifierA);
					auto idB = std::get<0>(identifierB);

					auto lineA = std::get<1>(identifierA);
					auto lineB = std::get<1>(identifierB);

					auto columnA = std::get<2>(identifierA);
					auto columnB = std::get<2>(identifierB);

					if (idA == idB && (lineA != lineB || columnA != columnB))
					{
						throw exceptions::custom_exception(
							"%s: error duplicated identifier '%s' at: line %d, column %d",
							sourceFilename,
							idB.str(),
							lineB,
							columnB
						);
					}
				}
			}
		}
	}

	void
	DuplicatedValidation::validateFunctionDeclExpr(const I8* sourceFilename, ast::expr::ExpressionFunctionDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}
		std::vector<std::tuple<TString, U32, U32>> identifiers = utils::AstUtils::extractFunctionIdentifiers(decl);

		// Validar bloco.
		if (decl->blockDecl)
		{
			identifiers = validateBlockAndExtractIdentifier(sourceFilename, decl->blockDecl.get());

			// Valida os identificadores.
			for (auto& identifierA : identifiers)
			{
				for (auto& identifierB : identifiers)
				{
					auto idA = std::get<0>(identifierA);
					auto idB = std::get<0>(identifierB);

					auto lineA = std::get<1>(identifierA);
					auto lineB = std::get<1>(identifierB);

					auto columnA = std::get<2>(identifierA);
					auto columnB = std::get<2>(identifierB);

					if (idA == idB && (lineA != lineB || columnA != columnB))
					{
						throw exceptions::custom_exception(
							"%s: error duplicated identifier '%s' at: line %d, column %d",
							sourceFilename,
							idB.str(),
							lineB,
							columnB
						);
					}
				}
			}
		}
	}

	void
	DuplicatedValidation::validateAnomClassDeclExpr(const I8* sourceFilename, ast::expr::ExpressionAnomClassDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}

		// Verificar a lista de variaveis.
		for (auto& varA : decl->variableList)
		{
			// Verificar se ha duplicacoes nas variaveis.
			for (auto& varB : decl->variableList)
			{
				if (utils::AstUtils::equals(varA.get(), varB.get()))
				{
					throw exceptions::custom_exception(
						"%s: error duplicated identifier '%s' in anom class at: line %d, column %d",
						sourceFilename,
						varB->identifier.str(),
						varB->line,
						varB->column
					);
				}
			}

			// Verificar se ha duplicacoes nas funcoes.
			for (auto& funcB : decl->functionList)
			{
				if (varA->identifier == funcB->identifier)
				{
					throw exceptions::custom_exception(
						"%s: error duplicated identifier '%s' in anom class at: line %d, column %d",
						sourceFilename,
						funcB->identifier.str(),
						funcB->line,
						funcB->column
					);
				}
			}
		}

		// Verificar se ha duplicacoes nas funcoes.
		for (auto& funcA : decl->functionList)
		{
			for (auto& funcB : decl->functionList)
			{
				if (utils::AstUtils::equals(funcA.get(), funcB.get()))
				{
					throw exceptions::custom_exception(
						"%s: error duplicated identifier '%s' in anom class at: line %d, column %d",
						sourceFilename,
						funcB->identifier.str(),
						funcB->line,
						funcB->column
					);
				}
			}
			validateClassFunction(sourceFilename, funcA.get());
		}

		// Verificar se ha duplicacoes nas constructores.
		for (auto& constA : decl->constructorList)
		{
			for (auto& constB : decl->constructorList)
			{
				if (utils::AstUtils::equals(constA.get(), constB.get()))
				{
					throw exceptions::custom_exception(
						"%s: error duplicated constructors in anom class at: line %d, column %d",
						sourceFilename,
						constB->line,
						constB->column
					);
				}
			}
			validateClassConstructor(sourceFilename, constA.get());
		}
	}

	std::vector<std::tuple<TString, U32, U32>>
	DuplicatedValidation::validateBlockAndExtractIdentifier(const I8* sourceFilename, ast::BlockDecl* const decl)
	{
		if (decl == nullptr)
		{
			throw std::bad_alloc();
		}
		std::vector<std::tuple<TString, U32, U32>> identifiers;

		for (auto& stmt : decl->stmtList)
		{
			if (stmt->nodeType == AstNodeType_e::VariableStmt)
			{
				if (auto variable = reinterpret_cast<ast::stmt::StmtVariableDecl*>(stmt.get()))
				{
					if (variable->patternDecl)
					{
						auto variableIdentifiers = utils::AstUtils::extractPatternIdentifiers(variable->patternDecl.get());
						identifiers.insert(identifiers.end(), variableIdentifiers.begin(), variableIdentifiers.end());
					}
					else
					{
						identifiers.push_back(std::tuple<TString, U32, U32>(
							variable->identifier,
							variable->line,
							variable->column
						));
					}

					if (variable->initExpression && variable->initExpression->nodeType == AstNodeType_e::FunctionDeclExpr)
					{
						validateFunctionDeclExpr(sourceFilename, reinterpret_cast<ast::expr::ExpressionFunctionDecl*>(variable->initExpression.get()));
					}
					else if (variable->initExpression && variable->initExpression->nodeType == AstNodeType_e::AnomClassDeclExpr)
					{
						validateAnomClassDeclExpr(sourceFilename, reinterpret_cast<ast::expr::ExpressionAnomClassDecl*>(variable->initExpression.get()));
					}
				}
			}

			if (stmt->nodeType == AstNodeType_e::IfStmt)
			{
				validateIfStmt(sourceFilename, reinterpret_cast<ast::stmt::StmtIfDecl*>(stmt.get()));
			}

			if (stmt->nodeType == AstNodeType_e::IfLetStmt)
			{
				validateIfLetStmt(sourceFilename, reinterpret_cast<ast::stmt::StmtIfLetDecl*>(stmt.get()));
			}

			if (stmt->nodeType == AstNodeType_e::ForStmt)
			{
				validateForStmt(sourceFilename, reinterpret_cast<ast::stmt::StmtForDecl*>(stmt.get()));
			}

			if (stmt->nodeType == AstNodeType_e::WhileStmt)
			{
				validateWhileStmt(sourceFilename, reinterpret_cast<ast::stmt::StmtWhileDecl*>(stmt.get()));
			}

			if (stmt->nodeType == AstNodeType_e::DoWhileStmt)
			{
				validateDoWhileStmt(sourceFilename, reinterpret_cast<ast::stmt::StmtDoWhileDecl*>(stmt.get()));
			}

			if (stmt->nodeType == AstNodeType_e::TryStmt)
			{
				validateTryStmt(sourceFilename, reinterpret_cast<ast::stmt::StmtTryDecl*>(stmt.get()));
			}

			if (stmt->nodeType == AstNodeType_e::MatchStmt)
			{
				validateMatchStmt(sourceFilename, reinterpret_cast<ast::stmt::StmtMatchDecl*>(stmt.get()));
			}
		}
		return identifiers;
	}
} }