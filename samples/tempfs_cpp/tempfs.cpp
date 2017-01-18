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
// file name:  tempfs.cpp
// created:    2006.12.20
//    Temporary file system application sample. Demonstrates
//    implementing a full read+write file system in a standalone
//    application.
//----------------------------------------------------------------------------

#include "../common/portability.c"

#include "pfmapi.h"
#define CCALL PT_CCALL

static PfmOpenAttribs zeroOpenAttribs = {};
static PfmAttribs zeroAttribs = {};
static PfmMediaInfo zeroMediaInfo = {};

struct File;

struct FileList
{
   File* file;
   FileList** prev;
   FileList* next;
   int64_t listId;
   File** position;

   void Iterate( PfmMarshallerListOp* op, bool* noMore);

   FileList( File* file, int64_t listId);
   ~FileList( void);
};

struct Volume;

struct File
{
   Volume* volume;
   File* parent;
   File** sibPrev;
   File* sibNext;
   File** openPrev;
   File* openNext;
   int64_t openId;
   int64_t openSequence;
   wchar_t* name;
   int8_t fileType;
   uint8_t fileFlags;
   int64_t fileId;
   int64_t createTime;
   int64_t accessTime;
   int64_t writeTime;
   int64_t changeTime;
   static const size_t blockSize = 16384;
   static const uint64_t maxFileSize = static_cast<uint64_t>( blockSize)*INT_MAX;
   union
   {
         // fileType == pfmFileTypeFile
      struct
      {
         uint64_t fileSize;
         size_t blockCount;
         uint8_t** blockList;
      } file;
         // fileType == pfmFileTypeFolder
      struct
      {
         File* firstChild;
         FileList* firstList;
      } folder;
   } data;

#ifdef DEBUG
   void CheckConsistency( void);
#else
   void CheckConsistency( void) { ; }
#endif
   void Opened( PfmOpenAttribs* openAttribs);
   void Open( int64_t newOpenId, PfmOpenAttribs* openAttribs, int64_t* parentFileId, const wchar_t** endName);
   void Close( int64_t openSequence);
   void Delete( int64_t writeTime, bool leaveChildren);
   int/*perr*/ Move( File* parent, File** sibPrev, const wchar_t* name, int64_t writeTime);
   void Flush( uint8_t fileFlags, int64_t createTime, int64_t accessTime, int64_t writeTime, int64_t changeTime);
   int/*perr*/ Read( uint64_t fileOffset, void* data, size_t requestedSize, size_t* outActualSize);
   int/*perr*/ PrepSetSize( uint64_t newFileSize);
   int/*perr*/ Write( uint64_t fileOffset, const void* data, size_t requestedSize, size_t* outActualSize);
   int/*perr*/ SetSize( uint64_t fileSize);
   int/*perr*/ ListFactory( int64_t listId, FileList** outList);
   int/*perr*/ ListEnd( int64_t listId);

   File( Volume* volume);
   ~File( void);
   int/*perr*/ Init( File* parent, File** sibPrev, const wchar_t* name, int8_t fileType, uint8_t fileFlags, int64_t writeTime);
};

struct Volume: PfmFormatterDispatch
{
   PfmMarshaller* marshaller;
   uint64_t capacity;
   int64_t lastFileId;
   File root;
   File* firstOpenFile;

#ifdef DEBUG
   void CheckConsistency( void);
#else
   void CheckConsistency( void) { ; }
#endif
   int/*perr*/ FileFactory( File* parent, File** sibPrev, const wchar_t* name, int8_t createFileType, uint8_t createFileFlags, int64_t writeTime, File** file);
   int/*perr*/ FindFile( const PfmNamePart* nameParts, size_t namePartCount, File** file, File** parent, File*** sibPrev);
   int/*perr*/ FindOpenFile( int64_t openId, File** file);

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

   Volume( void);
   ~Volume( void);
};

#ifdef DEBUG

void File::CheckConsistency( void)
{
   if (this == &volume->root)
   {
      ASSERT( !sibPrev);
   }
   else
   {
      ASSERT( sibPrev);
      ASSERT( *sibPrev == this);
      if (sibNext)
      {
         ASSERT( sibNext->sibPrev == &sibNext);
      }
   }
   if (openPrev)
   {
      ASSERT( *openPrev == this);
      if (openNext)
      {
         ASSERT( openNext->openPrev == &openNext);
      }
   }
   if (fileType == pfmFileTypeFolder)
   {
      for (File* child = data.folder.firstChild; !!child; child = child->sibNext)
      {
         ASSERT( child->parent == this);
         child->CheckConsistency();
      }
   }
}

