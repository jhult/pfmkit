//----------------------------------------------------------------------------
// Copyright 2006-2015 Joe Lowe
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
// file name:  hellofs.c
// created:    2006.10.06
//    Hello world file system sample application.
//
//    This sample should compile with MSVC12 (DevStudio 2013) on Windows,
//    and via various other (GCC, LLVM) compilers on Windows, Mac, and Linux.
//----------------------------------------------------------------------------

#include "../common/portability.c"

#include "pfmapi.h"

static PfmOpenAttribs zeroOpenAttribs = {0};
static PfmAttribs zeroAttribs = {0};
static PfmMediaInfo zeroMediaInfo = {0};

static const wchar_t helloFileName[] = L"readme.txt";
static const char helloData[] = "Hello world.\r\n";
static const size_t helloDataSize = sizeof(helloData)-sizeof(helloData[0]);
static const int64_t helloRootId = 2;
static const int64_t helloFileId = 3;

typedef struct Volume Volume;
struct Volume
{
   PfmFormatterDispatch vtbl;
   PfmMarshaller* marshaller;
   int64_t folderOpenId;
   int64_t fileOpenId;
};
static __inline Volume* Volume_vcast( PfmFormatterDispatch* vol_)
   { return containerof( Volume, vtbl, vol_); }

void VolumeOpen( PfmFormatterDispatch* vol_, PfmMarshallerOpenOp* op, void* formatterUse)
{
   Volume* vol = Volume_vcast( vol_);
   const PfmNamePart* nameParts = (*op)->NameParts( op);
   size_t namePartCount = (*op)->NamePartCount( op);
   int8_t createFileType = (*op)->CreateFileType( op);
   int64_t newExistingOpenId = (*op)->NewExistingOpenId( op);
   int perr = 0;
   int/*bool*/ existed = 0/*false*/;
   PfmOpenAttribs openAttribs = zeroOpenAttribs;
   int64_t parentFileId = 0;
   const wchar_t* endName = 0;

   if (namePartCount == 0)
   {
      if (!vol->folderOpenId)
      {
         vol->folderOpenId = newExistingOpenId;
      }
      existed = 1/*true*/;
      openAttribs.openId = vol->folderOpenId;
      openAttribs.openSequence = 1;
      openAttribs.accessLevel = pfmAccessLevelReadData;
      openAttribs.attribs.fileType = pfmFileTypeFolder;
      openAttribs.attribs.fileId = helloRootId;
      perr = 0;
   }
   else if (namePartCount == 1)
   {
      if (sswcmpf(nameParts[0].name,helloFileName) != 0)
      {
         perr = pfmErrorNotFound;
      }
      else
      {
         if (!vol->fileOpenId)
         {
            vol->fileOpenId = newExistingOpenId;
         }
         existed = 1/*true*/;
         openAttribs.openId = vol->fileOpenId;
         openAttribs.openSequence = 1;
         openAttribs.accessLevel = pfmAccessLevelReadData;
         openAttribs.attribs.fileType = pfmFileTypeFile;
         openAttribs.attribs.fileId = helloFileId;
         openAttribs.attribs.fileSize = helloDataSize;
         endName = helloFileName;
      }
   }
   else
   {
      perr = pfmErrorParentNotFound;
   }
   if (perr == pfmErrorNotFound && createFileType != 0)
   {
      perr = pfmErrorAccessDenied;
   }

   (*op)->Complete( op, perr, existed, &openAttribs, parentFileId, endName, 0, 0, 0, 0);
}

void VolumeReplace( PfmFormatterDispatch* vol_, PfmMarshallerReplaceOp* op, void* formatterUse)
{
   (*op)->Complete( op, pfmErrorAccessDenied, 0/*openAttribs*/, 0);
}

