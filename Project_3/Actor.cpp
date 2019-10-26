#include "Actor.h"
#include "StudentWorld.h"
Wall::Wall(int x, int y, StudentWorld *gp1)
	:Actor(IID_WALL, x, y, gp1, 0, 0, 1.0)
{
	setCollide(true);
	setBlocksFire(true);
	setFlammable(false);//Set the three initial properties of a wall: That it may collide, that it blocks fire, and that it is not flammable
}
void Wall::doSomething()
{
	return;
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
Penelope::Penelope(int x, int y, StudentWorld *gp1)
	: Actor(IID_PLAYER, x, y, gp1, 0, 0, 1.0)
{
	setCollide(true);
	levelFinish = false;
	setCanBeFollowed(true);
	setCanBeInfected(true);//Set the four base properties of penelope: She can be infected, and followed, and can collide. Levelfinish informs studentworld when the level is over. 
}

void Penelope::doSomething()
{
	setAlive(getWorld()->getisPAlive());

	setFlames(getWorld()->getFlamesW());
	setVaccines(getWorld()->getVaccinesW());
	setMines(getWorld()->getMinesW());//Set all of penelope's properties (like how many mines etc.) to whatever is stored in the studentworld object. That count resets every time the level restarts
	getWorld()->setinfCountP(getInfectionCount());

	if (getInfected())
		increaseInfection();//If infected, increase infection count
	if (getInfectionCount() >= 500)//If 500 level reached, turn penelope into a zombie
	{
		setAlive(false);
		getWorld()->setisPAlive(false);
	}

	int a;
	this->getWorld()->getKey(a);
	int x_dir, y_dir;
	x_dir = getX();
	y_dir = getY();
	if (a == KEY_PRESS_LEFT)
	{
		setDirection(left);
		x_dir -= 4;
		if (!getWorld()->isCollide(x_dir, y_dir, this) )//Checks if no potential collision at prospective spot
			moveTo(x_dir, y_dir);

		if (getWorld()->isOverlap(x_dir, y_dir) && getWorld()->getCitCount() <= 0)//Checks if overlap with exit at prospective spot
			levelFinish = true;
	}//Each direction key checks if there would be a collision, and if not, moves penelope. Then it checks if there would be an overlap with anything where p is at
	if (a == KEY_PRESS_RIGHT)
	{
		setDirection(right);
		x_dir += 4;
		if (!getWorld()->isCollide(x_dir, y_dir, this))
			moveTo(x_dir, y_dir);

		if (getWorld()->isOverlap(x_dir, y_dir)&&getWorld()->getCitCount()<=0)//added citcount
			levelFinish = true;
	}
	if (a == KEY_PRESS_UP)
	{
		setDirection(up);
		y_dir += 4;

		if (!getWorld()->isCollide(x_dir, y_dir, this))
			moveTo(x_dir, y_dir);

		if (getWorld()->isOverlap(x_dir, y_dir) && getWorld()->getCitCount() <= 0)
			levelFinish = true;
	}
	if (a == KEY_PRESS_DOWN)
	{
		setDirection(down);
		y_dir -= 4;
		if (!getWorld()->isCollide(x_dir, y_dir, this))
			moveTo(x_dir, y_dir);

		if (getWorld()->isOverlap(x_dir, y_dir) && getWorld()->getCitCount() <= 0)
			levelFinish = true;
	}
	if (a == KEY_PRESS_SPACE && getFlames() > 0)
	{
		getWorld()->playSound(SOUND_PLAYER_FIRE);
		getWorld()->decFlamesW();
		getWorld()->setIsFlame(true);
		getWorld()->setPenDir(getDirection());//If its a flame, play a sound, decrement the flame count in studentworld, set the presence of a flame to true, and update studentworld on penelope's direction
	}
	if (a == KEY_PRESS_TAB && getMines() > 0)
	{
		getWorld()->decMinesW();
		getWorld()->setMinePlaced(true);
	}
	if (a == KEY_PRESS_ENTER && getVaccines() > 0)
	{
		getWorld()->decVaccinesW();
		setInfectionCount(0);
		setInfected(false);
	}
}




///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

EMP::EMP(int id, int x, int y, StudentWorld *gp1, int depth)
	:Actor(id, x, y, gp1, 0, depth, 1.0)
{
	setOverlap(true);
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


Exit::Exit(int x, int y, StudentWorld *gp1)
	:EMP(IID_EXIT, x, y, gp1, 1)
{
	setBlocksFire(true);
	setFlammable(false);
}
void Exit::doSomething()
{
	getWorld()->citsLeave(getX(), getY());
	if (getWorld()->isOverlap(getX(), getY()) && getWorld()->getCitCount() <= 0)
	{
		getWorld()->setLevelFinish(true);
		getWorld()->playSound(SOUND_LEVEL_FINISHED);

	}
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

Pit::Pit(int x, int y, StudentWorld *gp1)
	:EMP(IID_PIT, x, y, gp1, 0)
{
	setFlammable(false);
}

void Pit::doSomething()
{
	if (getWorld()->isOverlap(getX(), getY()))
		getWorld()->setisPAlive(false);
	bool t = getWorld()->deathByMinePit(getX(), getY());
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

Goodie::Goodie(int id, int x, int y, StudentWorld *gp1)
	:Actor(id, x, y, gp1, 0, 1, 1.0)
{
	setOverlap(true);
	setFlammable(true);
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

VaccineGoodie::VaccineGoodie(int x, int y, StudentWorld *gp1)
	:Goodie(IID_VACCINE_GOODIE, x, y, gp1)
{


}

void VaccineGoodie::doSomething()
{
	if (!getAlive())
		return;
	if (getWorld()->isOverlap(getX(), getY()))
	{
		getWorld()->increaseScore(50);
		setAlive(false);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->incVaccinesW();
		//increase vaccine count, add 50 points. Do the same for all goodies.
	}
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

GasCanGoodie::GasCanGoodie(int x, int y, StudentWorld *gp1)
	:Goodie(IID_GAS_CAN_GOODIE, x, y, gp1)
{

}

void GasCanGoodie::doSomething()
{
	if (!getAlive())
		return;
	if (getWorld()->isOverlap(getX(), getY()))
	{
		getWorld()->increaseScore(50);
		setAlive(false);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->incFlamesW();
	}
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

LandMineGoodie::LandMineGoodie(int x, int y, StudentWorld *gp1)
	:Goodie(IID_LANDMINE_GOODIE, x, y, gp1)
{

}

void LandMineGoodie::doSomething()
{
	if (!getAlive())
		return;
	if (getWorld()->isOverlap(getX(), getY()))
	{
		getWorld()->increaseScore(50);
		setAlive(false);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->incMinesW();
	}
}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

Projectile::Projectile(int id, int x, int y, StudentWorld *gp1, Direction dir)
	:Actor(id, x, y, gp1, dir, 0, 1)//switched dir and 0
{
	count = 0;
}

void Projectile::doSomething()
{

}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

Flame::Flame(int x, int y, StudentWorld *gp1, Direction dir)
	:Projectile(IID_FLAME, x, y, gp1, dir)
{

}

void Flame::doSomething()
{
	if (!getAlive())
		return;
	if (getCount() == 2) {
		setAlive(false);
	}
	countInc();//checks and increments how many ticks the flame has been alive for
	getWorld()->isFireOverlap(getX(), getY());
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

Mine::Mine(int x, int y, StudentWorld *gp1)
	:EMP(IID_LANDMINE, x, y, gp1, 1)
{
	mineTick = 0;
	setExplodes(true);
}

void Mine::doSomething()
{
	if (!getAlive())
	{
		return;
	}
	if (mineTick <= 30)
	{
		incMineTick();
		return;
	}
	if (getWorld()->isOverlap(getX(), getY())||getWorld()->deathByMinePit(getX(), getY()))//If somebody overlaps with the mine
	{
		setAlive(false);
		getWorld()->octaFlame(getX(), getY());//call a method that generates the 9 flame pattern
	}

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

Person::Person(int id, int x, int y, StudentWorld *gp1)
	:Actor(id, x, y, gp1, 0, 0, 1)
{
	isZombie = false;
	pCount = 1;
	setisPerson(true);
	setCollide(true);
	setDeathByPit(true);
}

bool Person::isParalyze()
{
	if (pCount == 2)
	{
		pCount--;
		return true;
	}
	pCount++;
	return false;//Checks if the citizen is paralyzed on the current tick
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

Zombie::Zombie(int x, int y, StudentWorld *gp1)
	:Person(IID_ZOMBIE, x, y, gp1)
{
	setIsZombie(true);
	//isSmart = false;
	movementPlan = 0;
	setCanFollow(true);
}

void Zombie::vomited()
{
	int vomitX = getX();
	int vomitY = getY();
	switch (getDirection())
	{
	case GraphObject::right:
		vomitX += SPRITE_WIDTH;
		break;
	case GraphObject::up:
		vomitY += SPRITE_HEIGHT;
		break;
	case GraphObject::left:
		vomitX -= SPRITE_WIDTH;
		break;
	case GraphObject::down:
		vomitY -= SPRITE_HEIGHT;
		break;
	}
	if (getWorld()->isVomitOverlap(vomitX, vomitY))
	{
		int a = randInt(1, 3);
		if (a == 1)
		{
			getWorld()->createVomit(vomitX, vomitY);
			getWorld()->playSound(SOUND_ZOMBIE_VOMIT);//If vomit would overlap with anything, then generate vomit at that spot

		}
	}

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

DumbZombie::DumbZombie(int x, int y, StudentWorld *gp1)
	:Zombie(x, y, gp1)
{
	//setIsSmart(false);
	setisPerson(true);
}

void DumbZombie::doSomething()
{

	if (!getAlive())
		return;
	if (isParalyze())
		return;
	vomited();
	if (getMovementPlan() == 0)
	{
		int a = randInt(3, 10);
		setMovementPlan(a);
		int b = randInt(0, 3) * 90;
		setDirection(b);//select random direction and set it
	}
	int destX = getX();
	int destY = getY();
	switch (getDirection())
	{
	case GraphObject::right:
		destX++;
		break;
	case GraphObject::up:
		destY++;
		break;
	case GraphObject::left:
		destX--;
		break;
	case GraphObject::down:
		destY--;
		break;//update prospective x and y of the zombie if the move is possible

	}
	if (!(getWorld()->isCollide(destX, destY, this)))//added to check if the move is possible
	{
		
		moveTo(destX, destY);
		setMovementPlan(getMovementPlan() - 1);
	}
	else
		setMovementPlan(0);
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

SmartZombie::SmartZombie(int x, int y, StudentWorld *gp1)
	:Zombie(x, y, gp1)
{
	//setIsSmart(false);
	setisPerson(true);
	setWorth2K(true);
}

void SmartZombie::doSomething()
{

	if (!getAlive())
		return;
	if (isParalyze())
		return;
	vomited();
	if (getMovementPlan() == 0)
	{
		int a = randInt(3, 10);
		setMovementPlan(a);
		Direction bx = 0;
		Direction by = 0;
		int zd = getWorld()->findClosestVictim(getX(), getY(), bx, by);//Find closest victim to attack
		if (zd == 4) 
		{
			int b = randInt(0, 3) * 90;
			setDirection(b);
		}
		if (zd == 1)
		{
			setDirection(bx);
		}
		if (zd == 2)
		{
			setDirection(by);
		}
		if (zd == 3)
		{
			int r = randInt(1, 2);
			if (r == 1)
				setDirection(bx);
			else
				setDirection(by);
		}
		
	}
	int destX = getX();
	int destY = getY();
	
	switch (getDirection())//Question: if zombie is separated from penelope by a wall, will it ever try to get around the wall?
	{
	case GraphObject::right:
		destX++;
		break;
	case GraphObject::up:
		destY++;
		break;
	case GraphObject::left:
		destX--;
		break;
	case GraphObject::down:
		destY--;
		break;

	}
	if (!(getWorld()->isCollide(destX, destY, this)))//added
	{

		moveTo(destX, destY);
		setMovementPlan(getMovementPlan() - 1);
	}
	else
		setMovementPlan(0);
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

Citizen::Citizen(int x, int y, StudentWorld *gp1)
	:Person(IID_CITIZEN, x, y, gp1)
{
	setCanBeFollowed(true);
	setCanExit(true);
	setCanBeInfected(true);
}

void Citizen::doSomething()
{
	if (!getAlive())
		return;
	if (!isParalyze())
		return;
	if (getInfected())
		increaseInfection();
	if (getInfectionCount() >= 500)
	{
		setAlive(false);
		getWorld()->setCitCount(getWorld()->getCitCount() - 1);
		getWorld()->incScore(-1000);
		getWorld()->spawnZombie(getX(), getY(), randInt(1, 10));//If citizen dies to infection
	}
	int dist_p = 0;
	int dist_z = 9999;
	Direction x1 = 0;
	dist_z = getWorld()->findClosestZombie(getX(), getY(), x1);
	bool a = getWorld()->getDistFromPen(getX(), getY(), dist_p);//this, findclosest zombie, and determine same RC are all helper methods in studentworld
	if (dist_p < dist_z &&a)//if penelope is within 80 and no zombie is closer than pen
	{
		Direction b = getDirection();
		Direction b1=0;
		int m = getWorld()->determineSameRC(getX(), getY(), b, b1);
		if (m == 1)
		{
			setDirection(b);
			if (b == GraphObject::up)
			{
				if (!getWorld()->isCollide(getX(), getY()+2, this))
					moveTo(getX(), getY() + 2);
			}
			if (b == GraphObject::down)
			{
				if (!getWorld()->isCollide(getX(), getY()-2, this))
					moveTo(getX(), getY() - 2);
			}
			return;
		}//check each direction and viability of move there
		else if (m == 2)
		{
			setDirection(b);
			if (b == GraphObject::left)
			{
				if (!getWorld()->isCollide(getX()-2, getY(), this))
					moveTo(getX()-2, getY());
			}
			if (b == GraphObject::right)
			{
				if (!getWorld()->isCollide(getX()+2, getY(), this))
					moveTo(getX()+2, getY());
			}
			return;
		}
		else if (m == 0)//if penelope isnt on the same row or column as the cit
		{
			int r = randInt(1, 2);
			if (r == 1)//b1, which is y
			{
				if (b1 == GraphObject::up)
				{
					if (!getWorld()->isCollide(getX(), getY() + 2, this))
					{
						setDirection(b1);
						moveTo(getX(), getY() + 2);
					}
				}
				if (b1 == GraphObject::down)
				{
					if (!getWorld()->isCollide(getX(), getY() - 2, this))
					{
						setDirection(b1);
						moveTo(getX(), getY() - 2);
					}
				}//choose one random direction
			}
			else//b, which is x
			{
				if (b == GraphObject::right)
				{
					if (!getWorld()->isCollide(getX()+2, getY(), this))
					{
						setDirection(b);
						moveTo(getX()+2, getY());
					}
				}
				if (b == GraphObject::left)
				{
					if (!getWorld()->isCollide(getX()-2, getY(), this))
					{
						setDirection(b);
						moveTo(getX()-2, getY());
					}
				}//choose one random direction and implement
			}
		}


	}
	else//this is if the zombie is closer to citizen than the player.
	{
		if (dist_z >= 6400)//if distance greater than 80
			return;
		int destX = getX();
		int destY = getY();
		int z1, z2, z3, z4;
		z1 = z2 = z3 = z4 = 0;//variables to store the distance to the nearest zombie from each of the four prospective parts
		if (!getWorld()->isCollide(destX, destY+2, this))//up
		{
			z1 = getWorld()->findClosestZombie(destX, destY + 2, x1);
		}
		if (!getWorld()->isCollide(destX, destY - 2, this))//down
		{
			z2 = getWorld()->findClosestZombie(destX, destY - 2, x1);
		}
		if (!getWorld()->isCollide(destX+2, destY, this))//right
		{
			z3 = getWorld()->findClosestZombie(destX+2, destY, x1);
		}
		if (!getWorld()->isCollide(destX-2, destY, this))//left
		{
			z4 = getWorld()->findClosestZombie(destX-2, destY, x1);
		}
		if (dist_z >= z1 && dist_z >= z2 && dist_z >= z3 && dist_z >= z4)
			return;
		else
		{
			if (z1 >= dist_z && z1 >= z2 && z1 >= z3 && z1 >= z4)//up is best
			{
				setDirection(90);
				moveTo(destX, destY+2);
				return;
			}
			if (z2 >= dist_z && z2 >= z1 && z2 >= z3 && z2 >= z4)//down is best
			{
				setDirection(270);
				moveTo(destX, destY-2);
				return;
			}
			if (z3 >= dist_z && z3 >= z1 && z3 >= z2 && z3 >= z4)//right is best
			{
				setDirection(0);
				moveTo(destX+2, destY);
				return;
			}
			if (z4 >= dist_z && z4 >= z2 && z4 >= z3 && z4 >= z1)//left is best
			{
				setDirection(180);
				moveTo(destX-2, destY);
				return;
			}
		}
	}
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

Vomit::Vomit(int x, int y, StudentWorld *gp1, Direction dir)
	:Projectile(IID_VOMIT, x, y, gp1, dir)
{

}

void Vomit::doSomething()
{
	if (!getAlive())
		return;
	if (getCount() == 2) {
		setAlive(false);
	}
	countInc();
	bool t= getWorld()->isVomitOverlap(getX(), getY());
}

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
