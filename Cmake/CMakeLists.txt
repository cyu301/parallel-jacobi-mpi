cmake_minimum_required(VERSION 3.9)

project(Jacobi
        DESCRIPTION "Parallelized Jacobi"
        LANGUAGES CXX)

find_package(MPI REQUIRED)

set(Jacobi_src 
    main.cpp
    cart2dGrid.h
    cart2dGrid.cpp
    dataIO.h
    dataIO.cpp
    jacobiGrid.h
    jacobiGrid.cpp
)

set(NAME jacobi)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})
add_executable(${NAME} ${Jacobi_src})
target_link_libraries(${NAME} PUBLIC ${MPI_CXX_LIBRARIES})
target_link_libraries(${NAME} PUBLIC m)