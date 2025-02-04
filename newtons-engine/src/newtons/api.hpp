#ifdef NWT_WINDOWS
    #ifdef NWT_EXPORTS
        #define NWT_API __declspec(dllexport)
    #else
        #define NWT_API __declspec(dllimport)
    #endif
#else
    #define NWT_API
#endif