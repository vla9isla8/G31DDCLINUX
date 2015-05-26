/* ***********************************************************************/
/*                                                                       */
/*                       WiNRADiO G31DDC Excalibur                       */
/*               C/C++ header file for G31DDC receiver API               */
/*           (C) Copyright 2014 RADIXON. All rights reserved.            */
/*                                                                       */
/* ***********************************************************************/

#ifndef __G31DDCAPI_H__
#define __G31DDCAPI_H__

#include <stdint.h>

#pragma pack(push,1)

#define G31DDC_CLASS_ID_DEVICE_ENUMERATOR   0x35001
#define G31DDC_CLASS_ID_DEVICE              0x35102

#ifndef __G3XDDC_INTERFACE__
#define __G3XDDC_INTERFACE__ 5
#define G3XDDC_INTERFACE_TYPE_PCIE          0
#define G3XDDC_INTERFACE_TYPE_USB           1
#endif //__G3XDDC_INTERFACE__

#ifndef __G3XDDC_OPEN__
#define __G3XDDC_OPEN__ 5
#define G3XDDC_OPEN_DEMO   ((char*)0)
#define G3XDDC_OPEN_FIRST  ((char*)-1l)
#endif //__G3XDDC_OPEN__

#ifndef __G3XDDC_FRONT_PANEL_LED__
#define __G3XDDC_FRONT_PANEL_LED__ 1
#define G3XDDC_FRONT_PANEL_LED_MODE_DIAG    0
#define G3XDDC_FRONT_PANEL_LED_MODE_ON      1
#define G3XDDC_FRONT_PANEL_LED_MODE_OFF     2 
#endif // __G3XDDC_FRONT_PANEL_LED__

#if !defined(__G3XDDC_MODES__) || __G3XDDC_MODES__<1
#define G3XDDC_MODE_CW               0
#define G3XDDC_MODE_AM               1
#define G3XDDC_MODE_FM               2
#define G3XDDC_MODE_LSB              4
#define G3XDDC_MODE_USB              5
#define G3XDDC_MODE_AMS              8
#endif //__G3XDDC_MODES__

#ifndef __G3XDDC_SIDE_BAND__
#define __G3XDDC_SIDE_BAND__ 5
#define G3XDDC_SIDE_BAND_LOWER         0x01
#define G3XDDC_SIDE_BAND_UPPER         0x02
#define G3XDDC_SIDE_BAND_BOTH          (G3XDDC_SIDE_BAND_LOWER|G3XDDC_SIDE_BAND_UPPER)
#endif //__G3XDDC_SIDE_BAND__


#if !defined(__G3XDDC_DEMODULATOR_PARAMS__) || __G3XDDC_DEMODULATOR_PARAMS__<1
#define G3XDDC_DEMODULATOR_PARAM_AMS_SIDE_BAND          0x00000001
#define G3XDDC_DEMODULATOR_PARAM_AMS_CAPTURE_RANGE      0x00000002
#define G3XDDC_DEMODULATOR_PARAM_CW_FREQUENCY           0x00000003

typedef struct
{
    uint32_t  Tune;   //Hz
    uint32_t  Lock;   //Hz
} G3XDDC_AMS_CAPTURE_RANGE;
#endif //__G3XDDC_DEMODULATOR_PARAMS__

#if !defined(__G3XDDC_DEMODULATOR_PARAMS__) || __G3XDDC_DEMODULATOR_PARAMS__<3
#undef __G3XDDC_DEMODULATOR_PARAMS__
#define __G3XDDC_DEMODULATOR_PARAMS__ 5
#define G3XDDC_DEMODULATOR_PARAM_DSB_SIDE_BAND          0x00000006
#define G3XDDC_DEMODULATOR_PARAM_ISB_SIDE_BAND          0x00000007
#endif //__G3XDDC_DEMODULATOR_PARAMS__

#if !defined(__G3XDDC_DEMODULATOR_STATES__) || __G3XDDC_DEMODULATOR_STATES__<1

#define G3XDDC_DEMODULATOR_STATE_AMS_LOCK               0x00000001
#define G3XDDC_DEMODULATOR_STATE_AMS_FREQUENCY          0x00000002
#define G3XDDC_DEMODULATOR_STATE_AM_DEPTH               0x00000003
#define G3XDDC_DEMODULATOR_STATE_TUNE_ERROR             0x00000004
#define G3XDDC_DEMODULATOR_STATE_FM_DEVIATION           0x00000006

#endif //__G3XDDC_DEMODULATOR_STATES__

#if !defined(__G3XDDC_DEMODULATOR_STATES__) || __G3XDDC_DEMODULATOR_STATES__<3

#undef __G3XDDC_DEMODULATOR_STATES__
#define __G3XDDC_DEMODULATOR_STATES__ 5

