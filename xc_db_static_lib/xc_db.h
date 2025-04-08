
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the XC_DB_dll_exportS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// XC_DB_DLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef XC_DB_dll_exportS
#define XC_DB_DLL_API __declspec ( dllexport )
#else
#define XC_DB_DLL_API __declspec ( dllimport )
#endif

// This class is exported from the xc_db_dll.dll
class XC_DB_DLL_API CXc_db_dll {
public:
	CXc_db_dll(void);
	// TODO: add your methods here.
};

extern XC_DB_DLL_API int nXc_db_dll;

XC_DB_DLL_API int fnXc_db_dll(void);

