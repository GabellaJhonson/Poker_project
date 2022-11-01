#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <ctime>
//#include "Combinations.h"
using namespace std;
 
class Card {
public:
    string Nominalo;
    string Colour;
    int Power;
    bool DrawLock;
    vector <string> Print;
    //
 
    Card (string N = "no", char Col = 0, int Pow = 0, bool Lock = 0) : Nominalo(N), Power(Pow), DrawLock(Lock) {
        Colour = Col + 256;
        if (DrawLock == true) {
            string s;
            Print.push_back(" ______");
            s = "|" + Nominalo + "    |";
            Print.push_back(s);
            s = "| " + Colour + " " + Colour + " |";
            Print.push_back(s);
            Print.push_back(s);
            Print.push_back(s);
            s = "|____" + Nominalo + "|";
            Print.push_back(s);
            if (Nominalo == "10") {
                Print[1] = "|10   |";
                Print[5] = "|___10|";
            }
        }
        else {
            Print = { " _____ ","|\\ ~ /|","|}}:{{|", "|}}:{{|", "|}}:{{|","|/_~_\\|" };
        }
       }
    Card(const Card& other) {
        Nominalo = other.Nominalo;
        Colour = other.Colour;
        Power = other.Power;
        DrawLock = other.DrawLock;
        Print = other.Print;
    }
    ~Card() {
        Print.clear();
    }
    Card& operator = (const Card& other) {
        Nominalo = other.Nominalo;
        Colour = other.Colour;
        Power = other.Power;
        DrawLock = other.DrawLock;
        Print = other.Print;
        return *this;
    }
    void Reverse() {
        if (DrawLock) {
            Lock();
        }
        else {
            UnLock();
        }
    }
    void Lock() {
        DrawLock = false;
        Print.clear();
        Print = {" _____ ","|\\ ~ /|","|}}:{{|", "|}}:{{|", "|}}:{{|","|/_~_\\|"};
    }
    void UnLock() {
        DrawLock = true;
        Print.clear();
        string s;
        Print.push_back(" ______");
        s = "|" + Nominalo + "    |";
        Print.push_back(s);
        s = "| " + Colour + " " + Colour + " |";
        Print.push_back(s);
        Print.push_back(s);
        Print.push_back(s);
        s = "|____" + Nominalo + "|";
        Print.push_back(s);
        if (Nominalo == "10") {
            Print[1] = "|10   |";
            Print[5] = "|___10|";
        }
    }
};
 
void CreateDeck(vector <Card>& Deck) {
    Deck.clear();
    srand(unsigned(time(0)));
    vector <string> Names{ "2", "3", "4", "5", "6",
                          "7", "8", "9", "10", "B", "D", "K", "T"};
 
    vector <char> Colours{03, 04, 05, 06};
    
    for (int i = 0; i < 52; i++)                                           //create Deck
    {
        Card C(Names[i / 4], Colours[i % 4], i / 4, true);
        Deck.push_back(C); 
    }
    //random_shuffle(Deck.begin(), Deck.end());
    for (int i = 0; i < 52; i++)
    {
        swap(Deck[i], Deck[rand() % 52]);
    }
}
 
