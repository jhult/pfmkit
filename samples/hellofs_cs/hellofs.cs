//----------------------------------------------------------------------------
// Copyright 2008-2015 Joe Lowe
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
// file name:  hellofs.cs
// created:    2008.04.25
//    Hello World file system sample application.
//----------------------------------------------------------------------------
using System;
using System.Text;
using System.IO;

class Volume: Pfm.FormatterDispatch
{
   public Pfm.Marshaller marshaller;
   public long folderOpenId = 0;
   public long fileOpenId = 0;
   static string helloFileName = "readme.txt";
   static string helloFileData = "Hello World.\r\n";
   static int helloDataSize = helloFileData.Length;
   static long helloRootId = 2;
   static long helloFileId = 3;

   public void Dispose()
   {
   }

   public void Open( Pfm.MarshallerOpenOp op)
   {
      string[] nameParts = op.NameParts();
      int createFileType = op.CreateFileType();
      long newExistingOpenId = op.NewExistingOpenId();
      int perr = 0;
      bool existed = false;
      Pfm.OpenAttribs openAttribs = new Pfm.OpenAttribs();
      long parentFileId = 0;
      string endName = null;

      if (nameParts.Length == 0)
      {
         if (folderOpenId == 0)
         {
            folderOpenId = newExistingOpenId;
         }
         existed = true;
         openAttribs.openId = folderOpenId;
         openAttribs.openSequence = 1;
         openAttribs.accessLevel = Pfm.accessLevelReadData;
         openAttribs.attribs.fileType = Pfm.fileTypeFolder;
         openAttribs.attribs.fileId = helloRootId;
      }
      else if(nameParts.Length == 1)
      {
         if(nameParts[0].ToLowerInvariant() != helloFileName.ToLowerInvariant())
         {
            perr = Pfm.errorNotFound;
         }
         else
         {
            if (fileOpenId == 0)
            {
               fileOpenId = newExistingOpenId;
            }
            existed = true;
            openAttribs.openId = fileOpenId;
            openAttribs.openSequence = 1;
            openAttribs.accessLevel = Pfm.accessLevelReadData;
            openAttribs.attribs.fileType = Pfm.fileTypeFile;
            openAttribs.attribs.fileId = helloFileId;
            openAttribs.attribs.fileSize = helloFileData.Length;
            endName = helloFileName;
         }
      }
      else
      {
         perr = Pfm.errorParentNotFound;
      }
      if(perr == Pfm.errorNotFound && createFileType != 0)
      {
         perr = Pfm.errorAccessDenied;
      }

      op.Complete( perr, existed, openAttribs, parentFileId, endName, 0, null, 0, null);
   }

   public void Replace( Pfm.MarshallerReplaceOp op)
   {
      op.Complete( Pfm.errorAccessDenied, null/*openAttribs*/, null);
   }

   public void Move( Pfm.MarshallerMoveOp op)
   {
      op.Complete( Pfm.errorAccessDenied, false/*existed*/, null/*openAttribs*/, 0/*parentFileId*/, null/*endName*/, 0, null, 0, null);
   }

   public void MoveReplace( Pfm.MarshallerMoveReplaceOp op)
   {
      op.Complete( Pfm.errorAccessDenied);
   }

   public void Delete( Pfm.MarshallerDeleteOp op)
   {
      op.Complete( Pfm.errorAccessDenied);
   }

   public void Close( Pfm.MarshallerCloseOp op)
   {
      op.Complete( Pfm.errorSuccess);
   }

