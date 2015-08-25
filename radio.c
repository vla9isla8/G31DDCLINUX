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
struct G31DDC_API{
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
	G3XDDC_DDC_INFO ddc1_info;
	G3XDDC_DDC_INFO ddc2_info;
	G31DDC_GET_SIGNAL_LEVEL GetSignalLevel;
}
FILE *fp;
struct stat filestat;
float P_dBm,V_RMS;
int writetofile = 0;
double maxdBm=-100000,mindBm=0;
void DDC2PreprocessedStreamCallback(uint32_t Channel,const float *Buffer,uint32_t NumberOfSamples,float SlevelPeak,float SlevelRMS,uintptr_t UserData) {
	int i = 0;
	Channel = 0;
	double a = 0;
	maxdBm=-100000,mindBm=0;
	NumberOfSamples = NOC;
	for (i=0;i<NOC;i+=2) {
		if(writetofile) fprintf(fp,"%.32f;%.32f\n",Buffer[i],Buffer[i+1]);
		a=10*log10((Buffer[i] * Buffer[i] + Buffer[i+1] * Buffer[i+1]) / 50) + 30;
		if (a>maxdBm) maxdBm=a;
		if (a<mindBm) mindBm=a;	
		//P_dBm = 10*log10((SlevelRMS * SlevelRMS) / 50) + 30;
	}
}
struct G31DDC_API *radio_load_api(){
	void *API;
	API=dlopen("libg31ddcapi.so",RTLD_LAZY);
	if (API) {
		struct G31DDC_API *radio_api = (struct G31DDC_API*)malloc(struct G31DDC_API);
		*radio_api.OpenDevice=(G31DDC_OPEN_DEVICE)dlsym(API,"OpenDevice");
		*radio_api.CloseDevice=(G31DDC_CLOSE_DEVICE)dlsym(API,"CloseDevice");
		*radio_api.SetPower=(G31DDC_SET_POWER)dlsym(API,"SetPower");
		*radio_api.SetDDC1=(G31DDC_SET_DDC1)dlsym(API,"SetDDC1");
		*radio_api.GetDDC2=(G31DDC_GET_DDC2)dlsym(API,"GetDDC2");
		*radio_api.GetDDC1=(G31DDC_GET_DDC1)dlsym(API,"GetDDC1");
		*radio_api.StartIF=(G31DDC_START_IF)dlsym(API,"StartIF");
		*radio_api.StopIF=(G31DDC_STOP_IF)dlsym(API,"StopIF");
		*radio_api.StopAudio=(G31DDC_STOP_AUDIO)dlsym(API,"StopAudio");
		*radio_api.StartDDC1=(G31DDC_START_DDC1)dlsym(API,"StartDDC1");
		*radio_api.StopDDC1=(G31DDC_STOP_DDC1)dlsym(API,"StopDDC1");
		*radio_api.StartDDC2=(G31DDC_START_DDC2)dlsym(API,"StartDDC2");
		*radio_api.StartAudio=(G31DDC_START_AUDIO)dlsym(API,"StartAudio");
		*radio_api.StopDDC2=(G31DDC_STOP_DDC2)dlsym(API,"StopDDC2");
		*radio_api.SetDDC2Frequency=(G31DDC_SET_DDC2_FREQUENCY)dlsym(API,"SetDDC2Frequency");
		*radio_api.SetDDC1Frequency=(G31DDC_SET_DDC1_FREQUENCY)dlsym(API,"SetDDC1Frequency");
		*radio_api.GetDDC1Frequency=(G31DDC_GET_DDC1_FREQUENCY)dlsym(API,"GetDDC1Frequency");
		*radio_api.GetDDC2Frequency=(G31DDC_GET_DDC2_FREQUENCY)dlsym(API,"GetDDC2Frequency");
		*radio_api.StartDDC1Playback=(G31DDC_START_DDC1_PLAYBACK)dlsym(API,"StartDDC1Playback");
		*radio_api.SetDemodulatorFilterBandwidth=(G31DDC_SET_DEMODULATOR_FILTER_BANDWIDTH)dlsym(API,"SetDemodulatorFilterBandwidth");
		*radio_api.SetDemodulatorMode=(G31DDC_SET_DEMODULATOR_MODE)dlsym(API,"SetDemodulatorMode");
		*radio_api.SetAGC=(G31DDC_SET_AGC)dlsym(API,"SetAGC");
		*radio_api.SetAGCParams=(G31DDC_SET_AGC_PARAMS)dlsym(API,"SetAGCParams");
		*radio_api.SetCallbacks=(G31DDC_SET_CALLBACKS)dlsym(API,"SetCallbacks");
		*radio_api.GetDeviceInfo=(G31DDC_GET_DEVICE_INFO)dlsym(API,"GetDeviceInfo");
		*radio_api.GetDeviceList=(G31DDC_GET_DEVICE_LIST)dlsym(API,"GetDeviceList");
		*radio_api.GetSignalLevel=(G31DDC_GET_SIGNAL_LEVEL)dlsym(API,"GetSignalLevel");
		dlclose(API);
		return radio_api;
	}else{
		return NULL;
	}
}
int radio_start(struct G31DDC_API *radio_api){
	
	*radio_api.Callbacks.IFCallback=NULL;
	*radio_api.Callbacks.DDC1StreamCallback=NULL;
	*radio_api.Callbacks.DDC1PlaybackStreamCallback=NULL;
	*radio_api.Callbacks.DDC2StreamCallback=NULL;
	*radio_api.Callbacks.DDC2PreprocessedStreamCallback=DDC2PreprocessedStreamCallback;
	*radio_api.Callbacks.AudioStreamCallback=NULL;
	*radio_api.Callbacks.AudioPlaybackStreamCallback=NULL;
	
	fp=fopen(DATAFILE, "w");
	int32_t hDevice=0;
	struct timespec tt;
	int freq = 10000000,freq2=0;
	uint32_t ddc1_type;
	uint32_t ddc2_type;
	int Count,i;
	Count	=	*radio_api.GetDeviceList(NULL,0);
		switch (Count){
			case -1:
				printf("Error get list of diveces: error number - %d\n",errno);
			break;
			case 0:
				printf("Devices not found\n");
			break;
			default:
				List	=	(*radio_api.G31DDC_DEVICE_INFO*)malloc(Count*sizeof(*radio_api.G31DDC_DEVICE_INFO));
				Count	=	*radio_api.GetDeviceList(List,Count*sizeof(*List));
				printf("Available devices: %d\n",Count);
				
				for(i = 0;i < Count;i++){
					printf("Device %8s \n",List[i].SerialNumber);
				}
				hDevice=*radio_api.OpenDevice(List[0].SerialNumber);
				if (hDevice) {
					free(List);
					*radio_api.G31DDC_DEVICE_INFO info;
					*radio_api.G3XDDC_DDC_INFO info1={0},info2={0};
					*radio_api.GetDeviceInfo(hDevice,&info,sizeof(info));
					*radio_api.SetCallbacks(hDevice,radio_api.Callbacks,(uintptr_t)NULL);
					if(!SetPower(hDevice,1)){
						return errno;
					}
					puts("Success.\n");
					if(!StartIF(hDevice,300)){
						return errno;
					}
					puts("Success.\n");
					if(!GetDDC1(hDevice,&ddc1_type,&ddc1_info)){
						return errno;
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
					
				}else printf("Failed to open device. Error code=%d\n",errno);
			break;
		}
}
int radio_stop(int32_t hDevice){
		fclose(fp);			
		puts("Stopping DDC2");
		StopDDC2(hDevice,0);
		puts("Stopping DDC1");
		StopDDC1(hDevice);
		puts("Stopping IF");
		StopIF(hDevice);
		puts("Closing the device");
		CloseDevice(hDevice);
		puts("Device closed");		
}
