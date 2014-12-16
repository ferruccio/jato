#include "catch.hpp"

#include <limits>
#include <string>

#include <jato.h>

#include <boost/uuid/uuid_generators.hpp>

TEST_CASE("FieldValue (bit_type)") {
    using T = std::uint8_t;
    using J = jato::bit_type;

    const auto min = 0;
    jato::FieldValue a { J(min) };
    REQUIRE(boost::get<J>(a).type == J::type);
    CHECK(boost::get<J>(a).value == min);

    const auto max = 1;
    jato::FieldValue b { J(max) };
    REQUIRE(boost::get<J>(b).type == J::type);
    CHECK(boost::get<J>(b).value == max);
}

TEST_CASE("FieldValue (ubyte_type)") {
    using T = std::uint8_t;
    using J = jato::ubyte_type;

    const auto min = std::numeric_limits<T>::min();
    jato::FieldValue a { J(min) };
    REQUIRE(boost::get<J>(a).type == J::type);
    CHECK(boost::get<J>(a).value == min);

    const auto max = std::numeric_limits<T>::max();
    jato::FieldValue b { J(max) };
    REQUIRE(boost::get<J>(b).type == J::type);
    CHECK(boost::get<J>(b).value == max);
}

TEST_CASE("FieldValue (short_type)") {
    using T = std::int16_t;
    using J = jato::short_type;

    const auto min = std::numeric_limits<T>::min();
    jato::FieldValue a { J(min) };
    REQUIRE(boost::get<J>(a).type == J::type);
    CHECK(boost::get<J>(a).value == min);

    const auto max = std::numeric_limits<T>::max();
    jato::FieldValue b { J(max) };
    REQUIRE(boost::get<J>(b).type == J::type);
    CHECK(boost::get<J>(b).value == max);
}

TEST_CASE("FieldValue (long_type)") {
    using T = std::int32_t;
    using J = jato::long_type;

    const auto min = std::numeric_limits<T>::min();
    jato::FieldValue a { J(min) };
    REQUIRE(boost::get<J>(a).type == J::type);
    CHECK(boost::get<J>(a).value == min);

    const auto max = std::numeric_limits<T>::max();
    jato::FieldValue b { J(max) };
    REQUIRE(boost::get<J>(b).type == J::type);
    CHECK(boost::get<J>(b).value == max);
}

TEST_CASE("FieldValue (currency_type)") {
    using T = std::int64_t;
    using J = jato::currency_type;

    const auto min = std::numeric_limits<T>::min();
    jato::FieldValue a { J(min) };
    REQUIRE(boost::get<J>(a).type == J::type);
    CHECK(boost::get<J>(a).value == min);

    const auto max = std::numeric_limits<T>::max();
    jato::FieldValue b { J(max) };
    REQUIRE(boost::get<J>(b).type == J::type);
    CHECK(boost::get<J>(b).value == max);
}

TEST_CASE("FieldValue (float_type)") {
    using T = float;
    using J = jato::float_type;

    const auto min = std::numeric_limits<T>::min();
    jato::FieldValue a { J(min) };
    REQUIRE(boost::get<J>(a).type == J::type);
    CHECK(boost::get<J>(a).value == min);

    const auto max = std::numeric_limits<T>::max();
    jato::FieldValue b { J(max) };
    REQUIRE(boost::get<J>(b).type == J::type);
    CHECK(boost::get<J>(b).value == max);
}

TEST_CASE("FieldValue (double_type)") {
    using T = double;
    using J = jato::double_type;

    const auto min = std::numeric_limits<T>::min();
    jato::FieldValue a { J(min) };
    REQUIRE(boost::get<J>(a).type == J::type);
    CHECK(boost::get<J>(a).value == min);

    const auto max = std::numeric_limits<T>::max();
    jato::FieldValue b { J(max) };
    REQUIRE(boost::get<J>(b).type == J::type);
    CHECK(boost::get<J>(b).value == max);
}

TEST_CASE("FieldValue (datetime_type)") {
    using T = double;
    using J = jato::datetime_type;

    const auto min = std::numeric_limits<T>::min();
    jato::FieldValue a { J(min) };
    REQUIRE(boost::get<J>(a).type == J::type);
    CHECK(boost::get<J>(a).value == min);

    const auto max = std::numeric_limits<T>::max();
    jato::FieldValue b { J(max) };
    REQUIRE(boost::get<J>(b).type == J::type);
    CHECK(boost::get<J>(b).value == max);
}

