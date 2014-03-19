// sudokuSolverFinal.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "minQueue.h"
#include "time.h"

int Icount = 0;
#define L 9
#define UNDEFINED 0
bool solve=true;

int test[L][L] ={{1, 2, 8, 4, 6, 5, 3, 7, 9},
                      {3, 7, 4, 2, 1, 9, 8, 5, 6},
                      {9, 5, 6, 0, 3, 0, 0, 4, 2},
                      {7, 6, 5, 0, 0, 8, 0, 2, 3},
                      {2, 4, 9, 6, 7, 3, 0, 0, 1},
                      {8, 1, 3, 0, 0, 2, 9, 0, 0},
                      {5, 0, 2, 3, 8, 6, 7, 1, 4},
                      {0, 8, 7, 0, 2, 0, 6, 0, 5},
                      {0, 3, 1, 7, 0, 4, 2, 9, 8}};

/*{{1, 2, 8, 4, 6, 5, 3, 7, 9},
                      {3, 7, 4, 2, 1, 9, 8, 5, 6},
                      {9, 5, 6, 0, 3, 0, 0, 4, 2},
                      {7, 6, 5, 0, 0, 8, 0, 2, 3},
                      {2, 4, 9, 6, 7, 3, 0, 0, 1},
                      {8, 1, 3, 0, 0, 2, 9, 0, 0},
                      {5, 0, 2, 3, 8, 6, 7, 1, 4},
                      {0, 8, 7, 0, 2, 0, 6, 0, 5},
                      {0, 3, 1, 7, 0, 4, 2, 9, 8}};*/ //working set*/

class solveSudoku
{
public:
	solveSudoku();
	solveSudoku(bool insert);
	solveSudoku(int grid[L][L]);
	~solveSudoku();
   List* possibilities(int row, int col);
  void getBox(int row, int col, int box[L]);
  void GetChildQueue(void);

  bool reject(void);
  bool accept(void);
  bool UsedInRow(int row, int num);
  bool UsedInCol(int col, int num);
  bool UsedInBox(int boxStartRow, int boxStartCol, int num);
  bool isSafe(int row, int col, int num);
  void printGrid(void);

  solveSudoku* getNext();
  MinQueue* childQ;

private:

	int SudokuGrid[L][L];

};

solveSudoku::solveSudoku()
{
}

solveSudoku::solveSudoku(bool insert)
{
  if(insert)
  {
   for(int i = 0; i < L; i++)
   {
	   for(int j = 0; j < L; j++)
	   {
		   SudokuGrid[i][j] = test[i][j];
	   }
   }
  }
}


solveSudoku::solveSudoku(int grid[L][L])
{
   for( int i = 0; i < L; i++)
   {
	   for(int j = 0; j < L; j++)
	   {
		   SudokuGrid[i][j] = grid[i][j];
	   }
   }

}

solveSudoku::~solveSudoku()
{
	delete this;
}

solveSudoku* solveSudoku::getNext()
{
	int choice;
	CellSet* cell = childQ->next();
	if(cell == nullptr)
		return nullptr;
	choice  = cell->Set->getChoice();
	solveSudoku* newS  = new solveSudoku(SudokuGrid);
	if(choice != 0)
	{
		newS->SudokuGrid[cell->i][cell->j] = choice;
	}
	if(cell->Set->listLength() > 0)
	{
		childQ->push(cell);
	}
	return newS; //add condition for choice==0
}



/*Implementation of Naked Single*/
List* solveSudoku::possibilities(int row, int col)
{
	List* retList =  new List;
	int cell[L];
	for(int i  = 0; i < L; i++)
	{
		if(SudokuGrid[row][i] != UNDEFINED)
		{
			retList->remove(SudokuGrid[row][i]);
		}
		else if(SudokuGrid[i][col] != UNDEFINED)
		{
			retList->remove(SudokuGrid[i][col]);
		}
	}

	getBox(row,col,cell);
	for(int i = 0; i< L; i++)
	{
		if( cell[i] != UNDEFINED)
		{
			retList->remove(cell[i]);
		}
	}
	return retList;
}


/*Implementation of getchildqueue*/
void solveSudoku::GetChildQueue()
{
	int size = L*L;
	childQ = new MinQueue(size);
	for(int i = 0; i < L; i++)
	{
		for(int j = 0; j < L; j++)
		{
			if(SudokuGrid[i][j] == UNDEFINED)
			{
				CellSet* newSet = new CellSet;
				newSet->i = i;
				newSet->j = j;
				newSet->left = nullptr;
				newSet->right = nullptr;
				newSet->Set = possibilities(i,j);
				childQ->push(newSet);
			}
		}
	}
}

