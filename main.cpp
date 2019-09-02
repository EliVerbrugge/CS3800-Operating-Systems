#include <iostream>
#include <map> 

struct FILE_NODE
{
    public:
        std::string name;
        FILE_NODE(std::string filename, FILE_NODE * parent)
        {
            name = filename;
            fileMap[".."] = parent;

        }
    private:
        std::map<std::string, FILE_NODE *> fileMap;
        bool isDir;
};

int main()
{
    std::cout << "Hello" << std::endl;
    FILE_NODE root("root", NULL);
    std::cout << me.name << std::endl;
    FIL
    
}