#define G3XDDC_DEMODULATOR_STATE_DSB_LOCK               0x00000007
#define G3XDDC_DEMODULATOR_STATE_DSB_FREQUENCY          0x00000008

#endif //__G3XDDC_DEMODULATOR_STATES__

#define G31_AUDIO_SAMPLE_RATE                           48000

#ifndef __G3XDDC_DEVICE_STATE__
#define __G3XDDC_DEVICE_STATE__ 5

#define G3XDDC_DEVICE_STATE_ERROR_HIGH_TEMP             0x00000001

#endif //__G3XDDC_DEVICE_STATE__

typedef int (*G31DDCAPI_CREATE_INSTANCE)(uint32_t Code,void **Intf);

typedef struct
{
    char        DevicePath[64];
    uint8_t        InterfaceType;
    char        SerialNumber[9];
    int16_t        HWVersion;
    int16_t        FWVersion;
    uint8_t        EEPROMVersion;
    uint32_t      Flags;
    uint32_t      ChannelCount;
    uint32_t      DDCTypeCount;
} G31DDC_DEVICE_INFO;

#ifndef __G3XDDC_DDC_INFO__
#define __G3XDDC_DDC_INFO__ 5

typedef struct
{
    uint32_t      SampleRate;
    uint32_t      Bandwidth;
    uint32_t      BitsPerSample;
} G3XDDC_DDC_INFO;

#endif //__G3XDDC_DDC_INFO__

#ifdef __cplusplus

#define G31DDC_DECLARE_INTERFACE(__name__) struct __name__
#define G31DDC_THIS
#define G31DDC_METHOD(__type__,__method__,__params__) virtual __type__ __method__ __params__=0
#define G31DDC_METHOD_VOID(__type__,__method__) virtual __type__ __method__(void)=0
#else //__cplusplus
#define G31DDC_THIS struct G31DDC_INTERFACE *This,
#define G31DDC_DECLARE_INTERFACE(__name__) typedef struct __name__ { const struct __name__##Vtbl *lpVtbl;} __name__;\
    typedef const struct __name__##Vtbl __name__##Vtbl;struct __name__##Vtbl
#define G31DDC_METHOD(__type__,__method__,__params__) __type__ (*__method__)__params__
#define G31DDC_METHOD_VOID(__type__,__method__) __type__ (*__method__)(struct G31DDC_INTERFACE *This)
#endif //__cplusplus

#undef G31DDC_INTERFACE
#define G31DDC_INTERFACE IG31DDCDeviceEnumerator

G31DDC_DECLARE_INTERFACE(IG31DDCDeviceEnumerator)
{
    G31DDC_METHOD_VOID(uint32_t,AddRef);
    G31DDC_METHOD_VOID(uint32_t,Release);

    G31DDC_METHOD_VOID(int,Enumerate);
    G31DDC_METHOD_VOID(uint32_t,GetCount);
    G31DDC_METHOD(int,GetDeviceInfo,(G31DDC_THIS uint32_t DeviceIndex,G31DDC_DEVICE_INFO *DeviceInfo,uint32_t BufferLength));
};


#undef G31DDC_INTERFACE
#define G31DDC_INTERFACE IG31DDCDeviceCallback

struct IG31DDCDevice;

G31DDC_DECLARE_INTERFACE(IG31DDCDeviceCallback)
{
    G31DDC_METHOD(void,G31DDC_IFCallback,(G31DDC_THIS struct IG31DDCDevice *Device,const int16_t *Buffer,uint32_t NumberOfSamples,uint16_t MaxADCAmplitude,uint32_t ADCSampleRate));
    G31DDC_METHOD(void,G31DDC_DDC1StreamCallback,(G31DDC_THIS struct IG31DDCDevice *Device,const void *Buffer,uint32_t NumberOfSamples,uint32_t BitsPerSample));
    G31DDC_METHOD(int,G31DDC_DDC1PlaybackStreamCallback,(G31DDC_THIS struct IG31DDCDevice *Device,void *Buffer,uint32_t NumberOfSamples,uint32_t BitsPerSample));
    G31DDC_METHOD(void,G31DDC_DDC2StreamCallback,(G31DDC_THIS struct IG31DDCDevice *Device,uint32_t Channel,const float *Buffer,uint32_t NumberOfSamples));
    G31DDC_METHOD(void,G31DDC_DDC2PreprocessedStreamCallback,(G31DDC_THIS struct IG31DDCDevice *Device,uint32_t Channel,const float *Buffer,uint32_t NumberOfSamples,float SlevelPeak,float SlevelRMS));
    G31DDC_METHOD(void,G31DDC_AudioStreamCallback,(G31DDC_THIS struct IG31DDCDevice *Device,uint32_t Channel,uint32_t Type,const float *Buffer,uint32_t NumberOfSamples));
    G31DDC_METHOD(int,G31DDC_AudioPlaybackStreamCallback,(G31DDC_THIS struct IG31DDCDevice *Device,uint32_t Channel,float *Buffer,uint32_t NumberOfSamples));
};

