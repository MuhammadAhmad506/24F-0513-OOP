
#include "GameHeader.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <windows.h> 
#include <conio.h>  
#include <fstream>

using namespace std;


Position::Position() : x(0), y(0) {}

void clearScreen() {
	system("cls");
}

void Position::NewPosition(int newX, int newY) {
    x = newX;
    y = newY;
}


GameMap::GameMap() {
    Initialize_GameMap();
}

void GameMap::Initialize_GameMap() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 70; j++) {
            arr[i][j] = ' ';
        }
    }
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 70; j++) {
            arr[0][j] = '-';
            arr[19][j] = '-';
            arr[i][0] = '|';
            arr[i][69] = '|';
        }
    }
}

void GameMap::Print_GameMap() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 70; j++) {
            cout << arr[i][j];
        }
        cout << endl;
    }
}

char GameMap::getCell(int x, int y) const {
    if (x >= 0 && x < 20 && y >= 0 && y < 70) {
        return arr[x][y];
    }
    return ' ';
}

void GameMap::setCell(int x, int y, char value) {
    if (x >= 0 && x < 20 && y >= 0 && y < 70) {
        arr[x][y] = value;
    }
}

// Resource implementation
Resource::Resource(const string& name, int amount) : name(name), amount(amount) {}

string Resource::getName() const {
    return name;
}

int Resource::getAmount() const {
    return amount;
}

void Resource::setAmount(int newAmount) {
    amount = newAmount;
}

void Resource::change(int delta) {
    amount += delta;
    if (amount < 0) amount = 0;
}

void Resource::display() const {
    cout << name << ": " << amount << endl;
}

// Economy implementation
Economy::Economy() : healthOfEconomy(100), amount(10000), taxAmount(0) {}

void Economy::Set_TaxAmount(int a) {
    taxAmount = a;
}

int Economy::Get_TaxAmount() const {
    return taxAmount;
}

void Economy::Econ_Health_Checker() {
    if (healthOfEconomy <= 50 && healthOfEconomy > 0) {
        cout << "Economy in danger: " << healthOfEconomy << "% health left" << endl;
    }
    else if (healthOfEconomy <= 0) {
        cout << "Your kingdom's economy has collapsed due to inflation!" << endl;
        Sleep(2000);
    }
}

void Economy::Display_Economy() const {
    cout << "Economy Health: " << healthOfEconomy << "% Amount: " << amount << endl;
}

void Economy::IncreaseEcon(int a) {
    amount += a;
}

int Economy::Get_Amount() const {
    return amount;
}



// ResourceManager implementation


ResourceManager::~ResourceManager() {
    for (int i = 0; i < MAX_RESOURCES; i++) {
        delete resources[i];
    }
}

void ResourceManager::addResource(int index, int amount) {
    if (index >= 0 && index < MAX_RESOURCES) {
        resources[index]->change(amount);
    }
}

bool ResourceManager::removeResource(int index, int amount) {
    if (index >= 0 && index < MAX_RESOURCES) {
        if (resources[index]->getAmount() >= amount) {
            resources[index]->change(-amount);
            return true;
        }
    }
    return false;
}

Resource* ResourceManager::getResource(int index) const {
    if (index >= 0 && index < MAX_RESOURCES) {
        return resources[index];
    }
    return nullptr;
}

void ResourceManager::displayResources() const {
    cout << "=== Resources ===" << endl;

    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (resources[i] != nullptr) {
            resources[i]->display();
        }
        else {
            cout << "Resource " << i + 1 << " is not available." << endl;
        }
        resources[i]->display();
    }
}



// Person implementation
Person::Person() : resentment_level(0), symbol(' ') {}

void Person::setName(const string& n) {
    name = n;
}

string Person::getName() const {
    return name;
}

void Person::setSymbol(char s) {
    symbol = s;
}

char Person::getSymbol() const {
    return symbol;
}

int Person::getResentmentLevel() const {
    return resentment_level;
}

void Person::increaseResentment(int amount) {
    resentment_level += amount;
}

// King implementation
King::King() {
    setSymbol('K');
    setName("King");
}

