#include "Functional.h"
bool IS_RUNNING = true;
Map game(3, 18);//5 : So lane, 20 : Chieu dai lane
Player m(game.getL(),game.getH());
void ThreadFunc1() {
	while (IS_RUNNING) {
		m.move();
		//system("cls");
	}
}
int main() {
	thread t1(ThreadFunc1);
	while (IS_RUNNING) {
		GotoXY(0, 0);
		cout <<"..................................................LIFE LEFT: "<<m.life()<<" .......................................\n";
		cout << "LEVEL : "; game.printlv();
		if (!game.makeMap(m)) {
			system("cls");
			cout << "GAME OVER!\n";
			Sleep(1000);
			IS_RUNNING = false;
			exitGame(&t1);
			return 0;
		}
	}
	int tmp; cin >> tmp;
	return 0;
}