#undef G31DDC_INTERFACE
#define G31DDC_INTERFACE IG31DDCDevice

G31DDC_DECLARE_INTERFACE(IG31DDCDevice)
{
    G31DDC_METHOD_VOID(uint32_t,AddRef);
    G31DDC_METHOD_VOID(uint32_t,Release);

    G31DDC_METHOD(int,Open,(G31DDC_THIS const char *DevicePath));
    G31DDC_METHOD_VOID(void,Close);

    G31DDC_METHOD_VOID(int,IsOpen);
    G31DDC_METHOD_VOID(int,IsConnected);

    G31DDC_METHOD(void,SetCallback,(G31DDC_THIS IG31DDCDeviceCallback *Callback));
    G31DDC_METHOD_VOID(IG31DDCDeviceCallback*,GetCallback);

    G31DDC_METHOD(int,GetDeviceInfo,(G31DDC_THIS G31DDC_DEVICE_INFO *Info,uint32_t BufferLength));

    G31DDC_METHOD(int,SetLED,(G31DDC_THIS uint32_t LEDMode));
    G31DDC_METHOD(int,GetLED,(G31DDC_THIS uint32_t *LEDMode)); 

    G31DDC_METHOD(int,SetPower,(G31DDC_THIS int Power));
    G31DDC_METHOD(int,GetPower,(G31DDC_THIS int *Power));

    G31DDC_METHOD(int,SetSpreading,(G31DDC_THIS int Enabled));
    G31DDC_METHOD(int,GetSpreading,(G31DDC_THIS int *Enabled));
    G31DDC_METHOD(int,SetDithering,(G31DDC_THIS int Enabled));
    G31DDC_METHOD(int,GetDithering,(G31DDC_THIS int *Enabled));
    G31DDC_METHOD(int,SetMWFilter,(G31DDC_THIS int Enabled));
    G31DDC_METHOD(int,GetMWFilter,(G31DDC_THIS int *Enabled));

    G31DDC_METHOD(int,SetAttenuator,(G31DDC_THIS uint32_t Attenuator));
    G31DDC_METHOD(int,GetAttenuator,(G31DDC_THIS uint32_t *Attenuator));

    G31DDC_METHOD(int,SetInverted,(G31DDC_THIS int Inverted));
    G31DDC_METHOD(int,GetInverted,(G31DDC_THIS int *Inverted));

    G31DDC_METHOD(int,SetADCNoiseBlanker,(G31DDC_THIS int Enabled));
    G31DDC_METHOD(int,GetADCNoiseBlanker,(G31DDC_THIS int *Enabled));
    G31DDC_METHOD(int,SetADCNoiseBlankerThreshold,(G31DDC_THIS uint16_t Threshold));
    G31DDC_METHOD(int,GetADCNoiseBlankerThreshold,(G31DDC_THIS uint16_t *Threshold));

    G31DDC_METHOD(int,StartIF,(G31DDC_THIS uint16_t Period));
    G31DDC_METHOD_VOID(int,StopIF);

    G31DDC_METHOD(int,GetDDCInfo,(G31DDC_THIS uint32_t DDCIndex,G3XDDC_DDC_INFO *Info));
    G31DDC_METHOD(int,GetDDC1Count,(G31DDC_THIS uint32_t *Count));
    G31DDC_METHOD(int,SetDDC1,(G31DDC_THIS uint32_t DDCIndex));
    G31DDC_METHOD(int,GetDDC1,(G31DDC_THIS uint32_t *DDCTypeIndex, G3XDDC_DDC_INFO *DDCInfo));

    G31DDC_METHOD(int,StartDDC1,(G31DDC_THIS uint32_t SamplesPerBuffer));
    G31DDC_METHOD(int,StopDDC1,());

    G31DDC_METHOD(int,StartDDC1Playback,(G31DDC_THIS uint32_t SamplesPerBuffer,uint32_t BitsPerSample));
    G31DDC_METHOD(int,PauseDDC1Playback,());
    G31DDC_METHOD(int,ResumeDDC1Playback,());

    G31DDC_METHOD(int,SetDDC1Frequency,(G31DDC_THIS uint32_t Frequency));
    G31DDC_METHOD(int,GetDDC1Frequency,(G31DDC_THIS uint32_t *Frequency));

    G31DDC_METHOD(int,GetDDC2,(G31DDC_THIS uint32_t *DDCTypeIndex,G3XDDC_DDC_INFO *DDCInfo));

    G31DDC_METHOD(int,StartDDC2,(G31DDC_THIS uint32_t Channel,uint32_t SamplesPerBuffer));
    G31DDC_METHOD(int,StopDDC2,(G31DDC_THIS uint32_t Channel));

    G31DDC_METHOD(int,SetDDC2Frequency,(G31DDC_THIS uint32_t Channel,int32_t Frequency));
    G31DDC_METHOD(int,GetDDC2Frequency,(G31DDC_THIS uint32_t Channel,int32_t *Frequency));

    G31DDC_METHOD(int,SetDDC2NoiseBlanker,(G31DDC_THIS uint32_t Channel,int Enabled));
    G31DDC_METHOD(int,GetDDC2NoiseBlanker,(G31DDC_THIS uint32_t Channel,int *Enabled));
    G31DDC_METHOD(int,SetDDC2NoiseBlankerThreshold,(G31DDC_THIS uint32_t Channel,double Threshold));
    G31DDC_METHOD(int,GetDDC2NoiseBlankerThreshold,(G31DDC_THIS uint32_t Channel,double *Threshold));
    G31DDC_METHOD(int,GetDDC2NoiseBlankerExcessValue,(G31DDC_THIS uint32_t Channel,double *Value));

    G31DDC_METHOD(int,GetSignalLevel,(G31DDC_THIS uint32_t Channel,float *Peak,float *RMS));

    G31DDC_METHOD(int,SetAGC,(G31DDC_THIS uint32_t Channel,int Enabled));
    G31DDC_METHOD(int,GetAGC,(G31DDC_THIS uint32_t Channel,int *Enabled));
    G31DDC_METHOD(int,SetAGCParams,(G31DDC_THIS uint32_t Channel,double AttackTime,double DecayTime,double ReferenceLevel));
    G31DDC_METHOD(int,GetAGCParams,(G31DDC_THIS uint32_t Channel,double *AttackTime,double *DecayTime,double *ReferenceLevel));
    G31DDC_METHOD(int,SetMaxAGCGain,(G31DDC_THIS uint32_t Channel,double MaxGain));
    G31DDC_METHOD(int,GetMaxAGCGain,(G31DDC_THIS uint32_t Channel,double *MaxGain));
    G31DDC_METHOD(int,SetGain,(G31DDC_THIS uint32_t Channel,double Gain));
    G31DDC_METHOD(int,GetGain,(G31DDC_THIS uint32_t Channel,double *Gain));
    G31DDC_METHOD(int,GetCurrentGain,(G31DDC_THIS uint32_t Channel,double *Gain));

    G31DDC_METHOD(int,SetNotchFilter,(G31DDC_THIS uint32_t Channel,uint32_t FilterIndex,int Enabled));
    G31DDC_METHOD(int,GetNotchFilter,(G31DDC_THIS uint32_t Channel,uint32_t FilterIndex,int *Enabled));
    G31DDC_METHOD(int,SetNotchFilterFrequency,(G31DDC_THIS uint32_t Channel,uint32_t FilterIndex,int32_t Frequency));
    G31DDC_METHOD(int,GetNotchFilterFrequency,(G31DDC_THIS uint32_t Channel,uint32_t FilterIndex,int32_t *Frequency));
    G31DDC_METHOD(int,SetNotchFilterBandwidth,(G31DDC_THIS uint32_t Channel,uint32_t FilterIndex,uint32_t Bandwidth));
    G31DDC_METHOD(int,GetNotchFilterBandwidth,(G31DDC_THIS uint32_t Channel,uint32_t FilterIndex,uint32_t *Bandwidth));
    G31DDC_METHOD(int,SetNotchFilterLength,(G31DDC_THIS uint32_t Channel,uint32_t FilterIndex,uint32_t Length));
    G31DDC_METHOD(int,GetNotchFilterLength,(G31DDC_THIS uint32_t Channel,uint32_t FilterIndex,uint32_t *Length));

    G31DDC_METHOD(int,SetDemodulatorMode,(G31DDC_THIS uint32_t Channel,uint32_t Mode));
    G31DDC_METHOD(int,GetDemodulatorMode,(G31DDC_THIS uint32_t Channel,uint32_t *Mode));
    G31DDC_METHOD(int,SetDemodulatorFrequency,(G31DDC_THIS uint32_t Channel,int32_t Frequency));
    G31DDC_METHOD(int,GetDemodulatorFrequency,(G31DDC_THIS uint32_t Channel,int32_t *Frequency));
    G31DDC_METHOD(int,SetDemodulatorFilterBandwidth,(G31DDC_THIS uint32_t Channel,uint32_t Bandwidth));
    G31DDC_METHOD(int,GetDemodulatorFilterBandwidth,(G31DDC_THIS uint32_t Channel,uint32_t *Bandwidth));
    G31DDC_METHOD(int,SetDemodulatorFilterShift,(G31DDC_THIS uint32_t Channel,int32_t Shift));
    G31DDC_METHOD(int,GetDemodulatorFilterShift,(G31DDC_THIS uint32_t Channel,int32_t *Shift));
    G31DDC_METHOD(int,SetDemodulatorFilterLength,(G31DDC_THIS uint32_t Channel,uint32_t Length));
    G31DDC_METHOD(int,GetDemodulatorFilterLength,(G31DDC_THIS uint32_t Channel,uint32_t *Length));

    G31DDC_METHOD(int,SetDemodulatorParam,(G31DDC_THIS uint32_t Channel,uint32_t Code,const void *Buffer,uint32_t BufferSize));
    G31DDC_METHOD(int,GetDemodulatorParam,(G31DDC_THIS uint32_t Channel,uint32_t Code,void *Buffer,uint32_t BufferSize));
    G31DDC_METHOD(int,GetDemodulatorState,(G31DDC_THIS uint32_t Channel,uint32_t Code,void *Buffer,uint32_t BufferSize));

    G31DDC_METHOD(int,StartAudio,(G31DDC_THIS uint32_t Channel,uint32_t SamplesPerBuffer));
    G31DDC_METHOD(int,StopAudio,(G31DDC_THIS uint32_t Channel));

    G31DDC_METHOD(int,StartAudioPlayback,(G31DDC_THIS uint32_t Channel,uint32_t SamplesPerBuffer));
    G31DDC_METHOD(int,PauseAudioPlayback,(G31DDC_THIS uint32_t Channel));
    G31DDC_METHOD(int,ResumeAudioPlayback,(G31DDC_THIS uint32_t Channel));

    G31DDC_METHOD(int,SetAudioFilter,(G31DDC_THIS uint32_t Channel,int Enabled));
    G31DDC_METHOD(int,GetAudioFilter,(G31DDC_THIS uint32_t Channel,int *Enabled));
    G31DDC_METHOD(int,SetAudioFilterParams,(G31DDC_THIS uint32_t Channel,uint32_t CutOffLow,uint32_t CutOffHigh,double Deemphasis));
    G31DDC_METHOD(int,GetAudioFilterParams,(G31DDC_THIS uint32_t Channel,uint32_t *CutOffLow,uint32_t *CutOffHigh,double *Deemphasis));
    G31DDC_METHOD(int,SetAudioFilterLength,(G31DDC_THIS uint32_t Channel,uint32_t Length));
    G31DDC_METHOD(int,GetAudioFilterLength,(G31DDC_THIS uint32_t Channel,uint32_t *Length));
    G31DDC_METHOD(int,SetAudioGain,(G31DDC_THIS uint32_t Channel,double Gain));
    G31DDC_METHOD(int,GetAudioGain,(G31DDC_THIS uint32_t Channel,double *Gain));
    G31DDC_METHOD(int,SetVolume,(G31DDC_THIS uint32_t Channel,uint8_t Volume));
    G31DDC_METHOD(int,GetVolume,(G31DDC_THIS uint32_t Channel,uint8_t *Volume));
    G31DDC_METHOD(int,SetMute,(G31DDC_THIS uint32_t Channel,int Mute));
    G31DDC_METHOD(int,GetMute,(G31DDC_THIS uint32_t Channel,int *Mute));

    G31DDC_METHOD(int,GetSpectrumCompensation,(G31DDC_THIS uint32_t CenterFrequency,uint32_t Bandwidth,float *Buffer,uint32_t Count));

    G31DDC_METHOD(int,SetFrequency,(G31DDC_THIS uint32_t Channel,uint32_t Frequency));
    G31DDC_METHOD(int,GetFrequency,(G31DDC_THIS uint32_t Channel,uint32_t *Frequency));

};


