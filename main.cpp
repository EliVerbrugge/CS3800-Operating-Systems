#include <iostream>
#include <map> 
#include <exception>
#include <vector>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <time.h> 

#include "fileSystem.h"

//splits the input into words so we can actually parse commands
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

//handles all of the various commands and tells the filesystem what to do
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

        //parsing the various commands and letting the filesystem handle them
        if(command == "pwd")
        {
            std::cout << files.workingDirectory() << std::endl;
        }
        else if( command == "ls" )
        {
            if(commands.size() > 1 && commands[1] == "-l")
            {
                std::vector <std::string> output;
                output = files.longListLocalNodes();
                
                for (std::vector<std::string>::const_iterator i = output.begin(); i != output.end(); ++i)
                    std::cout << *i << std::endl;
            }
            else
            {
                std::cout << files.listLocalNodes() << std::endl;
            }
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
        else if( command == "chmod")
        {
            if(commands.size() > 1)
            {
                files.changeUserPerm(commands[2], commands[1][0]-'0');
                files.changeGroupPerm(commands[2], commands[1][1]-'0');
                files.changeWorldPerm(commands[2], commands[1][2]-'0');
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

