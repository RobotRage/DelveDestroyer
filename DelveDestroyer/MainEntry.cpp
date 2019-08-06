#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream> 
#include <array>
#include <time.h>  
#include <ctype.h>
#include <Windows.h>
#include <vector>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

bool shoot = false;
bool shootPossible = false;

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

void RenderObjects()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLineWidth(4);
	glColor3f(.0, 1.0, 0);

	glBegin(GL_TRIANGLES);
	glVertex3f(Player.Triangle[0] + Player.pos[0], 0 + Player.pos[1], 0);
	glVertex3f(0 + Player.pos[0], Player.Triangle[1] + Player.pos[1], 0);
	glVertex3f(Player.Triangle[2] + Player.pos[0], 0 + Player.pos[1], 0);
	glEnd();

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
		if (RightP[i].active && i >0)
		{

			glBegin(GL_LINES);
			glVertex3f(RightP[i-1].pos[0], RightP[i-1].pos[1], 0);
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
			bullets.erase(bullets.begin() + i);
			break;
		}
	}
}

void GenWalls(double wallMoveSpeed)
{

	double wallDistY = 0.2;

	if (RightP.size() < 12)
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
				double rnd = rand() % 300;
				rnd = rnd / 1000;
				RightP[i].pos[0] = rnd + 0.5;	 
			}
			if (!LeftP[i].active)
			{
				double rnd = rand() % 300;
				rnd = rnd / 1000;
				LeftP[i].pos[0] = rnd - 0.5;
			}
			RightP[i].pos[1] = RightP[i - 1].pos[1] + wallDistY;
			RightP[i].active = true;

			LeftP[i].pos[1] = LeftP[i - 1].pos[1] + wallDistY;
			LeftP[i].active = true;

			if (RightP[i - 1].pos[1] <= - 1 - wallDistY | LeftP[i - 1].pos[1] <= -1 - wallDistY)
			{
				RightP.erase(RightP.begin() + i -1 );
				LeftP.erase(LeftP.begin() + i -1 );            
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
			if(Player.pos[1] > -1)
				Player.pos[1] -= Player.speed;
		}
		if (Player.move[3])
		{
			if(Player.pos[1] < 1 - control.Triangle[1])
				Player.pos[1] += Player.speed;
		}
	}
#pragma endregion
	GenWalls(0.025);
	bulletController();
}

int main(void)
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

	RightP[0].pos[0] = 0.95;
	RightP[0].pos[1] = -1;
	LeftP[0].pos[0] = -0.95;
	LeftP[0].pos[1] = -1;

	RightP[0].active = true;
	LeftP[0].active = true;

	while (!glfwWindowShouldClose(window))
	{
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
	else if (key == 32 && action == GLFW_RELEASE)
	{
	}

#pragma endregion Movement Input

}
