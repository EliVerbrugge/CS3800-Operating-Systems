#include "fileNode.h"

struct PROCESS
{
    String processName;
    uint8_t duration;
    uint8_t startTime;
    uint8_t scheduledTime;
    PROCESS()
    {
        processName = "invalid_process";
        duration = random(10, 50);
        startTime = millis()/1000;
        scheduledTime = 0;
    }
    PROCESS(String name)
    {
        processName = name;
        duration = random(10, 50);
        startTime = millis()/1000;
        scheduledTime = 0;
    }
}

struct USER
{
    String username;
    uint8_t dirPermission;
    uint8_t filePermission;
    USER()
    {
        username = "invalid_user";
    }
    USER(String name, uint8_t dir, uint8_t file)
    {
        username = name;
        dirPermission = dir;
        filePermission = file;
    }
};

struct FILE_SYSTEM
{
    FILE_NODE *currentFile;
    USER *userArray[10];
    PROCESS *processArray[20];
    PROCESS* runningProcess;
    uint8_t numUsers = 0;
    uint8_t currUser = 0;
    uint8_t currProcess = -1;
    FILE_SYSTEM()
    {
        currentFile = new FILE_NODE("root", NULL, true);
        userArray[numUsers] = new USER("verbruggeeli", 7, 7);
        numUsers++;
    }

    ~FILE_SYSTEM()
    {
        delete currentFile;
    }

    void updateTick()
    {
        if(currProcess == -1)
        {
            uint8_t min = 1000;
            for(int i = 0; i < 20; i++)
            {
                if(processArray[i]->processName != "invalid_process" && processArray[i]->duration < min)
                {
                    currProcess = i;
                    min = processArray[i]->duration
                }
            }
            if(currProcess == -1)
            {
                return;
            }     
        }

        processArray[i]->scheduledTime += 1;

        if(processArray[i]->duration - processArray[i]->scheduledTime == 0)
        {
            currProcess == -1
        }
    }

    void addProcess(String name )
    {
        userArray[numUsers] = new USER(name, 7, 3);
        numUsers++;
        return;
    }

    void deleteUser(String name )
    {
        int i;
        for(i = 1; i < numUsers; i++)
        {
            if(userArray[i]->username == name)
            {
                delete userArray[i];
                break;
            }
        }

        if (i < numUsers) 
        { 
            numUsers-=1;
            for (int j=i; j<numUsers; j++) 
                userArray[j] = userArray[j+1]; 
        } 
        return;
    }

    void switchTo(FILE_NODE* file)
    {
        currentFile = file;
        return;
    }

    //allows to switch the current node to a node with the specified name
    void traverse(String name)
    {
        if( currentFile->getInstance(name) != NULL
         && currentFile->getInstance(name)->isDirectory()
         && userArray[currUser]->dirPermission & 0b100 )
        {
            currentFile = currentFile->getInstance(name);
        }
        else if(!(userArray[currUser]->dirPermission & 0b100))
        {
            Serial.println("You do not have permission to access this file");
        }
        else
        {
            Serial.println("cd: " + name + ": Not a directory");
        }
    }

    void displayUsers()
    {
        for(int i = 0; i < numUsers; i++)
        {
            Serial.println("-" + userArray[i]->username);
        }
    }

    void addUser(String name )
    {
        userArray[numUsers] = new USER(name, 7, 3);
        numUsers++;
        return;
    }

    void switchUser(String name )
    {
        for(int i = 0; i < numUsers; i++)
        {
            if(userArray[i]->username == name)
            {
                currUser = i;
            }
        }
        return;
    }

    void deleteUser(String name )
    {
        int i;
        for(i = 1; i < numUsers; i++)
        {
            if(userArray[i]->username == name)
            {
                delete userArray[i];
                break;
            }
        }

        if (i < numUsers) 
        { 
            numUsers-=1;
            for (int j=i; j<numUsers; j++) 
                userArray[j] = userArray[j+1]; 
        } 
        return;
    }

    USER* getCurrentUser()
    {
        return userArray[currUser];
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
