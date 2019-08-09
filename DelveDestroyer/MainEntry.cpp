#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream> 
#include <array>
#include <time.h>  
#include <ctype.h>
#include <windows.h>
#include <vector>


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

bool shoot = false;
bool shootPossible = false;
bool hit = false;


double speedMulti = 0;

struct Player
{
	double pos[2];			//0 = position x, 1 = position y
	GLfloat Triangle[3];	//0 = left point, 1 = top point, 2 = right point
	double speed;
	bool move[4];			// 0 = left, 1 = right, 2 = down 3 = up
}Player, control;

struct WallPoints
{
	bool active = false;
	double pos[2];			//0 = position x, 1 = position y
	GLfloat Line[1];

	struct WallVals
	{
		double slope;
		double b;
	}WallVal;
};
std::vector<WallPoints> LeftP;
std::vector<WallPoints> RightP;

struct Bullet
{
	bool active = false;
	double pos[2];
	GLfloat Line[1];
	double speed;
};
std::vector<Bullet> bullets;

struct Boulders
{
	double centre[1];
	bool active = false;
	GLfloat Point[8][2];
	double speed;

	double slope;
	double b;
};
std::vector<Boulders> Boulder;
void CollisionController()
{
	speedMulti = 0;
	hit = true;
	Player.pos[0] = 0;
	Player.pos[1] = -0.7;
}
void RenderObjects()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLineWidth(4);
	glColor3f(.0, 1.0, 0);

	if (hit)
	{
		//glColor3f(1.0, .0, 0);
	}
	else
	{
		//lColor3f(.0, 1.0, 0);
	}

	glBegin(GL_TRIANGLES);
	glVertex3f(Player.Triangle[0] + Player.pos[0], 0 + Player.pos[1], 0);
	glVertex3f(0 + Player.pos[0], Player.Triangle[1] + Player.pos[1], 0);
	glVertex3f(Player.Triangle[2] + Player.pos[0], 0 + Player.pos[1], 0);
	glEnd();
	glColor3f(.0, 1.0, 0);
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i].active == true)
		{
			glLineWidth(4);
			glBegin(GL_LINES);
			glVertex3f(bullets[i].pos[0], bullets[i].pos[1], 0);
			glVertex3f(bullets[i].pos[0], bullets[i].pos[1] + 0.05, 0);
			glEnd();
		}
	}

	for (int i = 0; i < RightP.size(); i++)
	{
		if (RightP[i].active && i > 0)
		{

			glBegin(GL_LINES);
			glVertex3f(RightP[i - 1].pos[0], RightP[i - 1].pos[1], 0);
			glVertex3f(RightP[i].pos[0], RightP[i].pos[1], 0);
			glEnd();
		}
		if (LeftP[i].active && i > 0)
		{

			glBegin(GL_LINES);
			glVertex3f(LeftP[i - 1].pos[0], LeftP[i - 1].pos[1], 0);
			glVertex3f(LeftP[i].pos[0], LeftP[i].pos[1], 0);
			glEnd();
		}
	}

	for (int i = 0; i < Boulder.size(); i++)
	{
		if (Boulder[i].active)
		{
			glBegin(GL_LINE_STRIP);
			glVertex3f(Boulder[i].Point[0][0], Boulder[i].Point[0][1], 0);
			glVertex3f(Boulder[i].Point[1][0], Boulder[i].Point[1][1], 0);
			glVertex3f(Boulder[i].Point[2][0], Boulder[i].Point[2][1], 0);
			glVertex3f(Boulder[i].Point[3][0], Boulder[i].Point[3][1], 0);
			glVertex3f(Boulder[i].Point[4][0], Boulder[i].Point[4][1], 0);
			glVertex3f(Boulder[i].Point[5][0], Boulder[i].Point[5][1], 0);
			glVertex3f(Boulder[i].Point[6][0], Boulder[i].Point[6][1], 0);
			glVertex3f(Boulder[i].Point[7][0], Boulder[i].Point[7][1], 0);
			glVertex3f(Boulder[i].Point[0][0], Boulder[i].Point[0][1], 0);
			glEnd();
		}
	}
}

bool equalRound(double a, double b, double round)
{
	bool isEqual;
	isEqual = (a > (b)-round) && (a < (b)+round) ? true : false;
	return isEqual;
}

void debugRenderPoint(double x, double y)
{
	glLineWidth(8);
	glColor3f(1.0, .0, 0);

	glBegin(GL_LINES);
	glVertex3f(x, y, 0);
	glVertex3f(x, y + 0.09, 0);
	glEnd();
}

