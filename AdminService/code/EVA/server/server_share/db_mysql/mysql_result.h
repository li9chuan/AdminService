#ifndef SERVER_SHARED_DBRESULT_H_
#define SERVER_SHARED_DBRESULT_H_

#include "mysql_def.h"

class CMysqlResult
{
public:
    friend class CMysqlConnect;

    //      bool;
    my_bool GetBool  ( );
    //      int8
    sint8   GetInt8  ( ) { return GetFieldParam<sint8>( m_Idx++ );  }
    //      uint8;
    uint8   GetUint8 ( ) { return GetFieldParam<uint8>( m_Idx++ );  }
    //      int16;
    sint16  GetInt16 ( ) { return GetFieldParam<sint16>( m_Idx++ ); }
    //      uint16;
    uint16  GetUint16( ) { return GetFieldParam<uint16>( m_Idx++ ); }
    //      int32;
    sint32  GetInt32 ( ) { return GetFieldParam<sint32>( m_Idx++ ); }
    //      uint32;
    uint32  GetUint32( ) { return GetFieldParam<uint32>( m_Idx++ ); }
    //      int64;
    sint64  GetInt64 ( ) { return GetFieldParam<sint64>( m_Idx++ ); }
    //      uint64;
    uint64  GetUint64( ) { return GetFieldParam<uint64>( m_Idx++ ); }
    //      float;
    float   GetFloat ( ) { return GetFieldParam<float>( m_Idx++ ); }
    //      double;
    double  GetDouble( ) { return GetFieldParam<double>( m_Idx++ ); }
    //      string;
    NLMISC::CSString&   GetString() { return GetBlob(); }
    //      blob;
    NLMISC::CSString&   GetBlob();
    //      next;
    bool    NextRow( void );
    //      release;
    void    Release( void ){ delete this; }
    //      count;
    size_t  GetCount ( void ) const { return m_RowCount; }

protected:
    CMysqlResult( void );
   ~CMysqlResult( void );

    // initialize;
    bool Initialize( MYSQL_STMT* , MYSQL_RES* );
    // row offset;
    uint32 GetRowOffset( void ) { return m_RowCurr < 0 ? 0 : ( m_RowCurr*m_FieldCount ); }
    // get param ;
    template< typename T >
    T GetFieldParam( size_t sz );

    std::vector< DBResultInfo >         m_ResultArray;
    std::vector< DBResultInfo >         m_Result;
    uint32                              m_RowCount;
    sint32                              m_RowCurr;
    uint32                              m_FieldCount;
    uint32                              m_Idx;

private:

    void __Reset();
};

#include "mysql_template.inl"

#endif // SERVER_SHARED_DBINTERFACE_H_
