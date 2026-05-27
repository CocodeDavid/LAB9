#include "Graph.h"
#include <string>

template<class T>
Vertex<T>::Vertex(const T& value) {
	data = value;
	inDegree = 0;
	outDegree = 0;
	connectedTo = {};
}

template<class T>
Vertex<T>::~Vertex() {}

template<class T>
void Vertex<T>::addNeighbor(Vertex<T> *to, int weight) { //crea arista del vertice actual hacia otro de destino y actualiza grados
	Edge<T>* temp = new Edge<T>;
	temp->to = to;
	temp->weight = weight;
	outDegree++;  //aumenta la salida del origen
	to->inDegree++;    //aumenta la entrada del destino
	connectedTo.insert(connectedTo.size(), temp);  //
}

template<class T>
int Vertex<T>::getWeight(const T &value) {   //busca una arista y si la encunetra devuelve su peso
	for(int i=0; i < connectedTo.size(); i++){
		Edge<T>* temp = connectedTo.get(i);
		if(temp->to->data == value){
			return connectedTo.get(i)->weight;
		}
	}
	return -1;
}

template<class T>
Graph<T>::Graph() {
	count = 0;
	vertexList = {};
}

template<class T>
Graph<T>::~Graph() {}

template<class T>
Vertex<T>* Graph<T>::addVertex(const T &value) {
	Vertex<T>* newVertex = new Vertex<T>(value);
	vertexList.insert(vertexList.size(), newVertex);            //crea un nuevo vértice y lo agrega a la lista de vértices del grafo
	count++;
	return newVertex;
}

template<class T>
void Graph<T>::addEdge(const T& from, const T& to, int weight) {  //conecta dos vertices con una arista, si no existen los crea 
	Vertex<T>* fromVertex = getVertex(from);
	if(!fromVertex){
		fromVertex = addVertex(from);                   
	}
	Vertex<T>* toVertex = getVertex(to);
	if(!toVertex){
		toVertex = addVertex(to);  
	}
	fromVertex->addNeighbor(toVertex, weight);
}

template<class T>
Vertex<T> *Graph<T>::getVertex(const T &value) { //busca un vertice recorriendo la lista de estos y cuando lo encuentra lo devuelve
	for(int i=0; i < vertexList.size();i++ ){
		if(vertexList.get(i)->data == value) return vertexList.get(i);
	}
	return NULL;
}

template class Edge<string>;
template class Vertex<string>;
template class Graph<string>;