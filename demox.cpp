#include <iostream>
#include <string>
using namespace std;
#include <conio.h>//random function
#include <thread>
#include <Windows.h>//set the pointer & coloring
#include <fstream>//file saving
#include <deque>//lane
#include <cstdlib>//srand
#include <ctime>//srand
bool sound = true;
//Don't read these
void cls()
{
	// Get the Win32 handle representing standard output.
	// This generally only has to be done once, so we make it static.
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };

	// std::cout uses a buffer to batch writes to the underlying console.
	// We need to flush that to the console because we're circumventing
	// std::cout entirely; after we clear the console, we don't want
	// stale buffered text to randomly be written out.
	std::cout.flush();

	// Figure out the current width and height of the console window
	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		// TODO: Handle failure!
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	// Flood-fill the console with spaces to clear it
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

	// Reset the attributes of every character to the default.
	// This clears all background colour formatting, if any.
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

	// Move the cursor back to the top left for the next sequence of writes
	SetConsoleCursorPosition(hOut, topLeft);
}
void GotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void clearScreen(){
	HANDLE hOut;
	COORD Position;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}
void clearBox()
{
	HANDLE hOut;
	COORD Position;
	DWORD Written;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	Position.X = 0;
	Position.Y = 0;
	FillConsoleOutputCharacter(hOut, ' ', 1000000, Position, &Written);

	SetConsoleCursorPosition(hOut, Position);
}
//They are just some special functions


class Player {
	int px = 55;//x - coor
	int py = 4;//y - coor
	int life=3;
public:
	void getHit() {
		life--;
	}
	int health() {
		return life;
	}
	void reset() {
		//reset postion of player
		px = 55;
		py = 4;
	}
	void reset(int l) {
		//reset position and life of player
		px = 55;
		py = 4;
		this->life = l;
	}
	int getX() {
		return px;
	}
	int getY() {
		return py;
	}
	void drawPlayer() {
		cout << "Y";
	}
	void move(int temp) {
		//W
		if (temp == 87) {
			if (py>4)
				this->py--;
		}
		//S
		if (temp == 83) {
			this->py++;
		}
		//A
		if (temp == 65) {
			if(px>43)
			this->px--;
		}
		//D
		if (temp == 68) {
			if(px<67)
			this->px++;
		}
		cls();//Clear screen after moving
	}
};

