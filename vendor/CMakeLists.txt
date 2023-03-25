include(FetchContent)
set(FETCHCONTENT_QUIET NO)

if (NOT sfml_FOUND)
    FetchContent_Declare(
            sfml
            GIT_REPOSITORY "https://github.com/SFML/SFML"
            GIT_TAG 2.5.x
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SFML
    )
    FetchContent_MakeAvailable(sfml)
endif ()