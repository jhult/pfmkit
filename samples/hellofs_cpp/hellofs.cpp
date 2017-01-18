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
// file name:  hellofs.cpp
// created:    2006.10.06
//    Hello world file system sample application.
//----------------------------------------------------------------------------

#include "../common/portability.c"

#include "pfmapi.h"
#define CCALL PT_CCALL

static PfmOpenAttribs zeroOpenAttribs = {};
static PfmAttribs zeroAttribs = {};
static PfmMediaInfo zeroMediaInfo = {};

static const wchar_t helloFileName[] = L"readme.txt";
static const char helloData[] = "Hello world.\r\n";
static const size_t helloDataSize = sizeof( helloData)-sizeof( helloData[0]);
static const int64_t helloRootId = 2;
static const int64_t helloFileId = 3;

struct Volume: PfmFormatterDispatch
{
   PfmMarshaller* marshaller;
   int64_t folderOpenId;
   int64_t fileOpenId;

   Volume(void);
   ~Volume(void);

      // PfmFormatterDispatch
   void CCALL Open( PfmMarshallerOpenOp* op, void* formatterUse);
   void CCALL Replace( PfmMarshallerReplaceOp* op, void* formatterUse);
   void CCALL Move( PfmMarshallerMoveOp* op, void* formatterUse);
   void CCALL MoveReplace( PfmMarshallerMoveReplaceOp* op, void* formatterUse);
   void CCALL Delete( PfmMarshallerDeleteOp* op, void* formatterUse);
   void CCALL Close( PfmMarshallerCloseOp* op, void* formatterUse);
   void CCALL FlushFile( PfmMarshallerFlushFileOp* op, void* formatterUse);
   void CCALL List( PfmMarshallerListOp* op, void* formatterUse);
   void CCALL ListEnd( PfmMarshallerListEndOp* op, void* formatterUse);
   void CCALL Read( PfmMarshallerReadOp* op, void* formatterUse);
   void CCALL Write( PfmMarshallerWriteOp* op, void* formatterUse);
   void CCALL SetSize( PfmMarshallerSetSizeOp* op, void* formatterUse);
   void CCALL Capacity( PfmMarshallerCapacityOp* op, void* formatterUse);
   void CCALL FlushMedia( PfmMarshallerFlushMediaOp* op, void* formatterUse);
   void CCALL Control( PfmMarshallerControlOp* op, void* formatterUse);
   void CCALL MediaInfo( PfmMarshallerMediaInfoOp* op, void* formatterUse);
   void CCALL Access( PfmMarshallerAccessOp* op, void* formatterUse);
   void CCALL ReadXattr( PfmMarshallerReadXattrOp* op, void* formatterUse);
   void CCALL WriteXattr( PfmMarshallerWriteXattrOp* op, void* formatterUse);
};

