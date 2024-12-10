#include "/home/vlad/vsCode/2_course/programming/practice/practika3/headers/BURSE/burse.hpp"

int main() {
  Burse burse;
  string pat = "config.json";
  burse.readingConfiguration(pat);
  burse.loadingConfiguration();
}