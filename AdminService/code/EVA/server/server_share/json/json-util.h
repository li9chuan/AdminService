#pragma once

#include <string>
#include <sstream>
#include "json.h"
#include <nel/misc/string_common.h>

namespace Json{

    inline std::string getJsonStr(Value &json)
    {
        StyledWriter writer;
        return writer.write(json);
    }
    inline Value parseJsonToArray(const std::string &str)
    {
        Reader reader;
        Value json(arrayValue);
        bool succ = reader.parse(str, json);
        if (!succ) json.clear();
        return json;
    }
    inline Value parseJsonToObj(const std::string &str)
    {
        Reader reader;
        Value json(objectValue);
        bool succ = reader.parse(str, json);
        if (!succ) json.clear();
        return json;
    }
    inline Value parseJsonStr(const std::string& str)
    {
        Reader reader;
        Value json(nullValue);
        bool succ = reader.parse(str, json);
        if (!succ) json.clear();
        return json;
    }
    inline bool parseJsonFromStr(const std::string& str, Value& jdata)
    {
        Reader reader;
        bool bSucc = reader.parse(str, jdata);
        if (!bSucc)
        {
            jdata.clear();
        }
        return bSucc;
    }
    inline bool HasJsonSubMember(const Value& v, const std::string& str)
    {
        if (!v.isObject())
        {
            return false;
        }
        Value::Members ms = v.getMemberNames();
        for (unsigned int i = 0; i < ms.size(); i++)
        {
            if (str == ms[i])
            {
                return true;
            }
        }
        return false;
    }
    inline int GetIntFromJsonMember(const Value& v, const std::string& k, int defult_result = -1)
    {
        if (!HasJsonSubMember(v, k))
        {
            return defult_result;
        }
        if (v[k].isInt())
        {
            return v[k].asInt();
        }
        if (v[k].isString())
        {
            sint ret = -1;
            NLMISC::fromString(v[k].asString(), ret);
            return (int)ret;
        }
        return defult_result;
    }
    inline std::string GetStrFromJsonMember(const Value& v, const std::string& k)
    {
        if (!HasJsonSubMember(v, k))
        {
            return "";
        }
        if (v[k].isInt())
        {
            sint i = v[k].asInt();
            return NLMISC::toString(i);
        }
        if (v[k].isString())
        {
            return v[k].asString();
        }
        return "";
    }
    inline bool JTableAddHead(Value& j, const std::string& str)
    {
        Value& head = j["head"];
        if (head.isArray() || head.isNull())
        {
            j["head"].append(str);
        }
        return true;
    }
    class JsonTable
    {
        Value* pHeader;
        Value* pData;
        Value* pJson;
        Value jDataZone;
        Value jEmptyLine;
    public:
        JsonTable(Value* pJson)
        {

            this->pJson = pJson;
            pHeader = &((*pJson)["head"]);
            pData = &((*pJson)["value"]);
        }
        JsonTable()
        {
            pJson = &jDataZone;
            pHeader = &((*pJson)["head"]);
            pData = &((*pJson)["value"]);
        }
        int AddName(const std::string& name)
        {
            int v = pHeader->size();
            pHeader->append(name);
            return v;
        }
        Value& AddLine()
        {
            int v = pData->size();
            pData->append(jEmptyLine);
            return ((*pData)[v]);
        }
        void CopyNameSet(JsonTable* pSrc)
        {
            if (pSrc == NULL)
            {
                return;
            }
            pHeader->clear();
            *pHeader = *(pSrc->pHeader);
        }
        inline bool EmptyHeader()
        {
            return pHeader->size() <= 0;
        }
        inline Value* GetJPtr()
        {
            return pJson;
        }
    };
}


