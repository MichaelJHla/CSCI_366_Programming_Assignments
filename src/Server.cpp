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
#include "Server.hpp"


/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file){
}

//This method is used to initialize the server using the two player boards
//If any of the player boards are not the correct size then a server exception is thrown
void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board){
    this->board_size = board_size;//Assigns the board size to the global variable

    //Checks if the passed board_size is valid
    if (board_size != BOARD_SIZE){
        throw ServerException("Incorrect Board Size");
    }

    //Turns each text file of the board into a string
    ifstream p1Board(p1_setup_board);
    string p1Str((std::istreambuf_iterator<char>(p1Board)), std::istreambuf_iterator<char>());
    ifstream p2Board(p2_setup_board);
    string p2Str((std::istreambuf_iterator<char>(p2Board)), std::istreambuf_iterator<char>());

    //If a board does not exist throw a ServerException
    if (!p1Board || !p2Board){
        throw ServerException("Missing board");
    }

    //Checks the length of the boards to make sure they are large enough
    if (p1Str.length() < BOARD_SIZE){
        throw ServerException("No board provided");
    }
    if (p2Str.length() < BOARD_SIZE){
        throw ServerException("No board provided");
    }
}

Server::~Server() {
}

BitArray2D *Server::scan_setup_board(string setup_board_name){
    BitArray2D *board = new BitArray2D(board_size, board_size);
    ifstream boardFile(setup_board_name);
    if (!boardFile){
        throw ServerException("Bad board name");
    }

    string line;
    for (int i = 0; i < board_size; i++){
        getline(boardFile, line);
        for (int j = 0; j<line.length(); j++){
            for (char ship : SHIPS){
                if (line.at(j) == ship){
                    board->set(j, i); //Reverse the order since i represents the current line
                }
            }
        }
    }
    return board;
}

//This method is used to evaluate one player's shot versus the opponents board
//First it checks to make sure the player number is a valid number
//Then it checks if the shot is in bounds or not
//Then it takes the shot coordinates to the board to see if its a hit or a miss
int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    //Checks if the player number is valid or not
    if (player < 1 || player > MAX_PLAYERS){
        throw ServerException("Invalid player number");
    }

    //The shot is out of bounds
    if (x >= board_size || x < 0 || y >= board_size || y < 0){
        return OUT_OF_BOUNDS;
    }

    //Switches the player number so the correct board is selected
    char shot;
    if (player == 1){
        shot = p2_setup_board->get(x, y);
    } else {
        shot = p1_setup_board->get(x, y);
    }

    if (shot == 1){
        return HIT;
    } else{
        return MISS;
    }
}

//This method is used to take a shot file and deserialize them to x,y coordinates which are then passed to evaluate_shot
//  and process the results from evaluate_shot and serialize the result as a JSON to be used
int Server::process_shot(unsigned int player) {
    //Checks if the player number is valid or not
    if (player < 1 || player > MAX_PLAYERS){
        throw ServerException("Invalid player number");
    }

    //Checks if the actual file exists
    string fileName = "player_" + to_string(player) + ".shot.json"; //String is separated so it can be removed later
    ifstream shotFile(fileName);
    if (!shotFile){//Checks if there is an actual file present
        return NO_SHOT_FILE;
    }
    //Deserializes the JSON
    cereal::JSONInputArchive readFile(shotFile);

    int x, y; //variables used to store the deserialized shot coordinates
    readFile(y, x);

    //Clearing
    shotFile.close();
    remove(fileName.c_str());

    //This block serializes the code to a result.JSON file
    ofstream resultFile("player_" + to_string(player) + ".result.json");
    cereal::JSONOutputArchive cerealArchive(resultFile);
    int result = evaluate_shot(player, x, y);
    cerealArchive(CEREAL_NVP(result));
    return SHOT_FILE_PROCESSED;
}