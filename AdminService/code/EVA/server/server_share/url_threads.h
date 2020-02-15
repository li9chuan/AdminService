#ifndef SERVER_URL_THREADS_H_
#define SERVER_URL_THREADS_H_

#include "url_task.h"
#include <server_share/buf_fifo2.h>
#include <nel/misc/singleton.h>

class CUrlThreads : public NLMISC::IRunnable, public NLMISC::CSingleton< CUrlThreads >
{
public:
    CUrlThreads( void );
    virtual ~CUrlThreads( void ) { };

    void PostToMain( CUrlTask* );
    void PostToSub ( CUrlTask* );
    void TickUpdate ( void );

    void StartThreads();
    void CloseThreads();

protected:
    void run( void );

    volatile bool                               m_IsExit;
    NLMISC::CBufFIFO2< CUrlTask >               m_MainThreadsQueue;
    NLMISC::CBufFIFO2< CUrlTask >               m_SubThreadsQueue;
    NLMISC::IThread*                            m_Thread;
};

#define UrlThreads       CUrlThreads::instance()

#endif  //  SERVER_URL_THREADS_H_;
