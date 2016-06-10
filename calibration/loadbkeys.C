int fBad[49152];

void loadbkeys() {
  for(int k=0; k!=49152; ++k) fBad[k] = 0; // reset array
  ifstream tmpfile( "badkeys.dat" );
  int n=0;
  for(int tmp;;) {
    tmpfile >> tmp;
    if(!tmpfile.good()) break;
    fBad[tmp] = -1; // set bad key
    ++n;
  }
  tmpfile.close();
  cout << "*** loadbkeys n=" << n << endl;
}
