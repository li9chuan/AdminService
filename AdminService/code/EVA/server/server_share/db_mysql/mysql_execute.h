#ifndef SERVER_SHARED_DBEXECUTE_H_
#define SERVER_SHARED_DBEXECUTE_H_

#include <server_share/server_def.h>
#include "mysql_statement.h"
#include "mysql_connect.h"

class CMysqlExecute
{
public:
    CMysqlExecute( void );
   ~CMysqlExecute( void );

    //   connect;
    bool Connect( NLMISC::CSString , NLMISC::CSString , NLMISC::CSString , NLMISC::CSString , uint16 );
    //   close;
    void Close( void );
    //   execute;
    void Execute( CMysqlStmt* );
    //   query;
    sint32 Query( CMysqlStmt* , CMysqlResult** );

    //   add stmt;
    void AddStmt( size_t , char const* );
    //   get stmt;
    CMysqlStmt* GetStmt( size_t );

    CMysqlConnect                   m_Connect;
    std::vector<CMysqlStmt*>        m_StmtTable;
};


#endif // SERVER_SHARED_DBEXECUTE_H_;