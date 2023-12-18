#-Configuration-#
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 17)
project(Sapper VERSION 1.0.0 LANGUAGES CXX)

#-Add SFML-#
find_package(SFML 2.5 COMPONENTS graphics system window audio)
if (SFML_FOUND)
   message(STATUS "The SFML installed in the system will be used")
else ()
   # Если SFML не найден - выкачиваем и собираем из исходников
   message(STATUS "Get SFML from the source code")
   include(FetchContent)
   FetchContent_Declare(SFML
           GIT_REPOSITORY https://github.com/SFML/SFML.git
           GIT_TAG 2.5.x)
   set(SFML_BUILD_AUDIO ON CACHE BOOL "" FORCE)
   set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
   set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
   # Обход потенциальных проблем при сборке sfml-graphics
   # (stb_image.h: 'argument': conversion from 'int' to 'short')
   set(WARNINGS_AS_ERRORS FALSE CACHE BOOL "" FORCE)
   FetchContent_MakeAvailable(SFML)
endif ()
find_package(Threads REQUIRED)
set(SFML_LIBS sfml-system sfml-window sfml-graphics)
#sfml-audio)

#-Enabling files-#
include_directories(include)

file(GLOB SOURCES src/*.cpp include/*.h)

add_executable(Sapper ${SOURCES})
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   set(CMAKE_EXE_LINKER_FLAGS "-static") # для того чтобы можно было запускать приложение в Windows двойным
   #кликом и из командной строки без прописывания в PATH путей к бинарникам libstdc++
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "CLANG")
   set(CMAKE_EXE_LINKER_FLAGS "-static") # для того чтобы можно было запускать приложение в Windows двойным
   #кликом и из командной строки без прописывания в PATH путей к бинарникам libstdc++
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
   set(CMAKE_EXE_LINKER_FLAGS "/MT") # для того чтобы можно было запускать приложение в Windows двойным
endif()

target_link_libraries(Sapper ${SFML_LIBS})

#-Copy files to the binary directory-#
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#-Connecting MSVC libraries-#
#if(MSVC)
#   foreach(lib ${SFML_LIBS})
#      get_target_property(lib_path ${lib} LOCATION)
#      file(COPY ${lib_path} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#   endforeach()
#endif()