void Volume::CheckConsistency(void)
{
   if (firstOpenFile)
   {
      ASSERT( firstOpenFile->openPrev == &firstOpenFile);
   }
   root.CheckConsistency();
}

#endif

void File::Opened( PfmOpenAttribs* openAttribs)
{
   openSequence ++;
   openAttribs->openId = openId;
   openAttribs->openSequence = openSequence;
   openAttribs->accessLevel = pfmAccessLevelWriteData;
   openAttribs->attribs.fileType = fileType;
   openAttribs->attribs.fileFlags = fileFlags;
   openAttribs->attribs.fileId = fileId;
   openAttribs->attribs.fileSize = 0;
   if (fileType == pfmFileTypeFile)
   {
      openAttribs->attribs.fileSize = data.file.fileSize;
   }
   openAttribs->attribs.createTime = createTime;
   openAttribs->attribs.accessTime = accessTime;
   openAttribs->attribs.writeTime = writeTime;
   openAttribs->attribs.changeTime = changeTime;
}

void File::Open( int64_t newOpenId, PfmOpenAttribs* openAttribs, int64_t* parentFileId, const wchar_t** endName)
{
   if (!openPrev)
   {
      if (!openId)
      {
         openId = newOpenId;
      }
      openPrev = &volume->firstOpenFile;
      openNext = *openPrev;
      if (openNext)
      {
         ASSERT( openNext->openPrev == openPrev);
         openNext->openPrev = &openNext;
      }
      *openPrev = this;
      volume->CheckConsistency();
   }
   Opened( openAttribs);
   if (parentFileId && parent)
   {
      *parentFileId = parent->fileId;
   }
   if (endName && name)
   {
      *endName = name;
   }
}

void File::Flush( uint8_t newFileFlags, int64_t newCreateTime, int64_t newAccessTime, int64_t newWriteTime, int64_t newChangeTime)
{
   if (newFileFlags != pfmFileFlagsInvalid)
   {
      fileFlags = newFileFlags;
   }
   if (newCreateTime != pfmTimeInvalid)
   {
      createTime = newCreateTime;
   }
   if (newAccessTime != pfmTimeInvalid)
   {
      accessTime = newAccessTime;
   }
   if (newWriteTime != pfmTimeInvalid)
   {
      writeTime = newWriteTime;
   }
   if (newChangeTime != pfmTimeInvalid)
   {
      changeTime = newChangeTime;
   }
}

int/*perr*/ File::Read( uint64_t fileOffset, void* inBuffer, size_t requestedSize, size_t* outActualSize)
{
   int perr = pfmErrorInvalid;
   uint64_t startOffset = fileOffset;
   uint64_t endOffset = fileOffset;
   uint8_t* buffer = static_cast<uint8_t*>(inBuffer);
   if (fileType == pfmFileTypeFile)
   {
      perr = 0;
      if (fileOffset < data.file.fileSize)
      {
         endOffset = fileOffset+requestedSize;
         if (endOffset < startOffset || endOffset > data.file.fileSize)
         {
            endOffset = data.file.fileSize;
         }
      }
      uint64_t offset = startOffset;
      while (offset < endOffset)
      {
         size_t blockIndex = static_cast<size_t>( offset/blockSize);
         size_t blockOffset = static_cast<size_t>( offset%blockSize);
         size_t partSize = blockSize-blockOffset;
         if (offset+partSize > endOffset)
         {
            partSize = static_cast<size_t>( endOffset-offset);
         }
         uint8_t* block = data.file.blockList[blockIndex];
         if (block)
         {
            memcpy( buffer+static_cast<size_t>( offset-startOffset), block+blockOffset, partSize);
         }
         else
         {
            memset( buffer+static_cast<size_t>( offset-startOffset), 0, partSize);
         }
         offset += partSize;
      }
   }
   *outActualSize = static_cast<size_t>( endOffset-startOffset);
   return perr;
}

int/*perr*/ File::PrepSetSize( uint64_t newFileSize)
{
   ASSERT( fileType == pfmFileTypeFile);
   int perr = 0;
   if (newFileSize > maxFileSize)
   {
      perr = pfmErrorNoSpace;
   }
   else
   {
      size_t newBlockCount = static_cast<size_t>( (newFileSize+blockSize-1)/blockSize);
      if (newBlockCount > data.file.blockCount)
      {
         newBlockCount += newBlockCount/4+4;
         uint8_t** newBlockList = static_cast<uint8_t**>( malloc( newBlockCount*sizeof( newBlockList[ 0])));
         if (!newBlockList)
         {
            perr = pfmErrorNoSpace;
         }
         else
         {
            if (data.file.blockList)
            {
               memcpy( newBlockList, data.file.blockList, data.file.blockCount*sizeof( newBlockList[ 0]));
               free( data.file.blockList);
            }
            memset( newBlockList+data.file.blockCount, 0, (newBlockCount-data.file.blockCount)*sizeof( newBlockList[ 0]));
            data.file.blockList = newBlockList;
            data.file.blockCount = newBlockCount;
         }
      }
   }
   return perr;
}

