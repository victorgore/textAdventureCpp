/*
Title: Alice.cpp
Description: This program is a text-based adventure game. The player is Alice, who has fallen into a rabbit hole and must find her way to wonderland.
Author: Victor Gore
Date: August 5, 2023
*/
/*
DOCUMENTATION
Program Purpose:
    This program is a text-based adventure game. The player is Alice, who has fallen into a rabbit hole and must find her way to wonderland.
    
Compile: (assuming Cygwin is running): g++ -o Alice Alice.cpp
Execution: (assuming Cygwin is running): ./Alice.exe

Classes: Location, Item, Character, Control, Game

Variables:
Item class:
    string name - name of the item

Character class:
    string name - name of the character
    string dialogue - dialogue with the character

Location class:
    string description - description of the room
    vector<Item> items - vector of items in the room
    map<string, Location*> directions - map of directions the player can move in the room
    vector<Character> characters - vector of characters in the room

Control class:
    string input - input from the user
    vector<string> words - vector of words from the user's input
    string command - the command the user wants to execute
    string argument - the argument for the command    
*/
/*
TEST PLAN
Normal case:
---------------------------------------------
You have fallen into a rabbit hole with many paths. The place is dimly lit and musty.
Available directions: north 
Characters in the room: 
---------------------------------------------
Enter a direction (north, east, south, or west), 'take <item>' to pick up an item, 'leave <item>' to leave an item, or 'talk <character>' to talk to a character: n
---------------------------------------------
You have entered the Tulgey Wood. It is dark and mysterious. You hear strange noises.
Available directions: east south west 
Characters in the room: 
---------------------------------------------
Enter a direction (north, east, south, or west), 'take <item>' to pick up an item, 'leave <item>' to leave an item, or 'talk <character>' to talk to a character: west
---------------------------------------------
This is the croquet ground. The Queen of Hearts is playing croquet. With the other characters
Available directions: east 
Characters in the room: hatter dutchess queen rabbit 
---------------------------------------------
Enter a direction (north, east, south, or west), 'take <item>' to pick up an item, 'leave <item>' to leave an item, or 'talk <character>' to talk to a character: talk hatter
You may have thought I was going to give you a hint, I'm just here to say: I love my hats! I have a hat for every occasion.
---------------------------------------------
This is the croquet ground. The Queen of Hearts is playing croquet. With the other characters
Available directions: east 
Characters in the room: hatter dutchess queen rabbit 
---------------------------------------------
Enter a direction (north, east, south, or west), 'take <item>' to pick up an item, 'leave <item>' to leave an item, or 'talk <character>' to talk to a character: 



Bad Case(1) - Character not in room:
---------------------------------------------
Enter a direction (north, east, south, or west), 'take <item>' to pick up an item, 'leave <item>' to leave an item, or 'talk <character>' to talk to a character: talk queen
Character not found in the room.
---------------------------------------------
You have entered the Tulgey Wood. It is dark and mysterious. You hear strange noises.
Available directions: east south west 
Characters in the room: 
---------------------------------------------
Enter a direction (north, east, south, or west), 'take <item>' to pick up an item, 'leave <item>' to leave an item, or 'talk <character>' to talk to a character: 

Bad Case(2) - Item not in room:
Enter a direction (north, east, south, or west), 'take <item>' to pick up an item, 'leave <item>' to leave an item, or 'talk <character>' to talk to a character: take hat
Item not found in the room.

Bad Case (3) - Invalid direction:
Enter a direction (north, east, south, or west), 'take <item>' to pick up an item, 'leave <item>' to leave an item, or 'talk <character>' to talk to a character: n
Invalid direction. Please enter north, east, south, or west.
---------------------------------------------
This is the croquet ground. The Queen of Hearts is playing croquet. With the other characters
Available directions: east 
Characters in the room: hatter dutchess queen rabbit 

*/


#include <iostream> // input/output stream
#include <string> // string library (getline, etc...)
#include <map> // map library (used for rooms)
#include <vector> // vector library (used for items)
#include <algorithm> //algorithm library (for transform, find_if)
#include <fstream> // file stream

using namespace std; // standard namespace

/*
Item class represents an item in the game
*/
class Item {
public:
    Item(const string& name) : name(name) {} // Constructor

    string getName() const {
        return name; // Returns the name of the item
    }

private:
    string name; // Name of the item
};

