#include "url_threads.h"
#include "game_def.h"

CUrlThreads::CUrlThreads( void ) : m_IsExit( false )
                                 , m_Thread( NULL )
{
    m_MainThreadsQueue.init(1024 * 32);
    m_SubThreadsQueue.init(1024 * 32);
}

void CUrlThreads::PostToMain( CUrlTask* pUrlTask )
{
    bool res = m_MainThreadsQueue.push_back( pUrlTask );
    if ( res ) return;
    SAFE_DELETE(  pUrlTask );
}

void CUrlThreads::PostToSub( CUrlTask* pUrlTask )
{
    bool res = m_SubThreadsQueue.push_back( pUrlTask );
    if ( res ) return;
    SAFE_DELETE( pUrlTask );
}

void CUrlThreads::TickUpdate( void )
{
    while( true )
    {
        CUrlTask* pUrlTask = m_MainThreadsQueue.pop_front();
        if ( NULL == pUrlTask ) break;
        pUrlTask->Callback();
        SAFE_DELETE( pUrlTask );
    }
}

void CUrlThreads::run( void )
{
    while( !m_IsExit)
    {
        CUrlTask* pUrlTask = m_SubThreadsQueue.pop_front();

        if ( NULL == pUrlTask )
        {
            NLMISC::nlSleep( 3 );
            continue;
        }

        if ( pUrlTask->Request() )
        {
            PostToMain( pUrlTask );
        }
        else
        {
            SAFE_DELETE(pUrlTask);
        }
    }
}

void CUrlThreads::StartThreads( void )
{
    m_Thread = NLMISC::IThread::create( this );
    if ( NULL == m_Thread ) return;
    m_IsExit = false;
    m_Thread->start();
}

void CUrlThreads::CloseThreads( void )
{
    if ( NULL == m_Thread ) return;
    m_IsExit = true;
    m_Thread->wait();
}

