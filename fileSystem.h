#include "fileNode.h"

struct FILE_SYSTEM
{
    FILE_NODE *currentFile;

    FILE_SYSTEM()
    {
        currentFile = new FILE_NODE("root", NULL, true);
    }

    FILE_SYSTEM(FILE_NODE* file)
    {
        currentFile = file;
    }

    ~FILE_SYSTEM()
    {
        delete currentFile;
    }

    //allows to switch the current node to a node with the specified name
    void traverse(String name)
    {
        if( currentFile->getInstance(name) != NULL
         && currentFile->getInstance(name)->isDirectory() )
        {
            currentFile = currentFile->getInstance(name);
        }
        else
        {
            Serial.println("cd: " + name + ": Not a directory");
        }
    }

    //change the user permission for node with given name
    void changeUserPerm(String name, uint8_t permission)
    {
        if( currentFile->getInstance(name) != NULL)
        {
            currentFile->getInstance(name)->changeUserPermission(permission);
        }
        else
        {
            Serial.println("chmod: " + name + ": Not found");
        }
    }

    //change the group permission for node with given name
    void changeGroupPerm(String name, uint8_t permission)
    {
        if( currentFile->getInstance(name) != NULL)
        {
            currentFile->getInstance(name)->changeGroupPermission(permission);
        }
        else
        {
            Serial.println("chmod: " + name + ": Not found");
        }
    }

    //change the group permission for node with given name
    void changeWorldPerm(String name, uint8_t permission)
    {
        if( currentFile->getInstance(name) != NULL)
	    {
	        currentFile->getInstance(name)->changeWorldPermission(permission);
	    }   
        else
        {
            Serial.println("chmod: " + name + ": Not found");
        }
    }

    //prints the working directory for node we are currently at
    String workingDirectory()
    {
        return currentFile->getDirectory();
    }

    //prints names of all child nodes of node we are currently at
    String listLocalNodes()
    {
        return currentFile->getLocalDirectory();
    }

    //prints attributes of all child nodes of node we are currently at
    //mimics ls -l output
    String* longListLocalNodes()
    {
        String *list = currentFile->getLocalDirectoryLong();
        return list;
    }

    //adds a node with name as a child to current node
    //or if a node with name exits, update the timestamp
    FILE_NODE *addNode(String name, bool isDir)
    {
        if( currentFile->getInstance(name) == NULL )
        {
            return currentFile->createInstance(name, isDir);
        }
        else
        {
            return currentFile->getInstance(name)->updateTimeStamp();
        }
    }
    
    FILE_NODE *getCurrentNode()
    {
        return currentFile;
    }

    //deletes a node with name which is a child to current node
    //only works if we specified we wanted to delete the node type
    //which matches the name (folder or file)
    void deleteNode(String name, bool deleteDir)
    {
        if( currentFile->getInstance(name) != NULL)
        {
            if( currentFile->getInstance(name)->isDirectory() && deleteDir
                || !currentFile->getInstance(name)->isDirectory() && !deleteDir )
            {
                    currentFile->deleteItem(name);
            }
            else
            {
                if(currentFile->getInstance(name)->isDirectory() && !deleteDir)
                {
                    Serial.println("rm: " + name + ": Not a file");
                }
                else if(!currentFile->getInstance(name)->isDirectory() && deleteDir)
                {
                    Serial.println("rmdir: " + name + ": Not a directory");
                }
            }
        }
        else
        {
            Serial.println("rmdir: " + name + ": Not found");
        }
    }

};
