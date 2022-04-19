#include <IntermediateDataStructure.h>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <complex.h>
#include <fftw3.h>

void testTextLableTrack();
void testWordLableTrack();
void testIntegerLableTrack();
void testAutoNumberLableTrack();
void testIntermediateDataStructure();
void testCiglet();
void printLabels(std::vector<TextTrackEntry> label);


int main(int argc, char const *argv[]) {
  std::cout << "TEXT LABEL TRACK TEST\n";
  testTextLableTrack();
  std::cout << "WORD LABEL TRACK TEST\n";
  testWordLableTrack();
  std::cout << "INTEGER LABEL TRACK TEST\n";
  testIntegerLableTrack();
  std::cout << "AUTO NUMBER LABEL TRACK TEST\n";
  testAutoNumberLableTrack();
  std::cout << "INTERMEDIATE DS TEST\n";
  testIntermediateDataStructure();

  // std::cout << "CIGLET TEST\n";
  // testCiglet();



  return 0;
}


void testTextLableTrack() {
  TextLabelTrack tt("Phonemes", false);

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
  WordLabelTrack tt("Phonemes", false);

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

  std::cout << "Delete:\n";
  tt.remove(0);
  tt.remove(3);
  tt.remove(1);
  //Expected: b d
  printLabels(tt.getTextLabels());

  std::cout << "Insert:\n";
  tt.insert(0.05, "a");
  tt.insert(0.5, "c");
  tt.insert(0.95, "e");
  //Expected: a b c d e
  printLabels(tt.getTextLabels());

  std::cout << "Move outer:\n";
  tt.move(0, 1.1);
  tt.move(3, 0.05);
  //Expected: e b c d a
  printLabels(tt.getTextLabels());

  std::cout << "Move inner:\n";
  tt.move(1, 0.6);
  //Expected: e c b d a
  printLabels(tt.getTextLabels());

  std::cout << "Move inner reverse:\n";
  tt.move(2, 0.1);
  //Expected: e b c d a
  printLabels(tt.getTextLabels());

  std::cout << "Set:\n";
  tt.set(0, "a");
  tt.set(4, "e");
  //Expected: a b c d e
  printLabels(tt.getTextLabels());

  std::cout << "Insert invalid:\n";
  tt.insert(1.2, " f");
  tt.insert(1.3, "g ");
  tt.insert(1.4, " h ");
  tt.insert(1.5, "i i");
  //Expected: a b c d e
  printLabels(tt.getTextLabels());
}

void testIntegerLableTrack() {
  IntegerLabelTrack tt("IdkWhat", false);

  std::cout << "Name: " << tt.name << "\n";

  tt.insert(0.1, "2");
  tt.insert(0.9, "4");
  //Test middle insert
  tt.insert(0.5, "3");
  //test beggining insert
  tt.insert(0.05, "1");
  //test end insert
  tt.insert(0.95, "5");
  //Expected: 1 2 3 4 5
  printLabels(tt.getTextLabels());

  std::cout << "Delete:\n";
  tt.remove(0);
  tt.remove(3);
  tt.remove(1);
  //Expected: 2 4
  printLabels(tt.getTextLabels());

  std::cout << "Insert:\n";
  tt.insert(0.05, "1");
  tt.insert(0.5, "3");
  tt.insert(0.95, "5");
  //Expected: 1 2 3 4 5
  printLabels(tt.getTextLabels());

  std::cout << "Move outer:\n";
  tt.move(0, 1.1);
  tt.move(3, 0.05);
  //Expected: 5 2 3 4 1
  printLabels(tt.getTextLabels());

  std::cout << "Move inner:\n";
  tt.move(1, 0.6);
  //Expected: 5 3 2 4 1
  printLabels(tt.getTextLabels());

  std::cout << "Move inner reverse:\n";
  tt.move(2, 0.1);
  //Expected: 5 2 3 4 1
  printLabels(tt.getTextLabels());

  std::cout << "Set:\n";
  tt.set(0, "1");
  tt.set(4, "5");
  //Expected: 1 2 3 4 5
  printLabels(tt.getTextLabels());

  std::cout << "Insert invalid:\n";
  tt.insert(1.2, " 6");
  tt.insert(1.3, "7 ");
  tt.insert(1.4, " 8 ");
  tt.insert(1.5, "9 9");
  tt.insert(1.6, "0-1");
  tt.insert(1.7, "1.0");
  tt.insert(1.8, "z");
  //Expected: a b c d e
  printLabels(tt.getTextLabels());

}

