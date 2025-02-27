#ifdef NWT_WINDOWS
    #ifdef NWT_EXPORTS
        //#define NWT_API __declspec(dllexport)
        #define NWT_API
#else
        //#define NWT_API __declspec(dllimport)
        #define NWT_API
#endif
#else
    #define NWT_API
#endif