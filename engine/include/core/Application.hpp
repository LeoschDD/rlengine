#pragma once

#include "core/Headers.hpp"
#include "core/Log.hpp"

namespace rle
{
    class Application
    {
    private:
        bool running_{false};
        
    private:
        bool Init();

    protected:
        virtual bool OnInit() {return true;}

    public:
        Application();
        ~Application() = default;

        void Run();
    };
}