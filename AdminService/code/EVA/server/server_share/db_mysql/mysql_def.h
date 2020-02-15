#ifndef SERVER_SHARED_DBDEF_H_
#define SERVER_SHARED_DBDEF_H_

# ifdef _MSC_VER
#   if _MSC_VER < 1700
#include <my_global.h>
#endif
#endif

#include <mysql.h>
#include <errmsg.h>
#include <nel/misc/thread.h>

#include <server_share/server_def.h>
#include <server_share/buf_fifo2.h>
#include <server_share/db_record/db_record_base.h>

#ifdef NL_OS_WINDOWS
static const char* DB_NAMES = "utf8";
#else
static const char* DB_NAMES = "utf8mb4";
#endif

// 执行SQL语句时出错再次尝试执行间隔时间（毫秒）
#define ERROR_SLEEP_TIME 2000


// inlitialize sql worker;
class CMysqlExecute;
typedef void ( *INIT_SQL_WORKER )( CMysqlExecute* );
// db function processing msg;
typedef void* ( *DB_PROC )( void* );


// db execute data;
struct DBExecuteData
{
    CRecordBase*    data;
    DB_PROC        func;
    DBExecuteData( void ) : data( NULL )
                          , func( NULL )
    {
        
    }
};

// db result struct;
struct DBResultInfo
{
    NLMISC::CSString Buffer;
    unsigned long    BufferSize;
    enum_field_types Field;
    my_bool          IsNull;

    DBResultInfo( void ) : BufferSize( 0 )
                         , Field( MYSQL_TYPE_NULL )
                         , IsNull( false )
    {
        Buffer.clear();
    }
};


// db statement struct;
struct DBStatementInfo
{
    NLMISC::CSString Buffer;
    enum_field_types Field;
    my_bool          IsUnsigned;
    my_bool          IsNullValue;

    DBStatementInfo( void ) : Field( MYSQL_TYPE_NULL )
                            , IsUnsigned( 0 )
                            , IsNullValue( 0 )
    {
        Buffer.clear();
    }
};

#endif // SERVER_SHARED_DBDEF_H_;