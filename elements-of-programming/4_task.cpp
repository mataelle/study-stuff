#include <iostream>
#include <vector>
#include <queue>
#include <map>
using namespace std;

class Tree
{
private:
	map <int, map <int, int> > edges;
	vector <int> used;
	vector <int> stack;
	size_t vertices_number;
	vector<int> dfs_list;
	int start_vertex;
	static const int UNDEFINED_VERTEX = -1;
		
	class Iterator
	{
	private:
		Tree* Iterated_Tree;
		size_t current_vertex;
	public:	
		Iterator(Tree& _Iterated_Tree, size_t _current_vertex): 
		  Iterated_Tree(&_Iterated_Tree), current_vertex(_current_vertex) {}
		Iterator& operator ++ ()
		{
			current_vertex = Iterated_Tree->next_DFS_vertex(Iterated_Tree->start_vertex, current_vertex, 0);
			return *this;
		}
		int operator * () const
		{
			return current_vertex;
		}

		bool operator != (Iterator& it) const
		{
			return (current_vertex != it.current_vertex);
		}
	};



public:
	Tree() {}
	explicit Tree(int size):vertices_number(size), used(size)
	{
		for (int i = 0; i < size; i++)
			used[i] = 0;
	}

	Iterator begin()
	{
		return Iterator(*this, start_vertex);
	}

	Iterator end()
	{
		return Iterator(*this, UNDEFINED_VERTEX);
	}

	void add_edge(int vertex_1, int vertex_2)
	{
		edges[vertex_1][vertex_2] = 1;
		edges[vertex_2][vertex_1] = 1;
	}

	int next_DFS_vertex(int begin_vertex, int vertex, int flag)
	{
		stack.push_back(begin_vertex);
		used[begin_vertex] = 1;
		if(begin_vertex==vertex) flag = 1;
		for (map<int, int>::iterator it=edges[begin_vertex].begin(); it!=edges[begin_vertex].end(); it++)
		{
			if(it->second == 1 && used[it->first]==0)
				if(flag==1)
				{
					stack.clear();
					for(int i = 0;i<used.size();i++)
						used[i]=0;
					return it->first;
				}
				else
				{
					if(it->first==vertex)
						return next_DFS_vertex(it->first, vertex, 1);
					return next_DFS_vertex(it->first, vertex, 0);
				}
			else 
			{
				map<int, int>::iterator jt = it;
				if(++(jt)==edges[begin_vertex].end())
				{
					if(stack.size()==used.size())
					{
						stack.clear();
						for(int i = 0;i<used.size();i++)
							used[i]=0;
						return UNDEFINED_VERTEX;
					}
					stack.pop_back();
					if(!stack.empty()) 
					{
						begin_vertex = stack[stack.size()-1];
						stack.pop_back();
					}
					else return UNDEFINED_VERTEX;
					return next_DFS_vertex(begin_vertex, vertex, flag);
				}
			}
		}
	}

	void DFS(int t)
	{
		start_vertex=t;
		dfs_list.push_back(t);
		used[t] = 1;
		for (map<int, int>::iterator it=edges[t].begin(); it!=edges[t].end(); it++)
				if(it->second == 1 && used[it->first]==0)
				DFS(it->first);
	}

	vector<int> get_DFS() const
	{
		return dfs_list;
	}
	vector<int> BFS(int t)
	{
		start_vertex = t;
		vector<int> bfs_list;
		for (int i = 0; i < vertices_number; i++)
			used[i] = 0;

		queue <int> q;
		q.push(t);

		used[t] = 1;

		while (!q.empty())
		{
			int t = q.front();
			bfs_list.push_back(t);
			q.pop();
			for (map<int, int>::iterator it=edges[t].begin(); it!=edges[t].end(); it++)
				if(it->second == 1 && used[it->first]==0)
				{
					q.push(it->first);
					used[it->first] = 1;
				}
		}
		for (int i = 0; i < vertices_number; i++)
			used[i] = 0;
		return bfs_list;
	}

