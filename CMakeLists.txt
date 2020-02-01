
cmake_minimum_required(VERSION 3.0)

project(Krampus)

set(KEngineSRC "")
file(GLOB_RECURSE KEngineSRC "src/*")

set(SHARED TRUE)

set(EAGLE_DIR   "/usr/local/Eagle"      CACHE PATH "Eagle directory")
set(ALLEGRO_DIR "/usr/local/allegro525" CACHE PATH "Allegro 5 directory")
set(NILOREA_DIR "/usr/local/nilorea"    CACHE PATH "Nilorea directory")

set(EAGLE_INCLUDE_DIR   "${EAGLE_DIR}/include"   CACHE PATH "Eagle include directory"     FORCE)
set(ALLEGRO_INCLUDE_DIR "${ALLEGRO_DIR}/include" CACHE PATH "Allegro 5 include directory" FORCE)
set(NILOREA_INCLUDE_DIR "${NILOREA_DIR}/include" CACHE PATH "Nilorea include directory"   FORCE)

set(INCLUDE_DIRS "./include" "${EAGLE_INCLUDE_DIR}" "${ALLEGRO_INCLUDE_DIR}" "${NILOREA_INCLUDE_DIR}" CACHE STRING "Include directories" FORCE)

set(EAGLE_LIB_DIR   "${EAGLE_DIR}/lib"   CACHE PATH "Eagle lib directory"     FORCE)
set(ALLEGRO_LIB_DIR "${ALLEGRO_DIR}/lib" CACHE PATH "Allegro 5 lib directory" FORCE)
set(NILOREA_LIB_DIR "${NILOREA_DIR}/lib" CACHE PATH "Nilorea lib directory"   FORCE)


unset (EAGLE_LIB    CACHE)
unset (EAGLE_A5_LIB CACHE)
unset (ALLEGRO_LIB  CACHE)
unset (NILOREA_LIB  CACHE)


if ("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")

   if (SHARED)
      find_library(EAGLE_LIB    EagleDebugShared       ${EAGLE_LIB_DIR})
      find_library(EAGLE_A5_LIB EagleA5DebugShared     ${EAGLE_LIB_DIR})
      find_library(ALLEGRO_LIB  allegro_monolith-debug ${ALLEGRO_LIB_DIR})
      find_library(NILOREA_LIB  nilorea                ${NILOREA_LIB_DIR})
   else()
      find_library(EAGLE_LIB    EagleDebugStatic              ${EAGLE_LIB_DIR})
      find_library(EAGLE_A5_LIB EagleA5DebugStatic            ${EAGLE_LIB_DIR})
      find_library(ALLEGRO_LIB  allegro_monolith-debug-static ${ALLEGRO_LIB_DIR})
      find_library(NILOREA_LIB  nilorea                       ${NILOREA_LIB_DIR})
   endif()

else() #CMAKE_BUILD_TYPE != DEBUG

   if (SHARED)
      find_library(EAGLE_LIB    EagleShared      ${EAGLE_LIB_DIR})
      find_library(EAGLE_A5_LIB EagleA5Shared    ${EAGLE_LIB_DIR})
      find_library(ALLEGRO_LIB  allegro_monolith ${ALLEGRO_LIB_DIR})
      find_library(NILOREA_LIB  nilorea          ${NILOREA_LIB_DIR})
   else()
      find_library(EAGLE_LIB    EagleStatic             ${EAGLE_LIB_DIR})
      find_library(EAGLE_A5_LIB EagleA5Static           ${EAGLE_LIB_DIR})
      find_library(ALLEGRO_LIB  allegro_monolith-static ${ALLEGRO_LIB_DIR})
      find_library(NILOREA_LIB  nilorea                 ${NILOREA_LIB_DIR})
   endif()

endif()

set(LIBS "${EAGLE_LIB}" "${EAGLE_A5_LIB}" "${ALLEGRO_LIB}" "${NILOREA_LIB}" CACHE STRING "Link libs" FORCE)


add_library(KrampusObjects OBJECT "${KEngineSRC}")

target_include_directories(KrampusObjects PUBLIC "${INCLUDE_DIRS}")

add_executable(KrampusMasterServer "krampus_master_server.cpp")# ${KrampusObjects})
add_executable(KrampusServer "krampus_server.cpp")# ${KrampusObjects})
add_executable(KrampusClient "krampus_main.cpp")# ${KrampusObjects})

set(PFX "Krampus")
set(TSFX "")
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
   string(APPEND TSFX "Debug")
endif()
set(ESFX "")
if (WIN32)
#   string(APPEND ESFX ".exe")
endif()
set (SFX "${TSFX}${ESFX}")

set_target_properties(KrampusMasterServer PROPERTIES OUTPUT_NAME "${PFX}MasterServer${SFX}")
set_target_properties(KrampusServer       PROPERTIES OUTPUT_NAME "${PFX}Server${SFX}")
set_target_properties(KrampusClient       PROPERTIES OUTPUT_NAME "${PFX}Client${SFX}")

target_include_directories(KrampusMasterServer PUBLIC ${INCLUDE_DIRS})
target_include_directories(KrampusServer       PUBLIC ${INCLUDE_DIRS})
target_include_directories(KrampusClient       PUBLIC ${INCLUDE_DIRS})

target_link_libraries(KrampusMasterServer PUBLIC KrampusObjects)
target_link_libraries(KrampusServer       PUBLIC KrampusObjects)
target_link_libraries(KrampusClient       PUBLIC KrampusObjects)

target_link_libraries(KrampusMasterServer PUBLIC ${LIBS})
target_link_libraries(KrampusServer       PUBLIC ${LIBS})
target_link_libraries(KrampusClient       PUBLIC ${LIBS})

if (WIN32)
   target_link_libraries(KrampusMasterServer PUBLIC ws2_32)
   target_link_libraries(KrampusServer       PUBLIC ws2_32)
   target_link_libraries(KrampusClient       PUBLIC ws2_32)
endif()

install(TARGETS KrampusMasterServer KrampusServer KrampusClient DESTINATION ${CMAKE_INSTALL_PREFIX})