void King::Impose_Tax(int a, bool everyone, Kingdom* kingdom) {
    Set_TaxAmount(a);
    int tax = Get_TaxAmount();
    int excess = tax - 1000;
    int econCrippleFactor = everyone ? 10 : 5;

    if (excess > 0) {
        int lossOfHp = (excess / 500) * econCrippleFactor;
        healthOfEconomy -= lossOfHp;
        if (healthOfEconomy < 0) healthOfEconomy = 0;
    }
    if (excess < 0) {
        healthOfEconomy += 10;
        if (healthOfEconomy > 100) healthOfEconomy = 100;
    }

    Econ_Health_Checker();
    if (!everyone) {
        kingdom->getNobles()->increaseResentment(5);
        cout << "Nobles bear resentment towards the king." << endl;
        Sleep(2000);
    }
}

void King::spawn(GameMap& map, int x, int y) {
    NewPosition(x, y);
    map.setCell(x, y, getSymbol());
}

// Nobles implementation
Nobles::Nobles() {
    setSymbol('N');
    setName("Nobles");
}

void Nobles::spawn(GameMap& map, int x, int y) {
    NewPosition(x, y);
    map.setCell(x, y, getSymbol());
}

// Locals implementation
Locals::Locals() {
    setSymbol('L');
    setName("Locals");
}

void Locals::spawn(GameMap& map, int x, int y) {
    NewPosition(x, y);
    map.setCell(x, y, getSymbol());
}

// TaxCollector implementation
TaxCollector::TaxCollector() {
    setSymbol('T');
    setName("TaxCollector");
}

// Implement the new methods:
void TaxCollector::move(GameMap& map, int dx, int dy) {
    // Clear current position
    map.setCell(x, y, ' ');

    // Calculate new position with bounds checking
    int newX = max(1, min(18, x + dx));
    int newY = max(1, min(68, y + dy));

    // Update position
    NewPosition(newX, newY);
    map.setCell(newX, newY, getSymbol());
}

bool TaxCollector::isAtPosition(int x, int y) const {
    return this->x == x && this->y == y;
}




void waitKey() {
    cout << "Press any key to continue...";
    _getch();
}



void TaxCollector::CollectTax(King& king, Kingdom* kingdom, GameMap& map) {
    // Store starting position
    int startX = x;
    int startY = y;

    // Display instructions
    clearScreen();
    cout << "TAX COLLECTION MODE" << endl;
    cout << "Use WASD keys to move the Tax Collector (T)" << endl;
    cout << "Move to Nobles (N) or Locals (L) to collect taxes" << endl;
    cout << "Press 'C' to collect taxes when on target" << endl;
    cout << "Press 'Q' to finish tax collection" << endl;
    waitKey();

    char input;
    bool collecting = true;

    while (collecting) {
        clearScreen();
        map.Print_GameMap();
        cout << "\nTax Collector Controls: WASD to move, C to collect, Q to quit" << endl;

        input = _getch();
        input = tolower(input);

        // Clear current position
        map.setCell(x, y, ' ');

        // Handle movement
        switch (input) {
        case 'w': if (x > 1) x--; break;    // Up
        case 's': if (x < 18) x++; break;   // Down
        case 'a': if (y > 1) y--; break;    // Left
        case 'd': if (y < 68) y++; break;   // Right

        case 'c': {  // Collect taxes
            // Check if on Nobles
            if (x == kingdom->getNobles()->getX() && y == kingdom->getNobles()->getY()) {
                king.IncreaseEcon(king.Get_TaxAmount());
                cout << "Collected taxes from nobles!" << endl;
                kingdom->getNobles()->increaseResentment(2);
                waitKey();
            }
            // Check if on Locals
            else if (kingdom->IsTaxingEveryone() &&
                x == kingdom->getLocals()->getX() && y == kingdom->getLocals()->getY()) {
                king.IncreaseEcon(king.Get_TaxAmount() / 2);
                cout << "Collected taxes from locals!" << endl;
                kingdom->getLocals()->increaseResentment(5);
                waitKey();
            }
            else {
                cout << "Not on a valid tax collection target!" << endl;
                waitKey();
            }
            break;
        }

        case 'q':  // Quit tax collection
            collecting = false;
            break;
        }

        // Update Tax Collector position
        map.setCell(x, y, 'T');
    }

    // Return to starting position

}


