typedef void (*G31DDC_IF_CALLBACK)(const int16_t *Buffer,uint32_t NumberOfSamples,uint16_t MaxADCAmplitude,uint32_t ADCSampleRate,uintptr_t UserData);
typedef void (*G31DDC_DDC1_STREAM_CALLBACK)(const void *Buffer,uint32_t NumberOfSamples,uint32_t BitsPerSample,uintptr_t UserData);
typedef int (*G31DDC_DDC1_PLAYBACK_STREAM_CALLBACK)(void *Buffer,uint32_t NumberOfSamples,uint32_t BitsPerSample,uintptr_t UserData);
typedef void (*G31DDC_DDC2_STREAM_CALLBACK)(uint32_t Channel,const float *Buffer,uint32_t NumberOfSamples,uintptr_t UserData);
typedef void (*G31DDC_DDC2_PREPROCESSED_STREAM_CALLBACK)(uint32_t Channel,const float *Buffer,uint32_t NumberOfSamples,float SlevelPeak,float SlevelRMS,uintptr_t UserData);
typedef void (*G31DDC_AUDIO_STREAM_CALLBACK)(uint32_t Channel,uint32_t Type,const float *Buffer,uint32_t NumberOfSamples,uintptr_t UserData);
typedef int (*G31DDC_AUDIO_PLAYBACK_STREAM_CALLBACK)(uint32_t Channel,float *Buffer,uint32_t NumberOfSamples,uintptr_t UserData);

