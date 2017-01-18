//---------------------------------------------------------------------------
// Copyright 2005-2015 Joe Lowe
//
// Permission is granted to any person obtaining a copy of this Software,
// to deal in the Software without restriction, including the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and sell copies of
// the Software.
//
// The above copyright and permission notice must be left intact in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS WITHOUT WARRANTY.
//---------------------------------------------------------------------------
// filename:   ptfactory1.h
// created:    2005.12.02
//
// Portable and lightweight plugin/dll loader and class factory mechanism.
// Inline logic for Windows, uses weak linked helper library for Linux/Mac
//---------------------------------------------------------------------------
#ifndef PTFACTORY1_H
#define PTFACTORY1_H
#include "ptpublic.h"
PT_EXTERNC_START
#ifdef __cplusplus_cli
#pragma managed(push,off)
#endif

#ifdef KERNEL

#define PTFACTORY1_DECLARE(Prefix,keyName,valName) \
   PT_INLINE int/*error*/ PT_CCALL Prefix##GetInterface(const char* interfaceId,void* toInterface) { *(void**)(toInterface) = 0; return -1; } \
   PT_INLINE void PT_CCALL Prefix##Unload(void) { }

#else

typedef struct PtFactory1_Ref_ PtFactory1_Ref;
struct PtFactory1_Ref_
{
   void (PT_CCALL*RegisterAtExit)(void (PT_CCALL*atExit)(void));
   void (PT_CCALL*AtExit)(void);
   const wchar_t* keyName;
   const wchar_t* valName;
   int/*bool*/ atExitRegistered;
   void* library;
};

#ifdef _WIN32

#ifdef _INC_WINDOWS

#define PTFACTORY1_BYTE BYTE
#define PTFACTORY1_DWORD DWORD
#define PTFACTORY1_HKEY HKEY
#define PTFACTORY1_HMODULE HMODULE
#define PTFACTORY1_FARPROC FARPROC

#else

   // This header supports use in code that does not include
   // windows.h . If any of the following definitions cause
   // conflicts then include windows.h before this header.
#define PTFACTORY1_BYTE unsigned char
#define PTFACTORY1_DWORD unsigned long
#define PTFACTORY1_HKEY void*
#define PTFACTORY1_HMODULE void*
#define PTFACTORY1_BOOL long
typedef void (*PTFACTORY1_FARPROC)(void);
long _InterlockedExchange(long volatile*,long);
#pragma intrinsic(_InterlockedExchange)
#ifndef _M_IX86
void* _InterlockedExchangePointer(void*volatile*,void*);
#pragma intrinsic(_InterlockedExchangePointer)
#endif
PT_IMPORT PTFACTORY1_DWORD __stdcall GetLastError(void);
PT_IMPORT void* __stdcall LoadLibraryExW(const wchar_t*,void*,unsigned);
PT_IMPORT PTFACTORY1_BOOL __stdcall FreeLibrary(PTFACTORY1_HMODULE);
PT_IMPORT PTFACTORY1_FARPROC __stdcall GetProcAddress(PTFACTORY1_HMODULE,const char*);
PT_IMPORT long __stdcall RegOpenKeyExW(void*,const wchar_t*,unsigned,unsigned,void**);
PT_IMPORT long __stdcall RegQueryValueExW(void*,const wchar_t*,PTFACTORY1_DWORD*,PTFACTORY1_DWORD*,PTFACTORY1_BYTE*,PTFACTORY1_DWORD*);
PT_IMPORT long __stdcall RegCloseKey(void*);

#endif

#define PTFACTORY1_InterlockedExchange _InterlockedExchange
#ifdef _M_IX86
#define PTFACTORY1_InterlockedExchangePointer(p,v) (void*)_InterlockedExchange((long*)(p),(long)(v))
#else
#define PTFACTORY1_InterlockedExchangePointer _InterlockedExchangePointer
#endif

