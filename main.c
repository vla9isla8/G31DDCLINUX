#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <complex.h>
#include <string.h>
#include <curses.h>         	// requires curses library
#include "g31ddcapi.h"
#include <time.h>
//#include <pulse/simple.h>		// requires pulse library
//#include <pulse/error.h>
// FFT stuff
typedef double complex cplx;

// counters
unsigned long long int TotalAudio=0,TotalDDC1=0,TotalDDC2=0;
unsigned long long int StartTime;

#define NOC 1024
#define FFT_SIZE 512
cplx DDC2_FFT_Buff[FFT_SIZE];
char DDC2_FFT_Out[6*FFT_SIZE+1];
int MinD2=0,MaxD2=0;
#define DB_PRECISION 8
char dB_Char[DB_PRECISION]={'0','4','5','1','2','6','3','7'};

char BufferIndexArray[4]={'/','-','\\','|'};

int DDC2_BufferIndex=0;

void fft(cplx buf[],bool copyback);
void Translate(cplx fftbuff[],char *outbuff,bool invert,int *min,int *max);
void DDC2StreamCallback(uint32_t Channel,const float *Buffer,uint32_t NumberOfSamples,uintptr_t UserData) {
	int i;
	//DDC2_BufferIndex=(DDC2_BufferIndex+1)&3;
	//TotalDDC2+=NumberOfSamples;
	for (i=0;i<NOC;i++) 
	DDC2_FFT_Buff[i/2]=Buffer[i]+I*(Buffer[++i]);
	//fft(DDC2_FFT_Buff,true);
	Translate(DDC2_FFT_Buff,DDC2_FFT_Out,true,&MinD2,&MaxD2);
	printf("SL: %d",MaxD2);
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
	G31DDC_GET_DDC1_FREQUENCY GetDDC1Frequency;
	G31DDC_GET_DDC2_FREQUENCY GetDDC2Frequency;
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
	//Callbacks.DDC1StreamCallback=DDC1StreamCallback;
	//Callbacks.DDC1PlaybackStreamCallback=DDC1PlaybackStreamCallback;
	//Callbacks.DDC2StreamCallback=DDC2StreamCallback;
	//Callbacks.DDC2PreprocessedStreamCallback=DDC2PreprocessedStreamCallback;
	//Callbacks.AudioStreamCallback=AudioStreamCallback;
	//Callbacks.AudioPlaybackStreamCallback=AudioPlaybackStreamCallback;
	
	int32_t hDevice=0;
	void *API;
	struct timespec tt;
	int freq = 10000000,freq2=0,i;
	for(i=0; i<argc; i++){
		if(!strcmp(argv[i],"-f") && argc >=i+2){
			freq = strtol(argv[i+1],0,10);
			//printf("Частота установлена %d на Гц\n",freq);
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
		GetDDC1Frequency=(G31DDC_GET_DDC1_FREQUENCY)dlsym(API,"GetDDC1Frequency");
		GetDDC2Frequency=(G31DDC_GET_DDC2_FREQUENCY)dlsym(API,"GetDDC2Frequency");
		StartDDC1Playback=(G31DDC_START_DDC1_PLAYBACK)dlsym(API,"StartDDC1Playback");
		SetDemodulatorFilterBandwidth=(G31DDC_SET_DEMODULATOR_FILTER_BANDWIDTH)dlsym(API,"SetDemodulatorFilterBandwidth");
		SetDemodulatorMode=(G31DDC_SET_DEMODULATOR_MODE)dlsym(API,"SetDemodulatorMode");
		SetAGC=(G31DDC_SET_AGC)dlsym(API,"SetAGC");
		SetAGCParams=(G31DDC_SET_AGC_PARAMS)dlsym(API,"SetAGCParams");
		SetCallbacks=(G31DDC_SET_CALLBACKS)dlsym(API,"SetCallbacks");
		GetDeviceInfo=(G31DDC_GET_DEVICE_INFO)dlsym(API,"GetDeviceInfo");
		GetDeviceList=(G31DDC_GET_DEVICE_LIST)dlsym(API,"GetDeviceList");
		int Count,i;
		Count	=	GetDeviceList(NULL,0);
		switch (Count){
			case -1:
				printf("Error get list of diveces: error number - %d\n",errno);
			break;
			case 0:
				printf("Devices not found\n");
			break;
			default:
				List	=	(G31DDC_DEVICE_INFO*)malloc(Count*sizeof(G31DDC_DEVICE_INFO));
				Count	=	GetDeviceList(List,Count*sizeof(*List));
				printf("Available devices: %d\n",Count);
				
				for(i = 0;i < Count;i++){
					printf("Device %8s \n",List[i].SerialNumber);
				}
				hDevice=OpenDevice(List[0].SerialNumber);
				if (hDevice) {
					G31DDC_DEVICE_INFO info;
					G3XDDC_DDC_INFO info1={0},info2={0};
					GetDeviceInfo(hDevice,&info,sizeof(info));
					puts("-----------------------------------");
					puts("-----------------------------------");
					printf("Device %8s opened, handle=%d\n",info.SerialNumber,hDevice);	
					printf("Interface: %d\n",				info.InterfaceType);
					printf("Channel Count: %u\n",			info.ChannelCount);
					printf("DDC Type Count: %u\n",			info.DDCTypeCount);			
					puts("-----------------------------------");
					puts("-----------------------------------");
					SetCallbacks(hDevice,&Callbacks,(uintptr_t)NULL);
					printf("Starting up device... ");
					if(!SetPower(hDevice,1)){
						printf("Error starting device. Error number = %d\n",errno);
						return 0;
					}
					puts("Success.\n");
					printf("Starting up IF-sender... ");
					if(!StartIF(hDevice,300)){
						printf("Error starting up IF-sender. Error number = %d\n",errno);
						return 0;
					}
					puts("Success.\n");
					printf("Setting up DDC type... ");
					if(!SetDDC1(hDevice,12)){
						printf("Error of setting DDC type. Error number = %d\n",errno);
						return 0;
					}
					puts("Success.\n");
					printf("Starting up DDC1... ");
					if(!StartDDC1(hDevice,NOC)){
						printf("Error starting up DDC. Error number = %d\n",errno);
						return 0;
					}
					puts("Success.\n");
					printf("Setting up frequency for DDC1... ");
					if(!SetDDC1Frequency(hDevice,freq)){
						printf("Error of setting frequency of DDC1. Error number = %d\n",errno);
						return 0;
					}
					int setFreq;
					if(!GetDDC1Frequency(hDevice,&setFreq)){
						printf("Error of getting frequency of DDC1. Error number = %d\n",errno);
						return 0;
					}
					printf("DDC1 frequency was set %dHz\n\n",setFreq);
					clock_gettime(CLOCK_REALTIME,&tt);
					StartTime=tt.tv_sec;
					StartTime*=1000000000;
					StartTime+=tt.tv_nsec;
					
					printf("Starting up DDC2... ");
					if(!StartDDC2(hDevice,0,NOC)){
						printf("Error starting up DDC2. Error number = %d\n",errno);
						return 0;
					}
					puts("Success.\n");
					printf("Setting up frequency of DDC2... ");
					if(SetDDC2Frequency(hDevice,0,freq2)==0){
						printf("Error of setting frequency of DDC2. Error number = %d\n",errno);
						return 0;
					}
					int setFreq2;
					if(!GetDDC2Frequency(hDevice,0,&setFreq2)){
						printf("Error of getting frequency of DDC2. Error number = %d\n",errno);
						return 0;
					}
					printf("DDC2 frequency was set %dHz\n\n",setFreq2);
					while(1){
						
					}
					clock_gettime(CLOCK_REALTIME,&tt);
					unsigned long long int t=tt.tv_sec;
					t*=1000000000;
					t+=tt.tv_nsec;
					t-=StartTime;
					t/=1000000;
					puts("Stopping IF");
					StopIF(hDevice);
					puts("Stopping Audio");
					StopAudio(hDevice,0);
					puts("Stopping DDC2");
					StopDDC2(hDevice,0);
					puts("Stopping DDC1");
					StopDDC1(hDevice);
					puts("Closing the device");
					CloseDevice(hDevice);
					puts("Device closed");		
				}else printf("Failed to open device. Error code=%d\n",errno);
			break;
		}
		dlclose(API);
    }else printf("Can't open API: error number - %d",errno);
	return 0;
}
double window[FFT_SIZE];
void _fft(cplx buf[], cplx out[],int step){
	int i;
	if (step < FFT_SIZE) {
		_fft(out,buf,step*2);
		_fft(out+step,buf+step,step*2);
		for (i=0;i<FFT_SIZE;i+=2*step) {
			cplx t=cexp(-1*I*M_PI*i/FFT_SIZE)*out[i+step];
			buf[i/2]=out[i]+t;
			buf[(i+FFT_SIZE)/2]=out[i]-t;
		}
	}
}
void fft(cplx buf[],bool copyback){
	int i;
	cplx out[FFT_SIZE];
	for (i=0;i<FFT_SIZE;i++) out[i] = buf[i]*window[i];
	_fft(buf,out,1);
	if (copyback) for (i=0;i<FFT_SIZE;i++) buf[i]=out[i];
}
void Translate(cplx fftbuff[],char *outbuff,bool invert,int *min,int *max){
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