class Lane {
	deque<unsigned char> lane;
	int length;
	int YLane;
	int XLane;
	unsigned char obj; // store special character
	int stop = 0; //var to stop lane in mili second
	bool is_reverse = false; //reverse direction of the lane
	int delay=0;//adjust the speed of lane
	int count=0;
public:
	Lane(int y,int length,int random) {
		this->length = length;//set length of lane
		this->YLane = y;//set y positon of lane 
		this->XLane = 55 - this->length / 2;//55 middle of the screen
		srand(time(0) + random);//activate rand() to produce true random number
		int type = rand();//generate random number 
		if (type % 2 == 0) is_reverse = true;//the direction of the lane is 50:50
		type = type % 3;// shrink value of type
		if (type == 0)obj = 241;//bat animal
		else if (type == 1)obj = 206;//bull animal
		else obj = 220;//car
		for (int i = 0; i < this->length; i++)lane.push_back(obj);//fill lane
	}
	bool drawLane(Player&man) {
		//draw trafic light first
		GotoXY(this->XLane - 1, this->YLane);
		unsigned char stp = 223;
		if (this->stop != 0) {
			HANDLE h1 = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(h1, 4);
			cout << stp;
			SetConsoleTextAttribute(h1, 15);
		}
		else {
			if (this->obj==220) {
				HANDLE h1 = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(h1, 2);
				cout << stp;
				SetConsoleTextAttribute(h1, 15);
			}
			else cout << "<";
		}
		GotoXY(this->XLane + this->length, this->YLane);
		if (this->stop != 0) {
			HANDLE h2 = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(h2, 4);
			cout << stp;
			SetConsoleTextAttribute(h2, 15);
		}
		else {
			if(this->obj == 220){
				HANDLE h1 = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(h1, 2);
				cout << stp;
				SetConsoleTextAttribute(h1, 15);
			}
			else cout << ">";
		}
		//end of : draw traffic light

		//point to where to start the lane
		GotoXY(this->XLane, this->YLane);
		for (int i = 0; i < this->length; i++) {
			//if hit the player
			if (this->XLane + i == man.getX() && this->YLane == man.getY()) {
				if (this->lane[i] == obj) {
					unsigned char head = 254;
					unsigned char body = 197;
					unsigned char leg = 195;
					unsigned char revleg = 180;
					if(sound) cout << "\a";//make sound
					//print dead body
					HANDLE h1 = GetStdHandle(STD_OUTPUT_HANDLE);
					SetConsoleTextAttribute(h1, 4);
					if(!this->is_reverse)
					cout << leg<<body<<head;
					else cout << head << body << revleg;
					SetConsoleTextAttribute(h1, 15);
					Sleep(500);
					//revive dead body
					man.reset();
					man.getHit();
					return false; //signal that the player has been hit
				}
				else man.drawPlayer();
			}
			//else just print lane
			else cout << lane[i];
		}
		return true;
	}
	void moveLane() {
		int sec = rand() % 100;
		if (sec == 50 && this->stop <= 0) {
			if(obj == 220)
			this->stop = sec;//stop lane for 50ms
		}
		if (stop<=0) {
			//delay
			if (count >= delay) {
				//if lane is not stopped
				int r = rand() % 10;
				unsigned char emt = 95;//empty character
				if (!is_reverse)lane.pop_back();
				else lane.pop_front();//for lane with reverse order
				if (r == 5) {
					//5 : signal to add object to the lane
					if (!is_reverse)
						lane.push_front(obj);
					else lane.push_back(obj);//for lane with reverse order
				}
				else {
					if (!is_reverse)lane.push_front(emt);
					else lane.push_back(emt);//for lane with reverse order
				}
				count = 0;
			}
			else count++;
		}
		else stop--;//reduce the stop time every step
	}
	void freeze(int n) {
		this->stop = n;
	}
	unsigned char is() {
		//specify which kind of lane is this
		return obj;
	}
	void slow() {
		delay ++;
	}
	void fast() {
		delay --;
		if (delay < 0)delay = 0;
	}
};
class Map {
	deque<Lane> map;//store all the lane
	int level = 1;//store level
	int height;//height of map
	int trial;//health of the player
public:
	void slowDown() {
		for (int i = 0; i < map.size(); i++) map[i].slow();
	}
	void faster() {
		for (int i = 0; i < map.size(); i++) map[i].fast();
	}
	int getLv() {
		return level;
	}
	Map(int level) {
		if (level < 1 || level > 10) level = 1;//level must be between 1-10
		this->level = level;
		int y=5;
		for (int i = 0; i < level + 3; i++) {
			Lane tmp(y++, 25,i);//increase the height of map
			map.push_back(tmp);
		}
		this->height = y;//set height of map
	}

