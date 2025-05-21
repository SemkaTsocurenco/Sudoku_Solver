#include <iostream>
#include "mainwindow.h"
#include <qchar.h>
#include <qwidget.h>
#include <utility>
#include <vector>
#include <bits/stdc++.h>


#include <QApplication>
#include <QMainWindow>
#include <QGridLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QWidget>
#include <vector>
#include <map>
#include <random>
#include <climits>
#include <omp.h>      

using namespace std;


int calculateConflicts(const vector<vector<int>>& g, int base)
{
    const int N = base * base;
    int conflicts = 0;

    /* ---------- строки ---------- */
#pragma omp parallel for reduction(+:conflicts) schedule(static)
    for (int r = 0; r < N; ++r) {
        std::array<int, 36> cnt{};           // N ≤ 25 ⇒ 26, берём с запасом
        for (int c = 0; c < N; ++c) ++cnt[g[r][c]];
        for (int v = 1; v <= N; ++v) if (cnt[v] > 1) conflicts += cnt[v] - 1;
    }

    /* ---------- столбцы ---------- */
#pragma omp parallel for reduction(+:conflicts) schedule(static)
    for (int c = 0; c < N; ++c) {
        std::array<int, 36> cnt{};
        for (int r = 0; r < N; ++r) ++cnt[g[r][c]];
        for (int v = 1; v <= N; ++v) if (cnt[v] > 1) conflicts += cnt[v] - 1;
    }

    /* ---------- блоки ---------- */
#pragma omp parallel for collapse(2) reduction(+:conflicts) schedule(static)
    for (int br = 0; br < N; br += base)
        for (int bc = 0; bc < N; bc += base) {
            std::array<int, 36> cnt{};
            for (int dr = 0; dr < base; ++dr)
                for (int dc = 0; dc < base; ++dc)
                    ++cnt[g[br + dr][bc + dc]];
            for (int v = 1; v <= N; ++v) if (cnt[v] > 1) conflicts += cnt[v] - 1;
        }
    return conflicts;
}


bool hasInitialConflicts(const vector<vector<int>>& g, int base)
{
    return calculateConflicts(g, base) != 0;
}



class TabuSolver {
public:
    TabuSolver(const vector<vector<int>>& initial,
               int base,                 
               int maxIter     = 10000,
               int tabuTenure  =     10)
        : init(initial), base(base),
          N(base * base),
          maxIter(maxIter), tabuTenure(tabuTenure)
    {
        if (int(init.size()) != N)
            throw runtime_error("Неверная размерность начальной сетки");

        fixed.assign(N, vector<bool>(N, false));
        for (int r = 0; r < N; ++r)
            for (int c = 0; c < N; ++c) {
                if (init[r][c] < 0 || init[r][c] > N)
                    throw runtime_error("Число вне диапазона 1..N");
                if (init[r][c] != 0) fixed[r][c] = true;
            }

        if (hasInitialConflicts(init, base))
            throw runtime_error("Стартовая позиция содержит конфликты — судоку нерешаемо");

        for (int br = 0; br < N; br += base)
            for (int bc = 0; bc < N; bc += base) {
                vector<pair<int,int>> v;
                for (int dr = 0; dr < base; ++dr)
                    for (int dc = 0; dc < base; ++dc)
                        v.emplace_back(br + dr, bc + dc);
                subgridCells.push_back(move(v));
            }
    }

