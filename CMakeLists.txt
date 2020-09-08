cmake_minimum_required(VERSION 3.16)

project(abaddon)

include(FindPackageHandleStandardArgs)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)

# gtkmm
find_path(GTKMM_INCLUDE_DIRS gtkmm.h)
find_library(GTKMM_LIBRARY gtkmm)
link_libraries(${GTKMM_LIBRARY})
include_directories(${GTKMM_INCLUDE_DIRS})

# glibmm
find_path(GLIBMM_INCLUDE_DIRS glibmm.h)
find_library(GLIBMM_LIBRARY glibmm)
link_libraries(${GLIBMM_LIBRARY})
include_directories(${GLIBMM_INCLUDE_DIRS})

# libsigc++
find_path(SIGC_INCLUDE_DIRS sigc++/sigc++.h)
find_library(SIGC_LIBRARY sigc-2.0)
link_libraries(${SIGC_LIBRARY})
include_directories(${SIGC_INCLUDE_DIRS})

# giomm
find_path(GIOMM_INCLUDE_DIRS giomm.h)
find_library(GIOMM_LIBRARY giomm)
link_libraries(${GIOMM_LIBRARY})
include_directories(${GIOMM_INCLUDE_DIRS})

# gdkmm
find_path(GDKMM_INCLUDE_DIRS gdkmm.h)
find_library(GDKMM_LIBRARY gdkmm)
link_libraries(${GDKMM_LIBRARY})
include_directories(${GDKMM_INCLUDE_DIRS})

# atkmm
find_path(ATKMM_INCLUDE_DIRS atkmm.h)
find_library(ATKMM_LIBRARY atkmm)
link_libraries(${ATKMM_LIBRARY})
include_directories(${ATKMM_INCLUDE_DIRS})

# libcurl
find_package(curl)
find_library(CURL_LIBRARY libcurl)
link_libraries(${CURL_LIBRARY})

# cpr
find_path(CPR_INCLUDE_DIRS cpr/cpr.h)
find_library(CPR_LIBRARY cpr)
include_directories(${CPR_INCLUDE_DIRS})
link_libraries(${CPR_LIBRARY})

# zlib
find_package(zlib)
find_library(ZLIB_LIBRARY zlib)
include_directories(${ZLIB_INCLUDE_DIRS})
link_libraries(${ZLIB_LIBRARY})

# mbedtls
find_library(MBEDTLS_X509_LIBRARY mbedx509)
find_library(MBEDTLS_TLS_LIBRARY mbedtls)
find_library(MBEDTLS_CRYPTO_LIBRARY mbedcrypto)
include_directories(${MBEDTLS_INCLUDE_DIRS})
link_libraries(${MBEDTLS_TLS_LIBRARY})
link_libraries(${MBEDTLS_X509_LIBRARY})
link_libraries(${MBEDTLS_CRYPTO_LIBRARY})

# nlohmann/json
find_package(nlohmann_json 3.2.0 REQUIRED)

# ixwebsocket
find_path(IXWEBSOCKET_INCLUDE_DIRS ixwebsocket/ixwebsocket.h)
find_library(IXWEBSOCKET_LIBRARY ixwebsocket)
link_libraries(${IXWEBSOCKET_LIBRARY})
include_directories(${IXWEBSOCKET_LIBRARY}})

if(mingw)
	link_libraries(ws2_32 wsock2)
endif()

add_executable(abaddon
	abaddon.hpp
	util.hpp
	settings.hpp
	settings.cpp
	abaddon.cpp
	components/channels.hpp
	components/channels.cpp
	components/chatmessage.hpp
	components/chatmessage.cpp
	components/chatwindow.hpp
	components/chatwindow.cpp
	components/memberlist.hpp
	components/memberlist.cpp
	dialogs/editmessage.hpp
	dialogs/editmessage.cpp
	dialogs/token.hpp
	dialogs/token.cpp
	discord/discord.hpp
	discord/discord.cpp
	discord/http.hpp
	discord/http.cpp
	discord/objects.hpp
	discord/objects.cpp
	discord/store.hpp
	discord/store.cpp
	discord/websocket.hpp
	discord/websocket.cpp
	windows/mainwindow.hpp
	windows/mainwindow.cpp
)
