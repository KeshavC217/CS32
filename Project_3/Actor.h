#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int imageID, double startx, double starty, StudentWorld *gp1, Direction dir, int depth, double size)
		:GraphObject(imageID, startx, starty, dir, depth, size)
	{
		m_World = gp1;
		canCollide = false;
		canOverlap = false;
		isAlive = true;
		blocksFire = false;
		setFlammable(true);
		explodes = false;
		hasExploded = false;
		isPerson = false;
		canbeInfected = false;
		isVomit = false;
		shouldVomit = false;
		canBeFollowed = false;
		isSameRowOrCol=false;
		canFollow = false;
		deathByPit = false;
		canExit = false;
		isWorth2K = false;
		isInfected = false;
		infectionCount = 0;
	}

	virtual ~Actor() {}
	virtual void doSomething() = 0;

	virtual StudentWorld *getWorld(){return m_World;}

	void setCollide(bool t)	{canCollide = t;}
	bool getCollide()	{return canCollide;}

	void setOverlap(bool t)	{canOverlap = t;}
	bool getOverlap(){return canOverlap;}

	bool getAlive(){return isAlive;}
	void setAlive(bool t){	isAlive = t;}

	bool getBlocksFire() { return blocksFire; }
	void setBlocksFire(bool t) { blocksFire = t; }

	bool getFlammable() { return flammable; }
	void setFlammable(bool t) { flammable = t; }

	bool getExplodes() { return explodes; }
	void setExplodes(bool t) { explodes = t; }

	bool gethasExploded() { return hasExploded; }
	void sethasExploded(bool t) { hasExploded = t; }


	bool getisPerson() { return isPerson; }
	void setisPerson(bool t) { isPerson = t; }

	bool getCanBeInfected() { return canbeInfected; }
	void setCanBeInfected(bool t) { canbeInfected = t; }

	bool getisVomit() { return isVomit; }
	void setisVomit(bool t) { isVomit = t; }

	bool getShouldVomit() { return shouldVomit; }
	void setShouldVomit(bool t) { shouldVomit = t; }

	bool getIsSameRoworCol() { return isSameRowOrCol; }
	void setIsSameRoworCol(bool t) { isSameRowOrCol = t; }

	bool getcanBeFollowed() { return canBeFollowed; }
	void setCanBeFollowed(bool t) { canBeFollowed = t; }

	bool getcanFollow() { return canFollow; }
	void setCanFollow(bool t) { canFollow = t; }

	bool getDeathByPit() { return deathByPit; }
	void setDeathByPit(bool t) { deathByPit = t; }

	bool getCanExit() { return canExit; }
	void setCanExit(bool t) { canExit = t; }

	bool getWorth2k() { return isWorth2K; }
	void setWorth2K(bool t) { isWorth2K = t; }

	bool getInfected() { return isInfected; }
	void setInfected(bool t) { isInfected = t; }

	void increaseInfection() { infectionCount++; }
	int getInfectionCount(){ return infectionCount; }
	void setInfectionCount(int t) { infectionCount = t; }

	//Actor's .h declaration is just filled with getters and setter that might be used by almost any actor. 
private:
	StudentWorld *m_World;
	bool canCollide;
	bool canOverlap;
	bool isAlive;
	bool blocksFire;
	bool flammable;
	bool explodes;
	bool hasExploded = false;
	bool isPerson;
	bool canbeInfected;
	bool isVomit;
	bool shouldVomit;
	bool isSameRowOrCol;
	bool canBeFollowed;
	bool canFollow;
	bool deathByPit;
	bool canExit;
	bool isWorth2K;

	bool isInfected;
	int infectionCount;
};

class Wall : public Actor 
{
public:
	Wall(int x, int y, StudentWorld *gp1);
	virtual ~Wall() {}
	virtual void doSomething();
private:
};

class Penelope :public Actor
{
public:
	Penelope(int x, int y, StudentWorld *gp1);
	virtual ~Penelope() {}
	virtual void doSomething();
	