typedef struct
{
    G31DDC_IF_CALLBACK                          IFCallback;
    G31DDC_DDC1_STREAM_CALLBACK                 DDC1StreamCallback;
    G31DDC_DDC1_PLAYBACK_STREAM_CALLBACK        DDC1PlaybackStreamCallback;
    G31DDC_DDC2_STREAM_CALLBACK                 DDC2StreamCallback;
    G31DDC_DDC2_PREPROCESSED_STREAM_CALLBACK    DDC2PreprocessedStreamCallback;
    G31DDC_AUDIO_STREAM_CALLBACK                AudioStreamCallback;
    G31DDC_AUDIO_PLAYBACK_STREAM_CALLBACK       AudioPlaybackStreamCallback;
} G31DDC_CALLBACKS;

typedef int (*G31DDC_GET_DEVICE_LIST)(G31DDC_DEVICE_INFO *DeviceList,uint32_t BufferSize);
typedef int (*G31DDC_OPEN_DEVICE)(const char *SerialNumber);
typedef int (*G31DDC_CLOSE_DEVICE)(int32_t Handle);
typedef int (*G31DDC_IS_DEVICE_CONNECTED)(int32_t Handle);
typedef int (*G31DDC_GET_DEVICE_INFO)(int32_t Handle,G31DDC_DEVICE_INFO *Info,uint32_t BufferLength);
typedef int (*G31DDC_SET_POWER)(int32_t Handle,int Power);
typedef int (*G31DDC_GET_POWER)(int32_t Handle,int *Power);
typedef int (*G31DDC_SET_LED)(int32_t Handle,uint32_t LEDMode);
typedef int (*G31DDC_GET_LED)(int32_t Handle,uint32_t *LEDMode); 
typedef int (*G31DDC_SET_DITHERING)(int32_t Handle,int Enabled);
typedef int (*G31DDC_GET_DITHERING)(int32_t Handle,int *Enabled);
typedef int (*G31DDC_SET_ATTENUATOR)(int32_t Handle,uint32_t Attenuator);
typedef int (*G31DDC_GET_ATTENUATOR)(int32_t Handle,uint32_t *Attenuator);
typedef int (*G31DDC_SET_MWFILTER)(int32_t Handle,int Enabled);
typedef int (*G31DDC_GET_MWFILTER)(int32_t Handle,int *Enabled);
typedef int (*G31DDC_SET_ADC_NOISE_BLANKER)(int32_t Handle,int Enabled);
typedef int (*G31DDC_GET_ADC_NOISE_BLANKER)(int32_t Handle,int *Enabled);
typedef int (*G31DDC_SET_ADC_NOISE_BLANKER_THRESHOLD)(int32_t Handle,uint16_t Threshold);
typedef int (*G31DDC_GET_ADC_NOISE_BLANKER_THRESHOLD)(int32_t Handle,uint16_t *Threshold);
typedef int (*G31DDC_SET_FREQUENCY)(int32_t Handle,uint32_t Channel,uint64_t Frequency);
typedef int (*G31DDC_GET_FREQUENCY)(int32_t Handle,uint32_t Channel,uint64_t *Frequency);
typedef int (*G31DDC_START_IF)(int32_t Handle,uint16_t Period);
typedef int (*G31DDC_STOP_IF)(int32_t Handle);
typedef int (*G31DDC_GET_DDC_INFO)(uint32_t DDCIndex,G3XDDC_DDC_INFO *Info);
typedef int (*G31DDC_GET_DDC1_COUNT)(int32_t Handle,uint32_t *Count);
typedef int (*G31DDC_SET_DDC1)(int32_t Handle,uint32_t DDCIndex);
typedef int (*G31DDC_GET_DDC1)(int32_t Handle,uint32_t *DDCIndex,G3XDDC_DDC_INFO *DDCInfo);
typedef int (*G31DDC_START_DDC1)(int32_t Handle,uint32_t SamplesPerBuffer);
typedef int (*G31DDC_STOP_DDC1)(int32_t Handle);
typedef int (*G31DDC_SET_DDC1_FREQUENCY)(int32_t Handle,int32_t Frequency);
typedef int (*G31DDC_GET_DDC1_FREQUENCY)(int32_t Handle,int32_t *Frequency);
typedef int (*G31DDC_START_DDC1_PLAYBACK)(int32_t Handle,uint32_t SamplesPerBuffer,uint32_t BitsPerSample);
typedef int (*G31DDC_PAUSE_DDC1_PLAYBACK)(int32_t Handle);
typedef int (*G31DDC_RESUME_DDC1_PLAYBACK)(int32_t Handle);

