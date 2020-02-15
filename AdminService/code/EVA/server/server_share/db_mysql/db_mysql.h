#ifndef SERVER_SHARED_DBMYSQL_H_
#define SERVER_SHARED_DBMYSQL_H_

#include "mysql_def.h"
#include "mysql_connect.h"
#include "mysql_execute.h"

class CDBMysql : NLMISC::IRunnable
{
public:
    CDBMysql( void ) : m_Exit(true), m_Thread(NULL)
    {
        mysql_library_init( 0 , NULL , NULL );
        mysql_thread_init();

        m_SubThdEvents.init(1024*32);
        m_MainThdEvents.init(1024*32);
    };

    virtual ~CDBMysql( void )
    {
        mysql_thread_end();
        mysql_library_end();
    };

    //   connect;
    bool Connect( NLMISC::CSString , NLMISC::CSString , NLMISC::CSString , NLMISC::CSString , uint16 , INIT_SQL_WORKER );
    //   start threads;
    void StartThreads( void );
    //   close;
    void CloseThreads( void );
    //   update;
    void TickUpdate( void );
    //   post main threads;
    void PostToMain( CRecordBase* , DB_PROC );
    //   post sub threads;
    void PostToSub ( CRecordBase* , DB_PROC );

    CMysqlExecute& MainExec()   { return m_MainWorker; }
    CMysqlExecute& SubExec()    { return m_SubWorker; }

    uint32 SubEventSize()       { return m_SubThdEvents.size(); }

private:
    CMysqlExecute  m_MainWorker;
    CMysqlExecute  m_SubWorker;

protected:
    //   run;
    void run( void );

    NLMISC::IThread*                            m_Thread;
    bool                                        m_Exit;
    NLMISC::CSString                            m_Host;
    NLMISC::CSString                            m_User;
    NLMISC::CSString                            m_Password;
    NLMISC::CSString                            m_DBName;
    uint16                                      m_Port;
    INIT_SQL_WORKER                             m_Worker;
    NLMISC::CBufFIFO2<DBExecuteData>            m_SubThdEvents;
    NLMISC::CBufFIFO2<DBExecuteData>            m_MainThdEvents;
};

#endif //SERVER_SHARED_DBMYSQL_H_
