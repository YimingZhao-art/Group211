//This file is used to be the main file of the game
//Following are libraries we need to use.
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <string>
#include <cctype>
#include <iomanip>
#include <vector>
//Following are libraries defined by us.
#include "data.h"
#include "initial.h"
#include "savegame.h"
#include "readgame.h"
#include "game.h"
#include "battle.h"


using namespace std;

MAGICS magics[10];
BOSS boss[4];
POKEMON pokemons[4];

USER user;

///display the mode
void display1() {
    cout << "-------------------------------------------------------------------\n" <<
            "Choose one of the modes(Please input the name of the mode): \n" <<
            "loadgame\n" <<
            "newgame\n" <<
            "exit\n" ;
}

//display the command of mode
void display() {
    cout << "-------------------------------------------------------------------\n" <<
            "Choose one of the commands(Please input the name of the mode): \n" <<
            "Battle\n" <<
            "Bag\n" <<
            "Games\n" <<
            "SaveAndExit\n";
}

//display the choices of game
void display_game(){
    cout << "-------------------------------------------------------------------\n" <<
            "Choose one of the games(Input the Number): \n" <<
            "1.Tic_Tac_Toe\n" <<
            "2.Guess\n\n" <<
            "0.Exit\n";
}

//game implementation
//read a USER, and do change on it
void game(USER &user){
    int command;
    display_game();
    cin >> command;
    while(command < 1 || command > 2)
    {
        cout<<"Game do not exist, input again."<<endl;
        cin >> command;
    }
    while ( command != 0 ){
        //Tic_tac-toe
        if ( command == 1 ){
            if ( Tic_Tac_Toe() ){
                cout << "Your win 10 golds and 5 train.\n";
                user.money += 10;
                user.train += 5;
            }
            else
                cout << "Your don't win.\n";
        }
        
        //Guess
        else if ( command == 2 ){
            if ( GUESS() ){
                cout << "Your win 10 golds and 5 train.\n";
                user.money += 10;
                user.train += 5;
            }
            else
                cout << "Your don't win.\n";
        }
        
        display_game();
        cin >> command;
        if ( command == 0 )
            return;
    }
}

//display the bag information
//user_name
//user_money
//user_train
//pokemon_name
//pokemon_level,experience
//pokemon_hp,hpmax
//pokemon_character
void display_bag(USER &user) {
    cout << "-------------------------------------------------------------------\n";
    cout << "User: " << user.name << endl;
    cout << "Money: " << user.money << endl;
    cout << "Train: " << user.train << endl;
    for ( int i = 0; i < user.number_of_pokeman; i++ ){
        cout <<"-------------------------------------------------------------------\n";
        cout << "name: "<< user.bag[i].name << ":\n";
        cout << "level: " << user.bag[i].level << ' ' << "experience: " << user.bag[i].experience <<endl;
        cout << "hp: " << user.bag[i].hp << ' ' << "hpmax: " << user.bag[i].hpmax << endl;
        cout << "character: " << user.bag[i].character << endl;
    }
}

//distribute the train to pokemon
void Level_up(USER &user){
    cout <<"-------------------------------------------------------------------\n";
    cout << "Choose the pokemon you want to level-up: (from 0 - " << user.number_of_pokeman-1 << ")\n";
    int i; //pokemon
    cin >> i;
    
    //in case of 10 already
    while ( user.bag[i].level == 10 ){
        cout << "This pokemon is 10 already!\n";
        cout << "Choose the pokemon you want to level-up: (from 0 - " << user.number_of_pokeman-1 << ")\n";
        cin >> i;
    }
    
    cout << "Your have " << user.train << " at most, how much do you want to distribute: \n";
    int j;//distribute quantity
    cin >> j;
    
    //in case of illegale input
    while ( j > user.train || j <= 0 ){
        cout << "Your have " << user.train << " at most, how much do you want to distribute: \n";
        cin >> j;
    }
    
    int left;//distribute left
    int to_10 = ( 9 - user.bag[i].level ) * 100 + user.bag[i].experience; //distance from 10
    
    if ( j >= to_10 ){
        left = j - to_10;
        cout << left << " are left." << endl;
        cout << user.bag[i].name << " is 10 now.\n";
        user.bag[i].level = 10;
        user.bag[i].experience = 0;
        user.train = left;
    }
    
    else {
        
        if ( j + user.bag[i].experience < 100 ){
            cout << "Added.\n";
            user.bag[i].experience += j;
            cout << user.bag[i].name << " is " << user.bag[i].experience << "/100.\n";
        }
        
        else {
            int level_up;
            level_up = ( j + user.bag[i].experience ) / 100;
            left = ( j + user.bag[i].experience ) % 100;
            user.bag[i].level += level_up;
            user.bag[i].experience = left;
            cout << level_up << " level-up!\n";
        }
        user.train -= j;
    }
    
    user.bag[i].hp = user.bag[i].level * 24;
    user.bag[i].hpmax = user.bag[i].level * 24;
}


