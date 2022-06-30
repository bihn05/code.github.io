#include <graphics.h>
#include <iostream>
#include <fstream>
#include <cmath>
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) 
#define SIZE 2048
//部件包上限 
using namespace std;
using namespace ege;
//各种状态
//0指针 
//1起点模式 2起点修饰 3终点模式 4终点修饰 
POINT msg={0,0};
HWND h=GetForegroundWindow();
bool qbt_tmp1,qbt_tmp2; 
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
void qbt_update() {
	qbt_tmp2=qbt_tmp1;
	qbt_tmp1=KEY_DOWN('Q');
}
bool is_qbt_down() {
	return (qbt_tmp1==1 & qbt_tmp2==0);
}
bool is_qbt_rlse() {
	return (qbt_tmp1==0 & qbt_tmp2==1);
}
int crsx=256,crsy=240;
struct P {
	double x,y;
	void test_output_info() {
		cout<<"点("<<x<<","<<y<<")"; 
	}
};
P padd(P p1, P p2) {
	return {p1.x+p2.x,p1.y+p2.y};
}
double pplen(P p0, P p1) {
	double r=pow( pow(p0.x-p1.x,2)+pow(p0.y-p1.y,2) ,0.5);
	return abs(r);
}
int realx(int x) {
	return crsx+x;
}
int realy(int y) {
	return crsy-y;
}
int arealx(int x) {
	return x-crsx;
}
int arealy(int y) {
	return crsy-y;
}
void bseqx(P pstart, P pmod1, P pmod2, P pend,int color) {
	P p0,p1,p2,p3;
	p0=pstart,p1=pmod1,p2=pmod2,p3=pend;
	int step=pplen(p0,p1)+pplen(p1,p2)+pplen(p2,p3);
	step*=2;
	double vp,vq;
	int tx,ty;
	for (int i=0;i<step;i+=1) {
		vp=(double)i/step;
		//cout<<vp*100<<"%"<<endl;
		vq=1-vp;
		tx=p0.x*pow(vq,3)+3*p1.x*pow(vq,2)*pow(vp,1)+3*p2.x*pow(vq,1)*pow(vp,2)+p3.x*pow(vp,3);
		ty=p0.y*pow(vq,3)+3*p1.y*pow(vq,2)*pow(vp,1)+3*p2.y*pow(vq,1)*pow(vp,2)+p3.y*pow(vp,3);
		putpixel(realx(tx),realy(ty),color);
	}
}
void xdbseqx(P pstart, P pmod1, P pmod2, P pend,int color) {
	bseqx(pstart,padd(pstart,pmod1),padd(pend,pmod2),pend,color);
}
void adv_xdbseqx(P pos, double scale, P pstart, P pmod1, P pmod2, P pend,int color) {
	int tcx,tcy;
	tcx=crsx,tcy=crsy;
	crsx=pos.x,crsy=pos.y;
	
	pstart.x*=scale;
	pstart.y*=scale;
	pmod1.x*=scale;
	pmod1.y*=scale;
	pmod2.x*=scale;
	pmod2.y*=scale;
	pend.x*=scale;
	pend.y*=scale;
	xdbseqx(pstart,pmod1,pmod2,pend,color);
	
	crsx=tcx,crsy=tcy;
}
void draw_crs(double x, double y) {
	setcolor(0xffffff);
	line(x,y,x+5,y);
	line(x,y,x,y+5);
	line(x,y,x+7,y+7);
}
bool inside(int x,int y,int x1,int y1,int x2,int y2) {
	if (x<=x2 && x>=x1 && y<=y2 && y>=y1)return 1; else return 0;
}
struct bihua {
	bool enabled;
	int length;
	P point[64];
	P mod1[64];
	P mod2[64];
	void draw_output() {
		for (int i=1;i<length;i++) {
			xdbseqx(point[i-1],mod2[i-1],mod1[i],point[i],0x00ff00);
		}
	}
	void draw_output(int color) {
		for (int i=1;i<length;i++) {
			xdbseqx(point[i-1],mod2[i-1],mod1[i],point[i],color);
		}
	}
	void draw_output_adv(P pos, double scale) {
		for (int i=1;i<length;i++) {
			adv_xdbseqx(pos,scale,point[i-1],mod2[i-1],mod1[i],point[i],0x00ff00);
		}
	}
	void draw_output_adv(P pos, double scale, int color) {
		for (int i=1;i<length;i++) {
			adv_xdbseqx(pos,scale,point[i-1],mod2[i-1],mod1[i],point[i],color);
		}
	}
	void reset() {
		memset(point,0,64*sizeof(P));
		memset(mod1,0,64*sizeof(P));
		memset(mod2,0,64*sizeof(P));
	}
	void delete_point(int index) {
		for (int i=index;i<length;i++) {
			point[i]=point[i+1];
			mod1[i]=mod1[i+1];
			mod2[i]=mod2[i+1];
		}
		point[length]={0,0};
		mod1[length]={0,0};
		mod2[length]={0,0};
		length--;
	}
	void insert_point(P pos, int index) {
		if (index>length) {
			index=length;
		}
		for (int i=length;i>=index;--i) {
			point[i+1]=point[i];
			mod1[i+1]=mod1[i];
			mod2[i+1]=mod2[i];
		}
		point[index]=pos;
		length+=1;
	}
	void add_point(P pos) {
		point[length]=pos;
		length+=1;
	}
	void test_output_info() {
		cout<<"#测试 [笔画]报告"<<endl; 
		cout<<"逐线段: "<<endl;
		if (length==0)cout<<"\t无信息"<<endl; else {
			for (int i=1;i<64;i++) {
				cout<<"\t第 "<<i<<" 个, 还剩 "<<length<<" 个: "<<endl; 
				cout<<"绘制起点 ";
				point[i-1].test_output_info();
				cout<<" 调制点 "; 
				mod2[i-1].test_output_info();
				cout<<endl;
				cout<<"绘制终点 ";
				point[i].test_output_info();
				cout<<" 调制点 "; 
				mod1[i].test_output_info();
				cout<<endl;
			}
		}
		
		cout<<"##完毕"<<endl<<endl;
	}
};
struct hanzi {
	bool enabled;
	unsigned short id; //映射到Unicode 
	char name[17]; //字名 
	bihua bih[64];
	int count() { //笔画数 
		int temp=0;
		for (int i=0;i<64;i++) {
			if (bih[i].enabled)temp++;
		}
		return temp;
	}
	void test_output_info() {
		cout<<"#测试 [汉字]报告字体形状"<<endl; 
		for (int i=0;i<count();i++) {
			cout<<"第"<<i<<"笔"<<endl;
		}
		cout<<"##完毕"<<endl<<endl;
	}
	void display(P pos, double scale) {
		if (enabled) {
			for (int i=0;i<64;i++) {
				if (bih[i].enabled) {
					bih[i].draw_output_adv(pos,scale);
				}
			}
			putpixel(pos.x,pos.y,0xff0000);
		}
	}
};
hanzi object_hanzi; //用于状态显示的目标 
void draw_frame() {
	cleardevice();
	//绘制程序视图框架 调用相关函数 
	setfont(12,0,"MingLiU");
	setcolor(0xffffff);
	xyprintf(514,0,"笔划");
	xyprintf(514,201,"笔画");
	xyprintf(514,312,"部件库");
	setfont(20,0,"MingLiU");
	setcolor(0xffffff);
	setcolor(0xffffff);
	line(512,0,512,480);
	line(512,200,640,200);
	line(512,310,640,310);
	line(500,0,500,480);
	line(500,0,512,0);
	line(500,12,512,12);
	line(500,24,512,24);
	line(500,36,512,36);
	line(500,48,512,48);
	line(500,60,512,60);
	line(500,72,512,72);
	line(500,84,512,84);
	line(500,96,512,96);
	setcolor(0x333333);
	line(realx(0),0,realx(0),480);
	line(0,realy(0),512,realy(0));
	
}
typedef struct tagWNDINFO
{
	DWORD dwProcessId;
	HWND hWnd;
} WNDINFO, *LPWNDINFO;
static BOOL CALLBACK EnumProc(HWND hWnd, LPARAM lParam)
{
	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	LPWNDINFO pInfo = (LPWNDINFO)lParam;

	if (dwProcessId == pInfo->dwProcessId)
	{
		pInfo->hWnd = hWnd;
		return FALSE;
	}
	return TRUE;
}
static HWND GetProcessMainWnd()
{
	DWORD dwProcessId=::GetCurrentProcessId();
	WNDINFO wi;
	wi.dwProcessId = dwProcessId;
	wi.hWnd = NULL;
	EnumWindows(EnumProc, (LPARAM)&wi);
	return wi.hWnd;
}
int lbst;
int bihuapage=0;
int bhselect=0;
int huapage=0;
int huaselect=0;
int editmode=0;
HWND hf=GetFocus();
HWND hw;
int iid,iid2;
void hanziedit() {
	int indexpt;
	for (;;delay_fps(25)) {
		cleardevice();
		draw_frame();
		setfont(16,0,"MingLiU");
		setcolor(0xffffff);
		
		//摆烂的焦点检测 
		GetCursorPos(&msg);
		h=GetProcessMainWnd();
		ScreenToClient(h,&msg);
		hf=GetFocus();
		iid=GetDlgCtrlID(GetFocus());
		iid2=GetDlgCtrlID(h);
		if (iid != iid2)msg={-1,-1};
		lbt_update();
		lbst=lbt_tmp1-lbt_tmp2;
		
		if (KEY_DOWN(VK_F1)) {
			cleardevice();
			while (!((iid == iid2) & KEY_DOWN(VK_RETURN))) {
				delay_fps(5);
				xyprintf(0,0,"LWOS矢量字体编辑器 v1 - 帮助手册界面 (按回车键恢复)");
				xyprintf(0,16,"    这是由LW字体研究所设计的字体编辑器,能够将任何字符以矢量的形式描绘出来");
				xyprintf(0,16*2,"一. 快捷键");
				xyprintf(0,16*3,"  F1 - 弹出该界面"); 
			}
		}
		for (int i=0;i<10;i++) {
			if (i+huapage*10 >= object_hanzi.bih[bhselect].length)continue;
			if (i+huapage*10 == huaselect) {
				xyprintf(515,12+i*16,">第%d点(%d,%d)",i+bihuapage*10+1,
				object_hanzi.bih[bhselect].point[i+huapage*10].x,
				object_hanzi.bih[bhselect].point[i+huapage*10].y
				);
			} else {
				xyprintf(515,12+i*16," 第%d点(%d,%d)",i+bihuapage*10+1,
				object_hanzi.bih[bhselect].point[i+huapage*10].x,
				object_hanzi.bih[bhselect].point[i+huapage*10].y
				);
			}
		}
		for (int i=0;i<5;i++) {
			if (i+bihuapage*5 >= object_hanzi.count())continue;
			if (i+bihuapage*5 == bhselect) {
				xyprintf(515,213+i*16,">第%d笔",i+bihuapage*5+1);
			} else {
				xyprintf(515,213+i*16," 第%d笔",i+bihuapage*5+1);
			}
			if (object_hanzi.bih[i+bihuapage*5].enabled) {
				putpixel(628,221+i*16,0xffff00);
				object_hanzi.bih[i+bihuapage*5].draw_output_adv({628,221+i*16},0.1);
				putpixel(628,221+i*16,0xff0000);
			}
		}
		for (int i=0;i<object_hanzi.count();i++) {
			if (i!=bhselect) {
				object_hanzi.bih[i].draw_output(0x00ff00);
			} else {
				object_hanzi.bih[i].draw_output(0xffff00);
			}
		}
		
		
		xyprintf(515,184,"咔咔第%d页,共%d页",huapage+1,object_hanzi.bih[bhselect].length/5+1);
		xyprintf(515,293,"咔咔第%d页,共%d页",bihuapage+1,object_hanzi.count()/5+1);
		xyprintf(0,0,"%d,%d",msg.x,msg.y);
		setcolor(0xff0000);
		line(
		realx(object_hanzi.bih[bhselect].point[huaselect].x),
		realy(object_hanzi.bih[bhselect].point[huaselect].y),
		realx(object_hanzi.bih[bhselect].point[huaselect+1].x),
		realy(object_hanzi.bih[bhselect].point[huaselect+1].y)
		);
		line(
		realx(object_hanzi.bih[bhselect].point[huaselect+1].x),
		realy(object_hanzi.bih[bhselect].point[huaselect+1].y),
		realx(object_hanzi.bih[bhselect].point[huaselect+1].x+object_hanzi.bih[bhselect].mod1[huaselect+1].x),
		realy(object_hanzi.bih[bhselect].point[huaselect+1].y+object_hanzi.bih[bhselect].mod1[huaselect+1].y)
		);
		setcolor(0xff00ff);
		line(
		realx(object_hanzi.bih[bhselect].point[huaselect].x),
		realy(object_hanzi.bih[bhselect].point[huaselect].y),
		realx(object_hanzi.bih[bhselect].point[huaselect].x+object_hanzi.bih[bhselect].mod2[huaselect].x),
		realy(object_hanzi.bih[bhselect].point[huaselect].y+object_hanzi.bih[bhselect].mod2[huaselect].y)
		);
		setcolor(0xffffff);
		switch (editmode) {
			case 0:xyprintf(448,460,"正常模式");break;
			case 1:xyprintf(448,460,"描点模式");break;
			case 2:xyprintf(448,460,"调制点一");break;
			case 3:xyprintf(448,460,"调制点二");break;
		}
		if (inside(msg.x,msg.y,0,0,500,512)) {
			if (KEY_DOWN(VK_LBUTTON)) {
				if (editmode==1) {
					object_hanzi.bih[bhselect].point[huaselect]={arealx(msg.x),arealy(msg.y)};
				}
				if (editmode==2) {
					object_hanzi.bih[bhselect].mod2[huaselect]={
					arealx(msg.x)-object_hanzi.bih[bhselect].point[huaselect].x,
					arealy(msg.y)-object_hanzi.bih[bhselect].point[huaselect].y};
				}
				if (editmode==3) {
					object_hanzi.bih[bhselect].mod1[huaselect+1]={
					arealx(msg.x)-object_hanzi.bih[bhselect+1].point[huaselect].x,
					arealy(msg.y)-object_hanzi.bih[bhselect+1].point[huaselect].y};
				}
			}
			xyprintf(0,460,"编辑区");
		}
		if (inside(msg.x,msg.y,500,0,512,12)) {
			if (lbst==1) {
				editmode=0;
			}
			xyprintf(0,460,"指针模式");
		}
		if (inside(msg.x,msg.y,500,12,512,24)) {
			if (lbst==1) {
				editmode=1;
			}
			xyprintf(0,460,"描点");
		}
		if (inside(msg.x,msg.y,500,24,512,36)) {
			if (lbst==1) {
				editmode=2;
			}
			xyprintf(0,460,"修改第一调制点");
		}
		if (inside(msg.x,msg.y,500,36,512,48)) {
			if (lbst==1) {
				editmode=3;
			}
			xyprintf(0,460,"修改第二调制点");
		}
		if (editmode==1) {
			if (inside(msg.x,msg.y,500,48,512,60)) {
				if (lbst==1) {
					object_hanzi.bih[bhselect].insert_point({0,0},huaselect);
				}
				xyprintf(0,460,"插入点");
			}
			if (inside(msg.x,msg.y,500,60,512,72)) {
				if (lbst==1) {
					object_hanzi.bih[bhselect].add_point({0,0});
				}
				xyprintf(0,460,"添加点");
			}
			if (inside(msg.x,msg.y,500,72,512,84)) {
				if (lbst==1) {
					object_hanzi.bih[bhselect].delete_point(huaselect);
				}
				xyprintf(0,460,"删除点");
			}
		}
		if (inside(msg.x,msg.y,500,84,512,96)) {
			if (lbst==1) {
				object_hanzi.bih[bhselect].enabled^=1;
			}
			xyprintf(0,460,"启用/禁用此笔画");
		}
		if (inside(msg.x,msg.y,521,185,531,192)) {
			if (lbst==1) {
				huaselect--;
			}
			xyprintf(0,460,"上一划");
		}
		if (inside(msg.x,msg.y,521,193,531,200)) {
			if (lbst==1) {
				huaselect++;
			}
			xyprintf(0,460,"下一划");
		}
		if (inside(msg.x,msg.y,521,295,531,302)) {
			if (lbst==1) {
				bhselect--;
			}
			xyprintf(0,460,"上一笔画");
		}
		if (inside(msg.x,msg.y,521,303,531,310)) {
			if (lbst==1) {
				bhselect++;
			}
			xyprintf(0,460,"下一笔画");
		}
		if (bhselect==-1)bhselect=0;
		if (huaselect==-1)huaselect=0;
		huapage=huaselect/5; 
		bihuapage=bhselect/5;
	}
}
char ru[4];
std::string UtfToGbk(const char* utf8) {
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
	ru[0]=0xE0+u/0x100/0x10;
	ru[1]=0x80+u/0x100%0x10*0x4+u%0x100/0x10/0x4;
	ru[2]=0x80+u%0x100/0x10%0x4*0x10+u%0x100%0x10;
	ru[3]=0;
	return UtfToGbk(ru);
}
ifstream infile;
ofstream otfile;
const char hex_[17]="0123456789ABCDEF";
void out_char(unsigned char c) {
	cout<<hex_[c/0x10]<<hex_[c%0x10];
}
void out_short(unsigned short c) {
	out_char(c/0x100);
	out_char(c%0x100);
}
void out_int(unsigned int c) {
	out_short(c/0x10000);
	out_short(c%0x10000);
}
hanzi sto[SIZE];
hanzi buffer[256];
void list() {
	cout<<"部件包信息"<<endl; 
	for (int i=0;i<SIZE;i++) {
		if (sto[i].enabled) {
			cout<<"包内编号: #"<<i+1<<endl;
			cout<<"字: "<<sto[i].name;
			cout<<"(选)映射Unicode: 0x";
			out_int(sto[i].id);
			cout<<" ("<<uni(sto[i].id)<<")"<<endl;
			sto[i].test_output_info();
		}
	}
}
string filename;
void clear() {
	memset(sto,0,SIZE*sizeof(hanzi));
}
void output() {
	otfile.open("part.bin");
	for (int i=0;i<SIZE;i++) {
		if (sto[i].enabled) {
			otfile.write((char*)(&sto[i]),sizeof(hanzi));
		}
	}
	otfile.close();
	otfile.clear();
}
void input() {
	infile.open("part.bin");
	infile.read((char*)sto,SIZE*sizeof(hanzi));
	infile.close();
	infile.clear();
}

