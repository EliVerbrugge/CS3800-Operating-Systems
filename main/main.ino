#include "PersistentFiles.h"

void setup()
    {   
        Serial.begin(9600);
        PersistentFiles PersistentFileStorage;
        while(!Serial.available());

        Serial.println("Resetting files");
        PersistentFileStorage.resetFiles();

        Serial.println("Creating dummy tree");
        Node *root = createDummyTree(); 

        Serial.println("Serializing tree");
        PersistentFileStorage.serialize(root);

        Serial.println("Deserializing tree");
        Node *node1 = NULL;
        PersistentFileStorage.resetIndex();
        PersistentFileStorage.deSerialize(node1);

        Serial.println("Traversing tree");
        traverse(node1);
    }

void loop()
    {

    }

// A utility function to create a dummy tree shown in above diagram 
Node *createDummyTree() 
{ 
    Node *root = newNode("Eli", true); 
    root->child[0] = newNode("Desktop", true); 
    root->child[1] = newNode("Pcitures", true); 
    root->child[2] = newNode("Documents", true); 
    root->child[0]->child[0] = newNode("Help.png", false); 
    root->child[0]->child[1] = newNode("Test", true); 
    root->child[2]->child[0] = newNode("Yellowstone.txt", false); 
    root->child[2]->child[1] = newNode("Speech.pptx", false); 
    root->child[2]->child[2] = newNode("AHHH", true); 
    root->child[2]->child[3] = newNode("Games", true); 
    root->child[0]->child[1]->child[0] = newNode("GreatPic.jpg", false); 
    return root; 
} 

void traverse(Node *root) 
{ 
    if (root) 
    { 
        Serial.println(root->key); 
        for (int i = 0; i < MAX_FILES_PER_LEVEL; i++) 
            traverse(root->child[i]); 
    } 
} 
  