find_package(SFML REQUIRED COMPONENTS system window graphics)

include_directories(
	${SFML_INCLUDE_DIR}
)

set(Engine_SOURCES
	Engine.hpp
	Engine.cpp
)

source_group(Engine FILES ${Engine_SOURCES})

add_executable(LD28 main.cpp ${Engine_SOURCES})
target_link_libraries(LD28 ${SFML_LIBRARIES})