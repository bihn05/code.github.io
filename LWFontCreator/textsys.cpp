#include <iostream>
#include <fstream>
#include <graphics.h>

using namespace std;
using namespace ege;

struct FONT {
	unsigned long index;
	unsigned short pxdata[16];
	unsigned char pos;
	unsigned char end;
	unsigned char wid;
	unsigned char nothing[9];
};
FONT fnt;
string filename;
ifstream infile;
void load(unsigned short ch) {
	infile.open(filename.c_str(),ios_base::in | ios_base::binary);
	infile.seekg((ch)*48);
	infile.read((char*)&fnt,48);
	infile.close();
	infile.clear();
}
void font_display(int px,int py,int color) {
	for (int y=0;y<16;y++) {
		for (int x=0;x<16;x++) {
			if ((fnt.pxdata[y]>>(15-x))%2 == 1) {
				putpixel(px+x,py+y,color);
			}
		}
	}
}
char hex_[17]="0123456789ABCDEF";
void outh(unsigned char ch) {
	cout<<hex_[ch/0x10%0x10];
	cout<<hex_[ch%0x10];
}
void outs(unsigned short sh) {
	outh(sh/0x100%0x100);
	outh(sh%0x100);
}
void cprint(int x,int y,const char* str,int f_color) {
	int p=(int)str;
	unsigned char tmp;
	unsigned short tmp1;
	int t=x,u=y;
	unsigned long chr;
	while (*(char*)p!=0) {
		tmp=*(char*)p;
		outh(tmp);
		cout<<' ';
		if (tmp == 0x0a) {
			t=x;
			u+=16;
			p+=1;
			continue;
		}
		if (tmp>>7 ==0) {
			cout<<'.';
			load(*(char*)p);
			t-=fnt.pos;
			font_display(t,u,f_color);
			t+=fnt.wid;
			p+=1;
		}
		if ((tmp & 0xf8) == 0xf0) {
			;
		}
		if ((tmp & 0xf0) == 0xe0) {
			cout<<"3Bytes ";
			chr=0;
			chr|=(tmp%0x10)<<12;
			p+=1;
			tmp=*(char*)p;
			tmp1=(tmp%0x40)<<6;
			chr|=tmp1;
			p+=1;
			tmp=*(char*)p;
			tmp1=(tmp%0x40);
			chr|=tmp1;
			p+=1;
			load(chr);
			t-=fnt.pos;
			font_display(t,u,f_color);
			t+=fnt.wid;
		}
		if ((tmp & 0xe0) == 0xc0) {
			;
		}
	}
	cout<<endl;
}
char buf[65536];
int main() {
	filename="LW.LWFT";
	setinitmode(2);
	initgraph(1024,640);
	ifstream txt;
	a:
	txt.open("test.txt");
	txt.read(buf,65536);
	cleardevice();
	cprint(0,0,buf,0xffffff);
	getch();
	txt.seekg(0);
	txt.clear();
	txt.close();
	memset(buf,0,1024);
	goto a;
}
