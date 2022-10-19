cmake_minimum_required(VERSION 3.23)
project(Practica1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Practica1
        lista.c
        lista.h
        main.c p0.c p0.h cabezeras.h p1.c p1.h headers.h animText.c animText.h cmake-build-debug/textAnim.c cmake-build-debug/textAnim.h textAnim.c textAnim.h)
