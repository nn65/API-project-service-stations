cmake_minimum_required(VERSION 3.23)
project(API-project-service-stations C)

set(CMAKE_C_STANDARD 11)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -O2 -g3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -static-libasan -Wall -Werror -O2 -g3")
#Sbloccarli solo per misurare le performance in clion. Per il resto usare quelli sopra.
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

add_executable(API-project-service-stations main.c)

project(API-project-service-stations C)
