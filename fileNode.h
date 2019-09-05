
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

            char dt_string[100];
            time_t curr_time;
	        tm * curr_tm;

            time(&curr_time);
            curr_tm = localtime(&curr_time);
            strftime(dt_string, 50, "%B %d %R", curr_tm);
            timestamp = std::string(dt_string);
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

        std::string formatLongList()
        {
            std::string output = "";
            output += isDir ? "d" : "-";
            uint8_t permissions[3] = {userPermission, groupPermission, worldPermission};
            for(int i = 0; i < 3; i++)
            {
                //read mask
                if(permissions[i] & 0b100)
                {
                    output+="r";
                }
                else
                {
                    output+="-";
                }
                //write mask
                if(permissions[i] & 0b010)
                {
                    output+="w";
                }
                else
                {
                    output+="-";
                }
                //execute mask
                if(permissions[i] & 0b001)
                {
                    output+="x";
                }
                else
                {
                    output+="-";
                }
            }
            output += " " + owner;
            output += " " + group;
            output += " " + std::to_string(getSize());
            output += " " + timestamp;
            output += " " + name;
            return output;

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

        std::vector <std::string> getLocalDirectoryLong()
        { 
            std::vector <std::string> longList; 
            std::map<std::string, FILE_NODE *>::iterator it = fileMap.begin();   

            while (it != fileMap.end())
            {
                if(it->first != ".." && it->first != ".")
                {
                    longList.push_back((it->second)->formatLongList());
                }
               
                it++;
            }
            return longList;
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
            char dt_string[100];
            time_t curr_time;
	        tm * curr_tm;

            time(&curr_time);
            curr_tm = localtime(&curr_time);
            strftime(dt_string, 50, "%B %d %R", curr_tm);
            timestamp = std::string(dt_string);
            return this;
        }

        bool isDirectory()
        {
            return isDir;
        }

        void deleteItem(std::string name)
        {
            delete fileMap[name];
            fileMap.erase(name);
            return;
        }

        void changeUserPermission(uint8_t permission)
        {
            userPermission = permission;    
            return;
        }

        void changeGroupPermission(uint8_t permission)
        {
            groupPermission = permission;    
            return;
        }

        void changeWorldPermission(uint8_t permission)
        {
            worldPermission = permission;    
            return;
        }


        std::string getName()
        {
            return name;
        }

        std::string getTimestamp()
        {
            return timestamp;
        }

        std::string getOwner()
        {
            return owner;
        }

        std::string getGroup()
        {
            return group;
        }

        uint8_t getUserPermission()
        {
            return userPermission;
        }

        uint8_t getGroupPermission()
        {
            return groupPermission;
        }

        uint8_t getWorldPermission()
        {
            return groupPermission;
        }

        uint32_t getSize()
        {
            return sizeof(this) + sizeof(fileMap);
        }



    private:
        std::string name;
        bool isDir;
        std::string timestamp;

        uint8_t userPermission  = 7;
        uint8_t groupPermission = 6;
        uint8_t worldPermission = 4;

        std::string owner = "root";
        std::string group = "root";



};
