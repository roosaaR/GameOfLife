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

    bool getCellState();


private:
    // If cellState = true, cell is alive and if
    // cellState = false, cell is dead
    int cellState = false;
};

#endif // CELL_H
