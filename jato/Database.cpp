#include "jato.h"
#include "jet.h"

#include <filesystem>
#include <utility>

namespace jato {

    using std::function;
    using std::move;
    using std::make_shared;
    using std::make_unique;
    using std::string;

    namespace sys = std::tr2::sys;

    const sys::path data_db = "data.edb";
    const sys::path files_db = "files.edb";
    const sys::path index_db = "index.edb";

    auto make_table(jet::instance_ptr instance,
        jet::session_ptr session,
        JET_TABLEID table_id)->table_ptr;

    class database_impl : public interface::Database {
    public: // interface
        void transaction(function<void()> action) final override {}

        void create_table(const string& tablename) final override {
            auto table_id = jet::create_table(session->id(), data->id(), tablename);
            jet::close_table(session->id(), table_id);
        }

        void delete_table(const string& tablename) final override {
            jet::delete_table(session->id(), data->id(), tablename);
        }

        auto open_table(const string& tablename)->table_ptr final override {
            auto table_id = jet::open_table(session->id(), data->id(), tablename);
            return make_table(instance, session, table_id);
        }

        void rename_table(const string& oldname, const string& newname) final override {
            jet::rename_table(session->id(), data->id(), oldname, newname);
        }

        auto tables() const->vector < TableDescriptor > final override {
            return vector<TableDescriptor>();
        }

        void with_table(const string& tablename, function<void(interface::Table& table)>) final override {
        }

    public:
        database_impl(jet::instance_ptr instance, jet::session_ptr session)
            : instance(instance), session(session) {}

        void open(const sys::path& path) {
            data = make_shared<jet::db>(session, path / data_db);
            files = make_shared<jet::db>(session, path / files_db);
            index = make_shared<jet::db>(session, path / index_db);
        }

    private:
        jet::instance_ptr instance;
        jet::session_ptr session;
        jet::db_ptr data;
        jet::db_ptr files;
        jet::db_ptr index;
    };

    namespace {

        void with_session(function<void(jet::session_ptr session)> action) {
            auto instance = make_shared<jet::instance>();
            auto session = make_shared<jet::session>(instance);
            session->begin();
            action(session);
        }

    }

    void create_database(const sys::path& path) {
        if (!sys::create_directories(path))
            throw error("[create_database] database path already exists: " + path.string());
        with_session([&](jet::session_ptr session) {
            session->create_db(path / data_db);
            session->create_db(path / files_db);
            session->create_db(path / index_db);
        });
    }

    void drop_database(const sys::path& path) {
        if (!sys::exists(path))
            throw error("[drop_database] database path does not exist: " + path.string());
        if (!sys::is_directory(path))
            throw error("[drop_database] not a directory: " + path.string());
        if (!sys::remove(path / data_db))
            throw error("[drop_database] cannot delete data");
        if (!sys::remove(path / files_db))
            throw error("[drop_database] cannot delete files");
        if (!sys::remove(path / index_db))
            throw error("[drop_database] cannot delete index");
        if (!sys::remove_directory(path))
            throw error("[drop_database] cannot remove directory: " + path.string());
    }

    auto open_database(const sys::path& path)->database_ptr {
        try {
            auto instance = make_shared<jet::instance>();
            auto session = make_shared<jet::session>(instance);
            session->begin();
            auto database = make_unique<database_impl>(instance, session);
            database->open(path);
            return move(database);
        } catch (jet::error& ex) {
            throw jato::error(ex.what());
        }
    }

}
