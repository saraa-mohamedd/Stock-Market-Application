#include "deps.cpp"
#include "StockService.h"
#include "StockService.cpp"

int main() {
    StockService server;
    server.start();

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
