#include "cac_layer.h"

CACLayer::CACLayer(unsigned *matrixLength, unsigned tNos, unsigned lambda):lc(tNos,lambda),routing(matrixLength,tNos),phy(0)
{
    wave.setStateNetwork(&lc);
    routing.setStateNetwork(&lc);
}

bool CACLayer::tryRequest(unsigned from, unsigned to, double QoS, double QoSThereshold)
{
    routing.searchRoute(from,to);

    if (routing.hasFindedRoute())
    {
        Path p = routing.getPath();
        wave.searchLambda(p);
        if (wave.hasFindedLambda())
        {

            Connection con;
            con.setPath(p);
            con.setQoT(QoSToQoT(QoS));
            con.setQoTThereshold(QoSToQoT(QoSThereshold));
            con.setLambda(wave.getLastLambdaFinded());

            conList.push_back(con);

            return true;
        }
    }

    return false;
}

void CACLayer::preAlloc()
{
    Connection &con = getLastConnection();

    setUpConnection(con,Power(0.001));
    markConnectionsByConnection(con);

    con.markToUpdate();
    toUpdateList.push_front(&con);
}

//verifica o OSNR DA CONEXAO por todos os enlaces
//bool CACLayer::testOSNRByConnection(Connection &con)
//{
//    Path p = con.getPath();
//
//    bool flag = true;
//
//    p.goStart();
//    while(!p.isEndPath())
//    {
//        WDMLink &link = phy->getLink(p.getActualStep(),p.getNextStep());
//        Power po = link.getLastComponent().getPowerOut(con.getLambda());
//
//        std::cout << link;
//        std::cout << p.getActualStep() << "->" << p.getNextStep() << " ";
//        std::cout << po << " " << "rQoT = " << con.getQoT() << std::endl;
//        std::cin.get();
//
//        if (po.getOSNR() < con.getQoT())
//            flag = false;
//        p.goAhead();
//    }
//    return flag;
//}

//verifica o OSNR da soma TODAS as conexoes que passam por todos os enlaces da conexao entrante
//bool CACLayer::testOSNRByConnection(Connection &con)
//{
//    Path p = con.getPath();
//
//    bool flag = true;
//
//    p.goStart();
//    while(!p.isEndPath())
//    {
//        WDMLink &link = phy->getLink(p.getActualStep(),p.getNextStep());
//        Power po = link.getLastComponent().totalPowerOut();//mudança aki em relação à de cima
//
////        std::cout << link;
////        std::cout << p.getActualStep() << "->" << p.getNextStep() << " ";
////        std::cout << po << " " << "rQoT = " << con.getQoT() << std::endl;
////        std::cin.get();
//
//        if (po.getOSNR() < con.getQoT())
//            flag = false;
//        p.goAhead();
//    }
//    return flag;
//}


//verifica a OSNR no receptor (no fim da conexao)
bool CACLayer::testOSNRByConnection(Connection &con, QUALITY q)
{
    Path p = con.getPath();
    p.goLastLink();
    WDMLink &link = phy->getLink(p.getActualStep(),p.getNextStep());
    Power po = link.getLastComponent().getPowerOut(con.getLambda());

//    std::cout << po.getOSNR() << " " << con.getQoT() << std::endl;
//    std::cin.get();

    if (po.getOSNR() > (q==QoT?con.getQoT():con.getQoTThereshold()))
        return true;

    return false;
}

bool CACLayer::testConnectionRequest(QUALITY q)
{
    return testOSNRByConnection(getLastConnection(),q);
}

bool CACLayer::testConnectionsAffected(QUALITY q)
{
//    std::cout << toUpdateList.size() << std::endl;
    std::list<Connection*>::iterator it = toUpdateList.begin();
    it++;//pula a primeira conexão (causadora do tumultuo)

    for (; it != toUpdateList.end();it++)
        if (!testOSNRByConnection(*(*it),q))
            return false;
    return true;
}