void TaxCollector::spawn(GameMap& map, int x, int y) {
    NewPosition(x, y);
    map.setCell(x, y, getSymbol());
}

// General implementation
General::General() : general_key(1111) {
    setSymbol('G');
    setName("General");
}

void General::spawn(GameMap& map, int x, int y) {
    NewPosition(x, y);
    map.setCell(x, y, getSymbol());
}



void General::imposePenalty(Kingdom* kingdom) {
    cout << "\n=== PENALTY IMPOSED ===\n";
    cout << "Morale decreased by " << PENALTY_MORALE << "%\n";
    cout << "Happiness decreased by " << PENALTY_HAPPINESS << "%\n";

    kingdom->getArmy()->changeMorale(-PENALTY_MORALE);
    kingdom->getHappiness()->change(-PENALTY_HAPPINESS);

    // Small chance of rebellion
    if (rand() % 100 < 20) { // 20% chance
        int rebels = kingdom->getArmy()->getSize() / 10;
        kingdom->getArmy()->setSize(kingdom->getArmy()->getSize() - rebels);
        std::cout << rebels << " soldiers have deserted!\n";
    }

    Sleep(3000);
}




bool General::PassKey_Check(Kingdom* kingdom) {
    cout << "Enter the general's passkey: ";
    int passkey;
    cin >> passkey;
    if (passkey == general_key) {
        cout << "Access granted." << endl;
        return true;
    }
    cout << "Invalid passkey!" << endl;
    Sleep(1000);
    return false;
}

// Soldier implementation
Soldier::Soldier() : size(0), morale(50), training(1), damage(1), rebels(0) {}

int Soldier::getSize() const {
    return size;
}

int Soldier::getMorale() const {
    return morale;
}

int Soldier::getTraining() const {
    return training;
}

void Soldier::setSize(int newSize) {
    size = newSize;
    if (size < 0) size = 0;
}

void Soldier::changeMorale(int delta) {
    morale += delta;
    if (morale < 0) morale = 0;
    if (morale > 100) morale = 100;
}

void Soldier::train() {
    training++;
    changeMorale(5);
}

void Soldier::moral_checker() {
    if (morale < 50) {
        cout << "Soldiers' morale is critically low: " << morale << "%" << endl;
        damage -= static_cast<int>(damage * 0.1);
    }
}

void Soldier::spawn(GameMap& map, int x, int y) {
    map.setCell(x, y, 'S');
}

char Soldier::getSymbol() const {
    return 'S';
}

void Soldier::Display_Stats() const {
    cout << "Soldiers: " << size << endl;
    cout << "Morale: " << morale << "%" << endl;
    cout << "Training: " << training << endl;
    cout << "Damage: " << damage << endl;
    cout << "Rebels: " << rebels << endl;
}

// Kingdom implementation
Kingdom::Kingdom(const string& kingdomName) : name(kingdomName), taxEveryone(true) {
    gold = new Resource("Gold", 1000);
    food = new Resource("Food", 1000);
    population = new Resource("Population", 500);
    happiness = new Resource("Happiness", 50);
    army = new Soldier();
    king = new King();
    nobles = new Nobles();
    locals = new Locals();
    taxCollector = new TaxCollector();
    general = new General();
    gameMap = new GameMap();
}

Kingdom::~Kingdom() {
    delete gold;
    delete food;
    delete population;
    delete happiness;
    delete army;
    delete king;
    delete nobles;
    delete locals;
    delete taxCollector;
    delete general;
    delete gameMap;
}

string Kingdom::getName() const {
    return name;
}

Resource* Kingdom::getGold() const {
    return gold;
}

Resource* Kingdom::getFood() const {
    return food;
}

Resource* Kingdom::getPopulation() const {
    return population;
}

Resource* Kingdom::getHappiness() const {
    return happiness;
}

Soldier* Kingdom::getArmy() const {
    return army;
}

King* Kingdom::getKing() const {
    return king;
}

Nobles* Kingdom::getNobles() const {
    return nobles;
}

Locals* Kingdom::getLocals() const {
    return locals;
}

TaxCollector* Kingdom::getTaxCollector() const {
    return taxCollector;
}


