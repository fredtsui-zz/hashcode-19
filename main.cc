#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <set>
using namespace std;

struct image {
  char layout; // 'H' / 'V'
  int id;
  vector<string> tags;
  image(char c, int id, vector<string> tags): layout(c), id(id), tags(tags){}
};

struct slide {
  char layout;
  int img1;
  int img2;
};

int main(int argc, char* argv[]) {
  string input_file = "a_example.txt";
  if(argc > 1) {
    input_file = argv[1];
  }
  ifstream fin;
  fin.open(input_file);
  if(!fin.is_open()){
    cerr << "no file named " << input_file << " found" << endl;
  }
  set all_tags
  int n;
  vector<image> images;
  fin >> n;
  for(int i = 0; i < n; i++) {
    char layout;
    int num_tags;
    vector<string> tags;
    fin >> layout >> num_tags;
    for(int j = 0; j < num_tags; j++){
      string tmp;
      fin >> tmp;
      tags.push_back(tmp);
    }
    images.emplace_back(layout, i, tags);
  }

  return 0;
}