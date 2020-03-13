//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

//Help sources used:
//https://uscilab.github.io/cereal/serialization_archives.html
//https://stackoverflow.com/questions/47789927/how-to-serialize-multidimensional-array-on-cereal-c-serialize-library?rq=1
//https://www.javatpoint.com/cpp-int-to-string
//https://www.geeksforgeeks.org/2d-vector-in-cpp-with-user-defined-size/
//http://www.cplusplus.com/forum/beginner/11304/

#include "common.hpp"
#include "Client.hpp"

Client::~Client() {
}

//This method initializes the board as empty
//It first checks if the player number is valid
//  then it outputs a blank 2D vector to a JSON file using cereal
void Client::initialize(unsigned int player, unsigned int board_size){
    if (player > 0 && player < MAX_PLAYERS){ //Checks if the player number is valid
        this->player = player; //Assign valid player number to global player
        this->board_size = board_size; //assign board_size to global board_size

        //create 2D vector which represents the board
        vector<vector<int>> board(this->board_size, vector<int> (this->board_size, 0));

        //creates an output stream with the file name
        ofstream fileName("player_" + to_string(player) + ".action_board.json");

        //Serialize blank data to the file
        cereal::JSONOutputArchive cerealArchive(fileName);
        cerealArchive(CEREAL_NVP(board));

        initialized = true; //Notify the loop that the client has been initialized
    } else {
        throw ClientWrongPlayerNumberException(); //if not valid throw exception
    }
}

//This method is used to make a JSON file which sends what coordinates are used for a shot
void Client::fire(unsigned int x, unsigned int y) {
    //Serialize the shot data to the shot.json file
    ofstream shotFile("player_" + to_string(player) + ".shot.json");
    cereal::JSONOutputArchive cerealArchive(shotFile);
    cerealArchive(CEREAL_NVP(x), CEREAL_NVP(y));
}

//Checks to see if a JSON with the results of a shot is available to be opened
bool Client::result_available() {
    ifstream resultFile("player_" + to_string(player) + ".result.json");
    if (resultFile){ //Checks if the file was opened succesfully/exists
        return true;
    } else {
        return false;
    }
}

//This method is used to deserialize the result file and pass the result off to be used by a different method
int Client::get_result() {
    if (result_available()){ //Checks if a result has been returned yet
        //Deserialize the file
        ifstream resultFile("player_" + to_string(player) + ".result.json");
        cereal::JSONInputArchive readFile(resultFile);

        int result;//Variable used to store the result data
        readFile(result);//Adds the data from the file to the variable
        if (result != HIT && result != MISS && result != OUT_OF_BOUNDS){ //checks to make sure the result is valid
            throw ClientException("Invalid Result");//If not throw a client exception
        }
        return result;
    }
}

//This method is used to update the active action board with the result of the shot by substituting HIT or MISS
//It does this by deserializing the json file to a 2d vector
//After the board is in a 2D vector the x and y coordinate of the shot are updated with the result
//This updated board is then serialized to a json file that represents the updated action board
void Client::update_action_board(int result, unsigned int x, unsigned int y) {
    //create 2D vector for the board
    vector<vector<int>> board(board_size, vector<int> (board_size, 0));

    //deserialize the file
    ifstream inputFileName("player_" + to_string(player) + ".action_board.json");
    cereal::JSONInputArchive readFile(inputFileName);
    readFile(board);

    board[x][y] = result;//update the board with the result

    //serialize the board to a JSON
    ofstream outputFileName("player_" + to_string(player) + ".action_board.json");
    cereal::JSONOutputArchive writeFile(outputFileName);
    writeFile(CEREAL_NVP(board));
}

//This method reads a JSON file and deserializes the JSON
//It then converts the information to a string and returns that string
string Client::render_action_board(){
    ifstream fileName(to_string(player) + ".action_board.json"); //Creates input stream from file
    cereal::JSONInputArchive readFile(fileName); //deserialize the file

    //2D vector to store the file information
    vector<vector<int>> board(board_size, vector<int> (board_size, 0));
    readFile(board); //outputs the deserialized file to the board vector

    string boardString;//String that will store the board
    for (int i = 0; i < board_size; i++){
        for (int j = 0; j < board_size; j++){
            boardString += to_string(board[i][j]);
        }
        boardString += "\n";//Starts a new line after a row is finished
    }
    return boardString;//Returns the board as a string
}