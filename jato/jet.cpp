#include "jet.h"

#include <string>

namespace jet {

    using std::string;

    void attach_database(JET_SESID session, const string& filename) {
        auto err = JetAttachDatabase(session, filename.c_str(), 0);
        switch (err) {
        case JET_errSuccess:
        case JET_wrnDatabaseAttached:
            return;
        case JET_errBackupInProgress:
        case JET_errDatabaseFileReadOnly:
        case JET_errDatabaseInUse:
        case JET_errDatabaseInvalidPath:
        case JET_errDatabaseSharingViolation:
        case JET_errFileAccessDenied:
        case JET_errFileNotFound:
        case JET_errPrimaryIndexCorrupted:
        case JET_errSecondaryIndexCorrupted:
        case JET_errTooManyAttachedDatabases:
        default:
            throw error(err, "jet::attach_database");
        }
    }

    auto begin_session(JET_INSTANCE instance) -> JET_SESID {
        JET_SESID sesid = 0;
        auto err = JetBeginSession(instance, &sesid, NULL, NULL);
        switch (err) {
        case JET_errSuccess:
            return sesid;
        case JET_errClientRequestToStopJetService:
        case JET_errInstanceUnavailable:
        case JET_errInvalidParameter:
        case JET_errNotInitialized:
        case JET_errOutOfMemory:
        case JET_errOutOfSessions:
        case JET_errRestoreInProgress:
        case JET_errTermInProgress:
        default:
            throw error(err, "jet::begin_session");
        }
    }

    void close_database(JET_SESID session, JET_DBID db) {
        auto err = JetCloseDatabase(session, db, 0);
        switch (err) {
        case JET_errSuccess:
            return;
        case JET_errDatabaseNotFound:
        case JET_errInvalidDatabaseId:
        default:
            throw error(err, "jet::close_database");
        }
    }

    void close_table(JET_SESID session, JET_TABLEID table) {
        auto err = JetCloseTable(session, table);
        switch (err) {
        case JET_errSuccess:
            return;
        default:
            throw error(err, "jet::close_table");
        }
    }

    auto create_database(JET_SESID session, const string& filename) -> JET_DBID {
        JET_DBID db = 0;
        auto err = JetCreateDatabase(session, filename.c_str(), NULL, &db, 0);
        switch (err) {
        case JET_errSuccess:
            return db;
        case JET_errDatabaseDuplicate:
        case JET_errDatabaseInUse:
        case JET_errDatabaseInvalidPages:
        case JET_errDatabaseInvalidPath:
        case JET_errDatabaseLocked:
        case JET_errDatabaseNotFound:
        case JET_errDatabaseSharingViolation:
        case JET_errInTransaction:
        case JET_errOutOfMemory:
        case JET_errTooManyAttachedDatabases:
        case JET_wrnDatabaseAttached:
        case JET_wrnFileOpenReadOnly:
        default:
            throw error(err, "jet::create_database");
        }
    }

    auto create_instance(const string& instance_name) -> JET_INSTANCE {
        JET_INSTANCE instance = 0;
        auto err = JetCreateInstance(&instance, instance_name.c_str());
        switch (err) {
        case JET_errSuccess:
            return instance;
        case JET_errInstanceNameInUse:
        case JET_errInvalidParameter:
        case JET_errRunningInOneInstanceMode:
        case JET_errTooManyInstances:
        default:
            throw error(err, "jet::create_instance");
        }
    }

    auto create_table(JET_SESID session, JET_DBID db, const string& tablename) -> JET_TABLEID {
        JET_TABLEID table = 0;
        auto err = JetCreateTable(session, db, tablename.c_str(), 0, 0, &table);
        switch (err) {
        case JET_errSuccess:
            return table;
        case JET_errCallbackNotResolved:
        case JET_errCannotIndex:
        case JET_errCannotNestDDL:
        case JET_errColumnDuplicate:
        case JET_errColumnNotFound:
        case JET_errColumnRedundant:
        case JET_errDensityInvalid:
        case JET_errDDLNotInheritable:
        case JET_errIndexDuplicate:
        case JET_errIndexHasPrimary:
        case JET_errIndexInvalidDef:
        case JET_errIndexTuplesInvalidLimits:
        case JET_errIndexTuplesNonUniqueOnly:
        case JET_errIndexTuplesOneColumnOnly:
        case JET_errIndexTuplesSecondaryIndexOnly:
        case JET_errIndexTuplesVarSegMacNotAllowed:
        case JET_errIndexTuplesTextColumnsOnly:
        case JET_errInTransaction:
        case JET_errInvalidCodePage:
        case JET_errInvalidColumnType:
        case JET_errInvalidCreateIndex:
        case JET_errInvalidGrbit:
        case JET_errInvalidLanguageId:
        case JET_errInvalidParameter:
        case JET_errRecordTooBig:
        case JET_errTableDuplicate:
        case JET_errTooManyColumns:
        case JET_errUnicodeTranslationFail:
        default:
            throw error(err, "jet::create_table");
        }
    }

