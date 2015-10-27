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


//класс полинома, инты

template <class T>
class poly
{
private:
	vector <T> koef; //reversed
	size_t size;
	T non;
public: 
	poly(size_t _size, T _non): size(_size), non(_non) 
	{
		koef.assign(size, non);
	}
	T get(int i) 
	{
		return koef[i];
	}
	size_t get_size() 
	{
		return size;
	}
	void set_size(size_t _size) 
	{
		size = _size;
	}
	void set(int i, const T& value)
	{
		koef[i]=value;
	}

	void print()
	{
		for(int i = size-1;i>-1;i--)
		{
			if(koef[i]>non)
			{
				if(i<size-1) cout << "+";
				cout << koef[i];
				if (i==1) cout << "x";
				else 
					if(i>0) cout << "x^" << i;
			}
		}
	}
		
	poly<T> poly<T>::operator %(const poly<T>& b)
	{
		poly<T> second = b;
		size_t second_len = second.get_size();
		size_t first_len = size;
		float temp = 0;

		if(first_len==second_len)
		{
			temp = koef[second_len-1]*1.0/second.get(second_len-1);
			for(int j=second_len-1;j>-1;j--)
				{
					koef[j]-=temp*second.get(j);
				}
		}
		else
		{
			for(int i=first_len-second_len+1; i>-1, i>second_len-1; i--)
			{
				temp = koef[i+second_len-2]*1.0/second.get(i-second_len);
				for(int j=second_len-1;j>-1;j--)
				{
					koef[j+i-1]-=temp*second.get(j);
				}
			}
		}
		while(1)
		{
			if(koef[koef.size()-1]==0 && koef.size()>1)
			{
				vector<T>::iterator it = koef.begin();
				for(int i=1;i<koef.size();i++)
					it++;
				koef.erase(it);

			}
			else break;
		}
		size = koef.size();
		return *this;
	}
	bool poly<T>::operator ==(const poly<T> b) const
	{
		poly<T> ask = b;
		bool res = true;
		if(size-ask.get_size()==0)
			res = true;
		else return false;
			for(int i=0;i<size;i++)
			{
				if(ask.get(i)!=koef[i]) res = false;
			}
		return res;
	}


};


template <class T>
T gcd(const T& a, const T& b, const T& non)
{
	T c = a;
	if (b == non) return a;
	else return gcd (b, c % b, non);
}

void tester()
{
	int a = 0;
	int b = 0;
	for(int i = 0;i<3;i++)
	{
		a = (i+1)*i*i;
		b = (i+1)*(i+4)*(i+4);
		cout << "gcd(" << a << "," << b << ")=" << gcd(a,b,0) << endl;
	}

	poly<int> p(4,0);
	poly<int> q(2,0);
	poly<int> non(1,0);
	for(int i = 1;i<4;i++)
	{
		for(int j=0;j<4;j++)
			p.set(j,(i+1)*2*i);
		for(int j=0;j<2;j++)
			q.set(j,(i+3)*2*i);

		cout << "gcd(";
		p.print();
		cout << ",";
		q.print();
		cout << ")=";
		gcd(p,q,non).print();
		cout << endl;
	}

}


int main()
{

tester();

system("pause");
return 0;
}