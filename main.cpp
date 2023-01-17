#include <iostream>
#include <sstream>
#include <vector>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

using namespace std;
using namespace curlpp;
using namespace curlpp::options;

struct sharpeningWheel{ // create a struct to store variables for the sharpening wheels
  int amount;
  int uses;
};

struct resources{ // create another struct for ever other item, item price per unit and needed amount of items also transactions and current date
  int penguinCoin, wood, iron, string, stone, feathers, sharpSword, armor, arrows, crossbow, bluntSword, transactions, date;
  int woodPPU, ironPPU, stringPPU, stonePPU, feathersPPU, sharpSwordPPU, armorPPU, arrowsPPU, crossbowPPU, bluntSwordPPU;
  int neededArmor, neededCrossbow, neededArrows, neededBluntSword, neededSharpSword;
  int soldArmor, soldCrossbow, soldArrows, soldBluntSword, soldSharpSword;
  sharpeningWheel sharpWheels; // call on the sharpeningWheel struct and create an instance
}; 

typedef struct resources inventory; // create a 'type' that inherits the values of the two structs above ^^

class saveStateManager { // this class has methods for all the stuff needed to create, save and load game save states. It uses REPLIT's builtin database
  public:
    void initSaveState(string URL, string JWT) {
      Cleanup cleaner; // create an instance of curlpp::Cleanup

      Easy initRequest; // create our request handler using curlpp::Easy
      initRequest.setOpt(new options::Url(URL + "/")); // give it the database url --> this is the environment variable that replit gives as part of the builtin database
      initRequest.setOpt(new options::HttpHeader({"Authorization: Bearer " + JWT})); // the database uses a JWT(JSON Web Token) for authentication. This needs to passed into the requests header

      string data = "penguinCoins=0&wood=0&iron=0&stone=0&string=0&feathers=0&bluntSword=0&sharpSword=0&arrows=0&crossbow=0&armor=0&sharpeningWheelAmount=0&sharpeningWheelUses=0&day=1&transactions=0&firstRun=1"; // key-value pair data.
      
      initRequest.setOpt(new options::PostFields(data)); // pass the data to the post field
      initRequest.setOpt(new options::PostFieldSize(data.length())); // tell the server the length of the data

      initRequest.perform(); // send the request
    }

    inventory loadSave(string URL, string JWT){
      inventory b; // create an instance of the inventory type
      
      Cleanup cleaner; // create an instance of curlpp:Cleanup
      
      vector<string> keys = {"penguinCoins", "wood", "iron", "string", "stone", "feathers", "bluntSword", "sharpSword", "arrows", "crossbow", "armor", "sharpeningWheelAmount", "sharpeningWheelUses", "day", "transactions"}; // create a vector that stores all the keys for gameplay data we need to get from the database

      for (auto key : keys){ // loop through the vector of keys
        Easy loadRequest; // create our handler

        ostringstream os; // create a string stream to put the request output into so we can store the data for later use

        loadRequest.setOpt(new options::Url(URL + "/" + key)); 
        loadRequest.setOpt(new options::HttpHeader({"Authorization: Bearer " + JWT}));
        loadRequest.setOpt(new options::WriteStream(&os)); // tell the request to direct its out put to the stringstream variable(defaulte is cout)
        loadRequest.setOpt(new options::HttpGet(true)); // set the request type to an Http GET request

        loadRequest.perform(); // send the request

        if (key == "penguinCoins"){ // check what the current key is and store the output of the request in the proper variable
          b.penguinCoin = stoi(os.str());
        } else if (key == "wood"){
          b.wood = stoi(os.str());
        } else if (key == "iron"){
          b.iron = stoi(os.str());
        } else if (key == "string"){
          b.string = stoi(os.str());
        } else if (key == "feathers"){
          b.feathers = stoi(os.str());
        } else if (key == "bluntSword"){
          b.bluntSword = stoi(os.str());
        } else if (key == "sharpSword"){
          b.sharpSword = stoi(os.str());
        } else if (key == "arrows"){
          b.arrows = stoi(os.str());
        } else if (key == "crossbow"){
          b.crossbow = stoi(os.str());
        } else if (key == "armor"){
          b.armor = stoi(os.str());
        } else if (key == "sharpeningWheelAmount"){
          b.sharpWheels.amount = stoi(os.str());
        } else if (key == "sharpeningWheelUses"){
          b.sharpWheels.uses = stoi(os.str());
        } else if (key == "day"){
          b.date = stoi(os.str());
        } else if (key == "transactions"){
          b.transactions = stoi(os.str());
        } else if (key == "stone"){
          b.stone = stoi(os.str());
        }
        
      }

      b.woodPPU = 2; // set other default values that are not stored in the database
      b.ironPPU = 4;
      b.stringPPU = 3;
      b.stonePPU = 1;
      b.feathersPPU = 2;

      b.sharpSwordPPU = 15;
      b.bluntSwordPPU = 5;
      b.armorPPU = 10;
      b.arrowsPPU = 2;
      b.crossbowPPU = 10;

      b.neededArmor = 200;
      b.neededSharpSword = 100;
      b.neededBluntSword = 200;
      b.neededArrows = 1000;
      b.neededCrossbow = 100;

      b.soldArmor = 0;
      b.soldArrows = 0;
      b.soldBluntSword = 0;
      b.soldCrossbow = 0;
      b.soldSharpSword = 0;

      return b; // return the inventory variable so other functions can use it
    }

