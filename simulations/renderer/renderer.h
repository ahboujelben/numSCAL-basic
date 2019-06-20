/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef RENDERER_H
#define RENDERER_H

#include "simulations/simulation.h"

#include <memory>

namespace PNM {

class renderer : public simulation {
 public:
  renderer();
  ~renderer() override;
  renderer(const renderer &) = delete;
  renderer(renderer &&) = delete;
  auto operator=(const renderer &) -> renderer & = delete;
  auto operator=(renderer &&) -> renderer & = delete;

  virtual void run() override;
  virtual std::string getNotification() override;
  virtual int getProgress() override;

 private:
  void loadStateFiles();
  void processFrames();

  int currentFileIndex;
  int totalFiles;
};

}  // namespace PNM

#endif  // RENDERER_H
