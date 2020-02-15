#include "mysql_execute.h"

CMysqlExecute::CMysqlExecute( void )
{
    m_StmtTable.resize( 0xFF , NULL );
}

CMysqlExecute::~CMysqlExecute( void )
{
    Close();
}

bool CMysqlExecute::Connect( NLMISC::CSString Host , NLMISC::CSString User , NLMISC::CSString Password , NLMISC::CSString DBName , uint16 Port )
{
    return m_Connect.Connect( Host , User , Password , DBName , Port );
}

void CMysqlExecute::Close( void )
{
    for ( sint32 idx = 0; idx < 0xFF; idx++ )
    {
        if ( NULL == m_StmtTable[idx] ) continue;
        m_StmtTable[idx]->Release();
        m_StmtTable[idx] =  NULL;
    }
    m_StmtTable.clear();
    m_Connect.Close();
}

void CMysqlExecute::Execute( CMysqlStmt* pStmt )
{
    if ( NULL == pStmt ) return;
    m_Connect.Execute( pStmt );
}

sint32 CMysqlExecute::Query( CMysqlStmt* pStmt , CMysqlResult** pResult )
{
    if ( NULL == pStmt   ) return 0;
    if ( NULL == pResult ) return 0;
    return m_Connect.Query( pStmt , pResult );
}

void CMysqlExecute::AddStmt( size_t sz , char const* sql )
{
    if ( NULL == sql ) return;
    CMysqlStmt* pStmt = m_Connect.CreateStmt( sql , strlen( sql ) );
    if ( NULL == pStmt ) return;
    if ( m_StmtTable[ sz ] != NULL )
    {
        m_StmtTable[ sz ]->Release();
        m_StmtTable[ sz ] = NULL;
    }
    m_StmtTable[ sz ] = pStmt;
}

CMysqlStmt* CMysqlExecute::GetStmt( size_t sz )
{
    m_StmtTable[sz]->Reset();
    return m_StmtTable[sz];
}
