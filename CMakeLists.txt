project(helloworld)

cmake_minimum_required(VERSION 2.8)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(AMXConfig)
include(AddSAMPPlugin)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/amx
)

add_definitions(-DSAMPGDK_AMALGAMATION)

add_samp_plugin(helloworld
  amxplugin.cpp
  AttackBot.h
  Attacking.cpp
  DamageObject.h
  DamageObject.cpp
  helloworld.cpp
  helloworld.def
  sampgdk.c
  sampgdk.h
  streamer.hpp
  streamer.cpp
  callbacks.cpp
)

