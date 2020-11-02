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
string addressChange(string & input, int idx, int offset, vector<Page> & PageTable, bool dec, bool clock, int & clockPointer);

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
  bool clock = false;
  int clockPointer = 0;
  bool cpFound = false;
  if(argc == 3)
    clock = true;
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
      if(cpFound == false && temp.V == 1){
        cpFound = true;
        clockPointer = counter;
      }
      counter++;
    }
  }
  //cout << PageTable.size() << endl;
  PageTable.pop_back();

  // OFFSET IS THE Trailing shit we don't care as much about
  int offset = log2(pageSize);
  // IDX = the portion of shit we want to look at to find our indicy
  //int idx = vaN - offset;
  // TRYING paM ?
  int idx = paM - offset;

  bool runny = true;
  string cont = "";
  cin.clear();
  cin.seekg(0);
  // cout << "Page Table before: " << endl;
  // for(int i = 0; i < PageTable.size(); i++){
  //   cout << PageTable[i].V << " " << PageTable[i].P << " " << PageTable[i].F << " " << PageTable[i].U  << endl;
  // }
  while(runny){
    string ins;
    bool dec = false;
    getline(cin, ins);
    if(cin.fail()){
      runny = false;
      break;
    }
    cout << "Input = " << ins << endl;
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
      string dd = addressChange(ins, idx, offset, PageTable, dec, clock, clockPointer);
      if(dd != "DISK" && dd != "SEGFAULT" && dd[0] != 'P'){
        dd = binToHex(stol(dd));
        dd = "0x" + dd;
      }
      else if(dd[0] == 'P'){
        string temp1 = "";
        for(int i = 9; i < dd.length(); i++)
          temp1 = temp1 + dd[i];
        //cout << temp1 << endl;
        temp1 = binToHex(stol(temp1));
        temp1 = "0x" + temp1;
        dd = "PAGEFAULT " + temp1;
      }
      cout << "DD as Hex: " << dd << endl;

      // PRINTING NEW STATE OF PAGE TABLE:
      // if(clock == true){
      //   for(int i = 0; i < PageTable.size(); i++){
      //     cout << PageTable[i].V << " " << PageTable[i].P << " " << PageTable[i].F << " " << PageTable[i].U  << endl;
      //   }
      //   cout << endl << endl;
      // }
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

string addressChange(string & input, int idx, int offset, vector<Page> & PageTable, bool dec, bool clock, int & clockPointer){
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
  // PRINTING THE BINARY TEST STRING
  string ind, off = "";
  //cout << "IDX: " << idx << endl;
  //cout << "OFFSET: " << offset << endl;
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
  string temp = "";
  if(PageTable[l].V == 0){
    if(PageTable[l].P == 0){
      temp = "SEGFAULT";
    }
    else if(PageTable[l].P > 0 && clock == false){
      temp = "DISK";
    }
    else if(PageTable[l].P > 0 && clock == true){
      // Check if recentely used, if U == 0 then has not been used recently
      // if U == 0, evict the current indicy  and fill with
      bool evicted = false;
      while(!evicted){
        //cout << "Testing pointer: " << clockPointer << endl;
        if(PageTable[clockPointer].U == 0 && PageTable[clockPointer].V == 1){
          // cout << "Found Page to evict at pointer: " << clockPointer << endl;
          // cout << "New index = " << l << endl;
          PageTable[clockPointer].V = 0;
          PageTable[l].F = PageTable[clockPointer].F;
          PageTable[l].frameNum = PageTable[clockPointer].frameNum;
          PageTable[l].U = 1;
          PageTable[l].V = 1;
          evicted = true;
          if(clockPointer < PageTable.size()-1){
            //PageTable[clockPointer].U = 0;
            clockPointer++;
          }
          else
            clockPointer = 0;

        }
        else{
          PageTable[clockPointer].U = 0;
          if(clockPointer < PageTable.size()-1){
            clockPointer++;
          }
          else
            clockPointer = 0;
        }
      } // end While
      temp = "PAGEFAULT";
      temp = temp + PageTable[l].frameNum + off;
    }
  }
  else{
    temp = PageTable[l].frameNum;
    temp = temp + off;
    PageTable[l].U = 1;
  }
  return temp;
}
