#include "routing_sub_problem.h"

RoutingSubProblem::RoutingSubProblem(unsigned *matrixLength, unsigned tNos):route(tNos)
{
	findedRoute = false;
	this->tNos = tNos;
	this->matrixLength = matrixLength;
}

bool RoutingSubProblem::searchRoute(unsigned from, unsigned to)
{
	return findedRoute = dijkstra(from, to);
}

//dijkstra temporário
//a ideia do meu novo dijkstra eh deixar ele guloso porém arrependido.
//parametro para eu pensar: tNos, dist, lambdaLivres,Pase,Pin etc.

//ta estranho, porém funciona. Ver parâmetros do pow. eh pra ser i ou k. Fazer depois à mão
bool RoutingSubProblem::dijkstra(unsigned s, unsigned t)
{
    const unsigned inf = 0xFFFFFFFF;

	vector<float> dist(this->tNos,(float)inf);
	//vector<unsigned> dist(this->tNos,inf);
	vector<unsigned> prev(this->tNos,inf);
	vector<bool> pego(this->tNos,false);

	vector<float> funX(this->tNos,0.0);

    dist[t] = 0;
    prev[t] = t;
    pego[t] = true;

    unsigned k = t;
    //unsigned min = inf;
    float min = inf;
    unsigned assegure = 0;
    float alfa = 1.7f;//1.7;
    float beta = alfa - 0.5f;//alfa - 0.5f;
    float link_bias = 280;//480.0;

    float costLink = 0.0;
    do
    {
        for (int i = 0; i < tNos; i++)
            if (matrixLength[k*tNos + i] != 0 && !pego[i])//se existe o link
            {
                costLink = alfa*floor(matrixLength[k*tNos + i]/link_bias + lambdaControl->getTotalLambdasAllocatedIn(k,i)/lambdaControl->getTotalLambdas() + pow(funX[i],funX[i]));//pow(funX[i],funX[i])
                //std::cout << matrixLength[k*tNos + i] << " " << lambdaControl->getTotalLambdasAllocatedIn(k,i) << " " << pow(funX[i],funX[i]) << " " << costLink << std::endl;std::cin.get();
                if (dist[k] + costLink < dist[i])
                //if (dist[k] + matrixLength[k*tNos + i] < dist[i]);
                {
                    prev[i] = k;
                    dist[i] = dist[k] + costLink;
                    //dist[i] = dist[k] + matrixLength[k*tNos + i];

                    funX[i] = funX[k] + beta;
                }
            }
        k = 0;
        min = inf;
        for (int i = 0; i < tNos; i++)
            if (!pego[i] && dist[i] < min)
            {
                min = dist[i];
				k = i;
            }
        pego[k] = true;
        if (assegure++ >= tNos) //grafo disconexo
            return false;
    } while (k!=s);

    k = s;

    route.clear();
    do
    {
        route.addLinkToPath(k);
        k = prev[k];
    } while (prev[k] != k);
    route.addLinkToPath(k);
    return true;
}

//dijkstra temporário
//bool RoutingSubProblem::dijkstra(unsigned s, unsigned t)
//{
//    const unsigned inf = 0xFFFFFFFF;
//
//	vector<unsigned> dist(this->tNos,inf);
//	vector<unsigned> prev(this->tNos,inf);
//	vector<bool> pego(this->tNos,false);
//
//    dist[t] = 0;
//    prev[t] = t;
//    pego[t] = true;
//
//    unsigned k = t, min = inf;
//    unsigned assegure = 0;
//    do
//    {
//        for (int i = 0; i < tNos; i++)
//            if (matrixLength[k*tNos + i] != 0 && !pego[i])
//            {
//                if (dist[k] + matrixLength[k*tNos + i] < dist[i])
//                {
//                    prev[i] = k;
//                    dist[i] = dist[k] + matrixLength[k*tNos + i];
//                }
//            }
//        k = 0;
//        min = inf;
//        for (int i = 0; i < tNos; i++)
//            if (!pego[i] && dist[i] < min)
//            {
//                min = dist[i];
//				k = i;
//            }
//        pego[k] = true;
//        if (assegure++ >= tNos) //grafo disconexo
//            return false;
//    } while (k!=s);
//
//    k = s;
//
//    route.clear();
//    do
//    {
//        route.addLinkToPath(k);
//        k = prev[k];
//    } while (prev[k] != k);
//    route.addLinkToPath(k);
//    return true;
//}


//O(n.log(n))
//acho que vou ter que implementar a priority queue
//bool RoutingSubProblem::dijkstra(unsigned s, unsigned t)
//{
//    const unsigned inf = 0xFFFFFFFF;
//
//	vector<unsigned> dist(this->tNos,inf);
//	vector<unsigned> prev(this->tNos,inf);
//    dist[t] = 0;
//
//    //makequeue ->testar(, vector<unsigned>, greater<unsigned>)
//    priority_queue<unsigned> pq (dist.begin(),dist.end());
//
//    while (!pq.empty())
//    {
//        unsigned u = pq.top();pq.pop();
//        for ( int v = 0 ; v < tNos ; v++)
//        {
//            if (dist[v] > dist[u] + getEdge(u,v))
//            {
//                dist[v] = dist[u] + getEdge(u,v);
//                prev[v] = u;
//                //pq.decreasekey(u);
//            }
//        }
//    }
//
//	return false;
//}

unsigned RoutingSubProblem::getEdge(unsigned s, unsigned t)
{
    return matrixLength[s*tNos + t];
}

bool RoutingSubProblem::hasFindedRoute() const
{
	return findedRoute;
}

const Path &RoutingSubProblem::getPath() const
{
	return route;
}

void RoutingSubProblem::setStateNetwork(LambdaControl *lambdaControl)
{
    this->lambdaControl = lambdaControl;
}

//futuramente criar variavel bool para dizer se esta setado ou nao. Lançar exceção caso n esteja
void RoutingSubProblem::setMatrixLength(unsigned *matrixLength, unsigned tNos)
{
	this->matrixLength = matrixLength;
	this->tNos = tNos;
}
