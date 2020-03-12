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
#include "Server.hpp"

/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file){

}


void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board){
    this->board_size = board_size;
    if (board_size != BOARD_SIZE){
        throw ServerException("Incorrect Board Size");
    }
    if (p1_setup_board.length() < 1){
        throw ServerException("No board provided");
    }
    if (p2_setup_board.length() < 1){
        throw ServerException("No board provided");
    }

}


int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    if (x>board_size || x < 0){
        return OUT_OF_BOUNDS;
    } else if (y>board_size || y < 0){
        return OUT_OF_BOUNDS;
    }

    string line;
    ifstream boardFile("player_" + to_string(player) + ".setup_board.txt");
    string arrayOfBoard[board_size];
    int i = 0;
    if (boardFile){
        while (getline(boardFile, line)){
            arrayOfBoard[i] = line;
            i++;
        }
    }
    if (arrayOfBoard[y].at(x) != '_'){
        return HIT;
    } else{
        return MISS;
    }
}


int Server::process_shot(unsigned int player) {
    ifstream shotFile("player_" + to_string(player) + ".shot.json");
    if (!shotFile){
        return NO_SHOT_FILE;
    }
    cereal::JSONInputArchive readFile(shotFile);
    int x, y;
    readFile(x, y);
    ofstream resultFile("player_" + to_string(player) + ".result.json");
    cereal::JSONOutputArchive cerealArchive(resultFile);
    if (player ==1){
        cerealArchive(CEREAL_NVP(evaluate_shot(2, x, y)));
    } else {
        cerealArchive(CEREAL_NVP(evaluate_shot(1, x, y)));
    }
    return SHOT_FILE_PROCESSED;
}