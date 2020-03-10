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

#include "common.hpp"
#include "Client.hpp"

Client::~Client() {
}


void Client::initialize(unsigned int player, unsigned int board_size){
    if (player==2 || player==1){ //Checks if the player number is valid
        this->player = player; //Assign valid player number to global player
        this->board_size = board_size; //assign board_size to global board_size

        //create 2D vector for the board
        vector<vector<int>> board(this->board_size, vector<int> (this->board_size, 0));

        //creates an output stream with the file name
        std::ofstream fileName("player_" + to_string(player) + ".action_board.json");

        //Serialize blank data to the file
        cereal::JSONOutputArchive cerealArchive(fileName);
        cerealArchive(CEREAL_NVP(board));

        initialized = true; //Notify the loop that the client has been initialized
    } else {
        throw ClientWrongPlayerNumberException(); //if not valid throw exception
    }
}


void Client::fire(unsigned int x, unsigned int y) {

}


bool Client::result_available() {

}


int Client::get_result() {

}



void Client::update_action_board(int result, unsigned int x, unsigned int y) {

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
            boardString += std::to_string(board[i][j]);
        }
        boardString += "\n";//Starts a new line after a row is finished
    }
    return boardString;//Returns the board as a string
}