    void saveState(inventory b, string URL, string JWT){
      Cleanup cleaner; // create our cleaner

      vector<string> keys = {"penguinCoins", "wood", "iron", "string", "stone", "feathers", "bluntSword", "sharpSword", "arrows", "crossbow", "armor", "sharpeningWheelAmount", "sharpeningWheelUses", "day", "transactions"};

      for (auto key : keys){ // this loop deletes the pre existing keys in the database so we can stay at a constant amount of 16 keys.
        Easy deleteRequest; // create a handler

        deleteRequest.setOpt(new options::Url(URL + "/" + key)); // url
        deleteRequest.setOpt(new options::HttpHeader({"Authorization: Bearer " + JWT})); // auth

        deleteRequest.setOpt(new options::CustomRequest("DELETE")); // set request type to DELETE

        deleteRequest.perform(); // send request
      }

      
      Easy saveRequest; // create a handler

      saveRequest.setOpt(new options::Url(URL)); // url
      saveRequest.setOpt(new options::HttpHeader({"Authorization: Bearer " + JWT})); // auth

      string data = "penguinCoins=" + to_string(b.penguinCoin) + "&wood=" + to_string(b.wood) + "&iron=" + to_string(b.iron) + "&stone=" + to_string(b.stone) + "&string=" + to_string(b.string) + "&feathers=" + to_string(b.feathers) + "&bluntSword=" + to_string(b.bluntSword) + "&sharpSword=" + to_string(b.sharpSword) + "&arrows=" + to_string(b.arrows) + "&crossbow=" + to_string(b.crossbow) + "&armor=" + to_string(b.armor) + "&sharpeningWheelAmount=" + to_string(b.sharpWheels.amount) + "&sharpeningWheelUses=" + to_string(b.sharpWheels.uses) + "&day=" + to_string(b.date) + "&transactions=" + to_string(b.transactions) + "&firstRun=0"; // data from the invetory variable 'b' that was passed into this method

      saveRequest.setOpt(new options::PostFields(data)); // set the post fields
      saveRequest.setOpt(new options::PostFieldSize(data.length())); // set the pot fields length

      saveRequest.perform(); // send request
    }

    bool checkForExistingGame(string URL, string JWT){
      Cleanup cleaner; // cleaner

      Easy checkRequest; // handler

      ostringstream os; 
      int response;

      checkRequest.setOpt(new options::Url(URL + "/firstRun")); // url
      checkRequest.setOpt(new options::HttpHeader({"Authorization: Bearer" + JWT})); // auth
      checkRequest.setOpt(new options::WriteStream(&os)); // storage of output
      checkRequest.setOpt(new options::HttpGet(true)); // set as GET request

      checkRequest.perform(); // send request

      try{
        response = stoi(os.str());
      } catch (std::invalid_argument) { // if the variable does not exist on the database it will return an error std::invalid_argument
        response = 404; // if it does then response to 404 becuase value does not exist
      }

      if (response == 404){ // the bools being returned, tell the program whether or the user has played and saved their progress in the game before
        return false;
      } else{
        if (response == 1){
          return false;
        } else {
          return true;
        }
      }
    }
};

string dynamicPrinting(int spaces, int item){ // ui string printing

  string output = to_string(item);        // take in the amount of spaces need from start to end of string
                                          // take in the value of the item
  if (item >= 10 && item < 100){          // subtract the amount of characters in the item
    spaces = spaces - 2;                  // add the item and the spaces to the output string
    for (int i = 1; i <= spaces; i++){
      output = output + " ";
    }
    output = output + "|";
  } else if (item >= 100 && item < 1000){
    spaces = spaces - 3;
    for (int i = 1; i <= spaces; i++){
      output = output + " ";
    }
    output = output + "|";
  } else if (item >= 1000 && item < 10000){
    spaces = spaces - 4;
    for (int i = 1; i <= spaces; i++){
      output = output + " ";
    }
    output = output + "|";
  } else if (item >= 10000){
    spaces = spaces - 5;
    for (int i = 1;i <= spaces;i++){
      output = output + " ";
    }
    output = output + "|";
  }else {
    spaces--;
    for (int i = 1; i <= spaces; i++){
      output = output + " ";
    }
    output = output + "|";
  }

  return output;
}

inventory setDefault(){ // give the items in the inventory struct a default value. this is run when the game starts
  inventory b;

  b.transactions = 0;
  b.date = 1;

  b.penguinCoin = 20;
  b.wood = 10;
  b.iron = 10;
  b.string = 10;
  b.stone = 10;
  b.feathers = 10;
  
  b.bluntSword = 0;
  b.sharpSword = 0;
  b.armor = 0;
  b.sharpWheels.amount = 0;
  b.sharpWheels.uses = 0;
  b.arrows = 0;
  b.crossbow = 0;
  

  b.woodPPU = 2;
  b.ironPPU = 4;
  b.stringPPU = 3;
  b.stonePPU = 1;
  b.feathersPPU = 2;

  b.sharpSwordPPU = 15;
  b.bluntSwordPPU = 5;
  b.armorPPU = 10;
  b.arrowsPPU = 2;
  b.crossbowPPU = 10;

  b.neededArmor = 200;
  b.neededSharpSword = 100;
  b.neededBluntSword = 200;
  b.neededArrows = 1000;
  b.neededCrossbow = 100;

  b.soldArmor = 0;
  b.soldArrows = 0;
  b.soldBluntSword = 0;
  b.soldCrossbow = 0;
  b.soldSharpSword = 0;

  return b;
}

