#ifndef SERVER_WWW_TASK_H_
#define SERVER_WWW_TASK_H_

#include "nel/misc/sstring.h"

class curl_slist;

class CUrlTask
{
public:
    CUrlTask( void ){ };
    virtual ~CUrlTask( void ){ };

public:
    virtual bool Request() = 0;
    virtual void Callback() {}

    bool PostUrl( NLMISC::CSString, NLMISC::CSString, curl_slist* );
    virtual bool GetUrl ( NLMISC::CSString );

protected:
    static size_t WriteDataFromCurl( char* pBuffer , size_t BufferSize , size_t Member , NLMISC::CSString* WriterData )
    {
        if ( NULL == pBuffer )
            return 0;
        WriterData->append( pBuffer , BufferSize * Member );
        return BufferSize * Member;
    }

    NLMISC::CSString            m_WriteData;
};

#endif  //  SERVER_WWW_TASK_H_