General* Kingdom::getGeneral() const {
    return general;
}

bool Kingdom::IsTaxingEveryone() const {
    return taxEveryone;
}

void Kingdom::collectTaxes() {
    int taxAmount = population->getAmount() * happiness->getAmount() / 100;
    StatusDisplay::showLoadingBar("Collecting taxes", 3);
    gold->change(taxAmount);
    happiness->change(-5);
    cout << "Collected " << taxAmount << " gold in taxes." << endl;
    cout << "Kingdom happiness decreased by 5%." << endl;
}

void Kingdom::harvestFood() {
    int foodHarvested = population->getAmount() * 2;
    StatusDisplay::showLoadingBar("Harvesting food", 3);
    food->change(foodHarvested);
    cout << "Harvested " << foodHarvested << " units of food." << endl;
}

void Kingdom::trainArmy(int count) {
    // First verify the General's authorization
    if (!general->PassKey_Check(this)) {
        // Penalty already handled in PassKey_Check
        return;
    }

    // Check resources
    int goldCost = count * 10;
    if (gold->getAmount() < goldCost) {
        cout << "Not enough gold to train soldiers!" << endl;
        cout << "Needed: " << goldCost << " | Available: " << gold->getAmount() << endl;
        Sleep(2000);
        return;
    }
    if (population->getAmount() < count) {
        cout << "Not enough population to recruit from!" << endl;
        cout << "Attempted to recruit: " << count << " | Available: " << population->getAmount() << endl;
        Sleep(2000);
        return;
    }

    // Confirmation with detailed information
    cout << "\n=== Recruitment Details ===" << endl;
    cout << "Soldiers to recruit: " << count << endl;
    cout << "Gold cost: " << goldCost << endl;
    cout << "Population to conscript: " << count << endl;
    cout << "New army size: " << (army->getSize() + count) << endl;

    char confirm = StatusDisplay::getConfirmation("\nProceed with recruitment? (y/n): ");
    if (confirm != 'y' && confirm != 'Y') {
        cout << "Recruitment canceled by royal decree." << endl;
        Sleep(1000);
        return;
    }

    // Execute recruitment
    StatusDisplay::showLoadingBar(" " + general->getName() + " is training troops", 5);

    // Deduct costs
    gold->change(-goldCost);
    population->change(-count);

    // Add soldiers
    int oldSize = army->getSize();
    army->setSize(oldSize + count);

    // Effects based on recruitment size
    if (count > 50) {
        // Large recruitment affects morale
        army->changeMorale(-5);
        happiness->change(-3);
        cout << "Mass conscription has lowered morale and happiness!" << endl;
    }
    else if (count < 10) {
        // Small elite group gets bonus
        army->train(); // Extra training
        cout << "Elite squad receives special training!" << endl;
    }

    // Spawn soldiers on map
    for (int i = 0; i < min(count, 5); i++) { // Spawn up to 5 soldier symbols
        army->spawn(*gameMap, 10 + i, 10 + i);
    }

    // Recruitment report
    cout << "\n=== Recruitment Complete ===" << endl;
    cout << "Trained " << count << " new soldiers" << endl;
    cout << "Army size: " << oldSize << " -> " << army->getSize() << endl;
    cout << "Gold: -" << goldCost << endl;
    cout << "Population: -" << count << endl;

    // Small chance of special event
    if (rand() % 100 < 15) { // 15% chance
        int volunteers = rand() % (count / 5) + 1;
        population->change(-volunteers);
        army->setSize(army->getSize() + volunteers);
        cout << "\nBonus: " << volunteers << " volunteers joined the army!" << endl;
    }

    Sleep(3000);
}






void Kingdom::improveMorale() {
    if (gold->getAmount() < 100) {
        cout << "Not enough gold to improve morale!" << endl;
        return;
    }
    gold->change(-100);
    happiness->change(10);
    army->changeMorale(15);
    cout << "Held festivities to improve morale!" << endl;
}

void Kingdom::buildStructure() {
    if (gold->getAmount() < 300) {
        cout << "Not enough gold to build a structure!" << endl;
        return;
    }
    gold->change(-300);
    happiness->change(5);
    population->change(25);
    cout << "Built a new structure in your kingdom!" << endl;
}

