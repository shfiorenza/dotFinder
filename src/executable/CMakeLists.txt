set(EXECUTABLE_OUTPUT_PATH ${dotFinder_SOURCE_DIR})
add_executable(dot_finder.exe "main.cpp")
target_link_libraries(dot_finder.exe dotFinder)
install(TARGETS dot_finder.exe DESTINATION bin)
