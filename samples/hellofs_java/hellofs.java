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
// file name:  hellofs.java
// created:    2015.06.26
//----------------------------------------------------------------------------
import java.io.IOException;
import com.pismotec.pfm185.Pfm;

class hellofs {

public static class Volume implements Pfm.FormatterDispatch
{
   Pfm.Marshaller marshaller;
   long folderOpenId = 0;
   long fileOpenId = 0;
   String helloFileName = "readme.txt";
   String helloFileData = "Hello World.\r\n";
   int helloDataSize = helloFileData.length();
   long helloRootId = 2;
   long helloFileId = 3;

   public void Open( Pfm.MarshallerOpenOp op)
   {
      String[] nameParts = op.NameParts();
      int createFileType = op.CreateFileType();
      long newExistingOpenId = op.NewExistingOpenId();
      int perr = 0;
      boolean existed = false;
      Pfm.OpenAttribs openAttribs = new Pfm.OpenAttribs();
      long parentFileId = 0;
      String endName = null;

      if (nameParts.length == 0)
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
      else if(nameParts.length == 1)
      {
         if (!nameParts[0].equalsIgnoreCase( helloFileName))
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
            openAttribs.attribs.fileSize = helloFileData.length();
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
         openAttribs.attribs.fileSize = helloFileData.length();
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
         attribs.fileSize = helloFileData.length();
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
            actualSize = (int)( helloDataSize-fileOffset);
         }
         if (actualSize != 0)
         {
            for (i = 0; i < actualSize; i++)
            {
               data[i] = (byte)( helloFileData.charAt( (int)( fileOffset+i)));
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
      op.Complete( Pfm.errorSuccess, helloFileData.length()/*totalCapacity*/, 0/*availableCapacity*/);
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
         openAttribs.attribs.fileSize = helloFileData.length();
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
};

public static void main( String[] args)
{
   int err = 0;
   Pfm.Api pfm = null;
   Pfm.Mount mount = null;
   Pfm.MountCreateParams mcp = new Pfm.MountCreateParams();
   Pfm.MarshallerServeParams msp = new Pfm.MarshallerServeParams();
   Volume volume = new Volume();

   msp.dispatch = volume;
   msp.formatterName = "hellofs";

   if (args.length != 1)
   {
      System.out.println(
         "Sample file system application.\n" +
         "syntax: hellofs <mount name>\n");
      err = -1;
   }
   if (err == 0)
   {
      mcp.mountSourceName = args[0];
   }
   if (err == 0)
   {
      Pfm.ApiBox apiBox = new Pfm.ApiBox();
      err = Pfm.ApiFactory( apiBox);
      pfm = apiBox.val;
      if(err != 0)
      {
         System.out.println( "ERROR: " + err + " Unable to open PFM API.\n");
      }
   }
   if (err == 0)
   {
      Pfm.MarshallerBox marshallerBox = new Pfm.MarshallerBox();
      err = Pfm.MarshallerFactory( marshallerBox);
      volume.marshaller = marshallerBox.val;
      if(err != 0)
      {
         System.out.println( "ERROR: " + err +  " Unable to create marshaller.\n");
      }
   }
   if (err == 0)
   {
         // Communication between the driver and file system is done
         // over a pair of simple pipes. Application is responsible
         // for creating the pipes.
      Pfm.FileDescriptorBox fd1Box = new Pfm.FileDescriptorBox();
      Pfm.FileDescriptorBox fd2Box = new Pfm.FileDescriptorBox();
      err = Pfm.SystemCreatePipe( fd1Box, fd2Box);
      msp.toFormatterRead = fd1Box.val;
      mcp.toFormatterWrite = fd2Box.val;
      if(err == 0)
      {
         err = Pfm.SystemCreatePipe( fd1Box, fd2Box);
         mcp.fromFormatterRead = fd1Box.val;
         msp.fromFormatterWrite = fd2Box.val;
      }
   }
   if (err == 0)
   {
         // Various mount options are available through mountFlags,
         // visibleProcessId, and ownerSid.
      Pfm.MountBox mountBox = new Pfm.MountBox();
      err = pfm.MountCreate( mcp, mountBox);
      mount = mountBox.val;
      if (err != 0)
      {
         System.out.println( "ERROR: " + err + " Unable to create mount.\n");
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
      volume.marshaller.SetTrace( "hellofs");
         // Also send diagnostic messages to stdout. This can slow
         // things down quite a bit.
      Pfm.FileDescriptorBox stdoutFdBox = new Pfm.FileDescriptorBox();
      Pfm.SystemGetStdout( stdoutFdBox);
      volume.marshaller.SetStatus( stdoutFdBox.val);

         // The marshaller uses alertable I/O, so process can be
         // exited via ctrl+c.
      System.out.println( "Press CTRL+C to exit.\n");
         // The marshaller serve function will return at unmount or
         // if driver disconnects.
      volume.marshaller.ServeDispatch( msp);
   }

   Pfm.SystemCloseFd( msp.toFormatterRead);
   Pfm.SystemCloseFd( msp.fromFormatterWrite);
   if (pfm != null)
   {
      try { pfm.close(); } catch (Exception ex) { }
   }
   if (mount != null)
   {
      try { mount.close(); } catch (Exception ex) { }
   }
   if (volume.marshaller != null)
   {
      try { volume.marshaller.close(); } catch (Exception ex) { }
   }
}

}
