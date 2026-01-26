# CMakeList.txt: proyecto de CMake para IsometricGame, incluya el origen y defina
# la lógica específica del proyecto aquí.
#
cmake_minimum_required (VERSION 3.10)

# Habilite Recarga activa para los compiladores de MSVC si se admiten.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("IsometricGame")

# Agregue un origen al ejecutable de este proyecto.
add_executable (IsometricGame "IsometricGame.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET IsometricGame PROPERTY CXX_STANDARD 20)
endif()

# TODO: Agregue pruebas y destinos de instalación si es necesario.

# 1.- Generar variable
set(SDL3_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3")

# 2.- Incluir headers
target_include_directories(IsometricGame PRIVATE "${SDL3_DIR}/include")

# 3.- Enlazar librerías
target_link_libraries(IsometricGame PRIVATE "${SDL3_DIR}/lib/x64/SDL3.lib")

# 4.- copiar automaticamente los dll al lado del .exe
add_custom_command(TARGET IsometricGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/lib/SDL3/lib/x64/SDL3.dll"
        "$<TARGET_FILE_DIR:IsometricGame>"
)