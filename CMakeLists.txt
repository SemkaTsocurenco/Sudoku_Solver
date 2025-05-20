
cmake_minimum_required(VERSION 3.16)
project(SudokuSolver LANGUAGES CXX)

# -------------------------------------------------------------------
#  Компилятор и стандарт языка
# -------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -------------------------------------------------------------------
#  Qt5 Widgets
# -------------------------------------------------------------------
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# -------------------------------------------------------------------
#  OpenMP
# -------------------------------------------------------------------
find_package(OpenMP REQUIRED)

# -------------------------------------------------------------------
#  Исходники проекта
# -------------------------------------------------------------------
set(SRCS
    main.cpp
    mainwindow.cpp
)

set(HDRS
    mainwindow.h
    solver.h
)

# -------------------------------------------------------------------
#  Исполняемый файл
# -------------------------------------------------------------------
add_executable(Sudoku_solver ${SRCS} ${HDRS})

target_link_libraries(Sudoku_solver
    Qt5::Widgets
    OpenMP::OpenMP_CXX
)

# Qt нуждается в MOC / RCC, если используются сигналы-слоты или ресурсы
set_target_properties(Sudoku_solver PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# -------------------------------------------------------------------
#  Доп. предупреждения (gcc/clang)
# -------------------------------------------------------------------
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Sudoku_solver PRIVATE -Wall -Wextra -pedantic)
endif()
