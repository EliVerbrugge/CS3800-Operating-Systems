#include "PersistentFiles.h"
#include "RoveEEPROM.h"

RoveEEPROM EEPROM;

Node *newNode(String key, bool folder)
{ 
    Node *temp = new Node; 
    temp->isFolder = folder;
    temp->key = key; 
    for (int i = 0; i < MAX_FILES_PER_LEVEL; i++) 
        temp->child[i] = NULL; 
    return temp; 
} 

void PersistentFiles::resetFiles()
{
    resetIndex();

    //TODO: define how many bytes of EEPROM we are limited to using. Maybe everything?
    for (int i = 0 ; i < 1000; i++) 
    {
        EEPROM.write(i, 0);
    }
}

void PersistentFiles::resetIndex()
{
    //just set the index to 0
    index = 0;
    return;
}

// This function stores the given N-ary tree in persistent memory
void PersistentFiles::serialize(Node *root) 
{ 
    // Base case 
    if (root == NULL) return; 
  
    // Else, store current node and recur for its children 
    Serial.println("Serializing: " + root->key + " at: " + index);
    for(int j = 0; j < (root->key).length(); j++)
    {
        EEPROM.write(index, (root->key)[j]);
        index++;
    }

    if(root->isFolder)
    {
        EEPROM.write(index, FOLDER_END);
        index++;
    }
    else
    {
        EEPROM.write(index, FILE_END);
        index++;
    }
    

    for (int i = 0; i < MAX_FILES_PER_LEVEL && root->child[i]; i++) 
    {
         serialize(root->child[i]); 
    }
  
    // Store marker at the end of children 
    Serial.println("End of level for: " + root->key + " at: " + index);
    EEPROM.write(index, END_OF_LEVEL);
    index++;
} 

// This functionr returns 0 to indicate that the next item is a valid 
// tree key. Else returns 1 
int PersistentFiles::deSerialize(Node *&root) 
{ 
    char val; 
    String key;
    bool isFolder;
    do
    {
        val = EEPROM.read(index);
        index++;
        if ( val == 0 || val == END_OF_LEVEL ) 
            return 1; 
        key += val;
    }while(val != FOLDER_END && val != FILE_END);
    
    if(val == FOLDER_END)
    {
        isFolder = true;
    }
    else
    {
        isFolder = false;
    }
    
    Serial.println(key);
    root = newNode(key, isFolder); 
    for (int i = 0; i < MAX_FILES_PER_LEVEL; i++) 
    {
      if (deSerialize(root->child[i])) 
      {
          Serial.println("Breaking");
         break; 
      }
    }
  
    return 0; 
} 