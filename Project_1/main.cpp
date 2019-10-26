#include "Game.h"
int main()
{
	// Create a game
	// Use this instead to create a mini-game:   Game g(3, 4, 2);
	Game g(5, 6, 6);

	// Play the game
	g.play();
}
/*
#include "Arena.h"
#include "History.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
using namespace std;

int main()
{
	Arena a(4, 4);
	a.addPlayer(1, 2);
	for (int c = 1; c <= 27; c++)
		a.addZombie(1, 1);
	for (int c=1;c<=27;c++)
	a.player()->moveOrAttack(LEFT);
	
	a.history().display();
	cout << "====" << endl;
}*/