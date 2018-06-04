#ifndef ITERATOR_H
#define ITERATOR_H

#include "network.h"

namespace PNM {

enum class filter{all, closed};

template <typename T, filter f>
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
        if(current < net->getTotalNodes() + net->getTotalPores())
            ++current;
        return *this;
    }
    T operator *() const{
        if(current < net->getTotalNodes()){ // node
            return net->getNode(current);
        }
        else if(current < net->getTotalNodes() + net->getTotalPores()){ //pore
           return net->getPore(current-net->getTotalNodes());
        }
    }

protected:
    network* net;
    size_t current;
};

template <typename T>
class networkIterator<T, filter::closed>
{
public:
    explicit networkIterator(network* net, size_t current) : net(net), current(current) {
        bool currentState(false);
        if(current < net->getTotalNodes()){ // node
            currentState=net->getNode(current)->getClosed();
        }
        else if(current < net->getTotalNodes()+net->getTotalPores()){ //pore
           currentState=net->getPore(current-net->getTotalNodes())->getClosed();
        }
        if(currentState)
           this->operator ++();
    }

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
                while(poreIndex<net->getTotalPores() && net->getPore(poreIndex)->getClosed()){
                    ++poreIndex; ++current;
                }
            }
        }
        else if (current < net->getTotalNodes() + net->getTotalPores()){ //pore
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
        else if(current < net->getTotalNodes() + net->getTotalPores()){ //pore
           return net->getPore(current-net->getTotalNodes());
        }
    }

protected:
    network* net;
    size_t current;
};


template <filter f>
class networkIterator<pore*, f>
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
        if(current<net->getTotalPores())
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
class networkIterator<pore*, filter::closed>
{
public:
    explicit networkIterator(network* net, size_t current) : net(net), current(current) {
        bool currentState(false);
        if(current < net->getTotalPores()){ // node
            currentState=net->getPore(current)->getClosed();
        }
        if(currentState){
            this->operator ++();
        }
    }

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

template <filter f>
class networkIterator<node*, f>
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
        if(current<net->getTotalNodes())
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

template <>
class networkIterator<node*, filter::closed>
{
public:
    explicit networkIterator(network* net, size_t current) : net(net), current(current) {
        bool currentState(false);
        if(current < net->getTotalNodes()){ // node
            currentState=net->getNode(current)->getClosed();
        }
        if(currentState){
            this->operator ++();
        }
    }

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

template <typename T, filter f>
class iteratorBuilder
{
public:
    networkIterator<T, f> buildBegin(network* net){
        return networkIterator<T, f>(net, 0);;
    }
    networkIterator<T, f> buildEnd(network* net){
        return networkIterator<T, f>(net, net->getTotalNodes()+net->getTotalPores());
    }
};

template <filter f>
class iteratorBuilder<pore*, f>
{
public:
    networkIterator<pore*, f> buildBegin(network* net){
        return networkIterator<pore*, f>(net, 0);;
    }
    networkIterator<pore*, f> buildEnd(network* net){
        return networkIterator<pore*, f>(net, net->getTotalPores());
    }
};

template <filter f>
class iteratorBuilder<node*, f>
{
public:
    networkIterator<node*, f> buildBegin(network* net){
        return networkIterator<node*, f>(net, 0);;
    }
    networkIterator<node*, f> buildEnd(network* net){
        return networkIterator<node*, f>(net, net->getTotalNodes());
    }
};


template <typename T, filter f = filter::closed>
class networkRange
{
public:
    explicit networkRange(network* net) : net(net) {}
    auto begin() -> networkIterator<T,f>{
        return builder.buildBegin(net);
    }
    auto end() -> networkIterator<T,f>{
        return builder.buildEnd(net);
    }

protected:
    network* net;
    iteratorBuilder<T,f> builder;
};

}


#endif // ITERATOR_H