inventory store(inventory b){ // store function

  string choice;

  
  
  do{
    string dynamicPenguinCoinString = dynamicPrinting(9, b.penguinCoin);
    string dynamicDateString = dynamicPrinting(5, b.date);
    system("clear");

    cout << "|-----------------------------------------------------------------------------|--------------|\n";
    cout << "|                         The Emperor Penguin Emporium                        |    Day: "<< dynamicDateString << "\n";
    cout << "|-----------------------------------------------------------------------------|--------------|\n";
    cout << "|                                                                                            |\n";
    cout << "|                |-------------------------------------|-------------------------------------|\n";
    cout << "|-------------|  |      _____ ______ _      _          |         ____  _    ___     __       |\n";
    cout << "|  Choices    |  |     / ____|  ____| |    | |         |        |  _ \\| |  | \\ \\   / /       |\n";
    cout << "|-------------|  |    | (___ | |__  | |    | |         |        | |_) | |  | |\\ \\_/ /        |\n";
    cout << "|0. Exit Store|  |     \\___ \\|  __| | |    | |         |        |  _ <| |  | | \\   /         |\n";
    cout << "|-------------|  |     ____) | |____| |____| |____     |        | |_) | |__| |  | |          |\n";
    cout << "|                |    |_____/|______|______|______|    |        |____/ \\____/   |_|          |\n";
    cout << "|                |-------------------------------------|-------------------------------------|\n";
    cout << "|                |  ______ _   _ _______ ______ _____  |  ______ _   _ _______ ______ _____  |\n";
    cout << "|                | |  ____| \\ | |__   __|  ____|  __ \\ | |  ____| \\ | |__   __|  ____|  __ \\ |\n";
    cout << "|                | | |__  |  \\| |  | |  | |__  | |__) || | |__  |  \\| |  | |  | |__  | |__) ||\n";
    cout << "|                | |  __| | . ` |  | |  |  __| |  _  / | |  __| | . ` |  | |  |  __| |  _  / |\n";
    cout << "|                | | |____| |\\  |  | |  | |____| | \\ \\ | | |____| |\\  |  | |  | |____| | \\ \\ |\n";
    cout << "|                | |______|_| \\_|  |_|  |______|_|  \\_\\| |______|_| \\_|  |_|  |______|_|  \\_\\|\n";
    cout << "|----------------|                                     |                                     |\n";
    cout << "| Penguin Coins  |                 __                  |                 ___                 |\n";
    cout << "|                |                /_ |                 |                |__ \\                |\n";
    cout << "|                |                 | |                 |                   ) |               |\n";
    cout << "|     ₱ " << dynamicPenguinCoinString << "                 | |                 |                  / /                |\n";
    cout << "|                |                 | |                 |                 / /_                |\n";
    cout << "|                |                 |_|                 |                |____|               |\n";
    cout << "|--------------------------------------------------------------------------------------------|\n";
    cout << "Your Choice: ";
    
    cin >> choice;

    if (stoi(choice) == 1){ // detect for choice to sell or buy | this is the sell choice
      string choice1;
      do{
        
        string dynamicPenguinCoinString2 = dynamicPrinting(10, b.penguinCoin);
        string dynamicDateString2 = dynamicPrinting(5, b.date);
        string dynamicWoodString = dynamicPrinting(9, b.wood);
        string dynamicIronString = dynamicPrinting(9, b.iron);
        string dynamicStoneString = dynamicPrinting(9, b.stone);
        string dynamicCrossbowString = dynamicPrinting(9, b.crossbow);
        string dynamicArrowsString = dynamicPrinting(9, b.arrows);
        string dynamicBluntSwordString = dynamicPrinting(9, b.bluntSword);
        string dynamicSharpSwordString = dynamicPrinting(9, b.sharpSword);
        string dynamicArmorString = dynamicPrinting(17, b.armor);
        string dynamicStringString = dynamicPrinting(9, b.string);
        string dynamicFeathersString = dynamicPrinting(9, b.feathers);

        string dynamicWoodPPUString = dynamicPrinting(7, b.woodPPU);
        string dynamicIronPPUString = dynamicPrinting(7, b.ironPPU);
        string dynamicStonePPUString = dynamicPrinting(7, b.stonePPU);
        string dynamicStringPPUString = dynamicPrinting(7, b.stringPPU);
        string dynamicFeathersPPUString = dynamicPrinting(7, b.feathersPPU);
        string dynamicCrossbowPPUString = dynamicPrinting(7, b.crossbowPPU);
        string dynamicArrowsPPUString = dynamicPrinting(7, b.arrowsPPU);
        string dynamicBluntSwordPPUString = dynamicPrinting(7, b.bluntSwordPPU);
        string dynamicSharpSwordPPUString = dynamicPrinting(7, b.sharpSwordPPU);
        string dynamicArmorPPUString = dynamicPrinting(15, b.armorPPU);
        
        system("clear");

        cout << "|-----------------------------------------------------------------------------|--------------|\n";
        cout << "|                 The Emperor Penguin Emporium -- Sell                        |    Day: " << dynamicDateString2 << "\n";
        cout << "|-----------------------------------------------------------------------------|--------------|\n";
        cout << "|                                                                                            |\n";
        cout << "|                |------------------|------------------|------------------|------------------|\n";
        cout << "|------------|   |  Penguin Coins   |       Wood       |       Iron       |      Stone       |\n";
        cout << "|  Choices   |   |                  | Amount: " << dynamicWoodString <<  " Amount: " << dynamicIronString <<  " Amount: " << dynamicStoneString  <<  "\n";
        cout << "|------------|   |                  |  Price: ₱ " << dynamicWoodPPUString << "  Price: ₱ " << dynamicIronPPUString << "  Price: ₱ " << dynamicStonePPUString << "\n";
        cout << "|0. Go Back  |   |      ₱ " << dynamicPenguinCoinString2 << "                  |                  |                  |\n";
        cout << "|------------|   |                  |  To Sell Enter   |  To Sell Enter   |  To Sell Enter   |\n";
        cout << "|                |                  |        1         |        2         |        3         |\n";
        cout << "|                |------------------|------------------|------------------|------------------|\n";
        cout << "|                |     Crossbow     |      Arrows      |   Blunt Sword    |   Sharp Sword    |\n";
        cout << "|                | Amount: " << dynamicCrossbowString  << " Amount: " << dynamicArrowsString  << " Amount: " << dynamicBluntSwordString  << " Amount: " << dynamicSharpSwordString  <<"\n";
        cout << "|                |  Price: ₱ " << dynamicCrossbowPPUString << "  Price: ₱ " << dynamicArrowsPPUString << "  Price: ₱ " << dynamicBluntSwordPPUString << "  Price: ₱ " << dynamicSharpSwordPPUString << "\n";
        cout << "|                |                  |                  |                  |                  |\n";
        cout << "|                |  To Sell Enter   |  To Sell Enter   |  To Sell Enter   |  To Sell Enter   |\n";
        cout << "|                |        4         |        5         |        6         |        7         |\n";
        cout << "|                |------------------|------------------|------------------|------------------|\n";
        cout << "|                |                 Armor               |      String      |     Feathers     |\n";
        cout << "|                |            Amount: " << dynamicArmorString  << " Amount: " << dynamicStringString  << " Amount: " << dynamicFeathersString  << "\n";
        cout << "|                |             Price: ₱ " << dynamicArmorPPUString << "  Price: ₱ " << dynamicStringPPUString << "  Price: ₱ " << dynamicFeathersPPUString << "\n";
        cout << "|                |                                     |                  |                  |\n";
        cout << "|                |            To Sell Enter            |  To Sell Enter   |  To Sell Enter   |\n";
        cout << "|                |                  8                  |        9         |        10        |\n";
        cout << "|----------------|-------------------------------------|------------------|------------------|\n";
        cout << "Your Choice: ";
        
        cin >> choice1;
        if (stoi(choice1) == 1 && b.wood >= 1){
          b.wood--;
          b.penguinCoin = b.penguinCoin + b.woodPPU;
          b.transactions++;
        } else if (stoi(choice1) == 2 && b.iron >= 1){
          b.iron--;
          b.penguinCoin = b.penguinCoin + b.ironPPU;
          b.transactions++;
        } else if (stoi(choice1) == 9 && b.string >= 1){
          b.string--;
          b.penguinCoin = b.penguinCoin + b.stringPPU;
          b.transactions++;
        } else if (stoi(choice1) == 3 && b.stone >= 1){
          b.stone--;
          b.penguinCoin = b.penguinCoin + b.stonePPU;
          b.transactions++;
        } else if (stoi(choice1) == 10 && b.feathers >= 1){
          b.feathers--;
          b.penguinCoin = b.penguinCoin + b.feathersPPU;
          b.transactions++;
        } else if (stoi(choice1) == 7 && b.sharpSword >= 1){
          b.sharpSword--;
          b.soldSharpSword++;
          b.penguinCoin = b.penguinCoin + b.sharpSwordPPU;
          b.transactions++;
        } else if (stoi(choice1) == 6 && b.bluntSword >= 1){
          b.bluntSword--;
          b.soldBluntSword++;
          b.penguinCoin = b.penguinCoin + b.bluntSwordPPU;
          b.transactions++;
        } else if (stoi(choice1) == 4 && b.crossbow >= 1){
          b.crossbow--;
          b.soldCrossbow++;
          b.penguinCoin = b.penguinCoin + b.crossbowPPU;
          b.transactions++;
        } else if (stoi(choice1) == 8 && b.armor >= 1){
          b.armor--;
          b.soldArmor++;
          b.penguinCoin = b.penguinCoin + b.armorPPU;
          b.transactions++;
        } else if (stoi(choice1) == 5 && b.arrows >= 1){
          b.arrows = b.arrows - 10;
          b.soldArrows = b.soldArrows + 10;
          b.penguinCoin = b.penguinCoin + 10 * b.arrowsPPU;
          b.transactions++;
        } else{ // this informs the player if they dont have enough material of the item they are trying to sell.
          if (stoi(choice1) == 1){
            cout << "you dont have enough wood to sell\n";
            string garbage;
            cin >> garbage;
          } else if (stoi(choice1) == 2){
            cout << "you dont have enough iron to sell\n";
            string garbage;
            cin >> garbage;
          } else if (stoi(choice1) == 3){
            cout << "you dont have enough stone to sell\n";
            string garbage;
            cin >> garbage;
          } else if (stoi(choice1) == 4){
            cout << "you dont have enough crossbows to sell\n";
            string garbage;
            cin >> garbage;
          } else if (stoi(choice1) == 5){
            cout << "you dont have enough arrows to sell\n";
            string garbage;
            cin >> garbage;
          } else if (stoi(choice1) == 6){
            cout << "you dont have enough blunt swords to sell\n";
            string garbage;
            cin >> garbage;
          } else if (stoi(choice1) == 7){
            cout << "you dont have enough sharp swords to sell\n";
            string garbage;
            cin >> garbage;
          } else if (stoi(choice1) == 8){
            cout << "you dont have enough armor to sell\n";
            string garbage;
            cin >> garbage;
          } else if (stoi(choice1) == 9){
            cout << "you dont have enough string sets to sell\n";
            string garbage;
            cin >> garbage;
          } else if (stoi(choice1) == 10){
            cout << "you dont have enough feathers to sell\n";
            string garbage;
            cin >> garbage;
          }



          if (b.transactions == 10){ // this is important
              b.date++;              // dont want a player sitting on the first day and getting all of the required items
              b.transactions = 0;    // so every time a player buys, sells, or crafts an item b.transactions gets incremented by one
          }                          // every ten transactions the game moves on to the next day and transactions get resets to zero

          
        }
      }while(stoi(choice1) != 0);
    } else if (stoi(choice)== 2){ // buy choice
      string choice2;
      do{

        string dynamicPenguinCoinString2 = dynamicPrinting(9, b.penguinCoin);
        string dynamicDateString2 = dynamicPrinting(5, b.date);
        string dynamicWoodString = dynamicPrinting(9, b.wood);
        string dynamicIronString = dynamicPrinting(9, b.iron);
        string dynamicStoneString = dynamicPrinting(9, b.stone);
        string dynamicStringString = dynamicPrinting(9, b.string);
        string dynamicFeathersString = dynamicPrinting(38, b.feathers);

        string dynamicWoodPPUString = dynamicPrinting(7, b.woodPPU);
        string dynamicIronPPUString = dynamicPrinting(7, b.ironPPU);
        string dynamicStonePPUString = dynamicPrinting(7, b.stonePPU);
        string dynamicStringPPUString = dynamicPrinting(7, b.stringPPU);
        string dynamicFeathersPPUString = dynamicPrinting(36, b.feathersPPU);
        
        system("clear");

        cout << "|-----------------------------------------------------------------------------|--------------|\n";
        cout << "|                 The Emperor Penguin Emporium -- Buy                         |    Day: " << dynamicDateString2 <<"\n";
        cout << "|-----------------------------------------------------------------------------|--------------|\n";
        cout << "|                                                                                            |\n";
        cout << "|                |------------------|------------------|------------------|------------------|\n";
        cout << "|------------|   |                  |                  |                  |                  |\n";
        cout << "|  Choices   |   |       Wood       |       Iron       |      String      |      Stone       |\n";
        cout << "|------------|   | Amount: " << dynamicWoodString << " Amount: " << dynamicIronString << " Amount: " << dynamicStringString << " Amount: " << dynamicStoneString << "\n";
        cout << "|0. Go Back  |   |  Price: ₱ " << dynamicWoodPPUString  << "  Price: ₱ " << dynamicIronPPUString  << "  Price: ₱ " << dynamicStringPPUString  << "  Price: ₱ " << dynamicStonePPUString  << "\n";
        cout << "|------------|   |                  |                  |                  |                  |\n";
        cout << "|                |   To Buy Enter   |   To Buy Enter   |   To Buy Enter   |   To Buy Enter   |\n";
        cout << "|                |                  |                  |                  |                  |\n";
        cout << "|                |         1        |         2        |         3        |         4        |\n";
        cout << "|                |                  |                  |                  |                  |\n";
        cout << "|                |__________________|__________________|__________________|__________________|\n";
        cout << "|                |                                                                           |\n";
        cout << "|                |                                 Feathers                                  |\n";
        cout << "|                |                                                                           |\n";
        cout << "|----------------|                             Amount: " << dynamicFeathersString << "\n";
        cout << "| Penguin Coins  |                              Price: ₱ " << dynamicFeathersPPUString << "\n";
        cout << "|                |                                                                           |\n";
        cout << "|                |                               To Buy Enter                                |\n";
        cout << "|     ₱ " << dynamicPenguinCoinString2 << "                                                                           |\n";
        cout << "|                |                                     5                                     |\n";
        cout << "|                |                                                                           |\n";
        cout << "|----------------|---------------------------------------------------------------------------|\n";
        cout << "Your Choice: ";
          
        
        cin >> choice2;
        if (stoi(choice2) == 1 && b.penguinCoin > b.woodPPU){
          b.wood++;
          b.penguinCoin = b.penguinCoin - b.woodPPU;
          b.transactions++;
        } else if (stoi(choice2) == 2 && b.penguinCoin > b.ironPPU){
          b.iron++;
          b.penguinCoin = b.penguinCoin - b.ironPPU;
          b.transactions++;
        } else if (stoi(choice2) == 3 && b.penguinCoin > b.stringPPU){
          b.string++;
          b.penguinCoin = b.penguinCoin - b.stringPPU;
          b.transactions++;
        } else if (stoi(choice2) == 4 && b.penguinCoin > b.stonePPU){
          b.stone++;
          b.penguinCoin = b.penguinCoin - b.stonePPU;
          b.transactions++;
        } else if (stoi(choice2) == 5 && b.penguinCoin > b.feathersPPU){
          b.feathers++;
          b.penguinCoin = b.penguinCoin - b.feathersPPU;
          b.transactions++;
        } else {
          if (stoi(choice2) == 1){ // informs the player if they are to broke to purchase the item they want
            cout << "You dont have enough Penguin Conis to purchase wood\nPlease try again.\n";
            string garbage;
            cin >> garbage;
          } else if (stoi(choice2) == 2){
            cout << "You dont have enough Penguin Coins to purchase iron\nPlease try again.\n";
            string garbage;
            cin >> garbage;
          } else if (stoi(choice2) == 3){
            cout << "You dont have enough Penguin Coins to purchase string\nPlease try again.\n";
            string garbage;
            cin >> garbage;
          } else if (stoi(choice2) == 4){
            cout << "You dont have enough Penguin Coins to purchase stone\nPlease try again.\n";
            string garbage;
            cin >> garbage;
          } else if (stoi(choice2) == 5){
            cout << "You dont have enough Penguin Coins to purchase feathers\nPlease try again.\n";
            string garbage;
            cin >> garbage;
          }
  
        }

        if (b.transactions == 10){ // the 10:1 transactions to date thingy
          b.date++;
          b.transactions = 0;
        }


        
      }while(stoi(choice2) != 0);
    }

    if (b.transactions == 10){ // the 10:1 transactions to date thingy
      b.date++;
      b.transactions = 0;
    }
    
  }while(stoi(choice) != 0);

  return b; // return the inventory variable to pass the same values around the functions
}

