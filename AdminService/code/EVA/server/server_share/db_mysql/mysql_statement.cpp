#include "mysql_statement.h"

CMysqlStmt::CMysqlStmt( void ) : m_Bind(NULL)
                                , m_Count(0)
                                , m_Idx(0)
{
    m_Param.resize( 64 );
}

CMysqlStmt::~CMysqlStmt( void )
{
    if ( NULL == m_Bind ) return;
    free( m_Bind );
    m_Bind  = NULL;
    m_Count = 0;
}

void CMysqlStmt::SetBool( my_bool bl )
{
    DBStatementInfo& ParamStmt = m_Param[ m_Idx++ ];
    ParamStmt.Buffer.assign( 1 , (char)bl );
    ParamStmt.Field = MYSQL_TYPE_TINY;
    ParamStmt.IsUnsigned = 0;
}

void CMysqlStmt::SetInt8( sint8 i8 )
{
    DBStatementInfo& ParamStmt = m_Param[ m_Idx++ ];
    ParamStmt.Buffer.assign( 1 , (char)i8 );
    ParamStmt.Field = MYSQL_TYPE_TINY;
    ParamStmt.IsUnsigned = 0;
}

void CMysqlStmt::SetUInt8( uint8 ui8 )
{
    DBStatementInfo& ParamStmt = m_Param[ m_Idx++ ];
    ParamStmt.Buffer.assign( 1 , (char)ui8 );
    ParamStmt.Field = MYSQL_TYPE_TINY;
    ParamStmt.IsUnsigned = UNSIGNED_FLAG;
}

void CMysqlStmt::SetInt16( sint16 i16 )
{
    DBStatementInfo& ParamStmt = m_Param[ m_Idx++ ];
    ParamStmt.Buffer.assign( (char*)&i16 , 2 );
    ParamStmt.Field = MYSQL_TYPE_SHORT;
    ParamStmt.IsUnsigned = 0;
}

void CMysqlStmt::SetUint16( uint16 ui16 )
{
    DBStatementInfo& ParamStmt = m_Param[ m_Idx++ ];
    ParamStmt.Buffer.assign( (char*)&ui16 , 2 );
    ParamStmt.Field = MYSQL_TYPE_SHORT;
    ParamStmt.IsUnsigned = UNSIGNED_FLAG;
}

void CMysqlStmt::SetInt32( sint32 i32 )
{
    DBStatementInfo& Param = m_Param[ m_Idx++ ];
    Param.Buffer.assign( (char*)&i32 , 4 );
    Param.Field = MYSQL_TYPE_LONG;
    Param.IsUnsigned = 0;
}

void CMysqlStmt::SetUint32( uint32 ui32 )
{
    DBStatementInfo& ParamStmt = m_Param[ m_Idx++ ];
    ParamStmt.Buffer.assign( (char*)&ui32 , 4 );
    ParamStmt.Field = MYSQL_TYPE_LONG;
    ParamStmt.IsUnsigned = UNSIGNED_FLAG;
}

void CMysqlStmt::SetInt64( sint64 i64 )
{
    DBStatementInfo& ParamStmt = m_Param[ m_Idx++ ];
    ParamStmt.Buffer.assign( (char*)&i64 , 8 );
    ParamStmt.Field = MYSQL_TYPE_LONGLONG;
    ParamStmt.IsUnsigned = 0;
}

void CMysqlStmt::SetUint64( uint64 ui64 )
{
    DBStatementInfo& ParamStmt = m_Param[ m_Idx++ ];
    ParamStmt.Buffer.assign( (char*)&ui64 , 8 );
    ParamStmt.Field = MYSQL_TYPE_LONGLONG;
    ParamStmt.IsUnsigned = UNSIGNED_FLAG;
}

void CMysqlStmt::SetFloat( float fv )
{
    DBStatementInfo& ParamStmt = m_Param[ m_Idx++ ];
    ParamStmt.Buffer.assign( (char*)&fv , 4 );
    ParamStmt.Field = MYSQL_TYPE_FLOAT;
    ParamStmt.IsUnsigned = 0;
}

void CMysqlStmt::SetDouble( double db )
{
    DBStatementInfo& ParamStmt = m_Param[ m_Idx++ ];
    ParamStmt.Buffer.assign( (char*)&db , 8 );
    ParamStmt.Field = MYSQL_TYPE_DOUBLE;
    ParamStmt.IsUnsigned = 0;
}

void CMysqlStmt::SetDecimal( double db )
{
    DBStatementInfo& ParamStmt = m_Param[ m_Idx++ ];
    ParamStmt.Buffer.assign( (char*)&db , 8 );      /// M+2
    ParamStmt.Field = MYSQL_TYPE_DECIMAL;
    ParamStmt.IsUnsigned = 0;
}

void CMysqlStmt::SetString( char const* str )
{
    DBStatementInfo& ParamStmt = m_Param[ m_Idx++ ];
    ParamStmt.Buffer.assign( str );
    ParamStmt.Field = MYSQL_TYPE_STRING;
    ParamStmt.IsUnsigned = 0;
}

