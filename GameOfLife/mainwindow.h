/* Class: MainWindow
 *
 * Game of Life
 *
 * This project is my C++ implementation of Conway's Game of Life.
 * Game of Life is a zero-player game, and the progress of the game depends on
 * its initial state that user can decide by clicking cells alive.
 * The game is a two-dimensional grid that consists of the same sized
 * square cells. Every cell has eight neighbours and the game progresses
 * in states, which are determined by the amount of alive neighbour cells.
 *
 * Rules of the game and more detailed instructions can be found in the
 * Instructions.txt file.
 *
 * Created by: Roosa Rintamäki
 * roosa.rintamaki@tuni.fi
 * Date: 12.3.2022
 * */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QMainWindow>
#include <QPushButton>
#include <vector>
#include <unordered_set>
#include <cell.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Node {
// This struct links the cell to its neighbour cells. Neighbours are stored
// in an unordered set.

    std::unordered_set<Node*> neighbourCells;
    Cell* cell;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void game();

private slots:
    void on_StartButton_clicked();

    void on_ClearButton_clicked();

    void on_boardDimension_valueChanged(int arg1);

    void on_createBoard_clicked();

    void on_NextButton_clicked();

private:
    Ui::MainWindow *ui;

    //private functions
    void createBoard();
    void setInitialState(Cell* c);
    void calculateCells();
    int checkNeighbours(Node* node);
    void createNodes(std::vector<std::vector<Node*>> &nodes);
    void linkNodes(std::vector<std::vector<Node*>> &nodes);
    void populateNodes(std::vector<std::vector<Node*>> &nodes);
    void disableCells();
    void setNewState();
    void changePushButtonsState(bool state);


    //private attributes
    QTimer *timer;
    std::unordered_set<Node*> allCells;
    std::vector<Node*> dyingCells;
    std::vector<Node*> awakeningCells;
    QWidget *widget;
    QGridLayout *layout;
    int cellAmount = 0;
    int gridSize = 10;
    int round = 0;
    bool gameState  = false;

    //Constant for pushbutton's (=cell) size
    static const int CELL_SIZE = 13;

};
#endif // MAINWINDOW_H
