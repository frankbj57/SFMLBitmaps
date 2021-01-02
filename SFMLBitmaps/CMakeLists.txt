cmake_minimum_required(VERSION 3.0.0)
project(SFMLBitmaps VERSION 0.1.0)

add_executable(SFMLBitmaps 
    SFMLBitmaps.cpp 
    BitmapsApp.h BitmapsApp.cpp 
    SFApp.h SFApp.cpp
    Bitmap.h Bitmap.cpp
    CellularAutomaton.h CellularAutomaton.cpp
    CellularAutomatonDefinitions.h
    NeighborHood.h NeighborHood.cpp
    NextStateFunction.h NextStateFunction.cpp
    )

target_link_libraries(SFMLBitmaps sfml-graphics sfml-window sfml-system)
