#include <fstream>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <filesystem>
using namespace std;

int main() {
    while (true) {
        srand(time(0)); // Seed the random number generator to get different results each time

        std::fstream input_file("stocks.csv", ios::in);
        if (!input_file) {
            std::cerr << "Failed to open the file for reading." << std::endl;
            return 1;
        }

        std::vector<std::vector<std::string>> stocksInfo;

        int rowNum=0;
        for (string line; getline(input_file, line);){
            std::istringstream ss(std::move(line));
            std::vector<std::string> row;

            if (!stocksInfo.empty()) {
                row.reserve(stocksInfo.front().size());
            }

            // std::getline can split on other characters, here we use ','
            for (std::string value; std::getline(ss, value, ',');) {
                row.push_back(std::move(value));
            }

            //first row in csv contains the column names, so we skip it
            if (rowNum>0){

                // Generate a random number between -0.1 and 0.1
                double random = ((double) rand() / (RAND_MAX)) * 0.2 - 0.1;

                row[1] = std::to_string(std::stod(row[1]) + std::stod(row[1]) * random);
                std::stringstream s;
                s << std::fixed << std::setprecision(3) << std::stod(row[1]);
                row[1] = s.str();
                stocksInfo.push_back(std::move(row));
            }
            rowNum++;
        }

        input_file.close(); // Close the file
        std::filesystem::remove("stocks.csv"); // Remove the file to create a new one (override it)

        std::fstream output_file("stocks.csv", ios::out); // Open the file in write mode
        if (!output_file) {
            std::cerr << "Failed to open the file for writing." << std::endl;
            return 1;
        }

        output_file << "company,price,remainingShares,percent_per_sell,percent_per_buy\n";

        for (const std::vector<std::string>& row : stocksInfo) {
            for (const std::string& value : row) {
                output_file << value << ",";
            }
            output_file << "\n";
        }

        for (const std::vector<std::string>& row : stocksInfo) {
            for (const std::string& value : row) {
                std::cout << std::setw(10) << value;
            }
            std::cout << "\n";
            }

        output_file.close(); // Close the file

        std::this_thread::sleep_for(std::chrono::seconds(10)); // Sleep for 10 seconds
    }

    return 0;
}