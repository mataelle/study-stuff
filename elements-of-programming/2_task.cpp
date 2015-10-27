#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
using namespace std;

#define REQUIRE(cond, message)\
	if(cond){\
	cout << message <<endl;\
	}



template <class T, template <class> class BOp>
T pow (const T& a, size_t N, BOp<T> f)
{
	if(N == 1) return a;
	if (N%2 == 1) 
		return f(a, pow(a, N-1,f));
	else 
	{
		T t = pow(a, N/2, f);
		return f(t,t);
	}
}


//-----------------//numbers//--------------------

template <class T>
class Multiplication
{
	public:
	T const operator() (const T a,const T b)
		{
			return a*b;
		};
};

long dummy_num_pow(long a, long n)
{
	long res = 1;
	for(int i=0;i<n;i++)
		res*=a;
	return res;
};

void test_int()
{
	Multiplication<int> binary_operation; 
	for(int i=1;i<10;i++)
		for(int j=1;j<10;j++)
			REQUIRE(pow(i,j,binary_operation)==dummy_num_pow(i,j), "test_int done!");
};


//----------------//matr-test//--------------------------

class matrix
{
private:
	int n;
	vector < vector<long long> > matr;
public: 
	matrix(int _n): n(_n)
	{
		matr.resize (n+1, vector<long long> (1<<n));
		for(int i=0;i<n;i++)
			for(int j=0;j<n;j++)
				matr[i][j]=0;

	}
	const matrix matrix::operator *(const matrix b) const
	{
		assert(n==b.get_length());
		matrix res(n);

		for(int i=0;i<n;i++)
			for(int j=0;j<n;j++)
			{
				res.set(i,j,0);
				for(int r=0;r<n;r++)
					res.set(i,j, res.get(i,j)+matr[i][r]*b.get(r,j));
			}
		return res;
	}

	bool matrix::operator ==(const matrix b)
	{
		if(n!=b.get_length()) return false;
		for(int i=0;i<n;i++)
			for(int j=0;j<n;j++)
				if(matr[i][j]!=b.get(i,j)) return false;
		return true;
	}
	int get_length() const
		{
			return n;
		};
	long long get(int i, int j) const
		{
			return matr[i][j];
		};
	void set(int i, int j, long long val)
		{
			matr[i][j]=val;
		};
};

matrix dummy_matr_pow(matrix a, int j)
{
	matrix res=a;
	for(int i=1;i<j;i++)
		res = res*a;
	return res;
}

void test_map()
{
	int n = 5;
	Multiplication<matrix> binary_operation; 
	for(int i=2;i<=n;i++)
		for(int j=1;j<=n;j++)
		{
			matrix d(i);
			d.set(0,0,1);
			REQUIRE(pow(d,j,binary_operation)==dummy_matr_pow(d,j), "test_map done!");
		}
}


//-----------------------//end//------------------

int main()
{
	test_int();
	test_map();

	system("pause");
	return 0;
}