PT_INLINE int/*error*/ PT_CCALL PtFactory1_GetInterface(PtFactory1_Ref* factory,const char* iid,void* i)
{
   int error = 0;
   PTFACTORY1_DWORD valueSize;
   PTFACTORY1_DWORD valueType;
   long atExitRegistered;
   PTFACTORY1_HKEY key;
   void* library;
   const wchar_t* keyName;
   const wchar_t* libraryName = 0;
   enum { maxScratchChars = 300 };
   int (PT_CCALL*getFunc)(void**,const wchar_t*,const char*,void*);
   wchar_t scratch[maxScratchChars];
   *(void**)(i) = 0;
   atExitRegistered = PTFACTORY1_InterlockedExchange((long*)(&(factory->atExitRegistered)),1);
   library = PTFACTORY1_InterlockedExchangePointer(&(factory->library),0);
   if(!library)
   {
      libraryName = factory->valName;
      if((keyName = factory->keyName) != 0)
      {
            // Name of library is stored as value under key in HKCR
            // portion of registry.
         error = RegOpenKeyExW((PTFACTORY1_HKEY)(size_t)0x80000000/*HKEY_CLASSES_ROOT*/,keyName,0,0x00020019/*KEY_READ*/,&key);
         if(!error)
         {
            valueSize = (unsigned)(maxScratchChars*sizeof(scratch[0]));
            error = RegQueryValueExW(key,libraryName,0,&valueType,(PTFACTORY1_BYTE*)(scratch),&valueSize);
            scratch[maxScratchChars-1] = 0;
            if(!error && (valueType != 1/*REG_SZ*/ && valueType != 2/*REG_EXPAND_SZ*/))
            {
               error = 2/*ERROR_FILE_NOT_FOUND*/;
            }
            RegCloseKey(key);
            libraryName = scratch;
         }
      }
      if(!error)
      {
         library = (void*)(LoadLibraryExW(libraryName,0,8/*LOAD_WITH_ALTERED_SEARCH_PATH*/));
         if(!library) error = GetLastError();
      }
   }
   if(!error)
   {
      *(PTFACTORY1_FARPROC*)(&getFunc) = GetProcAddress((PTFACTORY1_HMODULE)(library),"FactoryMainGetInterface2");
      if(getFunc) error = getFunc(&library,libraryName,iid,i);
      else error = GetLastError();
   }
   if(library) library = PTFACTORY1_InterlockedExchangePointer(&(factory->library),library);
   if(library) FreeLibrary((PTFACTORY1_HMODULE)(library));
   if(!atExitRegistered)
   {
      factory->RegisterAtExit(factory->AtExit);
   }
   return error;
}

PT_INLINE void PT_CCALL PtFactory1_Unload(PtFactory1_Ref* factory)
{
   void* library;
   void (PT_CCALL*unloadFunc)(void**);
   library = PTFACTORY1_InterlockedExchangePointer(&(factory->library),0);
   if(library)
   {
      *(PTFACTORY1_FARPROC*)(&unloadFunc) = GetProcAddress((PTFACTORY1_HMODULE)(library),"FactoryMainUnload2");
         // Give library chance to grab self reference if it still has
         // active objects. Note there is a race condition during unload
         // where another thread could fault executing code on the way
         // out of a release function while we are unloading the library.
         // This can generally be avoided by limiting unloads to atexit
         // processing. The implementation can avoid the issue using
         // external helper code and delays, which is how COM avoids
         // issues.
      if(unloadFunc) unloadFunc(&library);
   }
   if(library) FreeLibrary((PTFACTORY1_HMODULE)(library));
}

#elif !defined(KERNEL)

#include <dlfcn.h>

#define PTFACTORY1_LIBRARYNAME "libptfactory1." PT_SHAREDEXTA
static void* PTFACTORY1_library = 0;
static union { int (*f)(PtFactory1_Ref*,const char*,void*); void* d; } PTFACTORY1_getInterface;
static union { void (*f)(PtFactory1_Ref*); void* d; } PTFACTORY1_unload;

PT_INLINE int/*error*/ PT_CCALL PtFactory1_GetInterface(PtFactory1_Ref* factory,const char* iid,void* i)
{
   if(!PTFACTORY1_getInterface.d)
   {
      if(!PTFACTORY1_library)
      {
         PTFACTORY1_library = dlopen(PTFACTORY1_LIBRARYNAME,RTLD_NOW);
      }
      if(PTFACTORY1_library)
      {
         PTFACTORY1_getInterface.d = dlsym(PTFACTORY1_library,"PtFactory1_GetInterface_");
      }
   }
   if(PTFACTORY1_getInterface.f)
   {
      return (PTFACTORY1_getInterface.f(factory,iid,i));
   }
   if(i) *(void**)i = 0;
   return -1;
}

PT_INLINE void PT_CCALL PtFactory1_Unload(PtFactory1_Ref* factory)
{
   if(!PTFACTORY1_unload.d && PTFACTORY1_library)
   {
      PTFACTORY1_unload.d = dlsym(PTFACTORY1_library,"PtFactory1_Unload_");
   }
   if(PTFACTORY1_unload.f)
   {
      PTFACTORY1_unload.f(factory);
   }
}

#endif

#define PTFACTORY1_DECLARE(Prefix,keyName,valName) \
   static void PT_CCALL Prefix##RegisterAtExit(void (PT_CCALL*func)(void)) { atexit(func); }; \
   static void PT_CCALL Prefix##AtExit(void); \
   static PtFactory1_Ref Prefix##_factory = { Prefix##RegisterAtExit,Prefix##AtExit,keyName,valName,0,0 }; \
   static void PT_CCALL Prefix##AtExit(void) { PtFactory1_Unload(&Prefix##_factory); } \
   PT_INLINE int/*error*/ PT_CCALL Prefix##GetInterface(const char* interfaceId,void* toInterface) { return PtFactory1_GetInterface(&Prefix##_factory,interfaceId,toInterface); } \
   PT_INLINE void PT_CCALL Prefix##Unload(void) { PtFactory1_Unload(&Prefix##_factory); }

#endif

#ifdef __cplusplus_cli
#pragma managed(pop)
#endif
PT_EXTERNC_END
#endif
