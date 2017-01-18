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
// file name:  tempfs.cs
// created:    2008.05.04
//   Temporary file system sample application.
//----------------------------------------------------------------------------
using System;
using System.Collections;
using System.Text;
using System.IO;
using System.Diagnostics;

class AssocArray_Long_Object :
   System.Collections.Generic.SortedDictionary<long, Object>
{
   public Object Get(long key)
   {
      try { return this[key]; }
      catch (System.Collections.Generic.KeyNotFoundException) { }
      return null;
   }

   public void Set(long key, Object val)
   {
      this[key] = val;
   }
}

class Volume: Pfm.FormatterDispatch
{
   public Pfm.Marshaller marshaller;
   const int blockSize = 8888;
   long lastFileId;
   long lastOpenSequence;
   long totalBlockCount;

   class NameLink
   {
      public string endName;
      public File file;

      public NameLink( string inEndName, File inFile)
      {
         endName = inEndName;
         file = inFile;
      }
   }

   class File
   {
      public int nameCount;
      public int fileType;
      public int fileFlags;
      public long openId;
      public long fileId;
      public long createTime;
      public long accessTime;
      public long writeTime;
      public long changeTime;
      public long fileSize;
      public ArrayList data;
      public SortedList children;

      public File( int inFileType, int inFileFlags, long inWriteTime)
      {
         fileType = inFileType;
         fileFlags = inFileFlags;
         createTime = inWriteTime;
         accessTime = inWriteTime;
         writeTime = inWriteTime;
         changeTime = inWriteTime;
         fileSize = 0;
         data = new ArrayList();
         children = new SortedList();
      }
   }

   class ListRef
   {
      public int position;
   }

   class OpenRef
   {
      public long openSequence;
      public File file;
      public AssocArray_Long_Object listRefs;

      public OpenRef( File inFile)
      {
         file = inFile;
         listRefs = new AssocArray_Long_Object();
      }
   }

   AssocArray_Long_Object openRefs;
   AssocArray_Long_Object fileIds;
   File root;

   public Volume()
   {
      lastFileId = 1;
      lastOpenSequence = 0;
      totalBlockCount = 0;
      openRefs = new AssocArray_Long_Object();
      fileIds = new AssocArray_Long_Object();
      root = new File( Pfm.fileTypeFolder, 0/*fileFlags*/, 0/*writeTime*/);
      root.nameCount = 1;
      root.fileId = 1;
      fileIds[root.fileId] = root;
   }

   void FileOpened( File file, ref Pfm.OpenAttribs openAttribs)
   {
      Debug.Assert( file.openId != 0);
      OpenRef openRef = (OpenRef)(openRefs.Get( file.openId));
      if (openRef == null)
      {
         openRef = new OpenRef( file);
         openRefs.Set( file.openId, openRef);
      }
      openRef.openSequence = ++lastOpenSequence;
      openAttribs.openId = file.openId;
      openAttribs.openSequence = openRef.openSequence;
      openAttribs.accessLevel = Pfm.accessLevelWriteData;
      openAttribs.attribs.fileType = file.fileType;
      openAttribs.attribs.fileFlags = file.fileFlags;
      openAttribs.attribs.fileId = file.fileId;
      openAttribs.attribs.createTime = file.createTime;
      openAttribs.attribs.accessTime = file.accessTime;
      openAttribs.attribs.writeTime = file.writeTime;
      openAttribs.attribs.changeTime = file.changeTime;
      openAttribs.attribs.fileSize = file.fileSize;
   }

   int/*perr*/ FileFindOpenId( long openId, out File file)
   {
      int perr = Pfm.errorInvalid;
      file = null;
      OpenRef openRef = (OpenRef)(openRefs.Get( openId));
      if (openRef != null)
      {
         file = openRef.file;
         perr = 0;
      }
      return perr;
   }