int/*perr*/ File::Write( uint64_t fileOffset, const void* inBuffer, size_t requestedSize, size_t* outActualSize)
{
   int perr = pfmErrorInvalid;
   uint64_t startOffset = fileOffset;
   uint64_t endOffset = fileOffset;
   const uint8_t* buffer = static_cast<const uint8_t*>( inBuffer);
   if (fileType == pfmFileTypeFile)
   {
      perr = 0;
      if (startOffset < maxFileSize)
      {
         endOffset = fileOffset+requestedSize;
         if (endOffset > maxFileSize)
         {
            endOffset = maxFileSize;
         }
         if (PrepSetSize( endOffset) != 0)
         {
            endOffset = startOffset;
         }
      }
      uint64_t offset = startOffset;
      while (offset < endOffset)
      {
         size_t blockIndex = static_cast<size_t>( offset/blockSize);
         size_t blockOffset = static_cast<size_t>( offset%blockSize);
         size_t partSize = blockSize-blockOffset;
         if (offset+partSize > endOffset)
         {
            partSize = static_cast<size_t>( endOffset-offset);
         }
         uint8_t* block = data.file.blockList[blockIndex];
         if (!block)
         {
            block = data.file.blockList[ blockIndex] = static_cast<uint8_t*>( malloc( blockSize));
            if (block)
            {
               volume->capacity += blockSize;
               if (partSize != blockSize)
               {
                  memset( block, 0, blockSize);
               }
            }
         }
         if (!block)
         {
            endOffset = offset;
         }
         else
         {
            memcpy( block+blockOffset, buffer+static_cast<size_t>( offset-startOffset), partSize);
            offset += partSize;
            if (offset > data.file.fileSize)
            {
               data.file.fileSize = offset;
            }
         }
      }
   }
   *outActualSize = static_cast<size_t>( endOffset-startOffset);
   return perr;
}

int/*perr*/ File::SetSize( uint64_t newFileSize)
{
   int perr = pfmErrorInvalid;
   if (fileType == pfmFileTypeFile)
   {
      perr = PrepSetSize( newFileSize);
      if (!perr)
      {
         if (newFileSize < data.file.fileSize)
         {
               // Zero from new file size to lesser of old file size or the
               // end of the will be end block.
            uint8_t* block;
            size_t blockIndex = static_cast<size_t>( (newFileSize+blockSize-1)/blockSize);
            uint64_t zeroOffset = static_cast<uint64_t>( blockIndex)*blockSize;
            ASSERT( zeroOffset-newFileSize < blockSize);
            if (zeroOffset > newFileSize)
            {
               block = data.file.blockList[blockIndex-1];
               if (block)
               {
                  memset( block+static_cast<size_t>(newFileSize%blockSize), 0, static_cast<size_t>( zeroOffset-newFileSize));
               }
            }
               // Free any blocks past the new end block.
            size_t blockCount = static_cast<size_t>( (data.file.fileSize+blockSize-1)/blockSize);
            while (blockIndex < blockCount)
            {
               block = data.file.blockList[ blockIndex];
               if (block)
               {
                  ASSERT( volume->capacity >= blockSize);
                  volume->capacity -= blockSize;
                  free( block);
                  data.file.blockList[ blockIndex] = 0;
               }
               blockIndex ++;
            }
         }
         data.file.fileSize = newFileSize;
      }
   }
   return perr;
}

void FileList::Iterate( PfmMarshallerListOp* op, bool* noMore)
{
   PfmAttribs attribs;
   memset( &attribs, 0, sizeof( attribs));
   for (;;)
   {
      File* file = *position;
      if (!file)
      {
         *noMore = true;
         break;
      }
      else
      {
         attribs.fileType = file->fileType;
         attribs.fileFlags = file->fileFlags;
         attribs.fileId = file->fileId;
         if (attribs.fileType == pfmFileTypeFile)
         {
            attribs.fileSize = file->data.file.fileSize;
         }
         attribs.createTime = file->createTime;
         attribs.accessTime = file->accessTime;
         attribs.writeTime = file->writeTime;
         attribs.changeTime = file->changeTime;
         if (op->Add( &attribs, file->name))
         {
            position = &file->sibNext;
            continue;
         }
      }
      break;
   }
}

