cmake_minimum_required(VERSION 3.12)
project(UNOServer)

set(CMAKE_CXX_STANDARD 14)

add_executable(UNOServer src/main.cpp src/Util/OnlineUtil.h src/Util/OnlineUtil.cpp src/Model/UNOCard.h
        src/Model/CardDeck.h src/Model/GameConstants.h
        src/Service/UserService.h src/Service/GameService.h
        src/Model/GameTable.h src/Model/Player.h src/Model/Dealer.h
        src/Util/TimeUtil.h src/Service/UserService.cpp src/Util/TimeUtil.cpp src/Model/Request.h
        src/Service/GameService.cpp src/Model/GameTable.cpp src/Model/CardDeck.cpp src/Model/Dealer.cpp
        src/Model/UNOCard.cpp src/Model/Player.cpp
        src/Util/InfoSecurity.cpp src/Util/InfoSecurity.h)

find_package(Threads)
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(UNOServer ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(UNOServer ${Boost_LIBRARIES})

