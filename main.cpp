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
            delete this->getInstance(name);
            fileMap.erase(name);
            std::cout <<; 
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
    Test.deleteItem("nudes");
    std::cout << Nudes.getDirectory() << std::endl;


}

