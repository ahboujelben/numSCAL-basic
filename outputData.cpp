/////////////////////////////////////////////////////////////////////////////
/// Name:        outputData.cpp
/// Purpose:     methods related to printing output files for USS simulations
///              are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
/////////////////////////////////////////////////////////////////////////////

#include "network.h"

using namespace std;

void network::initializeTwoPhaseOutputs()
{
//Call after Aker flow rate calculation...
    string path=".txt";/*"(Sec LS="+boost::lexical_cast<std::string>(surfactantInjection)+",BT LS="+boost::lexical_cast<std::string>(surfactantInjectionAfterBreakthrough)
            +",Ter LS="+boost::lexical_cast<std::string>(surfactantInjectionAfterHST)+",Sec Poly="+boost::lexical_cast<std::string>(polymerInjection)
    +",BT Poly="+boost::lexical_cast<std::string>(polymerInjectionAfterBreakthrough)
    +",Ter Poly="+boost::lexical_cast<std::string>(polymerInjectionAfterHST)+",M="+boost::lexical_cast<std::string>(oilViscosity/waterViscosity)
                +",Rate="+boost::lexical_cast<std::string>(flowRate*1e13)+",Wett="+boost::lexical_cast<std::string>(wettingTypeFlag)+").txt";*/


    string pathCap="Results/Capillary"+path;
    string pathSat="Results/Saturation"+path;
    string pathFract="Results/Fraction"+path;
    string pathPress="Results/Pressure"+path;
    string pathRP="Results/RelPerms"+path;
    string pathROcc="Results/PoreSizeOcc"+path;
    string pathXOcc="Results/PositionOcc"+path;
    string pathAvgConc="Results/AvgTracer"+path;
    string pathClust="Results/OilClusters"+path;
    //string pathWatCut="Results/FractFlow"+path;
    string pathPc="Results/CapPressure"+path;

    ofstream outfileCap;
    ofstream outfileSat;
    ofstream outfileFract;
    ofstream outfilePress;
    ofstream outfileRP;
    ofstream outfileROcc;
    ofstream outfileXOcc;
    ofstream outfileAvgConc;
    ofstream outfileClust;
    //ofstream outfileWatCut;
    ofstream outfilePc;

    outfileCap.open(pathCap.c_str());
    outfileSat.open(pathSat.c_str());
    outfileFract.open(pathFract.c_str());
    outfilePress.open(pathPress.c_str());
    outfileRP.open(pathRP.c_str());
    outfileROcc.open(pathROcc.c_str());
    outfileXOcc.open(pathXOcc.c_str());
    outfileAvgConc.open(pathAvgConc.c_str());
    outfileClust.open(pathClust.c_str());
    //outfileWatCut.open(pathWatCut.c_str());
    outfilePc.open(pathPc.c_str());

//---------------------------------------------------------

    double inletArea=0.0;
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed() && p->getInlet())
            inletArea+=pow(poreVolumeConstant,2-poreVolumeExponent)*pow(p->getRadius(),poreVolumeExponent)/(4*p->getShapeFactor())*pow(10,(6*poreVolumeExponent-12));
    }

    outfileCap<<"Water Visc"<<"\t"<<"Inj Rate"<<"\t"<<"Inlet Area"<<"\t"<<"OW IFT"<<endl;
    outfileCap<<waterViscosity<<"\t"<<flowRate<<"\t"<<inletArea<<"\t"<<OWSurfaceTension<<endl;
    outfileCap<<endl;
    double inletFlux=flowRate/inletArea;
    outfileCap<<"Inlet Flux: "<<inletFlux*86400<<"m/day"<<endl;
    outfileCap<<"Capillary Number: "<<waterViscosity*inletFlux/OWSurfaceTension<<endl;
    outfileCap<<"Network Volume: "<<totalElementsVolume<<endl;
    if(networkSource==1)
        clusterWaterPores();
    else
        clusterWaterElements();
    outfileCap<<"Swi Spanning? "<<isWaterSpanning<<endl;

