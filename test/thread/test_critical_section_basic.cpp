#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#define SERVER_MODE
#include "critical_section.h"
using namespace std;

int
test_critical_section_basic (int argc, char **argv)
{
  int rv = csect_initialize_static_critical_sections ();
  assert (rv == 0);

  SYNC_CRITICAL_SECTION cs;

  rv = csect_initialize_critical_section (&cs, "test");

  assert (rv == 0);

  rv = csect_enter_critical_section (nullptr, &cs, 10);
  assert (rv == 0);

  thread t1
  {
    [&cs] ()
    {
      cout << "Try enter cs as reader:" << endl;
      int rv = csect_enter_critical_section_as_reader (nullptr, &cs, 10);
        cout << "rv after enter cs as reader: " << rv << endl;
        csect_exit_critical_section (nullptr, &cs);
    }
  };

  this_thread::sleep_for (chrono::seconds (5));
  cout << "exit cs" << endl;
  rv = csect_exit_critical_section (nullptr, &cs);
  assert (rv == 0);

  t1.join ();

  return 0;
}
