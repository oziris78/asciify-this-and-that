
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")


#include "Application.h"


int main(void) {

    Application* app = new Application();
    app->run();
    delete app;

}

