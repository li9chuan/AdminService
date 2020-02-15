FILE(GLOB SRC *.cpp)

ADD_EXECUTABLE(admin_sample_app WIN32 ${SRC})

TARGET_LINK_LIBRARIES(admin_sample_app nelmisc nelnet)
NL_DEFAULT_PROPS(admin_sample_app "AdminModule, Sample")

INSTALL(TARGETS admin_sample_app RUNTIME DESTINATION ${NL_BIN_PREFIX} COMPONENT samplesmisc)
