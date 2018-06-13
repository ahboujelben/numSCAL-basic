/////////////////////////////////////////////////////////////////////////////
/// Name:        tools.h
/// Purpose:     utility functions are declared and defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef TOOLS_H
#define TOOLS_H

#include <cstdint>

namespace PNM {

void renderVideo();
void createRequiredFolders();
void cleanVideosFolder();
void cleanResultsFolder();
void cleanNetworkDescriptionFolder();
double getCPUTime();

template<typename T>
struct pointerHash {
    inline size_t operator()(const T* pointer) const {
        auto addr = reinterpret_cast<uintptr_t>(pointer);
#if SIZE_MAX < UINTPTR_MAX
        /* size_t is not large enough to hold the pointer’s memory address */
        addr %= SIZE_MAX; /* truncate the address so it is small enough to fit in a size_t */
#endif
        return addr;
    }
};

}

#endif // TOOLS_H
