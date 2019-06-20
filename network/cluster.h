/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef CLUSTER_H
#define CLUSTER_H

namespace PNM {

class cluster {
 public:
  cluster(int);
  cluster(const cluster &) = delete;
  cluster(cluster &&) = delete;
  auto operator=(const cluster &) -> cluster & = delete;
  auto operator=(cluster &&) -> cluster & = delete;

  int getId() const;
  void setId(int value);

  bool getInlet() const;
  void setInlet(bool value);

  bool getOutlet() const;
  void setOutlet(bool value);

  bool getSpanning() const;
  void setSpanning(bool value);

 private:
  int id;         // cluster id
  bool inlet;     // flag on whether the cluster is connected to the inlet
  bool outlet;    // flag on whether the cluster is connected to the outlet
  bool spanning;  // flag on whether the cluster is spanning
};

}  // namespace PNM

#endif  // CLUSTER_H
