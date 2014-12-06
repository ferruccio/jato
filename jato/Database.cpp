#include "jato.h"
#include "jet.h"

#include <filesystem>
#include <sstream>
#include <utility>

namespace jato {

    using std::function;
    using std::move;
    using std::make_shared;
    using std::make_unique;
    using std::string;

    namespace sys = std::tr2::sys;

    namespace {
        void jet_context(function<void()> action) {
            try {
                action();
            } catch (jet::error& ex) {
                std::stringstream ss;
                ss << "Jet Error(" << ex.origin() << ") code=" << ex.code();
                throw jato::error(ss.str());
            }
        }
    }

    auto make_table(jet::instance_ptr instance,
        jet::session_ptr session,
        JET_TABLEID table_id)->table_ptr;

    class database_impl : public interface::Database {
    public: // interface
        void transaction(function<void()> action) final override {}

        void create_table(const string& tablename) final override {
            jet_context([&](){
                auto table_id = jet::create_table(session->id(), data->id(), tablename);
                jet::close_table(session->id(), table_id);
            });
        }

        void delete_table(const string& tablename) final override {
            jet_context([&](){
                jet::delete_table(session->id(), data->id(), tablename);
            });
        }

        auto open_table(const string& tablename)->table_ptr final override {
            JET_TABLEID table_id = 0;
            jet_context([&](){
                table_id = jet::open_table(session->id(), data->id(), tablename);
            });
            return make_table(instance, session, table_id);
        }

        void rename_table(const string& oldname, const string& newname) final override {
            jet_context([&](){
                jet::rename_table(session->id(), data->id(), oldname, newname);
            });
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
            data = make_shared<jet::db>(session, path);
        }

    private:
        jet::instance_ptr instance;
        jet::session_ptr session;
        jet::db_ptr data;
    };

    class session_impl : public interface::Session {
    public:
        session_impl() {
            instance = make_shared<jet::instance>();
            session = make_shared<jet::session>(instance);
            session->begin();
        }

    public: // interface
        void create_database(const sys::path& path) final override {
            jet_context([&](){
                session->create_db(path);
            });
        }

        auto open_database(const sys::path& path)->database_ptr final override {
            unique_ptr<database_impl> database;
            jet_context([&](){
                session->begin();
                database = make_unique<database_impl>(instance, session);
                database->open(path);
            });
            return move(database);
        }

    private:
        jet::instance_ptr instance;
        jet::session_ptr session;
    };

    auto make_session()->session_ptr {
        return make_unique<session_impl>();
    }

    void drop_database(const sys::path& path) {
        if (!sys::remove(path))
            throw error("[drop_database] cannot delete database");
    }

}
