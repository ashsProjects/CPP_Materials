#include "game_of_life.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <algorithm>

using namespace std;

/**  game_of_life
 * This constructor first checks file and cell errors. If no errors, it reads the
 * game board from the file and checks to see if it is valid. After doing that, all
 * of the variables for the class are set using the passed in arguments and the game
 * board read from file. As a final check, if pregens is grater than 0, it will run the
 * game that many times.
*/
game_of_life::game_of_life(std::string filename, char l_cell, char d_cell, int pregens)
{
    if (d_cell == l_cell)
        throw(runtime_error(this->cell_dupe_error));

    //check if file was found; if not display error
    ifstream file_in(filename);
    if (!file_in)
        throw(runtime_error(this->file_not_found_error + filename + "\""));

    //reading from file and populating curr_gen
    string line;
    int width, height;

    //check if width and height are formatted correctly; if not, display error
    if (file_in >> width >> height)
        getline(file_in, line);
    else
        throw(runtime_error(this->invalid_read_error + filename + "\""));
        
    string curr_gen(width*height, d_cell);
    for (int row = 0; file_in && row < height; ++row) {
        getline(file_in, line);
        for (int col = 0; col < width; ++col) {
            curr_gen[col+(row*width)] = line[col];
        }
    }

    //Setting all variables
    this->curr_gen = curr_gen;
    SetLiveCell(l_cell);
    SetDeadCell(d_cell);
    this->broad_width = width;
    this->broad_height = height;
    this->gen_calculated = 0;
    
    //Generations to automatically generate at start of program
    if (pregens != 0) NextNGen(pregens);
}

/** GetGeneration
 * This method simply returns the gen_calculated variable from the class
*/
int game_of_life::GetGenerations()
{
    return this->gen_calculated;
}

/** NextGen
 * This is the main method that will calculate the next generation of the game from the current gen.
 * It is the same as HW3. However, in this version, the state of the current game is saved in the deque
 * within a struct, which is used for rollback. The most recent game is saved to the front of the deque
 * and when the gens_saved reaches 100, it will delete games from the end of the deque to make room for new games. 
*/
void game_of_life::NextGen()
{
    if (this->gens_saved == 100) {
        this->previous_gens.pop_back();
        gens_saved--;
    }
    this->previous_gens.push_front({curr_gen, broad_width, broad_height, gen_calculated, live_cell, dead_cell});
    gens_saved++;

    string to(broad_width*broad_height, dead_cell);
    //For each cell, Calculate neighbor cell
    for (int i = 0; i < broad_width*broad_height; ++i)
    {
        //Check Cell adjacency
        short neighbor_count = 0;
        int row = i/broad_width, col = i%broad_width;
        //calculate neighbors
        int tr, br, lc, rc;
        if (row-1 < 0)
            tr = broad_height - 1;
        else
            tr = row - 1;
        if (row+1 == broad_height)
            br = 0;
        else
            br = row+1;
        if (col-1 < 0)
            lc = broad_width - 1;
        else
            lc = col - 1;
        if (col+1 == broad_width)
            rc = 0;
        else
            rc = col + 1;
        //TL T TR L R BL B BR
        int neighbors[8] = {tr*broad_width + lc, tr*broad_width + 
                            col, tr*broad_width + rc, row*broad_width+lc, row*broad_width + 
                            rc, br*broad_width + lc, br*broad_width+col, br*broad_width+rc};
        for (size_t j = 0; j < 8; j++)
        {
            if (curr_gen[neighbors[j]] == this->live_cell)
                ++neighbor_count;
        }
        if (curr_gen[i] == this->live_cell){
            if(neighbor_count == 2 || neighbor_count == 3)
            
                to[i] = this->live_cell;
            else
                to[i] = this->dead_cell;
        }
        else
            if(neighbor_count == 3)
                to[i] = this->live_cell;
            else
                to[i] = this->dead_cell;
    }

    this->curr_gen = to;
    this->gen_calculated++;
}

/** NextNGen
 * This function will iterate n number of times and call the NextGen() method
 * to calculate the next generation. Since NextGen() already updates the curr_gen
 * variable and increments the generation, there is nothing else to do.
*/
void game_of_life::NextNGen(int n)
{
    for (int i = 0; i < n; i++) {
        game_of_life::NextGen();
    }
}

