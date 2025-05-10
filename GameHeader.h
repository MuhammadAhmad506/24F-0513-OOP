#include <iostream>


using namespace std;

// Forward declarations
class Kingdom;
class GameManager;
class Position;
class Person;
class Economy;
class King;
class Nobles;
class Locals;
class TaxCollector;
class General;
class Soldier;

// Utility function
void clearScreen();

// Position class
class Position {
public:
    int x, y;
    Position();
    void NewPosition(int newX, int newY);
    int getX() const { return x; }
    int getY() const { return y; }
    Position getPosition() const { return *this; }
};
// GameMap class
class GameMap : public Position {
private:
    char arr[20][70];
public:
    GameMap();
    void Initialize_GameMap();
    void Print_GameMap();
    char getCell(int x, int y) const;
    void setCell(int x, int y, char value);
};

// Resource class
class Resource {
private:
    string name;
    int amount;
public:
    Resource(const string& name, int amount);
    string getName() const;
    int getAmount() const;
    void setAmount(int newAmount);
    void change(int delta);
    void display() const;
};

// Economy class
class Economy {
protected:
    int healthOfEconomy;
    int amount;
    int taxAmount;
public:
    Economy();
    void Set_TaxAmount(int a);
    int Get_TaxAmount() const;
    void Econ_Health_Checker();
    void Display_Economy() const;
    void IncreaseEcon(int a);
    int Get_Amount() const;
};

// Person base class
class Person : public Position {
protected:
    string name;
    char symbol;
    int resentment_level;
public:
    Person();
    void setName(const string& n);
    string getName() const;
    void setSymbol(char s);
    char getSymbol() const;
    int getResentmentLevel() const;
    void increaseResentment(int amount);
};

// King class
class King : public Person, public Economy {
public:
    King();
    void Impose_Tax(int a, bool everyone, Kingdom* kingdom);
    void spawn(GameMap& map, int x, int y);
};

// Nobles class
class Nobles : public Person {
public:
    Nobles();
    void spawn(GameMap& map, int x, int y);
};

// Locals class
class Locals : public Person {
public:
    Locals();
    void spawn(GameMap& map, int x, int y);
};

// TaxCollector class
class TaxCollector : public Person {
public:
    TaxCollector();
    void CollectTax(King& king, Kingdom* kingdom, GameMap& map);
    void spawn(GameMap& map, int x, int y);
    void move(GameMap& map, int dx, int dy);
    bool isAtPosition(int x, int y) const;
};
// General class
class General : public Person {
private:
    int general_key;
    int failedAttempts;
    static const int maxAttempts = 3; // Maximum attempts for passkey
    static const int PENALTY_MORALE = 10;
    static const int PENALTY_HAPPINESS = 5;
public:
    General();
    void spawn(GameMap& map, int x, int y);
    bool PassKey_Check(Kingdom* kingdom);
    void imposePenalty(Kingdom* kingdom);
};



class ResourceManager {
private:
    static const int MAX_RESOURCES = 5;
    Resource* resources[MAX_RESOURCES];
public:
    ResourceManager();
    ~ResourceManager();
    void addResource(int index, int amount);
    bool removeResource(int index, int amount);
    Resource* getResource(int index) const;
    void displayResources() const;
    int getMaxResources() const { return MAX_RESOURCES; }
};





// Soldier class
class Soldier {
private:
    int size;
    int morale;
    int training;
    int damage;
    int rebels;
public:
    Soldier();
    int getSize() const;
    int getMorale() const;
    int getTraining() const;
    void setSize(int newSize);
    void changeMorale(int delta);
    void train();
    void moral_checker();
    void spawn(GameMap& map, int x, int y);
    char getSymbol() const;
    void Display_Stats() const;
};

// Kingdom class
class Kingdom {
private:
    string name;
    Resource* gold;
    Resource* food;
    Resource* population;
    Resource* happiness;
    Soldier* army;
    King* king;
    Nobles* nobles;
    Locals* locals;
    TaxCollector* taxCollector;
    General* general;
    GameMap* gameMap;
    bool taxEveryone;
    ResourceManager* resources;

public:
    Kingdom(const string& kingdomName);
    ~Kingdom();
    void handleTradeMenu();
    string getName() const;
    Resource* getGold() const;
    Resource* getFood() const;
    Resource* getPopulation() const;
    Resource* getHappiness() const;
    Soldier* getArmy() const;
    King* getKing() const;
    Nobles* getNobles() const;
    Locals* getLocals() const;
    TaxCollector* getTaxCollector() const;
    General* getGeneral() const;
    bool IsTaxingEveryone() const;
    void collectTaxes();
    void harvestFood();
    void trainArmy(int count);
    void improveMorale();
    void buildStructure();
    void randomEvent();
    void display() const;
};

// StatusDisplay class
class StatusDisplay {
public:
    static void showLoadingBar(const string& action, int seconds);
    static void displayBanner(const string& title);
    static void displayConfirmation(const string& message);
    static char getConfirmation(const string& message);
};

// GameManager class
class GameManager {
private:
    Kingdom* kingdom;
    GameMap* gameMap;
    int currentTurn;
    bool gameOver;
    string playerName;
    string kingName;

    void displayMainMenu();
    void displayMap();
    void saveGame();
    void Gameloop();
    void loadGame();
    void viewHighScores();
    void processPlayerTurn();
public:
    GameManager();
    ~GameManager();
    void startGame();
};