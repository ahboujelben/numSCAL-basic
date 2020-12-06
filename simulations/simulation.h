/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#ifndef SIMULATION_H
#define SIMULATION_H

#include <QObject>
#include <memory>
#include <string>

namespace PNM {

class networkModel;

class simulation : public QObject {
  Q_OBJECT
 public:
  virtual ~simulation() {}
  static std::shared_ptr<simulation> createSimulation();
  static std::shared_ptr<simulation> createRenderer();
  void setNetwork(const std::shared_ptr<networkModel> &value);
  void execute();
  virtual std::string getNotification() = 0;
  virtual int getProgress() = 0;
  virtual void interrupt();

 signals:
  void notifyGUI();
  void finished();

 protected slots:
  void updateGUI();

 protected:
  simulation(QObject *parent = nullptr);
  simulation(const simulation &) = delete;
  simulation(simulation &&) = delete;
  auto operator=(const simulation &) -> simulation & = delete;
  auto operator=(simulation &&) -> simulation & = delete;
  virtual void run() = 0;
  void initialise();
  void finalise();

  std::shared_ptr<networkModel> network;
  bool simulationInterrupted;
};

}  // namespace PNM

#endif  // SIMULATION_H
