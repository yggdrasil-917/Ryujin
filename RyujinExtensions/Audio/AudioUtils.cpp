#include "AudioUtils.hpp"


#include "RyujinCore/OS/FileSystem.hpp"


namespace Ryujin
{
    WaveInstance::WaveInstance()
    {
        
    }
    
    WaveInstance::~WaveInstance()
    {
        SAFE_DELETE_ARRAY(waveData);
    }
    
    bool WaveInstance::LoadFromFile(const char *filename)
    {
        if (!filename)
        {
            return false;
        }
        
        FileHandle file = File::Open(filename, FileMode::FM_Read, true);
        if (!file)
        {
            return false;
        }
        
        ChunkHeader chunkHeader;
        WaveHeader waveHeader;
        File::Read(&waveHeader, sizeof(waveHeader), 1, file);
        if (waveHeader.chunkId != WAVE_RIFF_ID && waveHeader.chunkId != WAVE_RIFF_ID_LE)
        {
            Debugf(Audio, "Bad RIFF format");
            return false;
        }
        
        if (waveHeader.waveFormat != WAVE_FORMAT_ID && waveHeader.waveFormat != WAVE_FORMAT_ID_LE)
        {
            Debugf(Audio, "Bad WAVE format");
            return false;
        }
        
        if (waveHeader.subChunk1Id != WAVE_SUBCHUNK1_ID && waveHeader.subChunk1Id != WAVE_SUBCHUNK1_ID_LE)
        {
            Debugf(Audio, "Bad fmt format");
            return false;
        }
        
        if (waveHeader.waveFormatTag != 1 || (waveHeader.numBitsPerSample != 16 && waveHeader.numBitsPerSample != 8))
        {
            return false;
        }
        
        uint32 skipBytes = waveHeader.pcmHeaderLength - (sizeof(WaveHeader) - 20);
        if(skipBytes > 0 && 0 != File::Seek(skipBytes, file))
        {
            return false;
        }
        
        int32 flag = 1;
        PtrSize stat = 0;
        while (flag < 10)
        {
            // check attempts.
            if(flag > 10)
            {
                return false;
            }
            
            // read chunk header
            stat = File::Read((void*)&chunkHeader, sizeof(ChunkHeader), 1, file);
            if(1 != stat)
            {
                return false;
            }
            
            // check chunk type.
            if(chunkHeader.dataId == WAVE_DATA_ID || chunkHeader.dataId == WAVE_DATA_ID_LE)
            {
                break;
            }
            
            // skip over chunk.
            ++flag;
            stat = File::Seek(chunkHeader.dataLength, file);
            if(stat != 0)
            {
                return false;
            }
        }
        
        waveDataLength = chunkHeader.dataLength;// / waveHeader.numBitsPerSample / 8;
        waveData = RYUJIN_NEW uint8[waveDataLength];
        
        stat = File::Read(waveData, waveDataLength, 1, file);
        if (1 != stat)
        {
            return false;
        }
        
        numSamplesPerSec = waveHeader.numSamplesPerSec;
        numChannels = (uint8)waveHeader.numChannels;
        numBitsPerSample = (uint8)waveHeader.numBitsPerSample;
        durationInSeconds = chunkHeader.dataLength / (numChannels * numBitsPerSample / 8) / numSamplesPerSec;
        
        return true;
    }}





