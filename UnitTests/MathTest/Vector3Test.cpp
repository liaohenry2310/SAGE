#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Sage::Math;

namespace MathTest
{
	TEST_CLASS(Vector3Test)
	{
	public:
		
		TEST_METHOD(TestConstructor)
		{
			Vector3 v;
			Assert::AreEqual(v.x, 0.0f, 0.0001f);
			Assert::AreEqual(v.y, 0.0f, 0.0001f);
			Assert::AreEqual(v.z, 0.0f, 0.0001f);
		}

		TEST_METHOD(TestPlusOperator)
		{
			Vector3 v1{ 1.0f, 0.0f, -10.0f };
			Vector3 v2{ 5.0f, -2.0f, 0.0f };
			Vector3 v = v1 + v2;

			Assert::AreEqual(v.x, 6.0f, 0.0001f);
			Assert::AreEqual(v.y, -2.0f, 0.0001f);
			Assert::AreEqual(v.z, -10.0f, 0.0001f);
		}

		TEST_METHOD(TestMinusOperator)
		{
			Vector3 v1{ 1.0f, 0.0f, -10.0f };
			Vector3 v2{ 5.0f, -2.0f, 0.0f };
			Vector3 v = v1 - v2;

			Assert::AreEqual(v.x, -4.0f, 0.0001f);
			Assert::AreEqual(v.y, 2.0f, 0.0001f);
			Assert::AreEqual(v.z, -10.0f, 0.0001f);
		}

		TEST_METHOD(TestNegateOperator)
		{
			Vector3 v1{ 1.0f, 0.0f, -10.0f };
			Vector3 v = -v1;

			Assert::AreEqual(v.x, -1.0f, 0.0001f);
			Assert::AreEqual(v.y, 0.0f, 0.0001f);
			Assert::AreEqual(v.z, 10.0f, 0.0001f);
		}

		TEST_METHOD(TestMultiplyOperator)
		{
			Vector3 v1{ 1.0f, 0.0f, -10.0f };
			Vector3 v = v1 * 5;

			Assert::AreEqual(v.x, 5.0f, 0.0001f);
			Assert::AreEqual(v.y, 0.0f, 0.0001f);
			Assert::AreEqual(v.z, -50.0f, 0.0001f);
		}

		TEST_METHOD(TestDivideOperator)
		{
			Vector3 v1{ 1.0f, 0.0f, -10.0f };
			Vector3 v = v1 / 5;

			Assert::AreEqual(v.x, 0.2f, 0.0001f);
			Assert::AreEqual(v.y, 0.0f, 0.0001f);
			Assert::AreEqual(v.z, -2.0f, 0.0001f);
		}

		TEST_METHOD(TestDivideOperator_DivideByZero)
		{
			Vector3 v1{ 1.0f, 0.0f, -10.0f };
			Vector3 v = v1 / 0;

			Assert::AreEqual(v.x, 1.0f, 0.0001f);
			Assert::AreEqual(v.y, 0.0f, 0.0001f);
			Assert::AreEqual(v.z, -10.0f, 0.0001f);
		}

		TEST_METHOD(TestVectorDot)
		{
			Vector3 v1{ 1.0f, 0.0f, -10.0f };
			Vector3 v2{ 5.0f, -2.0f, 0.0f };
			float v = Dot(v1, v2);

			Assert::AreEqual(v, 5.0f, 0.0001f);
		}

		TEST_METHOD(TestVectorMagnitudeSqr)
		{
			Vector3 v1{ 1.0f, 0.0f, -10.0f };
			float v = MagnitudeSqr(v1);

			Assert::AreEqual(v, 101.0f, 0.0001f);
		}

		TEST_METHOD(TestVectorMagnitude)
		{
			Vector3 v1{ 1.0f, 0.0f, -10.0f };
			float v = Magnitude(v1);

			Assert::AreEqual(v, 10.0499f, 0.0001f);
		}

		TEST_METHOD(TestVectorDistanceSqr)
		{
			Vector3 v1{ 1.0f, 0.0f, -10.0f };
			Vector3 v2{ 5.0f, -2.0f, 0.0f };
			float v = DistanceSqr(v1, v2);

			Assert::AreEqual(v, 120.0f, 0.0001f);
		}

		TEST_METHOD(TestVectorDistance)
		{
			Vector3 v1{ 1.0f, 0.0f, -10.0f };
			Vector3 v2{ 5.0f, -2.0f, 0.0f };
			float v = Distance(v1, v2);

			Assert::AreEqual(v, 10.9545f, 0.0001f);
		}

		TEST_METHOD(TestVectorNormalize)
		{
			Vector3 v1{ 1.0f, 0.0f, -10.0f };
			Vector3 v = Normalize(v1);

			Assert::AreEqual(v.x, 0.0995f, 0.0001f);
			Assert::AreEqual(v.y, 0.0f, 0.0001f);
			Assert::AreEqual(v.z, -0.9950f, 0.0001f);
		}

		TEST_METHOD(TestVectorCross)
		{
			Vector3 v1{ 1.0f, 0.0f, -10.0f };
			Vector3 v2{ 5.0f, -2.0f, 0.0f };
			Vector3 v = Cross(v1, v2);

			Assert::AreEqual(v.x, -20.0f, 0.0001f);
			Assert::AreEqual(v.y, -50.0f, 0.0001f);
			Assert::AreEqual(v.z, -2.0f, 0.0001f);
		}
	};
}
