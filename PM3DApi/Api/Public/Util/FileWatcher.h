#pragma once
#include <functional>
#include <string>
#include <thread>

namespace PM3D_API
{
    class FileWatcher
    {
    public:
        explicit FileWatcher(const std::wstring& pathToWatch, const std::function<void()>& onChange);
        ~FileWatcher();

    private:
        void Start();

        bool running;
        std::wstring path;
        std::wstring fileName;
        std::function<void()> onChange;
        std::thread thread;
    };
}