	bool const operator == (Tree& t)
	{
		if (vertices_number != t.vertices_number)
			return false;
		else
		{
			for (int i = 0; i < vertices_number; i++)
				for (int j = 0; j < vertices_number; j++)
					if (edges[i][j] != t.edges[i][j])
						return false;
		}

		return true;
	}
};


template <class DfsIterator>
void rebuild_tree(Tree* res_tree, DfsIterator& dfs_begin, DfsIterator& dfs_end, vector<int>& bfs_list)
{
	if (bfs_list.size() == 2)
	{
		res_tree->add_edge(bfs_list[0], bfs_list[1]);
	}
	else if (bfs_list.size() > 2)
	{
		int bfs_begin = 0;
		int dfs_ver_1 = *dfs_begin;
		DfsIterator temp_dfs_begin = dfs_begin;
		int dfs_ver_2 = *(++temp_dfs_begin);

		while(1)
		{
			if(*dfs_begin==*dfs_end) break;
			if(dfs_ver_2==*dfs_end) break;
			if(dfs_ver_1==bfs_list[bfs_begin] && dfs_ver_2==bfs_list[bfs_begin+1])
			{
					res_tree->add_edge(dfs_ver_1, dfs_ver_2);
					bfs_begin++;
					dfs_begin++;
					dfs_ver_2 = *(++temp_dfs_begin);
			}
			else
			{
				res_tree->add_edge(dfs_ver_1, bfs_list[bfs_begin+1]);
				vector<int> bfs_extract;
				bfs_extract.push_back(bfs_list[bfs_begin]);
				bfs_begin++;
				int bfs_next=bfs_begin+1;
				DfsIterator dfs_next = dfs_begin;
				dfs_next++;
				while(bfs_list[bfs_begin]!=*dfs_next)
				{
					bfs_extract.push_back(bfs_list[bfs_next]);
					dfs_next++;
					bfs_next++;
				}
				rebuild_tree(res_tree, dfs_begin, dfs_next, bfs_extract);
				
				int t = bfs_list[bfs_begin]; 
				bfs_list[bfs_begin] = bfs_list[bfs_begin+bfs_extract.size()-1]; 
				bfs_list[bfs_begin+bfs_extract.size()-1] = t;
				
				bfs_begin+=bfs_extract.size()-1;
				dfs_begin=dfs_next;
				dfs_ver_1 = *dfs_begin;
				temp_dfs_begin = dfs_begin;
				dfs_ver_2 = *(++temp_dfs_begin);
			}
		}
	}
}

int test()
{
	Tree my_tree(9);
	my_tree.add_edge(1, 2);
	my_tree.add_edge(2, 3);
	my_tree.add_edge(2, 5);
	my_tree.add_edge(3, 4);
	my_tree.add_edge(4, 6);
	my_tree.add_edge(4, 7);
	my_tree.add_edge(7, 0);
	my_tree.add_edge(7, 8);

	my_tree.DFS(3);
	vector <int> dfs_list = my_tree.get_DFS();
	vector <int> bfs_list = my_tree.BFS(3);

	Tree res_tree(bfs_list.size());
	rebuild_tree(&res_tree, my_tree.begin(), my_tree.end(), my_tree.BFS(3));
	res_tree.DFS(3);
	vector <int> result_dfs_list = res_tree.get_DFS();
	vector <int> result_bfs_list = res_tree.BFS(3);
	for(int i=0;i<9;i++)
		if(result_bfs_list[i]!=bfs_list[i]) 
		{
			cout << "fail bfs! i:"<< i << endl;
			system("pause");
			return -1;
		}
	for(int i=0;i<9;i++)
		if(result_dfs_list[i]!=dfs_list[i]) 
		{
			cout << "fail dfs! i:"<< i << endl;
			system("pause");
			return -1;
		}

}

int main()
{
	if (test()==0)
	cout << "test done!" << endl;
	system("pause");
	return 0;
}