//---------------------------------------------------------

    double waterSat=0.0;
    if(networkSource==1)
        waterSat=getWaterSaturation();
    else
        waterSat=getWaterSaturationPT();

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

    outfileSat<<"PVs Injected"<<"\t"<<"Sw"<<"\t\t"<<"PVs Injected"<<"\t"<<"Recovery"<<endl;
    outfileSat<<0<<"\t"<<waterSat<<"\t\t"<<0<<"\t"<<0<<endl;

    outfilePc<<"Sw"<<"\t"<<"Min Pc"<<"\t"<<"Current Pc"<<"\t\t"<<"PVs Injected"<<"\t"<<"Min Pc"<<"\t"<<"Current Pc"<<endl;
    outfilePc<<0<<"\t"<<0<<"\t"<<0<<"\t\t"<<0<<"\t"<<0<<"\t"<<0<<endl;

    outfileFract<<"PVs Injected"<<"\t"<<"Fw"<<endl;
    outfileFract<<0<<"\t"<<waterFract<<endl;

    outfileRP<<"Sw"<<"\t"<<"krw"<<"\t"<<"kro"<<endl;

    outfileClust<<"PVs Injected"<<"\t"<<"Oil Clusters"<<endl;
    if(networkSource==1)
        clusterOilPores();
    else
        clusterOilElements();
    outfileClust<<0<<"\t"<<oilClusters.size()<<endl;

    //outfileWatCut<<"PVs Injected"<<"\t"<<"Fract Water Flow"<<endl;
    //outfileWatCut<<0<<"\t"<<getWaterPartialFlow()/flowRate<<endl;

    outfilePress<<"PVs Injected"<<"\t"<<"Pressure"<<endl;
    outfilePress<<0<<"\t"<<deltaP<<endl;

//---------------------------------------------------------

    int nBins=100;
    int nCurrentBin=0;

    int numOil[nBins];
    int numOilMod[nBins];
    int numWater[nBins];
    int numTrapped[nBins];

    for (int n=1;n<=nBins;n++)
    {
        numOil[n-1]=0;
        numOilMod[n-1]=0;
        numWater[n-1]=0;
        numTrapped[n-1]=0;
    }

    double delR=(maxRadius-minRadius)/nBins;
    double lowerLimit=maxRadius-delR;
    double upperLimit=maxRadius;

    while(lowerLimit>=minRadius-delR/100.0)
    {
        nCurrentBin++;

        for(int i=0;i<totalPores;++i)
        {
            pore* p=getPore(i);

            if(p->getRadius()>lowerLimit && p->getRadius()<=upperLimit)
            {
                if(!p->getClosed())
                {
                    if(p->getWaterFraction()>0.5)
                    {
                        numWater[nCurrentBin-1]++;
                    }
                    else
                    {
                        if(p->getOilTrapped())
                        {
                            numTrapped[nCurrentBin-1]++;
                        }
                        else
                        {
                            if(p->getTheta()==wettabiltyThetaBackup[p])
                            {
                                numOil[nCurrentBin-1]++;
                            }
                            else
                            {
                                numOilMod[nCurrentBin-1]++;
                            }
                        }
                    }
                }
            }
        }

        upperLimit=lowerLimit;
        lowerLimit-=delR;
    }

    outfileROcc<<0<<"\t"<<"\t"<<"W-F"<<"\t"<<"O-F (Mod)"<<"\t"<<"O-F (Init)"<<"\t"<<"O-F (Trapped)"<<endl;

    for (int n=1;n<=nBins;n++)
        outfileROcc<<"\t"<<(minRadius+double(n)*delR)<<"\t"<<numWater[nBins-n]<<"\t"<<numOilMod[nBins-n]<<"\t"<<numOil[nBins-n]<<"\t"<<numTrapped[nBins-n]<<endl;

    outfileROcc<<endl;