typedef int (*G31DDC_GET_DDC2)(int32_t Handle,uint32_t *DDCTypeIndex,G3XDDC_DDC_INFO *DDCInfo);
typedef int (*G31DDC_START_DDC2)(int32_t Handle,uint32_t Channel,uint32_t SamplesPerBuffer);
typedef int (*G31DDC_STOP_DDC2)(int32_t Handle,uint32_t Channel);
typedef int (*G31DDC_SET_DDC2_FREQUENCY)(int32_t Handle,uint32_t Channel,int32_t Frequency);
typedef int (*G31DDC_GET_DDC2_FREQUENCY)(int32_t Handle,uint32_t Channel,int32_t *Frequency);

typedef int (*G31DDC_SET_AGC)(int32_t Handle,uint32_t Channel,int Enabled);
typedef int (*G31DDC_GET_AGC)(int32_t Handle,uint32_t Channel,int *Enabled);
typedef int (*G31DDC_SET_AGC_PARAMS)(int32_t Handle,uint32_t Channel,double AttackTime,double DecayTime,double ReferenceLevel);
typedef int (*G31DDC_GET_AGC_PARAMS)(int32_t Handle,uint32_t Channel,double *AttackTime,double *DecayTime,double *ReferenceLevel);
typedef int (*G31DDC_SET_MAX_AGC_GAIN)(int32_t Handle,uint32_t Channel,double MaxGain);
typedef int (*G31DDC_GET_MAX_AGC_GAIN)(int32_t Handle,uint32_t Channel,double *MaxGain);
typedef int (*G31DDC_SET_GAIN)(int32_t Handle,uint32_t Channel,double Gain);
typedef int (*G31DDC_GET_GAIN)(int32_t Handle,uint32_t Channel,double *Gain);
typedef int (G31DDC_GET_CURRENT_GAIN)(int32_t Handle,uint32_t Channel,double *Gain);