   int/*perr*/ FileFindFileId( long fileId, out File file)
   {
      int perr = Pfm.errorNotFound;
      file = (File)(fileIds.Get( fileId));
      if (file != null)
      {
         perr = 0;
      }
      return perr;
   }

   int/*perr*/ FileFindName( string[] nameParts, out File parent, out File file, out string endName)
   {
      int perr = 0;
      parent = null;
      file = root;
      endName = "";
      int i = 0;
      while (perr == 0 && i != nameParts.Length)
      {
         parent = file;
         file = null;
         endName = nameParts[i++];
         string foldedName = endName.ToLower();
         NameLink nameLink = (NameLink)(parent.children[ foldedName]);
         if (nameLink == null)
         {
            perr = Pfm.errorNotFound;
            if (i != nameParts.Length)
            {
               parent = null;
               endName = null;
               perr = Pfm.errorParentNotFound;
            }
         }
         else
         {
            file = nameLink.file;
            endName = nameLink.endName;
         }
      }
      return perr;
   }

   int/*actualSize*/ FileRead( File file, long fileOffset, byte[] data, int requestedSize)
   {
      int actualSize = 0;
      if (fileOffset < file.fileSize)
      {
         actualSize = requestedSize;
         if (fileOffset + actualSize > file.fileSize)
         {
            actualSize = (int)(file.fileSize - fileOffset);
         }
      }
      if (actualSize != 0)
      {
         data = new byte[actualSize];
         int i = 0;
         while (i < actualSize)
         {
            byte[] block = (byte[])(file.data[ (int)((fileOffset + i) / blockSize)]);
            int j = (int)((fileOffset + i) % blockSize);
            int partSize = blockSize - j;
            if (i + partSize > actualSize)
            {
               partSize = actualSize - i;
            }
            int k = j + partSize;
            while (j < k)
            {
               data[i++] = block[j++];
            }
            i += partSize;
         }
      }
      return actualSize;
   }

   int/*actualSize*/ FileWrite( File file, long fileOffset, byte[] data, int requestedSize)
   {
      long endOffset = fileOffset+requestedSize;
      int blockCount = (int)((file.fileSize+blockSize-1)/blockSize);
      Debug.Assert( blockCount == file.data.Count);
      int needBlockCount = (int)((endOffset + blockSize - 1) / blockSize);
      int adjustBlockCount = 0;
      while (blockCount < needBlockCount)
      {
         file.data.Add( new byte[blockSize]);
         blockCount ++;
         adjustBlockCount ++;
      }
      if (requestedSize == 0)
      {
         while (blockCount > needBlockCount)
         {
            file.data.RemoveAt( --blockCount);
            adjustBlockCount --;
         }
         Debug.Assert( file.data.Count == needBlockCount);
         file.fileSize = fileOffset;
      }
      else
      {
         int i = 0;
         while (i < requestedSize)
         {
            byte[] block = (byte[])(file.data[ (int)((fileOffset + i) / blockSize)]);
            int j = (int)((fileOffset + i) % blockSize);
            int partSize = blockSize - j;
            if (i + partSize > requestedSize)
            {
               partSize = requestedSize - i;
            }
            int k = j + partSize;
            while (j < k)
            {
               block[ j++] = data[ i++];
            }
            i += partSize;
         }
         if (endOffset > file.fileSize)
         {
            file.fileSize = endOffset;
         }
      }
      totalBlockCount += adjustBlockCount;
      return requestedSize;
   }

   void FileNameAdd( File parent, long writeTime, string endName, File file)
   {
      if (file.fileId == 0)
      {
         file.fileId = ++lastFileId;
      }
      NameLink nameLink = new NameLink( endName, file);
      string foldedName = endName.ToLower();
      parent.children[ foldedName] = nameLink;
      parent.writeTime = writeTime;
      parent.changeTime = writeTime;
      file.nameCount ++;
      fileIds.Set( file.fileId, file);
   }

