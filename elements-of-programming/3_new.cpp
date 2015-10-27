#include <cstdio>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
using namespace std;

template <class T>
class Polynomial
{
private:
	vector <T> coef_; 
	size_t size;
	T non;
public: 
	Polynomial(size_t _size, T _non): size(_size), non(_non) 
	{
		coef_.assign(size, non);
	}

	Polynomial(size_t _size, T _non, const vector<T> values): size(_size), non(_non)
	{
		coef_.assign(size, non);
		if(size-values.size()==1 && size>1)
		{
			coef_[0]=1;
			int border = size;
			for(int i=0;i<border-1;i++)
			{
				Polynomial<T> temp(2, non);
				temp.set(1,1);
				temp.set(0,0-values[i]);

				*this = (*this)*temp;
			}
		}
		else if(size==1)
		{
			coef_[0]=1;
		}
	}

	T get_non()
	{
		return non;
	}
	T get(int i) const 
	{
		assert(i<size);
		return coef_[i];

	}
	size_t get_size() const
	{
		return size;
	}
	void set_size(size_t _size) 
	{
		if(size<_size)
		{
			size = _size;
			coef_.resize(size);
		}
	}
	void set(int i, const T& value)
	{
		coef_[i]=value;
	}


	void Polynomial::clear()
	{
		while(coef_[coef_.size()-1]==0 && coef_.size()>1)
		{
			vector<T>::iterator it = coef_.begin();
				for(int i=1;i<coef_.size();i++)
					it++;
				coef_.erase(it);
		}
		size = coef_.size();
	}

	void Polynomial<T>::normalize()
	{
		if(coef_[size-1]<0)
		{
			for(int i=0;i<size;i++)
				coef_[i]=-coef_[i];
		}
		if(coef_[size-1]>1)
		{
			T temp = coef_[size-1];
			bool flag = true;
			for(int i=0;i<size;i++)
				if(coef_[i]%temp!=0) flag=false; 
			if(flag==true)
				for(int i=0;i<size;i++)
					coef_[i]=coef_[i]/temp;
		}
	}
	Polynomial<T> Polynomial<T>::operator %(const Polynomial<T>& b)
	{
		Polynomial<T> second = b;
		Polynomial<T> answer = *this;

		size_t second_len = second.get_size();
		size_t first_len = answer.get_size();
		float temp = 0;

		if(first_len==second_len)
		{
			temp = answer.get(second_len-1)*1.0/second.get(second_len-1);
			for(int j=second_len-1;j>-1;j--)
				{
					answer.set(j,answer.get(j)-temp*second.get(j));
				}
		}
		else if (first_len>second_len)
		{
			if(second_len>2)
			for(int i=first_len-second_len+1; i>-1, i>=second_len-1; i--)
			{
				temp = answer.get(i+second_len-2)*1.0/second.get(second_len-1);
				for(int j=second_len-1;j>-1;j--)
				{
					answer.set(j+i-1, answer.get(j+i-1) - temp*second.get(j));
				}
			}
			else 
			for(int i=first_len-second_len+1; i>-1, i>=second_len; i--)
			{
				temp = answer.get(i+second_len-2)*1.0/second.get(second_len-1);
				for(int j=second_len-1;j>-1;j--)
				{
					answer.set(j+i-1,answer.get(j+i-1)-temp*second.get(j));
				}
			}

		}

		answer.clear();
		answer.normalize();
		return answer;
	}
	bool Polynomial<T>::operator >(const Polynomial<T>& ask) const
	{	
		bool res = true;
		if(size<ask.get_size())
			return false;
		if(size>ask.get_size())
			return true;
		for(int i=size-1;i>-1;i--)
			if(coef_[i]>ask.get(i)) return true;
			else if(coef_[i]<ask.get(i)) return false;
		return false;
	}