void CMysqlStmt::SetString( char const* str , size_t sz1 )
{
    DBStatementInfo& ParamStmt = m_Param[ m_Idx++ ];
    ParamStmt.Buffer.assign( str , sz1 );
    ParamStmt.Field = MYSQL_TYPE_STRING;
    ParamStmt.IsUnsigned = 0;
}

void CMysqlStmt::SetString( std::string& str )
{
    SetString(str.c_str(), str.size());
}

void CMysqlStmt::SetBlob( void const* ptr , size_t sz1 )
{
    DBStatementInfo& ParamStmt = m_Param[ m_Idx++ ];
    ParamStmt.Buffer.assign( (char*)ptr , sz1 );
    ParamStmt.Field = MYSQL_TYPE_BLOB;
    ParamStmt.IsUnsigned = 0;
}

void CMysqlStmt::SetBlobTiny( void const* ptr , size_t sz1 )
{
    DBStatementInfo& ParamStmt = m_Param[ m_Idx++ ];
    ParamStmt.Buffer.assign( (char*)ptr , sz1 );
    ParamStmt.Field = MYSQL_TYPE_TINY_BLOB;
    ParamStmt.IsUnsigned = 0;
}

void CMysqlStmt::Bind( size_t Count )
{
    if ( m_Count < Count )
    {
        SAFE_DELETE( m_Bind );
        m_Bind = (MYSQL_BIND*)malloc( Count * sizeof(MYSQL_BIND) );
        m_Count = Count;
    }

    if ( NULL == m_Bind ) return;
    memset( m_Bind , 0 , m_Count * sizeof(MYSQL_BIND) );
    for ( size_t idx = 0; idx < Count; idx++ )
    {
        DBStatementInfo& ParamStmt = m_Param[ idx ];
        m_Bind[idx].buffer          = (void*)ParamStmt.Buffer.c_str();
        m_Bind[idx].buffer_length   = ParamStmt.Buffer.size();
        m_Bind[idx].buffer_type     = ParamStmt.Field;
        m_Bind[idx].is_unsigned     = ParamStmt.IsUnsigned;
        m_Bind[idx].is_null_value   = ParamStmt.IsNullValue;
    }
}

NLMISC::CSString CMysqlStmt::ToString( void )
{
    NLMISC::CSString StmtStr;

    for ( size_t idx = 0; idx < m_Count; idx++ )
    {
        DBStatementInfo& DBField = m_Param[idx];
        switch ( DBField.Field )
        {
        case MYSQL_TYPE_TINY:
            if ( DBField.IsUnsigned )
            {
                StmtStr.append( " u8(" );
                StmtStr.append( NLMISC::toString( *(uint8*)DBField.Buffer.c_str() ) );
                StmtStr.append( ")" );
            }
            else
            {
                StmtStr.append( " s8(" );
                StmtStr.append( NLMISC::toString( *(sint8*)DBField.Buffer.c_str() ) );
                StmtStr.append( ")");
            }
            break;
        case MYSQL_TYPE_SHORT:
            if ( DBField.IsUnsigned )
            {
                StmtStr.append( " u16(" );
                StmtStr.append( NLMISC::toString( *(uint16*)DBField.Buffer.c_str() ) );
                StmtStr.append( ")" );
            }
            else
            {
                StmtStr.append( " s16c" );
                StmtStr.append( NLMISC::toString( *(sint16*)DBField.Buffer.c_str() ) );
                StmtStr.append( ")");
            }
            break;
        case MYSQL_TYPE_LONG:
            if ( DBField.IsUnsigned )
            {
                StmtStr.append( " u32(" );
                StmtStr.append( NLMISC::toString( *(uint32*)DBField.Buffer.c_str() ) );
                StmtStr.append( ")");
            }
            else
            {
                StmtStr.append( " s32(" );
                StmtStr.append( NLMISC::toString( *(sint32*)DBField.Buffer.c_str() ) );
                StmtStr.append( ")" );
            }
            break;
        case MYSQL_TYPE_LONGLONG:
            if ( DBField.IsUnsigned )
            {
                StmtStr.append( " u64(" );
                StmtStr.append( NLMISC::toString( *(uint64*)DBField.Buffer.c_str() ) );
                StmtStr.append( ")");
            }
            else
            {
                StmtStr.append( " s64(" );
                StmtStr.append( NLMISC::toString( *(sint64*)DBField.Buffer.c_str() ) );
                StmtStr.append( ")");
            }
            break;
        case MYSQL_TYPE_FLOAT:
            StmtStr.append( " f(" );
            StmtStr.append( NLMISC::toString( *(float*)DBField.Buffer.c_str() ) );
            StmtStr.append( ")");
            break;
        case MYSQL_TYPE_DOUBLE:
            StmtStr.append( " d(" );
            StmtStr.append( NLMISC::toString( *(double*)DBField.Buffer.c_str() ) );
            StmtStr.append( ")" );
            break;
        case MYSQL_TYPE_STRING:
            StmtStr.append( " str(" );
            StmtStr.append( DBField.Buffer );
            StmtStr.append( ")" );
            break;
        case MYSQL_TYPE_BLOB:
        case MYSQL_TYPE_TINY_BLOB:
            StmtStr.append( " blob" );
            break;
        default:
            StmtStr.append( "unknow" );
            break;
        }
    }

    return StmtStr;
}