   int/*perr*/ FileNameRemove( File parent, long writeTime, string endName, File file)
   {
      int perr = Pfm.errorNotFound;
      NameLink nameLink;
      string foldedName = endName.ToLower();
      nameLink = (NameLink)(parent.children[foldedName]);
      if (nameLink != null && nameLink.file == file)
      {
         perr = 0;
         parent.children.Remove( foldedName);
         parent.writeTime = writeTime;
         parent.changeTime = writeTime;
         file.nameCount--;
         if (file.nameCount == 0)
         {
            fileIds.Remove( file.fileId);
            if (openRefs.Get( file.openId) == null)
            {
               FileWrite( file, 0, null, 0);
            }
         }
      }
      return perr;
   }

      // Pfm.FormatterDispatch.Open
      // Open existing file, or create new file.
   public void Open( Pfm.MarshallerOpenOp op)
   {
      string[] nameParts = op.NameParts();
      int createFileType = op.CreateFileType();
      int createFileFlags = op.CreateFileFlags();
      long writeTime = op.WriteTime();
      long newCreateOpenId = op.NewCreateOpenId();
      long newExistingOpenId = op.NewExistingOpenId();
      int perr;
      bool existed = false;
      Pfm.OpenAttribs openAttribs = new Pfm.OpenAttribs();
      long parentFileId = 0;
      string endName = null;

      File parent;
      File file;
      perr = FileFindName( nameParts, out parent, out file, out endName);
      if (perr == 0)
      {
         existed = true;
         if (parent != null)
         {
            parentFileId = parent.fileId;
         }
         if (file.openId == 0)
         {
            file.openId = newExistingOpenId;
         }
         FileOpened( file, ref openAttribs);
      }
      else if (perr == Pfm.errorNotFound && createFileType != Pfm.fileTypeNone)
      {
         file = new File( createFileType, createFileFlags, writeTime);
         FileNameAdd( parent, writeTime, endName, file);
         file.openId = newCreateOpenId;
         FileOpened( file, ref openAttribs);
         parentFileId = parent.fileId;
         perr = 0;
      }

      op.Complete( perr, existed, openAttribs, parentFileId, endName, 0, null, 0, null);
   }

      // Pfm.FormatterDispatch.Replace
      // Replace an existing file with a newly created file.
   public void Replace( Pfm.MarshallerReplaceOp op)
   {
      long targetOpenId = op.TargetOpenId();
      long targetParentFileId = op.TargetParentFileId();
      string targetEndName = op.TargetEndName();
      int createFileFlags = op.CreateFileFlags();
      long writeTime = op.WriteTime();
      long newCreateOpenId = op.NewCreateOpenId();
      int perr;
      Pfm.OpenAttribs openAttribs = new Pfm.OpenAttribs();
      File targetFile;
      File parent;
      File file;

      perr = FileFindOpenId( targetOpenId, out targetFile);
      if (perr == 0)
      {
         perr = FileFindFileId( targetParentFileId, out parent);
         if (perr == 0)
         {
            file = new File( targetFile.fileType, createFileFlags,writeTime);
            perr = FileNameRemove( parent, writeTime, targetEndName,targetFile);
            if (perr == 0)
            {
               FileNameAdd( parent, writeTime, targetEndName, file);
               file.openId = newCreateOpenId;
               FileOpened( file, ref openAttribs);
            }
         }
      }

      op.Complete( perr, openAttribs, null);
   }

