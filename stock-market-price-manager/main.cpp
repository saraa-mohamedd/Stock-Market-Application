#include "deps.cpp"
#include "StockService.h"
#include "StockService.cpp"

int main() {
    //seed the random number generator to get different results each time
    srand(time(0));

    StockService server;

    //create a thread for the periodic stock price update
    std::thread t([&](){
        while(true) {
            server.randomUpdateStocks();
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    });

    //continue with main thread to start server and handle HTTP requests
    server.start();

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    //join the thread t with main thread 
    //(will never be executed in this case,but it's a good practice to join the thread with main thread)
    t.join();
    return 0;
}