void VolumeMove( PfmFormatterDispatch* vol_, PfmMarshallerMoveOp* op, void* formatterUse)
{
   (*op)->Complete( op, pfmErrorAccessDenied, 0/*existed*/, 0/*openAttribs*/, 0/*parentFileId*/, 0/*endName*/, 0, 0, 0, 0);
}

void VolumeMoveReplace( PfmFormatterDispatch* vol_, PfmMarshallerMoveReplaceOp* op, void* formatterUse)
{
   (*op)->Complete( op, pfmErrorAccessDenied);
}

void VolumeDelete( PfmFormatterDispatch* vol_, PfmMarshallerDeleteOp* op, void* formatterUse)
{
   (*op)->Complete( op, pfmErrorAccessDenied);
}

void VolumeClose( PfmFormatterDispatch* vol_, PfmMarshallerCloseOp* op, void* formatterUse)
{
   (*op)->Complete( op, pfmErrorSuccess);
}

void VolumeFlushFile( PfmFormatterDispatch* vol_, PfmMarshallerFlushFileOp* op, void* formatterUse)
{
   Volume* vol = Volume_vcast( vol_);
   int64_t openId = (*op)->OpenId( op);
   uint8_t fileFlags = (*op)->FileFlags( op);
   uint8_t color = (*op)->FileFlags( op);
   int64_t createTime = (*op)->CreateTime( op);
   int64_t writeTime = (*op)->WriteTime( op);
   int perr = 0;
   PfmOpenAttribs openAttribs = zeroOpenAttribs;

   if (fileFlags != pfmFileFlagsInvalid ||
      color != pfmColorInvalid ||
      createTime != pfmTimeInvalid ||
      writeTime != pfmTimeInvalid)
   {
      perr = pfmErrorAccessDenied;
   }
   else if (openId == vol->folderOpenId)
   {
      openAttribs.openId = vol->folderOpenId;
      openAttribs.openSequence = 1;
      openAttribs.accessLevel = pfmAccessLevelReadData;
      openAttribs.attribs.fileType = pfmFileTypeFolder;
      openAttribs.attribs.fileId = helloRootId;
   }
   else if (openId == vol->fileOpenId)
   {
      openAttribs.openId = vol->fileOpenId;
      openAttribs.openSequence = 1;
      openAttribs.accessLevel = pfmAccessLevelReadData;
      openAttribs.attribs.fileType = pfmFileTypeFile;
      openAttribs.attribs.fileId = helloFileId;
      openAttribs.attribs.fileSize = helloDataSize;
   }
   else
   {
      perr = pfmErrorNotFound;
   }

   (*op)->Complete( op, perr, &openAttribs, 0);
}

void VolumeList( PfmFormatterDispatch* vol_, PfmMarshallerListOp* op, void* formatterUse)
{
   Volume* vol = Volume_vcast( vol_);
   int64_t openId = (*op)->OpenId( op);
   int64_t listId = (*op)->ListId( op);
   int perr = 0;
   PfmAttribs attribs = zeroAttribs;

   if (openId != vol->folderOpenId)
   {
      perr = pfmErrorAccessDenied;
   }
   else
   {
      attribs.fileType = pfmFileTypeFile;
      attribs.fileId = helloFileId;
      attribs.fileSize = helloDataSize;
      (*op)->Add( op, &attribs, helloFileName);
   }

   (*op)->Complete( op, perr, 1/*noMore*/);
}

void VolumeListEnd( PfmFormatterDispatch* vol_, PfmMarshallerListEndOp* op, void* formatterUse)
{
   (*op)->Complete( op, pfmErrorSuccess);
}

void VolumeRead( PfmFormatterDispatch* vol_, PfmMarshallerReadOp* op, void* formatterUse)
{
   Volume* vol = Volume_vcast( vol_);
   int64_t openId = (*op)->OpenId( op);
   uint64_t fileOffset = (*op)->FileOffset( op);
   void* data = (*op)->Data( op);
   size_t requestedSize = (*op)->RequestedSize( op);
   int perr = 0;
   size_t actualSize = 0;

   if (openId != vol->fileOpenId)
   {
      perr = pfmErrorAccessDenied;
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
         actualSize = (size_t)(helloDataSize-fileOffset);
      }
      memcpy( data, helloData, actualSize);
   }

   (*op)->Complete( op, perr, actualSize);
}

