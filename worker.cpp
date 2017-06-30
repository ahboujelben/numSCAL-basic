/////////////////////////////////////////////////////////////////////////////
/// Name:        worker.cpp
/// Purpose:     thread management functions are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
/////////////////////////////////////////////////////////////////////////////

#include "worker.h"
using namespace std;

worker::worker(network *net, int j, QObject *parent) :
    QObject(parent)
{
    n=net;
    job=j;
}

void worker::process()
{
    switch(job)
    {
        case 0:
        {
            //console mode
        }

        case 1:
        {
            n->setSimulationRunning(true);
            cout<<"Setting up Model..."<<endl;
            n->setupModel();
            cout<<"Model loaded."<<endl;
            n->setSimulationRunning(false);
            emit finished();
            break;
        }

        case 2:
        {
            n->setSimulationRunning(true);
            cout<<"Running Two Phase Flow Simulation..."<<endl;
            n->setSimulationType(1);
            n->runTwoPhaseFlowSimulation();
            cout<<"End of Two Phase Flow Simulation."<<endl;
            n->setSimulationRunning(false);
            emit finished();
            break;
        }
//        case 3:
//        {
//            n->setSimulationRunning(true);
//            cout<<"Running Depletion Simulation..."<<endl;
//            n->setSimulationType(2);
//            n->runDepletionSimulation();
//            cout<<"End of Depletion Simulation."<<endl;
//            n->setSimulationRunning(false);
//            emit finished();
//            break;
//        }
    }
}
int worker::getJob() const
{
    return job;
}

void worker::setJob(int value)
{
    job = value;
}

