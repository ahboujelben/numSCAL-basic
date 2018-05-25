/////////////////////////////////////////////////////////////////////////////
/// Name:        solver.cpp
/// Purpose:     methods related to solving the pressure field in the network
///              are defined here (uses Eigen library).
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "network.h"
#include "iterator.h"

//Eigen library
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseCholesky>

namespace PNM {

using namespace Eigen;

void network::solvePressures()
{
    SparseMatrix<double> conductivityMatrix(totalOpenedNodes,totalOpenedNodes);
    conductivityMatrix.reserve(VectorXi::Constant(totalOpenedNodes,maxConnectionNumber+3));
    VectorXd b=VectorXd::Zero(totalOpenedNodes);
    VectorXd pressures=VectorXd::Zero(totalOpenedNodes);

    int row=0;
    for(node* n : networkRange<node*>(this))
    {
        double conductivity(1e-200);
        for(element* e : n->getNeighboors())
        {
            pore* p = static_cast<pore*>(e);
            if(p->getActive())
            {
                if(p->getInlet())
                {
                    b(row)=-pressureIn*p->getConductivity();
                    conductivity-=p->getConductivity();
                }
                if(p->getOutlet())
                {
                    b(row)=-pressureOut*p->getConductivity();
                    conductivity-=p->getConductivity();
                }
                if(!p->getInlet() && !p->getOutlet())
                {
                    node* neighboor=p->getOtherNode(n);
                    conductivityMatrix.insert(row,neighboor->getRank())=p->getConductivity();
                    conductivity-=p->getConductivity();
                }
            }
        }
        conductivityMatrix.insert(row,n->getRank())=conductivity;
        row++;
    }
    conductivityMatrix.makeCompressed();

    if(solverChoice==1)
    {
        SimplicialLDLT<SparseMatrix<double> > solver;
        solver.compute(conductivityMatrix);
        pressures=solver.solve(b);
    }

    if(solverChoice==2)
    {
        ConjugateGradient<SparseMatrix<double>,Lower|Upper> solver;
        solver.setTolerance(1e-8);
        solver.setMaxIterations(1000);

        solver.compute(conductivityMatrix);
        pressures=solver.solve(b);
    }

    for(node* n : networkRange<node*>(this))
        n->setPressure(pressures[n->getRank()]);

}

void network::solvePressuresWithCapillaryPressures()
{
    SparseMatrix<double> conductivityMatrix(totalOpenedNodes,totalOpenedNodes);
    conductivityMatrix.reserve(VectorXi::Constant(totalOpenedNodes,maxConnectionNumber+3));
    VectorXd b=VectorXd::Zero(totalOpenedNodes);
    VectorXd pressures=VectorXd::Zero(totalOpenedNodes);

    int row=0;
    for(node* n : networkRange<node*>(this))
    {
        double conductivity(1e-200);
        for(element* e : n->getNeighboors())
        {
            pore* p = static_cast<pore*>(e);
            if(p->getActive())
            {
                if(p->getInlet())
                {
                    b(row) -= p->getVolume()/inletPoresVolume * flowRate;
                }
                if(p->getOutlet())
                {
                    conductivity-=p->getConductivity();
                }
                if(!p->getInlet() && !p->getOutlet())
                {
                    node* neighboor=p->getOtherNode(n);

                    conductivityMatrix.insert(row,neighboor->getRank())=p->getConductivity();
                    conductivity-=p->getConductivity();

                    //Capillary Pressure
                    double capillaryPressure=p->getCapillaryPressure();
                    if(neighboor==p->getNodeOut())
                       b(row)+=capillaryPressure*p->getConductivity();
                    if(neighboor==p->getNodeIn())
                       b(row)-=capillaryPressure*p->getConductivity();
                }
            }
        }
        conductivityMatrix.insert(row,n->getRank())=conductivity;
        row++;

    }
    conductivityMatrix.makeCompressed();

    if(solverChoice==1)
    {
        SimplicialLDLT<SparseMatrix<double> > solver;
        solver.compute(conductivityMatrix);
        pressures=solver.solve(b);
    }
    if(solverChoice==2)
    {
        ConjugateGradient<SparseMatrix<double>,Lower|Upper> solver;
        solver.setTolerance(1e-8);
        solver.setMaxIterations(1000);
        solver.compute(conductivityMatrix);
        pressures=solver.solve(b);
    }

    for(node* n : networkRange<node*>(this))
        n->setPressure(pressures[n->getRank()]);

}

double network::updateFlows()
{
    double outletFlow(0);
    for(pore* p : networkRange<pore*>(this))
    {
        p->setFlow(0);
        if(p->getActive())
        {
            if(p->getOutlet())
            {
                p->setFlow((p->getNodeOut()->getPressure()-pressureOut)*p->getConductivity());
                outletFlow+=p->getFlow();
            }
            if(p->getInlet())
            {
                p->setFlow((pressureIn-p->getNodeIn()->getPressure())*p->getConductivity());
            }
            if(!p->getInlet() && !p->getOutlet())
            {
                p->setFlow((p->getNodeOut()->getPressure()-p->getNodeIn()->getPressure())*p->getConductivity());
            }
        }
    }
    return outletFlow;
}

double network::updateFlowsWithCapillaryPressure()
{
    double outletFlow(0);
    for(pore* p : networkRange<pore*>(this))
    {
        p->setFlow(0);
        if(p->getActive())
        {
            if(p->getOutlet())
            {
                p->setFlow((p->getNodeOut()->getPressure())*p->getConductivity());
                outletFlow+=p->getFlow();
            }
            if(p->getInlet())
            {
                p->setFlow(p->getVolume()/inletPoresVolume * flowRate);
            }
            if(!p->getInlet() && !p->getOutlet())
            {
                p->setFlow((p->getNodeOut()->getPressure()-p->getNodeIn()->getPressure()-p->getCapillaryPressure())*p->getConductivity());
            }
        }
    }
    return outletFlow;
}

void network::calculatePermeabilityAndPorosity()
{
    ofstream file("Results/Network_Description/AbsolutePerm_Porosity.txt");
    flow=getOutletFlow();
    absolutePermeability=(flow*xEdgeLength)/(yEdgeLength*zEdgeLength*(pressureIn-pressureOut));
    porosity=totalElementsVolume/(xEdgeLength*yEdgeLength*zEdgeLength);
    file<<"Absolute permeability (mD):\t"<<absolutePermeability/0.987e-15<<endl<<"Porosity:\t"<<porosity<<endl;
}

void network::assignConductivities()
{
    for_each(networkRange<pore*>(this).begin(),networkRange<pore*>(this).end(),[this](pore* p){
        node* nodeIn=p->getNodeIn();
        node* nodeOut=p->getNodeOut();
        auto throatConductivityInverse(0.0),nodeInConductivityInverse(0.0),nodeOutConductivityInverse(0.0);

        throatConductivityInverse=1/(p->getShapeFactorConstant()*pow(p->getRadius(),4)/(16*p->getShapeFactor())/(p->getViscosity()*p->getLength()));

        if(nodeIn!=0)
            nodeInConductivityInverse=1/(nodeIn->getShapeFactorConstant()*pow(nodeIn->getRadius(),4)/(16*nodeIn->getShapeFactor())/(nodeIn->getViscosity()*p->getNodeInLength()));
        if(nodeOut!=0)
            nodeOutConductivityInverse=1/(nodeOut->getShapeFactorConstant()*pow(nodeOut->getRadius(),4)/(16*nodeOut->getShapeFactor())/(nodeOut->getViscosity()*p->getNodeOutLength()));

        p->setConductivity(1./(throatConductivityInverse+nodeInConductivityInverse+nodeOutConductivityInverse));
    });
}

}
