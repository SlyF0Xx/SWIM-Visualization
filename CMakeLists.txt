cmake_minimum_required(VERSION 3.13)
project(Visualization)

set(CMAKE_CXX_STANDARD 17)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run uic automatically when needed.
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets REQUIRED)
#find_package(Boost 1.72.0)
find_package (Qt5Declarative)




add_executable(Visualization main.cpp structures/Member.cpp structures/Member.h drawers/MemberDrawer2.cpp drawers/MemberDrawer2.h drawers/MainWidget.cpp drawers/MainWidget.h structures/Message.cpp structures/Message.h drawers/MessageDrawer.cpp drawers/MessageDrawer.h structures/IMessageEnvironment.h drawers/MessageEnvironmentForDrawers.cpp drawers/MessageEnvironmentForDrawers.h structures/ITimer.h drawers/VisualTimer.cpp drawers/VisualTimer.h drawers/Details.h drawers/Details.cpp)
#add_executable(Visualization main.cpp structures/Member.cpp structures/Member.h drawers/MemberDrawer2.cpp drawers/MemberDrawer2.h drawers/MainWidget.cpp drawers/MainWidget.h structures/Message.cpp structures/Message.h drawers/MessageDrawer.cpp drawers/MessageDrawer.h structures/IMessageEnvironment.h drawers/MessageEnvironmentForDrawers.cpp drawers/MessageEnvironmentForDrawers.h structures/ITimer.h drawers/VisualTimer.cpp drawers/VisualTimer.h)

#set(UI drawers/Details.ui)


#target_compile_options(Visualization PUBLIC -pthread)
target_link_libraries(Visualization Qt5::Widgets)
#target_link_libraries(Visualization pthread)

