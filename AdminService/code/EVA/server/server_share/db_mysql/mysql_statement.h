#ifndef SERVER_SHARED_DBSTATEMENT_H_
#define SERVER_SHARED_DBSTATEMENT_H_

#include "mysql_def.h"

class CMysqlStmt
{
public:
    friend class CMysqlConnect;

    //   bool;
    void SetBool  ( my_bool );
    //   int8;
    void SetInt8  ( sint8  );
    //   uint8;
    void SetUInt8 ( uint8  );
    //   int16;
    void SetInt16 ( sint16 );
    //   uint16;
    void SetUint16( uint16 );
    //   int32;
    void SetInt32 ( sint32 );
    //   uint32;
    void SetUint32( uint32 );
    //   int64;
    void SetInt64 ( sint64 );
    //   uint64;
    void SetUint64( uint64 );
    //   float;
    void SetFloat ( float  );
    //   double;
    void SetDouble( double );

    void SetDecimal( double );
    //   string;
    void SetString( char const* );
    void SetString( char const* , size_t );
    void SetString( std::string& );

    //   bolb;
    void SetBlob( void const* , size_t );
    void SetBlobTiny( void const* , size_t );
    //   release;
    void Release( void ) { delete this; }

    void Reset()  { m_Idx = 0; }

    MYSQL_BIND*                 GetBind()       { return m_Bind; }
    const NLMISC::CSString&     GetSql()        { return m_Sql; }

    void SetSql( char const* sql , size_t sz )  { m_Sql.assign(sql, sz); }

    //   to string;
    NLMISC::CSString ToString( void );

protected:
    CMysqlStmt( void );
   ~CMysqlStmt( void );

    //   Bind;
    void Bind( size_t );

    MYSQL_BIND*                             m_Bind;
    size_t                                  m_Count;
    std::vector< DBStatementInfo >          m_Param;
    NLMISC::CSString                        m_Sql;
    uint32                                  m_Idx;
};

#endif // SERVER_SHARED_DBSTATEMENT_H_
