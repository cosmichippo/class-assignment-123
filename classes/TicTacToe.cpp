#include "TicTacToe.h"
const int AI_PLAYER = 1;
const int HUMAN_PLAYER = -1;

TicTacToe::TicTacToe()
{
}

TicTacToe::~TicTacToe()
{
}

//
// make an X or an O
//
Bit* TicTacToe::PieceForPlayer(const int playerNumber)
{
    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit *bit = new Bit();
    // should possibly be cached from player class?
    bit->LoadTextureFromFile(playerNumber == 1 ? "x.png" : "o.png");
    bit->setOwner(getPlayerAt(playerNumber));
    return bit;
}

void TicTacToe::setUpBoard()
{
    // sets number of players
    setNumberOfPlayers(2);    
    // initialize the bitholders for the game
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            Square mySquare = Square();
            char test[] = "square.png";
            // init holder takes a local window position vector,
            // as well as a image to attach to a sprite class
            mySquare.initHolder(ImVec2(100 + (100 * j), 100 + (100 * i)), test, i, j);
            _grid[i][j] = mySquare;
        }
    }
    // this function initializes turns and sets the state variable
    startGame();
}

//
// scan for mouse is temporarily in the actual game class
// this will be moved to a higher up class when the squares have a heirarchy
// we want the event loop to be elsewhere and calling this class, not called by this class
// but this is fine for tic-tac-toe
//
void TicTacToe::scanForMouse()
{
    if (gameHasAI() && getCurrentPlayer() && getCurrentPlayer()->isAIPlayer()) 
    {
        updateAI();
        return;
    }

    ImVec2 mousePos = ImGui::GetMousePos();
    mousePos.x -= ImGui::GetWindowPos().x;
    mousePos.y -= ImGui::GetWindowPos().y;

    for (int y=0; y<3; y++) {
        for (int x=0; x<3; x++) {
            if (_grid[y][x].isMouseOver(mousePos)) {
                if (ImGui::IsMouseClicked(0)) {
                    if (actionForEmptyHolder(&_grid[y][x])) {
                        endTurn();
                    }
                } else {
                    _grid[y][x].setHighlighted(true);
                }
            } else {
                _grid[y][x].setHighlighted(false);
            }
        }
    }    
}

//
// draw the board and then the pieces
// this will also go somewhere else when the heirarchy is set up
//
void TicTacToe::drawFrame() 
{
    scanForMouse();

    for (int y=0; y<3; y++) {
        for (int x=0; x<3; x++) {
            _grid[y][x].paintSprite();
            if (_grid[y][x].bit()) {
                _grid[y][x].bit()->paintSprite();
            }
        }
    }
}

//
// about the only thing we need to actually fill out for tic-tac-toe
//
bool TicTacToe::actionForEmptyHolder(BitHolder *holder)
{
    // ran when mouse is clicked
    Bit* bit = holder->bit();

    // if no bit assigned and a winner hasn't been found
    if (!bit && this->checkForWinner() == nullptr) {
        // set player number, set position, attach the bit to the bitholder
        unsigned int playerNum = this->getCurrentPlayer()->playerNumber();
        bit = this->PieceForPlayer(playerNum);
        bit->setPosition(holder->getPosition());
        holder->setBit(bit); 
        return true;
    }
    return false;
}

bool TicTacToe::canBitMoveFrom(Bit *bit, BitHolder *src)
{
    // you can't move anything in tic tac toe
    return false;
}

bool TicTacToe::canBitMoveFromTo(Bit* bit, BitHolder*src, BitHolder*dst)
{
    // you can't move anything in tic tac toe
    return false;
}

//
// free all the memory used by the game on the heap
//
void TicTacToe::stopGame()
{

    for (int y=0; y<3; y++) {
        for (int x=0; x<3; x++) {
           _grid[y][x].destroyBit();
        }
    }
}

//
// helper function for the winner check
//
Player* TicTacToe::ownerAt(int index ) const
{
    if (!_grid[index/3][index%3].bit()) {
        return nullptr;
    }
    return _grid[index/3][index%3].bit()->getOwner();
}



