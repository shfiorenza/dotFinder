# find required libraries
find_package(OpenCV REQUIRED)
find_package(GSL REQUIRED)

set(LIB ${GSL_LIBRARIES} ${OpenCV_LIBRARIES})
set(INCLUDES ${GSL_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIR})
include_directories(${INCLUDES})

file(GLOB HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/dotFinder/*.hpp")
file(GLOB SOURCES "*.cpp")

# Make a static library (tradeoff of memory for better performance)
add_library(dotFinder STATIC ${SOURCES} ${HEADERS})
# Add an alias so the target can be used while building
add_library(dotFinder::dotFinder ALIAS dotFinder)
target_link_libraries(dotFinder ${LIB})

# Set the include directory for builds (tests, examples, etc) and for clients
target_include_directories(dotFinder PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

# Associate the include files with the target, so they will be installed
set_target_properties(dotFinder PROPERTIES PUBLIC_HEADER "${HEADERS}")

# Set install targets and destinations
install(TARGETS dotFinder
    EXPORT dotFinderTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/dotFinder)

# Export additional install files (for clients using CMake)
install(EXPORT dotFinderTargets
    NAMESPACE dotFinder::
    FILE dotFinderConfig.cmake
    DESTINATION lib/cmake/dotFinder)

# Add the executable subdirectory
add_subdirectory("executable")
