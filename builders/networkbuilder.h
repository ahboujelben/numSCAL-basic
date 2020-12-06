/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#ifndef NETWORKBUILDER_H
#define NETWORKBUILDER_H

#include <memory>
#include <string>

#include <QObject>

namespace PNM {

class networkModel;

class networkBuilder : public QObject {
  Q_OBJECT
 public:
  static std::shared_ptr<networkBuilder> createBuilder();
  std::shared_ptr<networkModel> build();
  virtual std::string getNotification() = 0;
  virtual int getProgress();
  std::shared_ptr<networkModel> getNetwork() const;

 signals:
  void notifyGUI();
  void finished();

 protected:
  networkBuilder(QObject *parent = nullptr) : QObject(parent) {}
  networkBuilder(const networkBuilder &) = delete;
  networkBuilder(networkBuilder &&) = delete;
  auto operator=(const networkBuilder &) -> networkBuilder & = delete;
  auto operator=(networkBuilder &&) -> networkBuilder & = delete;
  virtual void make() = 0;
  void initialise();
  void finalise();
  void signalProgress(int);

  std::shared_ptr<networkModel> network;
  int progress;
};

}  // namespace PNM

#endif  // NETWORKBUILDER_H
