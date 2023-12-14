#include "Sound.h"

#include <iostream>

#include "SoundManager.h"

Sound::~Sound()
{
    if (soundBuffer)
    {
        soundBuffer->Release();
        soundBuffer = nullptr;
    }
}

Sound::Sound(const SoundManager* soundManager, const char* filename)
{
    initialized = Initialize(soundManager, filename);
}

bool Sound::Initialize(const SoundManager* soundManager, const char* filename)
{
    FILE* filePtr;
    SoundManager::WaveHeaderType waveFileHeader;

    // Open the wave file in binary.
    auto error = fopen_s(&filePtr, filename, "rb");
    if (error != 0)
    {
        return false;
    }

    // Read in the wave file header.
    auto count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
    if (count != 1)
    {
        return false;
    }

    // Check that the chunk ID is the RIFF format.
    if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
        (waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
    {
        return false;
    }

    // Check that the file format is the WAVE format.
    if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
        (waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
    {
        return false;
    }

    // Check that the sub chunk ID is the fmt format.
    if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
        (waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
    {
        return false;
    }

    // Check that the audio format is WAVE_FORMAT_PCM.
    if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
    {
        return false;
    }

    // Check that the wave file was recorded in stereo format.
    if (waveFileHeader.numChannels != 2)
    {
        return false;
    }

    // Check that the wave file was recorded at a sample rate of 44.1 KHz.
    if (waveFileHeader.sampleRate != 44100)
    {
        return false;
    }

    // Ensure that the wave file was recorded in 16 bit format.
    if (waveFileHeader.bitsPerSample != 16)
    {
        return false;
    }

    // Check for the data chunk header.
    if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
        (waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
    {
        return false;
    }

    WAVEFORMATEX waveFormat;
    // Set the wave format of secondary buffer that this wave file will be loaded onto.
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nSamplesPerSec = 44100;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nChannels = 2;
    waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0;

    DSBUFFERDESC bufferDesc;
    // Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
    bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
    bufferDesc.dwReserved = 0;
    bufferDesc.lpwfxFormat = &waveFormat;
    bufferDesc.guid3DAlgorithm = GUID_NULL;

    // Create a temporary sound buffer with the specific buffer settings.
    IDirectSoundBuffer* tempBuffer;

    auto result = soundManager->getDirectSound()->CreateSoundBuffer(&bufferDesc, &tempBuffer, nullptr);
    if (FAILED(result))
    {
        return false;
    }

    // Test the buffer format against the direct sound 8 interface and create the secondary buffer.
    result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8,
                                        static_cast<LPVOID*>(static_cast<void*>(&soundBuffer)));
    if (FAILED(result))
    {
        return false;
    }

    // Release the temporary buffer.
    tempBuffer->Release();
    tempBuffer = nullptr;

    // Move to the beginning of the wave data which starts at the end of the data chunk header.
    result = fseek(filePtr, sizeof(SoundManager::WaveHeaderType), SEEK_SET);
    if (result != 0)
    {
        return false;
    }

    // Create a temporary buffer to hold the wave file data.
    auto waveData = new unsigned char[waveFileHeader.dataSize];
    if (!waveData)
    {
        return false;
    }

    // Read in the wave file data into the newly created buffer.
    count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
    if (count != waveFileHeader.dataSize)
    {
        return false;
    }

    // Close the file once done reading.
    error = fclose(filePtr);
    if (error != 0)
    {
        return false;
    }

    unsigned char* bufferPtr;
    unsigned long bufferSize;

    // Lock the secondary buffer to write wave data into it.
    result = soundBuffer->Lock(0, waveFileHeader.dataSize, static_cast<LPVOID*>(static_cast<void*>(&bufferPtr)),
                               &bufferSize, nullptr, nullptr,
                               0);
    if (FAILED(result))
    {
        return false;
    }

    // Copy the wave data into the buffer.
    memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

    // Unlock the secondary buffer after the data has been written to it.
    result = soundBuffer->Unlock((void*)bufferPtr, bufferSize, nullptr, 0);
    if (FAILED(result))
    {
        return false;
    }

    // Release the wave data since it was copied into the secondary buffer.
    delete [] waveData;
    waveData = nullptr;

    std::cout << "Sound::Initialize() OK" << std::endl;

    return true;
}