void Kingdom::randomEvent() {
    int event = rand() % 5;
    switch (event) {
    case 0:
        cout << "A plague has struck your kingdom!" << endl;
        population->change(-population->getAmount() / 10);
        happiness->change(-10);
        break;
    case 1:
        cout << "Bumper harvest this year!" << endl;
        food->change(food->getAmount() / 4);
        happiness->change(5);
        break;
    case 2:
        cout << "Trade flourishes in your kingdom!" << endl;
        gold->change(gold->getAmount() / 5);
        happiness->change(5);
        break;
    case 3:
        cout << "Civil unrest in your kingdom!" << endl;
        happiness->change(-15);
        if (army->getSize() > 0) {
            army->setSize(army->getSize() - army->getSize() / 20);
        }
        break;
    case 4:
        cout << "A mild season passes without incident." << endl;
        break;
    }
}

void Kingdom::display() const {
    cout << "=== " << name << " Kingdom Status ===" << endl;
    gold->display();
    food->display();
    population->display();
    happiness->display();
    army->Display_Stats();
    king->Display_Economy();
    cout << "Nobles Resentment: " << nobles->getResentmentLevel() << endl;
    cout << "Locals Resentment: " << locals->getResentmentLevel() << endl;
    cout << "Tax Policy: " << king->Get_TaxAmount() << (taxEveryone ? " (Everyone)" : " (Nobles only)") << endl;
    cout << "=============================" << endl;
}

// StatusDisplay implementation
void StatusDisplay::showLoadingBar(const string& action, int seconds) {
    const int barWidth = 40;
    int totalSteps = seconds * 10;
    int msBetweenUpdates = 1000 / 10;
    cout << action << " [";
    for (int i = 0; i < totalSteps; ++i) {
        int position = (i * barWidth) / totalSteps;
        clearScreen();
        cout << action << " [";
        for (int j = 0; j < barWidth; ++j) {
            if (j < position) cout << "=";
            else if (j == position) cout << ">";
            else cout << " ";
        }
        cout << "] " << int((i * 100) / totalSteps) << "%" << endl;
        Sleep(msBetweenUpdates);
    }
    cout << "\n" << action << " complete!" << endl;
    Sleep(500);
}

void StatusDisplay::displayBanner(const string& title) {
    int width = title.length() + 6;
    string border(width, '=');
    cout << "\n" << border << endl;
    cout << "=  " << title << "  =" << endl;
    cout << border << endl;
}

void StatusDisplay::displayConfirmation(const string& message) {
    cout << "\n" << message << endl;
}

char StatusDisplay::getConfirmation(const string& message) {
    char response;
    cout << message;
    cin >> response;
    return response;
}

// GameManager implementation
GameManager::GameManager() : kingdom(nullptr), gameMap(nullptr), currentTurn(0), gameOver(false) {}

GameManager::~GameManager() {
    delete kingdom;
    delete gameMap;
}

void GameManager::displayMainMenu() {
    clearScreen();
    cout << "=================================" << endl;
    cout << "       Medieval Kingdom Simulator" << endl;
    cout << "=================================" << endl;
    cout << "           MAIN MENU" << endl;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "3. View High Scores" << endl;
    cout << "4. Quit" << endl;
    cout << "Enter your choice: ";
}

void GameManager::displayMap() {
    clearScreen();
    StatusDisplay::displayBanner("Kingdom Map");
    gameMap->Print_GameMap();
    cout << "\nPress any key to continue...";
    _getch();
}

void GameManager::saveGame() {
    ofstream outFile("savegame.dat");
    if (outFile) {
        outFile << playerName << endl;
        outFile << kingName << endl;
        outFile << currentTurn << endl;
        // Save kingdom data
        outFile << kingdom->getName() << endl;
        outFile << kingdom->getGold()->getAmount() << endl;
        outFile << kingdom->getFood()->getAmount() << endl;
        outFile << kingdom->getPopulation()->getAmount() << endl;
        outFile << kingdom->getHappiness()->getAmount() << endl;
        outFile << kingdom->getArmy()->getSize() << endl;
        outFile << kingdom->getArmy()->getMorale() << endl;
        outFile << kingdom->getArmy()->getTraining() << endl;
        outFile << kingdom->getKing()->Get_TaxAmount() << endl;
        outFile << kingdom->IsTaxingEveryone() << endl;
        outFile.close();
        cout << "Game saved successfully!" << endl;
    }
    else {
        cout << "Error saving game!" << endl;
    }
    Sleep(1000);
}

