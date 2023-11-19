#ifndef GAME_OF_LIFE_H_DEFINED
#define GAME_OF_LIFE_H_DEFINED
#include <string>
#include <iostream>
#include <deque>

class game_of_life {
    std::string filename;
    char live_cell = '*';
    char dead_cell = '-';
    std::string curr_gen;
    int gen_calculated, broad_height, broad_width;
    struct game_save_state {
        std::string game_board;
        int width;
        int height;
        int generation;
        char live;
        char dead;
    };
    std::deque<game_save_state> previous_gens;
    int gens_saved = 0;

	constexpr static const char * cell_dupe_error = "game_of_life - Live Cell and Dead Cell cannot be the same character\n";
	constexpr static const char * file_not_found_error = "game_of_life - File Not Found \"";
	constexpr static const char * invalid_read_error = "game_of_life - Width and Height incorrectly formatted for file \"";
	constexpr static const char * error_end = "\"\n";

    public:
        /** 
         * @param None
         * @returns A copy constructor declaration. The new object will be the same as rhs.
        */
        game_of_life(const game_of_life &rhs) = default;

        /** 
         * @param None
         * @returns A copy assignment constructor declaration. The new object will be the same as rhs.
        */
        game_of_life &operator=(const game_of_life &rhs) = default;

        /** 
         * @param None
         * @returns A move assignment constructor declaration. It copies the resources of rhs to a new object.
        */
        game_of_life &operator=(game_of_life &&rhs) = default;

        /** 
         * @param None
         * @returns A move constructor declaration. It copies the resources of rhs to a new object.
        */
        game_of_life(game_of_life &&rhs) = default;

        /** 
         * @param None
         * @returns A virtual destructor declaration.
        */
        virtual ~game_of_life() = default;

        /** 
         * @param None
         * @returns A default deleted constructor declaration. It prevents a new object with no parameters from being created.
        */
        game_of_life() = delete;

        /** 
         * @param filename the name of the file from which to read
         * @returns Everything will be default and the only the file is read for the cells
        */
        game_of_life(std::string filename) : game_of_life(filename, '*', '-', 0) {}

        /** 
         * @param filename the name of the file from which to read
         * @param pregens the number of times to run the game at the start
         * @returns Everything will be default and only the file is read for the cells. It will also run pregens times
        */
        game_of_life(std::string filename, int pregens) : game_of_life(filename, '*', '-',  pregens) {}

        /** 
         * @param filename the name of the file from which to read
         * @param l_cell the character that will represent the live cells
         * @param d_cell the character that will represent the dead cells
         * @returns The game will be read with the l_cell and d_cell chars and it is not ran when initializing
        */
        game_of_life(std::string filename, char l_cell, char d_cell) : game_of_life(filename, l_cell, d_cell, 0) {}

        /** 
         * @param filename the name of the file from which to read
         * @param l_cell the character that will represent the live cells
         * @param d_cell the character that will represent the dead cells
         * @param pregens the number of times to run the game at the start
         * @returns The game will be read with the l_cell and d_cell chars and it is ran pregens times when initializing
        */
        game_of_life(std::string filename, char l_cell, char d_cell, int pregens);

        /** GetGenerations
         * @param None
         * @returns an integer value of the current generation number
        */ 
        int GetGenerations();

        /** NextGen
         * @param None
         * @returns None
        */
        void NextGen();

        /** NextNGen
         * @param n an integer representing how many generations to run
         * @returns None
        */
        void NextNGen(int n);

        /** SetLiveCell
         * @param live_cell the char to be used for the live_cells
         * @returns None
        */
        void SetLiveCell(char live_cell);

        /** SetDeadCell
         * @param dead_cell the char to be used for the dead_cells
         * @returns None
        */
        void SetDeadCell(char dead_cell);

        /** operator+
         * @param gens the number of generation to run the game
         * @returns a pointer to a new game with the next gens
        */
        game_of_life operator+(int gens);

        /** operator++
         * @param None
         * @returns a pointer to itself using *this
        */
        game_of_life& operator++();

