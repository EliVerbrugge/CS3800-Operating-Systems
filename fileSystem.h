
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


    std::string workingDirectory()
    {
        return currentFile->getDirectory();
    }

    std::string listLocalNodes()
    {
        return currentFile->getLocalDirectory();
    }

    std::vector <std::string> longListLocalNodes()
    {
        return currentFile->getLocalDirectoryLong();
    }

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
