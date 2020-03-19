#ifndef PROFILER_PROFILERBACKEND_H
#define PROFILER_PROFILERBACKEND_H

#ifdef __cplusplus
extern "C" {
#endif
#ifdef PROFILER_MPI
 void* profilerNewMPIA(char* name, int comm);
 void* profilerNewMPIB(char* name, int sort, int level, int comm);
#endif
void* profilerNewSerialB(char* name, int sort, int level);
void* profilerNewSerialA(char* name);
void profilerReset(void* profiler, char* name);
void profilerActive(void* profiler, int level, int stopPrint);
void profilerStart(void* profiler, char* name);
void profilerDeclare(void* profiler, char* name);
void profilerStop(void* profiler, char* name, long operations=0);
char* profilerStr(void* profiler, int verbosity, int cumulative, int precision);
void profilerStrSubroutine(void*profiler, char* result, int maxResult, int verbosity, int cumulative, int precision);
#ifdef __cplusplus
}
#endif


#endif //PROFILER_PROFILERBACKEND_H
