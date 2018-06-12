/////////////////////////////////////////////////////////////////////////////
/// Name:        outputData.cpp
/// Purpose:     methods related to printing output files for USS simulations
///              are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "network.h"
#include "iterator.h"

#include <fstream>

#include <boost/lexical_cast.hpp>

namespace PNM {

using namespace std;

void network::initializeTwoPhaseOutputs()
{
    string pathCap="Results/Capillary.txt";
    string pathSat="Results/Saturation.txt";
    string pathPress="Results/Pressure.txt";

    ofstream outfileCap;
    ofstream outfileSat;
    ofstream outfilePress;

    outfileCap.open(pathCap.c_str());
    outfileSat.open(pathSat.c_str());
    outfilePress.open(pathPress.c_str());

    double inletArea=0.0;
    for(pore* p : inletPores){
        inletArea+=pow(poreVolumeConstant,2-poreVolumeExponent)*pow(p->getRadius(),poreVolumeExponent)/(4*p->getShapeFactor())*pow(10,(6*poreVolumeExponent-12));
    }
    outfileCap<<"Water Visc"<<"\t"<<"Inj Rate"<<"\t"<<"Inlet Area"<<"\t"<<"OW IFT"<<endl;
    outfileCap<<waterViscosity<<"\t"<<flowRate<<"\t"<<inletArea<<"\t"<<OWSurfaceTension<<endl;
    outfileCap<<endl;
    double inletFlux=flowRate/inletArea;
    outfileCap<<"Inlet Flux: "<<inletFlux*86400<<"m/day"<<endl;
    outfileCap<<"Capillary Number: "<<waterViscosity*inletFlux/OWSurfaceTension<<endl;
    outfileCap<<"Network Volume: "<<totalNetworkVolume<<endl;

    double waterSat=0.0;
    waterSat=initialWaterSaturation=getWaterSaturation();
    outfileSat<<"PVs_Injected"<<"\t"<<"Sw"<<"\t"<<"Recovery"<<endl;
    outfileSat<<0<<"\t"<<waterSat<<"\t"<<0<<endl;

    outfilePress<<"PVs_Injected"<<"\t"<<"Pressure"<<endl;

    outfileCap.close();
    outfileSat.close();
    outfilePress.close();
}

void network::outputTwoPhaseData(double injectedPVs, int& outputCount, double waterSat)
{
    string pathSat="Results/Saturation.txt";
    string pathPress="Results/Pressure.txt";

    //Output Saturations
    double oilRecovery=(waterSat-initialWaterSaturation)/(1.0-initialWaterSaturation);
    ofstream outfileSat;
    outfileSat.open(pathSat.c_str(), ofstream::app);
    outfileSat<<injectedPVs<<"\t"<<waterSat<<"\t"<<oilRecovery<<endl;
    outfileSat.close();

    //Output Pressure
    ofstream outfilePress;
    outfilePress.open(pathPress.c_str(), ofstream::app);
    outfilePress<<injectedPVs<<"\t"<<deltaP<<endl;
    outfilePress.close();

    //Output Network Status files
    if(extractData)
    {
        //Output Movie Data
        string path="Results/Network_Status/phases_pores"+boost::lexical_cast<std::string>(1000000000+outputCount)+".txt";

        ofstream file1(path.c_str());
        for(pore* p : networkRange<pore*>(this))
        {
             char phaseFlag=p->getPhaseFlag()==phase::oil?'o':'w';
             file1<<p->getId()<<" "<<phaseFlag<<" "<<p->getConcentration()<<endl;
        }

        if(networkSource!=1)
        {
            string path="Results/Network_Status/phases_nodes"+boost::lexical_cast<std::string>(1000000000+outputCount)+".txt";
            ofstream file2(path.c_str());
            for(node* n : networkRange<node*>(this))
            {
                char phaseFlag=n->getPhaseFlag()==phase::oil?'o':'w';
                file2<<n->getId()<<" "<<phaseFlag<<" "<<n->getConcentration()<<endl;
            }
        }
        outputCount++;
    }
}

}
