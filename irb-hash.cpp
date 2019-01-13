#include <iostream>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <pthread.h>
#include <thread>
#include <fstream>
#include <ios>

using namespace std;

// Concatenates all strings in a vector
string catVec(vector<string> s) {
  stringstream ss;
  for (uint64_t i = 0; i < s.size(); i++) {
    ss << s[i];
  }
  return ss.str();
}

// Integer to hex string
string toHex(uint64_t i) {
  stringstream ss;
  ss << hex << i;
  return ss.str();
}

// Hex string to integer
uint64_t toInt(string s) {
  uint64_t i;
  stringstream ss;
  ss << hex << s;
  ss >> i;
  return i;
}

// Checks if value is in the used array
bool isUsed(vector<uint64_t> used, int64_t num) {
  if (num < 0) {
    return true;
  }
  bool out = false;
  for (uint64_t cur = 0; cur < used.size(); cur++) {
    if (used[cur] == (uint64_t)num) {
      out = true;
      break;
    }
  }
  return out;
}

// Returns largest byte in vector
uint64_t bigByte(vector<string> s) {
  cout << "Finding largest byte block..." << endl;
  string biggest = "0";
  for (uint64_t cur = 0; cur < s.size(); cur++) {
    if (toInt(s[cur]) > toInt(biggest)) {
      biggest = s[cur];
    }
  }
  return toInt(biggest);
}

// Generates recaman sequence iterated to a certain value
vector<uint64_t> genRec(uint64_t max) {
  cout << "Generating Recaman sequence..." << endl;
  vector<uint64_t> used = {};
  vector<uint64_t> seq = {};
  uint64_t cur = 0;
  for (uint64_t shift = 0; shift < max; shift++) {
    used.push_back(cur);
    if ((cur - shift) < 0) {
      cur += shift;
    }
    else if (isUsed(used, (cur - shift))) {
      cur += shift;
    }
    else {
      cur -= shift;
    }
    seq.push_back(cur);
  }
  return seq;
}

// Wraps thread function returning an iterated vector
vector<string> recVec(vector<string> inp) {
   vector<uint64_t> seq = genRec(bigByte(inp));
   vector<string> out = {};
   for (uint64_t cur = 0; cur < inp.size(); cur++) {
     out.push_back(toHex(seq[toInt(inp[cur])]));
   }
   return out;
}

// Pads a vector so all string are equal in length
vector<string> padVec(vector<string> inp) {
  cout << "Padding bytes..." << endl;
  string biggest = "";
  for (uint64_t cur = 0; cur < inp.size(); cur++) {
    if (inp[cur].length() > biggest.length()) {
      biggest = inp[cur];
    }
  }
  uint64_t maxChars = biggest.length();
  vector<string> out = {};
  for (uint64_t cur = 0; cur < inp.size(); cur++) {
    string pad = "";
    pad.insert(0, (maxChars - inp[cur].length()), '0');
    out.push_back(pad + inp[cur]);
  }
  return out;
}

// Splits string into vector of strings of given length
vector<string> toVec(string s, uint64_t blockSize) {
  cout << "Generating input vector... " << endl;
  vector<string> out = {};
    while (s.length() % blockSize != 0) {
      s.insert(0, 1, 0);
    }
  for (uint64_t cur = 0; cur < s.length(); cur += blockSize) {
    string cByte = "";
    for (uint64_t b = 0; b < blockSize; b++) {
      cByte += (toHex(int(s[(b + cur)])));
    }
    out.push_back(cByte);
  }
  return out;
}

// Returns the irb-hash of an input string with given block size
string irbh(string inp, uint64_t blockSize) {
  vector<string> vec = padVec(recVec(toVec(inp, blockSize)));
  cout << "Generating output vector..." << endl;
  string out = catVec(vec);
  out.erase(0, min(out.find_first_not_of('0'), out.size()-1));
  system("clear");
  return out;
}

// Primary function to hash user arguements
int main(int argc, char *argv[]) {
  uint64_t blockSize = stoi(argv[1]);
  string input = argv[2];
  string fileName = "";
  if (argc == 4) {
    fileName = argv[3];
  }
  system("clear");
  string out = irbh(input, blockSize);
  if (fileName == "") {
    cout << "IRB-Hash with " << blockSize << "-byte blocks:\n" << endl;
    cout << out << endl;
  }
  else {
    cout << "Writing hash to file..." << endl;
    ofstream outF(fileName, std::ios_base::binary | std::ios_base::out);
    for (uint64_t b = 0; b < out.length(); b+=2) {
      string cByte = "";
      cByte.insert(0, 1, out[(b+1)]);
      cByte.insert(0, 1, out[b]);
      outF << (char)toInt(cByte);
    }
    system("clear");
    cout << "IRB-Hash with " << blockSize << "-byte blocks written to " << fileName << "." << endl;
  }
}