    void delete_table(JET_SESID session, JET_DBID db, const string& tablename) {
        auto err = JetDeleteTable(session, db, tablename.c_str());
        switch (err) {
        case JET_errSuccess:
            return;
        case JET_errTableInUse:
        case JET_errCannotDeleteTempTable:
        case JET_errCannotDeleteSystemTable:
        case JET_errCannotDeleteTemplateTable:
        default:
            throw error(err, "jet::delete_table");
        }
    }

    void end_session(JET_SESID session) {
        auto err = JetEndSession(session, 0);
        switch (err) {
        case JET_errSuccess:
            return;
        case JET_errClientRequestToStopJetService:
        case JET_errInvalidParameter:
        case JET_errInvalidSesid:
        case JET_errNotInitialized:
        case JET_errOutOfMemory:
        case JET_errSessionInUse:
        case JET_errInstanceUnavailable:
        case JET_errOutOfBuffers:
        case JET_errRestoreInProgress:
        case JET_errTermInProgress:
        default:
            throw error(err, "jet::end_session");
        }
    }

    void init(JET_INSTANCE& instance) {
        auto err = JetInit(&instance);
        switch (err) {
        case JET_errSuccess:
            return;
        case JET_errReadVerifyFailure:
        case JET_errLogFileCorrupt:
        case JET_errCheckpointCorrupt:
        case JET_errMissingLogFile:
        case JET_errAttachedDatabaseMismatch:
        case JET_errDatabaseSharingViolation:
        case JET_errInvalidLogSequence:
        default:
            throw error(err, "jet::init");
        }
    }

    auto open_database(JET_SESID session, const string& filename) -> JET_DBID {
        JET_DBID db = 0;
        auto err = JetOpenDatabase(session, filename.c_str(), NULL, &db, 0);
        switch (err) {
        case JET_wrnFileOpenReadOnly: // TODO: handle this
        case JET_errSuccess:
            return db;
        case JET_errDatabaseInUse:
        case JET_errDatabaseInvalidPath:
        case JET_errDatabaseLocked:
        case JET_errDatabaseNotFound:
        case JET_errInvalidDatabase:
        case JET_errOneDatabasePerSession:
        default:
            throw error(err, "jet::open_database");
        }
    }

    auto open_table(JET_SESID session, JET_DBID db, const string& tablename) -> JET_TABLEID {
        JET_TABLEID table = 0;
        auto err = JetOpenTable(session, db, tablename.c_str(), NULL, 0, JET_bitTableUpdatable, &table);
        switch (err) {
        case JET_errSuccess:
            return table;
        case JET_errInvalidDatabaseId:
        case JET_errInvalidGrbit:
        case JET_errInvalidName:
        case JET_errObjectNotFound:
        case JET_errOutOfCursors:
        case JET_errTableInUse:
        case JET_wrnTableInUseBySystem:
        case JET_errTableLocked:
        case JET_errTooManyOpenTables:
        default:
            throw error(err, "jet::open_table");
        }
    }

    void rename_table(JET_SESID session, JET_DBID db, const string& oldname, const string& newname) {
        auto err = JetRenameTable(session, db, oldname.c_str(), newname.c_str());
        switch (err) {
        case JET_errSuccess:
            return;
        case JET_errClientRequestToStopJetService:
        case JET_errInstanceUnavailable:
        case JET_errInvalidDatabase:
        case JET_errInvalidDatabaseId:
        case JET_errInvalidName:
        case JET_errInvalidParameter:
        case JET_errNotInitialized:
        case JET_errObjectNotFound:
        case JET_errRestoreInProgress:
        case JET_errSessionSharingViolation:
        case JET_errTermInProgress:
        case JET_errTransReadOnly:
        default:
            throw error(err, "jet::rename_table");
        }
    }

    void term(JET_INSTANCE instance) {
        auto err = JetTerm(instance);
        switch (err) {
        case JET_errSuccess:
            return;
        case JET_errInvalidParameter:
        case JET_errNotInitialized:
        case JET_errTermInProgress:
        case JET_errRestoreInProgress:
        case JET_errBackupInProgress:
        case JET_errTooManyActiveUsers:
        default:
            throw error(err, "jet::term");
        }
    }

}