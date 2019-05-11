# Function for grouping files. Used for filters in Visual Studio.
function(createSourceGroups files rootPath)
	foreach(_source IN ITEMS ${files} )
		get_filename_component(_source_path "${_source}" PATH)
		file(RELATIVE_PATH _source_path_rel "${rootPath}" "${_source_path}")
		#message("${_source_path} ${_source_path_rel} ${rootPath}")
		string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
		source_group("${_group_path}" FILES "${_source}")
	endforeach()
endfunction(createSourceGroups)


project (Guise)
cmake_minimum_required(VERSION 2.8)

option(CODE_COVERAGE "Enable coverage reporting" OFF)
option(ENABLE_OPENGL_RENDERER "Enable default OpenGL renderer" ON)

# Require C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads)


# Warnings as errors
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()


if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")


# Find files
file(GLOB_RECURSE guise_src "${CMAKE_SOURCE_DIR}/source/*.cpp")
file(GLOB_RECURSE guise_inc "${CMAKE_SOURCE_DIR}/include/*.hpp" "${CMAKE_SOURCE_DIR}/include/*.inl")
file(GLOB guise_files ${guise_src} ${guise_inc})
file(GLOB_RECURSE test_files "${CMAKE_SOURCE_DIR}/tests/*.cpp" "${CMAKE_SOURCE_DIR}/tests/*.hpp")
file(GLOB_RECURSE example1_files "${CMAKE_SOURCE_DIR}/examples/example1.cpp")

# Guise dynamic lib
include_directories ("${CMAKE_SOURCE_DIR}/include") # Include header files
add_library(guise SHARED ${guise_files}) # Create shared library

if(ENABLE_OPENGL_RENDERER AND WIN32)
	target_link_libraries(guise "opengl32.lib")
endif(ENABLE_OPENGL_RENDERER AND WIN32)

createSourceGroups("${guise_src}" "${CMAKE_SOURCE_DIR}/source")
createSourceGroups("${guise_inc}" "${CMAKE_SOURCE_DIR}/include")

target_compile_definitions(guise PRIVATE GUISE_EXPORT)
install(TARGETS guise DESTINATION "${CMAKE_SOURCE_DIR}lib") # Install library
set_target_properties( guise
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/lib"
  ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang") ## Code coverage
  set_target_properties(guise PROPERTIES COMPILE_FLAGS "--coverage")
endif(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

install(FILES ${guise_inc} DESTINATION "${CMAKE_SOURCE_DIR}/include") # Install library headers



# Guise static lib
include_directories ("${CMAKE_SOURCE_DIR}/include")
add_library(guise_static STATIC ${guise_files}) # Create shared library

createSourceGroups("${guise_src}" "${CMAKE_SOURCE_DIR}/source")
createSourceGroups("${guise_inc}" "${CMAKE_SOURCE_DIR}/include")

target_compile_definitions(guise_static PRIVATE GUISE_STATIC)
install(TARGETS guise_static DESTINATION "${CMAKE_SOURCE_DIR}lib") # Install library
set_target_properties( guise_static
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/lib"
  ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib"
)
install(FILES ${guise_inc} DESTINATION "${CMAKE_SOURCE_DIR}/include") # Install library headers


# Test
SET(gtest_force_shared_crt on CACHE BOOL
  "Use shared (DLL) run-time lib even when Google Test is built as static lib.")
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/googletest/googletest
                 ${CMAKE_SOURCE_DIR}/vendor/googletest/googletest
                 EXCLUDE_FROM_ALL) # Add googletest directly to our build.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

include_directories ("${CMAKE_SOURCE_DIR}/include")
link_directories("${CMAKE_SOURCE_DIR}/lib")
add_executable(guise_tests ${test_files})
set_target_properties( guise_tests
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang") ## Code coverage
  set_target_properties(guise_tests PROPERTIES COMPILE_FLAGS "--coverage")
endif(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

target_link_libraries(guise_tests gtest_main guise)


# Example 1
include_directories ("${CMAKE_SOURCE_DIR}/include")
link_directories("${CMAKE_SOURCE_DIR}/lib")
add_executable(example1 ${example1_files})
set_target_properties( example1
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
)
target_link_libraries(example1 guise ${CMAKE_THREAD_LIBS_INIT})