typedef int (*G31DDC_SET_NOTCH_FILTER)(int32_t Handle,uint32_t Channel,uint32_t FilterIndex,int Enabled);
typedef int (*G31DDC_GET_NOTCH_FILTER)(int32_t Handle,uint32_t Channel,uint32_t FilterIndex,int *Enabled);
typedef int (*G31DDC_SET_NOTCH_FILTER_FREEQUENCY)(int32_t Handle,uint32_t Channel,uint32_t FilterIndex,int32_t Frequency);
typedef int (*G31DDC_GET_NOTCH_FILTER_FREEQUENCY)(int32_t Handle,uint32_t Channel,uint32_t FilterIndex,int32_t *Frequency);
typedef int (*G31DDC_SET_NOTCH_FILTER_BANDWIDTH)(int32_t Handle,uint32_t Channel,uint32_t FilterIndex,uint32_t Bandwidth);
typedef int (*G31DDC_GET_NOTCH_FILTER_BANDWIDTH)(int32_t Handle,uint32_t Channel,uint32_t FilterIndex,uint32_t *Bandwidth);
typedef int (*G31DDC_SET_NOTCH_FILTER_LENGTH)(int32_t Handle,uint32_t Channel,uint32_t FilterIndex,uint32_t Length);
typedef int (*G31DDC_GET_NOTCH_FILTER_LENGTH)(int32_t Handle,uint32_t Channel,uint32_t FilterIndex,uint32_t *Length);

typedef int (*G31DDC_SET_DEMODULATOR_FREQUENCY)(int32_t Handle,uint32_t Channel,int32_t Frequency);
typedef int (*G31DDC_GET_DEMODULATOR_FREQUENCY)(int32_t Handle,uint32_t Channel,int32_t *Frequency);
typedef int (*G31DDC_SET_DEMODULATOR_FILTER_BANDWIDTH)(int32_t Handle,uint32_t Channel,uint32_t Bandwidth);
typedef int (*G31DDC_GET_DEMODULATOR_FILTER_BANDWIDTH)(int32_t Handle,uint32_t Channel,uint32_t *Bandwidth);
typedef int (*G31DDC_SET_DEMODULATOR_FILTER_SHIFT)(int32_t Handle,uint32_t Channel,int32_t Shift);
typedef int (*G31DDC_GET_DEMODULATOR_FILTER_SHIFT)(int32_t Handle,uint32_t Channel,int32_t *Shift);
typedef int (*G31DDC_SET_DEMODULATOR_FILTER_LENGTH)(int32_t Handle,uint32_t Channel,uint32_t Length);
typedef int (*G31DDC_GET_DEMODULATOR_FILTER_LENGTH)(int32_t Handle,uint32_t Channel,uint32_t *Length);
typedef int (*G31DDC_SET_DEMODULATOR_MODE)(int32_t Handle,uint32_t Channel,uint32_t Mode);
typedef int (*G31DDC_GET_DEMODULATOR_MODE)(int32_t Handle,uint32_t Channel,uint32_t *Mode);
typedef int (*G31DDC_SET_DEMODULATOR_PARAM)(int32_t Handle,uint32_t Channel,uint32_t Code,const void *Buffer,uint32_t BufferSize);
typedef int (*G31DDC_GET_DEMODULATOR_PARAM)(int32_t Handle,uint32_t Channel,uint32_t Code,void *Buffer,uint32_t BufferSize);
typedef int (*G31DDC_GET_DEMODULATOR_STATE)(int32_t Handle,uint32_t Channel,uint32_t Code,void *Buffer,uint32_t BufferSize);

