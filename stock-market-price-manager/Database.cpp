#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_driver.h>

class Database {
public:

    Database(const std::string& host, const std::string& user, const std::string& password, const std::string& database)
        : host_(host), user_(user), password_(password), database_(database) {}

    bool connect() {
        driver_ = sql::mysql::get_driver_instance();
        connection_ = driver_->connect(host_, user_, password_);
        connection_->setSchema(database_);
        return true;
    }

    void executeQuery(const std::string& query) {
        sql::Statement* stmt = connection_->createStatement();
        try{
            stmt->execute(query);
        }
        catch(const std::exception& e){
            std::cerr << e.what() << std::endl;
            throw e;
        }
        delete stmt;
    }

    sql::ResultSet* executeSelect(const std::string& query) {
        sql::Statement* stmt = connection_->createStatement();
        sql::ResultSet* res = stmt->executeQuery(query);
        delete stmt;
        return res;
    }

    // Other methods for interacting with the database go here...

    private:
    std::string host_;
    std::string user_;
    std::string password_;
    std::string database_;
    sql::Driver* driver_;
    sql::Connection* connection_;
};
