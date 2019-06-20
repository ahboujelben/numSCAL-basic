/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef ITERATOR2_H
#define ITERATOR2_H

#include "networkmodel.h"
#include "node.h"
#include "pore.h"

namespace PNM {

using networkPtr = std::shared_ptr<networkModel>;

template <typename T>
class pnmIterator {
 public:
  explicit pnmIterator(networkPtr net, int current)
      : net(net), current(current) {}

  bool operator==(const pnmIterator &it) const {
    return this->current == it.current;
  }
  bool operator!=(const pnmIterator &it) const { return !(*this == it); }
  pnmIterator &operator++() {
    if (current < net->totalNodes + net->totalPores) ++current;
    return *this;
  }
  T *operator*() const {
    if (current < net->totalNodes) {  // node
      return net->getNode(current);
    } else if (current < net->totalNodes + net->totalPores) {  // pore
      return net->getPore(current - net->totalNodes);
    }
    throw std::out_of_range("pnmIterator out of range.\n");
  }

 protected:
  networkPtr net;
  int current;
};

template <>
class pnmIterator<pore> {
 public:
  explicit pnmIterator(networkPtr net, int current)
      : net(net), current(current) {}

  bool operator==(const pnmIterator &it) const {
    return this->current == it.current;
  }
  bool operator!=(const pnmIterator &it) const { return !(*this == it); }
  pnmIterator &operator++() {
    if (current < net->totalPores) ++current;
    return *this;
  }
  pore *operator*() const { return net->getPore(current); }

 protected:
  networkPtr net;
  int current;
};

template <>
class pnmIterator<node> {
 public:
  explicit pnmIterator(networkPtr net, int current)
      : net(net), current(current) {}

  bool operator==(const pnmIterator &it) const {
    return this->current == it.current;
  }
  bool operator!=(const pnmIterator &it) const { return !(*this == it); }
  pnmIterator &operator++() {
    if (current < net->totalNodes) ++current;
    return *this;
  }
  node *operator*() const { return net->getNode(current); }

 protected:
  networkPtr net;
  int current;
};

template <typename T>
class pnmIteratorBuilder {
 public:
  pnmIterator<T> buildBegin(networkPtr net) {
    return pnmIterator<T>(net, 0);
    ;
  }
  pnmIterator<T> buildEnd(networkPtr net) {
    return pnmIterator<T>(net, net->totalNodes + net->totalPores);
  }
};

template <>
class pnmIteratorBuilder<pore> {
 public:
  pnmIterator<pore> buildBegin(networkPtr net) {
    return pnmIterator<pore>(net, 0);
    ;
  }
  pnmIterator<pore> buildEnd(networkPtr net) {
    return pnmIterator<pore>(net, net->totalPores);
  }
};

template <>
class pnmIteratorBuilder<node> {
 public:
  pnmIterator<node> buildBegin(networkPtr net) {
    return pnmIterator<node>(net, 0);
    ;
  }
  pnmIterator<node> buildEnd(networkPtr net) {
    return pnmIterator<node>(net, net->totalNodes);
  }
};

template <typename T>
class pnmRange {
 public:
  explicit pnmRange(networkPtr net) : net(net) {}
  auto begin() -> pnmIterator<T> { return builder.buildBegin(net); }
  auto end() -> pnmIterator<T> { return builder.buildEnd(net); }

 protected:
  networkPtr net;
  pnmIteratorBuilder<T> builder;
};

class pnmInlet {
 public:
  explicit pnmInlet(networkPtr net) : net(net) {}
  auto begin() { return net->inletPores.begin(); }
  auto end() { return net->inletPores.end(); }

 protected:
  networkPtr net;
};

class pnmOutlet {
 public:
  explicit pnmOutlet(networkPtr net) : net(net) {}
  auto begin() { return net->outletPores.begin(); }
  auto end() { return net->outletPores.end(); }

 protected:
  networkPtr net;
};

}  // namespace PNM

#endif  // ITERATOR2_H