void solveSudoku::getBox(int row, int col, int box[N])
{
	int boxStartRow =row - row%3;
	int boxStartCol = col - col%3;

	 for (int i= 0; i < 3; i++)
	 {
        for (int j = 0; j < 3; j++)
		{
            box[i * 3 + j] = SudokuGrid[i+boxStartRow][j+boxStartCol];
		}
	 }
}

/* Returns a boolean which indicates whether any assigned entry
   in the specified row matches the given number. */
bool solveSudoku::UsedInRow( int row, int num)
{
    for (int col = 0; col < L; col++)
        if (SudokuGrid[row][col] == num)
            return true;
    return false;
}
 
/* Returns a boolean which indicates whether any assigned entry
   in the specified column matches the given number. */
bool solveSudoku::UsedInCol(int col, int num)
{
    for (int row = 0; row < L; row++)
        if (SudokuGrid[row][col] == num)
            return true;
    return false;
}
 
/* Returns a boolean which indicates whether any assigned entry
   within the specified 3x3 box matches the given number. */
bool solveSudoku::UsedInBox(int boxStartRow, int boxStartCol, int num)
{
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (SudokuGrid[row+boxStartRow][col+boxStartCol] == num)
                return true;
    return false;
}
 
/* Returns a boolean which indicates whether it will be legal to assign
   num to the given row,col location. */
bool solveSudoku::isSafe(int row, int col, int num)
{
    /* Check if 'num' is not already placed in current row,
       current column and current 3x3 box */
    return !UsedInRow(row, num) &&
           !UsedInCol(col, num) &&
           !UsedInBox(row - row%3 , col - col%3, num);
}



/*Implementation of reject function*/
bool solveSudoku::reject(void)
{
	bool res;
	for( int i = 0 ; i < L; i++)
	{
		for(int j = 0; j <L; j++)
		{
			int num = 0;
			if(SudokuGrid[i][j] != UNDEFINED)
			{
				num = SudokuGrid[i][j];
				SudokuGrid[i][j] = UNDEFINED;
				res = isSafe(i,j,num);
				SudokuGrid[i][j] = num;
				if(!res)
					return true;	
			}
		}
	}
	return false;
}

/*Implementation of accept function*/
bool solveSudoku::accept(void)
{
  if(reject())
	  return false;
  else
  {
	  for(int i = 0; i < L; i++)
	  {
		  for(int j = 0; j < L; j++)
		  {
			  if(SudokuGrid[i][j] == UNDEFINED)
				  return false;
		  }
	  }
  }
  return true;
}


/* A utility function to print grid  */
void solveSudoku::printGrid()
{
    for (int row = 0; row < L; row++)
    {
       for (int col = 0; col < L; col++)
             cout << " " << SudokuGrid[row][col];
       cout<<endl;
    }
	cout<<endl;
}
 


/*Smart BackTracking Algorithm*/
solveSudoku* BackTrack(solveSudoku* S)
{
	if (S->reject())
	{
	//	cout << "Rejected" <<endl;
		/*if(S->childQ)
			delete S->childQ;
		if(S)
			delete S;*/
		return nullptr;	
	}
	else if(S->accept())
	{
		//cout << "Accepted" <<endl;
		return S;
	}
	solveSudoku* solution = nullptr;
    solveSudoku*  newS =  nullptr;
	S->GetChildQueue();
	newS = S->getNext();
	while( newS != nullptr && solution == nullptr)
	{
		solution = BackTrack(newS);
		newS = S->getNext();
		Icount++;
 	}
	return solution;
}



int _tmain(int argc, _TCHAR* argv[])
{
   clock_t begin, end;
   double time_spent;
   begin = clock();
	solveSudoku S = new solveSudoku(solve);
	S.printGrid();
	solveSudoku* solution;
	solution = BackTrack(&S);
	if(solution != nullptr)
	{
	cout << "Solution Availalbe :\n"<< endl;
		solution->printGrid();
	}
	else
	{
		cout << "No Solution Available";
	}
   end = clock();
   time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
   cout << "\n BackTrackCount : " << Icount <<endl;
   cout << " \nTime Taken by the code to execute :" << time_spent<<  "seconds" <<endl;
	return 0;
}