Player* TicTacToe::checkForWinner()
{
    // check to see if either player has won
    for (int i = 0; i < 3; i++){
        int fRI = (3 * i) + 0;
        int sRI = (3 * i) + 1;
        int tRI = (3 * i) + 2;
        // parse through for rows
        if (ownerAt(fRI) == ownerAt(sRI) && ownerAt(sRI) == ownerAt(tRI) && ownerAt(fRI) != nullptr){
            return ownerAt(fRI);
        }
        int fCI = 0 + i;
        int sCI = 3 + i;
        int tCI = 6 + i;
        //parse through for columns
        if (ownerAt(fCI) == ownerAt(sCI) && ownerAt(sCI) == ownerAt(tCI) && ownerAt(fCI) != nullptr){
            return ownerAt(fCI);
        }
    }
    // diag check
    int f = 0; int s = 4; int t = 8;
    if (ownerAt(f) == ownerAt(s) && ownerAt(s) == ownerAt(t) && ownerAt(f) != nullptr){
            return ownerAt(f);
    }
    // inverse diag check
    if (ownerAt(2) == ownerAt(4) && ownerAt(4) == ownerAt(6) && ownerAt(2) != nullptr){
            return ownerAt(2);
    }

    return nullptr;
}

bool TicTacToe::checkForDraw()
{
    int full = 0;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (_grid[i][j].bit()){ 
                full += 1;
            }
        }
    }
    if (full == 9) {
        return true;
    }
    // check to see if the board is full
    return false;
}

//
// state strings
//
std::string TicTacToe::initialStateString()
{
    return "000000000";
}

//
// this still needs to be tied into imguis init and shutdown
// we will read the state string and store it in each turn object
//
std::string TicTacToe::stateString() const
{
    std::string s;
    for (int y=0; y<3; y++) {
        for (int x=0; x<3; x++) {
            Bit *bit = _grid[y][x].bit();
            if (bit) {
                s += std::to_string(bit->getOwner()->playerNumber()+1);
            } else {
                s += "0";
            }
        }
    }
    return s;
}

//
// this still needs to be tied into imguis init and shutdown
// when the program starts it will load the current game from the imgui ini file and set the game state to the last saved state
//
void TicTacToe::setStateString(const std::string &s)
{
    for (int y=0; y<3; y++) {
        for (int x=0; x<3; x++) {
            int index = y*3 + x;
            int playerNumber = s[index] - '0';
            if (playerNumber) {
                _grid[y][x].setBit( PieceForPlayer(playerNumber-1) );
            } else {
                _grid[y][x].setBit( nullptr );
            }
        }
    }
}


//
// this is the function that will be called by the AI
//
void TicTacToe::updateAI() 
{
}

//
// AI class
// this is a small class that just has a bunch of ints in it to allow us to recursively call minimax
//

TicTacToeAI* TicTacToe::clone() 
{
    TicTacToeAI* newGame = new TicTacToeAI();
    return newGame;
}

//
// helper function for the winner check
//
int TicTacToeAI::ownerAt(int index ) const
{
    return 0;
}

int TicTacToeAI::AICheckForWinner()
{
    return -1;
}

//
// helper function for a draw
//
bool TicTacToeAI::isBoardFull() const
{
    return false;    
}

//
// Returns: positive value if AI wins, negative if human player wins, 0 for draw or undecided
//
int TicTacToeAI::evaluateBoard() 
{
    // Check for winner
    return 0;
}

//
// player is the current player's number (AI or human)
//
int TicTacToeAI::negamax(TicTacToeAI* state, int depth, int playerColor) 
{
    return 0;
}

//
// evaluate board for minimax needs to return 
// actual winner
//
int TicTacToeAI::evaluateBoardMinimax()
{
    return 0; // No winner yet or draw
}

//
// player is the current player's number (AI or human)
//
int TicTacToeAI::minimax(TicTacToeAI* state, int depth, bool isMaximizingPlayer) 
{
    return 0;
}
