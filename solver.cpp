/////////////////////////////////////////////////////////////////////////////
/// Name:        solver.cpp
/// Purpose:     methods related to solving the pressure field in the network
///              are defined here (uses Eigen library).
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "network.h"
//Eigen library
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseCholesky>

namespace PNM {

using namespace Eigen;

void network::solvePressuresForRegularModel()
{
    SparseMatrix<double> conductivityMatrix(Nx*Ny*Nz,Nx*Ny*Nz);
    conductivityMatrix.reserve(VectorXi::Constant(Nx*Ny*Nz,7));
    VectorXd b=VectorXd::Zero(Nx*Ny*Nz);
    VectorXd pressures=VectorXd::Zero(Nx*Ny*Nz);


    int row=0;
    for (int k = 0; k < Nz; ++k)
        for (int j = 0; j < Ny; ++j)
            for (int i = 0; i < Nx; ++i)
            {
                node *n,*nW,*nE,*nN,*nS,*nU,*nD;
                double c,cE,cW,cN,cS,cU,cD;
                n=getNode(i,j,k);
                nW=getNode(i-1,j,k);
                nE=getNode(i+1,j,k);
                nS=getNode(i,j-1,k);
                nN=getNode(i,j+1,k);
                nD=(Nz!=1? getNode(i,j,k-1) : 0);
                nU=(Nz!=1? getNode(i,j,k+1) : 0);
                cW=getPoreX(i,j,k)->getConductivity();
                cE=getPoreXout(i,j,k)->getConductivity();
                cS=getPoreY(i,j,k)->getConductivity();
                cN=getPoreYout(i,j,k)->getConductivity();
                cD=getPoreZ(i,j,k)->getConductivity();
                cU=getPoreZout(i,j,k)->getConductivity();
                c=-cW-cE-cS-cN-cD-cU;

                if(i==0) b(row)=-pressureIn*cW;
                if(i==Nx-1) b(row)=-pressureOut*cE;

                conductivityMatrix.insert(row,n->getRank())=c;
                if(i!=0)conductivityMatrix.insert(row,nW->getRank())=cW;
                if(i!=Nx-1)conductivityMatrix.insert(row,nE->getRank())=cE;
                if(j!=0)if(Ny!=2) conductivityMatrix.insert(row,nS->getRank())=cS;
                if(j!=Ny-1)conductivityMatrix.insert(row,nN->getRank())=cN;
                if(k!=0)if(Nz!=1)if(Nz!=2)conductivityMatrix.insert(row,nD->getRank())=cD;
                if(k!=Nz-1)if(Nz!=1)conductivityMatrix.insert(row,nU->getRank())=cU;
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
        ConjugateGradient<SparseMatrix<double>,Lower, IncompleteCholesky<double> > solver;
        solver.setTolerance(1e-8);
        solver.setMaxIterations(1000);
        solver.compute(conductivityMatrix);
        pressures=solver.solve(b);
        //cout<<solver.error()<<" "<<solver.iterations()<<endl;
    }

    for (int i = 0; i < Nx; ++i)
        for (int j = 0; j < Ny; ++j)
            for (int k = 0; k < Nz; ++k)
                getNode(i,j,k)->setPressure(pressures[getNode(i,j,k)->getRank()]);
}

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
        ConjugateGradient<SparseMatrix<double>,Lower, IncompleteCholesky<double> > solver;
        solver.setTolerance(1e-8);
        solver.setMaxIterations(1000);
        solver.compute(conductivityMatrix);
        pressures=solver.solve(b);
        //cout<<solver.error()<<" "<<solver.iterations()<<endl;
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
            vector<int> neighboors=n->getConnectedNodes();
            vector<int> connectedPores=n->getConnectedPores();
            double conductivity(0);
            for(unsigned j=0;j<neighboors.size();++j)
            {
                pore* p=getPore(connectedPores[j]-1);
                if(!p->getClosed())
                {
                    if(p->getInlet())
                    {
                        b(row)-=pressureIn*p->getConductivity();
                        conductivity-=p->getConductivity();
                    }
                    if(p->getOutlet())
                    {
                        b(row)-=pressureOut*p->getConductivity();
                        conductivity-=p->getConductivity();
                    }
                    if(!p->getInlet() && !p->getOutlet())
                    {
                        node* neighboor=getNode(neighboors[j]-1);
                        conductivityMatrix.insert(row,neighboor->getRank())=p->getConductivity();
                        conductivity-=p->getConductivity();
                    }

                    //Capillary Pressure

                    double capillaryPressure=p->getCapillaryPressure();

                    node* neighboor=getNode(neighboors[j]-1);
                    if(neighboor==p->getNodeOut())
                       b(row)+=capillaryPressure*p->getConductivity();
                    if(neighboor==p->getNodeIn())
                       b(row)-=capillaryPressure*p->getConductivity();
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
        ConjugateGradient<SparseMatrix<double>,Lower, IncompleteCholesky<double> > solver;
        solver.setTolerance(1e-8);
        solver.setMaxIterations(1000);
        solver.compute(conductivityMatrix);
        pressures=solver.solve(b);
        //cout<<solver.error()<<" "<<solver.iterations()<<endl;
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
                p->setFlow((p->getNodeOut()->getPressure()-pressureOut-p->getCapillaryPressure())*p->getConductivity());
                outletFlow+=p->getFlow();
            }
            if(p->getInlet())
            {
                p->setFlow((pressureIn-p->getNodeIn()->getPressure()-p->getCapillaryPressure())*p->getConductivity());
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