FileList::FileList( File* inFile, int64_t inListId)
{
   file = inFile;
   ASSERT( file->fileType == pfmFileTypeFolder);
   prev = &file->data.folder.firstList;
   next = *prev;
   if (next)
   {
      ASSERT( next->prev == prev);
      next->prev = &next;
   }
   *prev = this;
   listId = inListId;
   position = &file->data.folder.firstChild;
}

FileList::~FileList(void)
{
   ASSERT( prev && *prev == this);
   *prev = next;
   if (next)
   {
      ASSERT( next->prev == &next);
      next->prev = prev;
   }
}

int/*perr*/ File::ListFactory( int64_t listId, FileList** outList)
{
   int perr = 0;
   FileList* list = 0;
   if (fileType != pfmFileTypeFolder)
   {
      perr = pfmErrorAccessDenied;
   }
   else
   {
      list = data.folder.firstList;
      while (list && list->listId != listId)
      {
         list = list->next;
      }
      if (!list)
      {
         list = new FileList( this, listId);
         if (!list)
         {
            perr = pfmErrorOutOfMemory;
         }
      }
   }
   *outList = list;
   return perr;
}

int/*perr*/ File::ListEnd( int64_t listId)
{
   int perr = pfmErrorNotFound;
   if (fileType == pfmFileTypeFolder)
   {
      FileList* list = data.folder.firstList;
      while (list && list->listId != listId)
      {
         list = list->next;
      }
      if (list)
      {
         perr = 0;
         delete list;
      }
   }
   return perr;
}

File::File( Volume* inVolume)
{
   volume = inVolume;
   parent = 0;
   sibPrev = 0;
   openPrev = 0;
   openId = 0;
   openSequence = 0;
   name = 0;
   fileType = 0;
   fileFlags = 0;
   memset( &data, 0, sizeof( data));
}

File::~File(void)
{
   ASSERT( !sibPrev && !openPrev);
   free( name);
   switch (fileType)
   {
   case pfmFileTypeFile:
      if (data.file.blockList)
      {
         for (size_t blockIndex = 0; blockIndex < data.file.blockCount; blockIndex ++)
         {
            uint8_t* block = data.file.blockList[ blockIndex];
            if (block)
            {
               ASSERT( volume->capacity >= blockSize);
               volume->capacity -= blockSize;
               free( block);
            }
         }
         free( data.file.blockList);
      }
      break;
   case pfmFileTypeFolder:
      ASSERT( !data.folder.firstChild);
      while (data.folder.firstList)
      {
         delete data.folder.firstList;
      }
      break;
   }
}

int/*perr*/ File::Init( File* inParent, File** inSibPrev, const wchar_t* inName, int8_t inFileType, uint8_t inFileFlags, int64_t inWriteTime)
{
   name = sswdup( inName);
   int perr = pfmErrorOutOfMemory;
   if (name)
   {
      perr = 0;
      parent = inParent;
      sibPrev = inSibPrev;
      sibNext = *sibPrev;
      if (sibNext)
      {
         ASSERT( sibNext->parent == parent && sibNext->sibPrev == sibPrev);
         sibNext->sibPrev = &sibNext;
      }
      *sibPrev = this;
      volume->CheckConsistency();
      fileType = inFileType;
      fileFlags = inFileFlags;
      fileId = ++(volume->lastFileId);
      createTime = accessTime = writeTime = changeTime = inWriteTime;
      parent->accessTime = parent->writeTime = parent->changeTime = inWriteTime;
   }
   return perr;
}

void File::Close( int64_t checkOpenSequence)
{
   if (openPrev && checkOpenSequence >= openSequence)
   {
      ASSERT( *openPrev == this);
      *openPrev = openNext;
      if (openNext)
      {
         ASSERT( openNext->openPrev == &openNext);
         openNext->openPrev = openPrev;
      }
      openPrev = 0;
      volume->CheckConsistency();
   }
   if (!sibPrev && !openPrev && this != &volume->root)
   {
      delete this;
   }
}

void File::Delete( int64_t writeTime, bool leaveChildren)
{
   if (sibPrev)
   {
      if (!leaveChildren && fileType == pfmFileTypeFolder)
      {
         while (data.folder.firstChild)
         {
            data.folder.firstChild->Delete( writeTime, false/*leaveChildren*/);
         }
      }
         // Make sure no list iterators are referencing this file.
      for (FileList* list = parent->data.folder.firstList; !!list; list = list->next)
      {
         if (list->position == &sibNext)
         {
            list->position = sibPrev;
         }
      }
      ASSERT( *sibPrev == this);
      *sibPrev = sibNext;
      if (sibNext)
      {
         ASSERT( sibNext->sibPrev == &sibNext);
         sibNext->sibPrev = sibPrev;
      }
      if (writeTime != pfmTimeInvalid)
      {
         parent->writeTime = writeTime;
      }
      parent = 0;
      sibPrev = 0;
      volume->CheckConsistency();
      free( name);
      name = 0;
   }
   if (!sibPrev && !openPrev && this != &(volume->root))
   {
      delete this;
   }
}

