#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <limits>
#include <ios>
#include <fstream>

using namespace std;

struct Page{
  long int V, P, F, U;
  string frameNum;
};

string hexToBin(string & hex);
string binToHex(long int bin);
string decToBin(int dec);
int binToDec(long int bin);
string addressChange(string & input, int idx, int offset, vector<Page> & PageTable, bool dec);

int main(int argc, char *argv[]){
  // ********************* ME TESTING THE CONVERSION FUNCTIONS **********
  // string b = "05";
  // cout << "Origional hex: " << b << endl;
  // b = hexToBin(b);
  // cout << "To Bin: " << b << endl;
  // cout << "Back To Hex: " << binToHex(stol(b)) << endl;
  // cout << decToBin(325) << endl;
  // *********************************************************************

  // Setting up to read the page file:
  // vaN = virtual Address size
  // paM = physical address size
  // pageSize = pageSize;
  fstream fifi;
  fifi.open(argv[1]);
  int vaN, paM, pageSize;
  int counter = 0;
  while(counter < 3){
    if(counter == 0){
      fifi >> vaN;
      counter++;
    }
    else if(counter == 1){
      fifi >> paM;
      counter++;
    }
    else if(counter == 2){
      fifi >> pageSize;
      counter++;
    }
  }
  bool run = true;
  counter = 0;
  vector<Page> PageTable;
  while(run){
    string t = "";
    getline(fifi, t);
    if(fifi.eof()){
      cout << "Finished building page table" <<endl;
      fifi.close();
      run = false;
    }
    else{
      struct Page temp;
      fifi >> temp.V;
      fifi >> temp.P;
      fifi >> temp.F;
      fifi >> temp.U;
      temp.frameNum = decToBin(temp.F);
      PageTable.push_back(temp);
      counter++;
    }
  }

  // for(int i = 0; i  < PageTable.size()-1; i++){
  //   cout << PageTable[i].V << " " << PageTable[i].P << " " << PageTable[i].F << " " << PageTable[i].U << " "<< endl;
  // }
  // OFFSET IS THE Trailing shit we don't care as much about
  int offset = log2(pageSize);
  //cout << "OffSet: " << offset << endl;
  // IDX = the portion of shit we want to look at to find our indicy
  int idx = vaN - offset;
//  cout << "idx: " << idx << endl;

  bool runny = true;
  string cont = "";
  cin.clear();
  cin.seekg(0);
  while(runny){
    string ins;
    bool dec = false;
    getline(cin, ins);
    if(ins[0] == '0' && ins[1] == 'x'){
      string ins2 = "";
      dec = false;
      for(int i = 0; i < ins.length(); i++){
        if(i == 0 || i == 1);
          //skip
        else
          ins2 += ins[i];
      }
      ins = ins2;
    }
    else
      dec = true;
    if(ins != ""){
      cout << "Input = " << ins << endl;
      string dd = addressChange(ins, idx, offset, PageTable, dec);
      if(dd != "DISK" && dd != "SEGFAULT"){
        dd = binToHex(stol(dd));
        dd = "0x" + dd;
      }
      cout << "DD as Hex: " << dd << endl;
    }
    if(cin.fail()){
      runny = false;
    }
  }
  return 0;
}

string hexToBin(string & hex){
  string temp = "";
  int i = 0;
  while(hex[i]){
    switch (hex[i]) {
      case '0':
        temp += "0000";
        break;
      case '1':
        temp += "0001";
        break;
      case '2':
        temp += "0010";
        break;
      case '3':
        temp += "0011";
        break;
      case '4':
        temp += "0100";
        break;
      case '5':
        temp += "0101";
        break;
      case '6':
        temp += "0110";
        break;
      case '7':
        temp += "0111";
        break;
      case '8':
        temp += "1000";
        break;
      case '9':
        temp += "1001";
        break;
      case 'A':
      case 'a':
        temp += "1010";
        break;
      case 'B':
      case 'b':
        temp += "1011";
        break;
      case 'C':
      case 'c':
        temp += "1100";
        break;
      case 'D':
      case 'd':
        temp += "1101";
        break;
      case 'E':
      case 'e':
        temp += "1110";
        break;
      case 'F':
      case 'f':
        temp += "1111";
        break;
      default:
        break;
    } // END switch
    i++;
  }
  return temp;
}

string binToHex(long int bin){
  int i = 1, j = 0, rem, dec = 0;
  int hex[1000];
  string temp = "";
  while(bin > 0){
    rem = bin % 2;
    dec = dec + rem * i;
    i = i * 2;
    bin = bin / 10;
  }
  i = 0;
  while (dec != 0) {
   hex[i] = dec % 16;
   dec = dec / 16;
   i++;
  }
  for(int a = i-1; a >=0; a--){
    if(hex[a] > 9){
      temp += (hex[a] + 55);
    }
    else{
      char aChar = '0' + hex[a];
      temp.push_back(aChar);
    }
  }
  return temp;
}

string decToBin(int dec){
  string reee = "";
  int i;
  for(i = 0; dec > 0; i++){
    char aChar = '0' + (dec%2);
    reee.push_back(aChar);
    dec = dec / 2;
  }
  reverse(reee.begin(), reee.end());
  return reee;
}

int binToDec(long int bin){
  int num = bin;
  int dec_value = 0;
  int base = 1;
  int temp = num;
  while (temp) {
    int last_digit = temp % 10;
    temp = temp / 10;
    dec_value += last_digit * base;
    base = base * 2;
  }
  return dec_value;
}

string addressChange(string & input, int idx, int offset, vector<Page> & PageTable, bool dec){
  string test = input;
  if(!dec)
    test = hexToBin(input);
  else
    test = decToBin(stoi(test));
  if(test.length() < (offset+idx)){
    int dif = (offset+idx) - test.length();
    string tt = "";
    for(int j = 0; j < dif; j++){
      tt.push_back('0');
    }
    test = tt + test;
  }
  string ind, off = "";
  int co = 0;
  for(int z = 0; z < test.length(); z++){
    if(co < idx)
      ind.push_back(test[z]);
    else
      off.push_back(test[z]);
    co++;
  }
  char char_array[ind.length() + 1];
  strcpy(char_array, ind.c_str());
  int l = strtoull(char_array, NULL ,2);
  string temp = PageTable[l].frameNum;
  temp = temp + off;
  if(PageTable[l].V == 0){
    if(PageTable[l].F > 0){
      temp = "SEGFAULT";
      //cout << "PTable: " << PageTable[l].F << endl;
    }
    else if(PageTable[l].F == 0){
      temp = "DISK";

    }
  }
  return temp;
}
