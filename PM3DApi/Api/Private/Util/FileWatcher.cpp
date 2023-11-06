#include "../../Public/Util/FileWatcher.h"

#include <iostream>
#include <ostream>
#include <windows.h>

#include "Api/Public/Util/Util.h"

PM3D_API::FileWatcher::FileWatcher(
    const std::wstring& pathToWatch,
    const std::function<void()>& onChange
) : running(true), onChange(onChange)
{
    // Split path into directory and filename
    const std::wstring::size_type pos = pathToWatch.find_last_of(L"\\/");

    // If there is no directory in the path, assume the current directory
    if (pos == std::wstring::npos)
    {
        path = L".";
        fileName = pathToWatch;
    }
    else
    {
        path = pathToWatch.substr(0, pos);
        fileName = pathToWatch.substr(pos + 1);
    }

    thread = std::thread([this]() { Start(); });
    thread.detach();

    std::cout << "Watching file " << Util::ws2s(fileName) << " in directory " << Util::ws2s(path) << "..." << std::endl;
}

PM3D_API::FileWatcher::~FileWatcher()
{
    running = false;

    if (thread.joinable())
        thread.join();
}

void PM3D_API::FileWatcher::Start()
{
    HANDLE hDir = CreateFile(path.c_str(), // pointer to the file name
                             FILE_LIST_DIRECTORY, // access (read/write) mode
                             FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, // share mode
                             NULL, // security descriptor
                             OPEN_EXISTING, // how to create
                             FILE_FLAG_BACKUP_SEMANTICS, // file attributes
                             NULL // file with attributes to copy
    );
    wchar_t filename[MAX_PATH];
    FILE_NOTIFY_INFORMATION Buffer[1024];
    DWORD BytesReturned;
    while (ReadDirectoryChangesW(
        hDir, // handle to directory
        &Buffer, // read results buffer
        sizeof(Buffer), // length of buffer
        TRUE, // monitoring option
        FILE_NOTIFY_CHANGE_SECURITY |
        FILE_NOTIFY_CHANGE_CREATION |
        FILE_NOTIFY_CHANGE_LAST_ACCESS |
        FILE_NOTIFY_CHANGE_LAST_WRITE |
        FILE_NOTIFY_CHANGE_SIZE |
        FILE_NOTIFY_CHANGE_ATTRIBUTES |
        FILE_NOTIFY_CHANGE_DIR_NAME |
        FILE_NOTIFY_CHANGE_FILE_NAME, // filter conditions
        &BytesReturned, // bytes returned
        NULL, // overlapped buffer
        NULL // completion routine
    ) && running)
    {
        int offset = 0;
        FILE_NOTIFY_INFORMATION* pNotify;
        pNotify = (FILE_NOTIFY_INFORMATION*)((char*)Buffer + offset);
        wcscpy_s(filename, L"");
        wcsncpy_s(filename, pNotify->FileName, pNotify->FileNameLength / 2);

        filename[pNotify->FileNameLength / 2] = NULL;

        if (Buffer[0].Action == FILE_ACTION_MODIFIED)
        {
            if (fileName == filename)
            {
                onChange();
            }
        }
    }

    CloseHandle(hDir);
}
