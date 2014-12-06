#include "catch.hpp"

#include <filesystem>
#include <string>

#include <jato.h>

namespace sys = std::tr2::sys;

struct TableTestFixture {

    const sys::path testdb = "C:/tmp/test-database.edb";
    const std::string sysobjects = "MSysObjects";

    TableTestFixture() {
        sys::remove(testdb);
    }

    ~TableTestFixture() {
    }

};

TEST_CASE_METHOD(TableTestFixture, "open MSysObjects table") {
    auto session = jato::make_session();
    session->create_database(testdb);

    auto db = session->open_database(testdb);
    CHECK_NOTHROW(db->open_table(sysobjects));
}
