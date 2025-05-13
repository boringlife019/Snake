#include<iostream>
#include<easyx.h>
#include<ctime>
#include<vector>
#include<Windows.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include"ps.h"


/*精灵类*/
class Sprite
{
protected:
	int m_x;
	int m_y;
	COLORREF m_color;
public:
	Sprite() :Sprite(0, 0, WHITE) {};
	Sprite(int x, int y, COLORREF mycolor = WHITE) :m_x(x), m_y(y), m_color(mycolor) {};
	virtual void draw();
	void moveBy(int x, int y);
	bool collision(const Sprite& other);
	void changeFoodPos();
	int getX() const { return m_x; }
	int getY() const { return m_y; }
};

/*蛇类*/
class Snake :public Sprite
{
private:
	std::vector<Sprite> nodes;

public:
	int dir;
	Snake() :Snake(0, 0) {}
	Snake(int x, int y) :Sprite(x, y), dir(VK_RIGHT)
	{
		nodes.push_back(Sprite(30, 10));
		nodes.push_back(Sprite(20, 10));
		nodes.push_back(Sprite(10, 10));
	}
	void draw() override;
	void bodyMove();
	bool collision(const Sprite& other);
	bool collisionWall();
	bool collisionSelf();
	void increment();
};
/*食物类*/
class Food :public Sprite
{
public:
	Food() :Sprite(0, 0, YELLOW)
	{
		changeFoodPos();
	}
	void draw()override;

};

/*墙类*/
class Wall :public Sprite
{
public:
	Wall() :Sprite(0, 0, LIGHTGRAY) {}
	void setWall();
};

/*游戏场景类*/
class GameSence
{
private:
	Snake snake;
	Food food;
	Wall wall;
public:
	void initAudio();
	void playEatSound();
	void run();
	void onMsg(const ExMessage& msg);
	void snakeEatFood();
	void snakeStrikeWall();
	void snakeHeadStrikeBody();
	void showStartScreen();
};
void Sprite::draw() {
	setfillcolor(m_color);
	fillrectangle(m_x, m_y, m_x + 10, m_y + 10);
};

void Sprite::moveBy(int x, int y)
{
	m_x += x;
	m_y += y;
}
bool Sprite::collision(const Sprite& other)
{
	return m_x == other.m_x && m_y == other.m_y;
}
void Sprite::changeFoodPos()
{
	int tem_x;
	int tem_y;
	int wallData[64][48] = { 0 };
	tem_x = rand() % 64 * 10;
	tem_y = rand() % 48 * 10;
	for (int i = 0; i < 64; ++i) {
		wallData[i][0] = 1;
	}

	for (int i = 0; i < 64; ++i) {
		wallData[i][48 - 1] = 1;
	}

	for (int i = 0; i < 48; ++i) {
		wallData[0][i] = 1;
	}

	for (int i = 0; i < 48; ++i) {
		wallData[64 - 1][i] = 1;
	}

	while (wallData[tem_x / 10][tem_y / 10] == 1)
	{
		tem_x = rand() % 64 * 10;
		tem_y = rand() % 48 * 10;
	}
	m_x = tem_x;
	m_y = tem_y;
}


/*Snake method*/
void Snake::draw()
{
	for (int i = 1; i < nodes.size(); i++)
	{
		nodes[i].draw();
	}
	IMAGE img_dz1;
	loadimage(&img_dz1, "wtf/dz1.png", 30, 30);
	const Sprite& head = nodes[0];
	putimage_alpha(head.getX() - 10, head.getY() - 10, 30, 30, &img_dz1);
}

void Snake::bodyMove()
{
	for (size_t i = nodes.size() - 1; i > 0; i--)
	{
		nodes[i] = nodes[i - 1];
	}

	switch (dir)
	{
	case VK_UP:
		nodes[0].moveBy(0, -10);
		break;
	case VK_DOWN:
		nodes[0].moveBy(0, 10);
		break;
	case VK_LEFT:
		nodes[0].moveBy(-10, 0);
		break;
	case VK_RIGHT:
		nodes[0].moveBy(10, 0);
		break;

	}
}
bool Snake::collision(const Sprite& other)
{
	return nodes[0].collision(other);
}

bool Snake::collisionWall()
{
	int x;
	int y;
	int flag = 0;
	const Sprite& head = nodes[0];


	y = 0;
	for (int i = 0; i < 64; i++)
	{
		x = i * 10;
		if (head.getX() == x && head.getY() == y)
		{
			flag = 1;
		}
		;
	}
	y = 470;
	for (int i = 0; i < 64; i++)
	{
		x = i * 10;
		if (head.getX() == x && head.getY() == y)
		{
			flag = 1;
		}
	}
	x = 0;
	for (int i = 0; i < 48; i++)
	{
		y = i * 10;
		if (head.getX() == x && head.getY() == y)
		{
			flag = 1;
		}
	}
	x = 630;
	for (int i = 0; i < 48; i++)
	{
		y = i * 10;
		if (head.getX() == x && head.getY() == y)
		{
			flag = 1;
		}
	}
	return flag == 1;
}

