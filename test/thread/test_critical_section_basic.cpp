#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include <cstring>
#define SERVER_MODE
#include "critical_section.h"
using namespace std;

extern int thread_initialize_entry (THREAD_ENTRY * entry_ptr);

//static const std::hash<std::thread::id> hasher;

int
test_critical_section_basic (int argc, char **argv)
{
  int rv = csect_initialize_static_critical_sections ();
  assert (rv == 0);

  cout << "Total number of threads: " << thread_num_total_threads () << endl;

  THREAD_ENTRY th;
  SYNC_CRITICAL_SECTION cs;

  thread_initialize_manager ();

  cout << "Total number of threads: " << thread_num_total_threads () << endl;

  thread_initialize_entry (&th);

  th.index = 0;

  thread_set_thread_entry_info (&th);
  th.tid = pthread_self ();	//hasher(this_thread::get_id());

  cout << "Total number of threads: " << thread_num_total_threads () << endl;

  rv = csect_initialize_critical_section (&cs, "test");

  assert (rv == 0);

  rv = csect_enter_critical_section (nullptr, &cs, 10);
  assert (rv == 0);


  thread t1
  {
    [&cs] ()
    {
      THREAD_ENTRY th;
        thread_initialize_entry (&th);
        th.index = 1;
        thread_set_thread_entry_info (&th);
        th.tid = pthread_self ();

        cout << "t1 Try enter cs as reader:" << endl;
      int rv = csect_enter_critical_section_as_reader (nullptr, &cs, 10);
        cout << "t1 rv after enter cs as reader: " << rv << endl;
        this_thread::sleep_for (chrono::seconds (2));
        cout << "t1 exits read cs" << endl;
        csect_exit_critical_section (nullptr, &cs);
    }
  };

  thread t2
  {
    [&cs] ()
    {
      THREAD_ENTRY th;
        thread_initialize_entry (&th);
        th.index = 2;
        thread_set_thread_entry_info (&th);
        th.tid = pthread_self ();

        cout << "t2 Try enter cs as reader:" << endl;
      int rv = csect_enter_critical_section_as_reader (nullptr, &cs, 10);
        cout << "t2 rv after enter cs as reader: " << rv << endl;
        this_thread::sleep_for (chrono::seconds (2));
        cout << "t2 exits read cs" << endl;
        csect_exit_critical_section (nullptr, &cs);
    }
  };


  cout << "main seep" << endl;
  this_thread::sleep_for (chrono::seconds (4));
  cout << "main exit cs" << endl;
  rv = csect_exit_critical_section (nullptr, &cs);

  rv = csect_enter_critical_section (nullptr, &cs, 10);
  assert (rv == 0);
  cout << "main reaquired cs" << endl;

  rv = csect_exit_critical_section (nullptr, &cs);
  assert (rv == 0);

  t1.join ();
  t2.join ();

  return 0;
}
