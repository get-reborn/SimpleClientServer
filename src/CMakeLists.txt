include_directories(
        ${CURRENT_PROJECT_DIR}/inc
)

add_library(${SIMPLE_LIB_NAME}
        SocketErr.cpp
        SocketClient.cpp
        SocketServer.cpp
        )

target_link_libraries(${SIMPLE_LIB_NAME}
        ws2_32
        )