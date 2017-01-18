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
// file name:  mounter.cs
// created:    2012.04.05
//    PFM file mounter sample in C#.
//----------------------------------------------------------------------------
using System;
using System.Security;

class Main_
{

static int Main(string[] args)
{
   int err = 0;
   int argi;
   string arg = null;
   bool showHelp = false;
   bool showBanner = true;
   Pfm.Api pfm = null;
   Pfm.FileMount mount = null;
   Pfm.FileMountCreateParams fmp = new Pfm.FileMountCreateParams();

   fmp.fileMountFlags |= Pfm.fileMountFlagInProcess;

   argi = 0;
   while(argi < args.Length)
   {
      arg = args[argi++];
      showBanner = false;
      if(arg.Length == 2 && arg[0] == '-')
      {
         switch(arg[1])
         {
         case 'h':
         case 'H':
         case '?':
            showHelp = true;
            break;
         case 'm':
         case 'M':
            if(argi < args.Length)
            {
               arg = args[argi++];
               if(arg.Length < 1 || arg.Length > 2 || (arg.Length == 2 && arg[1] != ':'))
               {
                  Console.Write("ERROR: Invalid drive letter \"{0}\".\n",arg);
                  err = -1;
               }
               else
               {
                  fmp.driveLetter = arg[0];
               }
            }
            break;
         case 'p':
         case 'P':
            if(argi < args.Length)
            {
               arg = args[argi++];
               fmp.password = arg;
            }
            break;
         default:
            Console.Write("WARNING: Ignoring unknown command line switch \"{0}\".\n",arg);
            break;
         }
      }
      else if(fmp.mountFileName == null)
      {
         fmp.mountFileName = arg;
      }
      else
      {
         Console.Write("WARNING: Ignoring extra command line argument \"{0}\".\n",arg);
      }
   }
   if(showHelp)
   {
      Console.Write("syntax: mounter [<switch> ...] <container file>\n"+
         "switches:\n"+
         "  -h  Show this help information.\n"+
         "  -m <drive letter>  Mount at specified drive letter.\n"+
         "  -p <password>  Answer for password prompt.\n");
      err = -1;
   }
   else if(showBanner)
   {
      Console.Write("Pismo Mounter Sample\n"+
         "Copyright 2012-2015 Joe Lowe\n"+
         "For help run: mounter -h\n");
      err = -1;
   }
   else if(fmp.mountFileName == null)
   {
      Console.Write("ERROR: Must supply container file name.\n");
      err = -1;
   }
   else
   {
      err = Pfm.InstallCheck();
      if(err == Pfm.instInstalled)
      {
         err = 0;
      }
      else if (err == Pfm.instOldBuild)
      {
         Console.Write("WARNING: PFM system extension is old, consider updating.\n");
         err = 0;
      }
      else
      {
         if(err == Pfm.instOldVersion)
         {
            Console.Write("NOTE: PFM system extension is too old, update required.\n");
         }
         else // (err == Pfm.instNotInstalled)
         {
            Console.Write("NOTE: PFM system extension is not installed.\n");
         }
            // Auto-installation is left as an exercise for the
            // implementor.
         err = -1;
         // if(!IsUserPriviliged())
         // {
         //    Console.Write("ERROR: PFM system extension must be installed. Rerun command as administrator.\n");
         // }
         // else
         // {
         //    Console.Write("NOTE: Installing PFM system extension.\n");
         //    if(RunCmd("pfminst install") == 0)
         //    {
         //       err = 0;
         //    }
         // }
         if(err != 0)
         {
            Console.Write("ERROR: PFM system extension installation failed.\n");
         }
      }
   }

   if(err == 0)
   {
      err = Pfm.ApiFactory(out pfm);
      if(err != 0)
      {
         Console.Write("ERROR: {0} Unable to open PFM Api.\n",err);
      }
   }
   if(err == 0)
   {
      err = pfm.FileMountFactory(out mount);
      if(err == 0)
      {
         err = mount.Start(fmp);
      }
      if(err != 0)
      {
         Console.Write("ERROR: {0} Unable to create mount.\n",err);
      }
   }
   if(err == 0)
   {
      err = mount.WaitReady();
   }
   if(err == 0)
   {
      Console.Write("Press 'q' to exit.\n");
      do
      {
         arg = Console.ReadKey(false/*display*/).Key.ToString();
      } while(arg.Length != 0 && arg != "q" && arg != "Q");
      mount.Cancel();
   }

   if(mount != null)
   {
      if(err != 0)
      {
         mount.Cancel();
      }
      mount.Detach();
      mount.Dispose();
   }
   if(pfm != null)
   {
      pfm.Dispose();
   }
   return err;
}

};