void testAutoNumberLableTrack() {
  AutoNumberLabelTrack tt("Syllables", false);


  std::cout << "Name: " << tt.name << "\n";

  tt.insert(0.1);
  std::cout << "Nabbme: \n";
  tt.insert(0.9);
  //Test middle insert
  tt.insert(0.5);
  //test beggining insert
  tt.insert(0.05);
  //test end insert
  tt.insert(0.95);
  //Expected: 1 2 3 4 5

  printLabels(tt.getTextLabels());

  std::cout << "Delete:\n";
  tt.remove(0);
  tt.remove(3);
  tt.remove(1);
  //Expected: 1 2
  printLabels(tt.getTextLabels());

  std::cout << "Insert:\n";
  tt.insert(0.05);
  tt.insert(0.5);
  tt.insert(0.95);
  //Expected: 1 2 3 4 5
  printLabels(tt.getTextLabels());

  std::cout << "Move outer:\n";
  tt.move(0, 1.1);
  tt.move(3, 0.05);
  //Expected: 1 2 3 4 5
  printLabels(tt.getTextLabels());

  std::cout << "Move inner:\n";
  tt.move(1, 0.6);
  //Expected: 1 2 3 4 5
  printLabels(tt.getTextLabels());

  std::cout << "Move inner reverse:\n";
  tt.move(2, 0.1);
  //Expected: 1 2 3 4 5
  printLabels(tt.getTextLabels());
}

void testIntermediateDataStructure() {
  IntermediateDataStructure data;
  data.initialize("ProjName", "D:\\School\\fall2021\\senior_project\\test.json");
  std::cout << "Name: " << data.projectName << "\tPath: " << data.projectPath << "\n";

  data.addAudioFile("C:\\Program Files (x86)\\UTAU\\o-ng-ngo-n-no-m-mo.wav");
  std::vector<double> *adata = data.getAudioData(0);
  std::cout << "Length: " << adata->size() << "\n";

  data.addTrack("Phonemes", WORD, false);
  data.addTrack("AAAAAAAAAAあ", WORD, true);
  data.addTrack("IdkWhat", INTEGER, true);
  data.addTrack("Syllables", AUTO_NUMBER, true);
  std::cout << "TrackName: " << data.tracks[0].name << "\n";

  LabelTrack *lt = data.getLabelTrack(0, "Phonemes");
  LabelTrack *lta = data.getLabelTrack(0, "AAAAAAAAAAあ");
  LabelTrack *lti = data.getLabelTrack(0, "IdkWhat");
  LabelTrack *ltaa = data.getLabelTrack(0, "Syllables");
  lta->set(0, "awoo");
  lti->set(0, "-200");
  ltaa->insert(0.5);


  lt->insert(0.1, "b");
  lt->insert(0.9, "d");
  lt->insert(0.5, "c");
  lt->insert(0.05, "a");
  lt->insert(0.95, "e");
  printLabels(lt->getTextLabels());

  lt = data.getLabelTrack(0, "Phonemes");
  printLabels(lt->getTextLabels());

  data.save();

  IntermediateDataStructure data_copy;
  data_copy.load("D:\\School\\fall2021\\senior_project\\test.json");
  data_copy.projectPath = "D:\\School\\fall2021\\senior_project\\test_copy.json";
  data_copy.save();

}