int/*perr*/ File::Move( File* inParent, File** inSibPrev, const wchar_t* inNewName, int64_t writeTime)
{
   int perr = 0;
   File* check = parent;
   while (check && check != this)
   {
      check = check->parent;
   }
   if (check)
   {
         // Can't move a folder into itself.
      perr = pfmErrorInvalid;
   }
   else
   {
      wchar_t* newName = sswdup( inNewName);
      if (!newName)
      {
         perr = pfmErrorOutOfMemory;
      }
      else if (inParent == parent)
      {
            // Simple rename, not changing parent folders.
         free( name);
         name = newName;
      }
      else
      {
         Delete( writeTime, true/*leaveChildren*/);
         name = newName;
         parent = inParent;
         sibPrev = inSibPrev;
         sibNext = *sibPrev;
         if (sibNext)
         {
            ASSERT( sibNext->parent == parent && sibNext->sibPrev == sibPrev);
            sibNext->sibPrev = &sibNext;
         }
         *sibPrev = this;
         volume->CheckConsistency();
      }
      if (!perr && writeTime != pfmTimeInvalid)
      {
         parent->writeTime = writeTime;
      }
   }
   return perr;
}

int/*perr*/ Volume::FileFactory( File* parent, File** sibPrev, const wchar_t* name, int8_t fileType, uint8_t fileFlags, int64_t writeTime, File** outFile)
{
   ASSERT( parent && sibPrev && name);
   File* file = new File( this);
   int perr = pfmErrorOutOfMemory;
   if (file)
   {
      perr = file->Init( parent, sibPrev, name, fileType, fileFlags, writeTime);
      if (perr)
      {
         delete file;
         file = 0;
      }
   }
   *outFile = file;
   return perr;
}

int/*perr*/ Volume::FindFile( const PfmNamePart* nameParts, size_t namePartCount, File** outFile, File** outParent, File*** outSibPrev)
{
   int perr = 0;
   File* file = &root;
   File* parent = 0;
   File** sibPrev = 0;
   size_t namePartIndex = 0;
   while (file && namePartIndex < namePartCount && file->fileType == pfmFileTypeFolder)
   {
      parent = file;
      sibPrev = &file->data.folder.firstChild;
      const wchar_t* name = nameParts[ namePartIndex].name;
      while ((file = *sibPrev) != 0 && sswcmpf( name, file->name) != 0)
      {
         sibPrev = &file->sibNext;
      }
      namePartIndex ++;
   }
   if (namePartIndex < namePartCount)
   {
      perr = pfmErrorParentNotFound;
   }
   *outFile = file;
   *outParent = parent;
   *outSibPrev = sibPrev;
   return perr;
}

int/*perr*/ Volume::FindOpenFile( int64_t openId, File** outFile)
{
   int perr = 0;
   File* file = firstOpenFile;
   while (file && file->openId != openId)
   {
      file = file->openNext;
   }
   if (!file)
   {
      perr = pfmErrorInvalid;
   }
   *outFile = file;
   return perr;
}

   // PfmFormatterDispatch::Open
void CCALL Volume::Open( PfmMarshallerOpenOp* op, void* formatterUse)
{
   const PfmNamePart* nameParts = op->NameParts();
   size_t namePartCount = op->NamePartCount();
   int8_t createFileType = op->CreateFileType();
   int8_t createFileFlags = op->CreateFileFlags();
   int64_t writeTime = op->WriteTime();
   int64_t newCreateOpenId = op->NewCreateOpenId();
   int64_t newExistingOpenId = op->NewExistingOpenId();
   int perr;
   bool existed = false;
   PfmOpenAttribs openAttribs = zeroOpenAttribs;
   int64_t parentFileId = 0;
   const wchar_t* endName = 0;
   File* file;
   File* parent;
   File** sibPrev;

   perr = FindFile( nameParts, namePartCount, &file, &parent, &sibPrev);
   if (!perr)
   {
      if (file)
      {
         existed = true;
         file->Open( newExistingOpenId, &openAttribs, &parentFileId, &endName);
      }
      else
      {
            // Name must have 1 or more parts or would have found root.
         ASSERT( namePartCount && parent && sibPrev);
         if (createFileType == pfmFileTypeNone)
         {
            perr = pfmErrorNotFound;
         }
         else
         {
            perr = FileFactory( parent, sibPrev, nameParts[ namePartCount-1].name, createFileType, createFileFlags, writeTime, &file);
            if (!perr)
            {
               file->Open( newCreateOpenId, &openAttribs, &parentFileId, &endName);
            }
         }
      }
   }

   op->Complete( perr, existed, &openAttribs, parentFileId, endName, 0, 0, 0, 0);
}

   // PfmFormatterDispatch::Replace
