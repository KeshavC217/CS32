#include "History.h"
//#include "Arena.h"
#include <iostream>
using namespace std;
History::History(int nRows, int nCols)
{
	Hrows = nRows;
	Hcols = nCols;
	for (int r = 0; r < nRows; r++)
		for (int c = 0; c < nCols; c++)
			gridH[r][c] = '.'; //Initialize all the elements in the array to dots, which is the default setting of history.

}
bool History::record(int r, int c)
{
	if (r<0 || c<0 || r>MAXROWS || c>MAXCOLS)
		return false;
	if (gridH[r-1][c-1] == '.')
		gridH[r-1][c-1] = 'A';
	else if (gridH[r-1][c-1]<'Z')
		gridH[r-1][c-1]++; //Each subtracts 1 because the rows start at 1 but the array starts at 0
	return true;
}
void History::display() const
{
	clearScreen();
	for (int r = 0; r < Hrows; r++)
	{
		for (int c = 0; c < Hcols; c++)
		{
			cout << gridH[r][c];
		}
		cout << endl;
	}
	cout << endl;
}

