#include "amaterasu.h"

int main(int argc, char *argv[]) {
    std::ignore = argc;
    std::ignore = argv;

    using namespace amts;

    Amaterasu app;

    app.preinit();
    app.init();
    app.load();
    app.run();
    app.unload();
    app.cleanup();
    
    return 0;
}