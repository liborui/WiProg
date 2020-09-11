CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(proxy)

SET(CMAKE_CXX_FLAGS "-std=c++11 -g")

SET (Is_Scheduler "OFF")
SET (Is_Device "OFF")
SET (Is_Offloader "OFF")
SET (Is_Server "OFF")
SET (Is_Client "OFF")

option (Is_Cloud "run on cloud" OFF)
option (Is_Ubuntu "run on Ubuntu" OFF)
option (Is_LinkSys "run on LinkSys" OFF)
option (Is_LinKIt "run on LinKit Smart" OFF)
option (Is_Raspberry "run on Raspberry Pi" OFF)

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib ${LINK_DIR} /usr/lib /usr/local/lib)

INCLUDE_DIRECTORIES(
	${INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    # ${PROJECT_SOURCE_DIR}/src/protobuf
    # ${PROJECT_SOURCE_DIR}/src/rpc_service
    ${PROJECT_SOURCE_DIR}/src/tcp_connection
    # ${PROJECT_SOURCE_DIR}/src/database
	${PROJECT_SOURCE_DIR}/src/basic
	${PROJECT_SOURCE_DIR}/src/profiler
	# ${PROJECT_SOURCE_DIR}/src/scheduler/lp_solve_5.5
    # /usr/local/include/sphinxbase
    # /usr/local/include/pocketsphinx
)

ADD_EXECUTABLE(
    resource_profiler_edge
    ${PROJECT_SOURCE_DIR}/src/client_new.cc
    ${PROJECT_SOURCE_DIR}/src/tcp_connection/tcp_client.cc
    ${PROJECT_SOURCE_DIR}/src/profiler/centrol_profiler.cc
    ${PROJECT_SOURCE_DIR}/src/profiler/network_profiler.cc
    ${PROJECT_SOURCE_DIR}/src/profiler/resource_profiler.cc
    # ${PROJECT_SOURCE_DIR}/src/profiler/test_centrol_profiler.cc
    # ${PROJECT_SOURCE_DIR}/src/profiler/test_network_profiler.cc
    # ${PROJECT_SOURCE_DIR}/src/profiler/test_resource_profiler.cc
    ${PROJECT_SOURCE_DIR}/src/basic/string_util.cc
    ${PROJECT_SOURCE_DIR}/src/basic/system_util.cc
)
TARGET_LINK_LIBRARIES(resource_profiler_edge -lsqlite3)