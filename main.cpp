#include <iostream>
#include <map> 
#include <exception>
#include <vector>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <time.h> 

struct FILE_NODE
{
    public:
        std::map<std::string, FILE_NODE *> fileMap;
        FILE_NODE(std::string filename, FILE_NODE * parent, bool isdir)
        {

            name = filename;
            fileMap[".."] = parent;
            fileMap["."] = this;
            isDir = isdir;
            time_t my_time = time(NULL); 
            timestamp = ctime(&my_time);
        }

        ~FILE_NODE()
        {
            std::map<std::string, FILE_NODE *>::iterator it = fileMap.begin();   

            while (it != fileMap.end())
            {
                if(it->first != ".." && it->first != ".")
                {
                    delete it->second;
                }
               
                it++;
            }
        }

        std::string getName()
        {
            return name;
        }

        std::string getDirectory()
        {
            std::string directory = "";
            FILE_NODE *currentNode = this;
            
            while(currentNode->getInstance("..") != NULL)
            {
                directory.insert(0, "/" + currentNode->name);
                currentNode = currentNode->getInstance("..");
            }

            directory.insert(0, "/" + currentNode->name);
            return directory;
        } 

        std::string getLocalDirectory()
        { 
            std::string localDirectory;
            std::map<std::string, FILE_NODE *>::iterator it = fileMap.begin();   

            while (it != fileMap.end())
            {
                if(it->first != ".." && it->first != ".")
                {
                    localDirectory.append((it->second)->name + "  ");
                }
               
                it++;
            }
            return localDirectory;
        }

        FILE_NODE *getInstance( std::string name)
        {
            if(name == ".")
            {
                return this;
            }
            else if(fileMap.count(name) != 0)
            {
                return fileMap[name];
            }

            return NULL;
        }

        FILE_NODE *createInstance( std::string name, bool isdir)
        {
            if(name == ".." || name == ".")
            {
                std::cout << "'..' or '.' are not valid filenames" << std::endl;
                return NULL;
            }
            FILE_NODE *instance = new FILE_NODE(name, this, isdir);
            fileMap[name] = instance;

            return instance;
        }

        FILE_NODE *updateTimeStamp()
        {
            time_t my_time = time(NULL); 
            timestamp = ctime(&my_time);
            return this;
        }

        std::string getTimestamp()
        {
            return timestamp;
        }

        bool isDirectory()
        {
            return isDir;
        }

        void deleteItem(std::string name)
        {
            delete fileMap[name];
            fileMap.erase(name);
        }

    private:
        std::string name;
        bool isDir;
        std::string timestamp;
};

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

    std::string workingDirectory()
    {
        return currentFile->getDirectory();
    }

    std::string listLocalNodes()
    {
        return currentFile->getLocalDirectory();
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
        if( currentFile->getInstance(name)->isDirectory() && deleteDir
         || !currentFile->getInstance(name)->isDirectory() && !deleteDir )
        {
            currentFile->deleteItem(name);
        }
    }

};

std::vector<std::string> split(std::string str) 
{ 
    std::istringstream ss(str); 
    std::vector<std::string> commands;

    do { 
        std::string word; 
        ss >> word; 
        if(word != "")
            commands.push_back(word);
  
    } while (ss); 
    return commands;
} 

void handleCommands()
{
    /***********************************************
    Declaring Local Variables
    ***********************************************/
    FILE_SYSTEM files;
    bool quit;
    std::vector<std::string> commands;
    std::string command;
    std::string input;

    /***********************************************
    Iniatilizing Local Variables
    ***********************************************/
    quit = false;
    
    do
    {   
        std::cout << "root@user$ ";
        std::getline(std::cin, input);
        commands = split(input);
        command = commands[0];
        if(command == "pwd")
        {
            std::cout << files.workingDirectory() << std::endl;
        }
        else if( command == "ls" )
        {
            std::cout << files.listLocalNodes() << std::endl;
        }
        else if( command == "touch")
        {
            if(commands.size() > 1)
            {
                files.addNode(commands[1], false);
            }
        }
        else if( command == "mkdir")
        {
            if(commands.size() > 1)
            {
                files.addNode(commands[1], true);
            }
        }
        else if( command == "cd")
        {
            if(commands.size() > 1)
            {
                files.traverse(commands[1]);
            }
        }
        else if( command == "rm")
        {
            if(commands.size() > 1)
            {
                files.deleteNode(commands[1], false);
            }
        }
        else if( command == "rmdir")
        {
            if(commands.size() > 1)
            {
                files.deleteNode(commands[1], true);
            }
        }
        else if( command == "quit" 
              || command == "q"
              || command == "exit" )
        {
            quit = true;
        }
        
    }while(!quit);
}

int main(int argc, char *argv[])
{
    handleCommands();

    return 0;
}