/** SetLiveCell
 * This method will check to see if the live_cell parameter is the same as the 
 * dead_cell character. If they are the same, it throws an error and exits.
 * Else, it will set this->live_cell to live_cell.
*/
void game_of_life::SetLiveCell(char cell)
{
    char curr_cell = this->live_cell;

    if (cell == this->dead_cell)
        throw(runtime_error(this->cell_dupe_error));
    else {
        this->live_cell = cell;
        for (size_t i = 0; i < curr_gen.length(); ++i) {
            if (curr_gen[i] == curr_cell)
                curr_gen[i] = cell;
        }
    }
}

/** SetDeadCell
 * This method will check to see if the dead_cell parameter is the same as the 
 * live_cell character. If they are the same, it throws an error and exits.
 * Else, it will set this->dead_cell to dead_cell.
*/
void game_of_life::SetDeadCell(char cell)
{
    char curr_cell = this->dead_cell;

    if (cell == this->live_cell)
        throw(runtime_error(this->cell_dupe_error));
    else {
        this->dead_cell = cell;
        for (size_t i = 0; i < curr_gen.length(); ++i) {
            if (curr_gen[i] == curr_cell)
                curr_gen[i] = cell;
        }
    }
}

/** operator+
 * This is an overloaded + operator. It can be used with the game_of_life
 * object + number of generations. For example, game+5 to run the game for
 * 5 generations. The method will iterate through gens and call the NextGen()
 * method to calculate the next generations. This returns a new game with the
 * next gens calculated. If gens is negative, the - method is called instead.
*/
game_of_life game_of_life::operator+(int gens)
{
    game_of_life copy(*this);
    if (gens < 0) {
        copy -= gens;
    }
    else {
        copy.NextNGen(gens);
    }
    return copy;
}

/** operator++
 * This is an overloaded ++ (pre-increment) operator. It can be used with the 
 * game_of_life object in the manner: ++game. This will run the game one time. 
 * In other words, the method will call NextGen() once and return the object itself.
*/
game_of_life& game_of_life::operator++()
{
    game_of_life::NextGen();
    return *this;
}

/** operator++
 * This is an overloaded ++ (post-increment) operator. It can be used with the 
 * game_of_life object in the manner: game2=game++. This will run the game one time.
 * In other words, the method will call NextGen(). However, the returned pointer
 * will be to an instance before calculating the next generation.
*/
game_of_life game_of_life::operator++(int fake)
{   
    (void) fake;
    game_of_life copy(*this);
    game_of_life::NextGen();
    return copy;
}

/** operator+=
 * This is an overloaded += operator. It can be used with the game_of_life object
 * in this manner: game+=10. This will run the game 10 times and return the object itself.
 * When a negative value is passed in, it calls the -= method.
*/
game_of_life &game_of_life::operator+=(int gens)
{
    if (gens < 0) {
        this->operator-=(gens);
    }
    else {
        game_of_life::NextNGen(gens);
    }
    return *this;
}

/** operator-=
 * This is an overloaded -= operator. It can be used with the game_of_life object
 * in this manner: game-=10. This will rollback the game gens times using the Rollback()
 * method. The returned game is the current game after rollingback gens times. Throws a
 * range error or domain error if gens is out of bound.
*/
game_of_life &game_of_life::operator-=(int gens)
{
    return Rollback(gens);
}

/** operator-
 * This is an overloaded - operator. It can be used with the game_of_life object
 * in this manner: game2=game-10. This will rollback the game gens times using the Rollback()
 * method. The returned game is a new game after rollingback gens times. Throws a
 * range error or domain error if gens is out of bound.
*/
game_of_life game_of_life::operator-(int gens)
{
    game_of_life copy(*this);
    copy.Rollback(gens);
    return copy;
}

/** operator--
 * This is an overloaded -- (pre-increment) operator. It can be used with the game_of_life object
 * in this manner: --game. This will rollback the game 1 time using the Rollback()
 * method. The returned game is the current game after rollingback 1 time. Throws a
 * range error or domain error if gens is out of bound.
*/
game_of_life &game_of_life::operator--()
{
    return Rollback(1);
}

/** operator-=
 * This is an overloaded -- (post-increment) operator. It can be used with the 
 * game_of_life object in the manner: game2=game--. This will rollback the game one time.
 * using Rollback(). However, the returned pointer will be to an instance before 
 * the rollback was performed, the original game.
*/
game_of_life game_of_life::operator--(int fake)
{
    (void) fake;
    game_of_life copy(*this);
    game_of_life::Rollback(1);
    return copy;
}

