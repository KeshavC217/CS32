#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;
#include "Actor.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include "Level.h" // required to use our provided class 

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
	: GameWorld(assetPath)
{
	levelFinish = false;
	isPAlive = true;
	m_Penelope = nullptr;
	wFlames = wMines = wVaccines = infectCountP=citCount = 0;
	isFlame = false;
	penDir = 0;
	minePlaced = false;
	citCount = 0;
	infectCountP = 0;
}//Initialize all base values

int StudentWorld::init()
{
	if (getLevel() > 99)
		return GWSTATUS_PLAYER_WON;
	Level lev(assetPath());
	ostringstream oss;
	oss.fill('0');
	oss << "level" << setw(2) << getLevel() << ".txt";
	string s = oss.str();
	string levelFile = s;
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found)
		return GWSTATUS_PLAYER_WON;
	else if (result == Level::load_fail_bad_format)
		return GWSTATUS_LEVEL_ERROR;
	else if (result == Level::load_success) {
		cerr << "Successfully loaded level" << endl;
		for (int c = 0; c < LEVEL_WIDTH; c++)
		{
			for (int j = 0; j < LEVEL_HEIGHT; j++)
			{
				Level::MazeEntry ge = lev.getContentsOf(c, j); // level_x=5, level_y=10  
				switch (ge)
					// so x=80 and y=160   
				{
				case Level::empty:
					break;
				case Level::smart_zombie:
					m_Actors.push_back(new SmartZombie(c*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
					break;
				case Level::dumb_zombie:
					m_Actors.push_back(new DumbZombie(c*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
					break;
				case Level::player:
					m_Penelope = new Penelope(c*SPRITE_WIDTH, j*SPRITE_HEIGHT, this);
					break;
				case Level::exit:
					m_Actors.push_back(new Exit(c * SPRITE_WIDTH, j * SPRITE_HEIGHT, this));
					break;
				case Level::wall:
					m_Actors.push_back(new Wall(c*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
					break;
				case Level::pit:
					m_Actors.push_back(new Pit(c*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
					break;
				case Level::gas_can_goodie:
					m_Actors.push_back(new GasCanGoodie(c*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
					break;
				case Level::landmine_goodie:
					m_Actors.push_back(new LandMineGoodie(c*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
					break;
				case Level::vaccine_goodie:
					m_Actors.push_back(new VaccineGoodie(c*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
					break;
				case Level::citizen:
					m_Actors.push_back(new Citizen(c*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
					citCount++;
				}
			}
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::isFireBlocked(int x, int y)
{
	vector<Actor*>::iterator it;
	it = m_Actors.begin();
	for (int c = 0; c < m_Actors.size(); c++)
	{
		if ((*it)->getBlocksFire())//getblocks fire is a property of objects
		{
			int destX = (*it)->getX();
			int destY = (*it)->getY();
			if (pow(x - destX, 2) + pow(y - destY, 2) <= 100)
				return true;//if overlap, returns true
		}
		it++;
	}
	return false;
}

void StudentWorld::octaFlame(int destx, int desty)
{
	if (!isFireBlocked(destx, desty))
		m_Actors.push_back(new Flame(destx, desty, this, 90));
	if (!isFireBlocked(destx - SPRITE_WIDTH, desty))
		m_Actors.push_back(new Flame(destx - SPRITE_WIDTH, desty, this, 90));
	if (!isFireBlocked(destx + SPRITE_WIDTH, desty))
		m_Actors.push_back(new Flame(destx + SPRITE_WIDTH, desty, this, 90));
	if (!isFireBlocked(destx - SPRITE_WIDTH, desty - SPRITE_HEIGHT))
		m_Actors.push_back(new Flame(destx - SPRITE_WIDTH, desty - SPRITE_HEIGHT, this, 90));
	if (!isFireBlocked(destx - SPRITE_WIDTH, desty + SPRITE_HEIGHT))
		m_Actors.push_back(new Flame(destx - SPRITE_WIDTH, desty + SPRITE_HEIGHT, this, 90));
	if (!isFireBlocked(destx + SPRITE_WIDTH, desty - SPRITE_HEIGHT))
		m_Actors.push_back(new Flame(destx + SPRITE_WIDTH, desty - SPRITE_HEIGHT, this, 90));
	if (!isFireBlocked(destx + SPRITE_WIDTH, desty + SPRITE_HEIGHT))
		m_Actors.push_back(new Flame(destx + SPRITE_WIDTH, desty + SPRITE_HEIGHT, this, 90));
	if (!isFireBlocked(destx, desty + SPRITE_HEIGHT))
		m_Actors.push_back(new Flame(destx, desty + SPRITE_HEIGHT, this, 90));
	if (!isFireBlocked(destx, desty - SPRITE_HEIGHT))
		m_Actors.push_back(new Flame(destx, desty - SPRITE_HEIGHT, this, 90));//spam 9 flames in the square

}

int StudentWorld::move()
{
	ostringstream oss;
	oss.fill('0');
	oss << "Score  " << setw(6) << internal << getScore();
	oss.fill(' ');
	oss << " Level " << getLevel() << " Lives " << getLives() << " Flames "  << getFlamesW() << " Mines "  << getMinesW() << " Vaccines "  << getVaccinesW()<<" Infected "  <<infectCountP;
	string s = oss.str();
	setGameStatText(s);

	if (m_Penelope->getAlive())
		m_Penelope->doSomething();
	int destx = m_Penelope->getX();
	int desty = m_Penelope->getY();
	if (isFlame)//If a flame has appeared in the scope of the world
	{
		isFlame = false;
		int a = penDir;
		//destX was here
		for (int c = 1; c <= 3; c++)
		{
			if (a == 0)
			{
				if (isFireBlocked(destx + c * SPRITE_WIDTH, desty))
					break;
				m_Actors.push_back(new Flame(destx + c * SPRITE_WIDTH, desty, this, a));
			}
			if (a == 90)
			{
				if (isFireBlocked(destx, desty + c * SPRITE_HEIGHT))
					break;
				m_Actors.push_back(new Flame(destx, desty + c * SPRITE_HEIGHT, this, a));
			}
			if (a == 180)
			{
				if (isFireBlocked(destx - c * SPRITE_WIDTH, desty))
					break;
				m_Actors.push_back(new Flame(destx - c * SPRITE_WIDTH, desty, this, a));
			}
			if (a == 270)
			{
				if (isFireBlocked(destx, desty - c * SPRITE_HEIGHT))
					break;
				m_Actors.push_back(new Flame(destx, desty - c * SPRITE_HEIGHT, this, a));
			}//check direction of penelope, and if a flame can be generated in that direction, generate it. 
		}
	}

	if (minePlaced)
	{
		m_Actors.push_back(new Mine(destx, desty, this));
		minePlaced = false;//generate a mine at penelopes location
	}

	vector<Actor*>::iterator it;
	it = m_Actors.begin();
	for (int c = 0; c < m_Actors.size(); c++)
	{
		if ((*it)->getAlive())
			(*it)->doSomething();
		else
		{
			delete (*it);
			it = m_Actors.erase(it);
			it--;//addition
		}
		it++;//This deletes and consequently destructs ever member of the world that isnt alive. If it is alive, it does something
	}


	if (!getisPAlive())
	{
		playSound(SOUND_PLAYER_DIE);
		setisPAlive(true);
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	if (getLevelFinish())//was m_p's getlevel
	{
		setLevelFinish(false);
		wFlames = wMines = wVaccines = citCount = 0;//added citcount
		return GWSTATUS_FINISHED_LEVEL;
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::findClosestVictim(int x, int y, int &d1, int &d2)
{
	int minx = 1000;
	int miny = 1000;
	vector<Actor*>::iterator it;
	it = m_Actors.begin();
	for (int c = 0; c < m_Actors.size(); c++)
	{
		Actor *a = *it;//loop through all characters and check which ones a zombie can chase
		if (!a->getcanBeFollowed())
		{
			it++;
			continue;
		}

		int destX = a->getX();
		int destY = a->getY();
		if (pow(x - destX, 2) + pow(y - destY, 2) <= pow(x-minx, 2) + pow(y-miny, 2))
		{
			minx = destX;
			miny = destY;
		}
		it++;//keep updating the distance to the closest thing that can be infected
	}
	int pX = m_Penelope->getX();
	int pY = m_Penelope->getY();
	if (pow(x - pX, 2) + pow(y - pY, 2) <= pow(x - minx, 2) + pow(y - miny, 2))
	{
		minx = pX;
		miny = pY;
	}
	if (pow(x - minx, 2) + pow(y - miny, 2) >= 6400)
	{
		return 4;//if minimum is greater than 80, return 4, which causes random movement
	}
	if (x == minx)
	{
		if (miny > y)
		{
			d1 = 90;//set the preferred y direction to up
		}
		else
		{
			d1 = 270;//set preffered y to down
		}
		return 1;
	}
	if (y == miny)
	{
		if (minx > x)
		{
			d2 = 0;//set preferred x to up
		}
		else
		{
			d2 = 180;//set preferred x to down, the function calls all 4 of these variables by reference so their values go back to actor
		}
		return 2;
	}
	if (minx > x)
		d1 = 0;//not on same row or col, just return one of the two directions the zombie may randomly pick from
	else
		d1 = 180;
	if (miny > y)
		d2 = 90;
	else
		d2 = 270;
	return 3;
}

int StudentWorld::findClosestZombie(int x, int y, int &d1)// return distance// d1 is useless but i cant remove it because too many changes throughout my code
{
	int minx = 1000;
	int miny = 1000;
	vector<Actor*>::iterator it;
	it = m_Actors.begin();
	for (int c = 0; c < m_Actors.size(); c++)
	{
		Actor *a = *it;
		if (!a->getcanFollow())
		{
			it++;
			continue;
		}

		int destX = a->getX();
		int destY = a->getY();//if the actor being examined is a zombie, smart or dumb
		if (pow(x - destX, 2) + pow(y - destY, 2) <= pow(x - minx, 2) + pow(y - miny, 2))
		{
			minx = destX;
			miny = destY;
		}
		it++;//continuously update the minimum
	}
	if (miny - y > minx - x)
	{
		if (y > miny)
			d1 = GraphObject::left;
		else
			d1 = GraphObject::right;//set preferred y direction so that cit may escape
	}
	else
	{
		if (x > minx)
			d1 = GraphObject::up;
		else
			d1 = GraphObject::down;//set preferred x such that cit may escape
	}
	return (pow(x - minx, 2) + pow(y - miny, 2));//return the square of the euclidean distance
}

void StudentWorld::cleanUp()
{
	if (m_Penelope != nullptr)
	{
		delete m_Penelope;
		m_Penelope = nullptr;
		vector<Actor*>::iterator it;
		it = m_Actors.begin();
		for (int c = 0; c < m_Actors.size(); c++)
		{
			Actor *a = *it;
			delete a;
			it++;
		}
		m_Actors.clear();//delete all actors, dead or alive
	}
}

bool StudentWorld::isCollide(int x, int y, Actor *p)
{
	vector <Actor*>::iterator it;
	it = m_Actors.begin();
	for (int c = 0; c < m_Actors.size(); c++)
	{
		Actor *a = *it;

		if (a->getCollide())//if the actor can collide with whatever calls this function
		{
			if (a == p) {
				it++;
				continue;
			}
			int p1X = a->getX();
			int p1Y = a->getY();

			if (abs(x - p1X) <= (SPRITE_WIDTH - 1) && abs(y - p1Y) <= (SPRITE_HEIGHT - 1)) {
				return true;
			}
			if (p != m_Penelope)
			{
				int pX = m_Penelope->getX();
				int pY = m_Penelope->getY();
				if (abs(x - pX) <= (SPRITE_WIDTH - 1) && abs(y - pY) <= (SPRITE_HEIGHT - 1))
					return true;
			}

		}
		it++;
	}
	return false;
}



bool StudentWorld::isOverlap(int x, int y)
{
	int destX = m_Penelope->getX();
	int destY = m_Penelope->getY();
	if (pow(x - destX, 2) + pow(y - destY, 2) <= 100)
		return true;
	return false;//returns true if caller overlaps with pen, because flames exit and goodies all do this
}

void StudentWorld::citsLeave(int x, int y)
{
	vector<Actor*>::iterator it;
	it = m_Actors.begin();
	bool b = false;
	for (int c = 0; c < m_Actors.size(); c++)
	{
		Actor *a = *it;
		if (a->getCanExit())
		{
			if (pow(x - a->getX(), 2) + pow(y - a->getY(), 2) <= 100) {
				a->setAlive(false);
				citCount--;
				b = true;
				increaseScore(500);
				playSound(SOUND_CITIZEN_SAVED);//if a citizen leaves through the exit
			}
		}
		it++;
	}
}

bool StudentWorld::deathByMinePit(int x, int y)
{
	vector<Actor*>::iterator it;
	it = m_Actors.begin();
	bool b = false;
	for (int c = 0; c < m_Actors.size(); c++)
	{
		Actor *a = *it;
		if (a->getDeathByPit())
		{
			if (pow(x - a->getX(), 2) + pow(y - a->getY(), 2) <= 100) {
				b = true;
				if (a->getCanExit())//This filters out everything else but citizens
				{
					if (a->getAlive())
					{
						increaseScore(-1000);
						citCount--;
						playSound(SOUND_CITIZEN_DIE);//If a citizen dies because of a mine or pit
					}
				}
				else if(a->getcanFollow()) {//Filters everything but smart and dumb zombies
					if (a->getAlive())
					{
						if (a->getWorth2k())
							increaseScore(2000);
						else
							increaseScore(1000);
						playSound(SOUND_ZOMBIE_DIE);//If a zombie dies because of a mine or pit

					}

				}
				a->setAlive(false);

			}
		}
		it++;
	}
	return b;
}

void StudentWorld::isFireOverlap(int x, int y)
{
	vector<Actor*>::iterator it;
	it = m_Actors.begin();
	int penX = m_Penelope->getX();
	int penY = m_Penelope->getY();
	if (pow(x - penX, 2) + pow(y - penY, 2) <= 100)
	{
		m_Penelope->setAlive(false);
		setisPAlive(false);//If any flame overlaps with penelope
	}
	for (int c = 0; c < m_Actors.size(); c++)
	{
		if ((*it)->getFlammable())
		{
			int destX = (*it)->getX();
			int destY = (*it)->getY();
			if (pow(x - destX, 2) + pow(y - destY, 2) <= 100)
			{
				if ((*it)->getExplodes()&&(*it)->gethasExploded()==false)
				{
					octaFlame(destX, destY);
					m_Actors.push_back(new Pit(destX, destY, this));
					(*it)->sethasExploded(true);
					(*it)->setAlive(false);
					playSound(SOUND_LANDMINE_EXPLODE);//if a mine explodes after fire overlaps with it
				}
				if ((*it)->getCanExit()&&(*it)->getAlive()) {
					increaseScore(-1000);
					citCount--;
					(*it)->setAlive(false);
					playSound(SOUND_CITIZEN_DIE);//if the fire kills a citizen
				}
				else if ((*it)->getcanFollow()) 
				{
					if ((*it)->getWorth2k()) 
					{
						increaseScore(2000);
						(*it)->setAlive(false);//if the fire kills a zombie
					}
					else
					{
						(*it)->setAlive(false);
						increaseScore(1000);
						int r = randInt(1, 10);
						if (r == 4) 
						{
							int vX = 0;
							int vY = 0;
							int b = randInt(0, 3) * 90;
							if (b == 0)
								vX += SPRITE_WIDTH;
							if (b == 90)
								vY += SPRITE_HEIGHT;
							if (b == 180)
								vX -= SPRITE_WIDTH;
							if (b == 270)
								vY -= SPRITE_HEIGHT;
							if (!overlapWithAnything(vX, vY))
								m_Actors.push_back(new VaccineGoodie(destX, destY, this));//The randomly generated goodie that the dumb zombie throws if it dies, 1/10 chance
						}
					}
					playSound(SOUND_ZOMBIE_DIE);

				}
				(*it)->setAlive(false);
			}//took out 				(*it)->setAlive(false); from the line before and distributed
		}

		it++;
	}
}

bool StudentWorld::overlapWithAnything(int x, int y)
{
	vector<Actor*>::iterator it;
	it = m_Actors.begin();
	for (int c = 0; c < m_Actors.size(); c++)
	{
		Actor *a = *it;
		int destX = a->getX();
		int destY = a->getY();
		if (pow(x - destX, 2) + pow(y - destY, 2) <= 100)
			return true;//If the object at x and y overlaps with anything at all
		it++;
	}
	return false;
}

bool StudentWorld::isVomitOverlap(int x, int y)
{
	vector<Actor*>::iterator it;
	it = m_Actors.begin();
	int penX = m_Penelope->getX();
	int penY = m_Penelope->getY();
	if (pow(x - penX, 2) + pow(y - penY, 2) <= 100)
	{
		m_Penelope->setInfected(true);
		return true;//If vomit overlaps with penelope, set her infected to true
	}
	for (int c = 0; c < m_Actors.size(); c++)
	{
		if ((*it)->getCanBeInfected())
		{
			int destX = (*it)->getX();
			int destY = (*it)->getY();
			if (pow(x - destX, 2) + pow(y - destY, 2) <= 100)
			{	
				if(!(*it)->getInfected())
					playSound(SOUND_CITIZEN_INFECTED);//If an actor can be infected, its probably a citizen, so infect it
				(*it)->setInfected(true);//make infected member
					return true;
			}
		}
		it++;
	}
	return false;
}

void StudentWorld::createVomit(int x, int y)
{
	m_Actors.push_back(new Vomit(x, y, this, 0));//create vomit at a certain specified location
}

void StudentWorld::spawnZombie(int x, int y, int r)
{
		if (r <= 3)
			m_Actors.push_back(new SmartZombie(x, y, this));
		else
			m_Actors.push_back(new DumbZombie(x, y, this));//if the citizen is dead, decide which zombie to spawn
		playSound(SOUND_ZOMBIE_BORN);

}

bool StudentWorld::getDistFromPen(int x, int y, int &dp)
{
	int destX = m_Penelope->getX();
	int destY = m_Penelope->getY();
	dp = pow(x - destX, 2) + pow(y - destY, 2);
	if (pow(x - destX, 2) + pow(y - destY, 2) <= 6400)
		return true;//Checks if distance from penelope and returns true if less than 80
	return false;
}

int StudentWorld::determineSameRC(int x, int y, int &d, int &d1)//return 1 for same x and 2 for same y, 0 if neither
{
	int destX = m_Penelope->getX();
	int destY = m_Penelope->getY();
	if (destX == x) {
		if (y > destY)
			d = GraphObject::down;
		else
			d = GraphObject::up;
		return 1;//if same x
	}
	if (destY == y) {
		if (x > destX)
			d = GraphObject::left;
		else
			d = GraphObject::right;
		return 2;//if same y
	}
	else 
	{
		if (x > destX)
			d = GraphObject::left;
		else
			d = GraphObject::right;
		if (y > destY)
			d1 = GraphObject::down;
		else
			d1 = GraphObject::up;
		return 0;//if neither, return two directions that it could move
	}
}

void StudentWorld::loadLevel()
{
	
}