    vector<vector<int>> solve()
    {
        grid = init;
        mt19937 gen(random_device{}());

        // случайно заполняем каждый блок
        for (auto& cells : subgridCells) {
            vector<bool> present(N + 1, false);
            for (auto [r,c] : cells) if (grid[r][c] != 0) present[grid[r][c]] = true;

            vector<int> missing;
            for (int v = 1; v <= N; ++v) if (!present[v]) missing.push_back(v);
            shuffle(begin(missing), end(missing), gen);

            int k = 0;
            for (auto [r,c] : cells)
                if (grid[r][c] == 0) grid[r][c] = missing[k++];
        }

        int bestConf = calculateConflicts(grid, base);
        auto bestGrid = grid;

        // (ключ «перестановка двух клеток») → итерация-до-которой-табу
        unordered_map<uint64_t, int> tabu;  // компактный ключ

        auto key = [this](int r1,int c1,int r2,int c2){
            return (uint64_t(r1) << 48) | (uint64_t(c1) << 32) |
                   (uint64_t(r2) << 16) | uint64_t(c2);
        };

        for (int iter = 0; iter < maxIter && bestConf > 0; ++iter)
        {
            int curConf   = calculateConflicts(grid, base);
            int bestDelta = INT_MAX;
            tuple<int,int,int,int> bestMove;
            bool found = false;

            for (auto& cells : subgridCells) {
                int sz = cells.size();
                for (int i = 0; i < sz; ++i)
                    for (int j = i+1; j < sz; ++j) {
                        auto [r1,c1] = cells[i];
                        auto [r2,c2] = cells[j];
                        if (fixed[r1][c1] || fixed[r2][c2]) continue;

                        swap(grid[r1][c1], grid[r2][c2]);
                        int newConf = calculateConflicts(grid, base);
                        int delta   = newConf - curConf;
                        uint64_t k  = key(r1,c1,r2,c2);

                        bool isTabu = tabu.count(k) && tabu[k] > iter;
                        if ((!isTabu || newConf < bestConf) && delta < bestDelta) {
                            bestDelta = delta;
                            bestMove  = {r1,c1,r2,c2};
                            found = true;
                        }
                        swap(grid[r1][c1], grid[r2][c2]);
                    }
            }
            if (!found) break;

            auto [r1,c1,r2,c2] = bestMove;
            swap(grid[r1][c1], grid[r2][c2]);
            tabu[key(r1,c1,r2,c2)] = iter + tabuTenure;

            int cur = calculateConflicts(grid, base);
            if (cur < bestConf) { bestConf = cur; bestGrid = grid; }
            if (bestConf == 0) break;
        }
        return bestGrid;
    }

private:
    const vector<vector<int>> init;
    vector<vector<int>> grid;
    vector<vector<bool>> fixed;
    vector<vector<pair<int,int>>> subgridCells;

    const int base;        
    const int N;            
    const int maxIter;
    const int tabuTenure;
};


QString MainWindow::getStyle(int initVal,  
                             int row, int col,
                             int base)
{
    const int top    = (row % base == 0)          ? 3 : 1;
    const int left   = (col % base == 0)          ? 3 : 1;
    const int right  = ((col + 1) % base == 0)    ? 3 : 1;
    const int bottom = ((row + 1) % base == 0)    ? 3 : 1;

    const char* txtColor   = !initVal ? "black"   : "#2e86de";
    const char* backColor  = !initVal ? "#e6ffff" : "#b5d6d6";

    return QString(
        "QLineEdit{background:%5;color:%6;font:20px bold;"
        "border-top:%1px solid black;border-left:%2px solid black;"
        "border-right:%3px solid black;border-bottom:%4px solid black;"
        "qproperty-alignment:AlignCenter;}")
        .arg(top).arg(left).arg(right).arg(bottom)
        .arg(backColor).arg(txtColor);
}

void MainWindow::solveSudoku()
{
    const int base = baseSelector->value();   
    const int N    = base * base;

    std::vector<std::vector<int>> initial(N, std::vector<int>(N));
    bool ok;
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c) {
            int v = cells[r][c]->text().toInt(&ok);
            initial[r][c] = ok ? v : 0;       
        }

    try {
        TabuSolver solver(initial, base);
        auto solved = solver.solve();

        for (int r = 0; r < N; ++r)
            for (int c = 0; c < N; ++c) {
                cells[r][c]->setText(QString::number(solved[r][c]));
                cells[r][c]->setStyleSheet(
                    getStyle(initial[r][c], r, c, base));
            }
    }
    catch (const std::exception& e) {           // неверный ввод, нерешаемо…
        QMessageBox::warning(this, "Sudoku",
                             QString::fromStdString(e.what()));
    }
}