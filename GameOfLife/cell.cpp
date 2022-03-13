#include "cell.h"


void Cell::setCellState(bool state) {
// Sets cell alive, if parameter is true and dead if parameter is false.

    if (state == false) {
        this->cellState = false;
    } else if (state == true) {
        this->cellState = true;
    }
}

bool Cell::getCellState() {
// Returns true if cell is alive and false if cell is dead.
    return this->cellState;
}

Cell::Cell(QWidget *parent) : QPushButton(parent)
{}