//---------------------------------------------------------

    if (networkSource!=3)
    {
        int posX=0;

        int numberOil[Nx];
        int numberOilMod[Nx];
        int numberWater[Nx];
        int numberTrapped[Nx];
        double totalConc[Nx];
        int numberPores[Nx];

        for (int n=1;n<=Nx;n++)
        {
            numberOil[n-1]=0;
            numberOilMod[n-1]=0;
            numberWater[n-1]=0;
            numberTrapped[n-1]=0;
            totalConc[n-1]=0.0;
            numberPores[n-1]=0;
        }

        while(posX<Nx)
        {
            posX++;

            for(int i=0;i<totalPores;++i)
            {
                pore* p=getPore(i);

                if(!p->getClosed() && !p->getOutlet() && p->getWaterFraction()>2*precisionFactor)
                {
                    if(p->getNodeIn()->getIndexX()==posX-1)
                    {
                        totalConc[posX-1]+=p->getConcentration();
                        numberPores[posX-1]++;
                    }
                }

                if(!p->getClosed() && !p->getOutlet())
                {
                    if(p->getNodeIn()->getIndexX()==posX-1)
                    {
                        if(p->getWaterFraction()>0.5)
                        {
                            numberWater[posX-1]++;
                        }
                        else
                        {
                            if(p->getOilTrapped())
                            {
                                numberTrapped[posX-1]++;
                            }
                            else
                            {
                                if(p->getTheta()==wettabiltyThetaBackup[p])
                                {
                                    numberOil[posX-1]++;
                                }
                                else
                                {
                                    numberOilMod[posX-1]++;
                                }
                            }
                        }
                    }
                }
            }
        }

        outfileXOcc<<0<<"\t"<<"\t"<<"W-F"<<"\t"<<"O-F (Mod)"<<"\t"<<"O-F (Init)"<<"\t"<<"O-F (Trapped)"<<endl;
        outfileAvgConc<<0<<endl;

        for (int n=1;n<=Nx;n++)
        {
            outfileXOcc<<"\t"<<n<<"\t"<<numberWater[n-1]<<"\t"<<numberOilMod[n-1]<<"\t"<<numberOil[n-1]<<"\t"<<numberTrapped[n-1]<<endl;

            if(numberPores[n-1]>0)
                outfileAvgConc<<"\t"<<n<<"\t"<<totalConc[n-1]/numberPores[n-1]<<endl;
            else
                outfileAvgConc<<"\t"<<n<<"\t"<<totalConc[n-1]<<endl;
        }

        outfileXOcc<<endl;
        outfileAvgConc<<endl;
    }
    else
    {
        int numBins=int(cbrt(totalOpenedNodes))+1;
        int numCurrentBin=0;

        int numberOil[numBins];
        int numberOilMod[numBins];
        int numberWater[numBins];
        int numberTrapped[numBins];
        double totalConc[numBins];
        int numberPores[numBins];

        for (int n=1;n<=numBins;n++)
        {
            numberOil[n-1]=0;
            numberOilMod[n-1]=0;
            numberWater[n-1]=0;
            numberTrapped[n-1]=0;
            totalConc[n-1]=0.0;
            numberPores[n-1]=0;
        }

        double delX=xEdgeLength/numBins;
        double lowerLimit=xEdgeLength-delX;
        double upperLimit=xEdgeLength;

        while(lowerLimit>=-delX/100.0)
        {
            numCurrentBin++;

            for(int i=0;i<totalPores;++i)
            {
                pore* p=getPore(i);

                if(!p->getClosed() && !p->getOutlet() && p->getWaterFraction()>2*precisionFactor)
                {
                    if(p->getNodeIn()->getXCoordinate()>lowerLimit && p->getNodeIn()->getXCoordinate()<=upperLimit)
                    {
                        totalConc[numCurrentBin-1]+=p->getConcentration();
                        numberPores[numCurrentBin-1]++;
                    }
                }

                if(!p->getClosed() && !p->getOutlet())
                {
                    if(p->getNodeIn()->getXCoordinate()>lowerLimit && p->getNodeIn()->getXCoordinate()<=upperLimit)
                    {
                        if(p->getWaterFraction()>0.5)
                        {
                            numberWater[numCurrentBin-1]++;
                        }
                        else
                        {
                            if(p->getOilTrapped())
                            {
                                numberTrapped[numCurrentBin-1]++;
                            }
                            else
                            {
                                if(p->getTheta()==wettabiltyThetaBackup[p])
                                {
                                    numberOil[numCurrentBin-1]++;
                                }
                                else
                                {
                                    numberOilMod[numCurrentBin-1]++;
                                }
                            }
                        }
                    }
                }
            }

            upperLimit=lowerLimit;
            lowerLimit-=delX;
        }

        outfileXOcc<<0<<"\t"<<"\t"<<"W-F"<<"\t"<<"O-F (Mod)"<<"\t"<<"O-F (Init)"<<"\t"<<"O-F (Trapped)"<<endl;
        outfileAvgConc<<0<<endl;

        for (int n=1;n<=numBins;n++)
        {
            outfileXOcc<<"\t"<<double(n)*delX<<"\t"<<numberWater[numBins-n]<<"\t"<<numberOilMod[numBins-n]<<"\t"<<numberOil[numBins-n]<<"\t"<<numberTrapped[numBins-n]<<endl;

            if(numberPores[numBins-n]>0)
                outfileAvgConc<<"\t"<<double(n)*delX<<"\t"<<totalConc[numBins-n]/numberPores[numBins-n]<<endl;
            else
                outfileAvgConc<<"\t"<<double(n)*delX<<"\t"<<totalConc[numBins-n]<<endl;
        }

        outfileXOcc<<endl;
        outfileAvgConc<<endl;
    }

