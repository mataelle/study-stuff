#include <cstdio>
#include <iostream>
#include <vector>
#include <map>
#include <cassert>
using namespace std;

#define REQUIRE(cond, message)\
	if(cond){\
	cout << message <<endl;\
	}

template <class T>
class VectorTransformation
{
	private:
		int number;
		int module;
	public:
		VectorTransformation(int _number, int _module):number(_number), module(_module) {};
		vector<T> operator() (const vector<T>& a) const
		{
			vector<T> res = a;
			for(int i=0;i<res.size();i++)
				res[i]=(res[i]+number)%module;
			return res;

		};
};



template <class T, class V, template <class> class BOp>
T apply (const T object, BOp<V> transform, size_t num_iterations)
{
	if (num_iterations==0) return object;
	else 
	return transform(apply(object,transform,num_iterations-1));
}


template <class T, class V, template <class> class BOp>
int find_cycle_element_index(const T& start_element, BOp<V>  transform, T element_in_cycle)
{
	T a=start_element; T b=start_element;
	a = apply(a, transform, 1);
	b = apply(b, transform, 2);

	int counter = 1;
	while(a!=b)

	{
		counter++;
		a = apply(a, transform, 1);
		b = apply(b, transform, 2);
	}
	element_in_cycle = a;
	int number_in_cycle = counter;

	return number_in_cycle;
}


template <class T, class V, template <class> class BOp>
int find_cycle_length(const T& first_el, BOp<V> transform, T element_in_cycle)
{
	T a = first_el;
	int counter = 1;
	a = apply(a,transform, 1);
	while(a!=element_in_cycle)
	{
		counter++;
		a = apply(a,transform, 1);
	}
	int cycle_len = counter;
	return cycle_len;
}


template <class T, class V, template <class> class BOp>
int little_memory(const T& first_el, BOp<V> functor)
{
	T element_in_cycle = first_el;
	int number_in_cycle = find_cycle_element_index(first_el, functor, element_in_cycle);
	element_in_cycle = apply(element_in_cycle,functor, number_in_cycle);

	int cycle_len = find_cycle_length(first_el, functor, element_in_cycle);

	int may_be_first_entrance = number_in_cycle%cycle_len;

	T a = first_el;
	T b = apply(a,functor,may_be_first_entrance);

	int counter = may_be_first_entrance;
	a = b;
	b = apply(b,functor,cycle_len);

	while(b!=element_in_cycle)
	{
		counter+=cycle_len;
		a = b;
		b = apply(b,functor,cycle_len);
	}

	while(a!=b)
	{
		counter++;
		a = apply(a,functor,1);
		b = apply(b,functor,1);
	}
	return counter;
}



template <class T, class V, template <class> class BOp>
int find_precycle_length_naive(const T& a, BOp<V> functor)
{
	map<T, int> elements;
	T element = a;
	int counter = 0;
	int cycle = 0;
	while(1)
	{
		if(elements.find(element)==elements.end()) 
		{
			elements[element] = 1;
			element = apply(element,functor,1);
			counter++;
		}
		else break;
	}

	while(1)
	{
		if(elements[element] == 2) break;
		else 
		{
			elements[element]++;
			element = apply(element,functor,1);
			cycle++;
		}
	}

	return counter-cycle;
}

void tester()
{
	int size = 0;
	int temp = 1341;

	for(int i=1;i<20;i++)
	{
		size = i;
		vector<int> test;
		for(int j=0;j<size;j++)
		{
			test.push_back(0);
			test[j] = (temp/(j+1) )%i;
		}
		VectorTransformation<int> functor(5, 7);
		cout << i;
		REQUIRE(little_memory(test, functor)==find_precycle_length_naive(test, functor), " OK");

	}

}


int main()
{
	tester();
	system("pause");
	return 0;
}