//implement the bag
void Bag(USER &user){
    display_bag(user);
    string command;
    cout << "-------------------------------------------------------------------\n" <<
            "Choose one of the commands(Input the name of command): \n" <<
            "Recover\n" << 
            "DistributeTrain\n" <<
            "Exit\n";
    cin >> command;
    while ( command != "Exit" ){
        if ( command == "Recover" ){
            user.money -= 5;
            for ( int i = 0; i < user.number_of_pokeman; i++ )
                user.bag[i].hp = user.bag[i].hpmax;
        }
        else if ( command == "DistributeTrain" ){
            //insert the level-up
            Level_up(user);
        }
        display_bag(user);
        cout << "-------------------------------------------------------------------\n" <<
        "Choose one of the commands(Input the name of command): \n" <<
        "Recover\n" <<
        "DistributeTrain\n" <<
        "Exit\n";
        cin >> command;
    }
    return;
}











int main()
{
    initialmagics(magics); //Initialize the data of magics.
    initialboss(boss); //Initial the data of boss.
    initialpokemon(pokemons); //Initial the data of Pokemons.
    cout << "----------------------------POKEMON--------------------------------\n";
    
    while ( 1 ) //Used when first open the program.
    {
        display1(); //When first open the game, display the menu for user to choose: load game/new game/exit.
        string get;
        cout << "-------------------------------------------------------------------\n";
        cout << "Instruction:" <<endl;
        cin >> get; //Get the instruction from the user.
        
        
        if ( get == "loadgame" )
        {
            cout << "Input your user name: ";
            cin >> user.name;
            if ( read(user) != 0 ) 
                break; //Call the function to load the game.
        }
        
        
        else if ( get == "newgame" )
        {
            //newgame(); //Call the function to create new game.
            cout << "Input your user name: ";
            cin >> user.name;
            user.money = 100;
            user.train = 0;
            for (int i = 0; i < 4; i++)
            {
                user.bossstatuse[i] = 0;
                user.capturestatus[i] = 0;
            }
            cout << "Choose one of the pokemons(input from 0-3):\n";
            for ( int i = 0; i < 4; i++ ){
                cout << i << ": " << pokemons[i].name <<endl;
            }
            
            int x;
            cin >> x;
            
            user.number_of_pokeman = 1;
            user.bag.push_back(pokemons[x]);
            user.capturestatus[x] = 1;
            break;
        }
        
        
        else if ( get == "exit" ){
            return 0; //shutdown the game.
        }
        
        else
            cout << "Wrong instruction! Please try again(loadgame, newgame, exit)." << endl;
    }
    

    //The following part would be main body of the program.
    string command;
    display();
    cin >> command;
    if ( command == "SaveAndExit" ){
        save(user);
    }
    while ( command != "SaveAndExit" ){
        //battle
        if ( command == "Battle" ){
            cout << "-------------------------------------------------------------------\n";
            cout<< "Choose battle type(please enter the number of the command):\n"
                << "1. Fight creeps\n"
                << "2. Fight Boss\n";
            int type;
            cin >> type;
            while( type !=1 && type != 2 )
            {
                cout<<"Wrong command, try again!\n";
                cin>>type;
            }
            if( type == 1 )
                normalbattle( user, pokemons, magics);
            else
                bossbattle( user, boss, magics );
            cout << "press \"Enter\" to continue" << endl;
            getchar();
        }
        
        
        //Bag
        else if ( command == "Bag" ){
            Bag( user );
            cout << "press \"Enter\" to continue" << endl;
            getchar();
        }
        
        
        //Game
        else if ( command == "Games" ){
            game( user );
            cout << "press \"Enter\" to continue" << endl;
            getchar();
        }
        
        
        display();
        cin >> command;
        if ( command == "SaveAndExit" )
            save(user);
    }
    return 0;
}