#include "pch.h"
#include "CppUnitTest.h"
#include "../lab_11_5/lab_11_5.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			ofstream f("testing.dat", ios::binary);
			Product prod;
			strcpy_s(prod.goods.name, "Beer");
			prod.goods.del_time = 2;
			f.write((char*)&prod, sizeof(Product));
			f.close();
			ifstream file("testing.dat", ios::binary);
			bool x = FiltrDays("testing.dat", "Beer");
			Assert::AreEqual(x, true);
			f.close();
		}
	};
}
