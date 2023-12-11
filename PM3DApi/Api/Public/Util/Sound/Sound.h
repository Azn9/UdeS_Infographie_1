#pragma once
#include <dsound.h>

class Sound
{
public:
    ~Sound();
    [[nodiscard]] bool isInitialized() const { return initialized; }

    [[nodiscard]] IDirectSoundBuffer8* getSoundBuffer() const { return soundBuffer; }

private:
    bool initialized = false;

    friend class SoundManager;
    explicit Sound(const SoundManager* soundManager, const char* filename);

    bool Initialize(const SoundManager* soundManager, const char* filename);

    IDirectSoundBuffer8* soundBuffer;
};
