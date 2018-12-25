/*
 mmmmm  m    m mmmmm    mm     mmm mmmmmmm
   #    ##  ## #   "#   ##   m"   "   #
   #    # ## # #mmm#"  #  #  #        #
   #    # "" # #       #mm#  #        #
 mm#mm  #    # #      #    #  "mmm"   #
*/
//20140127RC
//Developer: Arnav Dhamija
//COLLISIONS FIXED!
/*
recent changes:
	colors implemented, the spaceship doesn't disappear anymore
	enemy movement and inertia fixed
	health bar instead of -ve score
	splash screen and color improvements
	PAUSE BUTTON!
	POWERUPS

things left to do:
boss battles
a nicer GAME OVER SCREEN

cool ASCII art:

   mmm    mm   m    m  mmmmmm         mmmm  m    m mmmmmm mmmmm
 m"   "   ##   ##  ##  #             m"  "m "m  m" #      #   "#
 #   mm  #  #  # ## #  #mmmmm        #    #  #  #  #mmmmm #mmmm"
 #    #  #mm#  # "" #  #             #    #  "mm"  #      #   "m
  "mmm" #    # #    #  #mmmmm         #mm#    ##   #mmmmm #    "

	   #"
	  m"
   #      #
	  #
   #       #
	    "

and: gotoxy ((lBound + rBound - 7) / 2, lowBound + 1);
*/

#include <iostream.h>
#include <conio.h>
#include <dos.h>
#include <process.h>
#include <time.h>
#include <stdlib.h>

//boundary limits
const int topBound = 1;
const int lowBound = 40; //use 44 on Windows XP
const int rBound = 60;
const int lBound = 20;
const int maxHealth = 15;
const int spawn = 10;

unsigned long int score = 0;

//enemy/ship symbols
const char eSym = 157;
const char shipSym = 143;

struct posVector
{
	int x;
	int y;
	int status;
};

void boundaries ()
{
	textcolor (YELLOW);
	char sym = '.';
	int i;
	/*
	//top border; use only if needed
	for (i = lBound; i < rBound; i++)
	{
		gotoxy (i, topBound);
		cprintf ("%c", sym);
	} */
	//bottom border
	for (i = lBound; i <= rBound; i++)
	{
		gotoxy (i, lowBound);
		cprintf ("%c", sym);
	}
	//right border
	for (i = topBound; i < lowBound; i++)
	{
		gotoxy (lBound, i);
		cprintf ("%c", sym);
	}
	//left border
	for (i = topBound; i < lowBound; i++)
	{
		gotoxy (rBound, i);
		cprintf ("%c", sym);
	}
}

void movement (char a, posVector &ship)
{
		textcolor (LIGHTGREEN);
		if (a == 'w' && ship.y > topBound + 1)
		{
			gotoxy (ship.x, ship.y);
			cprintf (" ");
			gotoxy (ship.x, --ship.y);
			cprintf ("%c", shipSym);
		}
		else if (a == 'a' && ship.x > lBound + 1)
		{
			gotoxy (ship.x, ship.y);
			cprintf (" ");
			gotoxy (--ship.x, ship.y);
			cprintf ("%c", shipSym);
		}
		else if (a == 's' && ship.y < lowBound - 1)
		{
			gotoxy (ship.x, ship.y);
			cprintf (" ");
			gotoxy (ship.x, ++ship.y);
			cprintf ("%c", shipSym);
		}
		else if (a == 'd' && ship.x < rBound - 1)
		{
			gotoxy (ship.x, ship.y);
			cprintf (" ");
			gotoxy (++ship.x, ship.y);
			cprintf ("%c", shipSym);
		}
		else if (a == 'p')
		{
			textcolor (RED);
			gotoxy (1, 13);
			cprintf ("PAUSED");
			gotoxy (1, 14);
			cprintf ("STRIKE ANY KEY TO");
			gotoxy (1, 15);
			cprintf ("CONTINUE");
			getch ();
			gotoxy (1, 13);
			cprintf ("                  ");
			gotoxy (1, 14);
			cprintf ("                  ");
			gotoxy (1, 15);
			cprintf ("                  ");
		}
		else if (a == 'q')
			exit (0);
}

int collisions (posVector obj, posVector enemy, int type)
{
	if (obj.x == enemy.x && obj.y == enemy.y && enemy.status == 1 && type == 0)
		return 1;
	else if (obj.x == enemy.x && obj.y == enemy.y && obj.status == 1 && enemy.status == 1 && type == 1)
		return 2;
	return 0;
}