void GameManager::loadGame() {
    ifstream inFile("savegame.dat");
    if (inFile) {
        getline(inFile, playerName);
        getline(inFile, kingName);
        inFile >> currentTurn;

        string kingdomName;
        getline(inFile, kingdomName); // Clear newline
        getline(inFile, kingdomName);

        int gold, food, population, happiness;
        int armySize, armyMorale, armyTraining;
        int taxAmount;
        bool taxEveryone;

        inFile >> gold >> food >> population >> happiness;
        inFile >> armySize >> armyMorale >> armyTraining;
        inFile >> taxAmount >> taxEveryone;

        kingdom = new Kingdom(kingdomName);
        kingdom->getGold()->setAmount(gold);
        kingdom->getFood()->setAmount(food);
        kingdom->getPopulation()->setAmount(population);
        kingdom->getHappiness()->setAmount(happiness);
        kingdom->getArmy()->setSize(armySize);
        kingdom->getArmy()->changeMorale(armyMorale - 50); // Set morale
        for (int i = 1; i < armyTraining; i++) { // Set training level
            kingdom->getArmy()->train();
        }
        kingdom->getKing()->Impose_Tax(taxAmount, taxEveryone, kingdom);

        inFile.close();
        cout << "Game loaded successfully!" << endl;
    }
    else {
        cout << "No saved game found!" << endl;
    }
    Sleep(1000);
}

void GameManager::viewHighScores() {
    clearScreen();
    StatusDisplay::displayBanner("High Scores");
    cout << "High scores feature will be implemented here." << endl;
    cout << "Press any key to continue...";
    _getch();
}

void GameManager::startGame() {
    while (true) {
        displayMainMenu();
        int choice;
        cin >> choice;

        switch (choice) {
        case 1: { // New Game
            clearScreen();
            StatusDisplay::displayBanner("New Game");
            cout << "Enter a name for your kingdom: ";
            cin.ignore();
            getline(cin, playerName);

            cout << "Welcome to the Kingdom of " << playerName << "!" << endl;
            cout << "Press Enter to continue...";
            cin.ignore();

            cout << "Enter a name for your king: ";
            getline(cin, kingName);

            kingdom = new Kingdom(playerName);
            gameMap = new GameMap();

            // Spawn characters
            kingdom->getKing()->spawn(*gameMap, 5, 5);
            kingdom->getNobles()->spawn(*gameMap, 12, 16);
            kingdom->getLocals()->spawn(*gameMap, 10, 10);
            kingdom->getTaxCollector()->spawn(*gameMap, 8, 8);
            kingdom->getGeneral()->spawn(*gameMap, 9, 9);

            Gameloop();
            break;
        }
        case 2: // Load Game
            loadGame();
            if (kingdom) {
                gameMap = new GameMap();
                // Spawn characters at default positions
                kingdom->getKing()->spawn(*gameMap, 5, 5);
                kingdom->getNobles()->spawn(*gameMap, 6, 6);
                kingdom->getLocals()->spawn(*gameMap, 7, 7);
                kingdom->getTaxCollector()->spawn(*gameMap, 8, 8);
                kingdom->getGeneral()->spawn(*gameMap, 9, 9);
                Gameloop();
            }
            break;
        case 3: // View High Scores
            viewHighScores();
            break;
        case 4: // Quit
            return;
        default:
            cout << "Invalid choice! Please try again." << endl;
            Sleep(1000);
        }
    }
}

