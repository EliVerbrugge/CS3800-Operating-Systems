#include "PersistentFiles.h"
#include "fileSystem.h"
String command;
String commands[3];
FILE_SYSTEM filesys;
PersistentFiles PersistentFileStorage;
uint32_t timer;

void setup()
    {   
        Serial.begin(9600);
        while(!Serial.available());
        Serial.println("#  __      _____________.____   _________  ________     _____  ___________     #");  
        Serial.println("# /  \\   /  \\_   _____/|    |  \\_   ___ \\  \\_____ \\    /     \\  \\_   _____/    #");  
        Serial.println("# \\   \\/\\/  /|    __)_ |    |  /    \\  \\/  /   |   \\  /  \\ /  \\ |    __)_      #");  
        Serial.println("#  \\        /|        \\|    |__\\     \\____/    |    \\/    Y    \\|        \\     #");  
        Serial.println("#   \\__/\\  //_______  /|_______ \\______  /\\_______  /\\____|__  /_______  /     #");  
        Serial.println("#        \\/         \\/         \\/      \\/         \\/         \\/        \\/      #");  
        Serial.println("");
        timer = millis();
    }

void loop()
    {
        while (Serial.available()) 
        {
            char c = Serial.read();  
            commands[0] += c; 
            delay(2);  
        }

        //get rid of those pesky return characters
        commands[0].replace("\r", "");
        int space = commands[0].indexOf(" ");

        if(space != -1)
        {
            commands[1] = commands[0].substring(space+1, commands[0].length());
            commands[0] = commands[0].substring(0, space);
            int space2 = commands[0].indexOf(" ", space);
            if(space2 != -1)
            {
                commands[1] = commands[1].substring(0, space2);
                commands[2] = commands[1].substring(space2+1, commands[1].length());
            }
        }

        //get rid of those pesky return characters
        commands[0].replace("\r", "");
        
        if (timer > millis())
        {
            timer = millis();
        }

        if (millis() - timer > 1000) 
        {
            timer = millis(); 
            filesys.updateTick();
        }
        
    
        if (commands[0].length() > 0) 
        {   
            //print out the full command string properly
            if(commands[2] != "")
            {
                Serial.println(filesys.getCurrentUser()->username + "@USER ~ " + commands[0] + " " + commands[1]);
            }
            else if(commands[1] != "")
            {
                Serial.println(filesys.getCurrentUser()->username + "@USER ~ " + commands[0] + " " + commands[1]);
            }
            else
            {
                Serial.println(filesys.getCurrentUser()->username + "@USER ~ " + commands[0]);
            }
            
            if(commands[0] == "ls")
            {
                if(commands[1] == "-l")
                {
                    String* list = filesys.longListLocalNodes();

                    for(int i = 0; i < 10; i++)
                    {
                        Serial.println(list[i]);
                    }
                }
                else
                {
                    Serial.println(filesys.listLocalNodes());

                }
            }
            else if(commands[0] == "cd")
            {
                filesys.traverse(commands[1]);
            }
            else if(commands[0] == "touch")
            {
                filesys.addNode(commands[1], false);
            }
            else if(commands[0] == "rm")
            {
                filesys.deleteNode(commands[1], false);
            }
            else if(commands[0] == "mkdir")
            {
                filesys.addNode(commands[1], true);
            }
            else if(commands[0] == "rmdir")
            {
                filesys.deleteNode(commands[1], true);
            }
            else if(commands[0] == "users")
            {
                filesys.displayUsers();
            }
            else if(commands[0] == "useradd")
            {
                filesys.addUser(commands[1]);
            }
            else if(commands[0] == "userdel")
            {
                filesys.deleteUser(commands[1]);
            }
            else if(commands[0] == "chuser")
            {
                filesys.switchUser(commands[1]);
            }
            else if(commands[0] == "run")
            {
                filesys.addProcess(commands[1]);
            }
            else if(commands[0] == "kill")
            {
                filesys.deleteProcess(commands[1]);
            }
             else if(commands[0] == "ps")
            {
                filesys.displayRunningProcesses();
            }
            else if(commands[0] == "save")
            {
                PersistentFileStorage.resetFiles();
                FILE_NODE *root = filesys.getCurrentNode();
                PersistentFileStorage.serialize(root);
            }
            else if(commands[0] == "load")
            {
                FILE_NODE *node1 = new FILE_NODE("root", NULL, true);
                PersistentFileStorage.resetIndex();
                PersistentFileStorage.deSerialize(node1);
                node1 = node1->getInstance("root");
                filesys.switchTo(node1);
            }
            commands[0] = "";
            commands[1] = "";
            commands[2] = "";
        }
    }

