#include <iostream>
#include <math.h>
#include <string>

using namespace std;

string hexToBin(string & hex);
string binToHex(long int bin);

int main(){
  string b = "05";
  cout << "Origional hex: " << b << endl;
  b = hexToBin(b);
  cout << "To Bin: " << b << endl;
  cout << "Back To Hex: " << binToHex(stol(b)) << endl;
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
