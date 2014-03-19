#include <cmath>
#include <climits>
#include <string>
#include <iostream>
using namespace std;

#define len 10
#define N 81 /*size of sudoku grid*/

class List
{
public:
	List();
	~List();
	void remove(int n);
	int listLength(void);
	void setLength(int);
	int getChoice(void);

private:
	int list[10];

};


List::List()
{
	list[0] = 10;
	for(int i = 1; i < len; i++)
	{
		list[i] = 1;//1 indicates the presence of a number.
	}
}

List::~List()
{
	delete this;
}

void List::remove(int n)
{
	if (n < 10 && list[n]!= 0)
	{
		list[n] = 0;
		list[0] = list[0] - 1;
	}
}

 int List::listLength(void)
{
	return list[0];
}

void List::setLength( int length)
{
	list[0] = length;
}

int List::getChoice(void)
{
	int choice = 0;
	if(list[0] == 0)
	{
		return 0;
	}
	for( int i = 1; i< len; i++)
	{
		if(list[i] != 0)
		{
			choice = i;
			remove(i);
			break;
		}
		
	}
	return choice;
}

/*Structure of CellSet */
struct CellSet{
	int i;
	int j;
	List* Set;
	CellSet* left;
	CellSet* right;
};

class MinQueue
{
	public:
	CellSet** node;
	int queuesize;
	MinQueue(int n);
	~MinQueue();
	int parent(int);
	int left(int);
	int right(int);
	void MinQueueify(int);
	void buildMinQueue(int);
	CellSet* heapMinimum();
	CellSet* next();
	int queueIncreaseKey(int, int);
	int push(CellSet *);
	void printMinQueue();
	private:
};


/*Constructor */
MinQueue::MinQueue( int n)
{
	int i;
	node = new CellSet*[N];
	queuesize = 0;
	for(i = 0; i < N; i++)
		{
			/*node[i] = new CellSet;
			node[i]->left = nullptr;
			node[i]->right = nullptr;*/
			node[i] = nullptr;
		}
}

/* Destructor */
MinQueue::~MinQueue()
{
	for( int i = 0; i < N * N; i++)
	{
		if(node[i])
			delete node[i];
	}
	if(node)
		delete [] node;
	node = nullptr;
	delete this;
}
int MinQueue::parent(int i)
{
	return floor((i - 1)/2); /*adjusting to account for stupid 0 index*/
}
int MinQueue::left(int i)
{
	return 2 * i + 1;
}
int MinQueue::right(int i)
{
	return 2 * i + 2;
}

/*Increases the key of a particular elememt */
int MinQueue::queueIncreaseKey(int i, int key)
{
	if( node[i] == nullptr)
		return 0;
	else if(node[i]->Set->listLength() > key)
		return 0;
	else
	{
		node[i]->Set->setLength(key);
		CellSet* temp;
		while(i > 0 && node[parent(i)]->Set->listLength() > node[i]->Set->listLength())
		{
			temp = node[parent(i)];
			node[parent(i)] = node[i];
			node[i] = temp;
		}
		return 1;
	}
}


/*Inserts a new node */
int MinQueue::push(CellSet* newnode)
{
	int key = newnode->Set->listLength();
	newnode->Set->setLength(INT_MIN);
	queuesize = queuesize + 1;
	node[queuesize - 1] = newnode;                /*adjusting index*/
	return queueIncreaseKey(queuesize - 1,key);
}

void MinQueue::MinQueueify(int i)
{
	int smallest;
	int l = left(i);
	 int r = right(i);
	CellSet* temp;
	if( l < queuesize && (node[l]->Set->listLength() < node[i]->Set->listLength()))
	smallest = l;
	else
	smallest = i;
	if( r < queuesize && (node[r]->Set->listLength() < node[smallest]->Set->listLength()))
	smallest = r;
	if( smallest != i)
	{
		temp = node[i];
		node[i] = node[smallest];
		node[smallest] = temp;
		MinQueueify(smallest);
	}
}

CellSet* MinQueue::next() /*Equivalent to Pop*/
{
	if( queuesize <= 0)
	{
	for( int i = 0; i < N * N; i++)
		{
			if(node[i])
			{  
				if(node[i]->Set)
					delete node[i]->Set;
				delete node[i];
			}
		}
		return nullptr;
	}
	CellSet* min = node[0];
	node[0] = node[queuesize - 1];/*index adjustment */
	queuesize = queuesize - 1;
	MinQueueify(0);
	return min;
}


void MinQueue::printMinQueue()
{
	int i;
	for(i = 0; i < queuesize; i++)
	{
		cout << node[i]->i << "," << node[i]->j << "\n";
	}
}


///////////////////////Functions that are not needed\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

/*Gives the minimum of the heap */
CellSet* MinQueue::heapMinimum()
{
	return node[0];
}




/* Builds the MinQueue */
void MinQueue::buildMinQueue(int n)
{
	int i;
	queuesize = n;
	for (i = floor(N/2) - 1; i >= 0; i--)
	{
	MinQueueify(i);
	// printMinQueue();
	}
}


