#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

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

    using field_type = int; // for now

    struct FieldDescriptor {
        string name;
        field_type type;
    };

    struct TableDescriptor {
        string name;
    };

    namespace interface {
        struct Field {
            virtual ~Field() {}

        };
    }

    using field_ptr = unique_ptr < interface::Field >;

    namespace interface {
        struct Record {
            virtual ~Record() {}

            virtual void set_field(const string& fieldname, field_ptr field) = 0;
            virtual auto get_field(const string& fieldname) const->field_ptr = 0;
        };
    }

    using record_ptr = unique_ptr < interface::Record >;

    namespace interface {
        struct Table {
            virtual ~Table() {}

            virtual void create_field(const string& name, field_type type) = 0;
            virtual void delete_field(const string& name) = 0;
            virtual void rename_field(const string& oldname, const string& newname) = 0;

            //virtual auto create_record() const->record_ptr = 0;
            virtual void add_record(record_ptr record) = 0;

            virtual auto fields() const->vector < FieldDescriptor > = 0;

            virtual void foreach_record(function<auto(record_ptr)->bool> action) = 0;
        };
    }

    using table_ptr = unique_ptr < interface::Table >;

    namespace interface {
        struct Database {
            virtual ~Database() {}

            virtual void transaction(function<void()> action) = 0;

            virtual void create_table(const string& tablename) = 0;
            virtual void delete_table(const string& tablename) = 0;
            virtual auto open_table(const string& tablename)->table_ptr = 0;
            virtual void rename_table(const string& oldname, const string& newname) = 0;

            virtual auto tables() const->vector<TableDescriptor> = 0;
            virtual void with_table(const string& tablename, function<void(Table& table)>) = 0;
        };
    }

    using database_ptr = unique_ptr < interface::Database >;

    namespace interface {
        struct Session {
            virtual ~Session() {}

            virtual void create_database(const sys::path& path) = 0;
            virtual auto open_database(const sys::path& path)->database_ptr = 0;
        };
    }

    using session_ptr = unique_ptr < interface::Session > ;

    auto make_session()->session_ptr;

    void drop_database(const sys::path& path);

}