/** operator<
 * This is an overloaded < operator. It can be used with the game_of_life object
 * in this manner: game<game2. This will check if the percentage of live cells in game
 * is less than game2 using the PercentageLiveCells() method.
*/
bool game_of_life::operator<(game_of_life &other)
{
    return ((PercentLiveCells()) < other.PercentLiveCells());
}

/** operator<=
 * This is an overloaded < operator. It can be used with the game_of_life object
 * in this manner: game<=game2. This will check if the percentage of live cells in game
 * is less than or equal to game2 using the PercentageLiveCells() method.
*/
bool game_of_life::operator<=(game_of_life &other)
{
    return ((PercentLiveCells() - 0.5) <= other.PercentLiveCells());
}

/** operator>
 * This is an overloaded > operator. It can be used with the game_of_life object
 * in this manner: game>game2. This will check if the percentage of live cells in game
 * is greater than game2 using the PercentageLiveCells() method.
*/
bool game_of_life::operator>(game_of_life &other)
{
    return ((PercentLiveCells()) > other.PercentLiveCells());
}

/** operator>=
 * This is an overloaded >= operator. It can be used with the game_of_life object
 * in this manner: game>=game2. This will check if the percentage of live cells in game
 * is greater than or equal to game2 using the PercentageLiveCells() method.
*/
bool game_of_life::operator>=(game_of_life &other)
{
    return ((PercentLiveCells() + 0.5) >= other.PercentLiveCells());
}

/** operator==
 * This is an overloaded == operator. It can be used with the game_of_life object
 * in this manner: game==game2. This will check if the percentage of live cells in game
 * is equal to game2 using the PercentageLiveCells() method. 
*/
bool game_of_life::operator==(game_of_life &other)
{
    return ((other.PercentLiveCells() > PercentLiveCells() - 0.5) && 
            (other.PercentLiveCells() < PercentLiveCells() + 0.5));
}

/** GetWindow
 * This method first checks for domain errors if height and width are greater than the game's board
 * and range errors when row and columns are larger than height and width. It then finds the starting
 * index given by (row, col) in curr_gen and iterates height and width times, adding to curr_window
 * for each node in curr_gen. The returned string is a window of the current game specified by the
 * starting index and dimensions in the parameter.
*/
std::string game_of_life::GenWindow(int row, int col, int height, int width)
{   
    string curr_window(width*height, this->dead_cell);
    if (height > this->broad_height || width > broad_width) {
        string f_string = "game_of_life - GenWindow(int,int,int,int) - Dimensions out of bound for height: " +
        to_string(height) + ", width: " + to_string(width) + ", Max values are height: " + to_string(broad_height) +
        ", width: " + to_string(broad_width);
        throw domain_error(f_string);
    }
    else if (row > this->broad_height-1 || col > this->broad_width-1) {
        string f_string = "game_of_life - GenWindow(int,int,int,int) - Index Out of Bounds for row: " +
        to_string(row) + ", column: " + to_string(col) + 
        ", Max values are row: " + to_string(broad_height) + ", col: " + to_string(broad_width);
        throw range_error(f_string);
    }
    else {
        int rc, br;
        for (int i = 0; i < height; i++) {
            br = (row+i) % broad_height;
            for (int j = 0; j < width; j++) {
                rc = (col + j) % broad_width;
                curr_window[(i*width)+j] = this->curr_gen[rc+(br*broad_width)];
            }
        }
    }
    return curr_window;
}

/** GenSubGame
 * This method will call GenWindow using the four parameters and create a new game using
 * the returned string of the window, height, and width. The returned game is the window
 * of the current game specified by the coordinates in the parameters as a new game.
*/
game_of_life game_of_life::GenSubGame(int row, int col, int height, int width)
{
    string new_window = GenWindow(row, col, height, width);
    game_of_life new_game = game_of_life(new_window, height, width);
    return new_game;
}

/** ToggleCell
 * This method first checks for range errors if index is greater than the size of the current
 * game board. Then, it will change the cell at the index to the opposite cell.
*/
void game_of_life::ToggleCell(int index)
{
    if (static_cast<std::string::size_type>(index) >= this->curr_gen.size()) {
        string f_string = "game_of_life - ToggleCell(int) - Index Out of Bounds for index: " +
        to_string(index) + ", Max value is: " + to_string(curr_gen.size()-1);
        throw range_error(f_string);
    }
    else {
        if (this->curr_gen[index] == this->live_cell) {
            this->curr_gen[index] = dead_cell;
        }
        else {
            this->curr_gen[index] = live_cell;
        }
    }
}