inventory crafting(inventory b){ // crafting woo!

  string choice;
  
  do{

    string dynamicDateString = dynamicPrinting(5, b.date);
    string dynamicWoodString = dynamicPrinting(7, b.wood);
    string dynamicIronString = dynamicPrinting(7, b.iron);
    string dynamicStoneString = dynamicPrinting(7, b.stone);
    string dynamicCrossbowString = dynamicPrinting(7, b.crossbow);
    string dynamicArrowsString = dynamicPrinting(8, b.arrows);
    string dynamicBluntSwordString = dynamicPrinting(7, b.bluntSword);
    string dynamicSharpSwordString = dynamicPrinting(7, b.sharpSword);
    string dynamicArmorString = dynamicPrinting(7, b.armor);
    string dynamicStringString = dynamicPrinting(7, b.string);
    string dynamicFeathersString = dynamicPrinting(7, b.feathers);
    string dynamicSharpeningWheelsAmountString = dynamicPrinting(8, b.sharpWheels.amount);
    
    system("clear");

    cout << "|-----------------------------------------------------------------------------|--------------|\n";
    cout << "|                        Your Emperor Penguin Workshop™                       |    Day: " << dynamicDateString << "\n";
    cout << "|-----------------------------------------------------------------------------|--------------|\n";
    cout << "|                                                                                            |\n";
    cout << "|                |-------------------------|------------------------|------------------------|\n";
    cout << "|------------|   |    Sharpening Wheels    |      Blunt Sword       |      Sharp Sword       |\n";
    cout << "|  Choices   |   |         Recipe:         |        Recipe:         |        Recipe:         |\n";
    cout << "|------------|   |         2 wood          |         1 wood         |     1 blunt sword      |\n";
    cout << "|0. Exit     |   |       + 1 iron          |       + 2 iron         |   + 1 sharpening wheel |\n";
    cout << "|------------|---|       + 1 stone         |                        |                        |\n";
    cout << "|      Wood      |       + 1 string        |                        |                        |\n";
    cout << "| Amount: " << dynamicWoodString << " Current Amount: " << dynamicSharpeningWheelsAmountString << " Current Amount: " << dynamicBluntSwordString << " Current Amount: " << dynamicSharpSwordString << "\n";
    cout << "|________________|   To Craft(+1) Enter    |   To Craft(+1) Enter   |   To Craft(+1) Enter   |\n";
    cout << "|      Iron      |            1            |            2           |            3           |\n";
    cout << "| Amount: " << dynamicIronString << "_________________________|________________________|________________________|\n";
    cout << "|________________|         Arrows          |        Crossbow        |          Armor         |\n";
    cout << "|      Stone     |         Recipe:         |         Recipe:        |         Recipe:        |\n";
    cout << "| Amount: " << dynamicStoneString << "         30 wood         |         3 wood         |         8 iron         |\n";
    cout << "|________________|       + 10 stone        |       + 2 iron         |                        |\n";
    cout << "|     String     |       + 30 feathers     |       + 1 string       |                        |\n";
    cout << "| Amount: " << dynamicStringString << "                         |                        |                        |\n";
    cout << "|________________|                         |                        |                        |\n";
    cout << "|    Feathers    | Current Amount: " << dynamicArrowsString << " Current Amount: " << dynamicCrossbowString << " Current Amount: " << dynamicArmorString << "\n";
    cout << "| Amount: " << dynamicFeathersString << "   To Craft(+10) Enter   |   To Craft(+1) Enter   |   To Craft(+1) Enter   |\n";
    cout << "|                |            4            |            5           |            6           |\n";
    cout << "|----------------|-------------------------|------------------------|------------------------|\n";
    cout << "Your Choice: ";

    
    cin >> choice;
    if (stoi(choice) == 2){
      if(b.wood >= 1 && b.iron >= 2){
        b.bluntSword++;
        b.wood = b.wood - 1;
        b.iron = b.iron - 2;
        b.transactions++;
      } else{
        cout << "you do not have enough wood or iron to craft this item\n";
        string garbage;
        cin >> garbage;
      }
    } else if (stoi(choice) == 1){
      if (b.wood >= 2 && b.stone >= 1 && b.string >= 1 && b.iron >= 1){
        b.sharpWheels.amount++;
        b.sharpWheels.uses = b.sharpWheels.uses + 10;
        b.wood = b.wood -2;
        b.string = b.string -1;
        b.stone = b.stone -1;
        b.iron = b.iron -1;
        b.transactions++;
      } else{ // informs player if they do not have the required materials to craft said item
        cout << "you do not have enough iron, string, wood or stone to craft this item\n";
        string garbage;
        cin >> garbage;
      }
    } else if (stoi(choice) == 6){
      if (b.iron >= 8){
        b.armor ++;
        b.iron = b.iron - 8;
        b.transactions++;
      } else{ // informs player if they do not have the required materials to craft said item
        cout << "you do not have enough iron to craft this item\n";
        string garbage;
        cin >> garbage;
      }
    } else if (stoi(choice) == 4){
      if(b.wood >= 30 && b.stone >= 10 && b.feathers >= 30){
        b.arrows = b.arrows + 10;
        b.wood = b.wood - 30;
        b.stone = b.stone - 10;
        b.feathers = b.feathers - 30;
        b.transactions++;
      } else{ // informs player if they do not have the required materials to craft said item
        cout << "you do not have enough wood, stone or feathers to craft this item\n";
        string garbage;
        cin >> garbage;
      }
    } else if (stoi(choice) == 5){
      if (b.wood >= 3 && b.iron >= 2 && b.string >= 1){
        b.crossbow++;
        b.wood = b.wood - 3;
        b.iron = b.iron - 2;
        b.string = b.string - 1;
        b.transactions++;
      } else{ // informs player if they do not have the required materials to craft said item
        cout << "you dont have enought wood, iron or string to craft this item\n";
        string garbage;
        cin >> garbage;
      }
    } else if (stoi(choice) == 3){
      if (b.bluntSword >= 1 && b.sharpWheels.amount >= 1){
        b.sharpSword++;
        b.sharpWheels.uses--;
        b.bluntSword--;
        b.transactions++;
      } else{ // informs player if they do not have the required materials to craft said item
        cout << "you dont have enough blunt swords or sharpening wheels to craft this item\n";
        string garbage;
        cin >> garbage;
      }
    }

    if (b.sharpWheels.uses == (b.sharpWheels.amount * 10) - 10){ // this checks to see if one sharpening has been fully used
      b.sharpWheels.amount--;                                    // basically one sharpening wheel has ten uses until it breaks
    }                                                            // this checks if ten of all of its potential uses have been used up before removing one from the
                                                                 // amount variable
    if (b.transactions == 10){ // the 10:1 transactions to date thingy
      b.date++;
      b.transactions = 0;
    }
    
  }while(stoi(choice) != 0);

  return b; // returns the inventory variable for use between other functions
}


