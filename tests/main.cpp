#include <IntermediateDataStructure.h>
#include <iostream>
#include <string>
#include <vector>

void testTextLableTrack();
void testWordLableTrack();
void testIntermediateDataStructure();
void printLabels(std::vector<TextTrackEntry> label);


int main(int argc, char const *argv[]) {
  std::cout << "TEXT LABEL TRACK TEST\n";
  testTextLableTrack();
  std::cout << "WORD LABEL TRACK TEST\n";
  testWordLableTrack();
  std::cout << "INTERMEDIATE DS TEST\n";
  testIntermediateDataStructure();


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

void testIntermediateDataStructure() {
  IntermediateDataStructure data;
  data.initialize("ProjName", "aPath");
  std::cout << "Name: " << data.projectName << "\tPath: " << data.projectPath << "\n";

  data.addAudioFile("C:\\Program Files (x86)\\UTAU\\o-ng-ngo-n-no-m-mo.wav");
  std::vector<double> *adata = data.getAudioData(0);
  std::cout << "Length: " << adata->size() << "\n";

  data.addTrack("Phonemes", Word);
  std::cout << "TrackName: " << data.tracks[0].name << "\n";

  LabelTrack *lt = data.getLabelTrack(0, "Phonemes");

  lt->insert(0.1, "b");
  lt->insert(0.9, "d");
  lt->insert(0.5, "c");
  lt->insert(0.05, "a");
  lt->insert(0.95, "e");
  printLabels(lt->getTextLabels());

  lt = data.getLabelTrack(0, "Phonemes");
  printLabels(lt->getTextLabels());


}

void printLabels(std::vector<TextTrackEntry> label) {
  std::cout << "Printout:\n";
  for (int i = 0; i < label.size(); ++i) {
    std::cout << "Time: " << label[i].time << "\t\tPhoneme: " << label[i].data << "\n";
  }
  std::cout << "\n";
}
