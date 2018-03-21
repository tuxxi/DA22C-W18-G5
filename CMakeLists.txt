cmake_minimum_required(VERSION 3.9)
project(DA22C_W18_G5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#comment this out to only compile the project with no gui.
#otherwise we're gonna be makin a sick gui so you'd better have Qt 5 installed
set(QT_GUI true)

#these definitions can be use to enable or disable testing various data structures
#add_definitions(-D_TESTING)    #testing everything, enable this for testing in general to be compiled

#individual data structures tests
#add_definitions(-D_VECTOR_TEST)
#add_definitions(-D_HASHTABLE_TEST)
#add_definitions(-D_BST_TEST)

set(PROJECT_SRCS
        src/HashTable.h
        src/LinkedList.h
        src/BinarySearchTree.h
        src/Vector.h

        src/Olympian.h
        src/OlympianDatabase.h src/OlympianDatabase.cpp
        src/tests/BSTTest.h src/tests/VectorTest.h src/tests/HashTableTest.h
        src/tests/Tests.cpp
        src/Util.h src/Util.cpp
        )

#normal executable
add_executable(DA22C_W18_G5 ${PROJECT_SRCS} src/main.cpp)


if (QT_GUI)
    set(CMAKE_AUTOMOC ON)
    #-- Platform dependant stuff --
    if (MSVC)
        set(GUI_TYPE WIN32)
    endif(MSVC)

    # Set some Apple MacOS Specific settings
    if (APPLE)
	    set(GUI_TYPE MACOSX_BUNDLE)
    endif (APPLE)

    #find Qt libraries
    find_package(Qt5Widgets)
    get_target_property(QtWidgets_location Qt5::Widgets LOCATION)
    find_package(Qt5Core)
    get_target_property(QtCore_location Qt5::Core LOCATION)
    find_package(Qt5Gui)
    get_target_property(QtGui_location Qt5::Gui LOCATION)

    #append extra gui-specific source files
    list(APPEND PROJECT_SRCS
        src/gui/main.cpp
        src/gui/mainwindow.hpp src/gui/mainwindow.cpp
        src/gui/olympianmodel.hpp src/gui/olympianmodel.cpp
    )
    add_executable(DA22C_W18_G5_GUI ${GUI_TYPE} ${PROJECT_SRCS})

    #set target link libraries
    target_link_libraries(DA22C_W18_G5_GUI 
        Qt5::Widgets
		Qt5::Core 
		Qt5::Gui)
endif()


