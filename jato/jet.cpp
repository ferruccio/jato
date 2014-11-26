#include "jet.h"

#include <string>
#include <tuple>

namespace jet {

    using std::string;
    using std::tuple;
    using std::make_tuple;
    using std::vector;

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

    auto add_column(JET_SESID session, JET_TABLEID table, const string& columnname,
        JET_COLUMNDEF* column_def, void* def_value, unsigned long dv_size) -> JET_COLUMNID {
        JET_COLUMNID column = 0;
        handle_errors(
            "jet::add_column",
            JetAddColumn(session, table, columnname.c_str(), column_def, def_value, dv_size, &column));
        return column;
    }

    void attach_database(JET_SESID session, const string& filename, JET_GRBIT bits) {
        handle_errors(
            "jet::attach_database(1)",
            JetAttachDatabase(session, filename.c_str(), bits));
    }

    void attach_database(JET_SESID session, const string& filename, unsigned long db_maxsize, JET_GRBIT bits) {
        handle_errors(
            "jet::attach_database(2)",
            JetAttachDatabase2(session, filename.c_str(), db_maxsize, bits));
    }

    void backup(const string& backup_path, JET_GRBIT bits, JET_PFNSTATUS status) {
        handle_errors(
            "jet::backup",
            JetBackup(backup_path.c_str(), bits, status));
    }

    void backup_instance(JET_INSTANCE instance, const string& backup_path, JET_GRBIT bits, JET_PFNSTATUS status) {
        handle_errors(
            "jet::backup_instance",
            JetBackupInstance(instance, backup_path.c_str(), bits, status));
    }

    void begin_external_backup(JET_GRBIT bits) {
        handle_errors(
            "jet::begin_external_backup",
            JetBeginExternalBackup(bits));
    }

    void begin_external_backup_instance(JET_INSTANCE instance, JET_GRBIT bits) {
        handle_errors(
            "jet::begin_external_backup_instance",
            JetBeginExternalBackupInstance(instance, bits));
    }

    auto begin_session(JET_INSTANCE instance) -> JET_SESID {
        JET_SESID sesid = 0;
        handle_errors(
            "jet::begin_session(1)",
            JetBeginSession(instance, &sesid, NULL, NULL));
        return sesid;
    }

    auto begin_session(JET_INSTANCE instance, const string& username, const string& password) -> JET_SESID {
        JET_SESID sesid = 0;
        handle_errors(
            "jet::begin_session(2)",
            JetBeginSession(instance, &sesid, username.c_str(), password.c_str()));
        return sesid;
    }

    void begin_transaction(JET_SESID session) {
        handle_errors(
            "jet::begin_transaction(1)",
            JetBeginTransaction(session));
    }

    void begin_transaction(JET_SESID session, JET_GRBIT bits) {
        handle_errors(
            "jet::begin_transaction(2)",
            JetBeginTransaction2(session, bits));
    }

    void begin_transaction(JET_SESID session, long long txid, JET_GRBIT bits) {
        handle_errors(
            "jet::begin_transaction(3)",
            JetBeginTransaction3(session, txid, bits));
    }

    void close_database(JET_SESID session, JET_DBID db, JET_GRBIT bits) {
        handle_errors(
            "jet::close_database",
            JetCloseDatabase(session, db, bits));
    }

    void close_file(JET_HANDLE file) {
        handle_errors(
            "jet::close_file",
            JetCloseFile(file));
    }

    void close_file_instance(JET_INSTANCE instance, JET_HANDLE file) {
        handle_errors(
            "jet::close_file_instance",
            JetCloseFileInstance(instance, file));
    }

    void close_table(JET_SESID session, JET_TABLEID table) {
        handle_errors(
            "jet::close_table",
            JetCloseTable(session, table));
    }

    void commit_transaction(JET_SESID session, JET_GRBIT bits) {
        handle_errors(
            "jet::commit_transaction(1)",
            JetCommitTransaction(session, bits));
    }

    auto commit_transaction(JET_SESID session, JET_GRBIT bits, unsigned long duration) -> JET_COMMIT_ID {
        JET_COMMIT_ID commit;
        handle_errors(
            "jet::commit_transaction(2)",
            JetCommitTransaction2(session, bits, duration, &commit));
        return commit;
    }

    void compact(JET_SESID session, const string& src, const string& dst,
        JET_PFNSTATUS status, JET_CONVERT* convert, JET_GRBIT bits) {
        handle_errors(
            "jet::compact",
            JetCompact(session, src.c_str(), dst.c_str(), status, convert, bits));
    }

