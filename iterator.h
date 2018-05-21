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
        if(std::is_same<T, pore*>::value){
            ++current;
            while(current<net->getTotalPores() && net->getPore(current)->getClosed())
                ++current;
        }
        else if (std::is_same<T, node*>::value){
            ++current;
            while(current<net->getTotalNodes() && net->getNode(current)->getClosed())
                ++current;
        }
        else if (std::is_same<T, element*>::value){
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
        }
        return *this;
    }
    element* operator *() const{
        if(std::is_same<T, pore*>::value)
            return net->getPore(current);
        else if (std::is_same<T, node*>::value)
            return net->getNode(current);
        else if (std::is_same<T, element*>::value){
            if(current < net->getTotalNodes()){ // node
                return net->getNode(current);
            }
            else{ //pore
               return net->getPore(current-net->getTotalNodes());
            }
        }
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
        if(std::is_same<T, pore*>::value)
            return networkIterator<T>(net, net->getTotalPores());
        else if (std::is_same<T, node*>::value)
            return networkIterator<T>(net, net->getTotalNodes());
        else if (std::is_same<T, element*>::value)
            return networkIterator<T>(net, net->getTotalNodes()+net->getTotalPores());
        else
            throw "Type not supported by networkRange.";
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
