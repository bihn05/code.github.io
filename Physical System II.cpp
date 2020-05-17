#include <fstream>
#include <cmath>
using namespace std;

int main() {
	
	unsigned int filelength=-8;
	filelength+=12;
	unsigned char part1[17]="RIFF    WAVEfmt ";
	filelength+=22;
	unsigned int size=16;
	unsigned short format=1;
	unsigned short channel=1;
	unsigned int samplerate=44100;
	//byterate
	//blockalign
	unsigned short bytepersample=16;
	unsigned int byterate=channel*samplerate*bytepersample/8;
	unsigned int blockalign=channel*byterate/8;
	unsigned char part2[22];
	for (int tmp1=0;tmp1<4;tmp1++)part2[tmp1]=size/pow(0x100,tmp1);
	for (int tmp1=0;tmp1<2;tmp1++)part2[tmp1+4]=format/pow(0x100,tmp1);
	for (int tmp1=0;tmp1<2;tmp1++)part2[tmp1+6]=channel/pow(0x100,tmp1);
	for (int tmp1=0;tmp1<4;tmp1++)part2[tmp1+8]=samplerate/pow(0x100,tmp1);
	for (int tmp1=0;tmp1<4;tmp1++)part2[tmp1+12]=byterate/pow(0x100,tmp1);
	for (int tmp1=0;tmp1<4;tmp1++)part2[tmp1+16]=blockalign/pow(0x100,tmp1);
	for (int tmp1=0;tmp1<2;tmp1++)part2[tmp1+20]=bytepersample/pow(0x100,tmp1);
	unsigned int length=50000;
	unsigned char data[length];
	unsigned char part3[5]="data";
	filelength+=length;
	int t;
	for (int x=0;x<length/2;x++) {
		t=sin(x*80)*pow(2,16);
		data[x*2]=t;
		data[x*2+1]=t/pow(2,8);
	}
	unsigned char filedata[filelength+8];
	for (int tmp1=0;tmp1<4;tmp1++)part1[tmp1+4]=filelength/pow(0x100,tmp1)%0x100;
	
	
	
}