      // Pfm.FormatterDispatch.Move
      // Open an existing target file, or move a previously opened
      // file to a _new target name if the target does not exist.
   public void Move( Pfm.MarshallerMoveOp op)
   {
      long sourceOpenId = op.SourceOpenId();
      long sourceParentFileId = op.SourceParentFileId();
      string sourceEndName = op.SourceEndName();
      string[] targetNameParts = op.TargetNameParts();
      bool deleteSource = op.DeleteSource();
      long writeTime = op.WriteTime();
      long newExistingOpenId = op.NewExistingOpenId();
      int perr;
      bool existed = false;
      Pfm.OpenAttribs openAttribs = new Pfm.OpenAttribs();
      long parentFileId = 0;
      string endName = null;
      File sourceFile;
      File targetParent;
      File targetFile;
      File sourceParent;

      perr = FileFindOpenId( sourceOpenId, out sourceFile);
      if (perr == 0)
      {
         perr = FileFindName( targetNameParts, out targetParent, out targetFile, out endName);
         if (perr == 0)
         {
            existed = true;
            if (targetParent != null)
            {
               parentFileId = targetParent.fileId;
            }
            if (targetFile.openId == 0)
            {
               targetFile.openId = newExistingOpenId;
            }
            FileOpened( targetFile, ref openAttribs);
         }
         else if (perr == Pfm.errorNotFound)
         {
            FileNameAdd( targetParent, writeTime, endName, sourceFile);
            perr = 0;
            parentFileId = targetParent.fileId;
            FileOpened( sourceFile, ref openAttribs);
            if (deleteSource && FileFindFileId( sourceParentFileId, out sourceParent) == 0)
            {
               FileNameRemove( sourceParent, writeTime, sourceEndName, sourceFile);
            }
         }
      }

      op.Complete( perr, existed, openAttribs, parentFileId, endName, 0, null, 0, null);
   }

      // Pfm.FormatterDispatch.MoveReplace
      // Delete a previously opened target file and move a
      // previously opened source file to the target file name.
   public void MoveReplace( Pfm.MarshallerMoveReplaceOp op)
   {
      long sourceOpenId = op.SourceOpenId();
      long sourceParentFileId = op.SourceParentFileId();
      string sourceEndName = op.SourceEndName();
      long targetOpenId = op.TargetOpenId();
      long targetParentFileId = op.TargetParentFileId();
      string targetEndName = op.TargetEndName();
      bool deleteSource = op.DeleteSource();
      long writeTime = op.WriteTime();
      int perr;
      File sourceFile;
      File sourceParent = null;
      File targetFile = null;
      File targetParent = null;

      perr = FileFindOpenId( sourceOpenId, out sourceFile);
      if (perr == 0)
      {
         perr = FileFindFileId( sourceParentFileId, out sourceParent);
      }
      if (perr == 0)
      {
         perr = FileFindOpenId( targetOpenId, out targetFile);
      }
      if (perr == 0)
      {
         perr = FileFindFileId( targetParentFileId, out targetParent);
      }
      if (perr == 0)
      {
         perr = FileNameRemove( targetParent, writeTime, targetEndName, targetFile);
      }
      if (perr == 0)
      {
         FileNameAdd( targetParent, writeTime, targetEndName, sourceFile);
      }
      if (perr == 0 && deleteSource)
      {
         FileNameRemove( sourceParent, writeTime, sourceEndName, sourceFile);
      }

      op.Complete( perr);
   }

      // Pfm.FormatterDispatch.Delete
   public void Delete( Pfm.MarshallerDeleteOp op)
   {
      long openId = op.OpenId();
      long parentFileId = op.ParentFileId();
      string endName = op.EndName();
      long writeTime = op.WriteTime();
      int perr;
      File file;
      File parent;

      perr = FileFindOpenId(openId, out file);
      if (perr == 0)
      {
         perr = FileFindFileId( parentFileId, out parent);
         if (perr == 0)
         {
            FileNameRemove( parent, writeTime, endName, file);
         }
      }

      op.Complete( perr);
   }

      // Pfm.FormatterDispatch.Close
   public void Close( Pfm.MarshallerCloseOp op)
   {
      long openId = op.OpenId();
      long openSequence = op.OpenSequence();
      int perr = 0;
      OpenRef openRef;

      openRef = (OpenRef)(openRefs.Get( openId));
      if (openRef == null)
      {
         perr = Pfm.errorInvalid;
      }
      else
      {
         if (openRef.openSequence <= openSequence)
         {
            openRefs.Remove( openId);
            if (openRef.file.nameCount == 0)
            {
               FileWrite( openRef.file, 0, null, 0);
            }
         }
      }

      op.Complete( perr);
   }

