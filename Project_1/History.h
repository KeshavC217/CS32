#ifndef HISTORY_H
#define HISTORY_H
#include "globals.h"
class Arena;
class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
private:
	Arena *ap;
	int Hrows;
	int Hcols;
	char gridH[MAXROWS][MAXCOLS];
};
#endif