void BoulderController()
{
	for (int i = 0; i < Boulder.size(); i++)
	{
		for (int j = 0; j <= sizeof(Boulder[i].Point) / sizeof(Boulder[i].Point[0]); j++)
		{
			if (j > 0)
			{
				double slopeR = (Boulder[i].Point[j][1] - Boulder[i].Point[j - 1][1]) / (Boulder[i].Point[j][0] - Boulder[i].Point[j - 1][0]);
				double bR = Boulder[i].Point[j][1] - (slopeR * Boulder[i].Point[j][0]);

				Boulder[i].slope = slopeR;
				Boulder[i].b = bR;

				if (equalRound(Player.pos[1], (slopeR * (Player.pos[0]) + bR), 0.10))
				{
					if (((Player.pos[0] >= Boulder[i].Point[4][0] && Player.pos[0] <= Boulder[i].Point[0][0]) && (Player.pos[1] >= Boulder[i].Point[6][1] && Player.pos[1] <= Boulder[i].Point[2][1])))
					{
						if (Boulder[i].active)
						{
							CollisionController();
						}
						//bullets.erase(bullets.begin() + i);CollisionControllerCollisionController
					}
					/*
					else if (equalRound(Player.pos[0], Boulder[i].Point[4][0], 0.001) && (equalRound(Player.pos[1], Boulder[i].Point[4][1], 0.001)))
					{
						//CollisionController();
					}
					else if (equalRound(Player.pos[1], Boulder[i].Point[6][1], 0.001) && (equalRound(Player.pos[1], Boulder[i].Point[2][1], 0.001)))
					{
						//CollisionController();
					}*/
				}
			}
			Boulder[i].Point[j][1] = Boulder[i].Point[j][1] - (Boulder[i].speed + speedMulti);

			if (Boulder[i].Point[j][1] < -1.3)
			{
				Boulder.erase(Boulder.begin() + i);
			}
		}
	}
}

void instantiateBoulder()
{
	if (Boulder.size() < 10)
	{
		Boulder.push_back(Boulders());
		for (int i = 0; i < Boulder.size(); i++)
		{
			if (!Boulder[i].active)
			{
				Boulder[i].active = true;

				double pos = rand() % 100 - 50;
				double speed = rand() % 100 + 50;

				Boulder[i].speed = speed / 10000;

				for (int j = 0; j <= sizeof(Boulder[i].Point) / sizeof(Boulder[i].Point[0]); j++)
				{
					double xcos = 0.5 * cos((j * 45) *  3.14159265359 / 180);
					double ysin = 0.5 * sin((j * 45) *  3.14159265359 / 180);

					double xDash = xcos - ysin;
					Boulder[i].Point[j][0] = (xDash / 10) + pos / 100;

					double xsin = 0.5 * sin((j * 45) *  3.14159265359 / 180);
					double ycos = 0.5 * cos((j * 45) *  3.14159265359 / 180);

					double yDash = xsin + ycos;
					Boulder[i].Point[j][1] = (yDash / 10) + 1.2;
				}
			}
		}
	}
}

void instantiateBullet()
{
	bullets.push_back(Bullet());
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i].active == NULL | bullets[i].active == false)
		{
			bullets[i].speed = 0.03;

			bullets[i].active = true;

			bullets[i].pos[0] = Player.pos[0];
			bullets[i].pos[1] = Player.pos[1];

			bullets[i].Line[0] = 0.0;
			bullets[i].Line[1] = 0.05;
			break;
		}
	}
}

void bulletController()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i].active == true)
		{
			bullets[i].pos[1] = bullets[i].pos[1] + bullets[i].speed;
		}
		if (bullets[i].pos[1] >= 1)
		{
			//bullets.erase(bullets.begin() + i);
			bullets[i].active = false;
			break;
		}
	}
}