void CCALL Volume::Replace( PfmMarshallerReplaceOp* op, void* formatterUse)
{
   int64_t targetOpenId = op->TargetOpenId();
   int64_t targetParentFileId = op->TargetParentFileId();
   const PfmNamePart* targetEndName = op->TargetEndName();
   uint8_t createFileFlags = op->CreateFileFlags();
   int64_t writeTime = op->WriteTime();
   int64_t newCreateOpenId = op->NewCreateOpenId();
   int perr;
   PfmOpenAttribs openAttribs = zeroOpenAttribs;
   File* target;
   File* file;

   perr = FindOpenFile( targetOpenId, &target);
   if (!perr)
   {
      if (target == &root)
      {
            // Can't replace root.
         perr = pfmErrorAccessDenied;
      }
      else if (!target->name)
      {
         perr = pfmErrorDeleted;
      }
      else
      {
         perr = FileFactory( target->parent, target->sibPrev, target->name, target->fileType, createFileFlags, writeTime, &file);
         if (!perr)
         {
            target->Delete( writeTime, false/*leaveChildren*/);
            file->Open( newCreateOpenId, &openAttribs, 0, 0);
         }
      }
   }

   op->Complete( perr, &openAttribs, 0);
}

   // PfmFormatterDispatch::Move
void CCALL Volume::Move( PfmMarshallerMoveOp* op, void* formatterUse)
{
   int64_t sourceOpenId = op->SourceOpenId();
   int64_t sourceParentFileId = op->SourceParentFileId();
   const PfmNamePart* sourceEndName = op->SourceEndName();
   const PfmNamePart* targetNameParts = op->TargetNameParts();
   size_t targetNamePartCount = op->TargetNamePartCount();
   bool deleteSource = !!op->DeleteSource();
   int64_t writeTime = op->WriteTime();
   int8_t existingAccessLevel = op->ExistingAccessLevel();
   int64_t newExistingOpenId = op->NewExistingOpenId();
   int perr;
   bool existed = false;
   PfmOpenAttribs openAttribs = zeroOpenAttribs;
   int64_t parentFileId = 0;
   const wchar_t* endName = 0;
   File* file;
   File* target;
   File* parent;
   File** sibPrev;

   perr = FindOpenFile( sourceOpenId, &file);
   if (!perr)
   {
      perr = FindFile( targetNameParts, targetNamePartCount, &target, &parent, &sibPrev);
      if (!perr)
      {
            // Watch for and allow case change rename. ("FILE.TXT" -> "File.txt")
         if (target && (!targetNamePartCount || target != file))
         {
            existed = true;
            target->Open( newExistingOpenId, &openAttribs, &parentFileId, &endName);
         }
         else if (file->sibPrev && !deleteSource)
         {
               // Links are not supported.
            perr = pfmErrorInvalid;
         }
         else
         {
               // Target name must have 1 or more parts or would have found root.
            ASSERT( targetNamePartCount && parent && sibPrev);
            perr = file->Move( parent, sibPrev, targetNameParts[ targetNamePartCount-1].name, writeTime);
            if (!perr)
            {
               file->Open( sourceOpenId, &openAttribs, &parentFileId, &endName);
            }
         }
      }
   }

   op->Complete( perr, existed, &openAttribs, parentFileId, endName, 0, 0, 0, 0);
}

   // PfmFormatterDispatch::MoveReplace
void CCALL Volume::MoveReplace( PfmMarshallerMoveReplaceOp* op, void* formatterUse)
{
   int64_t sourceOpenId = op->SourceOpenId();
   int64_t sourceParentFileId = op->SourceParentFileId();
   const PfmNamePart* sourceEndName = op->SourceEndName();
   int64_t targetOpenId = op->TargetOpenId();
   int64_t targetParentFileId = op->TargetParentFileId();
   const PfmNamePart* targetEndName = op->TargetEndName();
   uint8_t/*bool*/ deleteSource = op->DeleteSource();
   int64_t writeTime = op->WriteTime();
   int perr;
   File* file;
   File* target;

   perr = FindOpenFile( sourceOpenId, &file);
   if (!perr)
   {
      perr = FindOpenFile( targetOpenId, &target);
      if (!perr)
      {
         if (target == &root)
         {
               // Can't replace root.
            perr = pfmErrorAccessDenied;
         }
         else if (!target->name)
         {
            perr = pfmErrorDeleted;
         }
         else if (file->sibPrev && !deleteSource)
         {
               // Links are not supported.
            perr = pfmErrorInvalid;
         }
         else if (file == target)
         {
            perr = pfmErrorInvalid;
         }
         else
         {
            perr = file->Move( target->parent, target->sibPrev, target->name, writeTime);
            if (!perr)
            {
               target->Delete( writeTime, false/*leaveChildren*/);
            }
         }
      }
   }

   op->Complete( perr);
}

   // PfmFormatterDispatch::Delete
