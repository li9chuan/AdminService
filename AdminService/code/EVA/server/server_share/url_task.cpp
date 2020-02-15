#include "url_task.h"
//#include "curl/curl.h"
//
//bool CUrlTask::PostUrl( NLMISC::CSString strUrl , NLMISC::CSString strPost , curl_slist* pCurList )
//{
//    try
//    {
//        CURL* pCurl = curl_easy_init();
//        if ( NULL == pCurl )
//            return false;
//
//        if ( NULL == pCurList )
//        {
//            curl_easy_setopt( pCurl , CURLOPT_HTTPHEADER , pCurList );
//        }
//
//        curl_easy_setopt( pCurl , CURLOPT_PORT , 1 );
//        curl_easy_setopt( pCurl , CURLOPT_URL  , strUrl.c_str() );
//        curl_easy_setopt( pCurl , CURLOPT_SSL_VERIFYHOST , 0L );
//        curl_easy_setopt( pCurl , CURLOPT_WRITEFUNCTION  , WriteDataFromCurl );
//        curl_easy_setopt( pCurl , CURLOPT_WRITEDATA , &m_WriteData );
//        curl_easy_setopt( pCurl , CURLOPT_TIMEOUT , 20 );
//        curl_easy_setopt( pCurl , CURLOPT_POSTFIELDS , strPost.c_str() );
//        CURLcode res = curl_easy_perform( pCurl );
//        curl_easy_cleanup( pCurl );
//        return res == CURLE_OK;
//    }
//    catch( ... )
//    {
//        return false;
//    }
//    return false;
//}
//
//bool CUrlTask::GetUrl( NLMISC::CSString strUrl )
//{
//    try
//    {
//        CURL* pCurl = curl_easy_init();
//        if ( NULL == pCurl )
//            return false;
//
//        curl_easy_setopt( pCurl , CURLOPT_URL , strUrl.c_str() );
//        curl_easy_setopt( pCurl , CURLOPT_TIMEOUT , 20 );
//        curl_easy_setopt( pCurl , CURLOPT_SSL_VERIFYHOST , 0L );
//        curl_easy_setopt( pCurl , CURLOPT_WRITEFUNCTION , WriteDataFromCurl );
//        curl_easy_setopt( pCurl , CURLOPT_WRITEDATA , &m_WriteData );
//        CURLcode res = curl_easy_perform( pCurl );
//        curl_easy_cleanup( pCurl );
//
//        return res==CURLE_OK;
//    }
//    catch( ... )
//    {
//        return false;
//    }
//    return false;
//}