void VolumeWrite( PfmFormatterDispatch* vol_, PfmMarshallerWriteOp* op, void* formatterUse)
{
   (*op)->Complete( op, pfmErrorAccessDenied, 0/*actualSize*/);
}

void VolumeSetSize( PfmFormatterDispatch* vol_, PfmMarshallerSetSizeOp* op, void* formatterUse)
{
   (*op)->Complete( op, pfmErrorAccessDenied);
}

void VolumeCapacity( PfmFormatterDispatch* vol_, PfmMarshallerCapacityOp* op, void* formatterUse)
{
   (*op)->Complete( op, pfmErrorSuccess, helloDataSize/*totalCapacity*/, 0/*availableCapacity*/);
}

void VolumeFlushMedia( PfmFormatterDispatch* vol_, PfmMarshallerFlushMediaOp* op, void* formatterUse)
{
   (*op)->Complete( op, pfmErrorSuccess, -1/*msecFlushDelay*/);
}

void VolumeControl( PfmFormatterDispatch* vol_, PfmMarshallerControlOp* op, void* formatteruse)
{
   (*op)->Complete( op, pfmErrorInvalid, 0/*outputSize*/);
}

void VolumeMediaInfo( PfmFormatterDispatch* vol_, PfmMarshallerMediaInfoOp* op, void* formatterUse)
{
   PfmMediaInfo mediaInfo = zeroMediaInfo;

   (*op)->Complete( op, pfmErrorSuccess, &mediaInfo, L"hellofs"/*mediaLabel*/);
}

void VolumeAccess( PfmFormatterDispatch* vol_, PfmMarshallerAccessOp* op, void* formatterUse)
{
   Volume* vol = Volume_vcast( vol_);
   int64_t openId = (*op)->OpenId( op);
   int perr = 0;
   PfmOpenAttribs openAttribs = zeroOpenAttribs;

   if (openId == vol->folderOpenId)
   {
      openAttribs.openId = vol->folderOpenId;
      openAttribs.openSequence = 1;
      openAttribs.accessLevel = pfmAccessLevelReadData;
      openAttribs.attribs.fileType = pfmFileTypeFolder;
      openAttribs.attribs.fileId = helloRootId;
   }
   else if (openId == vol->fileOpenId)
   {
      openAttribs.openId = vol->fileOpenId;
      openAttribs.openSequence = 1;
      openAttribs.accessLevel = pfmAccessLevelReadData;
      openAttribs.attribs.fileType = pfmFileTypeFile;
      openAttribs.attribs.fileId = helloFileId;
      openAttribs.attribs.fileSize = helloDataSize;
   }
   else
   {
      perr = pfmErrorNotFound;
   }

   (*op)->Complete( op, perr, &openAttribs, 0);
}

void VolumeReadXattr( PfmFormatterDispatch* vol_, PfmMarshallerReadXattrOp* op, void* formatterUse)
{
   (*op)->Complete( op, pfmErrorNotFound, 0/*xattrSize*/, 0/*transferredSize*/);
}

void VolumeWriteXattr( PfmFormatterDispatch* vol_, PfmMarshallerWriteXattrOp* op, void* formatterUse)
{
   (*op)->Complete( op, pfmErrorAccessDenied, 0/*transferredSize*/);
}