void GameManager::processPlayerTurn() {
    while (true) {
        clearScreen();
        StatusDisplay::displayBanner(playerName + "'s Kingdom - Turn " + to_string(currentTurn));

        int choice;
        cout << "\nGAME MENU\n";
        cout << "1. Display Kingdom Status\n";
        cout << "2. Military Menu\n";
        cout << "3. Financial Menu\n";
        cout << "4. Save Game\n";
        cout << "5. View Map\n";
        cout << "Enter your choice: ";
        cin >> choice;

        clearScreen();

        switch (choice) {
        case 1: // Display Kingdom Status
            StatusDisplay::displayBanner("Kingdom Status");
            kingdom->display();
            cout << "Press any key to continue...";
            _getch();
            break;

        case 2: { // Military Menu
            StatusDisplay::displayBanner("Military Menu");
            cout << "1. Train Army\n";
            cout << "2. Improve Morale\n";
            cout << "3. View Army Status\n";
            cout << "4. Back to Main Menu\n";
            cout << "Enter your choice: ";
            int militaryChoice;
            cin >> militaryChoice;

            if (militaryChoice == 1) {
                StatusDisplay::displayBanner("Army Recruitment");
                if (kingdom->getGeneral()->PassKey_Check(kingdom)) {
                    int count;
                    cout << "How many soldiers to train? ";
                    cin >> count;
                    if (count > 0) {
                        kingdom->trainArmy(count);
                    }
                    else {
                        cout << "Invalid number. Recruitment canceled." << endl;
                        Sleep(1000);
                    }
                }
                else {
                    cout << "Access denied. Wait for next turn." << endl;
                    Sleep(1000);
                }
            }
            else if (militaryChoice == 2) {
                StatusDisplay::displayBanner("Improving Morale");
                kingdom->improveMorale();
            }
            else if (militaryChoice == 3) {
                StatusDisplay::displayBanner("Army Status");
                kingdom->getArmy()->Display_Stats();
            }
            cout << "Press any key to continue...";
            _getch();
            break;
        }
        case 3: { // Financial Menu
            StatusDisplay::displayBanner("Financial Menu");
            cout << "1. Impose Tax Policy\n";
            cout << "2. Collect Taxes via Tax Collector\n";
            cout << "3. View Economy Status\n";
            cout << "4. Back to Main Menu\n";
            cout << "Enter your choice: ";
            int financeChoice;
            cin >> financeChoice;


            if (financeChoice == 1) {
                StatusDisplay::displayBanner("Impose Tax Policy");
                char taxChoice;
                cout << "Select tax policy:\n";
                cout << "a) Tax Everyone\n";
                cout << "b) Tax Only Nobles\n";
                cout << "Choice: ";
                cin >> taxChoice;
                if (taxChoice == 'a' || taxChoice == 'b') {
                    int taxAmount;
                    cout << "Enter tax amount: ";
                    cin >> taxAmount;
                    if (taxAmount >= 0) {
                        kingdom->getKing()->Impose_Tax(taxAmount, taxChoice == 'a', kingdom);
                        cout << "Tax policy updated." << endl;
                    }
                    else {
                        cout << "Invalid tax amount!" << endl;
                    }
                }
                else {
                    cout << "Invalid choice!" << endl;
                }
            }
            else if (financeChoice == 2) {
                StatusDisplay::displayBanner("Tax Collection");
                kingdom->getTaxCollector()->CollectTax(*kingdom->getKing(), kingdom, *gameMap);

            }
            else if (financeChoice == 3) {
                StatusDisplay::displayBanner("Economy Status");
                kingdom->getKing()->Display_Economy();
            }
            cout << "Press any key to continue...";
            _getch();
            break;
        }
        case 4: // Save Game
            saveGame();
            break;
        case 5: // Return to Main Menu
            displayMap();
            break;

        default:
            cout << "Invalid choice!" << endl;
            Sleep(1000);
            break;

        }
    }
}

void GameManager::Gameloop() {
    while (!gameOver) {
        currentTurn++;
        processPlayerTurn();

        // Check for game over conditions
        if (kingdom->getPopulation()->getAmount() <= 0) {
            gameOver = true;
            cout << "Your kingdom has fallen!" << endl;
            Sleep(2000);
        }
    }

    clearScreen();
    StatusDisplay::displayBanner("Game Over");
    cout << "Your kingdom lasted for " << currentTurn << " turns!" << endl;
    cout << "Final Kingdom Status:" << endl;
    kingdom->display();
    cout << "Press any key to exit...";
    _getch();
}