void mainMenu(bool loadExisting, string URL, string JWT){
  string choice;
  
  bool won = false;
  bool continueGame = false;

  inventory b; // create the inventory
  saveStateManager saveTool; // get the game save state code ready

  if (!loadExisting){ // check if the user does not want to continue their game
    b = setDefault(); // set the inventory values to default and create the key-value pairs on the database
    saveTool.initSaveState(URL, JWT);
  } else {
    b = saveTool.loadSave(URL, JWT); // if they do want to, load the pre-existing gamedata from the database
  }
  
  do{

    if ((b.sharpSword == b.neededSharpSword || b.bluntSword == b.neededBluntSword) && b.arrows == b.neededArrows && b.crossbow == b.neededCrossbow && b.armor == b.neededArmor && !won && !continueGame){ // check if the win conditions are satisfied
      

      string continueDecision;
      bool incorrectInput;
      
      do {

        incorrectInput = false;
        
        system("clear");
        cout << "  ___    ___ ________  ___  ___          ___       __   ________  ________   ___  ___\n";
        cout << " |\\  \\  /  /|\\   __  \\|\\  \\|\\  \\        |\\  \\     |\\  \\|\\   __  \\|\\   ___  \\|\\  \\|\\  \\ \n";
        cout << " \\ \\  \\/  / | \\  \\|\\  \\ \\  \\\\\\  \\       \\ \\  \\    \\ \\  \\ \\  \\|\\  \\ \\  \\\\ \\  \\ \\  \\ \\  \\ \n";
        cout << "  \\ \\    / / \\ \\  \\\\\\  \\ \\  \\\\\\  \\       \\ \\  \\  __\\ \\  \\ \\  \\\\\\  \\ \\  \\\\ \\  \\ \\  \\ \\  \\ \n";
        cout << "   \\/  /  /   \\ \\  \\\\\\  \\ \\  \\\\\\  \\       \\ \\  \\|\\__\\_\\  \\ \\  \\\\\\  \\ \\  \\\\ \\  \\ \\__\\ \\__\\ \n";
        cout << " __/  / /      \\ \\_______\\ \\_______\\       \\ \\____________\\ \\_______\\ \\__\\\\ \\__\\|__|\\|__| \n";
        cout << "|\\___/ /        \\|_______|\\|_______|        \\|____________|\\|_______|\\|__| \\|__|   ___  ___ \n";
        cout << "\\|___|/                                                                           |\\__\\|\\__\\ \n";
        cout << "                                                                                  \\|__|\\|__|\n\n";

        cout << "Would you like to continue [Y]es/[n]o: ";
        cin >> continueDecision;
        
        if (continueDecision == "Y"){
          won = false;
          continueGame = true;
        } else if (continueDecision == "n"){
          won = true;
          continueGame = false;
        } else { // error message if an incorrect input is entered
          cout << "You have entered an incorrect input.\nInput can only be Y for yes or n for no, not " << continueDecision << ".\nPress any key + enter to continue.";
          incorrectInput = true;
          string garbage;
          cin >> garbage;
        }
        
      }while(incorrectInput);
    } else { // if they are not, or you want to continue playing, the main game loop runs.

      string dynamicPenguinCoinString = dynamicPrinting(10, b.penguinCoin);
      string dynamicWoodString = dynamicPrinting(9, b.wood);
      string dynamicIronString = dynamicPrinting(9, b.iron);
      string dynamicStoneString = dynamicPrinting(9, b.string);
      string dynamicCrossbowString = dynamicPrinting(9, b.crossbow);
      string dynamicBluntSwordString = dynamicPrinting(9, b.bluntSword);
      string dynamicSharpSwordString = dynamicPrinting(9, b.sharpSword);
      string dynamicArmorString = dynamicPrinting(9, b.armor);
      string dynamicStringString = dynamicPrinting(9, b.string);
      string dynamicFeathersString = dynamicPrinting(9, b.feathers);
      string dynamicArrowsString = dynamicPrinting(9, b.arrows);
  
      string dynamicSharpWheelAmountString = dynamicPrinting(23, b.sharpWheels.amount);
      string dynamicSharpWheelUsesString = dynamicPrinting(23, b.sharpWheels.uses);
  
      string dynamicDateString = dynamicPrinting(5, b.date);
      
      system("clear");
      cout << "|-----------------------------------------------------------------------------|--------------|\n";
      cout << "|                        Penguin Army Armorer Tycoon                          |    Day: "<< dynamicDateString <<  "\n";
      cout << "|-----------------------------------------------------------------------------|--------------|\n";
      cout << "|                                                                                            |\n";
      cout << "|                |------------------|------------------|------------------|------------------|\n";
      cout << "|------------|   |  Penguin Coins   |       Wood       |       Iron       |      Stone       |\n";
      cout << "|  Choices   |   |                  |                  |                  |                  |\n";
      cout << "|------------|   |                  |                  |                  |                  |\n";
      cout << "|0. Save+Exit|   |      ₱ " << dynamicPenguinCoinString << " Amount: " << dynamicWoodString << " Amount: " << dynamicIronString << " Amount: "<< dynamicStoneString <<"\n";
      cout << "|1. Exit     |   |                  |                  |                  |                  |\n";
      cout << "|2. Store    |   |                  |                  |                  |                  |\n";
      cout << "|3. Crafting |   |------------------|------------------|------------------|------------------|\n";
      cout << "|4. Next Day |   |     Crossbow     |      Arrows      |   Blunt Sword    |   Sharp Sword    |\n";
      cout << "|------------|   |                  |                  |                  |                  |\n";
      cout << "|                |                  |                  |                  |                  |\n";
      cout << "|                | Amount: " << dynamicCrossbowString << " Amount: " << dynamicArrowsString << " Amount: " << dynamicBluntSwordString << " Amount: " << dynamicSharpSwordString << "\n";
      cout << "|                |                  |                  |                  |                  |\n";
      cout << "|                |                  |                  |                  |                  |\n";
      cout << "|----------------|------------------|------------------|------------------|------------------|\n";
      cout << "|         Sharpening Wheels         |      Armor       |      String      |     Feathers     |\n";
      cout << "|    Amount: " << dynamicSharpWheelAmountString << "                  |                  |                  |\n";
      cout << "|    Uses:   " << dynamicSharpWheelUsesString << "                  |                  |                  |\n";
      cout << "|                                   | Amount: " << dynamicArmorString << " Amount: " << dynamicStringString << " Amount: " << dynamicFeathersString << "\n";
      cout << "|                                   |                  |                  |                  |\n";
      cout << "|                                   |                  |                  |                  |\n";
      cout << "|-----------------------------------|------------------|------------------|------------------|\n";
      cout << "Your Choice: ";
      cin >> choice;
      if(choice == "0") {saveTool.saveState(b, URL, JWT); break;} // if you choice Save+Exit, it first saves then breaks out of the loop
      if(choice == "1") break; // if you choose Exit, you break out of the loop without saving
      if(choice == "2") b = store(b);   // choice logic
      if(choice == "3") b = crafting(b); // the returned inventory variables are outputted here and are stored in the main game loop. 
      if(choice == "4") {b.date++; b.transactions = 0;}        // this means that the same variable can be passed through functions to give it the inventory variables.
  
      if (b.transactions == 10){ // the 10:1 transactions to date thingy
        b.date++;
        b.transactions = 0;
      }
    }
    
    
  }while((!won || continueGame) && choice != "0");
}