void font_output() {
	cout<<"输出字体文件. . .";
	 
}
void font_input() {
	cout<<"加载字体文件. . .";
}
int room=0;
bool is_saved=0;
int buffer_pos=0;
int main() {
	filename="Font.fnt";
	setinitmode(2);
	initgraph(640,480);
	draw_frame();
	hanzi hz;
	hz.bih[0].enabled=1;
	hz.bih[0].add_point({-50,-50});
	hz.bih[0].add_point({-20,-30});
	hz.bih[0].add_point({0,24});
	buffer[0].enabled=1;
	buffer[0].bih[0].enabled=1;
	buffer[0].bih[0].add_point({-200,0});
	buffer[0].bih[0].add_point({180,20});
	for (;is_run();delay_fps(15)) {
		cleardevice();
		setfont(16,0,"MingLiU");
		setcolor(0xffffff);
		if (is_saved) {
			xyprintf(0,0,"当前编辑:%s",filename.c_str());
		} else {
			xyprintf(0,0,"当前编辑:%s*",filename.c_str());
		}
		setfont(11,0,"MingLiU");
		for (int i=0;i<17;i++) {
			if (i!=16)xyprintf(10+28*i,470,"%x",i);
			line(10+i*28,16,10+i*28,465);
		}
		for (int i=0;i<17;i++) {
			if (i!=16)xyprintf(4,16+28*i,"%x",i);
			line(10,16+i*28,459,16+i*28);
		}
		setcolor(0x555555);
		line(476,50,476+128,50+128);
		line(476+128,50,476,50+128);
		line(476+64,50,476+64,50+128);
		line(476,50+64,476+128,50+64);
		setcolor(0xffffff);
		rectangle(476,50,476+128,50+128);
		buffer[0].display({476+64,50+64},0.25);
		setfont(16,0,"MingLiU");
		xyprintf(480,200,"当前字符: %s",uni(0x4e00).c_str());
		for (int i=0;i<16;i++) {
			for (int j=0;j<16;j++) {
				putpixel(24+j*28,30+i*28,0xffffff);
				buffer[i*16+j].display({24+j*28,30+i*28},0.05);
			}
		}
	/*
			case 1: {
				object_hanzi=hz;
				hanziedit();
			}*/
	}
	return 0;
}
