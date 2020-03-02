#include "Debugger.h"


int main() {
    Debugger debugger("../nes/nestest.nes");
    debugger.run();
    return 0;
}