void printLabels(std::vector<TextTrackEntry> label) {
  std::cout << "Printout:\n";
  for (int i = 0; i < label.size(); ++i) {
    std::cout << "Time: " << label[i].time << "\t\tPhoneme: " << label[i].data << "\n";
  }
  std::cout << "\n";
}

void testCiglet() {
  IntermediateDataStructure data;
  data.initialize("ProjName", "D:\\School\\fall2021\\senior_project\\test.json");
  std::cout << "Name: " << data.projectName << "\tPath: " << data.projectPath << "\n";

  data.addAudioFile("C:\\Users\\digit\\Documents\\ust\\M.S.S. Planet\\test - Unnamed Track 2.wav");
  std::vector<double> *adata = data.getAudioData(0);

  int nhop = 256;
  int nfft = 1024;
  int hnfft = nfft/2;
  int nfrm = round(adata->size() / nhop);
  int nbins = nfft / 2 + 1;

  double *spec = (double*) fftw_malloc(sizeof(double)*nbins*nfrm);

  double *frame = (double*) fftw_malloc(sizeof(double)*nfft);
  fftw_complex *fft = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * nfft);

  fftw_plan plan = fftw_plan_dft_r2c_1d(nfft, frame, fft, FFTW_MEASURE);


  double mn = 100;
  double mx = -100;
  // Frame loop
  for(int i = 0; i < nfrm; ++i){
    // Copy data
    int center = i * nhop;
    for(int delta = -hnfft; delta < hnfft; ++delta) {
      int dc = delta + center;
      if(dc < 0 || dc > adata->size()){
        frame[delta+hnfft] = 0;
      }
      else {
        frame[delta+hnfft] = adata->data()[dc];
      }
    }

    fftw_execute(plan);


    for (int bin = 0; bin < nbins; ++bin) {
      int bstart = nbins*i;
      spec[bstart+bin] = log2(sqrt(fft[bin][0]*fft[bin][0] + fft[bin][1]*fft[bin][1]));


      if (!(isinf(spec[bstart+bin]) || isnan(spec[bstart+bin]))){
        if (spec[bstart+bin] > mx) {
          mx = spec[bstart+bin];
        }
        if (spec[bstart+bin] < mn) {
          mn = spec[bstart+bin];
        }
      }
    }
  }

  fftw_destroy_plan(plan);
  fftw_free(frame);
  fftw_free(fft);

  std::cout << "Max: " << mx << " Min: " << mn << "\n";

  mx -= mn;
  FILE *fp;
  fp = fopen ("D:\\School\\fall2021\\senior_project\\spect.pgm","wb");
  if (fp!=NULL)
  {
    fprintf(fp, "P5\n%d %d\n255\n", nfrm, nbins);
    unsigned char tmp = 0;
    for (int y = 0; y < nbins; y++){
      for (int x = 0; x < nfrm; x++) {
        double val = spec[x*nbins + y];
        if (isinf(val) || isnan(val)) {
          tmp = 0;
        }
        else {
          tmp = (int)(((val - mn)/mx) * 255);
        }

        fwrite(&tmp, 1, 1, fp);
      }
    }
    fclose (fp);
  }


  // std::cout << "Setup Vars\n";
  // w_stft(adata->data(), adata->size(), nhop, nfrm, 8, 1, NULL, NULL, Xm, NULL);
  // std::cout << "Ran stft\n";
  //
  // //min max
  // double mx = Xm[0][0];
  // double mn = Xm[0][0];
  // for (int x = 0; x < nfrm; x++) {
  //   for (int y = 0; y < nbins; y++){
  //     if(Xm[x][y] > mx) {
  //       mx = Xm[x][y];
  //     }
  //     if(Xm[x][y] < mn) {
  //       mn = Xm[x][y];
  //     }
  //   }
  // }
  // std::cout << "Min Maxed\n";
  //
  // std::cout << "Max: " << mx << "Min: " << mn << "\n";
  //
  //
  // mx -= mn;
  //normalize and write


}
