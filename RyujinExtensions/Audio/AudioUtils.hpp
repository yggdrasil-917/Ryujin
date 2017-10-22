#pragma once


#include "../Utils/ExtensionsBase.hpp"

namespace Ryujin
{
    // Note all IDs below are in big-endian
#define WAVE_RIFF_ID        0x52494646
#define WAVE_FORMAT_ID      0x57415645
#define WAVE_SUBCHUNK1_ID   0x666d7420
#define WAVE_SUBCHUNK2_ID   0x64617461
#define WAVE_DATA_ID        0x64617461
    
    // Little endian versions
#define WAVE_RIFF_ID_LE        0x46464952
#define WAVE_FORMAT_ID_LE      0x45564157
#define WAVE_SUBCHUNK1_ID_LE   0x20746d66
#define WAVE_SUBCHUNK2_ID_LE   0x61746164
#define WAVE_DATA_ID_LE        0x61746164
    
    struct WaveHeader
    {
        uint32 chunkId;
        uint32 chunkSize;
        uint32 waveFormat;
        uint32 subChunk1Id;
        uint32 pcmHeaderLength;
        uint16 waveFormatTag;
        uint16 numChannels;
        uint32 numSamplesPerSec;
        uint32 numAvgBytesPerSec;
        uint16 numBlockAlign;
        uint16 numBitsPerSample;
    };
    
    struct ChunkHeader
    {
        uint32 dataId;
        uint32 dataLength;
    };
    
    class EXTENSIONS_API WaveInstance
    {
    private:
        uint8* waveData;
        uint32 waveDataLength;
        
        uint16 numSamplesPerSec;
        uint8 numChannels;
        uint8 numBitsPerSample;
        int32 durationInSeconds;
        
    public:
        WaveInstance();
        ~WaveInstance();
        
        bool LoadFromFile(const char* filename);
        
        FORCEINLINE uint8* GetWaveData() const { return waveData; }
        FORCEINLINE uint32 GetWaveDataLength() const { return waveDataLength; }
        
        FORCEINLINE uint16 GetNumSamplesPerSec() const { return numSamplesPerSec; }
        FORCEINLINE uint8 GetNumChannels() const { return numChannels; }
        FORCEINLINE uint8 GetNumBitsPerSample() const { return numBitsPerSample; }
        FORCEINLINE int32 GetDurationInSeconds() const { return durationInSeconds; }
    };
}