/** ToggleCell
 * This method first checks for range errors if row or column are greater than the height
 * and width of the current board. Then, it will calculate the index and change the value
 * at the index to the opposite cell.
*/
void game_of_life::ToggleCell(int row, int col)
{
    if (row >= this->broad_width || col >=this->broad_height) {
        string f_string = "game_of_life - ToggleCell(int) - Index Out of Bounds for row: " +
        to_string(row) + ", column: " + to_string(col) + 
        ", Max values are row: " + to_string(broad_height) + ", col: " + to_string(broad_width);
        throw range_error(f_string);
    }
    else {
        int index = ((row)*broad_width+(col));
        if (this->curr_gen[index] == this->live_cell) {
            this->curr_gen[index] = dead_cell;
        }
        else {
            this->curr_gen[index] = live_cell;
        }
    }
}

/** IsStillLife
 * This method checks to see if the current board is the same as the next generation. If true,
 * this method returns false as the game is not still life.
*/
bool game_of_life::IsStillLife()
{
    game_of_life copy(*this);
    copy++;
    return (copy.curr_gen == this->curr_gen);
}

/** GetAvaliableGens
 * This method returns the gens_saved variable of the object, which is the number of
 * generations avaliable to rollback
*/
int game_of_life::GetAvailableGens()
{
    return this->gens_saved;
}

/** Rollback
 * This is a private helper method used to rollback generations. It throws a domain error if
 * the number of generations saved is 0 and a range erorr if the gens passed in is greater than
 * the number of generations saved. It uses a for loop to iterate gens times and pop the last
 * generation from the deque of structs. After iterating, it will set the current game to have 
 * the same state as the game on the front of the deque using the states saved in the struct.
 * After loading from the front, it will also delete the struct at the front of the queue.
*/
game_of_life &game_of_life::Rollback(int gens)
{
    if (gens == 0) return *this;
    else if (this->gens_saved == 0) {
        throw domain_error("game_of_life - operator-=(int) - No generations avaliable to Rollback to.");
    }
    else if (gens < 0 || this->gens_saved < gens) {
        string f_string = "game_of_life - operator-=(int) - Number of generations passed: [" +
        to_string(gens) + "] is greater than the number of avaliable generations: [" +
        to_string(gens_saved) + "] to Rollback to.";
        throw range_error(f_string);
    }
    else {
        for (int i = 0; i < gens-1; i++) {
            this->previous_gens.pop_front();
            gens_saved--;
        }
        this->curr_gen = this->previous_gens.front().game_board;
        this->broad_width = this->previous_gens.front().width;
        this->broad_height = this->previous_gens.front().height;
        this->gen_calculated = this->previous_gens.front().generation;
        this->live_cell = this->previous_gens.front().live;
        this->dead_cell = this->previous_gens.front().dead;

        this->previous_gens.pop_front();
        gens_saved--;
        return *this;
    }
}

/** PercentLiveCells
 * This method counts the number of live cells in the game using the count method from the
 * algorithm header. This is then divided by the total number of cells to 
 * get the percentage. The returned value is the percentage of live cells.
*/
double game_of_life::PercentLiveCells()
{
    int occurences = count(this->curr_gen.begin(), this->curr_gen.end(), this->live_cell);
    return static_cast<double>(occurences) / this->curr_gen.size() * 100.00;
}

/** game_of_life
 * This is a private constructor used to create a new game with a window rather than reading from a file.
 * It simply just sets the new objects game board and dimensions.
*/
game_of_life::game_of_life(string window, int height, int width) {
        this->curr_gen = window;
        this->broad_width = width;
        this->broad_height = height;
        this->gen_calculated = 0;
}

/** operator<<
 * This is an overloaded << operator. It replaces the to_string method and
 * creates a string to be printed using loops and the game_board of the
 * current game board. This is then outputted using cout.
*/
std::ostream &operator<<(std::ostream &os, const game_of_life &game)
{   
    string out = "Generation: ";
    out += to_string(game.gen_calculated);
    out += '\n';
    for (int row = 0; row < game.broad_height; ++row) {
        for (int col = 0; col < game.broad_width; ++col) {
            out += game.curr_gen[col+(row*game.broad_width)];
        }
        out += '\n';
    }
    return os << out;
}