   public void FlushFile( Pfm.MarshallerFlushFileOp op)
   {
      long openId = op.OpenId();
      int fileFlags = op.FileFlags();
      int color = op.Color();
      long createTime = op.CreateTime();
      long writeTime = op.WriteTime();
      int perr = 0;
      Pfm.OpenAttribs openAttribs = new Pfm.OpenAttribs();

      if (fileFlags != Pfm.fileFlagsInvalid ||
         color != Pfm.colorInvalid ||
         createTime != Pfm.timeInvalid ||
         writeTime != Pfm.timeInvalid)
      {
         perr = Pfm.errorAccessDenied;
      }
      else if (openId == folderOpenId)
      {
         openAttribs.openId = folderOpenId;
         openAttribs.openSequence = 1;
         openAttribs.accessLevel = Pfm.accessLevelReadData;
         openAttribs.attribs.fileType = Pfm.fileTypeFolder;
         openAttribs.attribs.fileId = helloRootId;
      }
      else if (openId == fileOpenId)
      {
         openAttribs.openId = fileOpenId;
         openAttribs.openSequence = 1;
         openAttribs.accessLevel = Pfm.accessLevelReadData;
         openAttribs.attribs.fileType = Pfm.fileTypeFile;
         openAttribs.attribs.fileId = helloFileId;
         openAttribs.attribs.fileSize = helloFileData.Length;
      }
      else
      {
         perr = Pfm.errorNotFound;
      }

      op.Complete( perr, openAttribs, null);
   }

   public void List( Pfm.MarshallerListOp op)
   {
      long openId = op.OpenId();
      int perr = 0;
      Pfm.Attribs attribs = new Pfm.Attribs();

      if (openId != folderOpenId)
      {
         perr = Pfm.errorAccessDenied;
      }
      else
      {
         attribs.fileType = Pfm.fileTypeFile;
         attribs.fileId = helloFileId;
         attribs.fileSize = helloFileData.Length;
         op.Add( attribs, helloFileName);
      }

      op.Complete( perr, true/*noMore*/);
   }

   public void ListEnd( Pfm.MarshallerListEndOp op)
   {
      op.Complete( Pfm.errorSuccess);
   }

   public void Read( Pfm.MarshallerReadOp op)
   {
      long openId = op.OpenId();
      long fileOffset = op.FileOffset();
      byte[] data = op.Data();
      int requestedSize = op.RequestedSize();
      int perr = 0;
      int actualSize = 0;
      int i;

      if (openId != fileOpenId)
      {
         perr = Pfm.errorAccessDenied;
      }
      else
      {
         actualSize = requestedSize;
         if (fileOffset > helloDataSize)
         {
            actualSize = 0;
         }
         else if (fileOffset+requestedSize > helloDataSize)
         {
            actualSize = (int)(helloDataSize-fileOffset);
         }
         if (actualSize != 0)
         {
            for (i = 0; i < actualSize; i++)
            {
               data[i] = (byte)(helloFileData[(int)fileOffset+i]);
            }
         }
      }

      op.Complete( perr, actualSize);
   }

   public void Write( Pfm.MarshallerWriteOp op)
   {
      op.Complete( Pfm.errorAccessDenied, 0/*actualSize*/);
   }

   public void SetSize( Pfm.MarshallerSetSizeOp op)
   {
      op.Complete( Pfm.errorAccessDenied);
   }

   public void Capacity( Pfm.MarshallerCapacityOp op)
   {
      op.Complete( Pfm.errorSuccess, helloFileData.Length/*totalCapacity*/, 0/*availableCapacity*/);
   }

   public void FlushMedia( Pfm.MarshallerFlushMediaOp op)
   {
      op.Complete( Pfm.errorSuccess, -1/*flushTimeoutMsecs*/);
   }

   public void Control( Pfm.MarshallerControlOp op)
   {
      op.Complete( Pfm.errorInvalid, 0/*outputSize*/);
   }

   public void MediaInfo( Pfm.MarshallerMediaInfoOp op)
   {
      Pfm.MediaInfo mediaInfo = new Pfm.MediaInfo();

      op.Complete( Pfm.errorSuccess, mediaInfo, "hellofs"/*mediaLabel*/);
   }

   public void Access( Pfm.MarshallerAccessOp op)
   {
      long openId = op.OpenId();
      int perr = 0;
      Pfm.OpenAttribs openAttribs = new Pfm.OpenAttribs();

      if (openId == folderOpenId)
      {
         openAttribs.openId = folderOpenId;
         openAttribs.openSequence = 1;
         openAttribs.accessLevel = Pfm.accessLevelReadData;
         openAttribs.attribs.fileType = Pfm.fileTypeFolder;
         openAttribs.attribs.fileId = helloRootId;
      }
      else if(openId == fileOpenId)
      {
         openAttribs.openId = fileOpenId;
         openAttribs.openSequence = 1;
         openAttribs.accessLevel = Pfm.accessLevelReadData;
         openAttribs.attribs.fileType = Pfm.fileTypeFile;
         openAttribs.attribs.fileId = helloFileId;
         openAttribs.attribs.fileSize = helloFileData.Length;
      }
      else
      {
         perr = Pfm.errorNotFound;
      }

      op.Complete( perr, openAttribs, null);
   }

