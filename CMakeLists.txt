cmake_minimum_required(VERSION 3.9)
set(PROJECT_NAME ThermoPiTemp)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp TempSensor.cpp TempSensor.h)

# Add local Cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(PkgConfig REQUIRED)
find_package(WiringPi REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} curl)
target_link_libraries(${PROJECT_NAME} ${WIRINGPI_LIBRARIES})
