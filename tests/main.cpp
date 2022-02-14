#include <IntermediateDataStructure.h>
#include <iostream>
#include <string>
#include <vector>

void testTextLableTrack();
void testWordLableTrack();
void printLabels(std::vector<TextTrackEntry> label);


int main(int argc, char const *argv[]) {
  testTextLableTrack();
  testWordLableTrack();

  return 0;
}


void testTextLableTrack() {
  TextLabelTrack tt("Phonemes");

  std::cout << "Name: " << tt.name << "\n";

  tt.insert(0.1, "b");
  tt.insert(0.9, "d");
  //Test middle insert
  tt.insert(0.5, "c");
  //test beggining insert
  tt.insert(0.05, "a");
  //test end insert
  tt.insert(0.95, "e");
  //Expected: a b c d e
  printLabels(tt.getTextLabels());

  tt.remove(0);
  tt.remove(3);
  tt.remove(1);
  //Expected: b d
  printLabels(tt.getTextLabels());

  tt.insert(0.05, "a");
  tt.insert(0.5, "c");
  tt.insert(0.95, "e");
  //Expected: a b c d e
  printLabels(tt.getTextLabels());

  tt.move(0, 1.1);
  tt.move(3, 0.05);
  //Expected: e b c d a
  printLabels(tt.getTextLabels());

  tt.set(0, "a");
  tt.set(4, "e");
  //Expected: a b c d e
  printLabels(tt.getTextLabels());


}

void testWordLableTrack() {
  WordLabelTrack tt("Phonemes");

  std::cout << "Name: " << tt.name << "\n";

  tt.insert(0.1, "b");
  tt.insert(0.9, "d");
  //Test middle insert
  tt.insert(0.5, "c");
  //test beggining insert
  tt.insert(0.05, "a");
  //test end insert
  tt.insert(0.95, "e");
  //Expected: a b c d e
  printLabels(tt.getTextLabels());

  tt.remove(0);
  tt.remove(3);
  tt.remove(1);
  //Expected: b d
  printLabels(tt.getTextLabels());

  tt.insert(0.05, "a");
  tt.insert(0.5, "c");
  tt.insert(0.95, "e");
  //Expected: a b c d e
  printLabels(tt.getTextLabels());

  tt.move(0, 1.1);
  tt.move(3, 0.05);
  //Expected: e b c d a
  printLabels(tt.getTextLabels());

  tt.set(0, "a");
  tt.set(4, "e");
  //Expected: a b c d e
  printLabels(tt.getTextLabels());

  tt.insert(1.2, " f");
  tt.insert(1.3, "g ");
  tt.insert(1.4, " h ");
  tt.insert(1.5, "i i");
  //Expected: a b c d e
  printLabels(tt.getTextLabels());


}

void printLabels(std::vector<TextTrackEntry> label) {
  std::cout << "Printout:\n";
  for (int i = 0; i < label.size(); ++i) {
    std::cout << "Time: " << label[i].time << "\t\tPhoneme: " << label[i].data << "\n";
  }
  std::cout << "\n";
}