// Class representing a character
class Character {
public:
    // Constructors for the Character class
    Character(const string& name, const string& dialogue)
        : name(name), dialogue(dialogue) {}

    string getName() const {
        return name; // Returns the name of the character
    }

    string getDialogue() const {
        return dialogue; // Returns the dialogue with the character
    }

private:
    string name;
    string dialogue;
};

/*
This class represents a location, or room, in the game
*/
class Location {
public:
    // Constructor for the Location class
    Location(const string& description)
        : description(description) {}

    // direction is the direction the player wants to move
    void addDirection(const string& direction, Location* nextLocation) {
        directions[direction] = nextLocation; // on the map, the direction is mapped to the next location
    }

    // itemName is the name of the item to add to the room
    void addItem(const string& itemName) {
        items.push_back(Item(itemName)); // if the item is in the room, add it to the vector of items
    }

    // name is the name of the character to add to the room
    void addCharacter(const string& name, const string& dialogue) {
        characters.emplace_back(name, dialogue); // if the character is in the room, add it to the vector of characters along with their dialogue
    }

    //Displays the room's description, items, and directions that the player can move
    void display() const {
        cout << "---------------------------------------------\n";
        cout << description << endl;
        if (!items.empty()) {
            cout << "You see the following items in the room:\n";
            for (const auto& item : items) { // for each item in the vector of items
                cout << "- " << item.getName() << endl; // print the item
            }
        }
        cout << "Available directions: ";
        for (const auto& direction : directions) { // for each direction in the map of directions
            cout << direction.first << " "; // print the direction
        }
        cout << endl;
        cout << "Characters in the room: ";
        for (const auto& character : characters) { // for each character in the vector of characters
            cout << character.getName() << " "; // print the character
        }
        cout << endl; // new line
        cout << "---------------------------------------------\n";
    }

    /*
    Move is used to move the player to the next location
    */
    Location* move(const string& direction, const vector<Item>& inventory) const {
    auto it = findDirection(direction); // find the direction in the map of directions
    if (it != directions.end()) {
        return it->second;
    } else {
        cout << "Invalid direction. Please enter north, east, south, or west.\n";
        return nullptr;
    }
}
  
    /*
    Used to take an item from the room and add it to the player's inventory
    */
    bool takeItem(const string& itemName, vector<Item>& inventory) {
        auto it = find_if(items.begin(), items.end(), [&](const Item& item) { // find the item in the vector of items
            return item.getName() == itemName; // if the item is found, return the item
        });

        if (it != items.end()) { // if the item is in the room
            inventory.push_back(*it); // add the item to the player's inventory
            items.erase(it); // remove the item from the room
            return true;
        }

        return false; // if the item is not in the room, return false
    }

    /*
    Used to leave an item in the room and remove it from the player's inventory
    */
    bool leaveItem(const string& itemName, vector<Item>& inventory) {
        auto it = find_if(inventory.begin(), inventory.end(), [&](const Item& item) { // find the item in the player's inventory
            return item.getName() == itemName; // if the item is found, return the item
        });

        if (it != inventory.end()) { // if the item is in the player's inventory
            items.push_back(*it); // add the item to the room
            inventory.erase(it); // remove the item from the player's inventory
            return true;
        }

        return false;
    }

    const vector<Character>& getCharacters() const {
        return characters;
    }

private:
    map<string, Location*>::const_iterator findDirection(const string& direction) const {
        // Convert the input direction to lowercase for case-insensitive comparison
        string lowerDirection = direction;
        transform(lowerDirection.begin(), lowerDirection.end(), lowerDirection.begin(), ::tolower);

        // Check if the input direction matches any full direction or single-letter abbreviation
        for (auto it = directions.begin(); it != directions.end(); ++it) {
            string lowerKey = it->first;
            transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(), ::tolower);
            if (lowerDirection == lowerKey || (lowerDirection.length() == 1 && lowerDirection[0] == lowerKey[0])) {
                return it;
            }
        }

        return directions.end();
    }

    /*
    The description of the room, items in the room, characters in the room, and directions the player can move
    */
    string description; 
    vector<Item> items;
    vector<Character> characters;
    map<string, Location*> directions;
};

