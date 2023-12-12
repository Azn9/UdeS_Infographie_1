#pragma once
#include <dsound.h>
#include <windows.h>
#include <string>

#include "Sound.h"
#include "Core/Public/Util/Singleton.h"

class SoundManager : public PM3D::CSingleton<SoundManager>
{
public:
    SoundManager() = default;

    bool Initialize(HWND);
    void Shutdown();

    [[nodiscard]] Sound loadSound(const std::string& filename) const;
    bool playSound(const Sound& sound) const;

    [[nodiscard]] bool isInitialized() const { return initialized; }

    IDirectSound8* getDirectSound() const { return directSound; }

    bool LoadAndPlay(const std::string& filename) const;
    void StopAllSounds();

    struct WaveHeaderType
    {
        char chunkId[4];
        unsigned long chunkSize;
        char format[4];
        char subChunkId[4];
        unsigned long subChunkSize;
        unsigned short audioFormat;
        unsigned short numChannels;
        unsigned long sampleRate;
        unsigned long bytesPerSecond;
        unsigned short blockAlign;
        unsigned short bitsPerSample;
        char dataChunkId[4];
        unsigned long dataSize;
    };

private:
    bool initialized = false;
    IDirectSound8* directSound;
    IDirectSoundBuffer* primaryBuffer;

    IDirectSoundBuffer8* secondaryBuffer1;
};
