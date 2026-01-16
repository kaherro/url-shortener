#include "crow_all.h"
#include "database.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <format> 
#include <string>
#include <unordered_map>

std::unordered_map<int, char> table_10_to_62 = {
    {0, '0'}, {1, '1'}, {2, '2'}, {3, '3'}, {4, '4'}, {5, '5'}, {6, '6'}, {7, '7'}, {8, '8'}, {9, '9'},
    {10, 'A'}, {11, 'B'}, {12, 'C'}, {13, 'D'}, {14, 'E'}, {15, 'F'}, {16, 'G'}, {17, 'H'}, {18, 'I'}, {19, 'J'}, 
    {20, 'K'}, {21, 'L'}, {22, 'M'}, {23, 'N'}, {24, 'O'}, {25, 'P'}, {26, 'Q'}, {27, 'R'}, {28, 'S'}, {29, 'T'}, 
    {30, 'U'}, {31, 'V'}, {32, 'W'}, {33, 'X'}, {34, 'Y'}, {35, 'Z'}, 
    {36, 'a'}, {37, 'b'}, {38, 'c'}, {39, 'd'}, {40, 'e'}, {41, 'f'}, {42, 'g'}, {43, 'h'}, {44, 'i'}, {45, 'j'}, 
    {46, 'k'}, {47, 'l'}, {48, 'm'}, {49, 'n'}, {50, 'o'}, {51, 'p'}, {52, 'q'}, {53, 'r'}, {54, 's'}, {55, 't'}, 
    {56, 'u'}, {57, 'v'}, {58, 'w'}, {59, 'x'}, {60, 'y'}, {61, 'z'} 
}; 

std::string to_base62(int id10) {
    std::string id62 = "";
    while(id10 > 62) {
        id62 += table_10_to_62[id10 % 62]; 
        id10 /= 62; 
    } 
    if(id10) id62 += table_10_to_62[id10]; 
    reverse(id62.begin(), id62.end()); 
    return id62; 
}


crow::response redirect(const std::string& url)
{
    crow::response res(302);
    res.add_header("Location", url);
    return res;
}

int main()
{
    crow::SimpleApp app; 
    Database db("test.db");

    //curl -X POST localhost:18080/short -d '{"url": "345678", "expiration_date" : "2025-05-09 14:51:51"}'
    CROW_ROUTE(app, "/short").methods(crow::HTTPMethod::POST)([&db](const crow::request& req){
        auto json = crow::json::load(req.body); 

        if(!json.has("url")) {
            std::cerr << "Error: url is requiered. "; 
            return crow::response(400, "Error: url is requiered."); 
        }

        std::string original_url = json["url"].s(); 
        if(original_url.substr(0, 7) != "http://" && original_url.substr(0, 8) != "https://") {
            original_url = "http://" + original_url;
        }

        int id = db.get_last_id() + 1; 
        std::string short_url = to_base62(id); 

        std::string expiration_date = "9999-12-31 23:59:59";
        if(json.has("expiration_date")) {
            expiration_date = json["expiration_date"].s(); 
        }

        db.insert_url(original_url, short_url, expiration_date); 

        crow::json::wvalue res; 
        res["short_url"] = "http://localhost:18080/" + short_url; 
        return crow::response(res); 
    });

    //curl localhost:18080/<short_url>
    CROW_ROUTE(app, "/<string>").methods(crow::HTTPMethod::GET)([&db](const crow::request& req, std::string short_url){
        std::string original_url = db.get_original_url(short_url); 
        std::string expiration_date = db.get_expiration_date(short_url); 

        auto now = std::chrono::system_clock::now();
        std::string current_date = std::format("{:%Y-%m-%d %H:%M:%S}", now);
        current_date = current_date.substr(0, 19); 
        if(current_date >= expiration_date) {
            std::cerr << "Error: short url was expired: "; 
            return crow::response(400, "Error: short_url was expired: " + expiration_date); 
        }
        
        return redirect(original_url); 
    });


    app.port(18080).multithreaded().run(); 
}