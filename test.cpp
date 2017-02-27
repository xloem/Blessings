#include <cstdio>
#include <string>
#include <chrono>
#include <thread>

#include "Symbol/SymbolUTF8.hpp"
#include "Symbol/Symbol.hpp"
#include "ANSILinux/TerminalIOANSILinux.hpp"
#include "Monitor.cpp"
#include "AdditionalStructs.hpp"

using namespace std;
using namespace blessings;

typedef SymbolUTF8 S;
typedef PropertyANSI P;
typedef TerminalIOANSILinux<S, S, P> TL;
typedef Monitor<S, S> M;
typedef MonitorCell<S> Cell;

int main() {
  TL term;

  term.Init();

  term.setNonCanonicalMode();
  term.setEchoInhibition();

  M monitor(&term, 2000);
  monitor.setResolution(50, 10);
  monitor.clearScreen();
  
  FILE * in = fopen("in.txt", "r");
  int s;
  char c;
  bool ex=false;

  for (int t=0;t<300;t++) {
    rewind(in);
    s = 0;
    
	  while (s<500) {
      for (int k=0;k<=t;k++) fscanf(in, "%c", &c);
      
      for (int k=0;k<50;k++) {
        if (c==*"\n") ex = true;
        if (ex) break; 
        
        monitor[s] = Cell(S(c), &P::defaultProperty);
        fscanf(in, "%c", &c);
        s++;
      }
      
      if (ex) break;
      
      while (c !=*"\n") fscanf(in, "%c", &c);
	  }
    
    if (ex) break;
    
	  monitor.draw(M::resChange::ignore);
	  this_thread::sleep_for(chrono::milliseconds(50));
	}
  
  fclose(in);
	
  return 0;
}
