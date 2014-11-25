#include "jet.h"

#include <string>

namespace jet {

    using std::string;

    namespace {
        warning_handler handle_warning;
    }

    auto set_warning_handler(warning_handler handler) -> warning_handler {
        auto old_handler = handle_warning;
        handle_warning = handler;
        return old_handler;
    }

    void handle_errors(const char* origin, JET_ERR code) {
        if (code == JET_errSuccess) return;
        if (code < JET_errSuccess) throw error(code, origin);
        if (handle_warning) handle_warning(code, origin);
    }

    void attach_database(JET_SESID session, const string& filename) {
        handle_errors(
            "jet::attach_database",
            JetAttachDatabase(session, filename.c_str(), 0));
    }

    auto begin_session(JET_INSTANCE instance) -> JET_SESID {
        JET_SESID sesid = 0;
        handle_errors(
            "jet::begin_session",
            JetBeginSession(instance, &sesid, NULL, NULL));
        return sesid;
    }

    void close_database(JET_SESID session, JET_DBID db) {
        handle_errors(
            "jet::close_database",
            JetCloseDatabase(session, db, 0));
    }

    void close_table(JET_SESID session, JET_TABLEID table) {
        handle_errors(
            "jet::close_table",
            JetCloseTable(session, table));
    }

    auto create_database(JET_SESID session, const string& filename) -> JET_DBID {
        JET_DBID db = 0;
        handle_errors(
            "jet::create_database",
            JetCreateDatabase(session, filename.c_str(), NULL, &db, 0));
        return db;
    }

    auto create_instance(const string& instance_name) -> JET_INSTANCE {
        JET_INSTANCE instance = 0;
        handle_errors(
            "jet::create_instance",
            JetCreateInstance(&instance, instance_name.c_str()));
        return instance;
    }

    auto create_table(JET_SESID session, JET_DBID db, const string& tablename) -> JET_TABLEID {
        JET_TABLEID table = 0;
        handle_errors(
            "jet::create_table",
            JetCreateTable(session, db, tablename.c_str(), 0, 0, &table));
        return table;
    }

    void delete_table(JET_SESID session, JET_DBID db, const string& tablename) {
        handle_errors(
            "jet::delete_table",
            JetDeleteTable(session, db, tablename.c_str()));
    }

    void end_session(JET_SESID session) {
        handle_errors(
            "jet::end_session",
            JetEndSession(session, 0));
    }

    void init(JET_INSTANCE& instance) {
        handle_errors(
            "jet::init",
            JetInit(&instance));
    }

    auto open_database(JET_SESID session, const string& filename) -> JET_DBID {
        JET_DBID db = 0;
        handle_errors(
            "jet::open_database",
            JetOpenDatabase(session, filename.c_str(), NULL, &db, 0));
        return db;
    }

    auto open_table(JET_SESID session, JET_DBID db, const string& tablename) -> JET_TABLEID {
        JET_TABLEID table = 0;
        handle_errors(
            "jet::open_table",
            JetOpenTable(session, db, tablename.c_str(), NULL, 0, JET_bitTableUpdatable, &table));
        return table;
    }

    void rename_table(JET_SESID session, JET_DBID db, const string& oldname, const string& newname) {
        handle_errors(
            "jet::rename_table",
            JetRenameTable(session, db, oldname.c_str(), newname.c_str()));
    }

    void term(JET_INSTANCE instance) {
        handle_errors(
            "jet::term",
            JetTerm(instance));
    }

}