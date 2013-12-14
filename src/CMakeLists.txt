find_package(SFML REQUIRED COMPONENTS system window graphics)
FIND_PACKAGE(Boost REQUIRED COMPONENTS system thread)

include_directories(
    ${SFML_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
)

link_directories(
    ${Boost_LIBRARY_DIRS}
)

set(Engine_SOURCES
    Engine.hpp
    Engine.cpp

    Resources.hpp
    Resources.cpp

    SpriteSheet.hpp
    SpriteSheet.cpp

    DataFile.hpp
    DataFile.cpp
)

set(Game_SOURCES
    Game/Menu.hpp
    Game/Menu.cpp

    Game/Countdown.hpp
    Game/Countdown.cpp

    Game/World.hpp
    Game/World.cpp

    Game/Player.hpp
    Game/Player.cpp

    Game/Weapon.hpp
    Game/Weapon.cpp
    Game/Bullet.hpp
    Game/Bullet.cpp

    Game/Enemy.hpp
    Game/Enemy.cpp
)

source_group(Engine FILES ${Engine_SOURCES})
source_group(Game FILES ${Game_SOURCES})

if (MSVC)
    if (${MSVC_VERSION} VERSION_LESS 10.0)
        message(FATAL_ERROR "You need to compile this project with at least Visual Studio 2010")
    endif ()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
elseif (UNIX)
    if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
        execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
        if (${GCC_VERSION} VERSION_GREATER 4.7 OR ${GCC_VERSION} VERSION_EQUAL 4.7)
            set(C11 "-std=c++11")
        elseif (${GCC_VERSION} VERSION_GREATER 4.3 OR ${GCC_VERSION} VERSION_EQUAL 4.3)
            set(C11 "-std=c++0x")
        else ()
            message(FATAL_ERROR "You need at least GCC 4.3 to compile this.")
        endif ()
    elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        set(C11 "-std=c++11 -stdlib=libc++")
    endif ()

    set(CMAKE_CXX_FLAGS                "-Wall -Wextra ${C11} -pthread")
    set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
    set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
endif ()

add_executable(LD28 main.cpp ${Engine_SOURCES} ${Game_SOURCES})
target_link_libraries(LD28 Kunlaboro ${SFML_LIBRARIES})