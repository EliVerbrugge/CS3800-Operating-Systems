#include "PersistentFiles.h"
#include "fileSystem.h"
void setup()
    {   
        Serial.begin(9600);
        PersistentFiles PersistentFileStorage;
        while(!Serial.available());
        FILE_SYSTEM filesys;
        Serial.println("#  __      _____________.____   _________  ________     _____  ___________     #");  
        Serial.println("# /  \\   /  \\_   _____/|    |  \\_   ___ \\  \\_____ \\    /     \\  \\_   _____/    #");  
        Serial.println("# \\   \\/\\/  /|    __)_ |    |  /    \\  \\/  /   |   \\  /  \\ /  \\ |    __)_      #");  
        Serial.println("#  \\        /|        \\|    |__\\     \\____/    |    \\/    Y    \\|        \\     #");  
        Serial.println("#   \\__/\\  //_______  /|_______ \\______  /\\_______  /\\____|__  /_______  /     #");  
        Serial.println("#        \\/         \\/         \\/      \\/         \\/         \\/        \\/      #");  
        Serial.println("");

        FILE_NODE *root = filesys.getCurrentNode();
        
        filesys.addNode("Corn", true);
        filesys.addNode("Help.txt", false);
        filesys.addNode("Me.png", false);

        String test = filesys.listLocalNodes();

        Serial.println(test);

        filesys.traverse("Corn");
        filesys.addNode("Word.doc", true);
        filesys.addNode("Prezzi.pptx", true);
        test = filesys.listLocalNodes();

        Serial.println(test);
        
        Serial.println("Resetting files");
        PersistentFileStorage.resetFiles();

        Serial.println("Serializing tree");
        PersistentFileStorage.serialize(root);

        Serial.println("Deserializing tree");
        FILE_NODE *node1 = NULL;
        PersistentFileStorage.resetIndex();
        PersistentFileStorage.deSerialize(node1);

        filesys = FILE_SYSTEM(node1);

        test = filesys.listLocalNodes();

        Serial.println(test);
        
    }

void loop()
    {

    }

