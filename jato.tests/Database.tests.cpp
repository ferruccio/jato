#include "catch.hpp"

#include <filesystem>
#include <string>

#include <jato.h>

namespace sys = std::tr2::sys;

struct DatabaseTestFixture {

    sys::path testdb = "C:/tmp/test-database";

    DatabaseTestFixture() {
        // wipe out db directory before test
        std::system(("rd /s /q " + testdb.directory_string() + " >nul 2>nul").c_str());
    }

    ~DatabaseTestFixture() {
    }

};

TEST_CASE_METHOD(DatabaseTestFixture, "open non-existant database") {
    REQUIRE_FALSE(sys::exists(testdb));
    CHECK_THROWS_AS(jato::open_database(testdb), jato::error);
}

TEST_CASE_METHOD(DatabaseTestFixture, "create existing database") {
    REQUIRE_FALSE(sys::exists(testdb));
    jato::create_database(testdb);
    CHECK(sys::exists(testdb));
    CHECK_THROWS_AS(jato::create_database(testdb), jato::error);
}

TEST_CASE_METHOD(DatabaseTestFixture, "create/open/delete new database") {
    REQUIRE_FALSE(sys::exists(testdb));
    jato::create_database(testdb);
    CHECK(sys::exists(testdb));

    CHECK_NOTHROW(auto db = jato::open_database(testdb));

    // db is out of scope now, so we should be able to delete the database
    CHECK_NOTHROW(jato::drop_database(testdb));

    REQUIRE_FALSE(sys::exists(testdb));
    jato::create_database(testdb);
    CHECK(sys::exists(testdb));

    auto db = jato::open_database(testdb);
    // db is still open, so we should not be able to delete the database
    CHECK_THROWS_AS(jato::drop_database(testdb), jato::error);
    CHECK(sys::exists(testdb));
}

TEST_CASE_METHOD(DatabaseTestFixture, "drop non-existant database") {
    REQUIRE_FALSE(sys::exists(testdb));
    CHECK_THROWS_AS(jato::drop_database(testdb), jato::error);
}
