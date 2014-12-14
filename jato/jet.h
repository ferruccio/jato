#pragma once

#include <esent.h>

#include <functional>
#include <stdexcept>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

namespace jet {

    using std::make_shared;
    using std::shared_ptr;
    using std::string;
    using std::tuple;
    using std::vector;

    class error : public std::runtime_error {
    public:
        error(JET_ERR code, const char* origin)
            : _code(code), _origin(origin), std::runtime_error("Jet Error") {}

        auto code() const -> JET_ERR { return _code; }
        auto origin() const -> const char* { return _origin; }

    private:
        JET_ERR _code;
        const char* _origin;
    };

    using warning_handler = std::function < void(JET_ERR code, const char* origin) >;

    auto set_warning_handler(warning_handler handler) -> warning_handler;

    auto jet_error(JET_ERR code) -> const char*;

    //
    // jet::* API
    //
    auto add_column(
        JET_SESID session,
        JET_TABLEID table,
        const string& columnname,
        JET_COLUMNDEF* column_def, 
        void* def_value, 
        unsigned long dv_size
    ) -> JET_COLUMNID;

    void attach_database(JET_SESID session, const string& filename, JET_GRBIT bits);
    void attach_database(JET_SESID session, const string& filename, unsigned long db_maxsize, JET_GRBIT bits);
    auto begin_session(JET_INSTANCE instance) -> JET_SESID;
    auto begin_session(JET_INSTANCE instance, const string& username, const string& password) -> JET_SESID;
    void begin_transaction(JET_SESID session);
    void begin_transaction(JET_SESID session, JET_GRBIT bits);
    void close_database(JET_SESID session, JET_DBID db, JET_GRBIT bits);
    void close_table(JET_SESID session, JET_TABLEID table);
    void commit_transaction(JET_SESID session, JET_GRBIT bits);
    auto create_database(JET_SESID session, const string& filename) -> JET_DBID;
    auto create_database(JET_SESID session, const string& filename, const string& connect, JET_GRBIT bits) -> JET_DBID;
    auto create_database(JET_SESID session, const string& filename, unsigned long max_size, JET_GRBIT bits) -> JET_DBID;
    void create_index(JET_SESID session, JET_TABLEID table, const string& indexname,
        JET_GRBIT bits, const string& key, unsigned long density);
    void create_index(JET_SESID session, JET_TABLEID table,
        JET_INDEXCREATE* ixcreate, unsigned long ic_size);
    void create_index(JET_SESID session, JET_TABLEID table,
        JET_INDEXCREATE2* ixcreate, unsigned long ic_size);
    auto create_instance(const string& instancename) -> JET_INSTANCE;
    auto create_instance(const string& instancename, const string& displayname, JET_GRBIT bits) -> JET_INSTANCE;
    auto create_table(JET_SESID session, JET_DBID db, const string& tablename) -> JET_TABLEID;
    auto create_table(JET_SESID session, JET_DBID db, const string& tablename,
        unsigned long pages, unsigned long density) -> JET_TABLEID;
    void create_table_column_index(JET_SESID session, JET_DBID db, JET_TABLECREATE* table_create);
    void create_table_column_index(JET_SESID session, JET_DBID db, JET_TABLECREATE2* table_create);
    void create_table_column_index(JET_SESID session, JET_DBID db, JET_TABLECREATE3* table_create);
    void delete_column(JET_SESID session, JET_TABLEID table, const string& columnname);
    void delete_column(JET_SESID session, JET_TABLEID table, const string& columnname, JET_GRBIT bits);
    void delete_index(JET_SESID session, JET_TABLEID table, const string& indexname);
    void delete_record(JET_SESID session, JET_TABLEID table);
    void delete_table(JET_SESID session, JET_DBID db, const string& tablename);
    void detach_database(JET_SESID session, const string& filename);
    void detach_database(JET_SESID session, const string& filename, JET_GRBIT bits);
    auto dup_cursor(JET_SESID session, JET_TABLEID table, JET_GRBIT bits) -> JET_TABLEID;
    auto dup_session(JET_SESID session) -> JET_SESID;
    auto enable_multi_instance(JET_SETSYSPARAM* sysparam, unsigned long count) -> unsigned long;
    void end_session(JET_SESID session, JET_GRBIT bits);
    void end_session(JET_SESID session);

    auto enumerate_columns(
        JET_SESID session,
        JET_TABLEID table,
        unsigned long column,
        JET_ENUMCOLUMNID* enum_columnid,
        JET_PFNREALLOC realloc,
        void* realloc_context,
        unsigned long data_most,
        JET_GRBIT bits
    ) -> tuple<unsigned long, JET_ENUMCOLUMN*> ;

    void free_buffer(char* buffer);
    void init(JET_INSTANCE& instance);
    auto get_bookmark(JET_SESID session, JET_TABLEID table) -> vector < char > ;
    auto open_database(JET_SESID session, const string& filename) -> JET_DBID;
    auto open_table(JET_SESID session, JET_DBID db, const string& tablename) -> JET_TABLEID;
    void rename_table(JET_SESID session, JET_DBID db, const string& oldname, const string& newname);
    void term(JET_INSTANCE instance);

    class instance {
    public:
        instance() {
            init();
        }

        instance(const instance&) = delete;
        instance(const instance&&) = delete;

        ~instance() {
            try {
                shutdown();
            } catch (error&) {
                // TODO: log it?
            }
        }

        auto operator=(const instance&) -> instance& = delete;

        void init() {
            if (instance_id == 0) {
                instance_id = jet::create_instance("xyzzy");
                jet::init(instance_id);
            }
        }

        void shutdown() {
            if (instance_id != 0) {
                jet::term(instance_id);
            }
        }

        auto id() const -> JET_INSTANCE { return instance_id; }

    private:
        JET_INSTANCE instance_id = 0;
    };

    using instance_ptr = shared_ptr<instance>;

    class db;
    using db_ptr = shared_ptr<db>;

    class session : public std::enable_shared_from_this<session> {
    public:
        explicit session(instance_ptr instance) : instance(instance) {}

        session(const session&) = delete;
        session(const session&&) = delete;

        ~session() {
            try {
                end();
            } catch (error&) {
                // TODO: log it?
            }
        }

        auto operator=(const session&) -> session& = delete;

        void begin() {
            if (session_id == 0) {
                session_id = begin_session(instance->id());
            }
        }

        void end() {
            if (session_id != 0) {
                end_session(session_id);
            }
        }

        void create_db(const std::string& filename) {
            auto db_id = create_database(session_id, filename);
            close_database(session_id, db_id, 0);
        }

        auto open_db(const std::string& filename) -> db_ptr {
            return make_shared<db>(shared_from_this(), filename);
        }

        auto id() -> JET_SESID { return session_id; }

    private:
        instance_ptr instance;
        JET_SESID session_id = 0;
    };

    using session_ptr = shared_ptr<session>;

    class db {
    public:
        db(session_ptr session, const string& filename)
            : session(session), filename(filename) {

            attach_database(session->id(), filename, 0);
            db_id = open_database(session->id(), filename);
        }

        db(const db&) = delete;
        db(const db&&) = delete;
        
        ~db() {}

        auto operator=(const db&) -> db& = delete;

        auto id() const->JET_DBID { return db_id; }

    private:
        string filename;
        session_ptr session;
        JET_DBID db_id = 0;
    };
}
