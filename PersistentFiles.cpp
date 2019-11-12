#include "PersistentFiles.h"


void PersistentFiles::resetFiles()
{
    //just set the index to 0
    index = 0;
    //TODO: define how many bytes of EEPROM we are limited to using. Maybe everything?
    for (int i = 0 ; i < 1000; i++) 
    {
        EEPROM.write(i, 0);
    }
}

// This function stores the given N-ary tree in persistent memory
void PersistentFiles::serialize(Node *root) 
{ 
    // Base case 
    if (root == NULL) return; 
  
    // Else, store current node and recur for its children 
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
         serialize(root->child[i]); 
  
    // Store marker at the end of children 
    EEPROM.write(index, END_OF_LEVEL);
    index++;
} 

// This functionr returns 0 to indicate that the next item is a valid 
// tree key. Else returns 1 
int PersistentFiles::deSerialize(Node *&root) 
{ 
    char val; 
    String key;
    do
    {
        val = EEPROM.read(index);
        if ( val == 0 || val == MARKER ) 
            return 1; 
        key += val;
    }while(val != FOLDER_END || val != FILE_END)
  
    root = newNode(val); 
    for (int i = 0; i < N; i++) 
      if (deSerialize(root->child[i])) 
         break; 
  
    return 0; 
} 