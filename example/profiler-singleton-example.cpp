#include <molpro/Profiler.h>
#ifdef HAVE_MPI_H
#include "mpi.h"
#endif
#include <chrono>
#include <iostream>
#include <thread>
using molpro::Profiler;

void sleep_milliseconds(const std::string &name, int repeats) {
  auto p = Profiler::single()->push(name);
  for (size_t i = 0; i < repeats; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds{1});
  }
  p += repeats;
}

void run() {
  sleep_milliseconds("fast", 200);
  sleep_milliseconds("medium", 300);
  sleep_milliseconds("slow", 500);
  Profiler::single()->stop();
}

int main(int argc, char *argv[]) {
#ifdef HAVE_MPI_H
  MPI_Init(&argc, &argv);
#endif
  auto p1 = Profiler::single("Singleton Example: job 1");
  run();
  auto p2 = Profiler::single("Singleton Example: job 2");
  run();
  std::cout << *p1;
  std::cout << *p2;
  p1.reset();
  p2.reset();
#ifdef HAVE_MPI_H
  MPI_Finalize();
#endif
  return 0;
}