//---------------------------------------------------------

    outfileCap.close();
    outfileSat.close();
    outfileFract.close();
    outfilePress.close();
    //outfileRP.close();
    outfileROcc.close();
    outfileXOcc.close();
    outfileAvgConc.close();
    outfileClust.close();
    //outfileWatCut.close();
    outfilePc.close();
}

void network::outputTwoPhaseData(double injectedPVs, double waterSatThreshold, double injectThreshold, double outputFactor,
                                    double &oldWaterSat, double &oldWaterSat_2, double &outputPVs, double &outputPVs_2,
                                        int &outputCount, double capillaryPressure, double capillaryPressure2)
{
//Call after time incremented, but before closed pores are released...

    string path=".txt";/*"(Sec LS="+boost::lexical_cast<std::string>(surfactantInjection)+",BT LS="+boost::lexical_cast<std::string>(surfactantInjectionAfterBreakthrough)
                    +",Ter LS="+boost::lexical_cast<std::string>(surfactantInjectionAfterHST)+",Sec Poly="+boost::lexical_cast<std::string>(polymerInjection)
            +",BT Poly="+boost::lexical_cast<std::string>(polymerInjectionAfterBreakthrough)
            +",Ter Poly="+boost::lexical_cast<std::string>(polymerInjectionAfterHST)+",M="+boost::lexical_cast<std::string>(oilViscosity/waterViscosity)
                        +",Rate="+boost::lexical_cast<std::string>(flowRate*1e13)+",Wett="+boost::lexical_cast<std::string>(wettingTypeFlag)+").txt";*/

    string pathSat="Results/Saturation"+path;
    string pathFract="Results/Fraction"+path;
    string pathPress="Results/Pressure"+path;
    string pathRP="Results/RelPerms"+path;
    string pathROcc="Results/PoreSizeOcc"+path;
    string pathXOcc="Results/PositionOcc"+path;
    string pathAvgConc="Results/AvgTracer"+path;
    string pathClust="Results/OilClusters"+path;
    //string pathWatCut="Results/FractFlow"+path;
    string pathPc="Results/CapPressure"+path;

//---------------------------------------------------------

    double waterSat, oilRecovery, waterFract;

    if(networkSource==1)
        waterSat=getWaterSaturation();
    else
        waterSat=getWaterSaturationPT();

    oilRecovery=(waterSat-initialWaterSaturation)/(1.0-initialWaterSaturation);

    if (waterSat-oldWaterSat>waterSatThreshold || outputPVs>injectThreshold)
    {
        oldWaterSat=waterSat;
        outputPVs=0.0;

        //Output Fractional Flows
//        outfileWatCut<<injectedPVs<<"\t"<<getWaterPartialFlow()/flowRate<<endl;

        //Output Saturations
        ofstream outfileSat;
        outfileSat.open(pathSat.c_str(), ofstream::app);
        outfileSat<<injectedPVs<<"\t"<<waterSat<<"\t\t"<<injectedPVs<<"\t"<<oilRecovery<<endl;
        outfileSat.close();

        //Output Fractions
        waterFract=0.0;

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
        if(networkSource==1)
            clusterOilPores();
        else
            clusterOilElements();
        ofstream outfileClust;
        outfileClust.open(pathClust.c_str(), ofstream::app);
        outfileClust<<injectedPVs<<"\t"<<oilClusters.size()<<endl;
        outfileClust.close();

        if(extractData)
        {
            //Output Movie Data
            string path="Results/Network_Status/phases_pores"+boost::lexical_cast<std::string>(1000000000+outputCount)+".txt";

            ofstream file1(path.c_str());
            for(int i=0;i<totalPores;++i)
            {
                pore* p=getPore(i);
                if(!p->getClosed()  )
                {
                    file1<<p->getId()<<" "<<p->getPhaseFlag()<<" "<<p->getConcentration()<<" "<<p->getPolymerConcentration()<<endl;
                }
            }

            if(networkSource==2 || networkSource==3)
            {
                string path="Results/Network_Status/phases_nodes"+boost::lexical_cast<std::string>(1000000000+outputCount)+".txt";
                ofstream file2(path.c_str());
                for(int i=0;i<totalNodes;++i)
                {
                    node* n=getNode(i);
                    if(!n->getClosed()  )
                    {
                        file2<<n->getId()<<" "<<n->getPhaseFlag()<<" "<<n->getConcentration()<<" "<<n->getPolymerConcentration()<<endl;
                    }
                }
            }

            outputCount++;
        }
    }

//---------------------------------------------------------

    if (waterSat-oldWaterSat_2>waterSatThreshold*outputFactor || outputPVs_2>injectThreshold*outputFactor)
    {
        oldWaterSat_2=waterSat;
        outputPVs_2=0.0;

//        //Output Pseudo-Pc Curve
//        double minCapPress=1.0e20;
//        double capPress=minCapPress;

//        for(int i=0;i<totalPores;++i)
//        {
//            pore* p=getPore(i);
//            if(!p->getClosed() && p->getWaterFraction()>2*precisionFactor && !p->getInitialWater())
//            {
//                capPress=2*OWSurfaceTension*cos(p->getTheta())/p->getRadius();

//                if(capPress<minCapPress)
//                {
//                    minCapPress=capPress;
//                }
//            }
//        }

        ofstream outfilePc;
        outfilePc.open(pathPc.c_str(), ofstream::app);
        outfilePc<<waterSat<<"\t"<<capillaryPressure<<"\t"<<capillaryPressure2<<"\t\t"<<injectedPVs<<"\t"<<capillaryPressure<<"\t"<<capillaryPressure2<<endl;
        outfilePc.close();

        ofstream outfileRP;
        outfileRP.open(pathRP.c_str(), ofstream::app);
        //Output Rel Perms
        if(relativePermeabilitiesCalculation)
        {
            calculateRelativePermeabilitiesOW();//pseudo-SS rel perm calculation
        }

        outfileRP<<waterSat<<"\t"<<waterRelativePermeability<<"\t"<<oilRelativePermeability<<endl;
        outfileRP.close();

        //Output Pore Size Occupancy
        int nBins=100;
        int nCurrentBin=0;

        int numOil[nBins];
        int numOilMod[nBins];
        int numWater[nBins];
        int numTrapped[nBins];

        for (int n=1;n<=nBins;n++)
        {
            numOil[n-1]=0;
            numOilMod[n-1]=0;
            numWater[n-1]=0;
            numTrapped[n-1]=0;
        }

        double delR=(maxRadius-minRadius)/nBins;
        double lowerLimit=maxRadius-delR;
        double upperLimit=maxRadius;

        while(lowerLimit>=minRadius-delR/100.0)
        {
            nCurrentBin++;

            for(int i=0;i<totalPores;++i)
            {
                pore* p=getPore(i);

                if(p->getRadius()>lowerLimit && p->getRadius()<=upperLimit)
                {
                    if(!p->getClosed())
                    {
                        if(p->getWaterFraction()>0.5)
                        {
                            numWater[nCurrentBin-1]++;
                        }
                        else
                        {
                            if(p->getOilTrapped())
                            {
                                numTrapped[nCurrentBin-1]++;
                            }
                            else
                            {
                                if(p->getTheta()==wettabiltyThetaBackup[p])
                                {
                                    numOil[nCurrentBin-1]++;
                                }
                                else
                                {
                                    numOilMod[nCurrentBin-1]++;
                                }
                            }
                        }
                    }
                }
            }

            upperLimit=lowerLimit;
            lowerLimit-=delR;
        }

        ofstream outfileROcc;
        outfileROcc.open(pathROcc.c_str(), ofstream::app);
        outfileROcc<<injectedPVs<<"\t"<<"\t"<<"W-F"<<"\t"<<"O-F (Mod)"<<"\t"<<"O-F (Init)"<<"\t"<<"O-F (Trapped)"<<endl;

        for (int n=1;n<=nBins;n++)
            outfileROcc<<"\t"<<(minRadius+double(n)*delR)<<"\t"<<numWater[nBins-n]<<"\t"<<numOilMod[nBins-n]<<"\t"<<numOil[nBins-n]<<"\t"<<numTrapped[nBins-n]<<endl;

        outfileROcc<<endl;
        outfileROcc.close();

        //Output Position Occupancy & Average Tracer Conc
        if (networkSource!=3)
        {
            int posX=0;

            int numberOil[Nx];
            int numberOilMod[Nx];
            int numberWater[Nx];
            int numberTrapped[Nx];
            double totalConc[Nx];
            int numberPores[Nx];

            for (int n=1;n<=Nx;n++)
            {
                numberOil[n-1]=0;
                numberOilMod[n-1]=0;
                numberWater[n-1]=0;
                numberTrapped[n-1]=0;
                totalConc[n-1]=0.0;
                numberPores[n-1]=0;
            }

            while(posX<Nx)
            {
                posX++;

                for(int i=0;i<totalPores;++i)
                {
                    pore* p=getPore(i);

                    if(!p->getClosed() && !p->getOutlet() && p->getWaterFraction()>2*precisionFactor)
                    {
                        if(p->getNodeIn()->getIndexX()==posX-1)
                        {
                            totalConc[posX-1]+=p->getConcentration();
                            numberPores[posX-1]++;
                        }
                    }

                    if(!p->getClosed() && !p->getOutlet())
                    {
                        if(p->getNodeIn()->getIndexX()==posX-1)
                        {
                            if(p->getWaterFraction()>0.5)
                            {
                                numberWater[posX-1]++;
                            }
                            else
                            {
                                if(p->getOilTrapped())
                                {
                                    numberTrapped[posX-1]++;
                                }
                                else
                                {
                                    if(p->getTheta()==wettabiltyThetaBackup[p])
                                    {
                                        numberOil[posX-1]++;
                                    }
                                    else
                                    {
                                        numberOilMod[posX-1]++;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            ofstream outfileXOcc;
            outfileXOcc.open(pathXOcc.c_str(), ofstream::app);
            outfileXOcc<<injectedPVs<<"\t"<<"\t"<<"W-F"<<"\t"<<"O-F (Mod)"<<"\t"<<"O-F (Init)"<<"\t"<<"O-F (Trapped)"<<endl;
            ofstream outfileAvgConc;
            outfileAvgConc.open(pathAvgConc.c_str(), ofstream::app);
            outfileAvgConc<<injectedPVs<<endl;

            for (int n=1;n<=Nx;n++)
            {
                outfileXOcc<<"\t"<<n<<"\t"<<numberWater[n-1]<<"\t"<<numberOilMod[n-1]<<"\t"<<numberOil[n-1]<<"\t"<<numberTrapped[n-1]<<endl;

                if(numberPores[n-1]>0)
                    outfileAvgConc<<"\t"<<n<<"\t"<<totalConc[n-1]/numberPores[n-1]<<endl;
                else
                    outfileAvgConc<<"\t"<<n<<"\t"<<totalConc[n-1]<<endl;
            }

            outfileXOcc<<endl;
            outfileAvgConc<<endl;
            outfileXOcc.close();
            outfileAvgConc.close();
        }
        else
        {
            int numBins=int(cbrt(totalOpenedNodes))+1;
            int numCurrentBin=0;

            int numberOil[numBins];
            int numberOilMod[numBins];
            int numberWater[numBins];
            int numberTrapped[numBins];
            double totalConc[numBins];
            int numberPores[numBins];

            for (int n=1;n<=numBins;n++)
            {
                numberOil[n-1]=0;
                numberOilMod[n-1]=0;
                numberWater[n-1]=0;
                numberTrapped[n-1]=0;
                totalConc[n-1]=0.0;
                numberPores[n-1]=0;
            }

            double delX=xEdgeLength/numBins;
            double lowerLimit=xEdgeLength-delX;
            double upperLimit=xEdgeLength;

            while(lowerLimit>=-delX/100.0)
            {
                numCurrentBin++;

                for(int i=0;i<totalPores;++i)
                {
                    pore* p=getPore(i);

                    if(!p->getClosed() && !p->getOutlet() && p->getWaterFraction()>2*precisionFactor)
                    {
                        if(p->getNodeIn()->getXCoordinate()>lowerLimit && p->getNodeIn()->getXCoordinate()<=upperLimit)
                        {
                            totalConc[numCurrentBin-1]+=p->getConcentration();
                            numberPores[numCurrentBin-1]++;
                        }
                    }

                    if(!p->getClosed() && !p->getOutlet())
                    {
                        if(p->getNodeIn()->getXCoordinate()>lowerLimit && p->getNodeIn()->getXCoordinate()<=upperLimit)
                        {
                            if(p->getWaterFraction()>0.5)
                            {
                                numberWater[numCurrentBin-1]++;
                            }
                            else
                            {
                                if(p->getOilTrapped())
                                {
                                    numberTrapped[numCurrentBin-1]++;
                                }
                                else
                                {
                                    if(p->getTheta()==wettabiltyThetaBackup[p])
                                    {
                                        numberOil[numCurrentBin-1]++;
                                    }
                                    else
                                    {
                                        numberOilMod[numCurrentBin-1]++;
                                    }
                                }
                            }
                        }
                    }
                }

                upperLimit=lowerLimit;
                lowerLimit-=delX;
            }

            ofstream outfileXOcc;
            outfileXOcc.open(pathXOcc.c_str(), ofstream::app);
            outfileXOcc<<injectedPVs<<"\t"<<"\t"<<"W-F"<<"\t"<<"O-F (Mod)"<<"\t"<<"O-F (Init)"<<"\t"<<"O-F (Trapped)"<<endl;
            ofstream outfileAvgConc;
            outfileAvgConc.open(pathAvgConc.c_str(), ofstream::app);
            outfileAvgConc<<injectedPVs<<endl;

            for (int n=1;n<=numBins;n++)
            {
                outfileXOcc<<"\t"<<double(n)*delX<<"\t"<<numberWater[numBins-n]<<"\t"<<numberOilMod[numBins-n]<<"\t"<<numberOil[numBins-n]<<"\t"<<numberTrapped[numBins-n]<<endl;

                if(numberPores[numBins-n]>0)
                    outfileAvgConc<<"\t"<<double(n)*delX<<"\t"<<totalConc[numBins-n]/numberPores[numBins-n]<<endl;
                else
                    outfileAvgConc<<"\t"<<double(n)*delX<<"\t"<<totalConc[numBins-n]<<endl;
            }

            outfileXOcc<<endl;
            outfileAvgConc<<endl;
            outfileXOcc.close();
            outfileAvgConc.close();
        }
    }

//---------------------------------------------------------

}
