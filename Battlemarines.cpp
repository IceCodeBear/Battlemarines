

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

// Global variables to avoid having to pass many variables to functions
const bool DEBUG = true; //set to true to show the location of the submarines
const int ROWS = 10; //number of rows
const int COLS = 10; //number of columns
const int SUBS = 5; // number of submarines
const int RANGE = 9; //furthest distance to detect sub.
char ocean[ROWS][COLS]; //

//creates the structure Location
struct Location{

    int r; // row
    int c; // col
};

vector<Location> subs_list;
vector<Location> drop_list;

// Prototypes
void welcome(); //prints welcome message
char toChar(int n); // converts int to char
void setBoard(Location x); // assign 's' and '.'
void setOcean(); //create the 10 x 10 board
int findShortestDistance(int r, int c); //gets the shortest distance
void printBoard(); //prints out the board
void addSubs(); //assign random coordinates to 5 submarines
Location getDepthChargeLocation (); //ask user to input data
bool isVaildLocation(Location x); // return true if the location is valid
bool inDropsList(Location x); //returns true if location is in drop_list
bool inSubsList(Location x); // returns true if location is in subs_list
void updateBoard(); //recalculate the distance and print out a new board
void submarineSunk(Location x); // print the coordinates of the submarine that sunk
void play(); //ask user to enter the coordinates
bool gameOver(); // return true if all 5 submarine are found

int main(){

    welcome();
    setOcean();
    while (!gameOver()){

        printBoard();
        play();
    }

    printBoard();
    cout << "!!! C O N G R A T U L A T I O N S !!!" << endl;
    cout << "      You found all 5 submarines     " << endl;
    cout << "*************************************" << endl;

    return 0;
}

//This function prints out the welcome message
void welcome(){

    cout << "**********************************" << endl;
    cout << "              Welcome             " << endl;
    cout << "            Lets begin            " << endl;
    cout << "**********************************" << endl;

}

//this function finds the distance from location (r,c) to the nearest submarine
int findShortestDistance(int r, int c){

    int dist, shortest = 10;
    int sr, sc; // submarine coordinates
    if (subs_list.size()==0)
        return 0;
    for (unsigned i = 0; i< subs_list.size(); i++)
        {
            sr = subs_list.at(i).r;
            sc = subs_list.at(i).c;
            dist = abs(r-sr)+ abs(c-sc);
            if (dist < shortest)
                shortest = dist;
        }
        return shortest;

}

//this function converts int to char
char toChar(int n){

    char c = '0'+ n;
    return c;
}

//this function prints out the area of th ocean
void printBoard(){

    cout << endl;
    cout << "_____________________" << endl;
    cout << "  ";
    for(int n = 0; n < ROWS; n++){
        cout << n << " ";
    }
    cout << endl;

    for (int i = 0; i < ROWS; i++){

        cout << i << " ";
        for (int j = 0; j < COLS; j++){
            cout << ocean[i][j] << " ";
        }
        cout << endl;
    }
    cout << "_____________________" << endl;

}

// assign 'S' or '.' to coordinates
void setBoard(Location x){

    if (!DEBUG && inSubsList(x)){
        ocean[x.r][x.c] = 'S';
    }
    else{
        ocean[x.r][x.c] = '.';
    }

}

//this function generates the game board
void setOcean(){

    Location coordinates;
    addSubs(); //place 5 submarine in random coordinates.

    for (int row = 0; row < ROWS; row++){

        for (int col = 0; col < COLS; col++){

            coordinates.r = row;   //temporary coordinates for row
            coordinates.c = col;   //temporary coordinates for col

            setBoard(coordinates); //assign '.' and 'S' to coordinates
        }

    }
}

//this function creates random submarine coordinates
void addSubs(){

    srand(time(0));
    Location coordinates;

    for (int i = 0; i < SUBS; i++){

        do{

            coordinates.r = rand() % ROWS; //assign a random number to r
            coordinates.c = rand() % COLS; //assign a random number to c

        }while(inSubsList(coordinates) != false);

        subs_list.push_back(coordinates); //fills subs_list
    }

}

//verify that the coordinates are in the board
bool isVaildLocation(Location x){

    if(x.r < ROWS && x.r >= 0){
        if(x.c < COLS && x.c >= 0){
            if(!inDropsList(x)){
                return true;
            }
        }
    }
    return false;

}

//Verify that the coordinates are in the drop_list
bool inDropsList(Location x){

    for (int i = 0; i < drop_list.size(); i++){

        if (drop_list.at(i).r == x.r){
            if (drop_list.at(i).c == x.c){
                return true;
            }
        }
    }
    return false;
}

//verify that the coordinates are in the subs_list
bool inSubsList(Location x){

    for (int i = 0; i < subs_list.size(); i++){

        if (subs_list.at(i).r == x.r){
            if(subs_list.at(i).c == x.c){
                return true;
            }
        }
    }
    return false;

}

//if a submarine is hit, the symbol '#' will be printed
void updateBoard(){

    for (int i = 0; i < drop_list.size(); i++){

        if (ocean[drop_list.at(i).r][drop_list.at(i).c] != '#'){
            if(findShortestDistance(drop_list.at(i).r, drop_list.at(i).c) > RANGE){
                ocean[drop_list.at(i).r][drop_list.at(i).c ] = '0'; // out of range
            }
            else if (findShortestDistance(drop_list.at(i).r, drop_list.at(i).c) <= RANGE && findShortestDistance(drop_list.at(i).r, drop_list.at(i).c) >= 0){
                ocean[drop_list.at(i).r][drop_list.at(i).c] = toChar(findShortestDistance(drop_list.at(i).r, drop_list.at(i).c));
            }
        }
    }
}
//ask the user to input the number for row and col
Location getDepthChargeLocation(){

    int row;
    int col;
    Location locate;
    cout << endl;

    do{

        cout << "Enter the r c coordinates (separated by a space) to hit: ";
        cin >> row;
        cin >> col;

        locate.r = row; //assign row to locate
        locate.c = col; //assign col to locate

        if (!isVaildLocation(locate)){
            cout << "The location is invalid. Try Again" << endl;
        }
        cout << endl;

    }while (isVaildLocation(locate) == false);

    return locate;
}

//prints out the location of the submarine that was hit
void submarineSunk(Location x){

    int count = 0;

    if(inSubsList(x) != false){

        cout << "!! Kaboom, Submarine has sunk at position (" << x.r << ", " << x.c << ") !!" << endl;
        count++;

        for (int i = 0; i < subs_list.size(); i++){

            if (subs_list.at(i).r == x.r){ //check if row matches one of the submarines
                if(subs_list.at(i).c == x.c){ //check if col matches one of the submarines
                    subs_list.erase(subs_list.begin() + i);
                }
            }
        }
        ocean[x.r][x.c] = '#'; //replace 'S' with '#'
    }
    else{
        cout << "No submarines was hit at position (" << x.r << ", " << x.c << ")" << endl;
    }

}

//run the coordinates to check whether or not the submarine is at that location.
void play(){

    Location getCoord = getDepthChargeLocation();
    submarineSunk(getCoord); //prints the position that the submarine was hit
    drop_list.push_back(getCoord);  //add location to the drop_list
    updateBoard(); //change the value of the distance to the nearest active submarine

}

//returns true if all the submarines are accounted
bool gameOver(){

    if(subs_list.size() != 0){
        return false;
    }

    return true;

}