/*
Control class conttols where the player is in the game and what they can do
*/
class Control {
public:
    Control() {
        // Read instructions from the file
        readInstructions();
        
        /*
        Create the locations in the game
        */
        hole = Location("You have fallen into a rabbit hole with many paths. The place is dimly lit and musty.");
        tulgey = Location("You have entered the Tulgey Wood. It is dark and mysterious. You hear strange noises.");
        dutchess_house = Location("You have found the dutchess house. The dutchess has some valuable advice, but she is at a croquet game.");
        pool = Location("You have found the pool of tears, the caterpillar is the only one here");
        wonderland = Location("You have made it to wonderland. You see a large castle in the distance. You will need to come back with the correct item to enter the castle.");
        safe_room = Location("This is the safe room. You can take an item or leave an item, but you can only carry one item at a time.");
        croquet_ground = Location("This is the croquet ground. The Queen of Hearts is playing croquet. With the other characters");

        /*
        Locations are connected based on a (north, south, east, west) direction 
        */
        // in the hole there is a path north to the tulgey wood
        hole.addDirection("north", &tulgey);

        // in the tulgey wood there is a path west to the croquet ground, south to the hole, east to the dutchess' house
        tulgey.addDirection("south", &hole);
        tulgey.addDirection("east", &dutchess_house);
        tulgey.addDirection("west", &croquet_ground);

        // in the dutchess' house there is a path west to the tulgey wood, east to wonderland, south to the pool
        dutchess_house.addDirection("west", &tulgey);
        dutchess_house.addDirection("east", &wonderland);
        dutchess_house.addDirection("south", &pool);

        // in the pool there is a path north to the dutchess' house
        pool.addDirection("north", &dutchess_house);

        // in wonderland there is a path west to the dutchess' house
        wonderland.addDirection("west", &dutchess_house);

        // in the croquet ground there is a path east to the tulgey wood
        croquet_ground.addDirection("east", &tulgey);

        // in the dutchess' house there is a path north to the safe room
        dutchess_house.addDirection("north", &safe_room);

        // in the safe room there is a path south to the dutchess' house
        safe_room.addDirection("south", &dutchess_house);

        // These items are added to the safe room
        safe_room.addItem("key");
        safe_room.addItem("mushroom");
        safe_room.addItem("cookies");
        safe_room.addItem("bottle");
        safe_room.addItem("sword");
        safe_room.addItem("watch");
        safe_room.addItem("upelkuchen");
        safe_room.addItem("chronosphere");
        safe_room.addItem("hat");
        safe_room.addItem("gloves");
        safe_room.addItem("letter");
        safe_room.addItem("fan");

        // This character is added to the pool
        pool.addCharacter("caterpillar", "The Caterpillar: You will need to grab an item from the storage to get to where you need!");

        // These characters are added to the croquet ground
        croquet_ground.addCharacter("hatter", "You may have thought I was going to give you a hint, I'm just here to say: I love my hats! I have a hat for every occasion.");
        croquet_ground.addCharacter("dutchess", "The Dutchess: I am the Dutchess of hearts. Beware of the Queen of hearts, she is not as nice as she seems.");
        croquet_ground.addCharacter("queen", "I am the Queen of hearts; talk to me an you will be rewarded with a little surprise.");
        croquet_ground.addCharacter("rabbit", "I am the White rabbit! If you find the right person, you will be rewarded with a map");

        
        currentLocation = &hole; // The game starts in the hole
    }

