add_executable(dwc dwc/main.cpp)

target_include_directories(dwc PUBLIC ./)
target_include_directories(dwc PUBLIC ${CONAN_INCLUDE_DIRS})

target_link_libraries(dwc ${CONAN_LIBS})

# vim:ft=cmake
