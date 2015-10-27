#include <iostream>
#include <cstdio>
#include <cassert>
#include <vector>
#include <map>
using namespace std;

class Monopoly
{
private:
	vector<int> id;
	vector<int> node;
	map<int, map< int, short> > levels;
	map<int, map< int, int> > children;
	int D;
	int N; 
	int current_node;
public:
	Monopoly()
	{		
		cin >> N >> D;
		id.assign(30001,0);
		node.assign(30001,0);

 		for (int i = 0; i < N; i++)
		{
			id[i] = i;
			node[i] = i;
		}
		current_node = N;
		for (int i = 0; i < N-1; i++) 
		{
			int a; int b;
			cin >> a >> b;
			a = find(a-1);
			b = find(b-1);
			
			assert(a != b);
			id[a] = b;
	        children[current_node][0] = node[a];
   	        children[current_node][1] = node[b];
	        node[b] = current_node++;
		}
	}
	int find(int a)
	{
		int temp = 0;
		while (a != id[a])
		{
			temp = a;
			a = id[a];
			id[temp]= id[id[temp]];
		}
	    return a;
	}
	int get_D()
	{
		return D;
	}
	int get_current_node()
	{
		return current_node;
	}
	int count(int _current_node, int d)
	{
		if (children[_current_node].find(0) == children[_current_node].end())
		    return 1;
		if (d == 0)
			return 30000;
		if (levels[_current_node].find(d) != levels[_current_node].end())
			return levels[_current_node][d];
		int a = children[_current_node][0]; 
		int b = children[_current_node][1];
		int res = max(count(a, d-1), count(b, D)+1);
		res = min(res, max(count(a, D)+1, count(b, d-1)));
		levels[_current_node][d] = res;
		return res;
	}
};
int main()
{
  int T = 0; cin >> T;
  for (int test_number = 1; test_number <= T; test_number++)
  {
	  Monopoly MyMonopoly;
	  printf("Case #%d: %d\n", test_number, MyMonopoly.count(MyMonopoly.get_current_node()-1, MyMonopoly.get_D()));
  }
  system("pause");
  return 0;
}