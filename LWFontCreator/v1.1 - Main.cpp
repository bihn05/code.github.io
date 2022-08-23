#include <graphics.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <time.h>
#define KEY_DN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) 

using namespace std;
using namespace ege;
bool kbblk=0;
bool KEY_DOWN(WORD KEY_VALUE) {
	if (kbblk==0) {
		return KEY_DN(KEY_VALUE);
	} else return 0;
}
clock_t start,end;
bool lbt_tmp1,lbt_tmp2; 
void lbt_update() {
	lbt_tmp2=lbt_tmp1;
	lbt_tmp1=KEY_DOWN(VK_LBUTTON);
}
bool is_lbt_down() {
	return (lbt_tmp1==1 && lbt_tmp2==0);
}
bool is_lbt_rlse() {
	return (lbt_tmp1==0 && lbt_tmp2==1);
}
bool rbt_tmp1,rbt_tmp2; 
void rbt_update() {
	rbt_tmp2=rbt_tmp1;
	rbt_tmp1=KEY_DOWN(VK_RBUTTON);
}
bool is_rbt_down() {
	return (rbt_tmp1==1 && rbt_tmp2==0);
}
bool is_rbt_rlse() {
	return (rbt_tmp1==0 && rbt_tmp2==1);
}
char hex_[17]="0123456789ABCDEF";
char tmp[5];
const char* hex2str(unsigned char ch) {
	tmp[0]=hex_[ch/0x10%0x10];
	tmp[1]=hex_[ch%0x10];
	tmp[2]=0;
	return tmp;
}
string dw2str(unsigned short sh) {
	string str1=hex2str(sh/0x100%0x100);
	string str2=hex2str(sh%0x100);
	return str1+str2;
}
char ru[4];
string UtfToGbk(const char* utf8) {
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) delete[] wstr;
	return str;
}
string uni(short u) {
	string rt;
	if (u<=0x100 && u>=0x00) {
		rt=(char)u;
		return rt;
	}
	ru[0]=0xE0+u/0x100/0x10;
	ru[1]=0x80+u/0x100%0x10*0x4+u%0x100/0x10/0x4;
	ru[2]=0x80+u%0x100/0x10%0x4*0x10+u%0x100%0x10;
	ru[3]=0;
	return UtfToGbk(ru);
}
struct FONT {
	unsigned long index;
	unsigned short pxdata[16];
	unsigned char pos;
	unsigned char end;
	unsigned char wid;
	unsigned char nothing[9];
};
FONT font_buffer[256];
FONT sdfnt; 
unsigned short temp_index=0x0041;
unsigned short sd=0x0000;
bool shade;
POINT msg={0,0};
void draw_ui() {
	setcolor(0xffffff);
	setfont(16,0,"MingLiU");
	xyprintf(480,464,"LW字体编辑器 v1.1");
	xyprintf(0,0,"当前字符:%s",uni(temp_index).c_str());
	xyprintf(0,16,"当前编辑码区:$%s00~%sFF",hex2str(temp_index/0x100%0x100),
	hex2str(temp_index/0x100%0x100)
	);
	xyprintf(0,32,"指针位置(%d,%d)",msg.x,msg.y);
	for (int y=0;y<17;y++) {
		line(32,64+20*y,353,64+20*y);
		line(32+20*y,64,32+20*y,385);
	}
}
void draw_yulan(FONT fnt,unsigned int color) {
	for (int y=0;y<16;y++) {
		//cout<<hex2str(fnt.pxdata[y]);
		for (int x=0;x<16;x++) {
			if ((fnt.pxdata[y]>>(15-x))%2 == 1) {
				setfillcolor(color);
				
				bar(32+20*x,64+20*y,
				32+20*x+20,64+20*y+20
				);
			}
		}
	}
}
bool inside(int x,int y,int x1,int y1,int x2,int y2) {
	if (x<=x2 && x>=x1 && y<=y2 && y>=y1)return 1; else return 0;
}
void font_display(int px,int py,FONT fnt) {
	for (int y=0;y<16;y++) {
		for (int x=0;x<16;x++) {
			if ((fnt.pxdata[y]>>(15-x))%2 == 1) {
				putpixel(px+x,py+y,0xffffff);
			} else {
				putpixel(px+x,py+y,0x000000);
			}
		}
	}
}
int posx,posy;
ifstream infile;
ofstream otfile;
string filename;
void load() {
	infile.open(filename.c_str(),ios_base::in | ios_base::binary);
	infile.seekg((temp_index/0x100%0x100*0x100)*48);
	infile.read((char*)font_buffer,48*256);
	infile.close();
	infile.clear();
	cout<<"load. "<<(clock()-start)<<"ms"<<endl;
}
long int tim;
const char fn1[3]="LW";
const char fn2[6]=".LWFT";
char cmd1[24]="copy LW.LWFT ";
tm *gmt=NULL;
char postfilename[20];
void save() {
	time(&tim);
	gmt=localtime(&tim);
	strcpy(postfilename,fn1);
	strcpy(postfilename+14,fn2);
	postfilename[2]=0x30+gmt->tm_year/10%10;
	postfilename[3]=0x30+gmt->tm_year%10;
	postfilename[4]=0x30+(gmt->tm_mon+1)/10%10;
	postfilename[5]=0x30+(gmt->tm_mon+1)%10;
	postfilename[6]=0x30+(gmt->tm_mday)/10%10;
	postfilename[7]=0x30+(gmt->tm_mday)%10;
	postfilename[8]=0x30+gmt->tm_hour/10%10;
	postfilename[9]=0x30+gmt->tm_hour%10;
	postfilename[10]=0x30+gmt->tm_min/10%10;
	postfilename[11]=0x30+gmt->tm_min%10;
	postfilename[12]=0x30+gmt->tm_sec/10%10;
	postfilename[13]=0x30+gmt->tm_sec%10;
	strcpy(cmd1+13, postfilename);
	system(cmd1);
	otfile.open(filename.c_str(),ios_base::out | ios::in | ios_base::binary);
	otfile.seekp((temp_index/0x100%0x100*0x100)*48);
	otfile.write((char*)font_buffer,48*256);
	otfile.close();
	otfile.clear();
	cout<<"save. "<<(clock()-start)<<"ms"<<endl;
}
HWND hf;
HWND hw;
//HWND h=GetForegroundWindow();
HWND h=getHWnd();
int iid1,iid2;
int main() {
	//cout<<sizeof(FONT);
	start=clock();
	filename="LW.LWFT";
	setinitmode(2);
	initgraph(640,480);
	draw_ui();
	load();
	FONT* fp;
	int i,j;
	int timer=0;
	for (;is_run();delay_fps(60)) {
		
		GetCursorPos(&msg);
		ScreenToClient(getHWnd(),&msg);
		kbblk=0;
		if (getHWnd() != GetForegroundWindow()) {
			msg={-1,-1};
			kbblk=1;
		}
		
		timer+=1;
		if (timer >= 10) {
			cleardevice();
			timer=0;
		}
		setcolor(0xffffff);
		xyprintf(200,0,"翻页[-][-] %c%c [+][+] 读取进度 设为参照",hex_[temp_index/0x1000%0x10],hex_[temp_index/0x100%0x10]);
		xyprintf(200,16,"字符[-][-] %c%c [+][+] 保存进度 清除参照",hex_[temp_index/0x10%0x10],hex_[temp_index%0x10]);
		if (shade) {
			xyprintf(200,32,"启用");
			draw_yulan(sdfnt,0xcccccc);
		} else {
			xyprintf(200,32,"禁用");
		}
		xyprintf(232,32,"参照 替换 覆刻");
		fp=&font_buffer[temp_index%0x100];
		xyprintf(356,400,"定位点:%d 字宽:%d",fp->pos,fp->wid);
		draw_yulan(font_buffer[temp_index%0x100],0xffffff);
		draw_ui();
		lbt_update();
		if (inside(msg.x,msg.y,30,60,360,390)) {
			posx=(msg.x-32)/20;
			posy=(msg.y-64)/20;
			xyprintf(0,48,"指针位置(%d,%d)",posx,posy);
			if (KEY_DOWN(VK_LBUTTON)) {
				fp->pxdata[posy]|=0x0001<<(15-posx);
			} else if (KEY_DOWN(VK_RBUTTON)) {
				fp->pxdata[posy]&=~(0x0001<<(15-posx));
			}
		}
		for (i=0;i<16;i++) {
			for (j=0;j<16;j++) {
				font_display(365+16*j,67+16*i,font_buffer[j+i*16]);
			}
		}
		if (KEY_DOWN('Z')) {
			font_buffer[temp_index%0x100].pos=0;
			font_buffer[temp_index%0x100].end=16;
			font_buffer[temp_index%0x100].wid=16;
		}
		if (KEY_DOWN('C')) {
			memset(fp->pxdata,0,32);
		}
		if (KEY_DOWN('V')) {
			for (i=0;i<16;i++) {
				font_buffer[temp_index%0x100].pxdata[i]|=sdfnt.pxdata[i];
			}
		}
		if (is_lbt_down()) {
			if (inside(msg.x,msg.y,234,2,253,15))temp_index-=0x1000;
			if (inside(msg.x,msg.y,258,2,277,15))temp_index-=0x100;
			if (inside(msg.x,msg.y,314,2,333,15))temp_index+=0x100;
			if (inside(msg.x,msg.y,338,2,357,15))temp_index+=0x1000;
			if (inside(msg.x,msg.y,234,18,253,31))temp_index-=0x10;
			if (inside(msg.x,msg.y,258,18,277,31))temp_index-=0x1;
			if (inside(msg.x,msg.y,314,18,333,31))temp_index+=0x1;
			if (inside(msg.x,msg.y,338,18,357,31))temp_index+=0x10;
			if (inside(msg.x,msg.y,365,2,438,15))load();
			if (inside(msg.x,msg.y,365,15,438,35))save();
			if (inside(msg.x,msg.y,200,32,264,48))shade^=1;
			if (inside(msg.x,msg.y,440,0,504,16)) {
				sdfnt=font_buffer[temp_index%0x100];
			}
			if (inside(msg.x,msg.y,440,16,504,32)) {
				memset(sdfnt.pxdata,0,32);
			}
			if (inside(msg.x,msg.y,270,32,303,48)) {
				for (i=0;i<16;i++) {
					fp->pxdata[i]=sdfnt.pxdata[i];
				}
			}
			if (inside(msg.x,msg.y,318,32,351,48)) {
				for (i=0;i<16;i++) {
					font_buffer[temp_index%0x100].pxdata[i]|=sdfnt.pxdata[i];
				}
			}
		}
		if (inside(msg.x,msg.y,0,384,640,454)) {
			if (is_lbt_down()) {
				if (msg.x<0)fp->pos=0;
				if (msg.x>352)fp->pos=15;
				fp->pos=(msg.x-32)/20;
			}
			if (is_lbt_rlse()) {
				if (msg.x<0)fp->end=0;
				if (msg.x>352)fp->end=15;
				fp->end=(msg.x-32)/20;
			}
		}
		fp->wid=fp->end-fp->pos;
		setcolor(0xff0000);
		line(32+fp->pos*20,64,32+fp->pos*20,400);
		setcolor(0x00ff00);
		line(32+fp->end*20,64,32+fp->end*20,400);
	}
	
	getch();
}
