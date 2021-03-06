# FluffyScript

Toy script para estudo de compiladores.

TODO: Melhorar a qualidade das mensagens de erro**.
OBS: Priorizar qualidade das mensagens de erro para facilitar testes.

  + Implementando uma forma de imprimir a linha onde o erro ocorreu.
  + Marcando o caractere onde o erro comecou.
  
Obs: Com a refatoracao do parser e mais facil testar cada recurso, pois agora o processamento
de cada producao esta separado por objeto com uma funcao estatica publica, responsavel pela
analise.

Obs: Temporariamente alguns parsers estao incompletos: principalmente o generic. Porem a correcao
vai ser aplicada quando o parser de tipos estiver completo, que sera implantado apos completar a declaracao
de classe.

Obs: Ocorreu um grande problema na analise de expressoes, uma ambiguidade entre a definicao de generics e os operadores < e >,
que ate o momento nao encontrei uma solucao possivel com apenas 1 fase de analise, exemplo:

Sendo foo um objeto com um funcao com generic a seguinte expressao pode ser facilmente interpretada de 2 formas:

```
	foo.count<Moo>(2);
```
	
Primeiro: o objeto <foo> acessa a propriedade <count> e compara com o identificador Moo e compara novamente com a expressao <(2)>.

Segundo: o objeto <foo> acessa a funcao <count> e passa <Moo> como generic, chamando a funcao com o parametro <2>.

Uma possivel solucao que estou estudando, seria dividir o processo de gerar a AST em 2 fases, na primeira processado apenas os elementos
estruturais(classes, interfaces, etc...) e junto seria criado um mapeamento de todos os tipo para considerar o escopo para a 2 fase de analise,
onde seria analisado os trechos onde houver codigo.


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
	
GlobalDeclList(OK)
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
	
ExportDecl(OK)
	:	"export"
	;
	
///////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////
	
ClassDecl(OK)
	:	"class" Identifier GenericDecl? ExtendsDecl? ImplementsDecl? ClassBlock
	;
	
ExtendsDecl(OK)
	:	"extends" NamedTypeDecl
	;
	
ImplementsDecl)OK_
	:	"implements" ImplementItemDeclList
	;
	
ImplementItemDeclList(OK)
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
	:	"static" "let" ReferenceModifier? Identifier ":" TypeDecl "=" ExpressionDecl ";"
	|	"static" "let" ReferenceModifier? Identifier ":" TypeDecl ";"
	|	"static" "let" ReferenceModifier? Identifier "=" ExpressionDecl ";"
	|	"static" "const" ReferenceModifier? Identifier ":" TypeDecl "=" ExpressionDecl ";"
	|	"static" "const" ReferenceModifier? Identifier "=" ExpressionDecl ";"
	;
	
ClassMember
	:	"let" ReferenceModifier? Identifier ":" TypeDecl "=" ExpressionDecl ";"
	|	"let" ReferenceModifier? Identifier ":" TypeDecl ";"
	|	"let" ReferenceModifier? Identifier "=" ExpressionDecl ";"
	|	"const" ReferenceModifier? Identifier ":" TypeDecl "=" ExpressionDecl ";"
	|	"const" ReferenceModifier? Identifier ":" TypeDecl ";"
	|	"const" ReferenceModifier? Identifier "=" ExpressionDecl ";"
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
	:	ReferenceModifier? Identifier ":" TypeDecl "=" ExpressionDecl
	|	ReferenceModifier? Identifier ":" TypeDecl
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
	
ReferenceModifier
	:	"ref"
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
