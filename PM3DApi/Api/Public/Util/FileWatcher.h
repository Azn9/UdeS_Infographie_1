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
        void Run();
        ~FileWatcher();

    private:
        void Start();

        bool running;
        std::wstring path = L"";
        std::wstring fileName = L"";
        std::function<void()> onChange;
        std::thread thread;
    };
}
