#include "Database.hpp"
#include "Error.hpp"
#include <fstream>
#include <string>

babel::Database::Database()
{
    std::ifstream file("./");
    int ret_value = sqlite3_open("./", &db);

    if (!file.good()) {
        run_query("CREATE TABLE `users` (`id` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
                  "`username` TEXT NOT NULL UNIQUE, `password` TEXT NOT NULL)");
    }
}

std::vector<std::map<std::string, std::string>> babel::Database::run_query(std::string &&request)
{
    sqlite3_stmt *stmt;
    std::vector<std::map<std::string, std::string>> ret;

    int rc = sqlite3_prepare_v2(db, request.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        throw babel::Error(sqlite3_errmsg(db));
    }
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        ret.push_back({});
        for (int i = 0; i < sqlite3_column_count(stmt); i++) {
            ret.back()[sqlite3_column_name(stmt, i)] = reinterpret_cast<const char *>(sqlite3_column_text(stmt, i));
        }
    }
    if (rc != SQLITE_DONE) {
        throw babel::Error(sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return (ret);
}

babel::Database::~Database() { sqlite3_close(db); }