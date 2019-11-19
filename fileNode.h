#ifndef FILE_NODE_H
#define FILE_NODE_H

#include "Energia.h"

template <typename T> 
struct PAIR { 
   String first; 
   T second;
}; 

struct FILE_NODE
{
    public:
        //make an array of pairs (string identifier and node) that is the size
        //of our limit per level + 1 for the parent
        static const uint8_t MAX_NUM_FILES = 10;

        PAIR<FILE_NODE*> fileArray[MAX_NUM_FILES+1];

        //construcutor: will assign a name, and timestamp to a node
        FILE_NODE(String filename, FILE_NODE * parent, bool isdir)
        {

            name = filename;

            PAIR<FILE_NODE*> parentNode;
            parentNode.first = "..";
            parentNode.second = parent;
            fileArray[0] = parentNode;
            currentIndex++;

            PAIR<FILE_NODE*> thisNode;
            thisNode.first = ".";
            thisNode.second = this;
            fileArray[1] = thisNode;
            currentIndex++;

            isDir = isdir;

            //hardcoding the timestamp for now, we can't get an accurate one in any case.
            timestamp = "Wed. Nov 18 18:58:00";
        }

        ~FILE_NODE()
        {
            for(int i = 0; i < currentIndex; i++)
            {
                PAIR<FILE_NODE*> it = fileArray[i];
                if(it.first != ".." && it.first != ".")
                {
                    delete it.second;
                }
            }
        }

        //will print out the working directory of this node by grabbing the parent
        //node until the nodes no longer have parents and appending the names
        String getDirectory()
        {
            String directory = "";
            FILE_NODE *currentNode = this;
            
            while(currentNode->getInstance("..") != NULL)
            {
                directory = "/" + currentNode->name + directory;
                currentNode = currentNode->getInstance("..");
            }

            directory = "/" + currentNode->name + directory;
            return directory;
        } 

        //formats all the node attributes in the expected format for ls -l
        String formatLongList()
        {
            String output = "";
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
            output += " " + getSize();
            output += " " + timestamp;
            output += " " + name;
            return output;

        }
        
        //prints all child node names with spacing
        String getLocalDirectory()
        { 
            String localDirectory;
            for(int i = 0; i < currentIndex; i++)
            {
                PAIR<FILE_NODE*> it = fileArray[i];
                if(it.first != ".." && it.first != ".")
                {
                    localDirectory += ((it.second)->name + "  ");
                }
            }
            return localDirectory;
        }

        //prints all child nodes with the expected format for ls -l
       String* getLocalDirectoryLong()
        { 
            String* longList = new String[MAX_NUM_FILES];
            for(int i = 1; i < currentIndex; i++)
            {
                PAIR<FILE_NODE*> it = fileArray[i];
                if(it.first != "..")
                {
                   longList[i] = (it.second)->formatLongList();
                }
            }
            return longList;
        }

        //checks if the current node has a direct association with the request node
        FILE_NODE *getInstance( String name)
        {
            for(int i = 1; i < currentIndex; i++)
            {
                PAIR<FILE_NODE*> it = fileArray[i];
                if(it.first == name)
                {
                    return it.second;
                }
            }
            return NULL;
        }

        //creates a child node with name and flag to determine whether it 
        //is a folder or file
        FILE_NODE *createInstance( String name, bool isdir)
        {
            if(name == ".." || name == ".")
            {
                Serial.println("'..' or '.' are not valid filenames");
                return NULL;
            }
            FILE_NODE *instance = new FILE_NODE(name, this, isdir);

            PAIR<FILE_NODE*> newNode;
            newNode.first = name;
            newNode.second = instance;

            fileArray[currentIndex] = newNode;
            currentIndex++;
            return instance;
        }

        //grabs the machine local time and formats it
        FILE_NODE *updateTimeStamp()
        {

            timestamp = "Thursday. Nov 19 05:17:46";
            return this;
        }

        //returns whether the node is a directory
        bool isDirectory()
        {
            return isDir;
        }

        //deletes a node, this could potentially delete itself so tread
        //with care
        void deleteItem(String name)
        {
           int i;
           for(i = 1; i < currentIndex; i++)
            {
                PAIR<FILE_NODE*> it = fileArray[i];
                if(it.first == name)
                {
                    delete it.second;
                    break;
                }
            }

            if (i < currentIndex) 
            { 
                currentIndex--;
                for (int j=i; j<currentIndex; j++) 
                    fileArray[j] = fileArray[j+1]; 
            } 
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
        String getName()
        {
            return name;
        }

        //returns the node timestamp
        String getTimestamp()
        {
            return timestamp;
        }

        //returns the node owner
        String getOwner()
        {
            return owner;
        }

        //returns the node group
        String getGroup()
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
            return sizeof(this) + sizeof(fileArray);
        }



    private:
        String name;
        uint8_t currentIndex = 0;
        bool isDir;
        String timestamp;

        uint8_t userPermission  = 7;
        uint8_t groupPermission = 6;
        uint8_t worldPermission = 4;

        String owner = "root";
        String group = "root";

};
#endif