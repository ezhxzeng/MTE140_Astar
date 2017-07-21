#include <iostream>
#include <math.h>
#include <algorithm> 
#include <string>
#include <sstream>

#include "a_star.hpp"

using namespace std;

// Calculates and returns the fcost
int AStar::getFCost(Node* currentNode) {
	// TODO Step1. Return fCost as a sum of gCost and hCost	
	//fcost = gCost + hcost
	currentNode->fCost = currentNode->hCost + currentNode->gCost;
	return currentNode->fCost;
}

// Computes the Euclidean distance between two nodes and scales it by 10 to avoid floats
int AStar::getHDistance(Node* A, Node* B) {
	// TODO Step2. Return the Euclidian distance scaled by 10
	
	return sqrt(pow((A->posX - B->posX),2) + pow((A->posY - B->posY),2)) * 10;
}

// Iterates through the node's parents and creates a list of the shortest path to take
list<Node*> AStar::retracePath (Node* startNode, Node* endNode) {
	list<Node*> path;	
	Node* currentNode = endNode;

	while (currentNode != startNode) {
		path.push_front(currentNode);
		currentNode = currentNode->parent;
	}
	return path;
}

// For the current node, cnode, discovers all walkable neighbours, and adds them to the neighbours list
list<Node*> AStar::getNeighbours(Node* cnode, Node* grid[Y_LENGTH][X_LENGTH]) {
	list<Node*> neighbours;	
	
	// TODO Step3. Add walkable neighbours to the neighbours list
	// Step3.1 Iterate from (cnode.y - 1) to (cnode.y + 1)
	// Step3.2 Iterate from (cnode.x - 1) to (cnode.x + 1)	
	// Step3.3 Add neighbours that are not outside the world bounds and that are walkable
	
	// assuming that corners are walkable
	for(int j = cnode->posY -1; j<= cnode->posY+1; j++){
		for(int i = cnode->posX -1; i<= cnode->posX+1; i++){
			if(grid[j][i]->walkable && grid[j][i]!=cnode)
				neighbours.push_back(grid[j][i]);
		}
	}
	return neighbours;
}

// Outputs the discovered path as a string
string AStar::getPathString(list<Node*> path) {
	stringstream outputStream;
	for (list<Node*>::iterator it = path.begin(); it != path.end(); ++it) {
		outputStream << "(" << (*it)->posX << "," << (*it)->posY << ")" << endl;
	}
	return outputStream.str();
}

// Outputs path distance
int AStar::getPathDistance(list<Node*> path) {
	return path.size() > 0 ? path.back() -> fCost : 0;
}

// Finds shortest path between startNode and endNode using A* algorithm
list<Node*> AStar::findPath(Node* world[Y_LENGTH][X_LENGTH], Node* startNode, Node* endNode) {
	list<Node*> openSet; // list of nodes not yet evaluated
	list<Node*> closedSet; // list of nodes already evaluated
	Node* currentNode;

	startNode->gCost = 0;
	startNode->hCost = getHDistance(startNode, endNode);
	startNode->fCost = startNode->hCost;
	
	openSet.push_back(startNode); // insert the starting node at the beginning of the open set
	while(!openSet.empty()) {	
		
		// TODO Step4. Find a node in the openSet that has the smallest fCost
		// If there is a conflict, select the node with the smaller hCost
		// Use <list> iterator to iterate through the list; see sample iterator code below
		Node* cur = openSet.front();
		for(list<Node*>::iterator it = openSet.begin(); it != openSet.end(); it++) { 
			if((cur->fCost == (*it)->fCost && cur->hCost > (*it)->hCost) || cur->fCost > (*it)->fCost){
				cur = *it;
			}
		}
		

		
		// TODO Step5. Remove the found node from the open list and insert it into closed list
		openSet.remove(cur);
		cur->parent = closedSet.front();
		closedSet.push_front(cur);
		
		
		// TODO Step6. Get a list of walkable neighbours for the current node		
		list<Node*> neighbours = getNeighbours(cur, world);
		
		// TODO Step7. Iterate through the neighbours list and add matching neighbours to the open list		
		for(list<Node*>::iterator it = neighbours.begin(); it != neighbours.end(); it++) { 
		
			// Step7.1. Check if the current neighbour is already in the closed list; if it is, skip it
			bool inClosedSet = false;
			for(list<Node*>::iterator it2 = closedSet.begin(); it2 != closedSet.end(); it2++) { 
				if(*it == *it2) inClosedSet = true;
			}
			if(!inClosedSet){
			// Step7.2. Compute gCost from the start node for the curent neighbour
			// If that cost is less than previously computed gCost, update the neighbour's parent to the current node, and 
			// update gCost, hCost, and fCost values for the neighbour to match the current node
			// Use getHDistance to get the cost from the current node to the current neighour	
				if(((*it)->parent != NULL && (*it)->gCost < (cur->gCost + getHDistance(*it, cur)) || (*it)->parent == NULL)){
					(*it)->parent = cur;
					(*it)->gCost = (cur->gCost + getHDistance(*it, cur));
					(*it)->hCost = getHDistance(*it, endNode);
					(*it)->fCost = getFCost(*it);
				}
			}
 		}
 		
 		// TODO Step8. Check if the current node is the end node; if it is, return the retraced path from start to end
		if(cur == endNode){
			return retracePath(startNode, endNode);
		}

	}
	
	list<Node*> empty;
	return empty;
}