void QuickUpdate()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		for (int f = 0; f < Boulder.size(); f++)
		{
			for (int j = 0; j <= sizeof(Boulder[f].Point) / sizeof(Boulder[f].Point[0]); j++)
			{
				if (j > 0)
				{
					double slopeR = (Boulder[f].Point[j][1] - Boulder[f].Point[j - 1][1]) / (Boulder[f].Point[j][0] - Boulder[f].Point[j - 1][0]);
					double bR = Boulder[f].Point[j][1] - (slopeR * Boulder[f].Point[j][0]);

					if (equalRound(bullets[i].pos[1], (slopeR * (bullets[i].pos[0]) + bR), 0.10))
					{
						//printf("2 %lf  :  6 %lf\n", Boulder[f].Point[2][1], Boulder[f].Point[6][1]);

						if (((bullets[i].pos[0] >= Boulder[f].Point[4][0] && bullets[i].pos[0] <= Boulder[f].Point[0][0]) && (bullets[i].pos[1] >= Boulder[f].Point[6][1] && bullets[i].pos[1] <= Boulder[f].Point[2][1])))
						{
							if (Boulder[f].active)
							{
								Boulder[f].active = false;
								//bullets.erase(bullets.begin() + i);
								bullets[i].active = false;
								break;
							}
						}
						else if (equalRound(bullets[i].pos[0], Boulder[f].Point[4][0], 0.05) && (equalRound(bullets[i].pos[1], Boulder[f].Point[4][1], 0.05)))
						{
							if (Boulder[f].active)
							{
								Boulder[f].active = false;
								bullets[i].active = false;
								break;
							}
						}
						else if (equalRound(bullets[i].pos[1], Boulder[f].Point[6][1], 0.05) && (equalRound(bullets[i].pos[1], Boulder[f].Point[2][1], 0.05)))
						{
							if (Boulder[f].active)
							{
								Boulder[f].active = false;
								bullets[i].active = false;
							}
						}
					}
				}
			}
		}

		for (int j = 0; j < RightP.size(); j++)
		{
			if (equalRound(bullets[i].pos[1], (RightP[j].WallVal.slope * (bullets[i].pos[0]) + RightP[j].WallVal.b), 0.10) && j != 0)
			{
				if ((bullets[i].pos[0] >= RightP[j].pos[0] && bullets[i].pos[0] <= RightP[j - 1].pos[0]) | (bullets[i].pos[0] <= RightP[j].pos[0] && bullets[i].pos[0] >= RightP[j - 1].pos[0]))
				{
					bullets[i].active = false;
					//bullets.erase(bullets.begin() + i);
					break;
				}
			}
			else if (equalRound(bullets[i].pos[1], (LeftP[j].WallVal.slope * (bullets[i].pos[0]) + LeftP[j].WallVal.b), 0.10) && j != 0)
			{
				if ((bullets[i].pos[0] >= LeftP[j].pos[0] && bullets[i].pos[0] <= LeftP[j - 1].pos[0]) | (bullets[i].pos[0] <= LeftP[j].pos[0] && bullets[i].pos[0] >= LeftP[j - 1].pos[0]))
				{
					//bullets.erase(bullets.begin() + i);
					bullets[i].active = false;
					break;
				}
			}
		}


		if (!bullets[i].active)
		{
			bullets.erase(bullets.begin() + i);
			break;
		}

	}
	for (int i = 0; i < RightP.size(); i++)
	{
		if (i != 0)
		{
			double slopeR = (RightP[i].pos[1] - RightP[i - 1].pos[1]) / (RightP[i].pos[0] - RightP[i - 1].pos[0]);
			double bR = RightP[i].pos[1] - (slopeR * RightP[i].pos[0]);
			double y = (Player.pos[1]) + 0;
			double x = Player.pos[0] + 0;

			RightP[i].WallVal.slope = slopeR;
			RightP[i].WallVal.b = bR;


			//debugRenderPoint(x, y);
			double roundA = 0.01;
			//calculate if player hitting any walls
			double addsub = 0.02;
			if (equalRound(y, (slopeR * (x - 0.05) + bR), roundA) | equalRound(y, (slopeR * (x + 0.05) + bR), roundA) | equalRound(y + 0.09, (slopeR * x + bR), roundA))
			{
				if (((Player.pos[1] - addsub >= RightP[i - 1].pos[1]) && (Player.pos[1] + addsub <= RightP[i].pos[1])) | ((Player.pos[1] + addsub <= RightP[i - 1].pos[1]) && (Player.pos[1] - addsub >= RightP[i].pos[1])))
				{
					CollisionController();
				}
			}

			double slopeL = (LeftP[i].pos[1] - LeftP[i - 1].pos[1]) / (LeftP[i].pos[0] - LeftP[i - 1].pos[0]);
			double bL = LeftP[i].pos[1] - (slopeL * LeftP[i].pos[0]);

			LeftP[i].WallVal.slope = slopeL;
			LeftP[i].WallVal.b = bL;

			if (equalRound(y, (slopeL * (x - 0.05) + bL), roundA) | equalRound(y, (slopeL * (x + 0.05) + bL), roundA) | equalRound(y + 0.09, (slopeL * x + bL), roundA))
			{
				if (((Player.pos[1] >= LeftP[i - 1].pos[1]) && (Player.pos[1] <= LeftP[i].pos[1])) | ((Player.pos[1] <= LeftP[i - 1].pos[1]) && (Player.pos[1] >= LeftP[i].pos[1])))
				{
					CollisionController();
				}
			}
		}
	}
}
void newWave()
{

}

