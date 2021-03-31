#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;
using std::abs;
using std::sort;

#define COORDINATE_SIZE 2
constexpr char NUMBER_OF_NEIGHBORS = 4;

enum class State 
{
  kEmpty, 
  kObstacle,
  kClosed,
  kPath,
  kStart,
  kFinish
};


const int deltas[4][2] = {
  {-1, 0}, 
  {0, -1}, 
  {1, 0}, 
  {0, 1}
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


bool Compare(vector<int> firstNode, vector<int> secondNode)
{
  if((firstNode[2] + firstNode[3]) > (secondNode[2] + secondNode[3]))
    return true; 
  else
    return false;
}


/**
 * Sort the two-dimensional vector of ints in descending order.
 */
void CellSort(vector<vector<int>> *v) 
{
  sort(v->begin(), v->end(), Compare);
}


int Heuristic(int x1, int y1, int x2, int y2)
{
  return (abs(x2 - x1) + abs(y2 - y1));
}


bool CheckValidCell(int x, int y, vector< vector<State> > &boardGrid)
{
  /* If x AND y are valid coordinates AND that coordinate is empty. Return TRUE */
  if ((x >= 0 && x < boardGrid.size()) && (y >= 0 && y < boardGrid[0].size()))
    return boardGrid[x][y] == State::kEmpty;
  return false;
}


void AddToOpen(int x, int y, int g, int h, vector< vector<int> > &openNodes, vector< vector<State> > &grid)
{
  vector<int> l_node{ x, y, g, h};

  openNodes.push_back(l_node);
  grid[x][y] = State::kClosed;
}


/** 
 * Expand current nodes's neighbors and add them to the open list.
 */
void ExpandNeighbors(vector<int> &currentNode,
                     int goal[2],
                     vector< vector<int> > &openNodes, 
                     vector< vector<State> > &boardGrid)
{
  int nextX;
  int nextY;
  int g;
  int h;

  for(char iterator = 0; iterator < NUMBER_OF_NEIGHBORS; iterator++)
  {
    nextX = currentNode[0] + deltas[iterator][0];
    nextY = currentNode[1] + deltas[iterator][1];

    if(CheckValidCell(nextX, nextY, boardGrid))
    {
      g = currentNode[2] + 1;
      h = Heuristic(nextX, nextY, goal[0], goal[1]);
      AddToOpen(nextX, nextY, g, h, openNodes, boardGrid);
    }
  }
}


vector< vector<State> > Search(vector< vector<State> > boardGrid, int start[COORDINATE_SIZE], int goal[COORDINATE_SIZE])
{
  vector< vector<State>> path{};
  vector<vector<int>> openNodes {};

  AddToOpen(start[0], start[1], 0, Heuristic(start[0], start[1], goal[0], goal[1]), openNodes, boardGrid);

  while(!openNodes.empty())
  {
    CellSort(&openNodes);

    vector<int> currentNode = openNodes.back();
    openNodes.pop_back();

    boardGrid[currentNode[0]][currentNode[1]] = State::kPath;

    if((currentNode[0] == goal[0]) && (currentNode[1] == goal[1]))
    {
      boardGrid[start[0]][start[1]] = State::kStart;
      boardGrid[goal[0]][goal[1]]   = State::kFinish;
      return boardGrid;
    }

    ExpandNeighbors(currentNode, goal, openNodes, boardGrid);
  }

  cout << "No path found." << std::endl;
  
  return path;
}


string CellString(State cell) 
{
  switch(cell) 
  {
    case State::kObstacle: return "⛰️   ";
    case State::kPath:     return "🚗   ";
    case State::kStart:    return "🚦   ";
    case State::kFinish:   return "🏁   ";
    default:               return "0    "; 
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