        /** operator++
         * @param fake a dummy variable not used in the method
         * @returns a pointer to itself before incrementing the game using *this
        */
        game_of_life operator++(int fake);

        /** operator+=
         * @param gens the number of generation to run the game
         * @returns a pointer to itself using *this
        */
        game_of_life& operator+=(int gens);

        /** operator-=
         * @param gens the number of generation torollback the game
         * @returns a pointer to itself using *this after rolling back gens games
        */
        game_of_life& operator-=(int gens);

        /** operator-
         * @param gens  the number of generation to Rollback the game
         * @returns a pointer to a new game with gens rollbacked
        */
        game_of_life operator-(int gens);

        /** operator--
         * @param None
         * @returns a pointer to itself using *this after rolling back gens game
        */
        game_of_life& operator--();

        /** operator--
         * @param fake  a dummy variable not used in the method
         * @returns a pointer to itself before rollingback the game using *this
        */
        game_of_life operator--(int fake);

        /** operator<
         * @param other a reference to the 2nd game being compared to
         * @returns true if percentage of live cells is less than other's
        */
        bool operator<(game_of_life &other);

        /** operator<=
         * @param other  a reference to the 2nd game being compared to
         * @returns true if percentage of live cells is less than or within 0.10 of other's
        */
        bool operator<=(game_of_life &other);

        /** operator>
         * @param other a reference to the 2nd game being compared to
         * @returns true if percentage of live cells is greater than other's
        */
        bool operator>(game_of_life &other);

        /** operator>=
         * @param other a reference to the 2nd game being compared to
         * @returns true if percentage of live cells is greater than or within 0.10 of other's
        */
        bool operator>=(game_of_life &other);

        /** operator==
         * @param other a reference to the 2nd game being compared to
         * @returns true if percentage of live cells is within 0.10 of other's
        */
        bool operator==(game_of_life &other);

        /** GetWindow
         * @param row the row at which to start
         * @param col the column at which to start
         * @param height the number of rows to include in new window
         * @param width the number of columns to include in new window
         * @returns a string of a new game with the above starting points and dimensions
        */
        std::string GenWindow(int row,int col,int height,int width);

        /** GenSubGame
         * @param row the row at which to start
         * @param col the column at which to start
         * @param height the number of rows to include in new window
         * @param width the number of columns to include in new window
         * @returns a new game initialized with the string gotten from GenWindow()
        */
        game_of_life GenSubGame(int row,int col,int height,int width);

        /** ToggleCell
         * @param index index value for current generation's game board
         * @returns None
        */
        void ToggleCell(int index);

        /** ToggleCell
         * @param row row for current generation's game board
         * @param col column for current generation's game board
         * @returns None
        */
        void ToggleCell(int row, int col);

        /** isStillLife
         * @param None
         * @returns true if next generation has different output than current generation
        */
        bool IsStillLife();

        /** GetAvaliableGens
         * @param None
         * @returns the number of generations that can be rolledback
        */
        int GetAvailableGens();

        /** operator<<
         * A friend method used by operator<< to output information regarding this object
        */
        friend std::ostream &operator<<(std::ostream &os, const game_of_life &game);

    private:
        /** game_of_life
         * @param window the game window for the new game
         * @param height the height of the game window
         * @param width the width of the game window
         * @returns the new object will have the window as curr_gen, height and width as the args, and the rest will be the same
        */
        game_of_life(std::string window, int height, int width);

        /** Rollback
         * @param gens  the number of gens to rollback the game
         * @returns a reference to the current game with gens rolledback
        */
        game_of_life& Rollback(int gens);

        /** PercentLiveCells
         * @param None
         * @returns the percentage of live cells in the current game
        */
       double PercentLiveCells();
};

/** operator<<
 * @param os reference to the output stream used for printing
 * @param game  a reference to a game_of_life object
 * @returns An ostream for printing the string to screen
*/
std::ostream & operator<<(std::ostream &os, const game_of_life &game);

#endif/*GAME_OF_LIFE_H_INCLUDED*/