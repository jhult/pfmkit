//----------------------------------------------------------------------------
// Copyright 2012-2015 Joe Lowe
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
//----------------------------------------------------------------------------
// file name:  portability.c
// created:    2012.05.04
//----------------------------------------------------------------------------
#ifdef _MSC_VER
#ifndef _CRT_NONSTDC_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#pragma warning(disable: 4100) // unreferenced parameter
#pragma warning(disable: 4163) // not available as intrinsic
#pragma warning(disable: 4505) // unreferenced function
#ifdef __cplusplus
#pragma warning(disable: 4355) // this reference in base constructor list
#endif
#endif
#include <stdint.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <wchar.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501
#include <windows.h>
#define FD_T HANDLE
#define FD_INVALID INVALID_HANDLE_VALUE
#else
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#define FD_T int
#define FD_INVALID -1
#endif
#define containerof(s,m,pm) ((s*)((const char*)(pm)-offsetof(s,m)))
#define IGNORE_LEAK(e)
#ifndef ASSERT
#define ASSERT(e)
#endif
#ifndef C_ASSERT
#define C_ASSERT(e) typedef char __C_ASSERT__[(e)?1:-1]
#endif

#ifdef _WIN32

static wchar_t* sswdup( const wchar_t* src)
{
   return wcsdup( src);
}

static int sswcmpf( const wchar_t* a, const wchar_t* b)
{
   return _wcsicmp( a, b);
}

static HANDLE stdout_fd( void)
{
   return GetStdHandle( STD_OUTPUT_HANDLE);
}

static HANDLE stdin_fd( void)
{
   return GetStdHandle( STD_INPUT_HANDLE);
}

static int/*err*/ read_fd( HANDLE fd, void* data, size_t max_size, size_t* out_size)
{
   int err = 0;
   DWORD size;

   if (!ReadFile( fd, data, (unsigned)max_size, &size, 0/*overlapped*/))
   {
      size = 0;
      err = GetLastError();
   }

   *out_size = size;
   return err;
}

static void close_fd( HANDLE fd)
{
   if (fd != FD_INVALID)
   {
      CloseHandle( fd);
   }
}

static long last_pipe_instance = 0;

static int/*err*/ create_socket_( int/*bool*/ bidir, int/*bool*/ async, HANDLE* outHandle1, HANDLE* outHandle2)
{
   int err = 0;
   HANDLE handle1;
   HANDLE handle2 = INVALID_HANDLE_VALUE;
   int options1 = FILE_FLAG_FIRST_PIPE_INSTANCE;
   int options2 = 0;
   int access2 = GENERIC_WRITE;
   __int64 now;
   enum { maxPipeNameChars = 100 };
   wchar_t pipeName[ maxPipeNameChars];

   if (bidir)
   {
      options1 |= PIPE_ACCESS_DUPLEX;
      access2 |= GENERIC_READ;
   }
   else
   {
      options1 |= PIPE_ACCESS_INBOUND;
   }
   if (async)
   {
      options1 |= FILE_FLAG_OVERLAPPED;
      options2 |= FILE_FLAG_OVERLAPPED;
   }
      // CreatePipe creates handles that do not support overlapped IO and
      // are unidirectional. Use named pipe instead.
   GetSystemTimeAsFileTime( (FILETIME*)&now );
   wsprintfW( pipeName, L"\\\\.\\Pipe\\AnonymousPipe.%i.%p.%i.%I64i", GetCurrentProcessId(), &last_pipe_instance, InterlockedIncrement( &last_pipe_instance), now);
   handle1 = CreateNamedPipeW( pipeName, options1, PIPE_TYPE_BYTE|PIPE_READMODE_BYTE|PIPE_WAIT, 1/*pipeCount*/, 0/*outBufSize*/, 0/*inBufSize*/, 30*1000/*timeout*/, 0/*security*/);
   if (!handle1)
   {
      handle1 = INVALID_HANDLE_VALUE;
   }
   if (handle1 == INVALID_HANDLE_VALUE)
   {
      err = GetLastError();
   }
   if (!err)
   {
      handle2 = CreateFileW( pipeName, access2, 0/*shareMode*/, 0/*security*/, OPEN_EXISTING, options2, 0/*template*/);
      if (handle2 == INVALID_HANDLE_VALUE)
      {
         err = GetLastError();
         CloseHandle( handle1);
         handle1 = INVALID_HANDLE_VALUE;
      }
   }

   *outHandle1 = handle1;
   *outHandle2 = handle2;
   return err;
}

