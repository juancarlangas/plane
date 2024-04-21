CMAKE_MINIMUM_REQUIRED(VERSION 3.22.1)

PROJECT(Plane
	VERSION 0.1
	DESCRIPTION "Para para analizar cuestionario de Lalo Plane"
	LANGUAGES CXX
	)

ADD_EXECUTABLE(Plane
	"${CMAKE_CURRENT_SOURCE_DIR}/src/app/main.cpp"
	)

target_compile_features(Plane PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(Plane PRIVATE /W4 /WX)
else()
    target_compile_options(Plane PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

set_target_properties(Plane PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
	OUTPUT_NAME "analizer-amd64"
	)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