TEST_CASE("FieldValue (binary_type)") {
    using T = std::vector<std::uint8_t>;
    using J = jato::binary_type;

    T empty;
    jato::FieldValue a { J(empty) };
    REQUIRE(boost::get<J>(a).type == J::type);
    CHECK(boost::get<J>(a).value == empty);

    T not_empty { 0, 1, 2, 253, 254, 255 };
    jato::FieldValue b { J(not_empty) };
    REQUIRE(boost::get<J>(b).type == J::type);
    CHECK(boost::get<J>(b).value == not_empty);
}

TEST_CASE("FieldValue (text_type)") {
    using T = std::string;
    using J = jato::text_type;

    T empty;
    jato::FieldValue a { J(empty) };
    REQUIRE(boost::get<J>(a).type == J::type);
    CHECK(boost::get<J>(a).value == empty);

    T not_empty { "non-empty string" };
    jato::FieldValue b { J(not_empty) };
    REQUIRE(boost::get<J>(b).type == J::type);
    CHECK(boost::get<J>(b).value == not_empty);
}

TEST_CASE("FieldValue (long_binary_type)") {
    using T = std::vector<std::uint8_t>;
    using J = jato::long_binary_type;

    T empty;
    jato::FieldValue a { J(empty) };
    REQUIRE(boost::get<J>(a).type == J::type);
    CHECK(boost::get<J>(a).value == empty);

    T not_empty { 0, 1, 2, 253, 254, 255 };
    jato::FieldValue b { J(not_empty) };
    REQUIRE(boost::get<J>(b).type == J::type);
    CHECK(boost::get<J>(b).value == not_empty);
}

TEST_CASE("FieldValue (long_text_type)") {
    using T = std::string;
    using J = jato::long_text_type;

    T empty;
    jato::FieldValue a { J(empty) };
    REQUIRE(boost::get<J>(a).type == J::type);
    CHECK(boost::get<J>(a).value == empty);

    T not_empty { "non-empty string" };
    jato::FieldValue b { J(not_empty) };
    REQUIRE(boost::get<J>(b).type == J::type);
    CHECK(boost::get<J>(b).value == not_empty);
}

TEST_CASE("FieldValue (ulong_long_type)") {
    using T = std::uint64_t;
    using J = jato::ulong_long_type;

    const auto min = std::numeric_limits<T>::min();
    jato::FieldValue a { J(min) };
    REQUIRE(boost::get<J>(a).type == J::type);
    CHECK(boost::get<J>(a).value == min);

    const auto max = std::numeric_limits<T>::max();
    jato::FieldValue b { J(max) };
    REQUIRE(boost::get<J>(b).type == J::type);
    CHECK(boost::get<J>(b).value == max);
}

TEST_CASE("FieldValue (long_long_type)") {
    using T = std::int64_t;
    using J = jato::long_long_type;

    const auto min = std::numeric_limits<T>::min();
    jato::FieldValue a { J(min) };
    REQUIRE(boost::get<J>(a).type == J::type);
    CHECK(boost::get<J>(a).value == min);

    const auto max = std::numeric_limits<T>::max();
    jato::FieldValue b { J(max) };
    REQUIRE(boost::get<J>(b).type == J::type);
    CHECK(boost::get<J>(b).value == max);
}

TEST_CASE("FieldValue (guid_type)") {
    using T = boost::uuids::uuid;
    using J = jato::guid_type;

    const auto nil = boost::uuids::nil_uuid();
    jato::FieldValue a { J(nil) };
    REQUIRE(boost::get<J>(a).type == J::type);
    CHECK(boost::get<J>(a).value.is_nil());

    boost::uuids::string_generator gen;
    const auto uuid = gen("{01234567-89ab-cdef-0123-456789abcdef}");
    jato::FieldValue b { J(uuid) };
    REQUIRE(boost::get<J>(b).type == J::type);
    CHECK(boost::get<J>(b).value == uuid);
}

TEST_CASE("FieldValue (ushort_type)") {
    using T = std::uint16_t;
    using J = jato::ushort_type;

    const auto min = std::numeric_limits<T>::min();
    jato::FieldValue a { J(min) };
    REQUIRE(boost::get<J>(a).type == J::type);
    CHECK(boost::get<J>(a).value == min);

    const auto max = std::numeric_limits<T>::max();
    jato::FieldValue b { J(max) };
    REQUIRE(boost::get<J>(b).type == J::type);
    CHECK(boost::get<J>(b).value == max);
}