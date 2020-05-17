#include <iostream>
#include <conio.h>
#include <cmath>
using namespace std;

#define ROOM_X 60
#define ROOM_Y 30
int frame=0;
struct ball {
	double x,y;
	double vx,vy;
	bool is_wall=true;
	void compute() {
		x+=vx;
		y+=vy;
		if (is_wall) {
			if (x>=ROOM_X-1 or x<=0) {
				//x=-x;
				vx=-vx;
			}
			if (y>=ROOM_Y-1 or y<=0) {
				//y=-y;
				vy=-vy;
			}
		}
	}
	void draw() {
		system("clear");
		cout<<"  +";
		for (int tmp1=0;tmp1<ROOM_X;tmp1++) {
			cout<<"=";
		}
		cout<<"+"<<endl;
		
		for (int tmp1=0;tmp1<y;tmp1++) {
			cout<<"  |";
			for (int tmp2=0;tmp2<ROOM_X;tmp2++) {
				cout<<" ";
			}
			cout<<"|"<<endl;
		}
		
		cout<<"  |";
		for (int tmp1=0;tmp1<x;tmp1++) {
			cout<<" ";
		}
		cout<<"@";
		for (int tmp1=x+1;tmp1<ROOM_X;tmp1++) {
			cout<<" ";
		}
		cout<<"|"<<endl;
		
		for (int tmp1=y;tmp1<ROOM_Y;tmp1++) {
			cout<<"  |";
			for (int tmp2=0;tmp2<ROOM_X;tmp2++) {
				cout<<" ";
			}
			cout<<"|"<<endl;
		}
		cout<<"  +";
		for (int tmp1=0;tmp1<ROOM_X;tmp1++) {
			cout<<"=";
		}
		cout<<"+"<<endl;
		frame++;
		cout<<"Frame="<<frame<<endl;
		cout<<"Now Pos("<<x<<", "<<y<<")"<<endl;
		cout<<"Vectot X="<<vx<<", Y="<<vy<<endl;
	}
};
int main() {
	ball b;
	b.x=1,b.y=7;
	b.draw();
	getch();
	b.vx=1,b.vy=1;
	for (double t=0;;t++) {
		b.vy+=0.1;
		b.compute();
		b.draw();
		usleep(1000);
	}
	return 0;
}
