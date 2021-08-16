# FluffyScript

Toy script para estudo de compiladores.

TODO: Melhorar a qualidade das mensagens de erro.

  + Implementando uma forma de imprimir a linha onde o erro ocorreu.
  + Marcando o caractere onde o erro comecou.
  
Obs: Com a refatoracao do parser e mais facil testar cada recurso, pois agora o processamento
de cada producao esta separado por objeto com uma funcao estatica publica, responsavel pela
analise.

Obs: Temporariamente alguns parsers estao incompletos: principalmente o generic. Porem a correcao
vai ser aplicada quando o parser de tipos estiver completo, que sera implantado apos completar a declaracao
de classe.

Elaborado a versao inicial e provisoria da descricao formal da linguagem:

```
Program(OK)
	:	IncludeDeclList? NamespaceDecl?
	;
	
IncludeDeclList(OK)
	:	IncludeDecl IncludeDeclList?
	|	IncludeDecl
	;
	
IncludeDecl(OK)
	:	"include" "{" IncludeDeclArgList "}" "from" ScopedIdentifierDecl ";"
	|	"include" "{" "*" "}" "from" ScopedIdentifierDecl ";"
	;
	
IncludeDeclArgList(OK)
	:	Identifier "," IncludeDeclArgList?
	|	Identifier
	;
		
NamespaceDecl(OK)
	;	"namespace" Identifier "{" GlobalDeclList? "}"
	;
	
GlobalDeclList
	:	GlobalDecl GlobalDeclList?
	|	GlobalDecl
	;
	
GlobalDecl
	:	ExportDecl? ClassDecl
	|	ExportDecl? InterfaceDecl
	|	ExportDecl? StructDecl
	|	ExportDecl? EnumDecl
	|	ExportDecl? TraitDecl
	|	ExportDecl? VariableDecl
	|	ExportDecl? FunctionDecl
	;
	
ExportDecl
	:	"export"
	;
	
///////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////
	
ClassDecl
	:	"class" Identifier GenericDecl? ExtendsDecl? ImplementsDecl? ClassBlock
	;
	
ExtendsDecl
	:	"extends" NamedTypeDecl
	;
	
ImplementsDecl
	:	"implements" ImplementItemDeclList
	;
	
ImplementItemDeclList
	:	NamedTypeDecl "," ImplementItemDeclList?
	|	NamedTypeDecl
	;
	
ClassBlock
	:	"{" ClassItemDeclList? "}"
	;
	
ClassItemDeclList
	:	ClassItemDecl ClassItemDeclList?
	|	ClassItemDecl
	;
	
ClassItemDecl
	:	AccessModifierDecl? ClassStaticMember
	|	AccessModifierDecl? ClassMember
	|	AccessModifierDecl? ClassStaticFunction
	|	AccessModifierDecl? ClassFunction
	|	AccessModifierDecl? ClassContructor
	|	ClassDestructor
	;
	
AccessModifierDecl
	:	"public"
	|	"protected"
	|	"private"
	;
	
ClassStaticMember
	:	"static" "let" Identifier ":" TypeDecl "=" ExpressionDecl ";"
	|	"static" "let" Identifier ":" TypeDecl ";"
	|	"static" "let" Identifier "=" ExpressionDecl ";"
	|	"static" "const" Identifier ":" TypeDecl "=" ExpressionDecl ";"
	|	"static" "const" Identifier "=" ExpressionDecl ";"
	;
	
ClassMember
	:	"let" Identifier ":" TypeDecl "=" ExpressionDecl ";"
	|	"let" Identifier ":" TypeDecl ";"
	|	"let" Identifier "=" ExpressionDecl ";"
	|	"const" Identifier ":" TypeDecl "=" ExpressionDecl ";"
	|	"const" Identifier ":" TypeDecl ";"
	|	"const" Identifier "=" ExpressionDecl ";"
	;
	
ClassStaticFunction
	:	"static" "fn" Identifier "(" ArgListDecl? ")" ":" ReturnTypeDecl BlockDecl
	|	"static" "fn" Identifier "(" ArgListDecl? ")" BlockDecl
	;
	
ClassFunction
	:	"virtual" "fn" Identifier GenericDecl? "(" ArgListDecl? ")" ":" ReturnTypeDecl "override" BlockDecl
	|	"virtual" "fn" Identifier GenericDecl? "(" ArgListDecl? ")" "override" BlockDecl
	|	"virtual" "fn" Identifier GenericDecl? "(" ArgListDecl? ")" ":" ReturnTypeDecl "final" BlockDecl
	|	"virtual" "fn" Identifier GenericDecl? "(" ArgListDecl? ")" "final" BlockDecl
	|	"virtual" "fn" Identifier GenericDecl? "(" ArgListDecl? ")" ":" ReturnTypeDecl BlockDecl
	|	"virtual" "fn" Identifier GenericDecl? "(" ArgListDecl? ")" BlockDecl
	|	"abstract" "fn" Identifier GenericDecl? "(" ArgListDecl? ")" ":" ReturnTypeDecl ";"
	|	"abstract" "fn" Identifier GenericDecl? "(" ArgListDecl? ")" ";"
	|	"fn" Identifier GenericDecl? "(" ArgListDecl? ")" ":" ReturnTypeDecl BlockDecl
	|	"fn" Identifier GenericDecl? "(" ArgListDecl? ")" BlockDecl
	;
	
ClassContructor
	:	"constructor" "(" ArgListDecl? ")" BlockDecl
	;
	
ClassDestructor
	:	"destructor" "(" ")" BlockDecl
	;
	
///////////////////////////////////////////////////////////////////////////////
// Interface
///////////////////////////////////////////////////////////////////////////////
	
InterfaceDecl
	:	"interface" Identifier GenericDecl? "{" InterfaceItemDeclList? "}"
	;
	
InterfaceItemDeclList
	:	InterfaceItemDecl InterfaceItemDeclList?
	|	InterfaceItemDecl
	;
	
InterfaceItemDecl
	:	"fn" Identifier GenericDecl? "(" ArgListDecl? ")" ":" ReturnTypeDecl ";"
	|	"fn" Identifier GenericDecl? "(" ArgListDecl? ")" ";"
	;
	
///////////////////////////////////////////////////////////////////////////////
// Struct
///////////////////////////////////////////////////////////////////////////////
	
StructDecl
	:	"struct" Identifier GenericDecl? "{" StructItemDeclList "}"
	;
	
StructItemDeclList
	:	StructItemDecl StructItemDeclList?
	|	StructItemDecl
	;
	
StructItemDecl
	:	"let" Identifier ":" TypeDecl ";"
	;

///////////////////////////////////////////////////////////////////////////////
// Enum
///////////////////////////////////////////////////////////////////////////////
	
EnumDecl
	:	"enum" Identifier GenericDecl? "{" EnumItemDeclList? "}"
	;
	
EnumItemDeclList
	:	EnumItemDecl "," EnumItemDeclList?
	|	EnumItemDecl
	;
	
EnumItemDecl
	:	Identifier "(" EnumItemParamList ")"
	|	Identifier "=" ExpressionDecl
	|	Identifier
	;
	
EnumItemParamList
	:	ScopedIdentifierDecl "," EnumItemParamList?
	|	ScopedIdentifierDecl
	;

///////////////////////////////////////////////////////////////////////////////
// Trait
///////////////////////////////////////////////////////////////////////////////
	
TraitDecl
	:	TraitDeclDefinition
	|	TraitDeclImplementation
	;

TraitDeclDefinition
	:	"trait" Identifier GenericDecl? "{" TraitItemDeclDefinitionList "}"
	;
	
TraitItemDeclDefinitionList
	:	TraitItemDeclDefinition TraitItemDeclDefinitionList?
	|	TraitItemDeclDefinition
	;

TraitItemDeclDefinition
	:	"fn" Identifier GenericDecl? "(" ArgListDecl? ")" ":" ReturnTypeDecl ";"
	|	"fn" Identifier GenericDecl? "(" ArgListDecl? ")" ";"
	;
	
TraitDeclImplementation
	:	"trait" Identifier GenericDecl? "for" ScopedIdentifierDecl "{" TraitItemDeclImplementationList "}"
	;
	
TraitItemDeclImplementationList
	:	TraitItemDeclImplementation TraitItemDeclImplementationList?
	|	TraitItemDeclImplementation
	;

TraitItemDeclImplementation
	:	"fn" Identifier GenericDecl? "(" ArgListDecl? ")" ":" ReturnTypeDecl BlockDecl
	|	"fn" Identifier GenericDecl? "(" ArgListDecl? ")" BlockDecl
	;
	
///////////////////////////////////////////////////////////////////////////////
// Variable
///////////////////////////////////////////////////////////////////////////////

VariableDecl
	:	"let" Identifier ":" TypeDecl "=" ExpressionDecl ";"
	|	"let" Identifier ":" TypeDecl ";"
	|	"let" Identifier "=" ExpressionDecl ";"
	|	"const" Identifier ":" TypeDecl "=" ExpressionDecl ";"
	|	"const" Identifier "=" ExpressionDecl ";"
	;

///////////////////////////////////////////////////////////////////////////////
// Function
///////////////////////////////////////////////////////////////////////////////

FunctionDecl
	:	"fn" Identifier "(" ArgListDecl? ")" ":" ReturnTypeDecl BlockDecl
	|	"fn" Identifier "(" ArgListDecl? ")" BlockDecl
	;

///////////////////////////////////////////////////////////////////////////////
// Type
///////////////////////////////////////////////////////////////////////////////

TypeDecl
	:	"void"
	|	ParameterDecl
	;
	
ParameterDecl
	:	PrimitiveTypeDecl ArrayDecl? NullableOperator?
	|	CompoundTypeDecl ArrayDecl? NullableOperator?
	|	NamedTypeDecl ArrayDecl? NullableOperator?
	;
	
PrimitiveTypeDecl
	:	"bool"
	|	"i8"
	|	"u8"
	|	"i16"
	|	"u16"
	|	"i32"
	|	"u32"
	|	"i64"
	|	"u64"
	|	"fp32"
	|	"fp64"
	|	"string"
	|	"object"
	;
	
CompoundTypeDecl
	:	"set" "<" TypeDecl ">"
	|	"vector" "<" TypeDecl ">"
	|	"map" "<" TypeDecl ">"
	;
	
ArrayDecl
	:	"[" "]" ArrayDecl?
	|	"[" ConstantInteger "]" ArrayDecl?
	|	"[" "]"
	|	"[" ConstantInteger "]"
	;
	
NamedTypeDecl
	:	ScopeResolutionOp Identifier NamedTypeDecl? GenericDefinitionDecl?
	|	Identifier GenericDefinitionDecl?
	;

///////////////////////////////////////////////////////////////////////////////
// Misc
///////////////////////////////////////////////////////////////////////////////

ArgListDecl
	:	ArgDecl "," ArgListDecl?
	|	ArgDecl
	;
	
ArgDecl
	:	Identifier ":" TypeDecl "=" ExpressionDecl
	|	Identifier ":" TypeDecl
	;
	
GenericDecl
	:	"<" GenericParamsList ">"
	;
	
GenericParamsList
	:	GenericParams "," GenericParamsList
	|	GenericParams
	;
	
GenericParams
	:	Identifier WhereClause?
	|	Identifier
	;
	
WhereClause
	:	":" "where" Identifier "is" TypeDecl WhereClauseOr?
	;
	
WhereClauseOr	
	:	"|" TypeDecl
	;
	
GenericDefinitionDecl
	:	"<" GenericDefinitionDeclList ">"
	;
	
GenericDefinitionDeclList
	:	ReturnTypeDecl "," GenericDefinitionDeclList?
	|	ReturnTypeDecl
	;
	
ScopeResolutionOp
	:	"::"
	;
	
NullableOperator
	:	"?"
	;
	
ConstantInteger
	:	ConstantI8
	|	ConstantU8
	|	ConstantI16
	|	ConstantU16
	|	ConstantI32
	|	ConstantU32
	|	ConstantI64
	|	ConstantU64
	;

ScopedIdentifierDecl
	:	"::" Identifier ScopedIdentifierDecl?
	|	Identifier
	;
```
