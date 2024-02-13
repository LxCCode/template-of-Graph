//感谢《数据结构（C++语言版）》提供的代码

#include<iostream>
#include"..\..\..\Vector\Vector\template-of-Vector\Vector.hpp"
#include"..\..\..\Queue\Queue\template-of-Queue\Queue.hpp"
using namespace std;

typedef enum {UNDISCOVERED, DISCOVERED, VISITED} VStatus;
typedef enum {UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD} EType;

template <typename Tv, typename Te>
class Graph {
private:
	void reset() {
		for (int i = 0; i < n; i++)
		{
		status(i) = UNDISCOVERED;
			dTime(i) = fTime(i) = -1;
			parent(i) = -1;
			priority(i) = INT_MAX;
			for (int j = 0; j < n; j++)
			{
				if (exists(i, j))
					type(i, j) = UNDETERMINED;
			}
		}
	}
	void BFS(int v, int& clock);
	void DFS(int v, int& clock);

public:
	int n;
	virtual int insert(Tv const& vertex) = 0;
	virtual Tv remove(int i) = 0;
	virtual Tv& vertex(int i) = 0;
	virtual int inDegree(int i) = 0;
	virtual int outDegree(int i) = 0;
	virtual int firstNbr(int i) = 0;
	virtual int nextNbr(int i, int j) = 0;
	virtual VStatus& status(int i) = 0;
	virtual int& dTime(int i) = 0;
	virtual int& fTime(int i) = 0;
	virtual int& parent(int i) = 0;
	virtual int& priority(int i) = 0;

	int e;
	virtual bool exists(int i, int j) = 0;
	virtual void insert(Te const& edge, int w, int i, int j) = 0;
	virtual Te remove(int i, int j) = 0;
	virtual EType& type(int i, int j) = 0;
	virtual Te& edge(int i, int j) = 0;
	virtual int& weight(int i, int j) = 0;

	void bfs(int s);
	void dfs(int s);
};

template <typename Tv> struct Vertex {
	Tv data;
	int inDegree, outDegree;
	VStatus status;
	int dTime, fTime;
	int parent;
	int priority;
	Vertex(Tv const& d = (Tv) 0) : data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
	                               dTime(-1), fTime(-1), parent(-1), priority(INT_MAX)
	{}
};

template <typename Te> struct Edge {
	Te data;
	int weight;
	EType type;
	Edge(Te const& d, int w) : data(d), weight(w), type(UNDETERMINED) {}
};

template <typename Tv, typename Te>
class GraphMatrix :public Graph<Tv, Te> {
private:
	Vector<Vertex<Tv>> V;
	Vector<Vector<Edge<Te>*>> E;
public:
	GraphMatrix() { this->n = this->e = 0; }
	~GraphMatrix() {
		for (int i = 0; i < this->n; i++)
		{
			for (int j = 0; j < this->n; j++)
			{
				delete E[i][j];
			}
		}
	}
	virtual Tv& vertex(int i) { return V[i].data; }
	virtual int inDegree(int i) { return V[i].inDegree; }
	virtual int outDegree(int i) { return V[i].outDegree; }
	virtual int firstNbr(int i) { return nextNbr(i, this->n); }
	virtual int nextNbr(int i, int j) { while ((-1 < j) && (!exists(i, --j))); return j; }
	virtual VStatus& status(int i) { return V[i].status; }
	virtual int& dTime(int i) { return V[i].dTime; }
	virtual int& fTime(int i) { return V[i].fTime; }
	virtual int& parent(int i) { return V[i].parent; }
	virtual int& priority(int i) { return V[i].priority; }

	virtual int insert(Tv const& vertex) {
		for (int i = 0; i < this->n; i++)
		{
			E[i].insert(NULL);
		}
		(this->n)++;
		E.insert(Vector<Edge<Te>*>(this->n, this->n, (Edge<Te>*)NULL));
		return V.insert(Vertex<Tv>(vertex));
	}

	virtual Tv remove(int i)
	{
		for (int j = 0; j < this->n; j++)
		{
			if (exists(i, j)) {
				delete E[i][j];
				V[j].inDegree--;
			}
		}
		E.remove(i);
		(this->n)--;
		Tv vBak = vertex(i);
		V.remove(i);
		for (int j = 0; j < this->n; j++)
		{
			if (Edge<Te>* e = E[j].remove(i))
			{
				delete e;
				V[j].outDegree--;
			}
		}
		return vBak;
	}

	virtual bool exists(int i, int j)
	{
		return (0 <= i) && (i < this->n) && (0 <= j) && (j < this->n) && E[i][j] != NULL;
	}

	virtual EType& type(int i, int j)
	{
		return E[i][j]->type;
	}
	virtual Te& edge(int i, int j)
	{
		return E[i][j]->data;
	}
	virtual int& weight(int i, int j)
	{
		return E[i][j]->weight;
	}
	virtual void insert(Te const& edge, int w, int i, int j)
	{
		if (exists(i, j))
			return;
		E[i][j] = new Edge<Te>(edge, w);
		(this->e)++;
		V[i].outDegree++;
		V[j].inDegree++;
	}
	virtual Te remove(int i, int j)
	{
		Te eBak = edge(i, j);
		delete E[i][j];
		E[i][j] = NULL;
		(this->e)--;
		V[i].outDegree--;
		V[j].inDegree--;
		return eBak;
	}
};

template <typename Tv, typename Te>
void Graph<Tv, Te>::bfs(int s) {
	reset();
	int clock = 0;
	int v = s;
	do 
		if (UNDISCOVERED == status(v))
			BFS(v, clock);
	while (s != (v = (++v % n)));
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::BFS(int v, int& clock) {
	Queue<int> Q;
	status(v) = DISCOVERED;
	Q.enqueue(v);
	while (!Q.empty())
	{
		int v = Q.dequeue();
		dTime(v) = ++clock;
		for(int u = firstNbr(v); -1 < u; u = nextNbr(v, u))
			if (UNDISCOVERED == status(u)) {
				status(u) = DISCOVERED;
				Q.enqueue(u);
				type(v, u) = TREE;
				parent(u) = v;
			}
			else {
				type(v, u) = CROSS;
			}
		status(v) = VISITED;
		cout << vertex(v) << endl;
	}
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::dfs(int s) {
	reset();
	int clock = 0;
	int v = s;
	do 
		if (UNDISCOVERED == status(v))
			DFS(v, clock);
	while (s != (v = (++v % n)));
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::DFS(int v, int& clock) {
	dTime(v) = ++clock;
	status(v) = DISCOVERED;
	for(int u = firstNbr(v); u > -1; u = nextNbr(v, u))
	{
		switch (status(u)) {
		case UNDISCOVERED:
			type(v, u) = TREE;
			parent(u) = v;
			DFS(u, clock);
			break;
		case DISCOVERED:
			type(v, u) = BACKWARD;
			break;
		default:
			type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
			break;
		}
	}
	status(v) = VISITED;
	cout << vertex(v) << endl;
	fTime(v) = ++clock;
}
