#ifndef SIGMA_TESTS
#define SIGMA_TESTS

#include "MakeChai.h"

#include <iostream>
#include <string>
#include <exception>
#include <assert.h>

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

#define print(x) std::cout << x << std::endl

#define CEVAL(c, s) c.eval(s, chaiscript::exception_specification<std::exception>());
#define CEVAL_T(c, t, s) c.eval<t>(s, chaiscript::exception_specification<std::exception>());

namespace sig {

	class Test
	{
	public:
		virtual void Run(chaiscript::ChaiScript&) = 0;
	};

	class ScriptRegisteringTest : public Test
	{
	public:
		void Run(chaiscript::ChaiScript& c) {
			try {
				CEVAL(c, "var v = Vector2(); print(v.get_x());");
				print("ScriptRegisteringTest: OK");
			} catch (const std::exception &e) {
				print(e.what());
			}
		}
	};

	class GameLogicTest : public Test
	{
	public:
		void Run(chaiscript::ChaiScript& c) {
			try {
				GameLogic *g = CEVAL_T(c, GameLogic*,	"var g = GameLogic();\n"
														"g.OnInitialize = fun() {\n"
														"	print(\"Hello!\");\n"
														"};\n"
														"return g;");
				assert(g != nullptr);
				print("GameLogicTest: OK");
			} catch (const std::exception &e) {
				print(e.what());
			}
		}
	};

	void run_tests() {
		chaiscript::ChaiScript chai(chaiscript::Std_Lib::library());
		chai::RegisterEngineClasses(chai);

		print("Running: ScriptRegisteringTest");
		ScriptRegisteringTest srt;
		srt.Run(chai);

		print("Running: GameLogicTest");
		GameLogicTest gt;
		gt.Run(chai);
	}

}

#endif // SIGMA_TESTS
