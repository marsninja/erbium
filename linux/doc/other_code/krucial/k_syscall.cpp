#include "k_vm.h"

namespace k { namespace krucial {
//Virtual Machine (Syscall Manager)-----------------------------------------
void k_sys_call(const instruction &i, k_memory &mem)
{
  switch(i.prm[0])
  {
  case 0:
    switch(i.prm[1])
    {
    case 0: cout << mem.integer(i.prm[2]); break;
    case 1: cout << mem.count(i.prm[2]); break;
    case 2: cout << mem.number(i.prm[2]); break;
    case 3: cout << mem.c_string(i.prm[2]);  break;
    } break;
  case 1:
    switch(i.prm[1])
    {
    case 0: cin >> mem.integer(i.prm[2]); break;
    case 1: cin >> mem.count(i.prm[2]); break;
    case 2: cin >> mem.number(i.prm[2]); break;
    case 3: cin >> mem.c_string(i.prm[2]); break;
    } break;
  }
}
//--------------------------------------------------------------------------
}}
