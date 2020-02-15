#include "db_mysql.h"

bool CDBMysql::Connect( NLMISC::CSString Host , NLMISC::CSString User, NLMISC::CSString PassWord, NLMISC::CSString DBName, uint16 Port , INIT_SQL_WORKER Worker )
{
    bool result = m_MainWorker.Connect( Host , User , PassWord , DBName , Port );
    if ( !result ) return result;
    if ( NULL != Worker ) { (*Worker)(&m_MainWorker); }

    m_Host      = Host;
    m_User      = User;
    m_Password  = PassWord;
    m_DBName    = DBName;
    m_Port      = Port;
    m_Worker    = Worker;

    return result;
}

void CDBMysql::CloseThreads( void )
{
    if ( NULL == m_Thread ) return;
    m_Thread->wait();
    m_Exit = true;
}

void CDBMysql::StartThreads( void )
{
    m_Thread = NLMISC::IThread::create( this );
    if ( NULL == m_Thread ) return;
    m_Thread->start();
    m_Exit = false;
}

void CDBMysql::TickUpdate( void )
{
    while ( true )
    {
        DBExecuteData* pMessage = m_MainThdEvents.pop_front();
        if ( NULL == pMessage ) break;
        if ( NULL == pMessage->func ) break;
        ( *pMessage->func )( pMessage->data );
        SAFE_DELETE( pMessage->data );
        SAFE_DELETE( pMessage );
    }
}

void CDBMysql::run( void )
{
    mysql_thread_init();

    bool result = m_SubWorker.Connect( m_Host.c_str() , m_User.c_str() , m_Password.c_str(), m_DBName.c_str(), m_Port );
    if ( !result )
    {
        nlerror( "mysql connect failes host=%s , user=%s , pwd = %s , db = %s , port = %d " , m_Host.c_str() , m_User.c_str() , m_Password.c_str() , m_DBName.c_str() , m_Port );
        return;
    }

    if ( NULL != m_Worker ) { (*m_Worker)(&m_SubWorker); }

    while ( true )
    {
        DBExecuteData* pMessage = m_SubThdEvents.pop_front();

        if ( NULL != pMessage && NULL != pMessage->func )
        {
            ( *pMessage->func )( pMessage->data );
            SAFE_DELETE( pMessage->data );
            SAFE_DELETE( pMessage );
        }
        else if ( m_Exit ) { break; }
        else
        {
            NLMISC::nlSleep( 3 );
        }
    }
    mysql_thread_end();
}

void CDBMysql::PostToMain( CRecordBase* pRecordBase , DB_PROC cb )
{
    DBExecuteData* pExecuteData = new  ( std::nothrow ) DBExecuteData();

    if ( NULL == pExecuteData )
    {
        SAFE_DELETE( pRecordBase );
        return;
    }
    pExecuteData->data = pRecordBase;
    pExecuteData->func = cb;

    if ( !m_MainThdEvents.push_back( pExecuteData ) )
    {
        SAFE_DELETE(pRecordBase);
        SAFE_DELETE(pExecuteData);
    }
}

void CDBMysql::PostToSub( CRecordBase* pRecordBase , DB_PROC cb )
{
    DBExecuteData* pExecuteData = new ( std::nothrow ) DBExecuteData();

    if ( NULL == pExecuteData )
    {
        SAFE_DELETE( pRecordBase );
        return;
    }

    pExecuteData->data = pRecordBase;
    pExecuteData->func = cb;

    if ( !m_SubThdEvents.push_back( pExecuteData ) )
    {
        SAFE_DELETE(pRecordBase);
        SAFE_DELETE(pExecuteData);
    }
}