void GenWalls(double wallMoveSpeed)
{
	double wallDistY = 0.1;

	if (RightP.size() < 30)
	{
		RightP.push_back(WallPoints());
		LeftP.push_back(WallPoints());
	}

	for (int i = 0; i < RightP.size(); i++)
	{
		RightP[i].pos[1] -= wallMoveSpeed;
		LeftP[i].pos[1] -= wallMoveSpeed;

		if (i != 0)
		{
			if (!RightP[i].active)
			{
				double rnd = rand() % 200;
				rnd = rnd / 1000;

				while (equalRound(RightP[i - 1].pos[0], 0.85 - rnd, 0.01))
				{
					rnd = rand() % 200;
					rnd = rnd / 1000;
				}
				RightP[i].pos[0] = 0.85 - rnd;
			}
			if (!LeftP[i].active)
			{
				double rnd = rand() % 200;
				rnd = rnd / 1000;

				while (equalRound(LeftP[i - 1].pos[0], rnd - 0.85, 0.01))
				{
					rnd = rand() % 200;
					rnd = rnd / 1000;
				}

				LeftP[i].pos[0] = rnd - 0.85;
			}
			RightP[i].pos[1] = RightP[i - 1].pos[1] + wallDistY;
			RightP[i].active = true;

			LeftP[i].pos[1] = LeftP[i - 1].pos[1] + wallDistY;
			LeftP[i].active = true;

			if (RightP[i - 1].pos[1] <= -1 - wallDistY | LeftP[i - 1].pos[1] <= -1 - wallDistY)
			{
				RightP.erase(RightP.begin() + i - 1);
				LeftP.erase(LeftP.begin() + i - 1);
				break;
			}
		}
	}
}

// Called every tick
void Update()
{
#pragma region MovePlayer
	if (Player.move)
	{
		if (Player.move[0])
		{
			Player.pos[0] -= Player.speed;
		}
		if (Player.move[1])
		{
			Player.pos[0] += Player.speed;
		}
		if (Player.move[2])
		{
			if (Player.pos[1] > -1)
				Player.pos[1] -= Player.speed;
		}
		if (Player.move[3])
		{
			if (Player.pos[1] < 1 - control.Triangle[1])
				Player.pos[1] += Player.speed;
		}
	}
#pragma endregion


	BoulderController();
	instantiateBoulder();
	GenWalls(0.01 + speedMulti);
	bulletController();
	speedMulti += 0.000001;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	srand(time(NULL));
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(640, 640, "DelveDestroyer", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	double updateDel = 0.01f;
	double updateTime = updateDel;

	//player size, shape, speed, position init
	Player.Triangle[0] = -0.05;
	Player.Triangle[1] = 0.1;
	Player.Triangle[2] = 0.05;

	control.Triangle[0] = -0.05;
	control.Triangle[1] = 0.1;
	control.Triangle[2] = 0.05;

	Player.speed = 0.01;

	Player.pos[0] = 0;
	Player.pos[1] = -0.7;

	RightP.push_back(WallPoints());
	LeftP.push_back(WallPoints());


	RightP[0].pos[0] = 1;
	RightP[0].pos[1] = -1;
	LeftP[0].pos[0] = -1;
	LeftP[0].pos[1] = -1;

	RightP[0].active = true;
	LeftP[0].active = true;


	while (!glfwWindowShouldClose(window))
	{
		QuickUpdate();

		double lastTime = glfwGetTime();
		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);


		if (lastTime > updateTime)
		{
			Update();
			updateTime = lastTime + updateDel;
		}

		RenderObjects();

		//debugRenderPoint(Player.pos[0], (Player.pos[1]));

		glfwSwapInterval(1);
		Sleep(1);
		glfwSwapBuffers(window);
		Sleep(1);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	char c = key;

#pragma region

	if (key == 263 | key == 65 && action == GLFW_REPEAT | action == GLFW_PRESS)
	{
		Player.move[0] = true;
	}
	else if (key == 263 | key == 65 && action == GLFW_RELEASE)
	{
		Player.move[0] = false;
	}

	if (key == 262 | key == 68 && action == GLFW_REPEAT | action == GLFW_PRESS)
	{
		Player.move[1] = true;
	}
	else if (key == 262 | key == 68 && action == GLFW_RELEASE)
	{
		Player.move[1] = false;
	}

	if (key == 265 | key == 87 && action == GLFW_REPEAT | action == GLFW_PRESS)
	{
		Player.move[3] = true;
	}
	else if (key == 265 | key == 87 && action == GLFW_RELEASE)
	{
		Player.move[3] = false;
	}

	if (key == 264 | key == 83 && action == GLFW_REPEAT | action == GLFW_PRESS)
	{
		Player.move[2] = true;
	}
	else if (key == 264 | key == 83 && action == GLFW_RELEASE)
	{
		Player.move[2] = false;
	}

	if (key == 32 && action == GLFW_PRESS)
	{
		instantiateBullet();
	}
#pragma endregion Movement Input
}
