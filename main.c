#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <complex.h>
#include <string.h>
#include <curses.h>			// requires curses library
#include "g31ddcapi.h"
#include <time.h>
//#include <pulse/simple.h>	// requires pulse library
//#include <pulse/error.h>

// FFT stuff
typedef double complex cplx;

// counters
unsigned long long int TotalAudio=0,TotalDDC1=0,TotalDDC2=0;
unsigned long long int StartTime;


#define FFT_SIZE 64
cplx DDC1_FFT_Buff[FFT_SIZE];
char DDC1_FFT_Out[6*FFT_SIZE+1];
int MinD1=0,MaxD1=0,MinD2=0,MaxD2=0,MinA=0,MaxA=0,MinI=0,MaxI=0;
#define DB_PRECISION 8
char dB_Char[DB_PRECISION]={'0','4','5','1','2','6','3','7'};

char BufferIndexArray[4]={'/','-','\\','|'};

int DDC1_BufferIndex=0;

void fft(cplx buf[],bool copyback);
void Translate(cplx fftbuff[],char *outbuff,bool invert,int *min,int *max);

void DDC1StreamCallback(const void *Buffer,uint32_t NumberOfSamples,uint32_t BitsPerSample,uintptr_t UserData) {
	int i;
	DDC1_BufferIndex=(DDC1_BufferIndex+1)&3;
	TotalDDC1+=NumberOfSamples;
	for (i=0;i<FFT_SIZE;i++) DDC1_FFT_Buff[i]=(double)((int*)Buffer)[i]/3e9+I*((double)((int*)Buffer)[i+1]/3e9);
	fft(DDC1_FFT_Buff,true);
	Translate(DDC1_FFT_Buff,DDC1_FFT_Out,true,&MinD1,&MaxD1);
}

