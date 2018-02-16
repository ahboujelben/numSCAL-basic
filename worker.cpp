/////////////////////////////////////////////////////////////////////////////
/// Name:        worker.cpp
/// Purpose:     thread management functions are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "worker.h"

worker::worker(PNM::network *net, int j, QObject *parent) :
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
            cout<<"Running Simulation..."<<endl;
            n->runFluidInjectionSimulation();
            cout<<"End of Simulation."<<endl;
            n->setSimulationRunning(false);
            emit finished();
            break;
        }
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

