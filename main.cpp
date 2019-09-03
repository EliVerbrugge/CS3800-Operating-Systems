#include <iostream>
#include <map> 

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

int main()
{
    std::cout << "Hello" << std::endl;
    FILE_NODE root("root", NULL);
    std::cout << root.getName() << std::endl;
    FILE_NODE Desktop("desktop", &root);
    std::cout << Desktop.getDirectory() << std::endl;
    FILE_NODE Test("test", &Desktop);
    FILE_NODE Nudes("nudes", &Test);
    std::cout << Nudes.getDirectory() << std::endl;
    std::cout << Test.getLocalDirectory() << std::endl;
    Test.deleteItem("nudes");
    std::cout << Test.getLocalDirectory() << std::endl;


}

