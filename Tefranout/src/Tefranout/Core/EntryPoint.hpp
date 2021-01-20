#ifndef __ENTRYPOINT__HPP
#define __ENTRYPOINT__HPP

#include "Tefranout_macro.hpp" // Needed for macro definition

#include "Application.hpp"

// Will be defined in client code (See Samples)
extern Tefranout::Application *Tefranout::CreateApplication();

// Embed main to keep control of the engine during its whole lifetime
int main()
{
    cout << "Welcome de Tefranout Engine !" << endl;

    auto *app = Tefranout::CreateApplication();
    app->Run();

    delete app;
}

#endif
