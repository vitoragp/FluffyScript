#include <iostream>
#include "ast\fl_ast_decl.h"
#include "transformation\fl_transformation_resolve_include.h"
#include "transformation\fl_transformation_resolve_types.h"
#include "fl_compiler.h"
#include "fl_exceptions.h"

int main()
{
	std::unique_ptr<fluffy::Compiler> compiler = std::unique_ptr<fluffy::Compiler>(new fluffy::Compiler());

	compiler->initialize();
	compiler->applyTransformation(new fluffy::transformations::ResolveInclude());
	compiler->applyTransformation(new fluffy::transformations::ResolveTypes());

	compiler->addBlockToBuild("source1",
		"include { foo::FooOne } in \"source2\"; \n"
		"namespace app { \n"
			"let a: FooOne; \n"
			"let b: subapp::FooOne; \n"
			"namespace subapp { \n"
				"class FooOne {} \n"
			"} \n"
		"} \n"
	);

	compiler->addBlockToBuild("source2",
		"namespace foo { \n"
			"export class FooOne {}"
		"} \n"
	);

	compiler->build();
	return 0;
}