	bool Polynomial<T>::operator <(const Polynomial<T>& ask) const
	{	
		bool res = true;
		if(size>ask.get_size())
			return false;
		if(size<ask.get_size())
			return true;
		for(int i=size-1;i>-1;i--)
			if(coef_[i]<ask.get(i)) return true;
			else if(coef_[i]<ask.get(i)) return false;
		return false;
	}



	bool Polynomial<T>::operator ==(const Polynomial<T>& ask) const
	{
		bool res = true;
		if(size-ask.get_size()==0)
			res = true;
		else return false;
			for(int i=0;i<size;i++)
			{
				if(ask.get(i)!=coef_[i]) res = false;
			}
		return res;
	}

	Polynomial<T> Polynomial<T>::operator *(const Polynomial<T>& second)
	{
		Polynomial<T> first = *this;
		size_t ans_cap = first.get_size()*second.get_size();
		Polynomial<T> answer(ans_cap, non);

		for(int i=0; i<second.get_size();i++)
			for(int j=0;j<first.get_size();j++)
					answer.set(i+j,answer.get(i+j)+second.get(i)*first.get(j));
		answer.clear();
		return answer;
	}
	
};


template<class T>
ostream& operator << (ostream& _cout, Polynomial<T>& p)
{
	int pointer = 1;
	if(p.get_size()>1)
		for(int i = p.get_size()-1;i>-1;i--)
		{
			if(p.get(i)>p.get_non())
			{
				if(pointer==0) _cout << "+";
				else pointer=0;

				_cout << p.get(i);
				if (i==1) _cout << "x";
				else 
					if(i>0) _cout << "x^" << i;
			}
			else if (p.get(i)<p.get_non())
			{
				if(pointer==1) pointer=0;
				_cout << p.get(i);
				if (i==1) _cout << "x";
				else 
					if(i>0) _cout << "x^" << i;
			}
		}
		else if(p.get_size()==0) 
			_cout << 1;
		else if(p.get_size()==1)
			_cout << p.get(0);
		return _cout;
}

template <class T>
T gcd(const T& _a, const T& _b, const T& non)
{	
	T a=_a;
	T b=_b;
	T temp = non;
	while(true)
	{
		if (a>b)
		{
			temp=a%b;
			temp.normalize();
			if (temp>non || temp<non) a=temp;
			else return b;
		}
		else if (a<b)
			{
				temp=b%a;
				temp.normalize();
				if (temp>non || temp<non) b=temp;
				else return a;
			}
		else break;
	}
	return a;
}

template <class T>
vector<T> find_gcd_another_way(vector<T> a, vector<T> b)
{
	vector<T> res;
	bool marker = true;
	for(int j=0;j<b.size();j++)
		for(int i=0;i<a.size();i++)
			if(a[i]==b[j] && b[j]!=-1 && marker == true)
			{
				res.push_back(b[j]);
				a[i]=-1;
				marker = false;
			}
	return res;
}


void test()
{
	for(int i=0;i<10;i++)
	{
		vector<long long> values_for_a;
		vector<long long> values_for_b;
		int sup1 = 4;
		int sup2 = 3;
		for(int j=1;j<sup1;j++)
			values_for_a.push_back(rand()%3);
		for(int j=1;j<sup2;j++)
			values_for_b.push_back(rand()%4+2);
		Polynomial<long long> a(sup1, 0, values_for_a);
		Polynomial<long long> b(sup2, 0, values_for_b);

		vector<long long> values_for_res = find_gcd_another_way(values_for_a, values_for_b);
		Polynomial<long long> res(values_for_res.size()+1, 0, values_for_res);
		
		Polynomial<long long> non(1,0);
		if(!(gcd(a,b,non)==res))
		{
			cout << "test failed!" << endl;
			cout << "a:" << a;
			cout << " b:" << b;
			cout << " gcd:";
			cout  << gcd(a,b,non); 
			cout << ';' << res;
			cout << endl;	
		}
	}
}


int main()
{

test();

system("pause");
return 0;
}