#ifndef PERSISTENT_FILES_H
#define PERSISTENT_FILES_h

//EEPROM library written for the Mars Rover Design Team
#include "RoveEEPROM.h"

#define MAX_FILES_PER_LEVEL 10
#define END_OF_LEVEL ')'
#define FILE_END '%'
#define FOLDER_END '&'

RoveEEPROM EEPROM;

// A node of the tree
struct Node { 
   String key; 
   bool isFolder;
   Node *child[MAX_FILES_PER_LEVEL];  // An array of pointers for N children 
};

// A utility function to create a new N-ary tree node 
Node *newNode(String key) 
{ 
    Node *temp = new Node; 
    temp->key = key; 
    for (int i = 0; i < MAX_FILES_PER_LEVEL; i++) 
        temp->child[i] = NULL; 
    return temp; 
} 

////////////////////////////////////////////////////////////////////////////////////////
//The purpose is to store the file structure to persistent memory at the end of emulation,
//and to load the file structure from persistent memory at the start of emulation. This
//is similar to serializing/deserializing a N-ary tree, a common problem.
////////////////////////////////////////////////////////////////////////////////////////
class PersistentFiles
{
    public:
        void serialize(Node *root); 
        int deSerialize(Node *&root); 
        void resetFiles();

    private:
        int index;
};