      // Pfm.FormatterDispatch.FlushFile
   public void FlushFile( Pfm.MarshallerFlushFileOp op)
   {
      long openId = op.OpenId();
      int flushFlags = op.FlushFlags();
      int fileFlags = op.FileFlags();
      long createTime = op.CreateTime();
      long accessTime = op.AccessTime();
      long writeTime = op.WriteTime();
      long changeTime = op.ChangeTime();
      int perr = 0;
      Pfm.OpenAttribs openAttribs = new Pfm.OpenAttribs();
      File file;

      perr = FileFindOpenId( openId, out file);
      if (perr == 0)
      {
         if (fileFlags != Pfm.fileFlagsInvalid)
         {
            file.fileFlags = fileFlags;
         }
         if (createTime != Pfm.timeInvalid)
         {
            file.createTime = createTime;
         }
         if (accessTime != Pfm.timeInvalid)
         {
            file.accessTime = accessTime;
         }
         if (writeTime != Pfm.timeInvalid)
         {
            file.writeTime = writeTime;
         }
         if (changeTime != Pfm.timeInvalid)
         {
            file.changeTime = changeTime;
         }
         if ((flushFlags & Pfm.flushFlagOpen) != 0)
         {
            FileOpened( file, ref openAttribs);
         }
      }

      op.Complete( perr, openAttribs, null);
   }

      // Pfm.FormatterDispatch.List
   public void List( Pfm.MarshallerListOp op)
   {
      long openId = op.OpenId();
      long listId = op.ListId();
      int perr = 0;
      bool noMore = true;
      SortedList children;
      Pfm.Attribs attribs = new Pfm.Attribs();
      OpenRef openRef;
      ListRef listRef;
      NameLink nameLink;
      File file;

      openRef = (OpenRef)(openRefs.Get( openId));
      if (openRef == null)
      {
         perr = Pfm.errorInvalid;
      }
      else
      {
         listRef = (ListRef)(openRef.listRefs.Get( listId));
         if (listRef == null)
         {
            listRef = new ListRef();
            openRef.listRefs.Set(listId, listRef);
         }
         children = openRef.file.children;
         while (true)
         {
            if(listRef.position >= children.Count)
            {
               noMore = true;
            }
            else
            {
               nameLink = (NameLink)(children.GetByIndex( listRef.position));
               file = nameLink.file;
               attribs.fileId = file.fileId;
               attribs.fileType = file.fileType;
               attribs.createTime = file.createTime;
               attribs.accessTime = file.accessTime;
               attribs.writeTime = file.writeTime;
               attribs.changeTime = file.changeTime;
               attribs.fileSize = file.fileSize;
               if (op.Add( attribs, nameLink.endName))
               {
                  listRef.position ++;
                  continue;
               }
            }
            break;
         }
      }

      op.Complete( perr, noMore);
   }

      // Pfm.FormatterDispatch.ListEnd
   public void ListEnd( Pfm.MarshallerListEndOp op)
   {
      long openId = op.OpenId();
      long listId = op.ListId();
      int perr = 0;
      OpenRef openRef;

      openRef = (OpenRef)(openRefs.Get( openId));
      if (openRef == null)
      {
         perr = Pfm.errorInvalid;
      }
      else
      {
         openRef.listRefs.Remove( listId);
      }

      op.Complete( perr);
   }

      // Pfm.FormatterDispatch.Read
   public void Read( Pfm.MarshallerReadOp op)
   {
      long openId = op.OpenId();
      long fileOffset = op.FileOffset();
      byte[] data = op.Data();
      int requestedSize = op.RequestedSize();
      int perr;
      int actualSize = 0;
      File file;

      perr = FileFindOpenId( openId, out file);
      if (perr == 0)
      {
         actualSize = FileRead( file, fileOffset, data, requestedSize);
      }

      op.Complete( perr, actualSize);
   }

