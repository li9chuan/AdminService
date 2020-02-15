#ifndef SERVER_SHARED_CONNECT_H_
#define SERVER_SHARED_CONNECT_H_

#include "mysql_def.h"

class CMysqlStmt;
class CMysqlResult;

class CMysqlConnect
{
public:
    CMysqlConnect( void );
   ~CMysqlConnect( void );

public:
    //     connect;
    bool   Connect( NLMISC::CSString , NLMISC::CSString , NLMISC::CSString , NLMISC::CSString , uint16 );
    //     close;
    void   Close( void );
    //     execute;
    sint32 Execute( CMysqlStmt* );
    //     query;
    sint32 Query( CMysqlStmt* , CMysqlResult** );
    //     create stmt;
    CMysqlStmt* CreateStmt( char const* , size_t );

protected:
    //     re connect;
    bool   ReConnect( void );
    bool   Procerror( CMysqlStmt* , char const* func = NULL );
    bool   ReError( char const* , char const* );

    PROP( MYSQL*             , Mysql     );
    PROP( NLMISC::CSString   , Host      );
    PROP( NLMISC::CSString   , User      );
    PROP( NLMISC::CSString   , Password  );
    PROP( NLMISC::CSString   , DBName    );
    PROP( uint16             , Port      );
    PROP( uint32             , Errno     );
    PROP( NLMISC::CSString   , Error     );
};

#endif // SERVER_SHARED_CONNECT_H_;