#pragma once

#include <vector>
#include <map>
#include <sqlite3.h>

namespace babel
{
    class Database {
        public:
            Database();
            Database(Database &&) = delete;
            Database(const Database &) = delete;
            Database &operator=(Database &&) = delete;
            Database &operator=(const Database &) = delete;
            ~Database();

            std::vector<std::map<std::string, std::string>> run_query(std::string &&request);

        private:
            sqlite3 *db;
    };
}