
#include <iostream>
#include <map> 
#include <exception>
#include <vector>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <time.h> 

#include "fileNode.h"

struct FILE_SYSTEM
{
    FILE_NODE *currentFile;

    FILE_SYSTEM()
    {
        currentFile = new FILE_NODE("root", NULL, true);
    }

    ~FILE_SYSTEM()
    {
        delete currentFile;
    }

    //allows to switch the current node to a node with the specified name
    void traverse(std::string name)
    {
        if( currentFile->getInstance(name) != NULL
         && currentFile->getInstance(name)->isDirectory() )
        {
            currentFile = currentFile->getInstance(name);
        }
        else
        {
            std::cout << "cd: " << name << ": Not a directory" << std::endl;
        }
    }

    //change the user permission for node with given name
    void changeUserPerm(std::string name, uint8_t permission)
    {
        if( currentFile->getInstance(name) != NULL)
        {
        currentFile->getInstance(name)->changeUserPermission(permission);
        }
        else
        {
        std::cout << "chmod: " << name << ": Not found" << std::endl;
        }
    }

    //change the group permission for node with given name
    void changeGroupPerm(std::string name, uint8_t permission)
    {
        if( currentFile->getInstance(name) != NULL)
        {
            currentFile->getInstance(name)->changeGroupPermission(permission);
        }
        else
        {
            std::cout << "chmod: " << name << ": Not found" << std::endl;
        }
    }

    //change the group permission for node with given name
    void changeWorldPerm(std::string name, uint8_t permission)
    {
        if( currentFile->getInstance(name) != NULL)
	    {
	        currentFile->getInstance(name)->changeWorldPermission(permission);
	    }   
        else
        {
            std::cout << "chmod: " << name << ": Not found" << std::endl;
        }
    }

    //prints the working directory for node we are currently at
    std::string workingDirectory()
    {
        return currentFile->getDirectory();
    }

    //prints names of all child nodes of node we are currently at
    std::string listLocalNodes()
    {
        return currentFile->getLocalDirectory();
    }

    //prints attributes of all child nodes of node we are currently at
    //mimics ls -l output
    std::vector <std::string> longListLocalNodes()
    {
        return currentFile->getLocalDirectoryLong();
    }

    //adds a node with name as a child to current node
    //or if a node with name exits, update the timestamp
    FILE_NODE *addNode(std::string name, bool isDir)
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

    //deletes a node with name which is a child to current node
    //only works if we specified we wanted to delete the node type
    //which matches the name (folder or file)
    void deleteNode(std::string name, bool deleteDir)
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
                    std::cout << "rm: " << name << ": Not a file" << std::endl;
                }
                else if(!currentFile->getInstance(name)->isDirectory() && deleteDir)
                {
                    std::cout << "rmdir: " << name << ": Not a directory" << std::endl;
                }
            }
        }
        else
        {
            std::cout << "rmdir: " << name << ": Not found" << std::endl;
        }
    }

};