void CCALL Volume::Delete( PfmMarshallerDeleteOp* op, void* formatterUse)
{
   int64_t openId = op->OpenId();
   int64_t parentFileId = op->ParentFileId();
   const PfmNamePart* endName = op->EndName();
   int64_t writeTime = op->WriteTime();
   int perr;
   File* file;

   perr = FindOpenFile( openId, &file);
   if (!perr)
   {
      if (file == &root)
      {
            // Can't delete root.
         perr = pfmErrorAccessDenied;
      }
      else if (!file->name)
      {
            // Already deleted.
      }
      else if (file->fileType == pfmFileTypeFolder && file->data.folder.firstChild)
      {
            // Don't allow non empty folder to be deleted
         perr = pfmErrorNotEmpty;
      }
      else
      {
         file->Delete( writeTime, false/*leaveChildren*/);
      }
   }

   op->Complete( perr);
}

   // PfmFormatterDispatch::Close
void CCALL Volume::Close( PfmMarshallerCloseOp* op, void* formatterUse)
{
   int64_t openId = op->OpenId();
   int64_t openSequence = op->OpenSequence();
   int perr;
   File* file;

   perr = FindOpenFile( openId, &file);
   if (!perr)
   {
      file->Close( openSequence);
   }

   op->Complete( perr);
}

   // PfmFormatterDispatch::FlushFile
void CCALL Volume::FlushFile( PfmMarshallerFlushFileOp* op, void* formatterUse)
{
   int64_t openId = op->OpenId();
   uint8_t flushFlags = op->FlushFlags();
   uint8_t fileFlags = op->FileFlags();
   uint8_t color = op->Color();
   int64_t createTime = op->CreateTime();
   int64_t accessTime = op->AccessTime();
   int64_t writeTime = op->WriteTime();
   int64_t changeTime = op->ChangeTime();
   int perr;
   PfmOpenAttribs openAttribs = zeroOpenAttribs;
   File* file;

   perr = FindOpenFile( openId, &file);
   if (!perr)
   {
      file->Flush( fileFlags, createTime, accessTime, writeTime, changeTime);
      if (flushFlags&pfmFlushFlagOpen)
      {
         file->Opened( &openAttribs);
      }
   }

   op->Complete( perr, &openAttribs, 0);
}

   // PfmFormatterDispatch::List
void CCALL Volume::List( PfmMarshallerListOp* op, void* formatterUse)
{
   int64_t openId = op->OpenId();
   int64_t listId = op->ListId();
   int perr = 0;
   bool noMore = false;
   File* file;
   FileList* list;

   perr = FindOpenFile( openId, &file);
   if (!perr)
   {
      perr = file->ListFactory( listId, &list);
      if (!perr)
      {
         list->Iterate( op, &noMore);
      }
   }

   op->Complete( perr, noMore);
}

   // PfmFormatterDispatch::ListEnd
void CCALL Volume::ListEnd( PfmMarshallerListEndOp* op, void* formatterUse)
{
   int64_t openId = op->OpenId();
   int64_t listId = op->ListId();
   int perr;
   File* file;

   perr = FindOpenFile( openId, &file);
   if (!perr)
   {
      perr = file->ListEnd( listId);
   }

   op->Complete( perr);
}

   // PfmFormatterDispatch::Read
void CCALL Volume::Read( PfmMarshallerReadOp* op, void* formatterUse)
{
   int64_t openId = op->OpenId();
   uint64_t fileOffset = op->FileOffset();
   void* data = op->Data();
   size_t requestedSize = op->RequestedSize();
   int perr;
   size_t actualSize = 0;
   File* file;

   perr = FindOpenFile( openId, &file);
   if (!perr)
   {
      perr = file->Read( fileOffset, data, requestedSize, &actualSize);
   }

   op->Complete( perr, actualSize);
}

   // PfmFormatterDispatch::Write
