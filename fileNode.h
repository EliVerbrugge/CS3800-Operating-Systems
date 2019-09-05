
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

        //construcutor: will assign a name, and timestamp to a node
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
            strftime(dt_string, 50, "%B %d ,R", curr_tm);
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

        //will print out the working directory of this node by grabbing the parent
        //node until the nodes no longer have parents and appending the names
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

        //formats all the node attributes in the expected format for ls -l
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
        
        //prints all child node names with spacing
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

        //prints all child nodes with the expected format for ls -l
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

        //checks if the current node has a direct association with the request node
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

        //creates a child node with name and flag to determine whether it 
        //is a folder or file
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

        //grabs the machine local time and formats it
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

        //returns whether the node is a directory
        bool isDirectory()
        {
            return isDir;
        }

        //deletes a node, this could potentially delete itself so tread
        //with care
        void deleteItem(std::string name)
        {
            delete fileMap[name];
            fileMap.erase(name);
            return;
        }

        //changes the user permission flag on this file
        void changeUserPermission(uint8_t permission)
        {
            userPermission = permission;    
            return;
        }

        //changes the group permission flag on this file
        void changeGroupPermission(uint8_t permission)
        {
            groupPermission = permission;    
            return;
        }

        //changes the world permission flag on this file
        void changeWorldPermission(uint8_t permission)
        {
            worldPermission = permission;    
            return;
        }


        //returns the node name
        std::string getName()
        {
            return name;
        }

        //returns the node timestamp
        std::string getTimestamp()
        {
            return timestamp;
        }

        //returns the node owner
        std::string getOwner()
        {
            return owner;
        }

        //returns the node group
        std::string getGroup()
        {
            return group;
        }

        //returns the node user permission
        uint8_t getUserPermission()
        {
            return userPermission;
        }

        //returns the node group permission
        uint8_t getGroupPermission()
        {
            return groupPermission;
        }

        //returns the node worled permission
        uint8_t getWorldPermission()
        {
            return groupPermission;
        }

        //returns the node size
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
