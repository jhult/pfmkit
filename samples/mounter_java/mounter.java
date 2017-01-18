//----------------------------------------------------------------------------
// Pismo Technic Inc. Copyright 2006-2017 Joe Lowe
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
// file name:  Mounter.java
// created:    2015.06.26
//    PFM file mounter sample in java.
//----------------------------------------------------------------------------
import java.io.IOException;
import com.pismotec.pfm184.Pfm;

class mounter {

public static void main( String[] args)
{
   int err = 0;
   int argi;
   String arg = null;
   boolean showHelp = false;
   boolean showBanner = true;
   Pfm.Api pfm = null;
   Pfm.FileMount mount = null;
   Pfm.FileMountCreateParams fmp = new Pfm.FileMountCreateParams();

   fmp.fileMountFlags |= Pfm.fileMountFlagInProcess;

   argi = 0;
   while (argi < args.length)
   {
      arg = args[argi++];
      showBanner = false;
      if (arg.length() == 2 && arg.charAt( 0) == '-')
      {
         switch (arg.charAt( 1))
         {
         case 'h':
         case 'H':
         case '?':
            showHelp = true;
            break;
         case 'm':
         case 'M':
            if (argi < args.length)
            {
               arg = args[argi++];
               if (arg.length() < 1 || arg.length() > 2 || (arg.length() == 2 && arg.charAt( 1) != ':'))
               {
                  System.out.println( "ERROR: Invalid drive letter \"" + arg + "\".\n");
                  err = -1;
               }
               else
               {
                  fmp.driveLetter = arg.charAt( 0);
               }
            }
            break;
         case 'p':
         case 'P':
            if (argi < args.length)
            {
               arg = args[argi++];
               fmp.password = arg;
            }
            break;
         default:
            System.out.println( "WARNING: Ignoring unknown command line switch \"" + arg + "\".\n");
            break;
         }
      }
      else if (fmp.mountFileName == null)
      {
         fmp.mountFileName = arg;
      }
      else
      {
         System.out.println( "WARNING: Ignoring extra command line argument \"" + arg + "\".\n");
      }
   }
   if (showHelp)
   {
      System.out.println( "syntax: mounter [<switch> ...] <container file>\n"+
         "switches:\n"+
         "  -h  Show this help information.\n"+
         "  -m <drive letter>  Mount at specified drive letter.\n"+
         "  -p <password>  Answer for password prompt.\n");
      err = -1;
   }
   else if (showBanner)
   {
      System.out.println( "Pismo Mounter Sample\n"+
         "Copyright 2012-2015 Joe Lowe\n"+
         "For help run: mounter -h\n");
      err = -1;
   }
   else if (fmp.mountFileName == null)
   {
      System.out.println( "ERROR: Must supply container file name.\n");
      err = -1;
   }
   else
   {
      err = Pfm.InstallCheck();
      if (err == Pfm.instInstalled)
      {
         err = 0;
      }
      else if (err == Pfm.instOldBuild)
      {
         System.out.println( "WARNING: PFM system extension is old, consider updating.\n");
         err = 0;
      }
      else
      {
         if (err == Pfm.instOldVersion)
         {
            System.out.println( "NOTE: PFM system extension is too old, update required.\n");
         }
         else // (err == Pfm.instNotInstalled)
         {
            System.out.println( "NOTE: PFM system extension is not installed.\n");
         }
            // Auto-installation is left as an exercise for the
            // implementor.
         err = -1;
         // if (!IsUserPriviliged())
         // {
         //    System.out.println( "ERROR: PFM system extension must be installed. Rerun command as administrator.\n");
         // }
         // else
         // {
         //    System.out.println( "NOTE: Installing PFM system extension.\n");
         //    if (RunCmd( "pfminst install") == 0)
         //    {
         //       err = 0;
         //    }
         // }
         if (err != 0)
         {
            System.out.println( "ERROR: PFM system extension installation failed.\n");
         }
      }
   }

   if (err == 0)
   {
      {
         Pfm.ApiBox box = new Pfm.ApiBox();
         err = Pfm.ApiFactory( box);
         pfm = box.val;
      }
      if (err != 0)
      {
         System.out.println( "ERROR: "+err+" Unable to open PFM Api.\n");
      }
   }
   if (err == 0)
   {
      {
         Pfm.FileMountBox box = new Pfm.FileMountBox();
         err = pfm.FileMountFactory( box);
         mount = box.val;
      }
      if (err == 0)
      {
         err = mount.Start( fmp);
      }
      if (err != 0)
      {
         System.out.println( "ERROR: "+err+" Unable to create mount.\n");
      }
   }
   if (err == 0)
   {
      err = mount.WaitReady();
   }
   if (err == 0)
   {
      System.out.println( "Press 'q'+enter to exit.\n");
      int c;
      do
      {
         c = 0;
         try { c = System.in.read(); } catch (IOException ex) { }
      } while (c != 'q' && c != 'Q');
      mount.Cancel();
   }

   if (mount != null)
   {
      if (err != 0)
      {
         mount.Cancel();
      }
      mount.Detach();
      try { mount.close(); } catch (Exception ex) { }
   }
   if (pfm != null)
   {
      try { pfm.close(); } catch (Exception ex) { }
   }
}

}
