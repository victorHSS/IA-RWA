#include "simulation.h"

Simulation::Simulation()
{
    cL = 0;
    phy = 0;
    cac = 0;
    actualizations = 0;
    considerPhyImpairments = false;
    considerBlockByQoT = true;
    reset();
}

void Simulation::setConsiderPhyImpairments(bool considerPhyImpairments)
{
    this->considerPhyImpairments = considerPhyImpairments;
}

void Simulation::setConsiderBlockByQoT(bool considerBlockByQoT)
{
    this->considerBlockByQoT = considerBlockByQoT;
}

void Simulation::reset()
{
    CVP = TVP = sizeInHops = blockByCont = blockByOSNRInnerCon = blockByOSNRAffectedCon = 0;

    for (int i = 0 ; i < rateGainAffected.size() ; i++)
        rateGainAffected[i] = 0.0;
    for (int i = 0 ; i < tGainAffected.size() ; i++)
        tGainAffected[i] = 0;
}

void Simulation::store()
{
    static bool flag = true;
    fstream file;
    fstream file2;
    fstream file3;

    string arq1 = "Bloqueios.txt";
    string arq2 = "VariacaoGanho.txt";
    string arq3 = "MediaBloqueioPorRota.txt";

    if (flag)//para quando iniciar uma simulação apagar o arquivo.
    {
        file.open(arq1.c_str(),fstream::out);
        file2.open(arq2.c_str(),fstream::out);
        file3.open(arq3.c_str(),fstream::out);
        flag = false;
        file << "Cont.   " << "OIN     " <<  "OIO     " << "Total" << endl;
        for ( int i = 0 ; i < actualizations ; i++)
            file2 << "G" << i << "\t";
        file2 << "G" << actualizations << endl;
    }
    else
    {
        file.open(arq1.c_str(),fstream::out | fstream::app);
        file2.open(arq2.c_str(),fstream::out | fstream::app);
        file3.open(arq3.c_str(),fstream::out | fstream::app);
    }

    locale mylocale("");
    file.imbue( mylocale );
    file2.imbue( mylocale );
    file3.imbue( mylocale );

    double t = 0.0;
    cout << "Bloqueio por continuidade: " << ((double)blockByCont)/tCalls*100 << "%" << endl;
    cout << "Bloqueio por OSNR Inner: " << (double)blockByOSNRInnerCon/tCalls*100 << "%" <<  endl;
    cout << "Bloqueio por OSNR Affected: " << (double)blockByOSNRAffectedCon/tCalls*100 << "%" <<  endl;
    t = ((double)blockByCont + blockByOSNRInnerCon + blockByOSNRAffectedCon)/tCalls*100;
    cout << "Bloqueio total: " << t << "%" <<  endl;

    file << ((double)blockByCont)/tCalls*100 << "\t" << (double)blockByOSNRInnerCon/tCalls*100
         << "\t" << (double)blockByOSNRAffectedCon/tCalls*100 << "\t" << t << endl;

    cout << "Média de saltos por rota: " << (double)sizeInHops/tCalls << endl;
    file3 << (double)sizeInHops/tCalls << endl;

    cout << "TVP: " << (double)TVP/tCalls*100.0 << "% CVP: " << (double)CVP/tCalls*100.0 << "%" << endl;
    cout << "Variação do ganho:" << endl;
    for ( int i = 0 ; i <= actualizations ; i++)
    {
        cout << "  " << i << ") " << rateGainAffected[i]/tGainAffected[i] << "%" << endl;
        file2 << rateGainAffected[i]/tGainAffected[i] << (i<actualizations?"\t":"");
    }
    file2 << endl;

    file.close();
    file2.close();
}

void Simulation::setUpClientLayer(ClientLayer &clientLayer)
{
    cL = &clientLayer;
}

void Simulation::setUpCACLayer(CACLayer &cacLayer)
{
    cac = &cacLayer;
}

void Simulation::setUpPhysicalLayer(PhysicalLayer &physicalLayer)
{
    phy = &physicalLayer;
    gain_before.reserve( phy->getTotalNodes() );
    gain_after.reserve( phy->getTotalNodes() );
}

bool Simulation::configure(char *progresso)
{
    cac->setPhysicalLayer(phy);
    locale mylocale("");
    cout.imbue( mylocale );
    this->progresso = progresso;
    return true;
}

void Simulation::run(double la_min, double la_max, double la_step, unsigned totalCalls)
{
    tCalls = totalCalls;
    cout << "Calls: " << tCalls << endl;
    int t = 0;
    for (double la = la_min ; la <= la_max ; la+= la_step)
    {
        reset();
        cL->reconfigureLoad(la);
        cout << (*cL) << endl;
        simulate(totalCalls);t++;
        cac->clearMatrixAllocation();//limpa alocações
        cac->clearListOfConnections();//limpa lista de conexoes
        store();

        fstream file;
        file.open(progresso,fstream::out);
        file << ((double) t/((la_max - la_min) / la_step + 1.0));
        file.close();
    }
}

