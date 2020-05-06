
cmake_minimum_required(VERSION 3.0)

project(Krampus)

set(KEngineSRC "")
file(GLOB_RECURSE KEngineSRC "src/*")

set(SHARED TRUE)

set(EAGLE_DIR   "/usr"      CACHE PATH "Eagle directory")
set(ALLEGRO_DIR "/usr" CACHE PATH "Allegro 5 directory")
set(NILOREA_DIR "/usr"    CACHE PATH "Nilorea directory")

set(EAGLE_INCLUDE_DIR   "${EAGLE_DIR}/include"   CACHE PATH "Eagle include directory"     FORCE)
set(ALLEGRO_INCLUDE_DIR "${ALLEGRO_DIR}/include" CACHE PATH "Allegro 5 include directory" FORCE)
set(NILOREA_INCLUDE_DIR "${NILOREA_DIR}/include" CACHE PATH "Nilorea include directory"   FORCE)

set(INCLUDE_DIRS "./include" "${EAGLE_INCLUDE_DIR}" "${ALLEGRO_INCLUDE_DIR}" "${NILOREA_INCLUDE_DIR}" CACHE STRING "Include directories" FORCE)

set(EAGLE_LIB_DIR   "${EAGLE_DIR}/lib"   CACHE PATH "Eagle lib directory"     FORCE)
set(ALLEGRO_LIB_DIR "${ALLEGRO_DIR}/lib" CACHE PATH "Allegro 5 lib directory" FORCE)
set(NILOREA_LIB_DIR "${NILOREA_DIR}/lib" CACHE PATH "Nilorea lib directory"   FORCE)

find_library(EAGLE_LIB    EagleShared      ${EAGLE_LIB_DIR})
find_library(EAGLE_A5_LIB EagleA5Shared    ${EAGLE_LIB_DIR})
find_library(ALLEGRO_LIB  allegro_monolith ${ALLEGRO_LIB_DIR})
find_library(NILOREA_LIB  NiloreaShared          ${NILOREA_LIB_DIR})

find_library(EAGLE_DEBUG_LIB    EagleDebugShared       ${EAGLE_LIB_DIR})
find_library(EAGLE_DEBUG_A5_LIB EagleA5DebugShared     ${EAGLE_LIB_DIR})
find_library(ALLEGRO_DEBUG_LIB  allegro_monolith-debug ${ALLEGRO_LIB_DIR})
find_library(NILOREA_DEBUG_LIB  NiloreaDebugShared                ${NILOREA_LIB_DIR})

find_library(EAGLE_STATIC_LIB    EagleStatic             ${EAGLE_LIB_DIR})
find_library(EAGLE_STATIC_A5_LIB EagleA5Static           ${EAGLE_LIB_DIR})
find_library(ALLEGRO_STATIC_LIB  allegro_monolith-static ${ALLEGRO_LIB_DIR})
find_library(NILOREA_STATIC_LIB  Nilorea                 ${NILOREA_LIB_DIR})

find_library(EAGLE_DEBUG_STATIC_LIB    EagleDebugStatic       ${EAGLE_LIB_DIR})
find_library(EAGLE_DEBUG_STATIC_A5_LIB EagleA5DebugStatic     ${EAGLE_LIB_DIR})
find_library(ALLEGRO_DEBUG_STATIC_LIB  allegro_monolith-debug-static ${ALLEGRO_LIB_DIR})
find_library(NILOREA_DEBUG_STATIC_LIB  NiloreaDebug                ${NILOREA_LIB_DIR})

set(LIBS_RELEASE "${EAGLE_LIB}" "${EAGLE_A5_LIB}" "${ALLEGRO_LIB}" "${NILOREA_LIB}" CACHE STRING "Link libs" FORCE)

set(LIBS_DEBUG "${EAGLE_DEBUG_LIB}" "${EAGLE_DEBUG_A5_LIB}" "${ALLEGRO_DEBUG_LIB}" "${NILOREA_DEBUG_LIB}" CACHE STRING "Link libs" FORCE)

set(LIBS_STATIC "${EAGLE_STATIC_LIB}" "${EAGLE_STATIC_A5_LIB}" "${ALLEGRO_STATIC_LIB}" "${NILOREA_STATIC_LIB}" CACHE STRING "Link libs" FORCE)

set(LIBS_DEBUG_STATIC "${EAGLE_DEBUG_STATIC_LIB}" "${EAGLE_DEBUG_STATIC_A5_LIB}" "${ALLEGRO_DEBUG_STATIC_LIB}" "${NILOREA_DEBUG_STATIC_LIB}" CACHE STRING "Link libs" FORCE)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
   if (SHARED) 
      set(LINK_LIBS "${LIBS_DEBUG}" CACHE STRING "Link libraries" FORCE)
   else()
      set(LINK_LIBS "${LIBS_STATIC_DEBUG}" CACHE STRING "Link libraries" FORCE)
   endif(SHARED)
else()
   if (SHARED)
      set(LINK_LIBS "${LIBS_RELEASE}" CACHE STRING "Link libraries" FORCE)
   else()
      set(LINK_LIBS "${LIBS_STATIC}" CACHE STRING "Link libraries" FORCE)
   endif()
endif()

add_library(KrampusObjects OBJECT "${KEngineSRC}")

target_include_directories(KrampusObjects PUBLIC "${INCLUDE_DIRS}")

target_compile_definitions(KrampusObjects PRIVATE DEBUG)
target_compile_definitions(KrampusObjects PRIVATE EAGLE_LOG)
target_compile_definitions(KrampusObjects PRIVATE EAGLE_ASSERT_EXCEPTION)
target_compile_options(KrampusObjects PRIVATE "-g")





add_executable(KrampusMasterServer "krampus_server_client_test.cpp")# ${KrampusObjects})
add_executable(KrampusServer "krampus_server.cpp")# ${KrampusObjects})
add_executable(KrampusClient "krampus_client.cpp")# ${KrampusObjects})

set(PFX "Krampus")

set(TSFX "")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
   string(APPEND TSFX "Debug")
   
   if (NOT WIN32)
      target_compile_options(KrampusMasterServer PUBLIC "-fsanitize=address")
      target_compile_options(KrampusServer PUBLIC "-fsanitize=address")
      target_compile_options(KrampusClient PUBLIC "-fsanitize=address")
   endif()
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

target_link_libraries(KrampusMasterServer PUBLIC ${LINK_LIBS})
target_link_libraries(KrampusServer       PUBLIC ${LINK_LIBS})
target_link_libraries(KrampusClient       PUBLIC ${LINK_LIBS})

if (WIN32)
   target_link_libraries(KrampusMasterServer PUBLIC ws2_32)
   target_link_libraries(KrampusServer       PUBLIC ws2_32)
   target_link_libraries(KrampusClient       PUBLIC ws2_32)
else()
   target_link_libraries(KrampusMasterServer PUBLIC asan.so pthread)
   target_link_libraries(KrampusServer       PUBLIC asan.so pthread)
   target_link_libraries(KrampusClient       PUBLIC asan.so pthread)
endif()

install(TARGETS KrampusMasterServer KrampusServer KrampusClient DESTINATION ${CMAKE_INSTALL_PREFIX})
