/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#include "tools.h"

#include <QDir>

#include <cstdlib>

namespace tools {

void createRequiredFolders() {
  createFolder("Input_Data");
  createFolder("Results");
  createFolder("Results/Profiling");
  createFolder("Videos");
  createFolder("Network_State");
  createFolder("numSCAL_Networks");
}

void initialiseFolder(std::string path) {
  createFolder(path);
  cleanFolder(path);
}

void createFolder(std::string path) { QDir().mkdir(path.c_str()); }

void cleanFolder(std::string path) {
  QDir directory(path.c_str());
  QStringList files = directory.entryList(QStringList() << "*.*", QDir::Files);
  for (QString filename : files) directory.remove(filename);
}

void cleanVideosFolder() {
  QDir directory("Videos");
  QStringList pngFiles =
      directory.entryList(QStringList() << "*.png", QDir::Files);
  for (QString filename : pngFiles) directory.remove(filename);
}

void renderVideo(int fps) {
#if defined(_WIN32)
  // Windows
  std::string command =
      "ffmpeg\\ffmpeg -framerate " + std::to_string(fps) +
      " -i Videos\\IMG%7d.png -y  Videos\\video.mp4 > nul 2>&1 2>&1";
#elif defined(__unix__) || defined(__unix) || defined(unix) || \
    (defined(__APPLE__) && defined(__MACH__))
  // AIX, BSD, Cygwin, HP-UX, Linux, OSX, and Solaris
  std::string command =
      "./ffmpeg/ffmpeg -framerate " + std::to_string(fps) +
      " -i Videos\\IMG%7d.png -y  Videos/video.mp4 > /dev/null 2>&1";
#endif
  system(command.c_str());
}

}  // namespace tools
