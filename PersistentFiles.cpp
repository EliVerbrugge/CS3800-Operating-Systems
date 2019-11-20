#include "PersistentFiles.h"
#include "RoveEEPROM.h"

RoveEEPROM EEPROM;


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
void PersistentFiles::serialize(FILE_NODE *root) 
{ 
    // Base case 
    if (root == NULL)
    {
        return; 
    }
  
    // Else, store current node and recur for its children 
    for(int j = 0; j < (root->getName()).length(); j++)
    {
        EEPROM.write(index, (root->getName())[j]);
        index++;
    }

    if(root->isDirectory())
    {
        EEPROM.write(index, FOLDER_END);
        index++;
    }
    else
    {
        EEPROM.write(index, FILE_END);
        index++;
    }
    

    for (int i = 2; i < MAX_FILES_PER_LEVEL && root->fileArray[i].second != NULL; i++) 
    {
        serialize(root->fileArray[i].second); 
    }
  
    // Store marker at the end of children 
    EEPROM.write(index, END_OF_LEVEL);
    index++;
    return;
} 

// This functionr returns 0 to indicate that the next item is a valid 
// tree key. Else returns 1 
int PersistentFiles::deSerialize(FILE_NODE *&root) 
{ 
     char val; 
    String key;
    bool isFolder;
    for (int i = 2; i < MAX_FILES_PER_LEVEL; i++) 
    {
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
      key.replace("&", "");
      key.replace("%", "");
      key.replace(")", "");
      key.replace(" ", "");

      root->createInstance(key, isFolder);
      deSerialize(root->fileArray[i].second);

      key = "";
    }
  
    return 0; 
} 