void CCALL Volume::Open( PfmMarshallerOpenOp* op, void* formatterUse)
{
   const PfmNamePart* nameParts = op->NameParts();
   size_t namePartCount = op->NamePartCount();
   int8_t createFileType = op->CreateFileType();
   int64_t newExistingOpenId = op->NewExistingOpenId();
   int perr = 0;
   bool existed = false;
   PfmOpenAttribs openAttribs = zeroOpenAttribs;
   int64_t parentFileId = 0;
   const wchar_t* endName = 0;

   if (!namePartCount)
   {
      if (!folderOpenId)
      {
         folderOpenId = newExistingOpenId;
      }
      existed = true;
      openAttribs.openId = folderOpenId;
      openAttribs.openSequence = 1;
      openAttribs.accessLevel = pfmAccessLevelReadData;
      openAttribs.attribs.fileType = pfmFileTypeFolder;
      openAttribs.attribs.fileId = helloRootId;
   }
   else if (namePartCount == 1)
   {
      if (sswcmpf(nameParts[0].name,helloFileName) != 0)
      {
         perr = pfmErrorNotFound;
      }
      else
      {
         if (!fileOpenId)
         {
            fileOpenId = newExistingOpenId;
         }
         existed = true;
         openAttribs.openId = fileOpenId;
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

   op->Complete( perr, existed, &openAttribs, parentFileId, endName, 0, 0, 0, 0);
}

void CCALL Volume::Replace( PfmMarshallerReplaceOp* op, void* formatterUse)
{
   op->Complete( pfmErrorAccessDenied, 0/*openAttribs*/, 0);
}

void CCALL Volume::Move( PfmMarshallerMoveOp* op, void* formatterUse)
{
   op->Complete( pfmErrorAccessDenied, 0/*existed*/, 0/*openAttribs*/, 0/*parentFileId*/, 0/*endName*/, 0, 0, 0, 0);
}

void CCALL Volume::MoveReplace( PfmMarshallerMoveReplaceOp* op, void* formatterUse)
{
   op->Complete( pfmErrorAccessDenied);
}

void CCALL Volume::Delete( PfmMarshallerDeleteOp* op, void* formatterUse)
{
   op->Complete( pfmErrorAccessDenied);
}

void CCALL Volume::Close( PfmMarshallerCloseOp* op, void* formatterUse)
{
   op->Complete( pfmErrorSuccess);
}

void CCALL Volume::FlushFile( PfmMarshallerFlushFileOp* op, void* formatterUse)
{
   int64_t openId = op->OpenId();
   uint8_t fileFlags = op->FileFlags();
   uint8_t color = op->FileFlags();
   int64_t createTime = op->CreateTime();
   int64_t writeTime = op->WriteTime();
   int perr = 0;
   PfmOpenAttribs openAttribs = zeroOpenAttribs;

   if (fileFlags != pfmFileFlagsInvalid ||
      color != pfmColorInvalid ||
      createTime != pfmTimeInvalid ||
      writeTime != pfmTimeInvalid)
   {
      perr = pfmErrorAccessDenied;
   }
   else if (openId == folderOpenId)
   {
      openAttribs.openId = folderOpenId;
      openAttribs.openSequence = 1;
      openAttribs.accessLevel = pfmAccessLevelReadData;
      openAttribs.attribs.fileType = pfmFileTypeFolder;
      openAttribs.attribs.fileId = helloRootId;
   }
   else if (openId == fileOpenId)
   {
      openAttribs.openId = fileOpenId;
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

   op->Complete( perr, &openAttribs, 0);
}

void CCALL Volume::List( PfmMarshallerListOp* op, void* formatterUse)
{
   int64_t openId = op->OpenId();
   int perr = 0;
   PfmAttribs attribs = zeroAttribs;

   if(openId != folderOpenId)
   {
      perr = pfmErrorAccessDenied;
   }
   else
   {
      attribs.fileType = pfmFileTypeFile;
      attribs.fileId = helloFileId;
      attribs.fileSize = helloDataSize;
      op->Add( &attribs, helloFileName);
   }

   op->Complete( perr, true/*noMore*/);
}

void CCALL Volume::ListEnd( PfmMarshallerListEndOp* op, void* formatterUse)
{
   op->Complete( pfmErrorSuccess);
}

void CCALL Volume::Read( PfmMarshallerReadOp* op, void* formatterUse)
{
   int64_t openId = op->OpenId();
   uint64_t fileOffset = op->FileOffset();
   void* data = op->Data();
   size_t requestedSize = op->RequestedSize();
   int perr = 0;
   size_t actualSize = 0;

   if(openId != fileOpenId)
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
         actualSize = static_cast<size_t>(helloDataSize-fileOffset);
      }
      memcpy( data, helloData, actualSize);
   }

   op->Complete( perr, actualSize);
}

void CCALL Volume::Write( PfmMarshallerWriteOp* op, void* formatterUse)
{
   op->Complete( pfmErrorAccessDenied, 0/*actualSize*/);
}

void CCALL Volume::SetSize( PfmMarshallerSetSizeOp* op, void* formatterUse)
{
   op->Complete( pfmErrorAccessDenied);
}

void CCALL Volume::Capacity( PfmMarshallerCapacityOp* op, void* formatterUse)
{
   op->Complete( pfmErrorSuccess, helloDataSize/*totalCapacity*/, 0/*availableCapacity*/);
}

void CCALL Volume::FlushMedia( PfmMarshallerFlushMediaOp* op, void* formatterUse)
{
   op->Complete( pfmErrorSuccess, -1/*msecFlushDelay*/);
}

void CCALL Volume::Control( PfmMarshallerControlOp* op, void* formatterUse)
{
   op->Complete( pfmErrorInvalid, 0/*outputSize*/);
}

void CCALL Volume::MediaInfo( PfmMarshallerMediaInfoOp* op, void* formatterUse)
{
   PfmMediaInfo mediaInfo = zeroMediaInfo;

   op->Complete( pfmErrorSuccess, &mediaInfo, L"hellofs"/*mediaLabel*/);
}

void CCALL Volume::Access( PfmMarshallerAccessOp* op, void* formatterUse)
{
   int64_t openId = op->OpenId();
   int perr = 0;
   PfmOpenAttribs openAttribs = zeroOpenAttribs;

   if(openId == folderOpenId)
   {
      openAttribs.openId = folderOpenId;
      openAttribs.openSequence = 1;
      openAttribs.accessLevel = pfmAccessLevelReadData;
      openAttribs.attribs.fileType = pfmFileTypeFolder;
      openAttribs.attribs.fileId = helloRootId;
      perr = 0;
   }
   else if(openId == fileOpenId)
   {
      openAttribs.openId = fileOpenId;
      openAttribs.openSequence = 1;
      openAttribs.accessLevel = pfmAccessLevelReadData;
      openAttribs.attribs.fileType = pfmFileTypeFile;
      openAttribs.attribs.fileId = helloFileId;
      openAttribs.attribs.fileSize = helloDataSize;
      perr = 0;
   }
   else
   {
      perr = pfmErrorNotFound;
   }

   op->Complete( perr, &openAttribs, 0);
}

void CCALL Volume::ReadXattr( PfmMarshallerReadXattrOp* op, void* formatterUse)
{
   op->Complete( pfmErrorNotFound, 0/*xattrSize*/, 0/*transferredSize*/);
}

void CCALL Volume::WriteXattr( PfmMarshallerWriteXattrOp* op, void* formatterUse)
{
   op->Complete( pfmErrorAccessDenied, 0/*transferredSize*/);
}

Volume::Volume(void)
{
   marshaller = 0;
   folderOpenId = 0;
   fileOpenId = 0;
}

Volume::~Volume(void)
{
   ASSERT(!marshaller);
}

int wmain(int argc,const wchar_t*const* argv)
{
   int err = 0;
   PfmApi* pfm = 0;
   PfmMount* mount = 0;
   FD_T toFormatterRead = FD_INVALID;
   FD_T fromFormatterWrite = FD_INVALID;
   PfmMountCreateParams mcp;
   PfmMarshallerServeParams msp;
   Volume volume;

   mcp.toFormatterWrite = FD_INVALID;
   mcp.fromFormatterRead = FD_INVALID;
   msp.dispatch = &volume;
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
      err = pfm->MountCreate( &mcp, &mount);
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
      volume.marshaller->SetTrace( L"hellofs");
         // Also send diagnostic messages to stdout. This can slow
         // things down quite a bit.
      volume.marshaller->SetStatus( stdout_fd());

         // The marshaller uses alertable I/O, so process can be
         // exited via ctrl+c.
      printf( "Press CTRL+C to exit.\n");
         // The marshaller serve function will return at unmount or
         // if driver disconnects.
      volume.marshaller->ServeDispatch( &msp);
   }

   close_fd( msp.toFormatterRead);
   close_fd( msp.fromFormatterWrite);
   if (mount)
   {
      mount->Release();
   }
   if (pfm)
   {
      pfm->Release();
   }
   if (volume.marshaller)
   {
      volume.marshaller->Release();
      volume.marshaller = 0;
   }
   PfmApiUnload();
   return err;
}
