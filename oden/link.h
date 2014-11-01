

// oden ƒŠƒ“ƒN—p

#ifndef ODEN_LIB_NAME
#  error " ***internal error*** ODEN_LIB_NAME is not defined. "
#endif

#ifdef _DEBUG
#  define ODEN_LIB_TYPE "d"
#else
#  define ODEN_LIB_TYPE ""
#endif

#ifdef _DLL
#  define ODEN_LIB_STDLINK "dll"
#else
#  define ODEN_LIB_STDLINK "st"
#endif

#ifndef ODEN_LIB_VERSION
#  error " ***internal error*** ODEN_LIB_VERSION is not defined. "
#endif

#if defined(ODEN_LIB_NAME)
#  pragma message("linking to lib file: oden-" ODEN_LIB_NAME "-" ODEN_LIB_STDLINK "-" ODEN_LIB_TYPE "-" ODEN_LIB_VERSION ".lib")
#  pragma comment(lib,"oden-" ODEN_LIB_NAME "-" ODEN_LIB_STDLINK "-" ODEN_LIB_TYPE "-" ODEN_LIB_VERSION ".lib")
#endif