void enemyMovement (posVector &enemy, posVector * &bullets, int scoreDouble)
{
	textcolor (LIGHTBLUE);
	for (int i = 0; i < 30; i++)
		if (collisions (bullets[i], enemy, 1) == 2)
		{
			bullets[i].status = 0;
			enemy.status = 0;
			gotoxy (bullets[i].x, bullets[i].y);
			cprintf (" ");
			gotoxy (bullets[i].x, bullets[i].y - 1);
			cprintf (" ");
			if (scoreDouble > 0)
				score += 200;
			else
				score += 100;
		}
	if (enemy.y <= lowBound && enemy.y > 1 && enemy.status != 0)
	{
		gotoxy (enemy.x, enemy.y);
		cprintf ("%c", eSym);
		gotoxy (enemy.x, enemy.y - 1);
		cprintf (" ");
	}

	if (enemy.status == 0)
	{
		gotoxy (enemy.x, enemy.y);
		cprintf (" ");
		gotoxy (enemy.x, enemy.y - 1);
		cprintf (" ");
	}
}

void lifePrinter (int lives)
{
	gotoxy (1, 4);
	textcolor (LIGHTGREEN);
	cprintf ("LIVES:");
	gotoxy (1, 5);
	cout << "              ";

	for (int i = 1; i <= lives; i++)
	{
		textcolor (LIGHTRED);
		gotoxy (i, 5);
		cprintf ("%c", char (3));
	}
}

void healthPrinter (int health)
{
	_setcursortype (_NOCURSOR);
	int i;
	for (i = 1; i <= maxHealth; i++)
	{
		gotoxy (i, 8);
		cout << ' ';
	}
	for (i = 1; i <= health; i ++)
	{
		textcolor (LIGHTGREEN);
		gotoxy (1, 7);
		cprintf ("HEALTH:");
		if (health > 8 )
			textcolor (LIGHTGREEN);
		else if (health > 4)
			textcolor (YELLOW);
		else
			textcolor (LIGHTRED);
		gotoxy (i, 8);
		cprintf ("%c", char (219)); //or 179
	}
}

void doublePrinter (int doubleScore)
{
	_setcursortype (_NOCURSOR);
	int i;
	for (i = 1; i <= 5; i++)
	{
		gotoxy (i, 11);
		cout << ' ';
	}
	for (i = 1; i <= doubleScore; i++)
	{
		textcolor (LIGHTGREEN);
		gotoxy (1, 10);
		cprintf ("DOUBLE SCORE!");
		if (doubleScore > 3 )
			textcolor (LIGHTGREEN);
		else if (doubleScore > 2)
			textcolor (YELLOW);
		else
			textcolor (LIGHTRED);
		gotoxy (i, 11);
		cprintf ("%c", char (219)); //or 179
	}
}

void splashScreen ()
{
	_setcursortype(_NOCURSOR);
	textcolor (LIGHTBLUE);
	gotoxy (18, 1);
	cprintf (" mmmmm  m    m mmmmm    mm     mmm mmmmmmm\n");
	gotoxy (18, 2);
	cprintf ("   #    ##  ## #   *#   ##   m*   *   #  \n");
	gotoxy (18, 3);
	cprintf ("   #    # ## # #mmm#*  #  #  #        #  \n");
	gotoxy (18, 4);
	cprintf ("   #    # ** # #       #mm#  #        #   \n");
	gotoxy (18, 5);
	cprintf (" mm#mm  #    # #      #    #  *mmm*   #   \n");

	textcolor (LIGHTGREEN);
	gotoxy (lBound + 10, lowBound / 2);
	cprintf ("CONTROLS:");
	gotoxy (lBound + 1, lowBound / 2 + 1);
	cprintf ("MOVEMENT: WASD");
	gotoxy (lBound + 1, lowBound / 2 + 2);
	cprintf ("SHOOTING: [SPACEBAR]");
	gotoxy (lBound + 1, lowBound / 2 + 3);
	cprintf ("PAUSE: P");
	gotoxy (lBound + 1, lowBound / 2 + 4);
	cprintf ("QUIT: Q");
	gotoxy (lBound + 3, lowBound / 2 + 5);
	cprintf ("STRIKE ANY KEY TO START!");
	getch ();
	clrscr ();
}

