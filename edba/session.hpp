#ifndef EDBA_SESSION_HPP
#define EDBA_SESSION_HPP

#include <edba/statement.hpp>

namespace edba {

///
/// \brief SQL session object that represents a single connection and is the gateway to SQL database
///
/// It is the main class that is used for access to the DB, it uses various singleton classes to
/// load drivers open connections and cache them. 
///
class EDBA_API session {
public:

    ///
    /// Create an empty session object, it should not be used until it is opened with calling open() function.
    ///
    session();

    ///
    /// Create a session using a pointer to backend::connection.
    ///
    session(const boost::intrusive_ptr<backend::connection>& conn);

    ///
    /// Close current connection.
    ///
    void close();
    ///
    /// Check if the session was opened.
    ///
    bool is_open();

    ///
    /// Create a new statement, by default is creates prepared statement - create_prepared_statement() unless \@use_prepared connection string
    /// property is set to off, then it uses normal statements by calling create_statement()
    ///
    /// This is the most convenient function to create statements with.
    ///
    statement prepare(const string_ref& query);

    ///
    /// Create ordinary statement it generally unprepared statement and it is never cached. It should
    /// be used when such statement is executed rarely or very customized.
    ///
    statement create_statement(const string_ref& q);
    ///
    /// Create prepared statement that will be cached for next calls.		
    ///
    statement create_prepared_statement(const string_ref& q);
    ///
    /// Execute list of sql commands as single request to database
    ///
    void exec_batch(const string_ref& q);

    ///
    /// Begin a transaction. Don't use it directly for RAII reasons. Use transaction class instead.
    ///
    void begin();
    ///
    /// Commit a transaction. Don't use it directly for RAII reasons. Use transaction class instead.
    ///
    void commit();
    ///
    /// Rollback a transaction. Don't use it directly for RAII reasons. Use transaction class instead.
    ///
    void rollback();

    ///
    /// Escape a string in range [\a b,\a e) for inclusion in SQL statement. It does not add quotation marks at beginning and end.
    /// It is designed to be used with text, don't use it with generic binary data.
    ///
    /// Some backends (odbc) may not support this.
    ///
    std::string escape(char const *b,char const *e);
    ///
    /// Escape a NULL terminated string \a s for inclusion in SQL statement. It does not add quotation marks at beginning and end.
    /// It is designed to be used with text, don't use it with generic binary data.
    ///
    /// Some backends (odbc) may not support this.
    ///
    std::string escape(char const *s);
    ///
    /// Escape a string \a s for inclusion in SQL statement. It does not add quotation marks at beginning and end.
    /// It is designed to be used with text, don't use it with generic binary data.
    ///
    /// Some backends (odbc) may not support this.
    ///
    std::string escape(std::string const &s);
    ///
    /// Get the backend name, as postgresql, odbc, sqlite3.
    /// Known backends are:
    /// - odbc
    /// - sqlite3
    /// - PgSQL
    /// - Oracle
    /// - MySQL
    /// 
    const std::string& backend();
    ///
    /// Get an SQL database name, it may be not the same as driver name for multiple engine drivers like odbc.
    /// This names can be used inside of SQL scripts to write statements specific only for some engine
    /// Currently known engines are:
    /// - sqlite3
    /// - PgSQL
    /// - Oracle
    /// - MySQL
    /// - Microsoft SQL Server
    ///
    const std::string& engine();
    ///
    /// Get the SQL Server version 
    ///
    void version(int& major, int& minor);
    ///
    /// Get human readable string describing SQL Server, usefull for logging
    ///
    const std::string& description();

private:
    boost::intrusive_ptr<backend::connection> conn_;
};

///
/// Syntactic sugar, same as prepare(q)
///
inline statement operator<<(session& s, string_ref query)
{
    return s.prepare(query);
}

}

#endif // EDBA_SESSION_HPP