    void compute_stats(JET_SESID session, JET_TABLEID table) {
        handle_errors(
            "jet::compute_stats",
            JetComputeStats(session, table));
    }

    auto create_database(JET_SESID session, const string& filename) -> JET_DBID {
        JET_DBID db = 0;
        handle_errors(
            "jet::create_database(1)",
            JetCreateDatabase(session, filename.c_str(), NULL, &db, 0));
        return db;
    }

    auto create_database(JET_SESID session, const string& filename, const string& connect, JET_GRBIT bits) -> JET_DBID {
        JET_DBID db = 0;
        handle_errors(
            "jet::create_database(2)",
            JetCreateDatabase(session, filename.c_str(), connect.c_str(), &db, bits));
        return db;
    }

    auto create_database(JET_SESID session, const string& filename, unsigned long max_size, JET_GRBIT bits) -> JET_DBID {
        JET_DBID db = 0;
        handle_errors(
            "jet::create_database(3)",
            JetCreateDatabase2(session, filename.c_str(), max_size, &db, bits));
        return db;
    }

    void create_index(JET_SESID session, JET_TABLEID table, const string& indexname,
        JET_GRBIT bits, const string& key, unsigned long density) {
        handle_errors(
            "jet::create_index(1)",
            JetCreateIndex(session, table, indexname.c_str(), bits,
            key.c_str(), key.length(), density));
    }

    void create_index(JET_SESID session, JET_TABLEID table,
        JET_INDEXCREATE* ixcreate, unsigned long ic_size) {
        handle_errors(
            "jet::create_index(2)",
            JetCreateIndex2(session, table, ixcreate, ic_size));
    }

    void create_index(JET_SESID session, JET_TABLEID table,
        JET_INDEXCREATE2* ixcreate, unsigned long ic_size) {
        handle_errors(
            "jet::create_index(3)",
            JetCreateIndex3(session, table, ixcreate, ic_size));
    }

    auto create_instance(const string& instancename) -> JET_INSTANCE {
        JET_INSTANCE instance = 0;
        handle_errors(
            "jet::create_instance(1)",
            JetCreateInstance(&instance, instancename.c_str()));
        return instance;
    }

    auto create_instance(const string& instancename, const string& displayname, JET_GRBIT bits) -> JET_INSTANCE {
        JET_INSTANCE instance = 0;
        handle_errors(
            "jet::create_instance(2)",
            JetCreateInstance2(&instance, instancename.c_str(), displayname.c_str(), bits));
        return instance;
    }

    auto create_table(JET_SESID session, JET_DBID db, const string& tablename) -> JET_TABLEID {
        JET_TABLEID table = 0;
        handle_errors(
            "jet::create_table(1)",
            JetCreateTable(session, db, tablename.c_str(), 0, 0, &table));
        return table;
    }

    auto create_table(JET_SESID session, JET_DBID db, const string& tablename,
        unsigned long pages, unsigned long density) -> JET_TABLEID {
        JET_TABLEID table = 0;
        handle_errors(
            "jet::create_table(2)",
            JetCreateTable(session, db, tablename.c_str(), pages, density, &table));
        return table;
    }

    void create_table_column_index(JET_SESID session, JET_DBID db, JET_TABLECREATE* table_create) {
        handle_errors(
            "jet::create_table_column_index(1)",
            JetCreateTableColumnIndex(session, db, table_create));
    }

    void create_table_column_index(JET_SESID session, JET_DBID db, JET_TABLECREATE2* table_create) {
        handle_errors(
            "jet::create_table_column_index(2)",
            JetCreateTableColumnIndex2(session, db, table_create));
    }

    void create_table_column_index(JET_SESID session, JET_DBID db, JET_TABLECREATE3* table_create) {
        handle_errors(
            "jet::create_table_column_index(3)",
            JetCreateTableColumnIndex3(session, db, table_create));
    }

    void defragment(JET_SESID session, JET_DBID db, const string& tablename,
        unsigned long* passes, unsigned long* seconds, JET_GRBIT bits) {
        handle_errors(
            "jet::defragment(1)",
            JetDefragment(session, db, tablename.c_str(), passes, seconds, bits));
    }

    void defragment(JET_SESID session, JET_DBID db, const string& tablename,
        unsigned long* passes, unsigned long* seconds, JET_CALLBACK callback, JET_GRBIT bits) {
        handle_errors(
            "jet::defragment(2)",
            JetDefragment2(session, db, tablename.c_str(), passes, seconds, callback, bits));
    }