    void startGame() {
        // Player inventory
        vector<Item> inventory;

        // Game loop
        while (true) {
            currentLocation->display(); // Display the current location

            // Check if player has won (The player needs the key in their inventory to enter wonderland)
            if (currentLocation == &wonderland && hasItem("key", inventory)) {
                cout << "You win!" << endl;
                exit(0);
            }

            // Player input
            string action;
            cout << "Enter a direction (north, east, south, or west), 'take <item>' to pick up an item, 'leave <item>' to leave an item, or 'talk <character>' to talk to a character: ";
            getline(cin, action);

            if (action.substr(0, 5) == "take ") { // Take an item
                if (!inventory.empty()) { // Check if the player is already carrying an item
                    cout << "You can only carry one item at a time. Leave the current item before taking another.\n";
                    continue;
                }

                string itemName = action.substr(5); // Get the item name
                bool success = currentLocation->takeItem(itemName, inventory); // Take the item
                if (!success) {
                    cout << "Item not found in the room.\n"; // Item not found
                }
            } else if (action.substr(0, 6) == "leave ") { // Leave an item
                string itemName = action.substr(6); // Get the item name
                bool success = currentLocation->leaveItem(itemName, inventory); // Leave the item
                if (!success) {
                    cout << "Item not found in the inventory.\n";
                }
            } else if (action.substr(0, 5) == "talk ") { // Talk to a character
                string characterName = action.substr(5); // Get the character name
                talkToCharacter(characterName); // Talk to the character
            } else {
                // Directly process the direction input
                Location* nextLocation = currentLocation->move(action, inventory); // Move to the next location
                if (nextLocation) { // If the next location is not null
                    currentLocation = nextLocation; // Move to the next location
                }
            }
        }
    }
/*
If the player talks to the queen of hearts, the game ends and they lose, but if they talk to the caterpillar, they are given a map
*/
void talkToCharacter(const string& characterName) {
    for (const auto& character : currentLocation->getCharacters()) {
        if (character.getName() == characterName) { // If the character is found in the room
            if (characterName == "queen") { // If the character is the queen of hearts
                cout << "Queen: You've stolen my memories, and now I'll take your life!" << endl;
                cout << "Game Over - The queen has killed you out of revenge for stealing her memories." << endl;
                exit(0); // The game ends
            } else if (characterName == "caterpillar") { // If the character is the caterpillar
                cout << character.getDialogue() << endl; // The caterpillar gives the player a map and advice
                openMapsFile(); // The map is opened
                return;
            } else {
                cout << character.getDialogue() << endl; // The character gives the player advice
                return;
            }
        }
    }
    cout << "Character not found in the room.\n"; // If the player tries to talk to a character that is not in the room
}

void openMapsFile() { // Function to open the maps file
    ifstream mapsFile("maps.txt"); // The maps file is opened
    if (mapsFile.is_open()) { // If the file is open
        string line; // The file is read line by line
        while (getline(mapsFile, line)) { 
            cout << line << endl;
        }
        mapsFile.close();
    } else {
        cout << "Unable to open maps.txt.\n"; // If the file is not open, an error message is displayed
    }
}

    // Function to read instructions from the file
    void readInstructions() {
        ifstream file("instructions.txt");
        if (file.is_open()) { // If the file is open
            string line; // The file is read line by line
            while (getline(file, line)) {
                instructions += line + "\n"; // The instructions are added to the string
            }
            file.close();
        } else {
            /*
            The default instructions are used if the file is not open, since the game needs instructions
            */
            cout << "Unable to open instructions.txt. Using default instructions.\n";
            instructions = "Welcome to the Game!\n";
            instructions += "Your goal is to find the key and reach the final room.\n";
            instructions += "You can move between rooms using the directions north, east, south, or west.\n";
            instructions += "Use 'take <item>' to pick up an item and 'leave <item>' to leave an item.\n";
            instructions += "You can also 'talk <character>' to interact with characters in the room.\n";
        }
    }

    // Function to get the instructions
    const string& getInstructions() const {
        return instructions; // The instructions are returned
    }

private:
    // Function to check if the player has an item
    bool hasItem(const string& itemName, const vector<Item>& inventory) {
        for (const auto& item : inventory) { // The player's inventory is checked
            if (item.getName() == itemName) { // If the item is found
                return true; 
            }
        }
        return false;
    }

    /*
    The following locations are created and a pointer to the current location is created
    */
    Location* currentLocation;
    Location hole{"You have fallen into a rabbit hole with many paths. The place is dimly lit and musty."};
    Location tulgey{"You have entered the Tulgey Wood. It is dark and mysterious. You hear strange noises."};
    Location dutchess_house{"You have found the dutchess house. The dutchess has some valuable advice, but she is at a croquet game."};
    Location pool{"You have found the pool of tears, the caterpillar is the only one here"};
    Location wonderland{"You have made it to wonderland. You see a large castle in the distance. You will need to come back with the correct item to enter the castle."};
    Location safe_room{"This is the safe room. You can take an item or leave an item, but you can only carry one item at a time."};
    Location croquet_ground{"This is the croquet ground. The Queen of Hearts is playing croquet. With the other characters"};
    string instructions;
};

/*
Game Class is used to run the game
*/
class Game {
public:
    /*
    Prints the instructions then starts the game
    */
    void run() {
        Control game;
        cout << game.getInstructions() << endl;
        game.startGame();
    }
};

int main() {
    Game game; 
    game.run(); // The game is run
    return 0;
}
