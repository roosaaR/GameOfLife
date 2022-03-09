#ifndef CELL_H
#define CELL_H
#include <QPushButton>

class Cell : public QPushButton
{
    Q_OBJECT
public:
    //Constructor: Creates a new cell object
    Cell(QWidget *parent = nullptr);

    void setCellState(int state);
    int getCellState();


private:
    // If cellState = 1, cell is alive and if cellState = 0,
    // cell is dead
    int cellState = 0;
};

#endif // CELL_H
