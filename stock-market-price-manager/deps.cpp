// #ifndef _WIN32_WINNT
// #define _WIN32_WINNT 0x0600

// #include <cpprest/http_listener.h>
// #include <cpprest/json.h>
// #include <cpprest/uri.h>
// #include <thread>
// #include <fstream>
// #include <iostream>
// #include <iomanip>
// #include <string>
// #include <filesystem>

// using namespace web;
// using namespace web::http;
// using namespace web::http::experimental::listener;

// json::value buyStock(std::string email, std::string company);
// json::value getUserStocks(std::string email);
// json::value sellStock(std::string email, std::string company);

// // json::value getStocksInfo(){
//     // std::ifstream input_file("./files/stocks.csv");
//     // std::vector<std::vector<std::string>> stocksInfo;

//     // if(!input_file){
//     //     throw std::runtime_error("Failed to open the file for reading.");
//     // }

//     // int rowNum=0;

//     // for (std::string line; getline(input_file, line);){
//     //     std::istringstream ss(std::move(line));
//     //     std::vector<std::string> row;

//     //     if (!stocksInfo.empty()) {
//     //         row.reserve(stocksInfo.front().size());
//     //     }

//     //     for (std::string value; std::getline(ss, value, ',');) {
//     //         row.push_back(std::move(value));
//     //     }

//     //     if (rowNum>0){
//     //         stocksInfo.push_back(std::move(row));
//     //     }
//     //     rowNum++;
//     // }

//     // input_file.close();

// //     std::string sqlQuery = "SELECT * FROM stockmarket.stocks";
// //     sql::ResultSet* res = db_.executeSelect(sqlQuery);
// //     json::value response;
// //     response[U("stocks")] = json::value::array();

// //     for (const std::vector<std::string>& row : stocksInfo) {
// //         json::value stock;
// //         stock[U("company")] = json::value::string(U(row[0]));
// //         stock[U("price")] = json::value::string(U(row[1]));
// //         stock[U("remainingShares")] = json::value::string(U(row[2]));
// //         stock[U("percentPerSell")] = json::value::string(U(row[3]));
// //         stock[U("percentPerBuy")] = json::value::string(U(row[4]));
// //         response[U("stocks")][response[U("stocks")].size()] = stock;
// //     }

// //     std::cout << response.serialize() << std::endl;
// //     return response;
// // }

// json::value buyStock(std::string email, std::string company){
//     std::ifstream input_file("./files/stocks.csv");
//     std::vector<std::vector<std::string>> stocksInfo;

//     if(!input_file){
//         throw std::runtime_error("Failed to open the stocks file for reading.");
//     }

//     int rowNum=0; bool found = false; double priceBought=0.0;

//     for (std::string line; getline(input_file, line);){
//         std::istringstream ss(std::move(line));
//         std::vector<std::string> row;

//         if (!stocksInfo.empty()) {
//             row.reserve(stocksInfo.front().size());
//         }

//         for (std::string value; std::getline(ss, value, ',');) {
//             row.push_back(std::move(value));
//         }

//         if (rowNum>0){
//             if (row[0] == company){
//                 found = true;
//                 if (std::stoi(row[2]) > 0){
//                     priceBought = std::stod(row[1]);
//                     // Decrease the remaining shares by 1 and increase the price by the percent per buy
//                     row[2] = std::to_string(std::stoi(row[2]) - 1);
//                     row[1] = std::to_string(std::stod(row[1]) + std::stod(row[1]) * 0.01 * std::stod(row[4])); 

//                     //format the price to have 3 decimal places
//                     std::stringstream s;
//                     s << std::fixed << std::setprecision(3) << std::stod(row[1]);
//                     row[1] = s.str();

//                     stocksInfo.push_back(std::move(row));
//                 }
//                 else{
//                     throw std::runtime_error("No remaining shares for this company.");
//                 }
//             }
//             else{
//                 stocksInfo.push_back(std::move(row));
//             }
//             stocksInfo.push_back(std::move(row));
//         }
//         rowNum++;
//     }

//     if (!found && stocksInfo[stocksInfo.size()-1][0] != company){
//         throw std::runtime_error("Company not found.");
//     }

//     input_file.close();
//     std::filesystem::remove("./files/stocks.csv");

//     std::fstream output_stocks_file("./files/stocks.csv", std::ios::out);
//     if (!output_stocks_file) {
//         throw std::runtime_error("Failed to open the stocks file for writing.");
//     }

//     //update the stocks file with new number of remaining shares and price
//     output_stocks_file << "company,price,remainingShares,percent_per_sell,percent_per_buy\n";

//     for (const std::vector<std::string>& row : stocksInfo) {
//         for (const std::string& value : row) {
//             output_stocks_file << value << ",";
//         }
//         output_stocks_file << "\n";
//     }

//     //update the user stocks file with the new stock
//     std::ofstream output_file("./files/user_stocks.csv", std::ios::app);
//     if (!output_file) {
//         throw std::runtime_error("Failed to open the user stocks file for writing.");
//     }

//     output_file << "\n" << email << "," << company << "," << std::fixed << std::setprecision(3) << priceBought;

//     json::value response = getUserStocks(email);
//     return response;
// }

