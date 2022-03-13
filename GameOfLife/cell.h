/* Class: Cell
 *
 * Cell class creates a new cell object of each cell.
 * This class stores the cell state, which is either "dead" or "alive",
 * and returns it as a boolean value.
 *
 * Created by: Roosa Rintamäki
 * roosa.rintamaki@tuni.fi
 */

#ifndef CELL_H
#define CELL_H
#include <QPushButton>

class Cell : public QPushButton
{
    Q_OBJECT
public:
    //Constructor: Creates a new cell object
    Cell(QWidget *parent = nullptr);

    void setCellState(bool state);

    bool getCellState();


private:
    // If cellState is true, the cell is alive and if cellState is false,
    // the cell is dead
    bool cellState = false;
};

#endif // CELL_H
