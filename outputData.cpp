/////////////////////////////////////////////////////////////////////////////
/// Name:        outputData.cpp
/// Purpose:     methods related to printing output files for USS simulations
///              are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "network.h"

namespace PNM {

void network::initializeTwoPhaseOutputs()
{
    string path=".txt";

    string pathCap="Results/Capillary"+path;
    string pathSat="Results/Saturation"+path;
    string pathFract="Results/WaterCapillaryFraction"+path;
    string pathPress="Results/Pressure"+path;
    string pathClust="Results/OilClusters"+path;
    string pathRelPerm="Results/RelPerms"+path;
    string pathWatCut="Results/FractionalFlow"+path;

    ofstream outfileCap;
    ofstream outfileSat;
    ofstream outfileFract;
    ofstream outfilePress;
    ofstream outfileClust;
    ofstream outfileRelPerm;
    ofstream outfileWatCut;

    outfileCap.open(pathCap.c_str());
    outfileSat.open(pathSat.c_str());
    outfileFract.open(pathFract.c_str());
    outfilePress.open(pathPress.c_str());
    outfileClust.open(pathClust.c_str());
    outfileRelPerm.open(pathRelPerm.c_str());
    outfileWatCut.open(pathWatCut.c_str());

    double inletArea=0.0;
    for(pore* p : accessiblePores)
    {
        if(p->getInlet())
            inletArea+=pow(poreVolumeConstant,2-poreVolumeExponent)*pow(p->getRadius(),poreVolumeExponent)/(4*p->getShapeFactor())*pow(10,(6*poreVolumeExponent-12));
    }

    outfileCap<<"Water Visc"<<"\t"<<"Inj Rate"<<"\t"<<"Inlet Area"<<"\t"<<"OW IFT"<<endl;
    outfileCap<<waterViscosity<<"\t"<<flowRate<<"\t"<<inletArea<<"\t"<<OWSurfaceTension<<endl;
    outfileCap<<endl;
    double inletFlux=flowRate/inletArea;
    outfileCap<<"Inlet Flux: "<<inletFlux*86400<<"m/day"<<endl;
    outfileCap<<"Capillary Number: "<<waterViscosity*inletFlux/OWSurfaceTension<<endl;
    outfileCap<<"Network Volume: "<<totalElementsVolume<<endl;

    clusterWaterElements();
    outfileCap<<"Swi Spanning? "<<isWaterSpanning<<endl;

    double waterSat=0.0;
    waterSat=initialWaterSaturation=getWaterSaturation();

    double waterFract=0.0;
    if(networkSource==1)
    {
        for(int i=0;i<totalPores;++i)
        {
            pore* p=getPore(i);
            if(!p->getClosed() && p->getWaterFraction()>0.5)
                waterFract+=1.0/totalOpenedPores;
        }
    }
    else
    {
        for(int i=0;i<totalElements;++i)
        {
            element* e=getElement(i);
            if(!e->getClosed() && e->getWaterFraction()>0.5)
                waterFract+=1.0/totalOpenedElements;
        }
    }

    outfileSat<<"PVs_Injected"<<"\t"<<"Sw"<<"\t"<<"Recovery"<<endl;
    outfileSat<<0<<"\t"<<waterSat<<"\t"<<0<<endl;

    outfileFract<<"PVs_Injected"<<"\t"<<"Fw"<<endl;
    outfileFract<<0<<"\t"<<waterFract<<endl;

    outfilePress<<"PVs_Injected"<<"\t"<<"Pressure"<<endl;

    outfileClust<<"PVs_Injected"<<"\t"<<"Oil_Clusters"<<endl;

    clusterOilElements();
    outfileClust<<0<<"\t"<<oilClusters.size()<<endl;

    outfileRelPerm<<"Sw\tKw\tKo"<<endl;

    outfileWatCut<<"PVs_Injected"<<"\t"<<"Fw"<<"\t"<<"Fo"<<endl;

    outfileCap.close();
    outfileSat.close();
    outfileFract.close();
    outfilePress.close();
    outfileClust.close();
    outfileRelPerm.close();
    outfileWatCut.close();
}

void network::outputTwoPhaseData(double injectedPVs, int& outputCount, double waterSat)
{
    string path=".txt";
    string pathSat="Results/Saturation"+path;
    string pathFract="Results/WaterCapillaryFraction"+path;
    string pathPress="Results/Pressure"+path;
    string pathClust="Results/OilClusters"+path;
    string pathRelPerm="Results/RelPerms"+path;
    string pathWatCut="Results/FractionalFlow"+path;

    double oilRecovery, waterFract;


    oilRecovery=(waterSat-initialWaterSaturation)/(1.0-initialWaterSaturation);

    //Output Saturations
    ofstream outfileSat;
    outfileSat.open(pathSat.c_str(), ofstream::app);
    outfileSat<<injectedPVs<<"\t"<<waterSat<<"\t"<<oilRecovery<<endl;
    outfileSat.close();

    //Output Fractions
    waterFract=0.0;

    if(networkSource==1)
    {
        for(pore* p : accessiblePores)
        {
            if(p->getWaterFraction()>0.5)
                waterFract+=1.0/totalOpenedPores;
        }
    }
    else
    {
        for(int i=0;i<totalElements;++i)
        {
            element* e=getElement(i);
            if(!e->getClosed() && e->getWaterFraction()>0.5)
                waterFract+=1.0/totalOpenedElements;
        }
    }

    ofstream outfileFract;
    outfileFract.open(pathFract.c_str(), ofstream::app);
    outfileFract<<injectedPVs<<"\t"<<waterFract<<endl;
    outfileFract.close();

    //Output Pressure
    ofstream outfilePress;
    outfilePress.open(pathPress.c_str(), ofstream::app);
    outfilePress<<injectedPVs<<"\t"<<deltaP<<endl;
    outfilePress.close();

    //Output Oil Clusters
    clusterOilElements();
    ofstream outfileClust;
    outfileClust.open(pathClust.c_str(), ofstream::app);
    outfileClust<<injectedPVs<<"\t"<<oilClusters.size()<<endl;
    outfileClust.close();

    //Output Rel Perm Curves
    if(relativePermeabilitiesCalculation)
    {
        ofstream outfileRelPerm;
        outfileRelPerm.open(pathRelPerm.c_str(), ofstream::app);
        calculateRelativePermeabilitiesUSS();
        outfileRelPerm<<waterSat<<"\t"<<waterRelativePermeability<<"\t"<<oilRelativePermeability<<endl;
        outfileRelPerm.close();
    }

    //Output Fract Flow
    double fractionalOil(0),fractionalWater(0);
    for(pore* p : accessiblePores)
        if(p->getPhaseFlag()==phase::oil && p->getConductivity()!=1e-200 && p->getOutlet())
            fractionalOil+=abs(p->getFlow());
    fractionalOil/=flowRate;
    fractionalWater=abs(1-fractionalOil)<0.0001?0:abs(1-fractionalOil);
    ofstream outfileWatCut;
    outfileWatCut.open(pathWatCut.c_str(), ofstream::app);
    outfileWatCut<<injectedPVs<<"\t"<<fractionalWater<<"\t"<<fractionalOil<<endl;
    outfileWatCut.close();

    //Output Network Status files
    if(extractData)
    {
        //Output Movie Data
        string path="Results/Network_Status/phases_pores"+boost::lexical_cast<std::string>(1000000000+outputCount)+".txt";

        ofstream file1(path.c_str());
        for(pore* p : accessiblePores)
        {
             char phaseFlag=p->getPhaseFlag()==phase::oil?'o':'w';
             file1<<p->getId()<<" "<<phaseFlag<<" "<<p->getConcentration()<<endl;
        }

        if(networkSource!=1)
        {
            string path="Results/Network_Status/phases_nodes"+boost::lexical_cast<std::string>(1000000000+outputCount)+".txt";
            ofstream file2(path.c_str());
            for(node* n : accessibleNodes)
            {
                char phaseFlag=n->getPhaseFlag()==phase::oil?'o':'w';
                file2<<n->getId()<<" "<<phaseFlag<<" "<<n->getConcentration()<<endl;
            }
        }
        outputCount++;
    }
}

}
