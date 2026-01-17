#include "crow_all.h"
#include "database.h"
#include "base62.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <format> 
#include <string>
#include <unordered_map>

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