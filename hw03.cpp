// Zoe Dauphinee, homework 3, 
// Data Hiding; Encapsulation; Delegation; Overloading the Output Operator

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;


class Warrior {
    private:
        class Weapon {
            friend ostream& operator<<(ostream& os, const Weapon& weapon);
            public:
                Weapon(const string& name, int strength) 
                    : name(name), strength(strength) {}
                const string& get_weapon_name() const;
                int get_strength() const;
                void change_strength(int new_strength);
            private:
                string name;
                int strength;
        };

        string name;
        Weapon weapon;

    public:
        Warrior(const string& name, const string& weapon_name, int weapon_strength)
            : name(name), weapon(weapon_name, weapon_strength) {}
        const string& get_name() const;
        const string& get_weapon_name() const;
        int get_strength() const;
        void change_strength(int new_strength);
    friend ostream& operator<<(ostream& os, const Weapon& weapon);
    friend ostream& operator<<(ostream& os, const Warrior& warrior);
};


void open_file(ifstream& file);

void read_file(ifstream& file, vector<Warrior>& warriors);

void create_warrior(const string& name, const string& weapon_name, int strength, vector<Warrior>& warriors);

size_t get_warrior(const string& name, const vector<Warrior>& warriors);

void battle(Warrior& fighter1, Warrior& fighter2);

void print_status(const vector<Warrior>& warriors);


int main() {
    ifstream file;
    vector<Warrior> warriors;
    open_file(file);
    read_file(file, warriors);
    file.close();
}


/*
--- function implelemtations ---
*/

// opens file
void open_file(ifstream& file) {
    file.open("warriors.txt");
    // checks to make sure file opened correctly
    if(!file) {
        cerr << "failed to open warriors.txt";
        exit(1);
    }
}

// reads file line by line, calls appropriate function based on command
void read_file(ifstream& file, vector<Warrior>& warriors) {
    string command;
    string name;
    string weapon_name;
    string fighter1_name;
    string fighter2_name;
    int strength;
    while(file >> command) {
        // calls to create new Warrior
        if (command == "Warrior") {
            file >> name >> weapon_name >> strength;
            create_warrior(name, weapon_name, strength, warriors);
        }
        // finds the two Warriors from their names and calls Battle
        else if (command == "Battle") {
            file >> fighter1_name >> fighter2_name;
            size_t fighter1_index = get_warrior(fighter1_name, warriors);
            size_t fighter2_index = get_warrior(fighter2_name, warriors);
            if (fighter1_index == warriors.size()) {
                cout << fighter1_name << " does not exist\n";
            } 
            else if (fighter2_index == warriors.size()) {
                cout << fighter2_name << " does not exist\n";
            }
            else { 
                battle(warriors[fighter1_index], warriors[fighter2_index]); 
            }
        }
        // calls print_status
        else if (command == "Status") {
            print_status(warriors);
        }
    }
}

// creates a new Warrior
void create_warrior(const string& name, const string& weapon_name, int strength, vector<Warrior>& warriors) {
    // checks to see if a Warrior with that name already exists
    for (size_t index = 0; index < warriors.size(); ++index) {
        if (warriors[index].get_name() == name) {
            cout << "a warrior with that name already exists!" << endl;
            return;
        }
    }
    // creates new Warrior and adds it to the vector
    warriors.emplace_back(name, weapon_name, strength);
}

// returns the index of the Warrior with passed-in name
size_t get_warrior(const string& name, const vector<Warrior>& warriors) {
    // if the warrior is in the vector, return its index
    for (size_t index = 0; index < warriors.size(); ++index) {
        if (warriors[index].get_name() == name) {
            return index;
        }
    }
    // if not in the vector, return the size of the vector
    return warriors.size();
}

void battle(Warrior& fighter1, Warrior& fighter2) {
    cout << fighter1.get_name() << " battles " << fighter2.get_name() << endl;
    // both already dead, print but don't change strengths
    if (fighter1.get_strength() == 0 && fighter2.get_strength() == 0) {
        cout << "Oh, NO! They're both dead! Yuck!\n";
    }
    // fighter 1 has strength 0
    else if (fighter1.get_strength() == 0) {
        cout << "He's dead, " << fighter2.get_name() << endl;
    }
    // fighter 2 has strength 0
    else if (fighter2.get_strength() == 0) {
        cout << "He's dead, " << fighter1.get_name() << endl;
    }
    // both fighters have strength
    else {
        // fighter 1 defeats fighter 2, change strengths accordingly
        if (fighter1.get_strength() > fighter2.get_strength()) {
            cout << fighter1.get_name() << " defeats " << fighter2.get_name() << endl;
            int new_strength = fighter1.get_strength() - fighter2.get_strength();
            fighter1.change_strength(new_strength);
            fighter2.change_strength(0);
        }
        // fighter 1 defeats fighter 2, change strengths accordingly
        else if(fighter1.get_strength() < fighter2.get_strength()) {
            cout << fighter2.get_name() << " defeats " << fighter1.get_name() << endl;
            int new_strength = fighter2.get_strength() - fighter1.get_strength();
            fighter2.change_strength(new_strength);
            fighter1.change_strength(0);
        }
        // fighter 1 defeats fighter 2, change strengths accordingly
        else if (fighter1.get_strength() == fighter2.get_strength()) {
            cout << "Mutual Annihilation: " << fighter1.get_name() << " and " 
            << fighter2.get_name() << " die at each other's hands\n";
            fighter1.change_strength(0);
            fighter2.change_strength(0);
        }
    }
}

void print_status(const vector<Warrior>& warriors) {
    cout << "There are: " << warriors.size() << " warriors\n";
    for (size_t index = 0; index < warriors.size(); ++index) {
        cout << warriors[index] << endl;
    }
}


/*
--- Weapon method implelemtations ---
*/

// getter for Weapon name
const string& Warrior::Weapon::get_weapon_name() const {
    return name;
}

// getter for Weapon strength
int Warrior::Weapon::get_strength() const {
    return strength;
}

// change the Weapon strength
void Warrior::Weapon::change_strength(int new_strength) {
    strength = new_strength;
}

// Weapon output operator
ostream& operator<<(ostream& os, const Warrior::Weapon& weapon) {
    os << weapon.name << ", " << weapon.strength;
    return os;
}


/*
--- Warrior method implelemtations ---
*/

// getter for name
const string& Warrior::get_name() const {
    return name;
}

// calls the Weapon's get_name method
const string& Warrior::get_weapon_name() const { 
    return weapon.get_weapon_name(); 
}

// calls the Weapon's get_strength method
int Warrior::get_strength() const { 
    return weapon.get_strength(); 
}

// calls the Weapon's change_strength method
void Warrior::change_strength(int new_strength) { 
    weapon.change_strength(new_strength); 
}

// Warrior output operator
ostream& operator<<(ostream& os, const Warrior& warrior) {
    os << "Warrior: " << warrior.name << ", weapon: " << warrior.weapon;
    return os;
}