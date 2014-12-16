#pragma once

#include <array>
#include <filesystem>
#include <functional>
#include <memory>
#include <stdexcept>
#include <cstdint>
#include <string>
#include <vector>

#include <boost/variant.hpp>
#include <boost/uuid/uuid.hpp>

namespace jato {

    using std::function;
    using std::string;
    using std::vector;
    using std::unique_ptr;

    namespace sys = std::tr2::sys;

    class error : public std::runtime_error {
    public:
        explicit error(const string& what) : runtime_error(what) {}
        explicit error(const char* what) : runtime_error(what) {}
    };

    using field_type = unsigned long;

    struct FieldDescriptor {
        string name;
        field_type type;
    };

    struct TableDescriptor {
        string name;
    };

    template <typename T, field_type type>
    class ft_def {
    public:
        explicit ft_def(T value) : value(value) {}
        T value;
        static const field_type type = type;
    };

    // _coltyp values *MUST* match JET_COLTYP values in esent.h
#define JATO_FIELD_TYPE(_ftype, _type, _coltyp) \
    struct _ftype : ft_def<_type, _coltyp> { \
        explicit _ftype(_type value) : ft_def(value) {} \
    };

    JATO_FIELD_TYPE(bit_type, std::uint8_t, 1)
    JATO_FIELD_TYPE(ubyte_type, std::uint8_t, 2)
    JATO_FIELD_TYPE(short_type, std::int16_t, 3)
    JATO_FIELD_TYPE(long_type, std::int32_t, 4)
    JATO_FIELD_TYPE(currency_type, std::int64_t, 5)
    JATO_FIELD_TYPE(float_type, float, 6)
    JATO_FIELD_TYPE(double_type, double, 7)
    JATO_FIELD_TYPE(datetime_type, double, 8)
    JATO_FIELD_TYPE(binary_type, std::vector<std::uint8_t>, 9)
    JATO_FIELD_TYPE(text_type, std::string, 10)
    JATO_FIELD_TYPE(long_binary_type, std::vector<std::uint8_t>, 11)
    JATO_FIELD_TYPE(long_text_type, std::string, 12)
    JATO_FIELD_TYPE(ulong_long_type, std::uint64_t, 14)
    JATO_FIELD_TYPE(long_long_type, std::int64_t, 15)
    JATO_FIELD_TYPE(guid_type, boost::uuids::uuid, 16)
    JATO_FIELD_TYPE(ushort_type, std::uint16_t, 17)

#undef JATO_FIELD_TYPE

    using FieldValue = boost::variant<
        bit_type,
        ubyte_type,
        short_type,
        long_type,
        currency_type,
        float_type,
        double_type,
        datetime_type,
        binary_type,
        text_type,
        long_binary_type,
        long_text_type,
        ulong_long_type,
        long_long_type,
        guid_type,
        ushort_type
    >;

    namespace interface {
        struct Record {
            virtual ~Record() {}

            virtual void set_field(const string& fieldname, FieldValue field) = 0;
            virtual auto get_field(const string& fieldname) const -> FieldValue = 0;
        };
    }

    using record_ptr = unique_ptr <interface::Record>;

    namespace interface {
        struct Table {
            virtual ~Table() {}

            virtual void create_field(const string& name, field_type type) = 0;
            virtual void delete_field(const string& name) = 0;
            virtual void rename_field(const string& oldname, const string& newname) = 0;

            //virtual auto create_record() const->record_ptr = 0;
            virtual void add_record(record_ptr record) = 0;

            virtual auto fields() const -> vector<FieldDescriptor> = 0;

            virtual void foreach_record(function< auto(record_ptr) -> bool > action) = 0;
        };
    }

    using table_ptr = unique_ptr<interface::Table>;

    namespace interface {
        struct Database {
            virtual ~Database() {}

            virtual void transaction(function< void() > action) = 0;

            virtual void create_table(const string& tablename) = 0;
            virtual void delete_table(const string& tablename) = 0;
            virtual auto open_table(const string& tablename) -> table_ptr = 0;
            virtual void rename_table(const string& oldname, const string& newname) = 0;

            virtual auto tables() const -> vector<TableDescriptor> = 0;
            virtual void with_table(const string& tablename, function<void(Table& table)>) = 0;
        };
    }

    using database_ptr = unique_ptr<interface::Database>;

    namespace interface {
        struct Session {
            virtual ~Session() {}

            virtual void create_database(const sys::path& path) = 0;
            virtual auto open_database(const sys::path& path) -> database_ptr = 0;
        };
    }

    using session_ptr = unique_ptr<interface::Session> ;

    auto make_session() -> session_ptr;

    void drop_database(const sys::path& path);

}