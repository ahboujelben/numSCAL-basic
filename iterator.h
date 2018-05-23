#ifndef ITERATOR_H
#define ITERATOR_H

#include "network.h"

namespace PNM {

template <typename T>
class networkIterator
{
public:
    explicit networkIterator(network* net, size_t current) : net(net), current(current) {}

    bool operator ==(const networkIterator& it) const{
        return this->current==it.current;
    }
    bool operator !=(const networkIterator& it) const{
        return !(*this==it);
    }
    networkIterator& operator ++(){
        if(current < net->getTotalNodes()){ // node
            ++current;
            if(current < net->getTotalNodes())
                while(current<net->getTotalNodes() && net->getNode(current)->getClosed())
                { ++current;}
            if(current == net->getTotalNodes()){
                auto poreIndex(0);
                while(net->getPore(poreIndex)->getClosed()){
                    ++poreIndex; ++current;
                }
            }
        }
        else{ //pore
            ++current;
            auto poreIndex = current-net->getTotalNodes();
            while(poreIndex<net->getTotalPores() && net->getPore(poreIndex)->getClosed()){
                ++poreIndex; ++current;
            }
        }
        return *this;
    }
    T operator *() const{
        if(current < net->getTotalNodes()){ // node
            return net->getNode(current);
        }
        else{ //pore
           return net->getPore(current-net->getTotalNodes());
        }
    }

protected:
    network* net;
    size_t current;
};

template <>
class networkIterator<pore*>
{
public:
    explicit networkIterator(network* net, size_t current) : net(net), current(current) {}

    bool operator ==(const networkIterator& it) const{
        return this->current==it.current;
    }
    bool operator !=(const networkIterator& it) const{
        return !(*this==it);
    }
    networkIterator& operator ++(){
        ++current;
        while(current<net->getTotalPores() && net->getPore(current)->getClosed())
            ++current;
        return *this;
    }
    pore* operator *() const{
        return net->getPore(current);
    }

protected:
    network* net;
    size_t current;
};


template <>
class networkIterator<node*>
{
public:
    explicit networkIterator(network* net, size_t current) : net(net), current(current) {}

    bool operator ==(const networkIterator& it) const{
        return this->current==it.current;
    }
    bool operator !=(const networkIterator& it) const{
        return !(*this==it);
    }
    networkIterator& operator ++(){
        ++current;
        while(current<net->getTotalNodes() && net->getNode(current)->getClosed())
            ++current;
        return *this;
    }
    node* operator *() const{
        return net->getNode(current);
    }

protected:
    network* net;
    size_t current;
};

template <typename T>
class iteratorBuilder
{
public:
    networkIterator<T> buildBegin(network* net){
        return networkIterator<T>(net, 0);;
    }
    networkIterator<T> buildEnd(network* net){
        return networkIterator<T>(net, net->getTotalNodes()+net->getTotalPores());
    }
};

template <>
class iteratorBuilder<pore*>
{
public:
    networkIterator<pore*> buildBegin(network* net){
        return networkIterator<pore*>(net, 0);;
    }
    networkIterator<pore*> buildEnd(network* net){
        return networkIterator<pore*>(net, net->getTotalPores());
    }
};

template <>
class iteratorBuilder<node*>
{
public:
    networkIterator<node*> buildBegin(network* net){
        return networkIterator<node*>(net, 0);;
    }
    networkIterator<node*> buildEnd(network* net){
        return networkIterator<node*>(net, net->getTotalNodes());
    }
};


template <typename T>
class networkRange
{
public:
    explicit networkRange(network* net) : net(net) {}
    auto begin() -> networkIterator<T>{
        return builder.buildBegin(net);
    }
    auto end() -> networkIterator<T>{
        return builder.buildEnd(net);
    }

protected:
    network* net;
    iteratorBuilder<T> builder;
};

}


#endif // ITERATOR_H