   public void ReadXattr( Pfm.MarshallerReadXattrOp op)
   {
      op.Complete( Pfm.errorNotFound, 0/*xattrSize*/, 0/*transferredSize*/);
   }

   public void WriteXattr( Pfm.MarshallerWriteXattrOp op)
   {
      op.Complete( Pfm.errorAccessDenied, 0/*transferredSize*/);
   }
}

public class Main_
{

static void Main( string[] args)
{
   int err = 0;
   Pfm.Api pfm = null;
   Pfm.Mount mount = null;
   Pfm.MountCreateParams mcp = new Pfm.MountCreateParams();
   Pfm.MarshallerServeParams msp = new Pfm.MarshallerServeParams();
   Volume volume = new Volume();
   IntPtr invalidFd = new IntPtr( -1);

   msp.dispatch = volume;
   msp.formatterName = "hellofs";

   if (args.Length != 1)
   {
      Console.Write(
         "Sample file system application.\n" +
         "syntax: hellofs <mount name>\n");
      err = -1;
   }
   if (err == 0)
   {
      mcp.mountSourceName = args[0];
      err = Pfm.ApiFactory( out pfm);
      if(err != 0)
      {
         Console.Write( "ERROR: {0} Unable to open PFM API.\n", err);
      }
   }
   if (err == 0)
   {
      err = Pfm.MarshallerFactory( out volume.marshaller);
      if(err != 0)
      {
         Console.Write( "ERROR: {0} Unable to create marshaller.\n", err);
      }
   }
   if (err == 0)
   {
         // Communication between the driver and file system is done
         // over a pair of simple pipes. Application is responsible
         // for creating the pipes.
      err = Pfm.SystemCreatePipe( out msp.toFormatterRead, out mcp.toFormatterWrite);
      if(err == 0)
      {
         err = Pfm.SystemCreatePipe( out mcp.fromFormatterRead, out msp.fromFormatterWrite);
      }
   }
   if (err == 0)
   {
         // Various mount options are available through mountFlags,
         // visibleProcessId, and ownerSid.
      err = pfm.MountCreate( mcp, out mount);
      if (err != 0)
      {
         Console.Write( "ERROR: {0} Unable to create mount.\n", err);
      }
   }

      // Close driver end pipe handles now. Driver has duplicated what
      // it needs. If these handles are not closed then pipes will not
      // break if driver disconnects, leaving us stuck in the
      // marshaller.
   Pfm.SystemCloseFd( mcp.toFormatterWrite);
   Pfm.SystemCloseFd( mcp.fromFormatterRead);

   if (err == 0)
   {
         // If tracemon is installed and running then diagnostic
         // messsages can be viewed in the "hellofs" channel.
      volume.marshaller.SetTrace( mount.GetMountEndName());
         // Also send diagnostic messages to stdout. This can slow
         // things down quite a bit.
      IntPtr stdoutFd = invalidFd;
      Pfm.SystemGetStdout( out stdoutFd);
      volume.marshaller.SetStatus( stdoutFd);

         // The marshaller uses alertable I/O, so process can be
         // exited via ctrl+c.
      Console.Write( "Press CTRL+C to exit.\n");
         // The marshaller serve function will return at unmount or
         // if driver disconnects.
      volume.marshaller.ServeDispatch( msp);
   }

   Pfm.SystemCloseFd( msp.toFormatterRead);
   Pfm.SystemCloseFd( msp.fromFormatterWrite);
   if (pfm != null)
   {
      pfm.Dispose();
   }
   if (mount != null)
   {
      mount.Dispose();
   }
   if (volume.marshaller != null)
   {
      volume.marshaller.Dispose();
   }
}

}