void Simulation::simulate(unsigned totalCalls)
{
    double actualTime = 0.0;
    unsigned s,t;
//    const unsigned pValue = 50;
//    string progress(pValue,' ');
//    unsigned v=0;
    for (int i = 0 ; i < totalCalls ; i++)
    {
//        if (i%(totalCalls/pValue)==0)
//        {
//            progress[v++]='.';
//            cout << "[" << progress << "] " << ((double)i/totalCalls*100) << "%" << endl;
//        }
//        cout << "Con estabilizadas: " << cac->getListOfConnections().size() << ":" << i << ":"<<totalCalls << endl;
        cL->getConnection(s,t);
        if (cac->tryRequest(s,t,cL->requestQoS(),cL->requestQoSThereshold()))
        {
//            cout << "s " << s << " t " << t << endl;
//            cout << cac->getLastConnection().getPath() << endl;
            sizeInHops += cac->getLastConnection().getPath().getLength();
//            cout << "pre alocando:{" << endl;
            if (considerPhyImpairments)
            {
                preComputeGainVariation(cac->getLastConnection());
                cac->preAlloc();
                posComputeGainVariation(cac->getLastConnection(),0 /* G0*/ );
                updatePhysicalLayer();
//            cout << "}" << endl;
            //ver variacao do ganho
//            cout << "con entrante {" << endl;
                if (cac->testConnectionRequest() || !considerBlockByQoT)
                {
//                    cout << "  con afetadas {" << endl;
                    if (cac->testConnectionsAffected() || !considerBlockByQoT)
                    {
    //                    cin.get();
//                        cout << "    C alocada:{" << endl;
                        cac->allocateResources(actualTime,cL->requestDuration());

                        if (!considerBlockByQoT)
                            if (!cac->testAllConnectionsAffected())
                                TVP++;
                            else if (!cac->testAllConnectionsAffected(CACLayer::QoTThereshold))
                                CVP++;
//                        cout << "    }" << endl;
                    }
                    else
                    {
                        //bloqueio por OSNR de outras conexoes
//                        cout << "    OSNR outras:{" << endl;
                        blockByOSNRAffectedCon++;
                        cac->desAllocPre();
                        updatePhysicalLayer();
//                        cout << "    }" << endl;
                    }
//                    cout << "  }" << endl;
                }
                else
                {
                    //bloqueio por OSNR entrante
//                    cout << "  OSNR entrante: {" << endl;
                    blockByOSNRInnerCon++;
                    cac->desAllocPre();
                    updatePhysicalLayer();
//                    cout << "  }" << endl;
                }
                cac->clearToUpdateList();
            }
            else
            {
                cac->allocateResources(actualTime,cL->requestDuration());
            }
        }
        else
        {
            //bloqueio por continuidade
            blockByCont++;
        }
//        cout << "}" << endl;
        actualTime += cL->nextConnectionInterval();

//        cout << "Atualizando cons{" << endl;
        cac->updateConnectionsByTime(actualTime);

        if (considerPhyImpairments)
        {
            cac->updateConnectionsRemoved();
            updatePhysicalLayer(false);
            //estudo futuro
    //        if (cac->testAllConnectionsAffected())
    //        {
    //            cout << "Con com OSNR degradada apos retirada de conexoes" << endl;
    //            cin.get();
    //        }
            cac->clearToUpdateList();
        }
//        cout << "}" << endl;
//        cin.get();
    }
}

void Simulation::updatePhysicalLayer(bool flag)
{
    std::list<Connection*> &toUpdateList = cac->getToUpdateList();
    for ( int i = 1 ; i <= actualizations ; i++)
    {
        std::list<Connection*>::iterator it = toUpdateList.begin();
        for(;it != toUpdateList.end();it++)
        {
            if (flag) preComputeGainVariation(*(*it));
            updatePhysicalLayerConnection(*(*it));
            if (flag) posComputeGainVariation(*(*it),i);
        }
    }
}

void Simulation::setUpActualizations(unsigned actualizations)
{
    this->actualizations = actualizations;
    rateGainAffected = vector <double>(actualizations+1,0.0);
    tGainAffected = vector <unsigned>(actualizations+1,0.0);
}

void Simulation::updatePhysicalLayerConnection(Connection &con)
{
    cac->setUpConnection(con,Power(0.001));
}


//dar um jeito para nao computar o ganho de uma conexao pre-admitida q nao eh efetivamente admitida
void Simulation::preComputeGainVariation(Connection &con)
{
    unsigned i = 0;
    Path p = con.getPath();
    p.goStart();

    while(!p.isEndPath())
    {
        WDMLink &link = phy->getLink(p.getActualStep(),p.getNextStep());
        OpticalAmplifier &oA = static_cast<OpticalAmplifier&> (link.getLastComponent());
        gain_before[i++] = oA.getGain();
        p.goAhead();
    }
}

void Simulation::posComputeGainVariation(Connection &con, unsigned actualization)
{
    unsigned i = 0;
    Path p = con.getPath();
    p.goStart();

    while(!p.isEndPath())
    {
        WDMLink &link = phy->getLink(p.getActualStep(),p.getNextStep());
        OpticalAmplifier &oA = static_cast<OpticalAmplifier&> (link.getLastComponent());
        gain_after[i] = oA.getGain();
        rateGainAffected[actualization] += (gain_after[i] - gain_before[i])/gain_before[i]*100.0;
        tGainAffected[actualization]++;
        i++;
        p.goAhead();
    }
}
