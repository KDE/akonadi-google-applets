cmake_minimum_required(VERSION  2.6)

option(ALL_COLLECTIONS "Support for all Akonadi collections" FALSE)
if (ALL_COLLECTIONS)
    add_definitions(-DALL_COLLECTIONS)
endif (ALL_COLLECTIONS)

add_subdirectory(src)