int main() {

  saveStateManager saveCheck; // create an instance of the save state manager JUST for checking if there is or is not a pre-existing game
  string URL = getenv("REPLIT_DB_URL"); // get the url of the database
  string JWT;
  size_t pos = URL.find_last_of("/");  // copy and save the JWT from the url --> just happens to be part of the path
  if (pos != string::npos) {
    JWT = URL.substr(pos);
  }

  string choice;

  bool loadSave = saveCheck.checkForExistingGame(URL, JWT); // run the check
  
  cout << " '||''''|                                                   '||'''|,                                               \n" // display cool graphic
"  ||   .                                                     ||   ||                                  ''           \n"
"  ||'''|  '||),,(|,  '||''|, .|''|, '||''| .|''|, '||''|     ||...|' .|''|, `||''|,  .|''|, '||  ||`  ||  `||''|,  \n"
"  ||       || || ||   ||  || ||..||  ||    ||  ||  ||        ||      ||..||  ||  ||  ||  ||  ||  ||   ||   ||  ||  \n"
" .||....| .||    ||.  ||..|' `|...  .||.   `|..|' .||.      .||      `|...  .||  ||. `|..||  `|..'|. .||. .||  ||. \n"
"                      ||                                                                 ||                        \n"
"                     .||                                                              `..|'                        \n"
"                           |''||''|                                                                                \n"
"                              ||                                                                                   \n"
"                              ||    '||  ||` .|'', .|''|, .|''|, `||''|,                                           \n"
"                              ||     `|..||  ||    ||  || ||  ||  ||  ||                                           \n"
"                             .||.        ||  `|..' `|..|' `|..|' .||  ||.                                          \n"
"                                      ,  |'                                                                        \n"
"                                       ''                                                                          \n";

  if (!loadSave){ // never played before? then you can only create a new game
    cout << "                                           Press Any Key to Play                                                   \n";
  } else{ // if you have played before you can create a new game, or continue an old one
    cout << "                                               1) New Game                                                         \n"
            "                                             2) Continue Game                                                      \n";
  }
  cin >> choice; // get users choice

  if (loadSave && choice == "1"){ // choice only matters if they have a pre-existing game right? and if the user wants to create a new game there is no point of loading the old one
    loadSave = false;
  }
  
  mainMenu(loadSave, URL, JWT); // runs the main game loop
}