#pragma once

#include <esent.h>

#include <stdexcept>
#include <string>

namespace jet {

    using std::string;

    class error : public std::runtime_error {
    public:
        error(JET_ERR code, const char* origin) : _code(code), _origin(origin), std::runtime_error("Jet Error") {}

        auto code() const -> JET_ERR { return _code; }
        auto origin() const -> const char* { return _origin; }

    private:
        JET_ERR _code;
        const char* _origin;
    };

    void attach_database(JET_SESID session, const string& filename);
    auto begin_session(JET_INSTANCE instance) -> JET_SESID;
    void close_database(JET_SESID session, JET_DBID db);
    void close_table(JET_SESID session, JET_TABLEID table);
    auto create_database(JET_SESID session, const string& filename) -> JET_DBID;
    auto create_instance(const string& instance_name) -> JET_INSTANCE;
    auto create_table(JET_SESID session, JET_DBID db, const string& tablename) -> JET_TABLEID;
    void delete_table(JET_SESID session, JET_DBID db, const string& tablename);
    void end_session(JET_SESID session);
    void init(JET_INSTANCE& instance);
    auto open_database(JET_SESID session, const string& filename) -> JET_DBID;
    auto open_table(JET_SESID session, JET_DBID db, const string& tablename) -> JET_TABLEID;
    void rename_table(JET_SESID session, JET_DBID db, const string& oldname, const string& newname);
    void term(JET_INSTANCE instance);

}