// json::value getUserStocks(std::string email){
//     std::ifstream input_file("./files/user_stocks.csv");
//     std::vector<std::vector<std::string>> userStocks;

//     if(!input_file){
//         throw std::runtime_error("Failed to open the user stocks file for reading.");
//     }

//     json::value response;
//     response[U("stocks")] = json::value::array();

//     int fileRowNum=0; bool found = false;

//     for (std::string line; getline(input_file, line);){
//         std::istringstream ss(std::move(line));
//         std::vector<std::string> row;

//         if (fileRowNum>0){
//             for (std::string value; std::getline(ss, value, ',');) {
//                 row.push_back(std::move(value));
//             }
//             if (row[0] == email){
//                 found = true;
//                 if (!userStocks.empty()) {
//                    row.reserve(userStocks.front().size());
//                 }
//                 userStocks.push_back(std::move(row));
//             }
//         }
//         fileRowNum++;
//     }

//     if (!found){
//         response[U("stocks")] = {};
//     }

//     for (const std::vector<std::string>& row : userStocks) {
//         json::value stock;
//         stock[U("company")] = json::value::string(U(row[0]));
//         stock[U("price")] = json::value::string(U(row[1]));
//         stock[U("priceBought")] = json::value::string(U(row[2]));
//         response[U("stocks")][response[U("stocks")].size()] = stock;
//     }

//     input_file.close();
//     return response;
// }

// json::value sellStock(std::string email, std::string company){
//     std::ifstream input_file("./files/stocks.csv");
//     std::vector<std::vector<std::string>> stocksInfo;

//     if(!input_file){
//         throw std::runtime_error("Failed to open the stocks file for reading.");
//     }

//     int rowNum=0; bool found = false; double priceSold=0.0;

//     for (std::string line; getline(input_file, line);){
//         std::istringstream ss(std::move(line));
//         std::vector<std::string> row;

//         if (!stocksInfo.empty()) {
//             row.reserve(stocksInfo.front().size());
//         }

//         for (std::string value; std::getline(ss, value, ',');) {
//             row.push_back(std::move(value));
//         }

//         if (rowNum>0){
//             if (row[0] == company){
//                 found = true;
//                 priceSold = std::stod(row[1]);
//                 // Increase the remaining shares by 1 and decrease the price by the percent per sell
//                 row[2] = std::to_string(std::stoi(row[2]) + 1);
//                 row[1] = std::to_string(std::stod(row[1]) - std::stod(row[1]) * 0.01 * std::stod(row[3])); 

//                 //format the price to have 3 decimal places
//                 std::stringstream s;
//                 s << std::fixed << std::setprecision(3) << std::stod(row[1]);
//                 row[1] = s.str();

//                 stocksInfo.push_back(std::move(row));
//             }
//             else{
//                 stocksInfo.push_back(std::move(row));
//             }
//         }
//         rowNum++;
//     }

//     if (!found && stocksInfo[stocksInfo.size()-1][0] != company){
//         throw std::runtime_error("Company not found.");
//     }

//     input_file.close();
//     std::filesystem::remove("./files/stocks.csv");

//     std::fstream output_stocks_file("./files/stocks.csv", std::ios::out);
//     if (!output_stocks_file) {
//         throw std::runtime_error("Failed to open the stocks file for writing.");
//     }

//     //update the stocks file with new number of remaining shares and price
//     output_stocks_file << "company,price,remainingShares,percent_per_sell,percent_per_buy\n";
//     for (const std::vector<std::string>& row : stocksInfo) {
//         for (const std::string& value : row) {
//             output_stocks_file << value << ",";
//         }
//         output_stocks_file << "\n";
//     }

//     //update the user stocks file with the sold stock
//     std::ifstream input_user_stocks_file("./files/user_stocks.csv");
//     std::vector<std::vector<std::string>> userStocks;

//     if (!input_user_stocks_file) {
//         throw std::runtime_error("Failed to open the user stocks file for reading.");
//     }

//     int userRowNum=0; bool userFound = false; double userpriceSold=0.0;
//     std::vector<std::vector<std::string>> userStockss;

//     for (std::string line; getline(input_user_stocks_file, line);){
//         std::istringstream ss(std::move(line));
//         std::vector<std::string> row;

//         if (!stocksInfo.empty()) {
//             row.reserve(stocksInfo.front().size());
//         }

//         for (std::string value; std::getline(ss, value, ',');) {
//             row.push_back(std::move(value));
//         }

//         if (userRowNum>0){
//             if (row[0] == company){
//                 userFound = true;
//                 userpriceSold = std::stod(row[1]);
//                 // Increase the remaining shares by 1 and decrease the price by the percent per sell
//                 row[2] = std::to_string(std::stoi(row[2]) + 1);
//                 row[1] = std::to_string(std::stod(row[1]) - std::stod(row[1]) * 0.01 * std::stod(row[3])); 

//                 //format the price to have 3 decimal places
//                 std::stringstream s;
//                 s << std::fixed << std::setprecision(3) << std::stod(row[1]);
//                 row[1] = s.str();

//                 userStockss.push_back(std::move(row));
//             }
//             else{
//                 userStockss.push_back(std::move(row));
//             }
//         }
//         userRowNum++;
//     }

//     json::value response = getUserStocks(email);
//     return response;

// }

// #endif