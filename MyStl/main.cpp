#include "MemBase.h"
#include <iostream>
#include <vector>
using namespace std;

class A :public CMemBase
{
private:
	char c[8];
public:
	A() :c(){}
	void print()
	{
		cout << c << endl;
	}
};

class A1 :public CMemBase
{
private:
	char c[16];
public:
	A1() :c(){}
	void print()
	{
		cout << c << endl;
	}
};

class A128 :public CMemBase
{
private:
	char c[128];
public:
	A128() :c(){}
	void print()
	{
		cout << c << endl;
	}
};

int main()
{
	vector<A*>		va;
	vector<A1*>		va1;
	vector<A128*>	va128;
	for (int i = 0; i < 99; ++i)
	{
		int n = i % 3;
		if (n == 0)
		{
			va.push_back(new A);
		}
		else if (n == 1)
		{
			va1.push_back(new A1);
		}
		else
		{
			va128.push_back(new A128);
		}
	}

	for (int i = 0; i < 99; ++i)
	{
		int n = i % 3;
		if (n == 0)
		{
			delete *va.rbegin();
			va.pop_back();
		}
		else if (n == 1)
		{
			delete *va1.rbegin();
			va1.pop_back();
		}
		else
		{
			delete *va128.rbegin();
			va128.pop_back();
		}
	}
	return 0;
}