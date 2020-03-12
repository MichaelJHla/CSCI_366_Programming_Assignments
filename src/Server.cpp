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

}


int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    if (x>board_size || x < 1){
        return OUT_OF_BOUNDS;
    } else if (y>board_size || y < 1){
        return OUT_OF_BOUNDS;
    }

}


int Server::process_shot(unsigned int player) {
    ifstream shotFile("player_" + to_string(player) + ".shot.json");
    cereal::JSONInputArchive readFile(shotFile);
    int x, y;
    readFile(x, y);
    if (player ==1){
        return evaluate_shot(2, x, y);
    } else {
        return evaluate_shot(1, x, y);
    }
    return NO_SHOT_FILE;
}