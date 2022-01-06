#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CoreTest
{
	TEST_CLASS(DebugTest)
	{
	public:
		
		TEST_METHOD(DebugLog)
		{
			LOG("Debug Log Test");
		}
	};
}