int CheckStreetFlash(vector <vector <bool>> Matrix);
int CheckCare(vector <vector <bool>> Matrix);
int CheckFullHouse(vector <vector <bool>> Matrix);
int CheckFlash(vector <vector <bool>> Matrix);
int CheckStreet(vector <vector <bool>> Matrix);
int CheckSet(vector <vector <bool>> Matrix);
int CheckDoublePare(vector <vector <bool>> Matrix);
int CheckPare(vector <vector <bool>> Matrix);
vector <vector <bool>> MatrixComb(vector <Card> Table, vector <Card> Hand) {
    vector <vector <bool>> Matrix(4, vector <bool>(13));
    int k = 0;
    for (Card i : Table) {
        if ((i.Colour) == "\x3") {
            k = 0;
        }
        if ((i.Colour) == "\x4") {
            k = 1;
        }
        if ((i.Colour) == "\x5") {
            k = 2;
        }
        if ((i.Colour) == "\x6") {
            k = 3;
        }
        Matrix[k][i.Power] = true;
    }
    for (Card i : Hand) {
        if ((i.Colour) == "\x3") {
            k = 0;
        }
        if ((i.Colour) == "\x4") {
            k = 1;
        }
        if ((i.Colour) == "\x5") {
            k = 2;
        }
        if ((i.Colour) == "\x6") {
            k = 3;
        }
        Matrix[k][i.Power] = true;
    }
    return Matrix;
}
// Efective matrix
class Combination {
private:
    string  Name;
    int HightCard;
    int PowComb;                 // power of combination (Pare == 1; DoublePare == 2)
    int PowNominal;         // 8, 9, 10, В, Д...
public:
    Combination() {
        Name = "NONE";
        HightCard = -1;
        PowComb = -1;
        PowNominal = -1;
    }
    Combination(vector <Card> Hand, vector <Card> Table = {}) {
        vector <vector <bool>> Matrix;
        Matrix = MatrixComb(Table, Hand);
        int box = 0;
        box = CheckStreetFlash(Matrix);
        if (box != -1) {
            Name = "StreetFlash";
            HightCard = box;
            PowNominal = box;
            PowComb = 8;
            return;
        }
        box = CheckCare(Matrix);
        if (box != -1) {
            Name = "Care";
            for (int j = 12; j >= 0; j--)
            {
                bool flag = true;
                for (int i = 0; i < 4; i++)
                {
                    if (Matrix[i][j] && j != box) {
                        HightCard = j;
                        flag = false;
                        break;
                    }
                }
                if (!flag) break;
 
            }
            PowNominal = box;
            PowComb = 7;
            return;
        }
        box = CheckFullHouse(Matrix);
        if (box != -1) {
            Name = "FullHouse";
            int count = 0;
            for (int j = 12; j >= 0; j--)
            {
                count = 0;
                for (int i = 0; i < 4; i++)
                {
                    if (Matrix[i][j])
                        count++;
                }
                if (count == 2) {
                    HightCard = j;
                }
            }
            PowNominal = box;
            PowComb = 6;
            return;
        }
        box = CheckFlash(Matrix);
        if (box != -1) {
            Name = "Flash";
            HightCard = box;
            PowNominal = box;
            PowComb = 5;
            return;
        }
        box = CheckStreet(Matrix);
        if (box != -1) {
            Name = "Street";
            HightCard = box;
            PowNominal = box;
            PowComb = 4;
            return;
        }
        box = CheckSet(Matrix);
        if (box != -1) {
            Name = "Set";
            for (int j = 12; j >= 0; j--)
            {
                bool flag = true;
                for (int i = 0; i < 4; i++)
                {
                    if (Matrix[i][j] && j != box) {
                        HightCard = j;
                        flag = false;
                        break;
                    }
                }
                if (!flag) break;
 
            }
            PowNominal = box;
            PowComb = 3;
            return;
        }
        box = CheckDoublePare(Matrix);         //??
        if (box != -1) {
            Name = "DoublePare";
            for (int j = 12; j >= 0; j--)
            {
                bool flag = true;
                for (int i = 0; i < 4; i++)
                {
                    if (Matrix[i][j] && j != box) {
                        HightCard = j;
                        flag = false;
                        break;
                    }
                }
                if (!flag) break;
 
            }
            PowNominal = box;
            PowComb = 2;
            return;
        }
        box = CheckPare(Matrix);
        if (box != -1) {
            Name = "Pare";
            for (int j = 12; j >= 0; j--)
            {
                bool flag = true;
                for (int i = 0; i < 4; i++)
                {
                    if (Matrix[i][j] && j != box) {
                        HightCard = j;
                        flag = false;
                        break;
                    }
                }
                if (!flag) break;
            }
            PowNominal = box;
            PowComb = 1;
            return;
        }
        //
        Name = "Nothing";
        PowComb = 0;
        PowNominal = 0;
        for (int j = 12; j >= 0; j--)
        {
            bool flag = true;
            for (int i = 0; i < 4; i++)
            {
                if (Matrix[i][j]) {
                    HightCard = j;
                    flag = false;
                    break;
                }
            }
            if (!flag) break;
        }
    }
    Combination(const Combination& other) {
        Name = other.Name;
        HightCard = other.HightCard;
        PowComb = other.PowComb;
        PowNominal = other.PowNominal;
    };
    ~Combination() {}
    Combination& operator = (const Combination& other) {
        Name = other.Name;
        HightCard = other.HightCard;
        PowComb = other.PowComb;
        PowNominal = other.PowNominal;
        return *this;
    }
    void update(vector <Card> Hand, vector <Card> Table) {
        Combination C(Hand, Table);
        *this = C;
    }
    bool operator < (const Combination& other) {
        if (PowComb < other.PowComb) {
            return true;
        }
        if (PowNominal < other.PowNominal) {
            return true;
        }
        if (HightCard < other.HightCard) {
            return true;
        }
        return false;
    }
    bool operator == (const Combination& other) {
        if (HightCard == other.HightCard && PowComb == other.PowComb && PowNominal == other.PowNominal) {
            return true;
        }
        else return false;
    }
    bool operator > (const Combination& other) {
        if (*this == other || *this < other) return false;
        else return true;
    }
    string name() {
        return Name;
    }
    int power() {
        return PowComb;
    }
};
int CheckStreetFlash(vector <vector <bool>> Matrix) {
    for (int i = 0; i < 4; i++)
    {
        bool flag = true;
        for (int j = 0; j < 9; j++)
        {
            if (Matrix[i][j] && Matrix[i][j + 1] && Matrix[i][j + 2]
                && Matrix[i][j + 3] && Matrix[i][j + 4]) {
                return j + 4;
            }
        }
    }
    return -1;
}
int CheckCare(vector <vector <bool>> Matrix) {
    for (int i = 0; i < 13; i++)
    {
        if (Matrix[0][i] && Matrix[1][i] && Matrix[2][i] && Matrix[3][i]) {
            return i;
        }
    }
    return -1;
}
int CheckFullHouse(vector <vector <bool>> Matrix) {
    vector <int> countes(13);
    int count = 0;
    int hight = 0;
    bool flag1 = false;
    bool flag2 = false;
    for (int j = 12; j >= 0; j--)
    {
        count = 0;
        for (int i = 0; i < 4; i++)
        {
            if (Matrix[i][j])
                count++;
        }
        countes[j] = count;
    }
    for (int i = 0; i < 13; i++)
    {
        if (flag1 && flag2) {
            return hight;
        }
        if (countes[i] == 3) {
            flag1 = true;
            hight = i;
        }
        if (countes[i] == 2) {
            flag2 = true;
        }
    }
    return -1;
}
int CheckFlash(vector <vector <bool>> Matrix) {
    int count = 0;
    bool flag;
    int hight = 0;
    for (int j = 0; j < 4; j++)
    {
        count = 0;
        hight = 0;
        for (int i = 12; i >= 0; i--)
        {
            flag = true;
            if (Matrix[j][i]) {
                if (hight == 0) {
                    hight = i;
                }
                count++;
            }
        }
        if (count >= 5) {
            for (int i = 12; i >= 0; i--)
            {
                if (Matrix[j][i]) return hight;
            }
        }
    }
    return -1;
}
int CheckStreet(vector <vector <bool>> Matrix) {
    vector <bool> nom(13);
    for (int j = 12; j >= 0; j--)
    {
        for (int i = 0; i < 4; i++)
        {
            if (Matrix[i][j]) {
                nom[j] = true;
                break;
            }
        }
    }
    int count = 0;
    bool flag = false;
    int hight = 0;
    for (int j = 12; j >= 4; j--)
    {
        if (nom[j] && nom[j - 1] && nom[j - 2] && nom[j - 3] && nom[j - 4]) {
            return j;
        }
    }
    return -1;
}
int CheckSet(vector <vector <bool>> Matrix) {
    vector <int> countes(13);
    int count = 0;
    for (int j = 12; j >= 0; j--)
    {
        count = 0;
        for (int i = 0; i < 4; i++)
        {
            if (Matrix[i][j])
                count++;
        }
        countes[j] = count;
    }
    for (int i = 0; i < 13; i++)
    {
        if (countes[i] == 3) {
            return i;
        }
    }
    return -1;
}
int CheckDoublePare(vector <vector <bool>> Matrix) {
    vector <int> countes(13);
    int count = 0;
    bool flag = false;
    int hight = 0;
    for (int j = 12; j >= 0; j--)
    {
        count = 0;
        for (int i = 0; i < 4; i++)
        {
            if (Matrix[i][j])
                count++;
        }
        countes[j] = count;
    }
    for (int i = 12; i >= 0; i--)
    {
        if (countes[i] == 2) {
            if (flag) {
                return hight;
            }
            hight = i;
            flag = true;
        }
    }
    return -1;
}
int CheckPare(vector <vector <bool>> Matrix) {
    vector <int> countes(13);
    int count = 0;
    for (int j = 12; j >= 0; j--)
    {
        count = 0;
        for (int i = 0; i < 4; i++)
        {
            if (Matrix[i][j])
                count++;
        }
        countes[j] = count;
    }
    for (int i = 0; i < 13; i++)
    {
        if (countes[i] == 2) {
            return i;
        }
    }
    return -1;
}
class PlayerStruct {
public:
    string Name;
    vector <Card> Hand;
    int Kapital;
    bool Fold;
    int GameStaff;
    Combination Comb;
 
    PlayerStruct() {
        GameStaff = 0;
        Card C1;
        Card C2;
        Hand.push_back(C1);
        Hand.push_back(C2);
    };
    PlayerStruct(string N) :Name(N) {
    };
    PlayerStruct(const PlayerStruct& other) {
        Name = other.Name;
        Hand = other.Hand;
        Kapital = other.Kapital;
        Fold = other.Fold;
        GameStaff = other.GameStaff;
        Comb = other.Comb;
    };
    ~PlayerStruct() {}
    PlayerStruct& operator = (const PlayerStruct& other) {
        Name = other.Name;
        Hand = other.Hand;
        Kapital = other.Kapital;
        Fold = other.Fold;
        GameStaff = other.GameStaff;
        Comb = other.Comb;
        return *this;
    }
    void clear() {
        GameStaff = 0;
        Fold = false;
    }
};
 
/*   | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | В | Д | К | Т |
чирва| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 |  0 | 0 | 0 | 0 | 0 |
бубна| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 |  0 | 0 | 0 | 0 | 0 |
пика | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 |  0 | 0 | 0 | 0 | 0 |
треф | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 |  0 | 0 | 0 | 0 | 0 |
 
SUM  | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 4 |  0 | 0 | 0 | 0 | 0 |
*/