      // Pfm.FormatterDispatch.Write
   public void Write( Pfm.MarshallerWriteOp op)
   {
      long openId = op.OpenId();
      long fileOffset = op.FileOffset();
      byte[] data = op.Data();
      int requestedSize = op.RequestedSize();
      int perr;
      int actualSize = 0;
      File file;

      perr = FileFindOpenId( openId, out file);
      if (perr == 0)
      {
         try
         {
            actualSize = FileWrite( file, fileOffset, data, requestedSize);
         }
         catch (OutOfMemoryException)
         {
            perr = Pfm.errorNoSpace;
         }
      }

      op.Complete( perr, actualSize);
   }

      // Pfm.FormatterDispatch.SetSize
   public void SetSize( Pfm.MarshallerSetSizeOp op)
   {
      long openId = op.OpenId();
      long fileSize = op.FileSize();
      int perr;
      File file;

      perr = FileFindOpenId( openId, out file);
      if (perr == 0)
      {
         try
         {
            FileWrite( file, fileSize, null, 0);
         }
         catch (OutOfMemoryException)
         {
            perr = Pfm.errorNoSpace;
         }
      }

      op.Complete( perr);
   }

      // Pfm.FormatterDispatch.Capacity
   public void Capacity( Pfm.MarshallerCapacityOp op)
   {
      int perr = 0;
      long totalCapacity;
      long availableCapacity;

      totalCapacity = totalBlockCount*blockSize;
      availableCapacity = 1000000000;

      op.Complete( perr, totalCapacity, availableCapacity);
   }

      // Pfm.FormatterDispatch.FlushMedia
   public void FlushMedia( Pfm.MarshallerFlushMediaOp op)
   {
      int perr = 0;

      op.Complete( perr, -1/*flushTimeoutMsecs*/);
   }

      // Pfm.FormatterDispatch.Control
   public void Control( Pfm.MarshallerControlOp op)
   {
      int perr;
      int outputSize = 0;

      perr = Pfm.errorInvalid;

      op.Complete( perr, outputSize);
   }

      // Pfm.FormatterDispatch.MediaInfo
   public void MediaInfo( Pfm.MarshallerMediaInfoOp op)
   {
      int perr = 0;
      Pfm.MediaInfo mediaInfo = new Pfm.MediaInfo();
      string mediaLabel;

      mediaLabel = "tempfs";

      op.Complete( perr, mediaInfo, mediaLabel);
   }

      // Pfm.FormatterDispatch.Access
      // Read attributes for open file or folder.
   public void Access( Pfm.MarshallerAccessOp op)
   {
      long openId = op.OpenId();
      int perr = 0;
      Pfm.OpenAttribs openAttribs = new Pfm.OpenAttribs();
      File file;

      perr = FileFindOpenId(openId, out file);
      if (perr == 0)
      {
         FileOpened( file, ref openAttribs);
      }

      op.Complete( perr, openAttribs, null);
   }

      // Pfm.FormatterDispatch.ReadXattr
   public void ReadXattr( Pfm.MarshallerReadXattrOp op)
   {
      int perr;
      int xattrSize = 0;
      int transferredSize = 0;

      perr = Pfm.errorNotFound;

      op.Complete( perr, xattrSize, transferredSize);
   }

      // Pfm.FormatterDispatch.WriteXattr
   public void WriteXattr( Pfm.MarshallerWriteXattrOp op)
   {
      int perr;
      int transferredSize = 0;

      perr = Pfm.errorAccessDenied;

      op.Complete( perr, transferredSize);
   }
}

class Main_
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
   msp.formatterName = "tempfs";

   if (args.Length != 1)
   {
      Console.Write(
         "Sample file system application.\n" +
         "syntax: tempfs <mount name>\n" +
         "A ram-backed volume will be created using the specified mount name.\n");
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
         // messsages can be viewed in the "tempfs" channel.
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
