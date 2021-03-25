#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;
using std::abs;

#define COORDINATE_SIZE 2

enum class State 
{
  kEmpty, 
  kObstacle,
  kClosed
};


vector<State> ParseLine(string line) 
{
    istringstream sline(line);
    int n;
    char c;
    vector<State> row;

    while (sline >> n >> c && c == ',') 
    {
      if (n == 0) 
        row.push_back(State::kEmpty);
      else
        row.push_back(State::kObstacle);
    }

    return row;
}


vector< vector<State> > ReadBoardFile(string path) 
{
  ifstream myfile (path);
  vector< vector<State> > board{};

  if (myfile) 
  {
    string line;

    while (getline(myfile, line)) 
    {
      vector<State> row = ParseLine(line);
      board.push_back(row);
    }
  }

  return board;
}


int Heuristic(int x1, int y1, int x2, int y2)
{
  return (abs(x2 - x1) + abs(y2 - y1));
}


void AddToOpen(int x, int y, int g, int h, vector< vector<int> > &openNodes, vector< vector<State> > &grid)
{
  vector<int> l_node{ x, y, g, h};

  openNodes.push_back(l_node);
  grid[x][y] = State::kClosed;
}

vector< vector<State> > Search(vector< vector<State> > boardGrid, int start[COORDINATE_SIZE], int goal[COORDINATE_SIZE])
{
  vector< vector<State>> path{};

  cout << "No path found." << std::endl;
  
  return path;
}


string CellString(State cell) 
{
  switch(cell) 
  {
    case State::kObstacle: return "⛰️   ";
    default: return "0   "; 
  }
}


void PrintBoard(const vector< vector<State> > board) 
{
  for (int i = 0; i < board.size(); i++) 
  {
    for (int j = 0; j < board[i].size(); j++) 
      cout << CellString(board[i][j]);
    
    cout << "\n";
  }
}


int main() 
{
  int init[COORDINATE_SIZE] = { 0,0 };
  int goal[COORDINATE_SIZE] = { 4,5 };

  auto board = ReadBoardFile("1.board");

  vector< vector<State> > solution = Search(board, init, goal);

  PrintBoard(solution);

  return 0;
}
