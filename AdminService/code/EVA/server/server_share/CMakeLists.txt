FILE(GLOB SRC *.cpp *.h)
FILE(GLOB SRC_JSON          json/*.h json/*.cpp json/*.inl)
FILE(GLOB SRC_PB            pb/*.h pb/*.cc)
FILE(GLOB SRC_RECORD        db_record/*.h db_record/*.cpp)
FILE(GLOB SRC_MYSQL         db_mysql/*.h db_mysql/*.cpp db_mysql/*.inl)

SOURCE_GROUP("json"         FILES ${SRC_JSON})
SOURCE_GROUP("pb"           FILES ${SRC_PB})
SOURCE_GROUP("dbrecord"     FILES ${SRC_RECORD})
SOURCE_GROUP("dbmysql"      FILES ${SRC_MYSQL})

NL_TARGET_LIB(servershare ${SRC_RECORD} ${SRC_MYSQL} ${SRC} ${SRC_JSON} ${SRC_PB})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${OPENSSL_INCLUDE_DIR} ${MYSQL_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR} ${PROTOBUF_INCLUDE_DIR} ${CURL_INCLUDE_DIR})

TARGET_LINK_LIBRARIES(  servershare
                        nelmisc
                        nelnet
                        ${OPENSSL_LIBRARIES}
                        ${CURL_LIBRARY}
                        ${LIBXML2_LIBRARIES}
                        ${ZLIB_LIBRARIE_RELEASE}
                        )

NL_DEFAULT_PROPS(servershare "Anly, Library: Service Shared")
NL_ADD_RUNTIME_FLAGS(servershare)
NL_ADD_LIB_SUFFIX(servershare)

ADD_DEFINITIONS(${LIBXML2_DEFINITIONS})

IF(WITH_PCH)
  ADD_NATIVE_PRECOMPILED_HEADER(servershare ${CMAKE_CURRENT_SOURCE_DIR}/stdpch.h ${CMAKE_CURRENT_SOURCE_DIR}/stdpch.cpp)
ENDIF(WITH_PCH)

#INSTALL(TARGETS servershare LIBRARY DESTINATION ${EVA_LIB_PREFIX} ARCHIVE DESTINATION ${EVA_LIB_PREFIX} COMPONENT libraries)
IF((WITH_INSTALL_LIBRARIES AND WITH_STATIC) OR NOT WITH_STATIC)
  INSTALL(TARGETS servershare LIBRARY DESTINATION lib ARCHIVE DESTINATION lib COMPONENT libraries)
ENDIF((WITH_INSTALL_LIBRARIES AND WITH_STATIC) OR NOT WITH_STATIC)


