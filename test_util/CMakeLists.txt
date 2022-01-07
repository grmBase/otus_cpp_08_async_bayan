cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})


# имя проекта
project(async VERSION ${PROJECT_VESRION})


# наверно нужен для gtest
find_package(Threads)


# имя основной утилиты взял из примера - там была async_cli
add_executable(async_cli
  main.cpp)

set_target_properties(async_cli 
  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(async_cli
  PRIVATE 
    ${PROJECT_SOURCE_DIR}/../include/
)


target_link_libraries(async_cli
  async
  ${CMAKE_THREAD_LIBS_INIT}
)


if (MSVC)
    target_compile_options(async_cli PRIVATE
        /W4
    )
else ()
    target_compile_options(async_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()


