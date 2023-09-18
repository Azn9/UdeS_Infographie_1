#pragma once

namespace PM3D
{
    template <class T>
    class CSingleton
    {
    public:
        static T& GetInstance()
        {
            return Instance;
        }
        static T* GetInstancePtr()
        {
            return &Instance;
        }

    protected :
        CSingleton() = default;
        ~CSingleton() = default;

    private :
        static T Instance;

        CSingleton(CSingleton&) = delete;
        void operator =(CSingleton&) = delete;
    };

    template <class T>
    T CSingleton<T>::Instance;
}
