#ifndef APP_MUSICPLAYER_H
#define APP_MUSICPLAYER_H
#include "helix.h"

typedef struct {
    /* chunk "riff" */
    char ChunkID[4];     /* "RIFF" */
    /* sub-chunk-size */
    uint32_t ChunkSize;  /* 36 + Subchunk2Size */
    /* sub-chunk-data */
    char Format[4];      /* "WAVE" */
} WAV_RIFF;

typedef struct {
    /* sub-chunk "fmt" */
    char Subchunk1ID[4];    /* "fmt " */
    /* sub-chunk-size */
    uint32_t Subchunk1Size; /* 16 for PCM */
    /* sub-chunk-data */
    uint16_t AudioFormat;   /* PCM = 1*/
    uint16_t NumChannels;   /* Mono = 1, Stereo = 2, etc. */
    uint32_t SampleRate;    /* 8000, 44100, etc. */
    uint32_t ByteRate;      /* = SampleRate * NumChannels * BitsPerSample/8 */
    uint16_t BlockAlign;    /* = NumChannels * BitsPerSample/8 */
    uint16_t BitsPerSample; /* 8bits, 16bits, etc. */
} WAV_FMT;

typedef struct  {
    /* sub-chunk "data" */
    char Subchunk2ID[4];    /* "data" */
    /* sub-chunk-size */
    uint32_t Subchunk2Size; /* data size */
    /* sub-chunk-data */
//    Data_block_t block;
} WAV_data;

void HAPP_musicPlayer_Handler(void);
void HAPP_musicPlayer_playDLSS(uint8_t* path);
void HAPP_musicPlayer_playWAV(uint8_t* path);

#endif