static int/*err*/ create_pipe( HANDLE* read, HANDLE* write)
{
   return create_socket_( 0/*bidir*/, 1/*async*/, read, write);
}

#else

static int sswcmpf( const wchar_t* a, const wchar_t* b)
{
   return wcscmp( a, b);
}

static int stdout_fd( void)
{
   return 1;
}

static int stdin_fd( void)
{
   return 0;
}

static int/*err*/ read_fd( int fd, void* data, size_t max_size, size_t* out_size)
{
   int err = 0;
   ptrdiff_t size;

   do
   {
      size = read( fd, data, (unsigned)max_size);
      if (size < 0)
      {
         err = errno;
         size = 0;
      }
   } while (err == EINTR);

   *out_size = (size_t)size;
   return err;
}

static void close_fd( int fd)
{
   if (fd != FD_INVALID)
   {
      close( fd);
   }
}

static int/*err*/ create_pipe( int* read, int* write)
{
   int err = 0;
   int fds[2];

      // pipe() sys call is not good enough on some platforms.
      // Instead use unidirectional domain socket.
   if (socketpair( AF_UNIX, SOCK_STREAM, 0, fds) != 0)
   {
      err = errno;
      fds[0] = -1;
      fds[1] = -1;
   }
   if (!err)
   {
         // Convert socket to uni-directional.
      shutdown( fds[0], SHUT_WR);
      shutdown( fds[1], SHUT_RD);
   }

   *read = fds[0];
   *write = fds[1];
   return err;
}

static wchar_t* sswdup( const wchar_t* src)
{
   wchar_t* dest = 0;

   if (src)
   {
      len = wcslen( src);
      dest = (wchar_t*)malloc( (len+1)*sizeof( dest[ 0]));
      if (dest)
      {
         memcpy( dest, src, (len+1)*sizeof( dest[ 0]));
      }
   }

   return dest;
}

static void setlocale_once(void)
{
   static int first = 0;
   if(!first)
   {
      first = 1;
      setlocale(LC_CTYPE,0);
   }
}

static wchar_t* sswdupa( const char* src)
{
   wchar_t* dest = 0;
   size_t len;

   setlocale_once();
   len = mbstowcs( 0/*dest*/, src, INT_MAX);
   if (len < INT_MAX)
   {
      dest = (wchar_t*)malloc( (len+1)*sizeof( dest[ 0]));
   }
   if (dest)
   {
      mbstowcs( dest, src, len);
      dest[ len] = 0;
   }

   return dest;
}

#ifdef __cplusplus
extern "C"
#endif
int wmain( int argc, const wchar_t*const* argv);

#ifdef __cplusplus
extern "C"
#endif
int main( int argc, const char*const* argav)
{
   int res = -1;
   const wchar_t** argwv;
   int i;
   int err_count = 0;

   argwv = (const wchar_t**)malloc( (argc+1)*sizeof( argwv[ 0]));
   IGNORE_LEAK( argwv);
   if (argwv)
   {
      for (i = 0; i < argc; i ++)
      {
         argwv[i] = sswdupa( argav[ i]);
         IGNORE_LEAK( argwv[ i]);
         err_count += !argwv[ i];
      }
      argwv[ i] = 0;
      if (!err_count)
      {
         res = wmain( argc, argwv);
      }
   }

   return res;
}

#endif
