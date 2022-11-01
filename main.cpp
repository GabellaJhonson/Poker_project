#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <conio.h>
#include <algorithm>
#include "Struct.h"
//#include "Combinations.h"
using namespace std;
 
enum eDirection { CALL, FOLD, HIDE_SHOW, RISE};
eDirection dir;
vector <PlayerStruct> Player;
vector <Card> Deck;
vector <Card> Table;
int bank = 0;              // общий банк
int staff;               // ставка на круге
int isCALL;
int isFold;
 
int testCount = 0;
double noCount = 0;
double pareCount = 0;
double dpCount = 0;
double setCount = 0;
double flCount = 0;
double stCount = 0;
double fhCount = 0;
double careCount = 0;
double stflashCount = 0;
 
void StartGame() {
    setlocale(LC_ALL, "Rus");
    int KAPITAL;
    int n;
    cout << "ВВЕДИТЕ КОЛИЧЕСТВО ИГРОКОВ:";
    string s;
    cin >> s;
    while (!isdigit(s[0]) || s.size() != 1 || s[0] == '0' || s[0] == '1') {
        system("cls");
        Sleep(1000);
        cout << "..." << endl;
        Sleep(500);
        cout << "..." << endl;
        Sleep(300);
        cout << "попробуй еще раз:" << endl;
        s.clear();
        cin >> s;
    }
    n = stoi(s);
    cout << "Начальный капитал каждого: ";
    cin >> s;
    while (s.empty() || find_if(s.begin(),
            s.end(), [](char c) { return !isdigit(c); }) != s.end() || s[0] == '0') {
        system("cls");
        Sleep(1000);
        cout << "..." << endl;
        Sleep(500);
        cout << "..." << endl;
        Sleep(300);
        cout << "попробуй еще раз:" << endl;
        s.clear();
        cin >> s;
    }
    KAPITAL = stoi(s);
    for (int i = 0; i < n; i++) {
        cout << "Введите имя игрока под номером " << i + 1 << ": ";
        Player.push_back(PlayerStruct());
        cin >> Player[i].Name;
        Player[i].Kapital = KAPITAL;
        Player[i].Fold = false;
    }
}
void Distribution(){
    for (int i = 0; i < (int)(Player.size()); i++)
    {
        Card C;
        C = Deck[Deck.size() - 1];  //раздали по две карты
        Deck.pop_back();
        Player[i].Hand[0] = C;
        C = Deck[Deck.size() - 1];
        Deck.pop_back();
        Player[i].Hand[1] = C;
       // Player[i].Comb.update(Player[i].Hand, Table);
    }
    for (int i = 0; i < 5; i++)
    {
        Card C;
        C = Deck[Deck.size() - 1]; // заполнили стол
        C.Lock();
        Deck.pop_back();
        Table.push_back(C);
    }
}
void Draw(int index) {
    system("cls");
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 5; j++) {
            cout << Table[j].Print[i] << "    ";
        }
        cout << endl;
    }
    cout << endl << endl;
    for (int i = 0; i < 6; i++)
    {
        cout << "                ";
        for (int j = 0; j < 2; j++) {
            cout << Player[index].Hand[j].Print[i] << "    ";
        }
        cout << endl;
    }
    cout << endl << Player[index].Name << endl;
    cout <<"                                              " << Player[index].Comb.name() << endl;
    cout << "j. CALL" << endl;
    cout << "+. RISE +" << staff << endl;
    cout << "z. HIDE/SHOW " << endl; // if you are tormoz and press UP+ more then you need
    cout << "l. FOLD " << endl << endl << endl;
    cout << "MY KAPITAL: " << Player[index].Kapital << endl;
    cout << "BANK " << bank << endl;
}
void Input() {
    bool flag = true;
    while (flag) {
        if (_kbhit()) { // возвращает тру если пользователь нажал на что-нибудь
            switch (_getch()) // на какую именно нажал пользовватель
            {
            case 'j':
                dir = CALL;
                flag = false;
                break;
            case 'l':
                dir = FOLD;        //FOLD
                flag = false;
                break;
            case 'z':
                dir = HIDE_SHOW;
                flag = false;
                break;
                case '+':
                dir = RISE;
                flag = false;
                break;
            }
        }
    }
}
bool Logic(int index) {
    Input();
    switch (dir) 
    {
    case CALL:
        Player[index].Kapital -= (staff - Player[index].GameStaff);
        bank += staff - Player[index].GameStaff;
        Player[index].GameStaff = staff;
        isCALL++;
        return false;
    case RISE:
        if (staff <= (Player[index].Kapital - 25)) {
            staff += 25;
            isCALL = 0;
        }  
        break;
    case FOLD:
        Player[index].Fold = true;
        isFold++;
        return false;
        break;
    case HIDE_SHOW:
        Player[index].Hand[0].Reverse();
        Player[index].Hand[1].Reverse();
        break;
    }
    return true;
}
bool Continue() { 
    bank = 0;
    Table.clear();
    vector <PlayerStruct> Box;
    for (int i = 0; i < (int)(Player.size()); i++)
    {
        if (Player[i].Kapital != 0) {
            Box.push_back(Player[i]);
        }
    }
    Player.clear();
    Player = Box;
    if (Player.size() == 1) {
        system("cls");
        Sleep(500);
        cout << Player[0].Name << ", но никто не пришел..." << endl;
        Sleep(1000);
        return false;
    }
    for (size_t i = 0; i < Player.size(); i++)
    {
        Player[i].clear();
    }
    Box.clear();
    return true; 
}
void SetupRound(int stage){
    staff = 0;
    isCALL = 0;     // счетчик CALL подряд (должно быть равным количеству игроков, тогда переход на stage)
    vector <Card> box;
    switch (stage)
    {
    case 0:
        break;
    case 1:
        Table[0].UnLock();
        Table[1].UnLock();
        Table[2].UnLock();
        break;
    case 2:
        Table[3].UnLock();
        break;
    case 3:
        Table[4].UnLock();
        break;
    }
    for (size_t i = 0; i < Table.size(); i++)
    {
        if (Table[i].DrawLock) box.push_back(Table[i]);
    }
    for (PlayerStruct &i : Player) {                                                     
        i.GameStaff = 0;
        i.Comb.update(i.Hand, box);
    }               
}
void CreateDeck(vector <Card>& Deck);
void CheckWinner() {
    sort(Player.begin(), Player.end(), [](PlayerStruct a, PlayerStruct b) {
        return a.Comb > b.Comb;
        });
    if (Player[0].Comb > Player[1].Comb) {
        Draw(0);
        cout << Player[0].Name << " WIN!!!" << endl;
        Player[0].Kapital += bank;
    }
    else {
        vector <PlayerStruct> winners;
        winners.push_back(Player[0]);
        for (size_t i = 1; i < Player.size(); i++)
        {
            if (Player[i].Comb == Player[i - 1].Comb && Player[i].Fold == false)
                winners.push_back(Player[i]);
        }
        bank /= winners.size();
        for (size_t i = 0; i < winners.size() - 1; i++)
        {
            cout << winners[i].Name << ", ";
            winners[i].Kapital += bank;
        }
        winners[winners.size() - 1].Kapital += bank;
        cout << winners[winners.size() - 1].Name << " are WIN!!!" << endl;
    }
    string s;
    cin >> s;
    bank = 0;
}
void TEST() {
    Player.push_back(PlayerStruct());
    Player[0].Name = "Zahar";
    Player[0].Kapital = 1000;
    Player[0].Fold = false;
    CreateDeck(Deck);
    Distribution();
    Combination C(Player[0].Hand, Table);
    /*Table[0].UnLock();
    Table[1].UnLock();
    Table[2].UnLock();
    Table[3].UnLock();
    Draw(3, 0);
    cout << endl << C.name() << endl;
    string a;
    cin >> a;*/
    if (C.power() == 8) {
        stflashCount++;
    }
    if (C.power() == 7) {
        careCount++;
    }
    if (C.power() == 6) {
        fhCount++;
    }
    if (C.power() == 5) {
        stCount++;
    }
    if (C.power() == 4) {
        flCount++;
    }
    if (C.power() == 3) {
        setCount++;
    }
    if (C.power() == 2) {
        dpCount++;
    }
    if (C.power() == 1) {
        pareCount++;
    }
    if (C.power() == 0) {
        noCount++;
    }
    cout << testCount << endl;
    Table.clear();
    Player.clear();
    testCount++;
    if (testCount == 2000) {
         noCount /= 20;
         pareCount /= 20;
         dpCount /= 20;
         setCount /= 20;
         flCount /= 20;
         stCount /= 20;
         fhCount /= 20;
         careCount /= 20;
         stflashCount /= 20;
         cout << endl << "Nothing: " << noCount << "%" << endl;
         cout << "Pare: " << pareCount << "%" << endl;
         cout << "Double pare: " << dpCount << "%" << endl;
         cout << "Set: " << setCount << "%" << endl;
         cout << "Flash: " << flCount << "%" << endl;
         cout << "Street: " << stCount << "%" << endl;
         cout << "Full House: " << fhCount << "%" << endl;
         cout << "Care: " << careCount << "%" << endl;
         cout << "StreetFlash: " << stflashCount << "%" << endl;
         return;
    }
    TEST();
}
int main() {
    //TEST();
    StartGame();
    do { 
    CreateDeck(Deck);
    Distribution();
    isFold = 0;
      for (int stage = 0, index; stage < 4; stage++){
          index = 0;
          SetupRound(stage);                               
          while (isCALL + isFold < (int)(Player.size())){                                                    
              do {
                  if (Player[index % Player.size()].Fold) {                  //add 11:30
                                      // надо завести переменную countOfFold - сколько фолданули и isCALL = isCALL-countOfFold
                      break;
                  }
                  Draw(index % Player.size());
              } while (Logic(index % Player.size()));
              index++;
          }
      }
      CheckWinner();
    } while (Continue());
 
   
    cout << endl << "Thank you! See u later:)";       //END
 }
 
 
//1. Доработать FOLD                                                 //+
//2. Какая - то фигня с переменной bank                              //+
//3. Проработать комбинации и сделать уже наконец Checkwin()
//4. После Checkwin() увеличение Winner.Kapital += bank
