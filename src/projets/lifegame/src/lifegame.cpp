// Lifegame sequential part
# include <vector>
# include <iostream>
# include <fstream>
# include <utility>
# include "gui/context.hpp"
# include "gui/window.hpp"
# include "gui/event_manager.hpp"
# include "gui/quad.hpp"
# include "gui/segment.hpp"

/**
 * @brief      Generate a grid for conway life game
 * 
 *   In 1970, the British mathematician John Conway created his "Game of Life" -- a set of rules that mimics the
 *   chaotic yet patterned growth of a colony of biological organisms. The "game" takes place on a two-dimensional grid
 *   consisting of "living" and "dead" cells, and the rules to step from generation to generation are simple :
 *      - Overpopulation : if a living cell is surrounded by more than three living cells, it dies;
 *      - Stasis         : if a living cell is surrounded by two or three living cells, it survives;
 *      - Underpopulation: if a living cell is surrounded by fewer than two living cells, it dies;
 *      - Reproduction   : if a dead cell is surrounded by exactly three living cells, it becomes a live cell.
 *      
 *   By enforcing these rules in sequential steps, beautiful and unexcepted patterns can appear.
 * 
 *   This class builds a grid of cells for conway life game. The boundaries of the grid are periodic :
 *   the used grid geometry is torroidal, where the grid wraps from top to bottom and left to right.
 */
class board_game {
public:
    using value_type=unsigned char;
    using container_type=std::vector<value_type>;
    using size_type=size_t;
    /**
     * @brief      Read the initial configuration from an input stream
     *
     * The format of the input stream must be as following :
     *     - Number of rows and columns for the grid
     *     - Number of the living cells in initial state
     *     - The indices i_row and j_col of each living cell in the grid
     *
     * @param      input  The input stream
     */
    board_game( std::istream& input ) : nb_rows(0), nb_cols(0), m_board()
    {
        input >> nb_rows >> nb_cols;
        container_type(nb_rows*nb_cols,value_type(0)).swap(m_board);
        size_type nb_cells, row, col;
        input >> nb_cells;
        for ( size_type i = 0; i < nb_cells; ++i )
        {
            input >> row >> col;
            (*this)(row,col) = value_type(1);
        }
    }
    board_game( const board_game& ) = delete;
    board_game( board_game&& board ) = default;
    ~board_game() = default;

    board_game& operator = ( const board_game& ) = delete;
    board_game& operator = ( board_game&& board ) = default;

    /**
     * @brief      Update the board following the conway life game.
     * 
     *  The rules of the conway life game is as following :
     *      - Overpopulation : if a living cell is surrounded by more than three living cells, it dies;
     *      - Stasis         : if a living cell is surrounded by two or three living cells, it survives;
     *      - Underpopulation: if a living cell is surrounded by fewer than two living cells, it dies;
     *      - Reproduction   : if a dead cell is surrounded by exactly three living cells, it becomes a live cell.
     */
    void update() {
        board_game& board = *this;
        container_type temp( m_board.size(), value_type(0));
        // Pour chaque cellule du tableau, on teste ses voisines :
#       pragma omp parallel for collapse(2)
        for ( size_type i = 0; i < nb_rows; ++i )
            for ( size_type j = 0; j < nb_cols; ++j )
            {
                // Compute indices for left, right, up and bottom cells ( for torroidal geometry )
                size_t left_index   = (j+nb_cols-1)%nb_cols;
                size_t right_index  = (j+1)%nb_cols;
                size_t bottom_index = (i+nb_rows-1)%nb_rows;
                size_t up_index     = (i+1)%nb_rows;
                // Count the number of living cells around the current cell ;
                value_type nb_living_neighbours = board(bottom_index, left_index) +
                                                  board(bottom_index, j)          +
                                                  board(bottom_index,right_index) +
                                                  board(i, left_index)            +
                                                  board(i, right_index)           +
                                                  board(up_index, left_index)     +
                                                  board(up_index, j)              +
                                                  board(up_index, right_index);
                if ( board(i,j) == 1 ) { // If the current cell is living :
                    temp[i*nb_cols+j] = ((nb_living_neighbours>3) || (nb_living_neighbours<2) ? 0 : 1 );
                } else  // Else if the current cell is dead
                    temp[i*nb_cols+j] = (nb_living_neighbours==3 ? 1 : 0);
            }
        temp.swap(m_board);// Update the board
    }
    gui::window& display( gui::window& scr ) const
    {
        const board_game& board = *this;
        auto fmt = scr.size();
        // Compute the size of each case to display :
        double wcell = double(fmt.first)/nb_cols;
        double hcell = double(fmt.second)/nb_rows;
        // Display living cells :
        std::list<gui::quad> cells;
        auto col = gui::color::rgba({0.75,1.,0.75});
        for ( size_type i = 0; i < nb_rows; ++i )
            for ( size_type j = 0; j < nb_cols; ++j )
            {
                double bottom_coord = i*hcell, up_coord = bottom_coord+hcell;
                double left_coord   = j*wcell, right_coord = left_coord+wcell;
                if ( board(i,j)== 1 )
                  // Compute the coordinates of each corners :
                  cells.emplace_back(gui::point{left_coord,bottom_coord,0,col},
                                     gui::point{right_coord,bottom_coord,0,col},
                                     gui::point{right_coord,up_coord,0,col},
                                     gui::point{left_coord, up_coord,0,col});

            }
        // Display a grid
        std::list<gui::segment> grid;
        auto colgrd = gui::color::rgba({0.25,0.25,0.25});
        for ( size_type j = 0; j < nb_cols; ++j )
            grid.emplace_back(gui::point{j*wcell,0,0,colgrd},
                              gui::point{j*wcell,fmt.second-1.,0,colgrd});
        for ( size_type i = 0; i < nb_rows; ++i )
            grid.emplace_back(gui::point{0,i*hcell,0,colgrd},
                              gui::point{fmt.first-1., i*hcell, 0, colgrd} );
        scr.clear();
        gui::draw(grid);
        gui::draw(cells);
        scr.blit();
        return scr;
    }

    unsigned char& operator () ( size_type i, size_type j )
    {
        return m_board[i*nb_cols+j];
    }
    unsigned char operator() (size_type i, size_type j ) const
    {
        return m_board[i*nb_cols+j];
    }
private:
    size_type nb_rows, nb_cols;
    container_type m_board;
};

gui::window& operator << ( gui::window& scr, const board_game& board )
{
    return board.display(scr);
}

int main( int nargs, char* argv[] )
{

    gui::context graphic_context(nargs, argv);
    gui::window& win =  graphic_context.new_window("Life game",800,800);
    std::string init_file_name = "static_cells.dat";
    if ( nargs > 1 )
        init_file_name = std::string(argv[1]);

    // Load an initial interesting pattern 
    std::ifstream inp_file(init_file_name);
    board_game board(inp_file);
    inp_file.close();

    // Tell to quit the program when the q key is pressed
    gui::event_manager manager;
    manager.on_key_event(int('q'), [] (int state) { exit(0); });
    manager.on_key_event(27, [] (int state) { exit(0); });// touche escape
    manager.on_display([&win, &board] () { win << board; });
    manager.on_idle([&win, &board] () { board.update(); win << board; });
    manager.loop();
    return EXIT_SUCCESS;
}