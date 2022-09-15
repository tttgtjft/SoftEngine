cmake_minimum_required(VERSION 3.24.1 FATAL_ERROR)

set(PROJECT_NAME SoftEngine)

add_subdirectory(SoftEngineCore)
add_subdirectory(SoftEngineEditor)

set_property(DIRECTORY ${CMAKE_CUURENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SoftEngineCore)