typedef int (*G31DDC_GET_AUDIO_SAMPLE_RATE_COUNT)(int32_t Handle,uint32_t *Count);
typedef int (*G31DDC_GET_AUDIO_SAMPLE_RATE)(int32_t Handle,uint32_t SampleRateIndex,uint32_t *SampleRate);
typedef int (*G31DDC_START_AUDIO)(int32_t Handle,uint32_t Channel,uint32_t SamplesPerBuffer);
typedef int (*G31DDC_STOP_AUDIO)(int32_t Handle,uint32_t Channel);
typedef int (*G31DDC_SET_AUDIO)(int32_t Handle,uint32_t Channel,uint32_t SampleRateIndex);
typedef int (*G31DDC_GET_AUDIO)(int32_t Handle,uint32_t Channel,uint32_t *SampleRateIndex);
typedef int (*G31DDC_START_AUDIO_PLAYBACK)(int32_t Handle,uint32_t Channel,uint32_t SamplesPerBuffer);
typedef int (*G31DDC_PAUSE_AUDIO_PLAYBACK)(int32_t Handle,uint32_t Channel);
typedef int (*G31DDC_RESUME_AUDIO_PLAYBACK)(int32_t Handle,uint32_t Channel);
typedef int (*G31DDC_SET_AUDIO_FILTER)(int32_t Handle,uint32_t Channel,int Enabled);
typedef int (*G31DDC_GET_AUDIO_FILTER)(int32_t Handle,uint32_t Channel,int *Enabled);
typedef int (*G31DDC_SET_AUDIO_FILTER_PARAMS)(int32_t Handle,uint32_t Channel,uint32_t CutOffLow,uint32_t CutOffHigh,double Deemphasis);
typedef int (*G31DDC_GET_AUDIO_FILTER_PARAMS)(int32_t Handle,uint32_t Channel,uint32_t *CutOffLow,uint32_t *CutOffHigh,double *Deemphasis);
typedef int (*G31DDC_SET_AUDIO_FILTER_LENGTH)(int32_t Handle,uint32_t Channel,uint32_t Length);
typedef int (*G31DDC_GET_AUDIO_FILTER_LENGTH)(int32_t Handle,uint32_t Channel,uint32_t *Length);
typedef int (*G31DDC_SET_AUDIO_GAIN)(int32_t Handle,uint32_t Channel,double Gain);
typedef int (*G31DDC_GET_AUDIO_GAIN)(int32_t Handle,uint32_t Channel,double *Gain);
typedef int (*G31DDC_SET_VOLUME)(int32_t Handle,uint32_t Channel,uint8_t Volume);
typedef int (*G31DDC_GET_VOLUME)(int32_t Handle,uint32_t Channel,uint8_t *Volume);
typedef int (*G31DDC_SET_MUTE)(int32_t Handle,uint32_t Channel,int Mute);
typedef int (*G31DDC_GET_MUTE)(int32_t Handle,uint32_t Channel,int *Mute);

typedef int (*G31DDC_GET_SPECTRUM_COMPENSATION)(int32_t Handle,uint32_t CenterFrequency,uint32_t Bandwidth,float *Buffer,uint32_t Count);

typedef int (*G31DDC_SET_INVERTED)(int32_t Handle,int Inverted);
typedef int (*G31DDC_GET_INVERTED)(int32_t Handle,int *Inverted);

typedef int (*G31DDC_SET_DDC2_NOISE_BLANKER)(int32_t Handle,uint32_t Channel,int Enabled);
typedef int (*G31DDC_GET_DDC2_NOISE_BLANKER)(int32_t Handle,uint32_t Channel,int *Enabled);
typedef int (*G31DDC_SET_DDC2_NOISE_BLANKER_THRESHOLD)(int32_t Handle,uint32_t Channel,double Threshold);
typedef int (*G31DDC_GET_DDC2_NOISE_BLANKER_THRESHOLD)(int32_t Handle,uint32_t Channel,double *Threshold);
typedef int (*G31DDC_GET_DDC2_NOISE_BLANKER_EXCESS_VALUE)(int32_t Handle,uint32_t Channel,double *Value);

typedef int (*G31DDC_GET_SIGNAL_LEVEL)(int32_t Handle,uint32_t Channel,float *Peak,float *RMS);

typedef int (*G31DDC_SET_CALLBACKS)(int32_t Handle,const G31DDC_CALLBACKS *Callbacks,uintptr_t UserData);

#pragma pack(pop)

#endif // __G31DDCAPI_H__
