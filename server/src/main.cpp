#include "Database.hpp"
#include "Server.hpp"
#include <iostream>

int main()
{
    try {
        babel::Database db;
        babel::Server srv(db);

        srv.run();
    } catch (const babel::Error &e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}