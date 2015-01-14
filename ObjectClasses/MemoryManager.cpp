/*
 * MemoryManager.cpp
 *
 *  Created on: Jul 30, 2013
 *      Author: kons
 */


#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "MemoryManager.h"
//#include "../Utils/configReader.h"
#include "Object.h"

using namespace std;

namespace traceGen {

MemoryManager::MemoryManager() {
	int i;
	rootset.resize(NUM_THREADS);

	//init all rootsets
	for(i = 0; i < NUM_THREADS ; i++){
		rootset.at(i).resize(ROOTSET_SIZE);
	}

	objectList.resize(NUM_THREADS*ROOTSET_SIZE);

	nextId = 1;
}

int MemoryManager::setRootPointer(int threadNumber, int rootsetNumber, Object* target){
	if(rootsetNumber >= ROOTSET_SIZE || threadNumber >= NUM_THREADS){
		return -1;
	}
	rootset[threadNumber][rootsetNumber] = target;
	return 0;
}

int MemoryManager::allocateObjectToRootset(int size, int threadNumber,
		int rootSetNumber, int maxPointers, int creationDate){
	//check if you can create more objects and get a slot in object list
	int listSlot = getListSlot();
	if(listSlot<0){
		printf("listError\n");
		fflush(stdout);
		return 0;
	}
	//create new Object
	//printf("creating root with id: %d\n", nextId);
	int id = nextId;
	nextId++;
	Object* newObject = new Object(id,size,maxPointers, creationDate);
	objectList[listSlot] = newObject;
	//set the rootset pointer as desired
	setRootPointer(threadNumber, rootSetNumber, newObject);
	return id;
}

int MemoryManager::allocateObject(int size, int maxPointers,
		Object* creatorObject, int pointerIndex, int creationDate){
	//check if you can create more objects and get a slot in object list
	int listSlot = getListSlot();
	if(listSlot<0){
		printf("listError\n");
		fflush(stdout);
		return 0;
	}
	//create new Object
	//printf("creating new Object with id: %d\n\n", nextId);
	int id = nextId;
	nextId++;
	Object* newObject = new Object(id,size,maxPointers, creationDate);
	objectList[listSlot] = newObject;
	//set the rootset pointer as desired
	setPointer(creatorObject, pointerIndex, newObject);
	return id;
}



int MemoryManager::setPointer(Object* startObject, int pointerIndex, Object* targetObject){
	return startObject->setPointer(pointerIndex,targetObject);
}

int MemoryManager::deletePointer(Object* startObject, int pointerNumber){
	return startObject->setPointer(pointerNumber, NULL);
}

Object* MemoryManager::getObjectByID(int id){
	int i;
	Object* temp;
	for(i=0;(unsigned int)i<objectList.size();i++){
		temp = objectList[i];
		if(temp){
			int currentId = temp->getID();
			if(id == currentId){
				return temp;
			}
		}
	}
	return NULL;
}

Object* MemoryManager::getRoot(int threadNumber, int rootSlotNumber){
	return rootset[threadNumber][rootSlotNumber];
}

void MemoryManager::deleteRoot(int thread, int root){
	rootset[thread][root] = NULL;
}

int MemoryManager::getListSlot(){
	unsigned int i;
	for(i = 0 ; i < objectList.size() ; i++){
		if(!objectList.at(i)){
			return i;
		}
		if(i+1 == objectList.size()){
			objectList.resize(objectList.size()*2);
		}
	}
	printf("listSlotERror\n");
	fflush(stdout);
	return -1;
}

int MemoryManager::isRoot(int thread, Object* obj){
	int i;
	for(i = 0 ; i < ROOTSET_SIZE ; i++){
		if(rootset[thread].at(i) && obj == rootset[thread].at(i)){
			return 1;
		}
	}
	return 0;
}

int MemoryManager::getRootsetNumberByReference(int thread, Object* obj){
	int i;
	for(i = 0 ; i < ROOTSET_SIZE ; i++){
		if(rootset[thread].at(i) && obj == rootset[thread].at(i)){
			return i;
		}
	}
	return -1;
}

void MemoryManager::visualizeState(){
	FILE* file = fopen("state.dot", "w");
		fprintf(file, "digraph G { \n");
		int i;
		int j;
		int k;
		//create threads and init pointers
		for(i = 0 ; i < NUM_THREADS ; i++){
			fprintf(file, "T%d;\n", i);
			for(j = 0 ; j < ROOTSET_SIZE ; j++){
				if(rootset.at(i).at(j)){
					fprintf(file, "T%d -> %d;\n", i, rootset.at(i).at(j)->getID());
				}
			}
		}

		for(k = 0 ; (unsigned int)k < objectList.size() ; k++){
			//Object* parent = objectList.at(k);
			if(objectList.at(k)){
				for(i = 0; i < objectList.at(k)->getPointersMax() ;i++){
					Object* child = objectList.at(k)->getReferenceTo(i);
					if(child){
						fprintf(file, "%d -> %d;\n",objectList.at(k)->getID(), child->getID());
					}
				}
			}
		}
		fprintf(file, "}\n");
		fclose(file);

}

void MemoryManager::setupObjects(){
	int i;
	Object* temp;
	for(i = 0 ; (unsigned int)i < objectList.size() ; i++){
		temp = objectList[i];
		if(temp){
			temp->visited = 0;
		}
	}
}

MemoryManager::~MemoryManager() {
}

} /* namespace gcKons */