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
// file name:  mounter.c
// created:    2012.04.05
//    PFM file mounter sample in C.
//----------------------------------------------------------------------------

#include "../common/portability.c"

#include "pfmapi.h"

int wmain( int argc, const wchar_t*const* argv)
{
   int err = 0;
   int argi;
   size_t len;
   const wchar_t* arg;
   int/*bool*/ showHelp = 0/*false*/;
   int/*bool*/ showBanner = (argc == 1);
   char data[1];
   PfmApi* pfm = 0;
   PfmFileMount* mount = 0;
   PfmFileMountCreateParams fmp;

   PfmFileMountCreateParams_Init( &fmp);
   fmp.fileMountFlags |= pfmFileMountFlagInProcess;

   argi = 1;
   while (argi < argc)
   {
      arg = argv[argi++];
      if (arg[0] == '-' && wcslen( arg) == 2)
      {
         switch (arg[1])
         {
         case 'h':
         case 'H':
         case '?':
            showHelp = 1/*true*/;
            break;
         case 'p':
         case 'P':
            if (argi < argc)
            {
               arg = argv[argi++];
               fmp.password = arg;
            }
            break;
         case 'm':
         case 'M':
            if (argi < argc)
            {
               arg = argv[argi++];
               len = wcslen(arg);
               if (len < 1 || len > 2 || (len == 2 && arg[1] != ':'))
               {
                  printf( "ERROR: Invalid drive letter \"%ls\".\n", arg);

               }
               else
               {
                  fmp.driveLetter = arg[0];
               }
            }
            break;
         default:
            printf( "WARNING: Ignoring unknown switch \"%ls\".\n", arg);
         }
      }
      else if (!fmp.mountFileName)
      {
         fmp.mountFileName = arg;
      }
      else
      {
         printf( "WARNING: Ignoring extra argument \"%ls\".\n", arg);
      }
   }
   if (showHelp)
   {
      printf( "syntax: mounter [<switch> ...] <container file>\n"
         "switches:\n"
         "  -h  Show this help information.\n"
         "  -m <drive letter>  Mount at specified drive letter.\n"
         "  -p <password>  Answer for password prompt.\n");
      err = -1;
   }
   else if (showBanner)
   {
      printf( "Pismo Mounter Sample\n"
         "Copyright 2012 Joe Lowe\n"
         "For help run: mounter -h\n");
      err = -1;
   }
   else if (!fmp.mountFileName)
   {
      printf( "ERROR: Must supply container file name.\n");
      err = -1;
   }
   else
   {
      err = PfmInstallCheck();
      switch (err)
      {
      case pfmInstInstalled:
         err = 0;
         break;
      case pfmInstOldBuild:
         printf( "WARNING: PFM system extension is old, consider updating.\n");
         err = 0;
         break;
      default:
      case pfmInstOldVersion:
      case pfmInstNotInstalled:
         if (err == pfmInstOldVersion)
         {
            printf( "NOTE: PFM system extension is too old, update required.\n");
         }
         else
         {
            printf( "NOTE: PFM system extension is not installed.\n");
         }
            // Auto-installation is left as an exercise for the
            // implementor.
         err = -1;
         // if (!IsUserPriviliged())
         // {
         //    printf( "ERROR: PFM system extension must be installed. Rerun command as administrator.\n");
         // }
         // else
         // {
         //    printf( "NOTE: Installing PFM system extension.\n");
         //    if (RunCmd( "pfminst install") == 0)
         //    {
         //       err = 0;
         //    }
         // }
         if (err)
         {
            printf( "ERROR: PFM system extension installation failed.\n");
         }
         break;
      }
   }

   if(!err)
   {
      err = PfmApiFactory( &pfm);
      if (err)
      {
         printf( "ERROR: %i Unable to open PFM Api.\n", err);
      }
   }
   if (!err)
   {
      err = (*pfm)->FileMountFactory( pfm, &mount);
      if (!err)
      {
         err = (*mount)->Start( mount, &fmp);
      }
      if (err)
      {
         printf( "ERROR: %i Unable to create mount.\n", err);
      }
   }
   if (!err)
   {
      err = (*mount)->WaitReady( mount);
   }
   if (!err)
   {
      printf( "Press 'q' to exit.\n");
      while ((err = read_fd( stdin_fd(), &data, 1, &len)) == 0 && len &&
         data[0] != 'q' && data[0] != 'Q')
      {
      }
      (*mount)->Cancel( mount);
   }

   if (mount)
   {
      if (err)
      {
         (*mount)->Cancel( mount);
      }
      (*mount)->Detach( mount);
      (*mount)->Release( mount);
   }
   if (pfm)
   {
      (*pfm)->Release( pfm);
   }
   PfmApiUnload();
   return err;
}