	bool drawMap(Player&x) {
		this->trial = x.health();
		GotoXY(x.getX(), x.getY());
		x.drawPlayer();
		bool status = true;
		for (int i = 0; i < map.size(); i++)
			if (!map[i].drawLane(x)) {
				return false; // signal that the player has been hit
		}
		//if player reach finish line
		if (x.getY() == this->height) {
			system("cls");
			GotoXY(50, 3);
			cout << "YOU WIN THIS LEVEL!";
			Sleep(1000);
			x.reset();
			reset(++this->level);
		}
		return true;
	}
	void moveMap() {
		//Sleep(100);
		for (int i = 0; i < map.size(); i++) {
			map[i].moveLane();
			if (map[i].is() == 241)map[i].moveLane();//bull go twice normal speed
		}
	}
	void reset(int level) {
		//reset map
		cout << "RELOADING...";
		while (!map.empty()) map.pop_back();
		this->level = level;
		int y = 5;
		for (int i = 0; i < level+3; i++) {
			Lane tmp(y++, 35,i);
			map.push_back(tmp);
		}
		this->height = y;
		cout << "DONE";
		system("cls");
	}
	void save(string path) {
		ofstream fout(path,ios::binary);
		fout << this->level<<" "<< this->trial;
		fout.close();
	}
	void reload(string path,Player&x) {
		ifstream fin(path,ios::binary);
		if (!fin.is_open()) {
			cout << "ERROR : CANNOT FIND SAVE FILE";
			Sleep(1000);
			return;
		}
		int lv=1;
		int l;
		fin >> lv >> l;
		reset(lv);
		x.reset(l);
		fin.close();
	}
	void freeze() {
		for (int i = 0; i < map.size(); i++)map[i].freeze(200);//Sleep(200)
	}
};

Map game(1);//level 1 game
Player man;
bool rep = true;//if player want to replay
bool RUN = true;

void background() {
	while (rep) {
		while (man.health() > 0 && rep) {
			while (RUN && man.health() > 0 && rep) {
				if (game.drawMap(man)) {
					GotoXY(0, 0);
					cout << "---------------------------------LIFE: " << man.health() << "-------------------------------------------\n";
					cout << "LEVEL: " << game.getLv();
					game.moveMap();
				}
			}
		}
	}
}
int main() {
	//MENU
	int choice=0;
	while (choice != 1 && choice != 2&&choice !=3&&choice!=4) {
		cout << "WELCOME\n";
		cout << "CS : Project : CROSSING THE ROAD\n";
		cout << "Ho Cong Duy - 1751060\n";
		cout << "1.Start game\n";
		cout << "2.Load game\n";
		cout << "3.Setting\n";
		cout << "4.Exit\n";
		cout << "Input: "; cin >> choice; cin.get();
		system("cls");
	}
	if (choice == 3) {
		int dif;
		int s;
		cout << "Input level : "; cin >> dif; cin.get();
		cout << "Sound?(0 - no, other - yes) :"; cin >> s; cin.get();
		if (s == 0) sound = false;
		game.reset(dif);
	}
	if (choice == 2) {
		string s;
		system("cls");
		cout << "Please input save file name: "; getline(cin, s);
		game.reload(s, man);
		man.reset();
	}
	if (choice == 4)return 0;
	//END_MENU
	system("cls");
	//GAME
	thread t(background);
	while (rep) {
		int temp; 
		temp = toupper(_getch());
		man.move(temp);
		if (temp == 32) {
			//SPACE TO FREEZE
			game.freeze();
		}
		if (temp == 27) {
			//ESC
			t.detach();
			return 0;
		}
		if (temp == 84) {
			//T
			RUN = false;
			string s;
			GotoXY(0, 0);
			system("cls");
			cout << "Please input path+name to save: "; getline(cin, s);
			game.save(s);
			man.reset();
			RUN = true;
		}
		if (temp == 76) {
			//L
			RUN = false;
			string s;
			GotoXY(0, 0);
			system("cls");
			cout << "Please input save file name: "; getline(cin, s);
			game.reload(s,man);
			man.reset();
			RUN = true;
		}
		if (temp == 88) {
			game.slowDown();
		}
		if (temp == 90) {
			game.faster();
		}
		if (man.health() <= 0) {
			//Lost all Life
			RUN = false;
			system("cls");
			GotoXY(0, 0);
			cout << "YOU LOSE\n";
			Sleep(400);
			int trial;
			cout << "Replay?(0-no, other-yes) : "; cin >> trial; cin.get();
			if (trial == 0) rep = false;
			game.reset(1);
			man.reset(3);
			RUN = true;
		}
	}
	RUN = false;
	t.detach();
	//EXIT GAME
	return 0;
}