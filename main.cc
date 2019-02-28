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
  vector<string> tags;
  slide(){}

  slide(image &a): layout('H'), img1(a.id), tags(a.tags){}

  slide(image &a, image &b): layout('V'), img1(a.id), img2(b.id){
    set<string> tmp;
    for(string it: a.tags){
      tmp.insert(it);
    }
    for(string it: b.tags){
      tmp.insert(it);
    }
    for(string it: tmp){
      tags.emplace_back(it);
    }
  }
};

int calculate_score(slide &a, slide &b) {
  int intersect = 0, a_size = a.tags.size(), b_size = b.tags.size();
  for(int i = 0; i < a_size; i++){
    for(int j = 0; j < b_size; j++){
      if(a.tags[i] == b.tags[j]) {
        intersect++;
        break;
      }
    }
  }

  return min(intersect, min(a_size - intersect, b_size - intersect));
}

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
  int n;
  vector<image> images;
  cout << "reading from " << input_file << endl;
  fin >> n;
  set<string> all_tags;
  for(int i = 0; i < n; i++) {
    char layout;
    int num_tags;
    vector<string> tags;
    fin >> layout >> num_tags;
    for(int j = 0; j < num_tags; j++){
      string tmp;
      fin >> tmp;
      tags.push_back(tmp);
      all_tags.insert(tmp);
    }
    images.emplace_back(layout, i, tags);
  }
  cout << "read complete, total number of tags: " << all_tags.size() << endl;

  // pre-processing
  vector<int> horizontal_imgs;
  vector<int> vertical_imgs;
  cout << "getting horizontal and vertical images" << endl;
  for(int i = 0; i < n; i++) {
    image it = images[i];
    if(images[i].layout == 'H'){
      horizontal_imgs.push_back(i);
    } else {
      vertical_imgs.push_back(i);
    }
  }
  cout << "finished pre-processing " << horizontal_imgs.size() << " " << vertical_imgs.size() << endl; 
  int num_horizontal = horizontal_imgs.size(), num_vertical = vertical_imgs.size();
  int max_score = 0;
  vector<slide> optimal;
  set<int> all_n;
  for(int i = 0; i < n; i++) {
    all_n.insert(i);
  }
  // case when horizontal starts
  for(int i = 0; i < num_horizontal; i++){
    cout << "Currently " << i << " out of " << num_horizontal << " Done" << endl;
    set<int> remaining(all_n);
    int score = 0, counter = n-1;
    slide ptr(images[horizontal_imgs[i]]);
    vector<slide> current;
    current.push_back(ptr);
next_step:    
    while(counter > 0) {
      cout << counter << " left";
      int score_one_step = 0;
      slide next_slide;
      // iterate through remaining possible slides,
      // find the best match
      
      for(int j = 0; j < num_horizontal; j++){
        if(remaining.find(horizontal_imgs[j]) == remaining.end()) continue;
        //cout << "looking at horizontal image " << horizontal_imgs[j] << endl;
        slide next(images[horizontal_imgs[j]]);
        int tmp = calculate_score(ptr, next);
        if(tmp > score_one_step) {
          score_one_step = tmp;
          next_slide = next;
        }
      }
      for(int j = 0; j < num_vertical - 1; j++) {
        if(remaining.find(vertical_imgs[j]) == remaining.end()) continue;
        for(int k = j+1; k < num_vertical; k++) {
          if(remaining.find(vertical_imgs[k]) == remaining.end()) continue;
          //cout << "looking at vertical images " << vertical_imgs[j] << " and " << vertical_imgs[k] << endl;
          slide next(images[vertical_imgs[j]], images[vertical_imgs[k]]);
          int tmp = calculate_score(ptr, next);
          if(tmp > score_one_step) {
            score_one_step = tmp;
            next_slide = next;
          }
        }
      }
      current.push_back(next_slide);
      counter --;
      remaining.erase(next_slide.img1);
      if(next_slide.layout == 'V'){
        counter--;
        remaining.erase(next_slide.img2);
      }
      
      score += score_one_step;
      cout << "\r";
    }
    if(score > max_score) {
      max_score = score;
      optimal = current;
    }
  }
  cout << max_score << endl;
  for(int i = 0; i < optimal.size(); i++) {
    cout << optimal[i].img1;
    if(optimal[i].layout == 'V') {
      cout << " " << optimal[i].img2;
    }
    cout << endl;
  }

  return 0;
}