    void delete_column(JET_SESID session, JET_TABLEID table, const string& columnname) {
        handle_errors(
            "jet::delete_column(1)",
            JetDeleteColumn(session, table, columnname.c_str()));
    }

    void delete_column(JET_SESID session, JET_TABLEID table, const string& columnname, JET_GRBIT bits) {
        handle_errors(
            "jet::delete_column(2)",
            JetDeleteColumn2(session, table, columnname.c_str(), bits));
    }

    void delete_index(JET_SESID session, JET_TABLEID table, const string& indexname) {
        handle_errors(
            "jet::delete_index",
            JetDeleteIndex(session, table, indexname.c_str()));
    }

    void delete_record(JET_SESID session, JET_TABLEID table) {
        handle_errors(
            "jet::delete_record",
            JetDelete(session, table));
    }

    void delete_table(JET_SESID session, JET_DBID db, const string& tablename) {
        handle_errors(
            "jet::delete_table",
            JetDeleteTable(session, db, tablename.c_str()));
    }

    void detach_database(JET_SESID session, const string& filename) {
        handle_errors(
            "jet::detach_database(1)",
            JetDetachDatabase(session, filename.c_str()));
    }

    void detach_database(JET_SESID session, const string& filename, JET_GRBIT bits) {
        handle_errors(
            "jet::detach_database(2)",
            JetDetachDatabase2(session, filename.c_str(), bits));
    }

    auto dup_cursor(JET_SESID session, JET_TABLEID table, JET_GRBIT bits) -> JET_TABLEID {
        JET_TABLEID dup_table = 0;
        handle_errors(
            "jet::dup_cursor",
            JetDupCursor(session, table, &dup_table, bits));
        return dup_table;
    }

    auto dup_session(JET_SESID session) -> JET_SESID {
        JET_SESID _dup_session = 0;
        handle_errors(
            "jet::dup_session",
            JetDupSession(session, &_dup_session));
        return _dup_session;
    }

    auto enable_multi_instance(JET_SETSYSPARAM* sysparam, unsigned long count) -> unsigned long {
        unsigned long set_succeed;
        handle_errors(
            "jet::enable_multi_instance",
            JetEnableMultiInstance(sysparam, count, &set_succeed));
        return set_succeed;
    }

    void end_external_backup() {
        handle_errors(
            "jet::end_external_backup",
            JetEndExternalBackup());
    }

    void end_external_backup_instance(JET_INSTANCE instance) {
        handle_errors(
            "jet::end_external_backup_instance(1)",
            JetEndExternalBackupInstance(instance));
    }

    void end_external_backup_instance(JET_INSTANCE instance, JET_GRBIT bits) {
        handle_errors(
            "jet::end_external_backup_instance(2)",
            JetEndExternalBackupInstance2(instance, bits));
    }

    void end_session(JET_SESID session, JET_GRBIT bits) {
        handle_errors(
            "jet::end_session",
            JetEndSession(session, bits));
    }

    void end_session(JET_SESID session) {
        handle_errors(
            "jet::end_session",
            JetEndSession(session, 0));
    }

    auto enumerate_columns(
        JET_SESID session,
        JET_TABLEID table,
        unsigned long column,
        JET_ENUMCOLUMNID* enum_columnid,
        JET_PFNREALLOC realloc,
        void* realloc_context,
        unsigned long data_most,
        JET_GRBIT bits) -> tuple < unsigned long, JET_ENUMCOLUMN* >
    {
        unsigned long enum_column_count = 0;
        JET_ENUMCOLUMN* enum_column = nullptr;
        handle_errors(
            "jet::enumerate_columns",
            JetEnumerateColumns(session, table, column, enum_columnid,
                                &enum_column_count, &enum_column,
                                realloc, realloc_context, data_most, bits));
        return make_tuple(enum_column_count, enum_column);
    }

    // escrow_udpate()

    // external_restore()

    void free_buffer(char* buffer) {
        handle_errors(
            "jet::free_buffer",
            JetFreeBuffer(buffer));
    }

    // get_attach_info

    // get_attach_info_instance

    void init(JET_INSTANCE& instance) {
        handle_errors(
            "jet::init",
            JetInit(&instance));
    }

    auto get_bookmark(JET_SESID session, JET_TABLEID table) -> vector < char > {
        unsigned long actual_size = 0;
        handle_errors(
            "jet::get_bookmark(1)",
            JetGetBookmark(session, table, NULL, 0, &actual_size));
        vector<char> bookmark(actual_size);
        handle_errors(
            "jet::get_bookmark(2)",
            JetGetBookmark(session, table, bookmark.data(), bookmark.size(), nullptr));
        return bookmark;
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