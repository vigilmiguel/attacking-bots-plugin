project(helloworld)

cmake_minimum_required(VERSION 2.8)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(AMXConfig)
include(AddSAMPPlugin)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/amx
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Headers
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Sources
)

set(HEADERS 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Headers/amx_var.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Headers/AttackBot.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Headers/DamageObject.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Headers/plugin.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Headers/plugincommon.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Headers/sampgdk.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Headers/streamer.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Headers/tick.h
)

set(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Sources/amxplugin.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Sources/AttackBot.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Sources/callbacks.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Sources/DamageObject.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Sources/helloworld.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Sources/helloworld.def
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Sources/sampgdk.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Sources/streamer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Sources/streamer.def
)

add_definitions(-DSAMPGDK_AMALGAMATION)

add_samp_plugin(helloworld
  ${HEADERS}
  ${SOURCES}
)