int main(int argc, char **argv){
	G31DDC_OPEN_DEVICE OpenDevice;
	G31DDC_CLOSE_DEVICE CloseDevice;
	G31DDC_START_IF StartIF;
	G31DDC_START_DDC1 StartDDC1;
	G31DDC_STOP_IF StopIF;
	G31DDC_SET_POWER SetPower;
	G31DDC_SET_DDC1 SetDDC1;
	G31DDC_STOP_DDC1 StopDDC1;
	G31DDC_START_DDC2 StartDDC2;
	G31DDC_STOP_DDC2 StopDDC2;
	G31DDC_START_AUDIO StartAudio;
	G31DDC_STOP_AUDIO StopAudio;
	G31DDC_SET_DDC1_FREQUENCY SetDDC1Frequency;
	G31DDC_SET_DDC2_FREQUENCY SetDDC2Frequency;
	G31DDC_START_DDC1_PLAYBACK StartDDC1Playback;
	G31DDC_GET_DDC1 GetDDC1;
	G31DDC_GET_DDC2 GetDDC2;
	G31DDC_SET_DEMODULATOR_FILTER_BANDWIDTH SetDemodulatorFilterBandwidth;
	G31DDC_SET_DEMODULATOR_MODE SetDemodulatorMode;
	G31DDC_SET_AGC SetAGC;
	G31DDC_SET_AGC_PARAMS SetAGCParams;
	G31DDC_GET_DEVICE_INFO GetDeviceInfo;
	G31DDC_SET_CALLBACKS SetCallbacks;
	G31DDC_CALLBACKS Callbacks;
	G31DDC_GET_DEVICE_LIST GetDeviceList;
	G31DDC_DEVICE_INFO *List;

	//Callbacks.IFCallback=IFCallback;
	Callbacks.DDC1StreamCallback=DDC1StreamCallback;
	//Callbacks.DDC1PlaybackStreamCallback=DDC1PlaybackStreamCallback;
	//Callbacks.DDC2StreamCallback=DDC2StreamCallback;
	//Callbacks.DDC2PreprocessedStreamCallback=DDC2PreprocessedStreamCallback;
	//Callbacks.AudioStreamCallback=AudioStreamCallback;
	//Callbacks.AudioPlaybackStreamCallback=AudioPlaybackStreamCallback;

	int32_t hDevice=0;
	void *API;
	int freq = 10000000,i;
	for(i=0; i<argc; i++){
		if(!strcmp(argv[i],"-f") && argc >=i+2){
			freq = strtol(argv[i+1],0,10);
			printf("Частота установлена %d на Гц\n",freq);
		}
	}
	API=dlopen("libg31ddcapi.so",RTLD_LAZY);
	if (API) {
		OpenDevice=(G31DDC_OPEN_DEVICE)dlsym(API,"OpenDevice");
		CloseDevice=(G31DDC_CLOSE_DEVICE)dlsym(API,"CloseDevice");
		SetPower=(G31DDC_SET_POWER)dlsym(API,"SetPower");
		SetDDC1=(G31DDC_SET_DDC1)dlsym(API,"SetDDC1");
		GetDDC2=(G31DDC_GET_DDC2)dlsym(API,"GetDDC2");
		GetDDC1=(G31DDC_GET_DDC1)dlsym(API,"GetDDC1");
		StartIF=(G31DDC_START_IF)dlsym(API,"StartIF");
		StopIF=(G31DDC_STOP_IF)dlsym(API,"StopIF");
		StopAudio=(G31DDC_STOP_AUDIO)dlsym(API,"StopAudio");
		StartDDC1=(G31DDC_START_DDC1)dlsym(API,"StartDDC1");
		StopDDC1=(G31DDC_STOP_DDC1)dlsym(API,"StopDDC1");
		StartDDC2=(G31DDC_START_DDC2)dlsym(API,"StartDDC2");
		StartAudio=(G31DDC_START_AUDIO)dlsym(API,"StartAudio");
		StopDDC2=(G31DDC_STOP_DDC2)dlsym(API,"StopDDC2");
		SetDDC2Frequency=(G31DDC_SET_DDC2_FREQUENCY)dlsym(API,"SetDDC2Frequency");
		SetDDC1Frequency=(G31DDC_SET_DDC1_FREQUENCY)dlsym(API,"SetDDC1Frequency");
		StartDDC1Playback=(G31DDC_START_DDC1_PLAYBACK)dlsym(API,"StartDDC1Playback");
		SetDemodulatorFilterBandwidth=(G31DDC_SET_DEMODULATOR_FILTER_BANDWIDTH)dlsym(API,"SetDemodulatorFilterBandwidth");
		SetDemodulatorMode=(G31DDC_SET_DEMODULATOR_MODE)dlsym(API,"SetDemodulatorMode");
		SetAGC=(G31DDC_SET_AGC)dlsym(API,"SetAGC");
		SetAGCParams=(G31DDC_SET_AGC_PARAMS)dlsym(API,"SetAGCParams");
		SetCallbacks=(G31DDC_SET_CALLBACKS)dlsym(API,"SetCallbacks");
		GetDeviceInfo=(G31DDC_GET_DEVICE_INFO)dlsym(API,"GetDeviceInfo");
		GetDeviceList=(G31DDC_GET_DEVICE_LIST)dlsym(API,"GetDeviceList");
		uint32_t Count,i,ItemSize;
		
		
		if(Count	=	GetDeviceList(NULL,0)){
		
			List	=	(G31DDC_DEVICE_INFO*)malloc(Count*sizeof(G31DDC_DEVICE_INFO));
			Count	=	GetDeviceList(List,Count*sizeof(G31DDC_DEVICE_INFO));
			printf("Available devices: %d\n",Count);
			
			for(i = 0;i < Count;i++){
				printf("Device %8s \n",List[i].SerialNumber);
			}
			hDevice=OpenDevice(List[0].SerialNumber);
			if (hDevice) {
				G31DDC_DEVICE_INFO info;
				G3XDDC_DDC_INFO info1={0},info2={0};
				GetDeviceInfo(hDevice,&info,sizeof(info));
				printf("Device %8s opened, handle=%d\n",info.SerialNumber,hDevice);	
				printf("Interface: %d\n",info.InterfaceType);
				printf("Channel Count: %u\n",info.ChannelCount);
				printf("DDC Type Count: %u\n",info.DDCTypeCount);
				getchar();
				
			}else {
				printf("Failed to open device. Error code=%d\n",errno);
			}
		}
		puts("Closing the API");
		dlclose(API);
    }
	
	return 0;
}
double window[FFT_SIZE];
void _fft(cplx buf[], cplx out[],int step)
{
int i;
if (step<FFT_SIZE) {
	_fft(out,buf,step*2);
	_fft(out+step,buf+step,step*2);

	for (i=0;i<FFT_SIZE;i+=2*step) {
		cplx t=cexp(-1*I*M_PI*i/FFT_SIZE)*out[i+step];
		buf[i/2]=out[i]+t;
		buf[(i+FFT_SIZE)/2]=out[i]-t;
		}
	}
}
void fft(cplx buf[],bool copyback)
{
int i;
cplx out[FFT_SIZE];
for (i=0;i<FFT_SIZE;i++) out[i] = buf[i]*window[i];
_fft(buf,out,1);
if (copyback) for (i=0;i<FFT_SIZE;i++) buf[i]=out[i];
}
void Translate(cplx fftbuff[],char *outbuff,bool invert,int *min,int *max)
{
double maxdB=-100000,mindB=0;
int i;
for (i=0;i<FFT_SIZE;i++) {
	double a=10*log10(creal(fftbuff[i])*creal(fftbuff[i])+cimag(fftbuff[i])*cimag(fftbuff[i])+1e-20);
	fftbuff[i]=a;
	if (a>maxdB) maxdB=a;
	if (a<mindB) mindB=a;
	}
maxdB-=mindB;
if (invert) for (i=0;i<FFT_SIZE/2;i++) {
	unsigned int index=(unsigned int)((DB_PRECISION-1)*((creal(fftbuff[i])-mindB)/maxdB));
	outbuff[6*(FFT_SIZE/2-1-i)+3]=dB_Char[index];
	index=(unsigned int)((DB_PRECISION-1)*((creal(fftbuff[FFT_SIZE/2+i])-mindB)/maxdB));
	outbuff[6*(FFT_SIZE-1-i)+3]=dB_Char[index];
	}
else for (i=0;i<FFT_SIZE;i++) {
	unsigned int index=(unsigned int)((DB_PRECISION-1)*((creal(fftbuff[i])-mindB)/maxdB));
	if (index>=DB_PRECISION) index=DB_PRECISION-1;
	outbuff[6*i+3]=dB_Char[index];
	}
*min=(int)mindB;
*max=(int)maxdB;
}
