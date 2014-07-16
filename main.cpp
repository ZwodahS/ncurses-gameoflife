#include <ncurses.h>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <iostream>
struct Cell
{
    Cell(const int& x, const int& y)
        : x(x), y(y), aliveNow(false), aliveNext(false)
    {
    }
    const int x;
    const int y;
    bool aliveNow;
    bool aliveNext;
};
int height, width;

int modX[] = {-1, 0, 1, 1, 1, 0, -1, -1};
int modY[] = {-1, -1, -1, 0, 1, 1, 1, 0};
int speed[] = {300, 200, 100, 50, 25};
int Max_Speed = 4;

int countAlive(std::vector<std::vector<Cell*> >& cells, Cell& cell)
{
    int count = 0;
    for (int i = 0; i < 8; i++)
    {
        int x = modX[i] + cell.x;
        int y = modY[i] + cell.y;
        if ( x >= 0 && x < width && y >= 0 && y < height)
        {
            if (cells[x][y]->aliveNow)
            {
                count++;
            }
        }
    }
    return count;
}

void step(std::vector<std::vector<Cell*> >& cells, Cell& cell)
{
    int count = countAlive(cells, cell);
    if (cell.aliveNow)
    {
        if (count < 2 || count > 3)
        {
            cell.aliveNext = false;
        }
        else
        {
            cell.aliveNext = true;
        }
    }
    else
    {
        if (count == 3)
        {
            cell.aliveNext = true;
        }
    }
}

void update(std::vector<std::vector<Cell*> >& cells, Cell& cell)
{
    cell.aliveNow = cell.aliveNext;
    cell.aliveNext = false;
}

void print(std::vector<std::vector<Cell*> >& cells, Cell& cell)
{
    mvwaddch(stdscr, cell.y, cell.x, cell.aliveNow ? 'X' : ' ');
}

void random(std::vector<std::vector<Cell*> >& cells, Cell& cell)
{
    cell.aliveNow = (rand() % 5 == 0) ? true : false;
}

void map(std::vector<std::vector<Cell*> >& cells, void (*apply)(std::vector<std::vector<Cell*> >& cells, Cell& cell) )
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            apply(cells, *cells[x][y]);
        }
    }
}

int main(int argc, char* argv[]) 
{
    initscr();
    nonl();
    cbreak();
    noecho();
    curs_set(0);
    getmaxyx(stdscr, height, width);
    std::vector<std::vector<Cell*> > cells;   
    for (int x = 0; x < width; x++)
    {
        std::vector<Cell*> columns;
        for (int y = 0; y < height; y++)
        {
            columns.push_back(new Cell(x, y));
        }
        cells.push_back(columns);
    }

    timeout(150);
    while(true)
    {
        // update the cell
        // 1. step their next value
        map(cells, step);
        // 2. update their next value
        map(cells, update);
        map(cells, print);
        int ch = getch();
        if (ch != ERR)
        {
            if (ch == 'r')
            {
                map(cells, random);
            }
            else if(ch >= '1' && ch <= '1' + Max_Speed)
            {
                timeout(speed[ch-'1']);
            }
        }
        wrefresh(stdscr);
    }
    endwin();
    return 0;
}
