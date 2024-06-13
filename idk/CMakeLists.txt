cmake_minimum_required(VERSION 3.10)

# Укажите имя проекта
project(CoalStab)

# Укажите стандарт C++
set(CMAKE_CXX_STANDARD 20)

# Укажите путь к библиотекам
set(LIBRARY_PATH "/Users/admi/Documents/idk")

# Создание исполняемого файла
add_executable(CoalStab main.cpp)

# Связывание исполняемого файла с динамическими библиотеками
target_link_libraries(CoalStab
    ${LIBRARY_PATH}/libdlyb.dylib
    ${LIBRARY_PATH}/libslopesafety.dylib
    ${LIBRARY_PATH}/libslopeangle.dylib
    ${LIBRARY_PATH}/libstability.dylib
    ${LIBRARY_PATH}/libstaticstability.dylib
    ${LIBRARY_PATH}/libtension.dylib
    ${LIBRARY_PATH}/libsafetyfactor.dylib
)

# Настройка rpath для поиска динамических библиотек
set_target_properties(CoalStab PROPERTIES
    BUILD_RPATH "${LIBRARY_PATH}"
    INSTALL_RPATH "${LIBRARY_PATH}"
)

# Найдите библиотеку SQLite3
find_package(SQLite3 REQUIRED)

# Подключите директории включения
include_directories(${SQLite3_INCLUDE_DIRS})

# Свяжите исполняемый файл с библиотекой SQLite3
target_link_libraries(CoalStab ${SQLite3_LIBRARIES})
