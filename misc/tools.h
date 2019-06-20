/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef TOOLS_H
#define TOOLS_H

#include <string>

namespace tools {
void createRequiredFolders();
void initialiseFolder(std::string);
void createFolder(std::string);
void cleanFolder(std::string);
void cleanVideosFolder();
void renderVideo(int fps = 25);
}  // namespace tools

#endif  // TOOLS_H
