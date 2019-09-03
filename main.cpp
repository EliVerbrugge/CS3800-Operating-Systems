#include <iostream>
#include <map> 
#include <exception>
#include <vector>
#include <algorithm>
class invalidfilename: public std::exception
{
  virtual const char* what() const throw()
  {
    return "'..' or '.' are not valid filenames";
  }
} invalfilename;


struct FILE_NODE
{
    public:
        std::map<std::string, FILE_NODE *> fileMap;
        FILE_NODE(std::string filename, FILE_NODE * parent)
        {
            name = filename;
            fileMap[".."] = parent;
            fileMap["."] = this;

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

        FILE_NODE *createInstance( std::string name)
        {
            if(name == ".." || name == ".")
            {
                throw invalfilename;
                return NULL;
            }
            FILE_NODE *instance = new FILE_NODE(name, this);
            fileMap[name] = instance;
            return instance;
        }

        bool deleteItem(std::string name)
        {
            delete fileMap[name];
            fileMap.erase(name);
            return true;
        }
    private:
        std::string name;
        bool isDir;
};

struct FILE_SYSTEM
{
    FILE_NODE *currentFile;
    FILE_SYSTEM()
    {
        currentFile = new FILE_NODE("root", NULL);
    }

    void traverse(std::string name)
    {
        currentFile = currentFile->getInstance(name);
    }

    std::string workingDirectory()
    {
        return currentFile->getDirectory();
    }

    std::string listLocalNodes()
    {
        return currentFile->getLocalDirectory();
    }

    FILE_NODE *addNode(std::string name)
    {
        return currentFile->createInstance(name);
    }

};


std::vector<std::string> split(std::string &s, std::string delim) {
    std::vector<std::string> commands;
    std::string command;
    size_t pos;
    while ((pos = s.find(delim)) != std::string::npos) 
    {
            command = s.substr(0, pos);
            commands.push_back(command);
            s.erase(0, pos + delim.length());
    }
    return commands;
}

void handleCommands(int argc, char *argv[])
{
    /***********************************************
    Declaring Local Variables
    ***********************************************/
    FILE_SYSTEM files;
    bool quit;
    ptrdiff_t index;
    std::vector<std::string> commands;
    std::string command;
    std::string input;

    /***********************************************
    Iniatilizing Local Variables
    ***********************************************/
    std::vector<std::string> mylist{"pwd", "cd", "ls"};
    quit = false;
    index = -1;
    
    
    std::cout << files.workingDirectory() << std::endl;
    files.addNode("Desktop");
    files.traverse("Desktop");
    std::cout << files.workingDirectory() << std::endl;

    do
    {   
        std::getline(std::cin, input);
        if(input == "pwd")
        {
            std::cout << files.workingDirectory() << std::endl;
        }
        else
        {
            commands = split(input, " ");
            for (std::vector<std::string>::const_iterator i = commands.begin(); i != commands.end(); ++i)
                std::cout << *i << ' ';
            
            std::cout << "got here" << std::endl;
                    std::cout << commands.size() << std::endl;

            command = commands.at(0);
            //index = std::find(mylist.begin(), mylist.end(), "a") - mylist.end();
            std::cout << "got here" << std::endl;
        }
        
    }while(!quit);
}

int main(int argc, char *argv[])
{
    handleCommands(argc, argv);

    return 0;
}

