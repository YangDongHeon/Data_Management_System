cmake_minimum_required(VERSION 3.16)

project(MyQtApp)

# Find and include Qt6
find_package(Qt6 COMPONENTS Widgets Sql REQUIRED)

set(CMAKE_AUTOMOC ON)

# Add include directories for Qt modules
include_directories(${Qt6Widgets_INCLUDE_DIRS})
include_directories(${Qt6Sql_INCLUDE_DIRS})

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add your source files
add_executable(MyQtApp
    main.cpp
    MainWindow.cpp
    MainWindow.h
    DataDialog.cpp
    DataDialog.h
    SearchResultDialog.cpp
    SearchResultDialog.h
    CustomItemDelegate.cpp
    CustomItemDelegate.h
    ItemDetailsWidget.h
    ItemDetailsWidget.cpp
)

# Link against the required Qt modules
target_link_libraries(MyQtApp PRIVATE Qt6::Widgets Qt6::Sql)
