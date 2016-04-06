cmake_minimum_required(VERSION 3.1)

set(DIR ${DIR}/app)

set(APP_SOURCES
    ${DIR}/application.cpp
    ${DIR}/keyboard_event.cpp
    ${DIR}/mouse_event.cpp
    ${DIR}/application_event.cpp
    ${DIR}/sdl_utility.cpp
    
    PARENT_SCOPE)
    
set(APP_HEADERS
    ${DIR}/application.hpp
    ${DIR}/basic_event.hpp
    ${DIR}/event_visitor.hpp
    ${DIR}/keyboard_event.hpp
    ${DIR}/mouse_event.hpp
    ${DIR}/application_event.hpp
    ${DIR}/sdl_utility.hpp
    
    PARENT_SCOPE)