void Snake::increment()
{
	nodes.push_back(Sprite());
}
bool Snake::collisionSelf()
{
	int flag = 0;
	for (int i = 1; i < nodes.size(); i++)
	{
		if (nodes[0].collision(nodes[i]))
		{
			flag = 1;
		}
	}
	return flag == 1;
}
/*wall method*/

void Wall::setWall()
{
	setfillcolor(m_color);

	m_y = 0;
	for (int i = 0; i < 64; i++)
	{
		m_x = i * 10;
		fillrectangle(m_x, m_y, m_x + 10, m_y + 10);
	}
	m_y = 470;
	for (int i = 0; i < 64; i++)
	{
		m_x = i * 10;
		fillrectangle(m_x, m_y, m_x + 10, m_y + 10);
	}
	m_x = 0;
	for (int i = 0; i < 48; i++)
	{
		m_y = i * 10;
		fillrectangle(m_x, m_y, m_x + 10, m_y + 10);
	}
	m_x = 630;
	for (int i = 0; i < 48; i++)
	{
		m_y = i * 10;
		fillrectangle(m_x, m_y, m_x + 10, m_y + 10);
	}



}


/*GameSence method*/
void GameSence::onMsg(const ExMessage& msg)
{
	if (msg.message == WM_KEYDOWN)
	{
		switch (msg.vkcode)
		{
		case VK_UP:
			if (snake.dir != VK_DOWN)
				snake.dir = msg.vkcode;
			break;
		case VK_DOWN:
			if (snake.dir != VK_UP)
				snake.dir = msg.vkcode;
			break;
		case VK_RIGHT:
			if (snake.dir != VK_LEFT)
				snake.dir = msg.vkcode;
			break;
		case VK_LEFT:
			if (snake.dir != VK_RIGHT)
				snake.dir = msg.vkcode;
			break;
		}

	}
}
void GameSence::run()
{
	BeginBatchDraw();
	cleardevice();
	wall.setWall();
	snake.draw();
	food.draw();
	EndBatchDraw();
	snake.bodyMove();
	snakeEatFood();
	snakeStrikeWall();
	snakeHeadStrikeBody();


	ExMessage msg = { 0 };
	while (peekmessage(&msg, EX_KEY))
	{
		onMsg(msg);
	}


}
void GameSence::snakeEatFood()
{
	if (snake.collision(food))
	{
		snake.increment();
		food.changeFoodPos();
		playEatSound();
	}
}
void GameSence::snakeStrikeWall()
{
	if (snake.collisionWall())
	{
		std::cout << "Game over";
		exit(0);
	}

}
void GameSence::snakeHeadStrikeBody()
{
	if (snake.collisionSelf())
	{
		std::cout << "Game over";
		exit(1);
	}
}


void GameSence::showStartScreen() {
	IMAGE img_dz;
	loadimage(&img_dz, "wtf/dz.jpg", 640, 480);
	cleardevice();
	putimage(0, 0, &img_dz);

	settextcolor(WHITE);

	settextstyle(40, 30, "微软");
	setbkmode(TRANSPARENT);
	outtextxy(42, 350, _T("请按回车键开始游戏"));

	settextstyle(20, 15, "微软");
	setbkmode(TRANSPARENT);
	outtextxy(0, 425, _T("按ESC退出游戏"));
}

void GameSence::initAudio() {
	mciSendString("open wtf/wu.WAV", NULL, 0, NULL);
}

void GameSence::playEatSound() {
	mciSendString("stop wtf/wu.WAV", NULL, 0, NULL);
	mciSendString("seek wtf/wu.WAV to start", NULL, 0, NULL);
	mciSendString("play wtf/wu.WAV", NULL, 0, NULL);
}



/*Food methods*/
void Food::draw()
{
	IMAGE img_rk;
	loadimage(&img_rk, "wtf/rk.jpg", 10, 10);
	putimage(m_x, m_y, &img_rk);
}



int main()
{
	initgraph(640, 480);
	srand((unsigned)time(NULL));
	GameSence scene;
	scene.initAudio();
	scene.showStartScreen();
	ExMessage msg1;
	while (true) {
		if (peekmessage(&msg1, EX_KEY)) {
			if (msg1.message == WM_KEYDOWN) {
				if (msg1.vkcode == VK_RETURN) {
					break;
				}
				else if (msg1.vkcode == VK_ESCAPE) {
					exit(0);
				}
			}
		}
	}

	while (1)
	{
		scene.run();
		Sleep(100);
	}

	return 0;
}