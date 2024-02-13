#include"Graph.hpp"

int main()
{
	GraphMatrix<char, int> G;
	G.insert('A');
	G.insert('B');
	G.insert('C');
	G.insert('D');
	G.insert('E');
	G.insert('F');
	G.insert('G');
	G.insert(0, 0, 0, 1);
	G.insert(0, 0, 0, 2);
	G.insert(0, 0, 0, 5);
	G.insert(0, 0, 1, 2);
	G.insert(0, 0, 3, 0);
	G.insert(0, 0, 3, 4);
	G.insert(0, 0, 4, 5);
	G.insert(0, 0, 5, 6);
	G.insert(0, 0, 6, 0);
	G.insert(0, 0, 6, 2);
	G.dfs(0);
	G.bfs(0);
}