#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include "moveData.h"

using std::vector;

class ChessPiece {
    public:
        int row;
        int column;
        char color;
        bool moved = false;

        void setRow(int input) {row = input;}
        void setColumn(int input) {column = input;}
        int getRow() {return row;}
        int getColumn() {return column;}
        int getColor() {return color;}
        virtual char getName() {return 'X';}
        //virtual vector<std::shared_ptr<MoveData>> getLegalMoves(vector<vector<ChessPiece*>> board);
        virtual vector<std::shared_ptr<MoveData>> getLegalMoves(vector<vector<std::shared_ptr<ChessPiece>>> board);


        ChessPiece(); 
        ~ChessPiece();
    private:
};

class Pawn: public ChessPiece {
    public:

        //vector<std::shared_ptr<MoveData>> getLegalMoves(vector<vector<ChessPiece*>> board);
        vector<std::shared_ptr<MoveData>> getLegalMoves(vector<vector<std::shared_ptr<ChessPiece>>> board);
        char getName() {return 'P';}
        Pawn();
        ~Pawn();
    private:
};

class Knight: public ChessPiece {
    public:
        vector<std::shared_ptr<MoveData>> getLegalMoves(vector<vector<std::shared_ptr<ChessPiece>>> board);
        char getName() {return 'N';}
        Knight();
        ~Knight();
    private:
};

class Bishop: public ChessPiece {
    public:
        char getName() {return 'B';}
        Bishop();
        ~Bishop();
    private:
};

class Rook: public ChessPiece {
    public:
        char getName() {return 'R';}
        Rook();
        ~Rook();
    private:
};

class Queen: public ChessPiece {
    public:
        char getName() {return 'Q';}
        Queen();
        ~Queen();
    private:
};

class King: public ChessPiece {
    public:
        char getName() {return 'K';}
        King();
        ~King();
    private:
};
#endif
