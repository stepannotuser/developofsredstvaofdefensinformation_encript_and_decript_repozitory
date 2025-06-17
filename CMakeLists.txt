cmake_minimum_required(VERSION 3.10)
project(developofsredstvaofdefensinformation_encript_and_decript_repozitory)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(developofsredstvaofdefensinformation_encript_and_decript_repozitory
    main.cpp
    Encryptor.cpp
)

if (WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

message("------------")
message("запуск производится в формате:")
message("./developofsredstvaofdefensinformation_encript_and_decript_repozitory <путь к папке для шифрования> <password> <mode encrypt/decrypt> [--show]")
message("------------")
