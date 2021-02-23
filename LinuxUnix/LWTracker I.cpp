#include <iostream>
#include <fstream>
#include <cmath>
#include <conio.h>

#define PI 3.141593

using namespace std;

const int bpm=150;
char hex_[17]="0123456789abcdef";
unsigned int SampleRate=11025;

unsigned short BitPerSample=8;
const short ChannelNum=1;
ifstream infile;
ofstream oufile;
char mem[65536];
string wavfile;
long filepoint=0;
string head;
string format;
char code[16][65536];
int tp[16]; //track point
int rtp[16]; //read track point
int tpp=0; //track
int v=80;

int c2i(char i) {
	if (i>='0' && i<='9')i-=(int)'0';
	if (i>='A' && i<='F')i-=55;
	if (i>='a' && i<='f')i-=87;
	return i;
}
int file_length(string filename) {
	ifstream f;
	f.open(filename.c_str());
	f.seekg(0,ios::end);
	int size=f.tellg();
	f.clear();
	f.close();
	return size;
}
void file_in(string filename) {
	infile.open(filename.c_str());
	if (infile.fail()) {
		cout<<filename<<" NOT FOUND";
		return;
	}
	infile.read((char*)mem,65536);
	infile.clear();
	infile.close();
	cout<<filename<<" --> MEMORY"<<endl;
}
void solve() {
	//cout<<mem<<endl;
	for (int i=0;i<16;i++) {
		for (int j=0;j<65536;j++) {
			code[i][j]=0;
		}
		tp[i]=0;
	}
	tpp=0;
	for (int i=0;i<65536;) {
		switch (mem[i++]) {
			case 0:break;
			case '@':{
				cout<<"\e[35m@"<<mem[i];
				tpp=c2i(mem[i++]);
				cout<<"\e[0m";
				break;
			}
			default:{
				code[tpp][tp[tpp]++]=mem[i-1];
				cout<<mem[i-1];
				break;
			}
		}
	}
	for (int i=0;i<16;i++) {
		code[i][tp[i]]='F';
	}
	cout<<endl;
	for (int i=0;i<16;i++) {
		cout<<"\e[34m#"<<hex_[i];
		cout<<"\e[0m";
		cout<<code[i]<<endl;
	}
}
int _time=3;
int length_=-3; //2**-3 * 1m
int type=0; //0正弦 1矩形 2三角 3锯齿 4噪声 5脉冲 6脉冲 7FM
int eff=0; //效果
int vol=0; //值
int vs,ds,vv; //震动 滑动 震动速度
double mode1,mode2=0;
double freq(double off) {
	return 522*pow(2,((double)1/12)*off)*pow(2,_time);
}
double sincwave(double f,int t,double amp) {
	double i;
	i=sin(f*t/SampleRate*2)*amp*pow(2,BitPerSample-1)+pow(2,BitPerSample-1);
	return i;
}
double fmwave(double f,int t,double amp) {
	double i;
	i=sin(f*t/SampleRate*2+mode2*(double)cos(mode1*t/SampleRate))*amp*pow(2,BitPerSample-2)+pow(2,BitPerSample-1);
	return i;
}
double square(double f,int t,double amp,double duty) {
	double i=0;
	f/=4;
	for (double m=1;m<v;m++) {
		i+=sin(duty*m*PI)*cos(PI*f*m*t/SampleRate)/m;
	}
	i*=4*amp/pow(PI,2);
	i+=(2*duty-1)/PI;
	i*=pow(2,BitPerSample-1);
	i+=pow(2,BitPerSample-1);
	return i;
}
double samplecreate(double f,int t) {
	f=freq(f);
	double i;
	switch (type) {
		case 0:i=sincwave(f,t,1);break;
		//case 1:i=square(f,t,0.9,0.5);break;
		case 7:i=fmwave(f,t,0.9);break;
	}
	return i;
}
ofstream wave;
void ins(int off) { //偏移 八度
	unsigned char ss[4];
	string outname;
	double off_;
	for (int t=0;t<(double)15/bpm*SampleRate*pow(2,length_);t++) {
		off_=off+(double)vs/4*sin((double)vv*t/SampleRate)+(double)ds*(double)t/((double)15/bpm*SampleRate*pow(2,length_));
		//cout<<filepoint<<" ";
		switch (BitPerSample) {
			case 8: {
				ss[0]=(int)samplecreate(off_,t)%256;
				filepoint+=1;
				wave.write((char*)ss,1);
				break;
			}
			case 16: {
				unsigned int o=samplecreate(off_,t);
				ss[0]=o;
				ss[1]=o>>8;
				cout<<hex_[ss[0]/0x10%0x10]<<hex_[ss[0]%0x10]<<" ";
				cout<<hex_[ss[1]/0x10%0x10]<<hex_[ss[1]%0x10];
				cout<<" "<<samplecreate(off_,t)<<endl;
				filepoint+=2;
				wave.write((char*)ss,2);
				break;
			}
		}
	}
}
void execute() {
	_time=3;
	length_=2; //2**-3 * 1s
	type=0; //0正弦 1矩形 2三角 3锯齿 4噪声 5脉冲 6脉冲 7FM
	eff=0; //效果
	vol=0; //值
	for (int p=0;code[tpp][p]!='F';) {
		cout<<code[tpp][p];
		//getch();
		switch (code[tpp][p++]) {
			case 'z':ins(0);break;
			case 's':ins(1);break;
			case 'x':ins(2);break;
			case 'd':ins(3);break;
			case 'c':ins(4);break;
			case 'v':ins(5);break;
			case 'g':ins(6);break;
			case 'b':ins(7);break;
			case 'h':ins(8);break;
			case 'n':ins(9);break;
			case 'j':ins(10);break;
			case 'm':ins(11);break;
			case 'q':length_++;break;
			case 'w':length_=0;break;
			case 'e':length_--;break;
			
			case 'r':_time++;break;
			case 't':_time=0;break;
			case 'y':_time--;break;
			case 'Q':vs++;break;
			case 'W':vs=0;break;
			case 'E':vs--;break;
			case 'R':vv++;break;
			case 'T':vv=0;break;
			case 'Y':vv--;break;
			case 'U':ds++;break;
			case 'u':ds--;break;
			case 'i':ds=0;break;
			case 'I': {
				_time=3;
				length_=-2;
				vv=0;
				vs=0;
				ds=0;
				break;
			}
			case 'A':type=7;break;
			case 'a': {
				mode1=pow((double)c2i(code[tpp][p++]),3);
				mode2=pow((double)c2i(code[tpp][p++]),3);
				cout<<"mode1:"<<mode1;
				cout<<",mode2:"<<mode2<<endl;
				getch();
				break;
			}
			case 'S':type=0;break;
			case 'D':type=1;break;
			case 'G':type=2;break;
		}
	}
}
void out() {
	wave.clear();
	wave.close();
	system("rm q.wav");
	ofstream outfile;
	outfile.open("q.wav");
	outfile.write((char*)"RIFF",4);
	int fs=36-8+filepoint;
	unsigned char data_fs[4];
	data_fs[0]=fs%256;
	data_fs[1]=fs/256%256;
	data_fs[2]=fs/256/256%256;
	data_fs[3]=fs/256/256/256%256;
	outfile.write((char*)data_fs,4);
	outfile.write((char*)"WAVE",4);
	outfile.write((char*)"fmt ",4);
	fs=16;
	data_fs[0]=fs%256;
	data_fs[1]=fs/256%256;
	data_fs[2]=fs/256/256%256;
	data_fs[3]=fs/256/256/256%256;
	outfile.write((char*)data_fs,4);
	short ts=1;
	data_fs[0]=ts%256;
	data_fs[1]=ts/256%256;
	outfile.write((char*)data_fs,2);
	outfile.write((char*)data_fs,2);
	data_fs[0]=SampleRate%256;
	data_fs[1]=SampleRate/256%256;
	data_fs[2]=SampleRate/256/256%256;
	data_fs[3]=SampleRate/256/256/256%256;
	outfile.write((char*)data_fs,4);
	fs=SampleRate*BitPerSample/8;
	data_fs[0]=fs%256;
	data_fs[1]=fs/256%256;
	data_fs[2]=fs/256/256%256;
	data_fs[3]=fs/256/256/256%256;
	outfile.write((char*)data_fs,4);
	ts=BitPerSample/8;
	data_fs[0]=ts%256;
	data_fs[1]=ts/256%256;
	outfile.write((char*)data_fs,2);
	ts=BitPerSample;
	data_fs[0]=ts%256;
	data_fs[1]=ts/256%256;
	outfile.write((char*)data_fs,2);
	outfile.write((char*)"data",4);
	//will be changed
	fs=filepoint;
	data_fs[0]=fs%256;
	data_fs[1]=fs/256%256;
	data_fs[2]=fs/256/256%256;
	data_fs[3]=fs/256/256/256%256;
	outfile.write((char*)data_fs,4);
	
	outfile.clear();
	outfile.close();
	
	system("rm p.wav");
	system("cat q.wav wave0 > p.wav");
}
int main() {
	cout<<"PLEASE INPUT IN-FILE FILENAME"<<endl;
	string filename;
	cin>>filename;
	file_in(filename);
	string outname;
	for (int t=0;t<16;t++) {
		outname="rm wave";
		outname[7]=hex_[t];
		system(outname.c_str());
	}
	string o;
	o="wave";
	o[4]=hex_[tpp];
	wave.open(o.c_str());
	solve();
	tpp=0;
	execute();
	
	out();
	cout<<endl<<"done.";
}
