#include "database.h"
#include <iostream>
#include <string>

Database::Database(const std::string& db_path) {
    int rc = sqlite3_open(db_path.c_str(), &db);
    if (rc) {
        std::cerr << "Error while opening database.\n" << sqlite3_errmsg(db) << std::endl;
    } 
    else {
        std::cout << "Database was opened.\n" << std::endl;
    }

    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS urls (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "original_url TEXT NOT NULL, short_url TEXT NOT NULL, created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
        "expiration_date TEXT NOT NULL);", 0, 0, 0);
    if (rc) {
        std::cerr << "Error while creating table." << sqlite3_errmsg(db) << std::endl;
    } 
}

Database::~Database() {
    close();
}

void Database::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
        std::cout << "Database connection closed." << std::endl;
    }
}

void Database::insert_url(std::string original_url, std::string short_url, std::string expiration_date) {
    std::string sql = "INSERT INTO urls(original_url, short_url, expiration_date) VALUES('";
    sql += original_url + "'," + "'" + short_url + "', '" + expiration_date + "');";
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0); 
    if(rc) {
        std::cerr << "Request (insert) error:" << sqlite3_errmsg(db) << std::endl; 
    }
}

int Database::get_last_id() {
    sqlite3_stmt* res;

    std::string sql = "SELECT MAX(id) FROM " + std::string(table_name) + ";";
    int last_id = 0;
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0) == SQLITE_OK) {
        if (sqlite3_step(res) == SQLITE_ROW) {
            last_id = sqlite3_column_int(res, 0);
        }
        sqlite3_finalize(res);
    } 
    else {
        std::cerr << "Request (last_id) error: " << sqlite3_errmsg(db) << std::endl;
    }
    return last_id;
}

std::string Database::get_original_url(std::string short_url) {
    sqlite3_stmt* res = nullptr;

    std::string sql = "SELECT original_url FROM " + std::string(table_name) + " WHERE short_url = '" + short_url + "';";
    std::string original_url; 
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0) == SQLITE_OK) {
        if (sqlite3_step(res) == SQLITE_ROW) {
            const unsigned char* text = sqlite3_column_text(res, 0);
            if (text) {
                original_url = reinterpret_cast<const char*>(text);
            }
        }
        sqlite3_finalize(res);
    } 
    else {
        std::cerr << "Request (last_id) error: " << sqlite3_errmsg(db) << std::endl;
    }
    return original_url;
}

std::string Database::get_expiration_date(std::string short_url) {
    sqlite3_stmt* res = nullptr;

    std::string sql = "SELECT expiration_date FROM " + std::string(table_name) + " WHERE short_url = '" + short_url + "';";
    std::string expiration_date; 
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0) == SQLITE_OK) {
        if (sqlite3_step(res) == SQLITE_ROW) {
            const unsigned char* text = sqlite3_column_text(res, 0);
            if (text) {
                expiration_date = reinterpret_cast<const char*>(text);
            }
        }
        sqlite3_finalize(res);
    } 
    else {
        std::cerr << "Request (expiration_date) error: " << sqlite3_errmsg(db) << std::endl;
    }
    return expiration_date;
}