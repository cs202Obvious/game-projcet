#ifndef FUNC
#define FUNC
#include <iostream>
using namespace std;
#include <Windows.h>
#include <string>
#include <thread>
#include <conio.h>
#include <cstdlib>
#include <vector>
#include <deque>
#include <wchar.h>
void clearScreen();
void clearBox();
//#include <curses.h>
void GotoXY(int x, int y);
void exitGame(thread*t);
//All classes
class Character {
protected:
	int xlim = 100;
	int ylim = 100;
public:
	int pX;
	int pY;
	void setLim(int x, int y);
	//void setLim(int x);
};
class Player : public Character {
	int heart;
public:
	Player(int x, int y);//What is Map???
	void reset() {
		pX = 55;
		pY = 4;
	}
	void move();
	void draw();
	int life();
	void getHit();
};
class Lane {
	deque<bool> obj;
public:
	Lane(int x, int y, int length);
	int length;
	int x;
	int y;
	bool Draw(Player&x);
	void runLane();
};
class Map {
	int x;
	int L;
	int H;
	vector<Lane> m;
	int lv = 0;
	int Lane_num;
public:
	int getL();
	int getH();
	Map(int num, int length);
	bool makeMap(Player& x);//What is Player??
	void runMap();
	void increaseLevel();
	void printlv();
};
/*class Monster : public Character {
public:
	Monster(int x, int y);
	void move(int speed);
	void Draw();
};*/
#endif
