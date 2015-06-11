#include <memory>
#include <vector>



struct point
{
	int x;
	int y;
};


// Test没有默认构造函数，无法value初始化和default初始化
class Test
{
public:
	Test(int x)
		: a(x),
		  p(nullptr)
	{
	}

	const int *getA() const
	{
		return &a;
	}

	// 返回值可以不用试const point *，因为p是顶层const，顶层const可以忽略，
	// 如果返回&p，是底层const，就要求是const point **
	point *getP() const
	{
		return p;
	}

	int a;
	point *p;
};


using namespace std;

int main(int argc, char *argv[])
{
    Test ta(1);
	const Test tb(888);
	//Test tc; //error

	int *pa1 = &ta.a;

	const int *pa2 = &tb.a;

	shared_ptr<int> sp1(new int(1));

	int (*pi) = new int(1);
	// shared_ptr<int>(pi); //error：重复定义
	{
	shared_ptr<int>(pi);
	}

	vector<Test> vec(5, 888);
	// vector<Test> vec(5); //error：无法值初始化
	
    return 0;
}
