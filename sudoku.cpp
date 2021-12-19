#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cctype>
#include "sudoku.h"

using namespace std;

//For measuring stack frame pushes of this function
//int totalStackFrames = 0;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* pre-supplied function to load a Sudoku board from a file */
void load_board(const char* filename, char board[9][9]) {

  cout << "Loading Sudoku board from file '" << filename << "'... ";

  ifstream in(filename);
  if (!in) {
    cout << "Failed!\n";
  }
  assert(in);

  char buffer[512];

  int row = 0;
  in.getline(buffer,512);
  while (in && row < 9) {
    for (int n=0; n<9; n++) {
      assert(buffer[n] == '.' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer,512);
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << '\n';
  assert(row == 9);
}

/* internal helper function */
void print_frame(int row) {
  if (!(row % 3)) {
    cout << "  +===========+===========+===========+\n";
  } else {
    cout << "  +---+---+---+---+---+---+---+---+---+\n";
  }
}

/* internal helper function */
void print_row(const char* data, int row) {
  cout << (char) ('A' + row) << " ";
  for (int i=0; i<9; i++) {
    cout << ( (i % 3) ? ':' : '|' ) << " ";
    cout << ( (data[i]=='.') ? ' ' : data[i]) << " ";
  }
  cout << "|\n";
}

/* pre-supplied function to display a Sudoku board */
void display_board(const char board[9][9]) {
  cout << "    ";
  for (int r=0; r<9; r++) {
    cout << (char) ('1'+r) << "   ";
  }
  cout << '\n';
  for (int r=0; r<9; r++) {
    print_frame(r);
    print_row(board[r],r);
  }
  print_frame(9);
}

/* add your functions here */
//Checks if character is a valid digit
bool check_if_char_digit(char ch) {
  const char acceptable_digits[9] = {'1','2','3','4','5','6','7','8','9'};
  for(int i = 0; i < 9; i++) {
    if(ch == acceptable_digits[i]) 
      return true;    
  }
  return false;
}

//Returns true if all the 9x9 char board positions are filled with digit chars, otherwise returns false
bool is_complete(const char board[9][9]) {
  assert(board);

  int row = 0;
  while(row < 9) {
    for(int i = 0; i < 9; i++) {
      if(!check_if_char_digit(board[row][i])) {
        return false;
      }
    }
    row++;
  }
  return true;
}


/*          Start of move validity functions        */
//Internal function to check if a board position is valid on a 9x9 board
bool check_position_in_range(const char position[2]) {

  int charPosition = toupper(position[0]);
  if(charPosition < 65 || charPosition > 73) 
    return false;
  
  int numberPosition = position[1] - '0';
  if(numberPosition < 1 || numberPosition > 9) 
    return false;
  
  return true;
}

//Internal function to check if a digit already exists in a row of the board
bool is_digit_in_row(char digit, char row[9]) {

  for(int i = 0; i < 9; i++) {
    if(row[i] == digit) {
      return true;
    }
  }
  return false;
}

//Internal function to check if a digit already exists in a column of the board
bool is_digit_in_column(char digit, int columnPosition, char board[9][9]) {
  assert(board);

    for(int i = 0; i < 9; i++) {
      if(board[i][columnPosition] == digit) {
        return true;
      }
    }
  return false;
}

//Internal function to check if the digit already exists in the 3x3 subsquare it is being inserted into
bool is_digit_already_in_grid(char digit, int columnNumber, int rowNumber, char board[9][9]) {
  assert(board);
  
  int horizontal_subsquare_index = (columnNumber)/3;
  int vertical_subsquare_index = (rowNumber)/3;

  int startRow = vertical_subsquare_index * 3;
  int row = startRow;

  int startColumn = horizontal_subsquare_index * 3;

  while(row < (startRow +3)) {
    for(int i = 0; i < 3; i++) {
      if(digit == board[row][i + startColumn]) {
        return true;
      }
    }
    row++;
  }
  return false;
}

//Internal helper function which checks to make sure that numbers are only inserted onto whitespace, rather than overwritting an existing number
bool is_square_whitespace(int columnNumber, int rowNumber, char board[9][9]) {
  assert(board);

  if(!check_if_char_digit(board[rowNumber][columnNumber])) {
    return true;
  }
  return false;
}

//Internal function which combines the move validity helper functions to check if a move is logically valid
bool move_valid(char digit, int columnNumber, int rowNumber, char board[9][9]) {
  assert(board);

  if(!is_digit_in_row(digit, board[rowNumber]) && !is_digit_in_column(digit, columnNumber, board) && !is_digit_already_in_grid(digit, columnNumber, rowNumber, board) && is_square_whitespace(columnNumber, rowNumber, board))
    return true;

  return false;
}

/*          End of move validity functions        */

//Inserts a digit onto the board at the position entered, positions are 2 char arrays which relate to the row and column
//of the board, with rows represented by letters A-I and columns by digits 1-9 e.g C5
bool make_move(const char position[2], char digit, char board[9][9]) {
  //Sanity checks and input sanitisation
  assert(board);
  if(!check_position_in_range(position) || !check_if_char_digit(digit))
    return false;
  
  int rowNumber = toupper(position[0]) - 65;
  int columnNumber = (position[1] - '0')-1;
  
  //Move validity logic checks
  if(move_valid(digit, columnNumber, rowNumber, board)) {
    board[rowNumber][columnNumber] = digit;
    return true;
  }
  return false;
}

//Saves inputted board (2 dimensional character array) to a file
bool save_board(const char filename[], char board[9][9]) {
  assert(board);
  
  if(strlen(filename) == 0)
    return false; 
  
  ofstream out(filename);

  int row = 0;
  while(row < 9  && !out.fail()) {
    for(int i = 0; i < 9; i++) {
      if(board[row][i] == ' ') {
        out.put('.');
      } else {
        out.put(board[row][i]);
      }
    }
    out.put('\n');
    row++;
    if(out.fail()) 
      return false;
    
  }

  out.close();
  return true;
}


//Helper function to turn integer index postions into a character array representing the board position
void index_position_to_char_position(char charPosition[], int row, int column) {
  char rowChars[9] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'};
  char colChars[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
  charPosition[0] = rowChars[row];
  charPosition[1] = colChars[column];
  return;
}

void get_new_board_position(const int row, const int column, int& newRow, int& newColumn) {
    if(column == 8) {
     newRow = row+1;
     newColumn = 0;
    } else {
     newRow= row;
     newColumn = column +1;
    }
}

//Cycles through the sudoku board, inserting digits from 1-9, using recursion to track back and re-attempt if a valid digit can no longer be entered in
bool cycle_through_board(char board[9][9], int row, int column) {
  assert(board);

  //For measuring stack frame pushes of this function
  //totalStackFrames += 1;
  
  if(is_complete(board)) {
    return true;
  }

  //Set the position character array using the row and column integer parameters that we've passed in
  char position[2] = {'A', '1'};
  index_position_to_char_position(position, row, column);

  int newRow;
  int newColumn;

  //Check if the space is a whitespace, only run move function if it isn't
  if(is_square_whitespace(column, row, board)) {
    board[row][column] = '.';
    for(char number = '1'; number <= '9'; number++) {

      //If it is a valid move, make the move and call the function again with the next position
      if(make_move(position, number, board)) {

        get_new_board_position(row, column, newRow, newColumn);

        if(cycle_through_board(board, newRow, newColumn)) {
          return true;
        }
      }
      
      board[row][column] = '.';
    }
  } else {

    get_new_board_position(row, column, newRow, newColumn);

    if(cycle_through_board(board, newRow, newColumn)) {
      return true;
    }
  }

  return false;
}

//Solves board
bool solve_board(char board[9][9]) {
  assert(board);

    if(cycle_through_board(board, 0, 0)) {
      return true;
    }
    return false;   
}