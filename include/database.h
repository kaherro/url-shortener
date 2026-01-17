#include <string>
#include "sqlite3.h"

class Database {
private:
    sqlite3* db;
    const std::string table_name = "urls";
    
public:
    Database(const std::string& db_path);
    ~Database();

    void insert_url(const std::string original_url, 
                    const std::string short_url, 
                    const std::string expiration_date);
    
    int get_last_id();
    
    std::string get_original_url(const std::string short_url);
    
    std::string get_expiration_date(const std::string short_url);
    
    bool is_connected() const { return db != nullptr; }
    void close();
};