static PfmFormatterDispatch_vtbl_t volumeVtbl = {
   VolumeOpen       ,
   VolumeReplace    ,
   VolumeMove       ,
   VolumeMoveReplace,
   VolumeDelete     ,
   VolumeClose      ,
   VolumeFlushFile  ,
   VolumeList       ,
   VolumeListEnd    ,
   VolumeRead       ,
   VolumeWrite      ,
   VolumeSetSize    ,
   VolumeCapacity   ,
   VolumeFlushMedia ,
   VolumeControl    ,
   VolumeMediaInfo  ,
   VolumeAccess     ,
   VolumeReadXattr  ,
   VolumeWriteXattr ,
   };

static Volume zeroVolume = {0};

int wmain(int argc,const wchar_t*const* argv)
{
   int err = 0;
   PfmApi* pfm = 0;
   PfmMount* mount = 0;
   PfmMountCreateParams mcp;
   PfmMarshallerServeParams msp;
   Volume volume = zeroVolume;

   volume.vtbl = &volumeVtbl;
   PfmMountCreateParams_Init( &mcp);
   mcp.toFormatterWrite = FD_INVALID;
   mcp.fromFormatterRead = FD_INVALID;
   PfmMarshallerServeParams_Init( &msp);
   msp.dispatch = &volume.vtbl;
   msp.formatterName = "hellofs";
   msp.toFormatterRead = FD_INVALID;
   msp.fromFormatterWrite = FD_INVALID;

   if (argc <= 1)
   {
      printf(
         "Sample file system application.\n"
         "syntax: hellofs <mount name>\n");
      err = -1;
   }
   else
   {
      mcp.mountSourceName = argv[1];
   }
   if (!err)
   {
      err = PfmApiFactory( &pfm);
      if (err)
      {
         printf( "ERROR: %i Unable to open PFM Api.\n", err);
      }
   }

   if (!err)
   {
      err = PfmMarshallerFactory( &volume.marshaller);
      if (err)
      {
         printf( "ERROR: %i Unable to create marshaller.\n", err);
      }
   }
   if (!err)
   {
         // Communication between the driver and file system is done
         // over a pair of simple pipes. Application is responsible
         // for creating the pipes.
      err = create_pipe( &msp.toFormatterRead, &mcp.toFormatterWrite);
      if (!err)
      {
         err = create_pipe( &mcp.fromFormatterRead, &msp.fromFormatterWrite);
      }
   }
   if (!err)
   {
         // Various mount options are available through mountFlags,
         // visibleProcessId, and ownerSid.
      err = (*pfm)->MountCreate( pfm, &mcp, &mount);
      if (err)
      {
         printf( "ERROR: %i Unable to create mount.\n", err);
      }
   }
      // Close driver end pipe handles now. Driver has duplicated what
      // it needs. If these handles are not closed then pipes will not
      // break if driver disconnects, leaving us stuck in the
      // marshaller.
   close_fd( mcp.toFormatterWrite);
   close_fd( mcp.fromFormatterRead);

   if (!err)
   {
         // If tracemon is installed and running then diagnostic
         // messsages can be viewed in the "hellofs" channel.
      (*volume.marshaller)->SetTrace( volume.marshaller, L"hellofs");
         // Also send diagnostic messages to stdout. This can slow
         // things down quite a bit.
      (*volume.marshaller)->SetStatus( volume.marshaller, stdout_fd());

         // The marshaller uses alertable I/O, so process can be
         // exited via ctrl+c.
      printf( "Press CTRL+C to exit.\n");
         // The marshaller serve function will return at unmount or
         // if driver disconnects.
      (*volume.marshaller)->ServeDispatch( volume.marshaller, &msp);
   }

   close_fd( msp.toFormatterRead);
   close_fd( msp.fromFormatterWrite);
   if (mount)
   {
      (*mount)->Release( mount);
   }
   if (pfm)
   {
      (*pfm)->Release( pfm);
   }
   if (volume.marshaller)
   {
      (*volume.marshaller)->Release( volume.marshaller);
      volume.marshaller = 0;
   }
   PfmApiUnload();
   return err;
}
