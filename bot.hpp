#pragma once

#include "thc.h"
#include <climits>
#include <iostream>

const int PAWN_VALUE = 1;
const int KNIGHT_VALUE = 3;
const int BISHOP_VALUE = 3;
const int ROOK_VALUE = 5;
const int QUEEN_VALUE = 9;

thc::Move findBestMove(thc::ChessRules cr, int depth);
int evaluate(thc::ChessRules* cr);
int countMaterial(thc::ChessRules* cr, bool white);
int search(thc::ChessRules* cr, int depth, int alpha, int beta);
int oldSearch(thc::ChessRules* cr, int depth);