bool CACLayer::testAllConnectionsAffected(QUALITY q)
{
    std::list<Connection*>::iterator it = toUpdateList.begin();

    for (; it != toUpdateList.end();it++)
        if (!testOSNRByConnection(*(*it),q))
            return false;
    return true;
}

void CACLayer::desAllocPre()
{
    setUpConnection(getLastConnection(),Power());
    //markConnectionsByConnection(getLastConnection());//n precisa
    conList.pop_back();//remove conexao da lista de conexoes
    toUpdateList.pop_front();//remove conexao da lista de atualizações
}

void CACLayer::setUpConnection(Connection &con, const Power &power)//so altera a camada fisica nao altera lambdacontrol
{
    Path p = con.getPath();
    Power po = power;

    p.goStart();
    //std::cout << p << std::endl;
    while(!p.isEndPath())
    {
//        std::cout << p.getActualStep() << "->" << p.getNextStep() << std::endl;
        WDMLink &link = phy->getLink(p.getActualStep(),p.getNextStep());
        po = link.processAllComponentBySignal(po,con.getLambda());
//        std::cout << "T Componentes: " << link.getTotalComponents()<< std::endl;
//        for (unsigned i = 0 ; i < link.getTotalComponents();i++)
//        {
//            std::cout << " In " << link.getComponent(i).getPowerIn(con.getLambda()) << std::endl;
//            std::cout << " Out " << link.getComponent(i).getPowerOut(con.getLambda()) << std::endl << std::endl;
//        }

        p.goAhead();
    }
}

void CACLayer::markConnectionsByConnection(Connection &con)
{
    Connection *aCon;
    Path p = con.getPath();

    p.goStart();
    while(!p.isEndPath())
    {
        for ( int l = 0 ; l < phy->totalLambdas() ; l++)
        {
            aCon = lc.getConnection(p.getActualStep(),p.getNextStep(),l);
            if (aCon && !aCon->haveToUpdate())//se passar conexao que ainda n estiver sido marcada por atualização
            {
                aCon->markToUpdate();
                toUpdateList.push_back(aCon);
            }
        }
        p.goAhead();
    }
}

void CACLayer::allocateResources(double actualTime, double duraction)
{
    Connection &con = getLastConnection();

    //setUp new Connection
    con.setDuraction(duraction);
    con.setStartTime(actualTime);

    //allocating
    lc.allocConnection(con,con.getLambda());
}

Connection &CACLayer::getLastConnection()
{
    return conList.back();
}

std::list<Connection> &CACLayer::getListOfConnections()
{
    return conList;
}

std::list<Connection*> &CACLayer::getToUpdateList()
{
    return toUpdateList;
}

void CACLayer::setPhysicalLayer(PhysicalLayer *phy)
{
    this->phy = phy;
}

double CACLayer::QoSToQoT(double gama)
{
    OpticalAmplifier oA = phy->getDefaultOpticalAmplifier();
    return ( ( gama * ( sqrt( oA.getB0() / oA.getBe() ) + gama ) ) / ( oA.getB0() / oA.getBe() ) );
}

void CACLayer::clearToUpdateList()
{
    std::list<Connection*>::iterator it = toUpdateList.begin();
    for (; it != toUpdateList.end();it++)
        (*it)->cleanUpdate();
    toUpdateList.clear();
}

void CACLayer::clearMatrixAllocation()
{
    lc.clear();
}

void CACLayer::clearListOfConnections()
{
    conList.clear();
}

void CACLayer::updateConnectionsByTime(double actualTime)
{
    std::list<Connection>::iterator it = conList.begin();

    tmpRemoved.clear();//so por garantia

    for (; it != conList.end();it++)
        if (it->hasEnded(actualTime))
        {
            lc.disallocConnection(*it);
            tmpRemoved.push_back(*it);
            it = conList.erase(it);
        }
}

void CACLayer::updateConnectionsRemoved()
{
    std::list<Connection>::iterator it = tmpRemoved.begin();

    for (; it != tmpRemoved.end();it++)
    {
        setUpConnection(*it,Power());
        markConnectionsByConnection(*it);
        it = tmpRemoved.erase(it);
    }
}
