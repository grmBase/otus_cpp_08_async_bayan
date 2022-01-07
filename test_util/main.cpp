//-----------------------------------------------------------------------------
#include <iostream>
//-----------------------------------------------------------------------------
#include "async.h"
//-----------------------------------------------------------------------------


int main(__attribute__((unused))int argc, __attribute__((unused))const char* argv[])
{
  std::cout << "in start of main()" << std::endl;

  std::size_t bulk = 5;
  auto handle1 = async::connect(bulk);
  if (handle1 == nullptr) {
    std::cerr << "error in connect()" << std::endl;
    return -33;
  }


  auto handle2 = async::connect(bulk);
  if (handle2 == nullptr) {
    std::cerr << "error in connect()" << std::endl;
    return -33;
  }

  async::receive(handle1, "1", 1);
  async::receive(handle2, "1\n", 2);
  async::receive(handle1, "\n2\n3\n4\n5\n6\n{\na\n", 15);
  async::receive(handle1, "b\nc\nd\n}\n89\n", 11);

  async::disconnect(handle1);
  async::disconnect(handle1);

  return 0;
}
//---------------------------------------------------------------------------

