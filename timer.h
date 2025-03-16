//
// Created by pknadimp on 3/15/25.
//

#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <iostream>
#include <string>

#define TIME_FUNCTION(func, name) \
{ \
auto start = std::chrono::high_resolution_clock::now(); \
func; \
auto end = std::chrono::high_resolution_clock::now(); \
std::chrono::duration<double, std::milli> duration = end - start; \
std::cout << name << " took " << duration.count() << " ms" << std::endl; \
}

#endif //TIMER_H
