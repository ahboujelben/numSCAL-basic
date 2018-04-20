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

namespace PNM {

using namespace Eigen;

void network::solvePressures()
{
    SparseMatrix<double> conductivityMatrix(totalOpenedNodes,totalOpenedNodes);
    conductivityMatrix.reserve(VectorXi::Constant(totalOpenedNodes,maxConnectionNumber+3));
    VectorXd b=VectorXd::Zero(totalOpenedNodes);
    VectorXd pressures=VectorXd::Zero(totalOpenedNodes);

    int row=0;
    for(int i=0;i<totalNodes;++i)
    {
        node* n=getNode(i);
        if(!n->getClosed())
        {
            vector<int>& neighboors=n->getConnectedNodes();
            vector<int>& connectedPores=n->getConnectedPores();
            double conductivity(0);
            for(unsigned j=0;j<neighboors.size();++j)
            {
                pore* p=getPore(connectedPores[j]-1);
                if(!p->getClosed())
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
                        node* neighboor=getNode(neighboors[j]-1);
                        conductivityMatrix.insert(row,neighboor->getRank())=p->getConductivity();
                        conductivity-=p->getConductivity();
                    }
                }
            }
            conductivityMatrix.insert(row,n->getRank())=conductivity;
            row++;
        }
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

    for(int i=0;i<totalNodes;++i)
    {
        node* n=getNode(i);
        if(!n->getClosed())
            n->setPressure(pressures[n->getRank()]);
    }
}

void network::solvePressuresWithCapillaryPressures()
{
    SparseMatrix<double> conductivityMatrix(totalOpenedNodes,totalOpenedNodes);
    conductivityMatrix.reserve(VectorXi::Constant(totalOpenedNodes,maxConnectionNumber+3));
    VectorXd b=VectorXd::Zero(totalOpenedNodes);
    VectorXd pressures=VectorXd::Zero(totalOpenedNodes);

    int row=0;
    for(int i=0;i<totalNodes;++i)
    {
        node* n=getNode(i);
        if(!n->getClosed())
        {
            vector<int>& neighboors=n->getConnectedNodes();
            vector<int>& connectedPores=n->getConnectedPores();
            double conductivity(0);
            for(unsigned j=0;j<neighboors.size();++j)
            {
                pore* p=getPore(connectedPores[j]-1);
                node* neighboor=getNode(neighboors[j]-1);
                if(!p->getClosed())
                {
                    if(p->getInlet() && p->getConductivity())
                    {
                        b(row) -= p->getVolume()/inletPoresVolume * flowRate;
                    }
                    if(p->getOutlet())
                    {
                        conductivity-=p->getConductivity();
                    }
                    if(!p->getInlet() && !p->getOutlet())
                    {
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

    for(int i=0;i<totalNodes;++i)
    {
        node* n=getNode(i);
        if(!n->getClosed())
            n->setPressure(pressures[n->getRank()]);
    }
}

double network::updateFlows()
{
    double outletFlow(0);
    for(pore* p : accessiblePores)
    {
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
    for(pore* p : accessiblePores)
    {
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
    cout<<"Absolute permeability (mD): "<<absolutePermeability/0.987e-15<<endl;
    cout<<"Porosity: "<<porosity<<endl;
    file<<"Absolute permeability (mD):\t"<<absolutePermeability/0.987e-15<<endl<<"Porosity:\t"<<porosity<<endl;
}

void network::setConstantFlowRateAker()
{
    assignViscositiesWithMixedFluids();
    assignConductivities();

    double Q1(0),Q2(0),A,B;

    pressureIn=1;
    pressureOut=0;
    solvePressures();
    updateFlows();
    Q1=getOutletFlow();

    pressureIn=2;
    pressureOut=0;
    solvePressures();
    updateFlows();
    Q2=getOutletFlow();

    B=(Q1-Q2*1/2)/(1-1/2);
    A=(Q1-B);

    deltaP=flowRate/A-B/A;

    pressureIn=deltaP;
    pressureOut=0;
    solvePressures();
    updateFlows();
}

}
