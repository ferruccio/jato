#include "catch.hpp"

#include <filesystem>
#include <string>

#include <jato.h>

namespace sys = std::tr2::sys;

struct DatabaseTestFixture {

    const sys::path testdb = "C:/tmp/test-database.edb";

    DatabaseTestFixture() {
        sys::remove(testdb);
    }

    ~DatabaseTestFixture() {
    }

};

TEST_CASE_METHOD(DatabaseTestFixture, "open non-existant database") {
    REQUIRE_FALSE(sys::exists(testdb));
    auto session = jato::make_session();
    CHECK_THROWS_AS(session->open_database(testdb), jato::error);
}

TEST_CASE_METHOD(DatabaseTestFixture, "create existing database") {
    REQUIRE_FALSE(sys::exists(testdb));
    auto session = jato::make_session();
    session->create_database(testdb);
    CHECK(sys::exists(testdb));
    CHECK_THROWS_AS(session->create_database(testdb), jato::error);
}

TEST_CASE_METHOD(DatabaseTestFixture, "create and open new database") {
    REQUIRE_FALSE(sys::exists(testdb));
    auto session = jato::make_session();
    session->create_database(testdb);
    CHECK(sys::exists(testdb));

    CHECK_NOTHROW(auto db = session->open_database(testdb));
}

TEST_CASE_METHOD(DatabaseTestFixture, "drop non-existant database") {
    REQUIRE_FALSE(sys::exists(testdb));
    CHECK_THROWS_AS(jato::drop_database(testdb), jato::error);
}
