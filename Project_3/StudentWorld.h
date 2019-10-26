#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include <vector>
#include "GameWorld.h"
#include <string>
#include <iostream>

class Penelope;
class Actor;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld()
	{
		cleanUp();
	}
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	bool isCollide(int x, int y, Actor *p);
	void loadLevel();
	bool isOverlap(int x, int y);
	bool getLevelFinish() { return levelFinish; }
	void setLevelFinish(bool t) { levelFinish = t; }
	bool getisPAlive() { return isPAlive; }
	void setisPAlive(bool t) { isPAlive = t; }

	void setinfCountP(int t) { infectCountP = t; }

	int getFlamesW() { return wFlames; }
	int getVaccinesW() { return wVaccines; }
	int getMinesW() { return wMines; }

	void incFlamesW() { wFlames += 5; }
	void incVaccinesW() { wVaccines++; }
	void incMinesW() { wMines += 2; }

	void decFlamesW() { wFlames--; }
	void decVaccinesW() { wVaccines--; }
	void decMinesW() { wMines--; }


	void setIsFlame(bool t) { isFlame = t; }
	void setPenDir(int t) { penDir = t; }

	bool isFireBlocked(int x, int y);
	void isFireOverlap(int x, int y);

	bool isVomitOverlap(int x, int y);

	void setMinePlaced(bool t) { minePlaced = t; }

	void octaFlame(int destx, int desty);

	int determineSameRC(int x, int y, int &d, int &d1);
	bool getDistFromPen(int x, int y, int &dp);

	int findClosestVictim(int x, int y, int &d1, int &d2);//1 if same x, 2 if same y, 3 if neither but within 80 pixels, and 4 if not within 80 pixels
	int findClosestZombie(int x, int y, int &d1);

	bool deathByMinePit(int x, int y);
	void citsLeave(int x, int y);

	int getCitCount() { return citCount; }
	void setCitCount(int t) { citCount = t; }

	void createVomit(int x, int y);

	void incScore(int t) { increaseScore(t); }

	void spawnZombie(int x, int y, int r);
	
	bool overlapWithAnything(int x, int y);
	
private:
	Penelope *m_Penelope;
	std::vector<Actor*> m_Actors;
	bool levelFinish;
	bool isPAlive;
	int wFlames;
	int wVaccines;
	int wMines;
	bool isFlame;
	int penDir;
	bool minePlaced;
	int citCount;

	int infectCountP;
};

#endif // STUDENTWORLD_H_
