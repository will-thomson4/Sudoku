//Loads 9x9 sudoku board from .dat file
void load_board(const char* filename, char board[9][9]);

//Displays board loaded from .dat file to the console
void display_board(const char board[9][9]);

//Checks whether a 9x9 sudoku board is complete
bool is_complete(const char board[9][9]);

//If a move is valid (i.e in 9x9 range, not repeated in row/column/square) then insert digit onto the board
bool make_move(const char position[2], char digit, char board[9][9]);

//Save a 9x9 board to a .dat file
bool save_board(const char filename[], char board[9][9]);

//Given a partially completed board, attempt to solve the board, return true if possible, false if not
bool solve_board(char board[9][9]);