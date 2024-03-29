//
// Created by ragonit on 19.06.2022.
//

#ifndef MAIN_CPP_MINESWEEPER_H
#define MAIN_CPP_MINESWEEPER_H


#include <iostream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

class MineSweepeer {
    int frees_;
    int h, w;
public:

    int GetFreeCells() {
        return frees_;
    }

    MineSweepeer(int a, int b)
            : h(a), w(b) {
    }
    int GetNumberOfDigits(int a) {
        int i = 0;
        if (a == 0)
            return 1;
        while (a > 0) {
            a /= 10;
            ++i;
        }
        return i;
    }
    int GetPower(int a, int b) {
        int ans = 1;
        for (;b > 0; --b) {
            ans *= a;
        }
        return ans;
    }

    char GetDigitFromNumber(int a, int b) {
        int c = GetPower(10, GetNumberOfDigits(a) - b);
        int d = GetPower(10, GetNumberOfDigits(a) - b - 1);
        int l = (a % c) / d;
        if (b >= GetNumberOfDigits(a))
            return  ' ';
        return '0' + l;
    }

    void Fill(vector<vector<char>>& f,char a){
        for(int i = 0; i < h; ++i){
            for(int j = 0; j < w; ++j){
                f[i][j] = a;
            };
        }
    }
    int Print(vector<vector<char>>& f)
    {
        for (int j = 0; j < GetNumberOfDigits(w-1); ++j) {
            for (int i = 0; i < w; ++i) {
                cout << GetDigitFromNumber(i,j) << " ";
            }
            cout << "\n";
        }
        for (int j = 0; j < w; ++j)
            cout << "^" << " ";
        cout << "\n";
        for(int i = 0; i < h; ++i)
        {
            for(int j = 0; j < w; ++j)
            {
                cout << f[i][j] << " ";
            };
            cout << "> " << i << " ";
            cout << "\n";
        }
        return 1;
    }

    void FillMinesAroundCell(int y, int x, vector<vector<char>>& field) {
        if (field[y][x] == '*') {
            return;
        }
        for (int i : {-1, 0, 1}) {
            for (int j : {-1, 0, 1}) {
                int y1 = y + i;
                int x1 = x + j;
                if (y1 < 0 || y1 >= h || x1 < 0 || x1 >= w){
                    continue;
                }
                field[y][x] += field[y1][x1] == '*';
            }
        }
    }
    int CountMinesAroundCell(int y, int x, vector<vector<char>>& fr) {
        int counter = 0;
        if (fr[y][x] == '*') {
            return 0;
        }
        for (int i : {-1, 0, 1}) {
            for (int j : {-1, 0, 1}) {
                int y1 = y + i;
                int x1 = x + j;
                if (y1 < 0 || y1 >= h || x1 < 0 || x1 >= w){
                    continue;
                }
                counter += fr[y1][x1] == '*';
            }
        }
        return counter;
    }
    ///\brief Проходит по всей таблице и заполняет каждую клетку без мины числом мин воркруг этой клетки
    ///
    /// \param field - поле с минами (вектор векторов с символами)
    void FillCells(vector<vector<char>>& field){
        for (int i = 0; i < h; ++i){
            for (int j = 0; j < w; ++j){
                FillMinesAroundCell(i, j, field);

            }
        }
    }
    void SetMines(vector<vector<char>>& v,float a){
        int h=v.size();
        int w=v[0].size();
        frees_ = (h * w - ceil(h * w * a / 100));
        for(int i = 0; i < ceil(h * w * a / 100); ++i){
            int x = rand() % w;
            int y = rand() % h;
            if(v[y][x] == '*') {
                --i;
                continue;
            }
            v[y][x] = '*';
        }
    }
    bool OpenAroundNumber(vector<vector<char>>& fr, vector<vector<char>>& bck, int y, int x) {
        if (CountMinesAroundCell(y, x, fr) > 0) {
            for (int i : {-1, 0, 1}) {
                for (int j : {-1, 0, 1}) {
                    int y1 = y + i;
                    int x1 = x + j;
                    if (y1 < 0 || y1 >= h || x1 < 0 || x1 >= w){
                        continue;
                    }
                    if (fr[y1][x1] == '*') {
                        if (!open(fr, bck, y1, x1))
                            return false;
                    }
                }
            }
        }
        return true;
    }
    void OpenZeros(vector<vector<char>>& fr, vector<vector<char>>& bck, int y, int x){
        if (fr[y][x] == bck[y][x])
            return;
        else
        {
            --frees_;
            fr[y][x] = bck[y][x];
        }
        for (int i : {-1, 0, 1}) {
            for (int j : {-1, 0, 1}) {
                int y1 = y + i;
                int x1 = x + j;
                if (y1 < 0 || y1 >= h || x1 < 0 || x1 >= w){
                    continue;
                }
                if (bck[y1][x1] == '0' && fr[y1][x1] == '0'){
                    continue;
                }
                if (bck[y1][x1] == '0'){
                    OpenZeros(fr, bck, y1, x1);
                }
                if (bck[y1][x1] != fr[y1][x1])
                {
                    --frees_;
                    fr[y1][x1] = bck[y1][x1];
                }
            }
        }
    }
    bool open(vector<vector<char>>& fr, vector<vector<char>>& bck, int y, int x){
        if (bck[y][x] == '*')
            return false;
        if (fr[y][x] == '|')
            return true;
        if (fr[y][x] == bck[y][x])
            return OpenAroundNumber(fr, bck, y, x);
        if (fr[y][x] == '*')
        {
            if (bck[y][x] == '0')
                OpenZeros(fr, bck, y, x);
            else
            {
                fr[y][x] = bck [y][x];
                --frees_;
            }
        }
        return true;
    }

};


#endif //MAIN_CPP_MINESWEEPER_H
