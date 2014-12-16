#include "jato.h"
#include "jet.h"

#include <filesystem>
#include <memory>
#include <utility>

namespace jato {

    using std::make_unique;

    class table_impl : public interface::Table {
    public: // interface
        void create_field(const string& name, field_type type) final override {

        }

        void delete_field(const string& name) final override {

        }

        void rename_field(const string& oldname, const string& newname) final override {

        }

        //auto create_record() const->record_ptr final override {
        //}

        void add_record(record_ptr record) final override {

        }

        auto fields() const -> vector<FieldDescriptor> final override {
            return vector<FieldDescriptor>();
        }

        void foreach_record(function< auto(record_ptr) -> bool > action) final override {

        }

    public:
        table_impl(jet::instance_ptr instance, jet::session_ptr session, JET_TABLEID table_id)
            : instance(instance), session(session), table_id(table_id) {}

        auto id() const -> JET_TABLEID { return table_id; }

    private:
        jet::instance_ptr instance;
        jet::session_ptr session;
        JET_TABLEID table_id;
    };

    auto make_table(jet::instance_ptr instance,
        jet::session_ptr session,
        JET_TABLEID table_id
    ) -> table_ptr {
        return make_unique<table_impl>(instance, session, table_id);
    }

}