	int getFlames() { return flames; }
	int getVaccines() { return vaccines; }
	int getMines() { return mines; }

	void setFlames(int t) { flames=t; }
	void setVaccines(int t) { vaccines=t; }
	void setMines(int t) { mines=t; }

private:
	bool p_alive;
	bool levelFinish;
	int flames;
	int vaccines;
	int mines;
};

class EMP : public Actor
{
public:
	EMP(int id, int x, int y, StudentWorld *gp1, int depth);
	virtual ~EMP() {}
	virtual void doSomething() = 0;
};

class Exit :public EMP
{
public:
	Exit(int x, int y, StudentWorld *gp1);
	virtual ~Exit() {}
	virtual void doSomething();
};

class Pit :public EMP
{
public:
	Pit(int x, int y, StudentWorld *gp1);
	virtual ~Pit() {}
	virtual void doSomething();
	
};

class Goodie :public Actor
{
public:
	Goodie(int id, int x, int y, StudentWorld *gp1);
	virtual ~Goodie() {}
	virtual void doSomething() = 0;
};

class VaccineGoodie : public Goodie
{
public:
	VaccineGoodie(int x, int y, StudentWorld *gp1);
	virtual ~VaccineGoodie() {}
	virtual void doSomething();
};

class GasCanGoodie : public Goodie
{
public:
	GasCanGoodie(int x, int y, StudentWorld *gp1);
	virtual ~GasCanGoodie() {}
	virtual void doSomething();
};

class LandMineGoodie : public Goodie
{
public:
	LandMineGoodie(int x, int y, StudentWorld *gp1);
	virtual ~LandMineGoodie() {}
	virtual void doSomething();
};

class Projectile :public Actor
{
public:
	Projectile(int id, int x, int y, StudentWorld *gp1, Direction dir);
	virtual ~Projectile() {}
	virtual void doSomething()=0;
	void countInc() { count++; }
	int getCount() { return count; }

private:
	int count;
};

class Flame :public Projectile
{
public:
	Flame(int x, int y, StudentWorld *gp1, Direction dir);
	virtual ~Flame() {}
	virtual void doSomething();
};


class Mine : public EMP
{
public:
	Mine(int x, int y, StudentWorld *gp1);
	virtual ~Mine() {}
	virtual void doSomething();
	void incMineTick() { mineTick++; }
private:
	int mineTick;
};

//Remember to set people's collides to true as well so penelope collides with them

class Person : public Actor
{
public:
	Person(int id, int x, int y, StudentWorld *gp1);
	virtual ~Person() {}
	virtual void doSomething() = 0;
	bool getisZombie() { return isZombie; }
	void setIsZombie(bool t) { isZombie = t; }
	bool isParalyze();
private:
	bool isZombie;
	int pCount;
};

class Zombie : public Person
{
public:
	Zombie(int x, int y, StudentWorld *gp1);
	virtual ~Zombie() {}
	virtual void doSomething()=0;
	int getMovementPlan() { return movementPlan; }
	void setMovementPlan(int t) { movementPlan = t; }
	void vomited();
private:
	int movementPlan;

};

class DumbZombie : public Zombie
{
public:
	DumbZombie(int x, int y, StudentWorld *gp1);
	virtual ~DumbZombie() {}
	virtual void doSomething();

};

class SmartZombie : public Zombie
{
public:
	SmartZombie(int x, int y, StudentWorld *gp1);
	virtual ~SmartZombie() {}
	virtual void doSomething();
};

class Citizen : public Person
{
public:
	Citizen(int x, int y, StudentWorld *gp1);
	virtual ~Citizen() {}
	virtual void doSomething();
};

class Vomit : public Projectile
{
public:
	Vomit(int x, int y, StudentWorld *gp1, Direction dir);
	virtual ~Vomit() {}
	virtual void doSomething();
};
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
