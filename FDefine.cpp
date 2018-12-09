#include "Functional.h"
#include "Highlight.h"
bool status = true;
void GotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void clearBox()
{
	HANDLE hOut;
	COORD Position;
	DWORD Written;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	Position.X = 0;
	Position.Y = 20;
	FillConsoleOutputCharacter(hOut, ' ', 1000000, Position, &Written);

	SetConsoleCursorPosition(hOut, Position);
}
void exitGame(thread*t) {
	cout << "EXIT GAME";
	t->detach();
}
void clearScreen()
{
	HANDLE hOut;
	COORD Position;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}
Lane::Lane(int x, int y, int length) {
	if (length % 2 != 0)length += 1;
	this->x = x;
	this->y = y;
	this->length = length;
	for (int i = 0; i < length; i++) obj.push_back(true);
}
bool Lane::Draw(Player&x) {
	unsigned char l(219);
	unsigned char lc(200);
	unsigned char rc(188);
	unsigned char r(205);
	GotoXY(this->x, this->y);
	cout << l;
	for (int i = 0; i < this->length; i++) {
		if ((this->x + i == x.pX) && (x.pY == this->y)) {
			if (this->obj[i]) {
				string s(">> ");
				color_print(s);
				Sleep(600);
				return false;
			}
			else cout << "X";
		} else {
			if (this->obj[i])cout << "#";
			else cout << " ";
		}
	}
	cout << l;
	GotoXY(this->x, this->y + 1);
	cout << lc;
	for (int i = 0; i < length; i++)if ((this->x+i == x.pX) && (x.pY == this->y+1))cout << "X";else cout << r;
	cout << rc;
	return true;
}
void Lane::runLane() {
	//system("cls");
	this->obj.pop_back();
	int num = rand() % 10;
	if (num  == 3 || num == 4 || num == 5) {
		this->obj.push_front(true);
	}
	else this->obj.push_front(false);
}

Map::Map(int num, int length) {
	if (num <= 0)num = 1;
	if (length <= 0)length = 3;
	this->Lane_num = num;
	this->L = length;
	int xcor = 55 - length / 2;
	int ycor = 5;
	this->x = xcor;
	for (int i = 0; i < num; i++) {
		Lane x(xcor, ycor, length);
		m.push_back(x);
		ycor += 2;
	}
	H = ycor;
}
bool Map::makeMap(Player& x) {
	clearScreen();
	unsigned char wall(219);
	unsigned char roof(220);
	unsigned char finish(177);
	GotoXY(55 - L / 2, 3);
	for (int i = 0; i < L + 2; i++)cout << roof;
	GotoXY(55 - L / 2, 4);
	cout << wall; for (int i = 0; i < L; i++)if (this->x+i == x.pX&&x.pY == 4)cout << "X"; else cout << " "; cout << wall;
	for (unsigned int i = 0; i < m.size(); i++) {
		if (!m[i].Draw(x)) {
			if(x.life()<=0) return false;
			else {
				x.reset();
				x.getHit();
			}
		}
		m[i].runLane();
	}
	GotoXY(55 - L / 2, H);
	for (int i = 0; i < L + 2; i++)if (this->x - 1 + i == x.pX&&x.pY == this->H) {
		system("cls");
		cout << "You win this round!";
		Sleep(1000);
		increaseLevel();
		x.reset();
	} 
	else cout << finish;
	return true;
}
void Map::runMap() {
	while (1) {
		//makeMap();
		//for (unsigned int i = 0; i < m.size(); i++) m[i].runLane();
	}
}
int Map::getL() {
	return L;
}
int Map::getH() {
	return H;
}
void Map::increaseLevel() {
	for (int i = 0; i < this->Lane_num; i++) {
		this->m.pop_back();
	}
	lv++;
	this->Lane_num++;
	if (this->lv > 3) this->L += 4;
	//Map::Map(Lane_num, L);
	int xcor = 55 - this->L / 2;
	int ycor = 5;
	this->x = xcor;
	for (int i = 0; i < this->Lane_num; i++) {
		Lane x(xcor, ycor, this->L);
		m.push_back(x);
		ycor += 2; 
	}
	H = ycor;
}

void Character::setLim(int x, int y) {
	xlim = x;
	ylim = y;
}

void Player::move() {
		int temp = toupper(_getch());
		//W
		if (temp == 87) {
			if (pY>4)
				this->pY--;
		}
		//S
		if (temp == 83) {
				this->pY++;
		}
		//A
		if (temp == 65) {
			if (pX>55 - xlim / 2)
				this->pX--;
		}
		//D
		if (temp == 68) {
			if (pX<54 + xlim / 2)
				this->pX++;
		}
		//clearScreen();
}
Player::Player(int x, int y) {
	heart = 3;
	pX = 55;
	pY = 4;
	setLim(x, y);
}
void Player::draw() {
	//GotoXY(pX, pY);
	cout << "X";
}
int Player::life() {
	return heart;
}
void Player::getHit() {
	heart--;
}
void Map::printlv() {
	cout << this->lv;
}