void main ()
{
	clrscr ();
	splashScreen ();
	posVector ship, enemy[3], bullets[30], lifeUp, scoreUp;
	textcolor (LIGHTGREEN);
	int sep, i, lives = 3, count = 0, del = 45, j, k = 0, o, sep1, heat = 0, cooling = 0, enemyCount = 0, health, scoreDouble = 0;
	char sym = '.';
	unsigned int seed;
	char a;
	_setcursortype (_NOCURSOR);

	//sanitising bullets
	for (i = 0; i < 30; i++)
		bullets[i].status = 0;

	//initialising ship position
	ship.x = (rBound + lBound) / 2;
	ship.y = (topBound + lowBound) * 3 / 4;

	time_t tmp;
	seed = (unsigned)time (&tmp);
	srand (seed);

	//some initialisation
	boundaries ();
	gotoxy (ship.x, ship.y);
	cout << shipSym;

	health = maxHealth;
	healthPrinter (health);

	while (1) //safe infinite loop :)
	{
		count++;
		enemy[0].status = 1;
		enemy[1].status = 1;
		enemy[2].status = 1;
		scoreUp.status = 1;

		//powerups spawn once in <spawn value> 
		if ((count % spawn) == 0)
			lifeUp.status = 1;
		else
			lifeUp.status = 0;

		if (((count + 5) % spawn) == 0)
			scoreUp.status = 1;
		else
			scoreUp.status = 0;
			
		if ((count % 6) == 0 && (del / 1.2) > 20)
			del /= 1.2;

		//randomizing the x-coordinates of all enemies
		enemy[0].x = (rand () % (rBound - lBound - 2)) + lBound + 1;
		enemy[1].x = (rand () % (rBound - lBound - 2)) + lBound + 1;
		enemy[2].x = (rand () % (rBound - lBound - 2)) + lBound + 1;
		lifeUp.x = (rand () % (rBound - lBound - 2)) + lBound + 1;
		scoreUp.x = (rand () % (rBound - lBound - 2)) + lBound + 1;

		//randomizing separation between enemies
		sep = (rand () % 10);
		sep1 = (rand () % 10);

		enemy[0].y = 2;
		enemy[1].y = enemy[0].y - sep;
		enemy[2].y = enemy[1].y - sep1;
		lifeUp.y = 0;
		scoreUp.y = 0;
		
		while (enemy[2].y <= lowBound)
		{
			//boundaries tend to get damaged, this repairs them
			boundaries ();

			//bullet free space detection
			for (int j = 0; j < 30; j++)
				if (bullets[j].status == 0)
					break;

			//WASD navigation
			if (kbhit ())
			{
				a = getch ();
				if (a != ' ')
					movement (a, ship);
				else
				{
					if (cooling == 0)
					{
						bullets[j].x = ship.x;
						bullets[j].y = ship.y - 1;
						bullets[j].status = 1;
						heat++;
					}
					if (heat >= 15)  //change this value to allow more/less bullets on screen
					{
						gotoxy (1, 13);
						cout << "             ";
						gotoxy (1, 13);
						textcolor (LIGHTRED);
						cprintf ("OVERHEATING!");
						cooling = 1;
					}
					else
					{
						gotoxy (1, 13);
						cout << "              ";
					}
				}
			}

			//bullets with full support for collisions :)
			for (i = 0; i < 30; i++)
			{
				textcolor (LIGHTGREEN);
				if (bullets[i].status == 1 && bullets[i].y > 1)
				{
					//bullet - ship collisions
					for (o = 0; o < 3; o++)
						if (collisions (bullets[i], enemy[o], 1) == 2)
						{
							gotoxy (enemy[o].x, enemy[o].y);
							cprintf (" ");
							gotoxy (enemy[o].x, enemy[o].y - 1);
							cprintf (" ");
							bullets[i].status = 0;
							enemy[o].status = 0;
							if (scoreDouble > 0)
								score += 200;
							else
								score += 100;
						}
					if (bullets[i].status == 1)
					{
						gotoxy (bullets[i].x, --bullets[i].y);
						cprintf ("|");
						gotoxy (bullets[i].x, bullets[i].y + 1);
						cprintf (" ");
					}
				}
				else if (bullets[i].y <= 1 && bullets[i].status == 1)
				{
					gotoxy (bullets[i].x, bullets[i].y);
					cout << ' ';
					bullets[i].status = 0;
				}
				if (bullets[i].status == 0)
					k++;
			}

			//detecting whether ship hits the enemy
			for (i = 0; i < 3; i++)
				if (collisions (ship, enemy[i], 0) == 1)
				{
					if (lives == 1)
					{
						clrscr ();
						textcolor (LIGHTGREEN);
						cprintf ("GAME OVER :(");
						cout << '\n';
						cprintf ("SCORE: ");
						cprintf ("%d", score);
						getch ();
						exit (0);
					}
					else
					{
						enemy[i].status = 0;
						gotoxy (1, 10);
						cout << "             ";
						gotoxy (1, 10);
						health = maxHealth;
						textcolor (LIGHTRED);
						cprintf ("-1 LIFE");
						--lives;
					}
				}

			//detecting whether ship hits lifeUp
			if (collisions (ship, lifeUp, 0))
			{
				textcolor (GREEN);
				lives++;
				lifeUp.status = 0;
				gotoxy (ship.x, ship.y - 1);
				cprintf ("%c", ' ');
				gotoxy (ship.x, ship.y);
				cprintf ("%c", shipSym);
			}

			//similar for scoreUp
			if (collisions (ship, scoreUp, 0))
			{
				textcolor (GREEN);
				scoreDouble = 5;
				scoreUp.status = 0;
				gotoxy (ship.x, ship.y - 1);
				cprintf ("%c", ' ');
				gotoxy (ship.x, ship.y);
				cprintf ("%c", shipSym);
			}

			//GAME OVER
			if (lives == 0)
			{
				clrscr ();
				textcolor (LIGHTGREEN);
				cprintf ("GAME OVER :(");
				cout << '\n';
				cprintf ("SCORE: ");
				cprintf ("%d", score);
				getch ();
				exit (0);
			}

			//invalidates all enemies crossing the border
			for (i = 0; i < 3; i++)
				if (enemy[i].y == lowBound)
					enemy[i].status = 0;

			//invalidates lifeUp
			if (lifeUp.y == lowBound)
			{
				lifeUp.status = 0;
				gotoxy (lifeUp.x, lifeUp.y - 1);
				cprintf ("%c", ' ');
			}

			//invalidates scoreUp
			if (scoreUp.y == lowBound)
			{
				scoreUp.status = 0;
				gotoxy (scoreUp.x, scoreUp.y - 1);
				cprintf ("%c", ' ');
			}

			if (enemyCount % 2 == 0) //delays the loop a bit
			{
				//power up movement bit
				if (lifeUp.status == 1)
				{
					if (lifeUp.y > 0)
					{
						textcolor (LIGHTRED);
						gotoxy (lifeUp.x, lifeUp.y);
						cprintf("%c", 3);
						gotoxy (lifeUp.x, lifeUp.y - 1);
						cprintf("%c", ' ');
					}
					lifeUp.y++;
				}

				if (scoreUp.status == 1)
				{
					if (scoreUp.y > 0)
					{
						textcolor (YELLOW);
						gotoxy (scoreUp.x, scoreUp.y);
						cprintf("%c", 234);
						gotoxy (scoreUp.x, scoreUp.y - 1);
						cprintf("%c", ' ');
					}
					scoreUp.y++;
				}

				//moving the enemies
				for (i = 0; i < 3; i++)
					enemyMovement (enemy[i], bullets, scoreDouble);
				//little block; essential for incrementing the loop
				enemy[1].y = enemy[0].y - sep;
				enemy[2].y = enemy[1].y - sep1;
				enemy[0].y++;
			}

			//makes the game playable :)
			delay (del);

			//health reduction
			for (i = 0; i < 3; i++)
				if (enemy[i].y == lowBound && enemy[i].status == 1)
				{
					--health;
					enemy[i].status = 0;
				}

			//if health reaches 0
			if (health <= 0)
			{
				gotoxy (1, 10);
				textcolor (LIGHTRED);
				cprintf ("-1 LIFE");
				--lives;
				health = maxHealth;
			}

			//just to ensure that the ship doesn't go away after it is struck by an enemy
			gotoxy (ship.x, ship.y);
			textcolor (LIGHTGREEN);
			cprintf ("%c", shipSym);

			//little touches: score and formatting
			gotoxy (1, 1);
			textcolor (LIGHTGREEN);
			cprintf ("SCORE:");
			gotoxy (1, 2);
			cout << "                 ";
			gotoxy (1, 2);
			textcolor (LIGHTGREEN);
			cprintf ("%d",score);
			healthPrinter (health);
			lifePrinter (lives);

			//calls to print the doublePrinter
			if (scoreDouble > 0)
				doublePrinter (scoreDouble);
			else
			{
				gotoxy (1, 10);
				cout << "             ";
				gotoxy (1, 11);
				cout << "             ";
			}

			enemyCount++;
		}

		//some cleaning for good UI
		gotoxy (1, 10);
		cout << "             ";
		gotoxy (1, 11);
		cout << "             ";
		heat = 0;
		cooling = 0;
		if (scoreDouble > 0)
			scoreDouble--;
	}
}