void CCALL Volume::Write( PfmMarshallerWriteOp* op, void* formatterUse)
{
   int64_t openId = op->OpenId();
   uint64_t fileOffset = op->FileOffset();
   const void* data = op->Data();
   size_t requestedSize = op->RequestedSize();
   int perr;
   size_t actualSize = 0;
   File* file;

   perr = FindOpenFile( openId, &file);
   if (!perr)
   {
      perr = file->Write( fileOffset, data, requestedSize, &actualSize);
   }

   op->Complete( perr, actualSize);
}

   // PfmFormatterDispatch::SetSize
void CCALL Volume::SetSize( PfmMarshallerSetSizeOp* op, void* formatterUse)
{
   int64_t openId = op->OpenId();
   uint64_t fileSize = op->FileSize();
   int perr;
   File* file;

   perr = FindOpenFile( openId, &file);
   if (!perr)
   {
      perr = file->SetSize( fileSize);
   }

   op->Complete( perr);
}

   // PfmFormatterDispatch::Capacity
void CCALL Volume::Capacity( PfmMarshallerCapacityOp* op, void* formatterUse)
{
   uint64_t totalCapacity = capacity;
   uint64_t availableCapacity = (100000000/File::blockSize)*File::blockSize;

   op->Complete( pfmErrorSuccess, totalCapacity, availableCapacity);
}

   // PfmFormatterDispatch::FlushMedia
void CCALL Volume::FlushMedia( PfmMarshallerFlushMediaOp* op, void* formatterUse)
{
   op->Complete( pfmErrorSuccess, -1/*msecFlushDelay*/);
}

   // PfmFormatterDispatch::Control
void CCALL Volume::Control( PfmMarshallerControlOp* op, void* formatterUse)
{
   op->Complete( pfmErrorInvalid, 0/*outputSize*/);
}

   // PfmFormatterDispatch::MediaInfo
void CCALL Volume::MediaInfo( PfmMarshallerMediaInfoOp* op, void* formatterUse)
{
   int perr = 0;
   PfmMediaInfo mediaInfo = zeroMediaInfo;
   const wchar_t* mediaLabel = L"tempfs";

   op->Complete( perr, &mediaInfo, mediaLabel);
}

   // PfmFormatterDispatch::Access
void CCALL Volume::Access( PfmMarshallerAccessOp* op, void* formatterUse)
{
   int64_t openId = op->OpenId();
   int perr = 0;
   PfmOpenAttribs openAttribs = zeroOpenAttribs;
   File* file;

   perr = FindOpenFile( openId, &file);
   if (!perr)
   {
      file->Opened( &openAttribs);
   }

   op->Complete( perr, &openAttribs, 0);
}

   // PfmFormatterDispatch::ReadXattr
void CCALL Volume::ReadXattr( PfmMarshallerReadXattrOp* op, void* formatterUse)
{
   op->Complete( pfmErrorNotFound, 0/*xattrSize*/, 0/*transferredSize*/);
}

   // PfmFormatterDispatch::WriteXattr
void CCALL Volume::WriteXattr( PfmMarshallerWriteXattrOp* op, void* formatterUse)
{
   op->Complete( pfmErrorAccessDenied, 0/*transferredSize*/);
}

Volume::Volume(void)
:
   root(this)
{
   marshaller = 0;
   capacity = 0;
   lastFileId = 1;
   root.fileType = pfmFileTypeFolder;
   root.fileFlags = 0;
   root.fileId = 1;
   root.createTime = root.accessTime = root.writeTime = root.changeTime = 0;
   firstOpenFile = 0;
}

Volume::~Volume(void)
{
   ASSERT( !marshaller);
   while (firstOpenFile)
   {
      firstOpenFile->Close( firstOpenFile->openSequence);
   }
   while (root.data.folder.firstChild)
   {
      root.data.folder.firstChild->Delete( pfmTimeInvalid, false/*leaveChildren*/);
   }
}

int wmain( int argc, const wchar_t*const* argv)
{
   int err = 0;
   PfmApi* pfm = 0;
   PfmMount* mount = 0;
   PfmMountCreateParams mcp;
   PfmMarshallerServeParams msp;
   Volume volume;

   msp.dispatch = &volume;
   msp.formatterName = "tempfs";

   if (argc <= 1)
   {
      printf(
         "Sample file system application.\n"
         "syntax: tempfs <mount name>\n");
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
         printf( "ERROR: %i Unable to open PFM Api.\n",err);
      }
   }

   if (!err)
   {
      err = PfmMarshallerFactory( &volume.marshaller);
      if (err)
      {
         printf( "ERROR: %i Unable to create marshaller.\n",err);
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
         printf( "ERROR: %i Unable to create mount.\n",err);
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
         // messsages can be viewed in the "tempfs" channel.
      volume.marshaller->SetTrace( L"tempfs");
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
   return err;
}
