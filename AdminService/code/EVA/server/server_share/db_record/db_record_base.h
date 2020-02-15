#ifndef SERVER_SHARED_DBRECORDBASE_H_
#define SERVER_SHARED_DBRECORDBASE_H_

#include <nel/misc/stream.h>
#include <nel/misc/sstring.h>
#include <server_share/game_def.h>

enum TDBSaveOPT
{
    SAVE_OPT_NULL   = 0,
    SAVE_OPT_INSERT = 0x1,    /// ²åÈë;
    SAVE_OPT_UPDATE = 0x2,    /// ¸üÐÂ;
    SAVE_OPT_DELETE = 0x4,    /// É¾³ý;
};

enum TDBTable
{
    TB_INVALD,
    TB_PLAYER_INFO,
    TB_HOLD_TK,
    TB_INDUSTRY,
};

class CRecordBase
{
public:
    bool is_null()                  {  return ( _flag == SAVE_OPT_NULL ); }
    void reset()                    {  _flag = SAVE_OPT_NULL; }

    void set_insert()               {  _flag |= SAVE_OPT_INSERT; }
    void set_update()               {  _flag |= SAVE_OPT_UPDATE; }
    void set_delete()               {  _flag |= SAVE_OPT_DELETE; }

    bool is_insert()                {  return ( _flag & SAVE_OPT_INSERT ); }
    bool is_update()                {  return ( _flag & SAVE_OPT_UPDATE ); }
    bool is_delete()                {  return ( _flag & SAVE_OPT_DELETE ); }

    void delete_charge_update( void )
    {
        _flag &= ~SAVE_OPT_DELETE;
        _flag |=  SAVE_OPT_UPDATE;
    }

    CRecordBase( void )
        : m_TableEnum(TB_INVALD), _flag( SAVE_OPT_NULL )
    { }

    virtual ~CRecordBase()  {}

    CRecordBase( const CRecordBase& _rhs )  { m_TableEnum=_rhs.m_TableEnum; _flag=_rhs._flag; }

    CRecordBase& operator = ( const CRecordBase& _rhs )
    {
        m_TableEnum = _rhs.m_TableEnum;
        _flag |= _rhs._flag;
        return ( *this );
    }

    uint8   m_TableEnum;
protected:
    uint8   _flag;
};

#endif // SERVER_SHARED_DBRECORDBASE_H_;
