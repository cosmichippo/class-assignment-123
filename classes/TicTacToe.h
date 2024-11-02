#pragma once
#include "Game.h"
#include "Square.h"

//
// the classic game of tic tac toe
//

//
// this is the AI class
// we use a small clone here so we can recursively call minimax
//
struct TicTacToeAI
{
    int   _grid[3][3];
    bool isBoardFull() const;
    int evaluateBoard();
    int evaluateBoardMinimax();
    void populateGrid(int playerColor, int row, int col); // added, used to populate the board by one character
    std::vector<TicTacToeAI*> generateChildren(TicTacToeAI* state, int playerColor);
    TicTacToeAI* copy();// copies grid
    void passStateString(std::string state);
    void invertGrid(bool isMaxPlayer); 
    // takes the negative of the grid -> used to ensure AI is always 'positive'
    std::pair<int, int> minimax(TicTacToeAI* state, int depth, bool isMaximizingPlayer);
    int negamax(TicTacToeAI* state, int depth, int playerColor);
    int ownerAt(int index ) const;
    int AICheckForWinner();
    void _union(TicTacToeAI* other);
};

//
// the main game class
//
class TicTacToe : public Game
{
public:
    TicTacToe();
    ~TicTacToe();

    // set up the board
    void        setUpBoard() override;
    void        drawFrame() override;

    Player*     checkForWinner() override;
    bool        checkForDraw() override;
    std::string initialStateString() override;
    std::string stateString() const override;
    void        setStateString(const std::string &s) override;
    bool        actionForEmptyHolder(BitHolder *holder) override;
    bool        canBitMoveFrom(Bit*bit, BitHolder *src) override;
    bool        canBitMoveFromTo(Bit* bit, BitHolder*src, BitHolder*dst) override;
    void        stopGame() override;

    TicTacToeAI* clone();
	void        updateAI() override;
    bool        gameHasAI() override { return true; }
private:
    Bit *       PieceForPlayer(const int playerNumber);
    Player*     ownerAt(int index ) const;

    void        scanForMouse();
    Square      _grid[3][3];
};

