#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <curses.h>         	// requires curses library
#include "g31ddcapi.h"
#include <time.h>
#include <sys/stat.h>
//#include <pulse/simple.h>		// requires pulse library
//#include <pulse/error.h>
// FFT stuff

// counters
unsigned long long int StartTime;

#define DATAFILE "tdata.txt"
#define NOC 1024
FILE *fp;
struct stat filestat;
float P_dBm,V_RMS;
int writetofile = 0;
double maxdBm=-100000,mindBm=0;
void DDC2StreamCallback(uint32_t Channel,const float *Buffer,uint32_t NumberOfSamples,uintptr_t UserData) {
	int i = 0;
	Channel = 0;
	double a = 0;
	maxdBm=-100000,mindBm=0;
	NumberOfSamples = NOC;
	for (i=0;i<NOC;i+=2) {
		if(writetofile){
			fprintf(fp,"%.32f;%.32f\n",Buffer[i],Buffer[i+1]);
		}
		a=10*log10((Buffer[i]*Buffer[i]+Buffer[i+1]*Buffer[i+1])/50) + 30;
		if (a>maxdBm) maxdBm=a;
		if (a<mindBm) mindBm=a;	
	}
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
	uint32_t ddc1_type;
	G3XDDC_DDC_INFO ddc1_info;
	uint32_t ddc2_type;
	G3XDDC_DDC_INFO ddc2_info;
	G31DDC_GET_SIGNAL_LEVEL GetSignalLevel;
	
	Callbacks.IFCallback=NULL;
	Callbacks.DDC1StreamCallback=NULL;
	Callbacks.DDC1PlaybackStreamCallback=NULL;
	Callbacks.DDC2StreamCallback=DDC2StreamCallback;
	Callbacks.DDC2PreprocessedStreamCallback=NULL;
	Callbacks.AudioStreamCallback=NULL;
	Callbacks.AudioPlaybackStreamCallback=NULL;
	
	fp=fopen(DATAFILE, "w");
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
		GetSignalLevel=(G31DDC_GET_SIGNAL_LEVEL)dlsym(API,"GetSignalLevel");
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
					free(List);
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
					
					printf("Getting DDC1 type... ");
					if(!GetDDC1(hDevice,&ddc1_type,&ddc1_info)){
						printf("Error of setting DDC type. Error number = %d\n",errno);
						return 0;
					}
					printf("Success.\nType of DDC1: %d\nDDC1 info:\n\tSample Rate: %d\n\tBandwidth: %d\n\tBits Per Sample %d\n",ddc1_type,ddc1_info.SampleRate,ddc1_info.Bandwidth,ddc1_info.BitsPerSample);
					
					printf("Setting up DDC1 type... ");
					if(!SetDDC1(hDevice,12)){
						printf("Error of setting DDC type. Error number = %d\n",errno);
						return 0;
					}
					puts("Success.\n");
					
					printf("Getting DDC1 type... ");
					if(!GetDDC1(hDevice,&ddc1_type,&ddc1_info)){
						printf("Error of setting DDC type. Error number = %d\n",errno);
						return 0;
					}

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
					
					printf("Getting DDC2 type... ");
					if(!GetDDC1(hDevice,&ddc2_type,&ddc2_info)){
						printf("Error of пetting DDC2 type. Error number = %d\n",errno);
						return 0;
					}

					
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
						
					printf("\E[H\E[2J");
					printf("\E[H\E[2J");
					WINDOW *w=initscr();
					cbreak();
					noecho();
					nodelay(w,TRUE);
						
					bool end=false;
					do {
						
						if (stat(DATAFILE, &filestat) == -1) {
						  /* check the value of errno */
						}
						GetDDC1Frequency(hDevice,&setFreq);
						GetDDC2Frequency(hDevice,0,&setFreq2);
						
						printf("\033[1;0HDDC1: %dkHz/%dkHz, Freq=%dkHz\t\tDDC2=%dkHz/%dkHz, Freq=%dkHz",ddc1_info.SampleRate/1000,ddc1_info.Bandwidth/1000,setFreq/1000,ddc2_info.SampleRate/1000,ddc2_info.Bandwidth/1000,setFreq2/1000);
						printf("\033[5;0HCurrent signal level [RMS]: %d dBm - %d dbm",(int)mindBm,(int)maxdBm);
						printf("\033[7;0HFILESIZE: %.3f Mb",(intmax_t) filestat.st_size/1048576.);
						usleep(10000);
						if (getch() == '\033') { // if the first value is esc
							getch(); // skip the [
							switch(getch()) { // the real value
								case 'A':
									// code for arrow up
									writetofile = (writetofile+1)&1;
									break;
								case 'B':
									// code for arrow down
									end=true;
									break;
								case 'C':
									// code for arrow right
									freq+=100000;
									SetDDC1Frequency(hDevice,freq);
									break;
								case 'D':
									// code for arrow left
									freq-=100000;
									SetDDC1Frequency(hDevice,freq);
									break;
							}
						}
					} while (!end);
					endwin();
					fclose(fp);	
						
					puts("Stopping IF");
					StopIF(hDevice);
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
