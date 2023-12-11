#include "SoundManager.h"

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <dinput.h>
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <iostream>
#include <ostream>

bool SoundManager::Initialize(const HWND hwnd)
{
    auto result = DirectSoundCreate8(nullptr, &directSound, nullptr);
    if (FAILED(result))
    {
        return false;
    }

    result = directSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
    if (FAILED(result))
    {
        return false;
    }

    DSBUFFERDESC bufferDesc;
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
    bufferDesc.dwBufferBytes = 0;
    bufferDesc.dwReserved = 0;
    bufferDesc.lpwfxFormat = nullptr;
    bufferDesc.guid3DAlgorithm = GUID_NULL;

    result = directSound->CreateSoundBuffer(&bufferDesc, &primaryBuffer, nullptr);
    if (FAILED(result))
    {
        return false;
    }

    WAVEFORMATEX waveFormat;
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nSamplesPerSec = 44100;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nChannels = 2;
    waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0;

    result = primaryBuffer->SetFormat(&waveFormat);
    if (FAILED(result))
    {
        return false;
    }

    std::cout << "SoundManager::Initialize() OK" << std::endl;
    initialized = true;
    return true;
}

void SoundManager::Shutdown()
{
    if (primaryBuffer)
    {
        primaryBuffer->Release();
        primaryBuffer = nullptr;
    }

    if (directSound)
    {
        directSound->Release();
        directSound = nullptr;
    }
}

Sound SoundManager::loadSound(const std::string& filename) const
{
    return Sound(this, filename.c_str());
}

bool SoundManager::playSound(const Sound& sound) const
{
    if (!initialized || !sound.initialized)
    {
        std::cerr << "SoundManager::playSound() failed: sound not initialized" << std::endl;
        return false;
    }

    const auto soundBuffer = sound.getSoundBuffer();

    // Set position at the beginning of the sound buffer.
    auto result = soundBuffer->SetCurrentPosition(0);
    if (FAILED(result))
    {
        return false;
    }

    // Set volume of the buffer to 100%.
    result = soundBuffer->SetVolume(DSBVOLUME_MAX);
    if (FAILED(result))
    {
        return false;
    }

    // Play the contents of the secondary sound buffer.
    result = soundBuffer->Play(0, 0, 0);
    if (FAILED(result))
    {
        return false;
    }

    std::cout << "SoundManager::playSound() OK" << std::endl;

    return true;
}
