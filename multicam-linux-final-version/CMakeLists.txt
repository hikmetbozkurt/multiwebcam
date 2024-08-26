cmake_minimum_required(VERSION 3.5)

project(cam LANGUAGES CXX)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(cam /home/pi/Desktop/multiwebcam/main.cpp
)

# Include directories
target_include_directories(cam PRIVATE ${OpenCV_INCLUDE_DIRS})

# Link against OpenCV libraries
target_link_libraries(cam PRIVATE ${OpenCV_LIBS})

# Installation rules
include(GNUInstallDirs)
install(TARGETS cam
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
