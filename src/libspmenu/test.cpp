/* g++ test.cpp -o test -lspmenu */
#include <iostream>
#include <libspmenu/libspmenu.h>

int main() {
    spmenu::spmenuSettings settings;

    settings.promptText = "libspmenu test";
    settings.windowPosition = "Bottom";
    settings.itemPosition = "Top";
    settings.standardInput = "Item1\nItem2\nItem3\n";

    std::cout << spmenu::Run(&settings);
}
