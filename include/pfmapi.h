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
// file name:  pfmapi.h
// created:    2005.12.02
//----------------------------------------------------------------------------
#ifndef PFMAPI_H
#define PFMAPI_H
#ifdef __cplusplus
#include "pfmapi.hxx"
#else
#include "ptfactory1.h"

#define PFM_COMPANY                 Pismo Technic Inc.
#define PFM_COMPANYA               "Pismo Technic Inc."
#define PFM_COMPANYW              L"Pismo Technic Inc."
#define PFM_REVURL                  com.pismotec
#define PFM_REVURLA                "com.pismotec"
#define PFM_REVURLW               L"com.pismotec"
#define PFM_PRODUCT                 Pismo File Mount
#define PFM_PRODUCTA               "Pismo File Mount"
#define PFM_PRODUCTW              L"Pismo File Mount"
#define PFM_PRODID                  PismoFileMount
#define PFM_PRODIDA                "PismoFileMount"
#define PFM_PRODIDW               L"PismoFileMount"
#define PFM_COPYRIGHT               Pismo Technic Inc. Copyright 2006-2017 Joe Lowe
#define PFM_COPYRIGHTA             "Pismo Technic Inc. Copyright 2006-2017 Joe Lowe"
#define PFM_COPYRIGHTW            L"Pismo Technic Inc. Copyright 2006-2017 Joe Lowe"
#define PFM_DATE                    2017.06.23
#define PFM_DATEA                  "2017.06.23"
#define PFM_DATEW                 L"2017.06.23"
#define PFM_BUILDTAG                pfm.1.0.0.185
#define PFM_BUILDTAGA              "pfm.1.0.0.185"
#define PFM_BUILDTAGW             L"pfm.1.0.0.185"
#define PFM_BUILD                   185
#define PFM_BUILDA                 "185"
#define PFM_BUILDW                L"185"
#define PFM_PREFIX                  pfm
#define PFM_PREFIXA                "pfm"
#define PFM_PREFIXW               L"pfm"
#define PFM_APIID                   pfmapi
#define PFM_APIIDA                 "pfmapi"
#define PFM_APIIDW                L"pfmapi"
#define PFM_APIBASENAME             pfmapi_185
#define PFM_APIBASENAMEA           "pfmapi_185"
#define PFM_APIBASENAMEW          L"pfmapi_185"
#define PFM_KERNELBASENAME          pfmfs_185
#define PFM_KERNELBASENAMEA        "pfmfs_185"
#define PFM_KERNELBASENAMEW       L"pfmfs_185"
#define PFM_CMDBASENAME             pfm
#define PFM_CMDBASENAMEA           "pfm"
#define PFM_CMDBASENAMEW          L"pfm"


enum {
   pfmMountFlagReadOnly = 0x00000001,
   pfmMountFlagWorldRead = 0x00000004,
   pfmMountFlagWorldWrite = 0x00000008,
   pfmMountFlagUncOnly = 0x00000010,
   pfmMountFlagVerbose = 0x00000020,
   pfmMountFlagForceUnbuffered = 0x00000080,
   pfmMountFlagForceBuffered = 0x00000100,
   pfmMountFlagGroupRead = 0x00000400,
   pfmMountFlagGroupWrite = 0x00000800,
   pfmMountFlagGroupOwned = 0x00001000,
   pfmMountFlagWorldOwned = 0x00002000,
   pfmMountFlagCacheNameSpace = 0x00004000,
   pfmMountFlagBrowse = 0x00010000,
   pfmMountFlagUnmountOnRelease = 0x00020000,
   pfmMountFlagUnmountOnDisconnect = 0x00040000,
   pfmMountFlagLocalDriveType = 0x00080000,
   pfmMountFlagAccelerate = 0x00200000,
};

enum {
   pfmUnmountFlagBackground = 0x0001,
};

enum {
   pfmStatusFlagInitializing = 0x0001,
   pfmStatusFlagReady = 0x0002,
   pfmStatusFlagDisconnected = 0x0004,
   pfmStatusFlagClosed = 0x0008,
};

enum {
   pfmErrorSuccess = 0,
   pfmErrorDisconnect = 1,
   pfmErrorCancelled = 2,
   pfmErrorUnsupported = 3,
   pfmErrorInvalid = 4,
   pfmErrorAccessDenied = 5,
   pfmErrorOutOfMemory = 6,
   pfmErrorFailed = 7,
   pfmErrorNotFound = 8,
   pfmErrorParentNotFound = 9,
   pfmErrorExists = 10,
   pfmErrorNoSpace = 11,
   pfmErrorBadName = 12,
   pfmErrorNotEmpty = 13,
   pfmErrorEndOfData = 14,
   pfmErrorNotAFile = 15,
   pfmErrorDeleted = 16,
   pfmErrorCorruptData = 17,
   pfmErrorTimeout = 18,
   pfmErrorNotAFolder = 19,
};

enum {
   pfmFileTypeNone = 0,
   pfmFileTypeFile = 1,
   pfmFileTypeFolder = 2,
   pfmFileTypeSymlink = 3,
};

enum { pfmFileFlagsInvalid = 0xFF };

enum {
   pfmFileFlagReadOnly = 0x01,
   pfmFileFlagHidden = 0x02,
   pfmFileFlagSystem = 0x04,
   pfmFileFlagExecute = 0x08,
   pfmFileFlagHasIcon = 0x10,
   pfmFileFlagArchive = 0x20,
   pfmFileFlagAlias = 0x40,
};

enum {
   pfmExtraFlagOffline = 0x01,
   pfmExtraFlagReserved1 = 0x02,
   pfmExtraFlagNtfsCompressed = 0x04,
   pfmExtraFlagNtfsEncrypted = 0x08,
};

enum {
   pfmColorInvalid = 0,
   pfmColorDefault = 1,
   pfmColorGray = 2,
   pfmColorGreen = 3,
   pfmColorPurple = 4,
   pfmColorBlue = 5,
   pfmColorYellow = 6,
   pfmColorRed = 7,
   pfmColorOrange = 8,
};

enum {
   pfmTimeInvalid = 0,
};

enum {
   pfmAccessLevelReadInfo = 1,
   pfmAccessLevelReadData = 2,
   pfmAccessLevelWriteInfo = 3,
   pfmAccessLevelDelete = 4,
   pfmAccessLevelWriteData = 5,
   pfmAccessLevelOwner = 6,
};

enum {
   pfmControlFlagForceUnbuffered = 1,
   pfmControlFlagForceBuffered = 2,
   pfmControlFlagSlowAppends = 4,
};

enum {
   pfmClientFlagXattr = 0x0001,
   pfmClientFlagAccelerate = 0x0002,
};

enum {
   pfmFileNameTypeAny = 0x00,
   pfmFileNameTypeWindows = 0x01,
   pfmFileNameTypeUnix = 0x02,
   pfmFileNameTypeMac = 0x03,
};

enum {
   pfmVolumeFlagReadOnly = 0x0001,
   pfmVolumeFlagCompressed = 0x0002,
   pfmVolumeFlagEncrypted = 0x0004,
   pfmVolumeFlagCaseSensitive = 0x0008,
   pfmVolumeFlagTouchMap = 0x0010,
   pfmVolumeFlagNoCreateTime = 0x0100,
   pfmVolumeFlagNoAccessTime = 0x0200,
   pfmVolumeFlagNoWriteTime = 0x0400,
   pfmVolumeFlagNoChangeTime = 0x0800,
   pfmVolumeFlagXattr = 0x1000,
   pfmVolumeFlagSymlinks = 0x2000,
   pfmVolumeFlagFakeNamedStreams = 0x4000,
   pfmVolumeFlagAccelerate = 0x8000,
};

enum {
   pfmFlushFlagOpen = 0x0001,
};

typedef struct PfmMountCreateParams PfmMountCreateParams; struct PfmMountCreateParams
{
   size_t paramsSize;
   const wchar_t* mountSourceName;
   int mountFlags;
   int reserved1;
   wchar_t driveLetter;
   wchar_t reserved2[sizeof(void*)/sizeof(wchar_t)-1];
   const wchar_t* ownerId;
   PT_FD_T toFormatterWrite;
   PT_FD_T fromFormatterRead;
   PT_FD_T toAuthWrite;
   PT_FD_T fromAuthRead;
   uint64_t blockModeOffset;
   unsigned blockModeAlign;
   unsigned reserved3[1];
   const wchar_t* appId;
};

PT_INLINE void PfmMountCreateParams_Init( PfmMountCreateParams* mcp)
   { memset( mcp, 0, sizeof( *mcp)); mcp->paramsSize = sizeof( *mcp);
   mcp->mountFlags = pfmMountFlagCacheNameSpace|pfmMountFlagUnmountOnDisconnect;
   mcp->toFormatterWrite = mcp->fromFormatterRead = mcp->toAuthWrite = mcp->fromAuthRead = PT_FD_INVALID; }

/*interface PfmMount*/ typedef struct PfmMount_vtbl_t PfmMount_vtbl_t, * PfmMount; struct PfmMount_vtbl_t
{
   void (PT_CCALL* AddRef)( PfmMount*);
   void (PT_CCALL* Release)( PfmMount*);
   int /*err*/ (PT_CCALL* Refresh)( PfmMount*);
   int (PT_CCALL* GetMountId)( PfmMount*);
   int (PT_CCALL* GetMountFlags)( PfmMount*);
   int (PT_CCALL* GetStatusFlags)( PfmMount*);
   int (PT_CCALL* GetVolumeFlags)( PfmMount*);
   int64_t (PT_CCALL* GetChangeInstance)( PfmMount*);
   const wchar_t* (PT_CCALL* GetMountSourceName)( PfmMount*);
   const wchar_t* (PT_CCALL* GetMountPoint)( PfmMount*);
   const wchar_t* (PT_CCALL* GetUncName)( PfmMount*);
   const wchar_t* (PT_CCALL* GetOwnerId)( PfmMount*);
   const wchar_t* (PT_CCALL* GetOwnerName)( PfmMount*);
   const wchar_t* (PT_CCALL* GetFormatterName)( PfmMount*);
   wchar_t (PT_CCALL* GetDriveLetter)( PfmMount*);
   int /*err*/ (PT_CCALL* WaitReady)( PfmMount*, int timeoutMsecs);
   int /*err*/ (PT_CCALL* Unmount)( PfmMount*, int unmountFlags);
   int /*err*/ (PT_CCALL* Flush)( PfmMount*);
   int /*err*/ (PT_CCALL* Control)( PfmMount*, int controlCode, const void* input, size_t inputSize, void* output, size_t maxOutputSize, size_t* outputSize);
   const wchar_t* (PT_CCALL* GetMountEndName)( PfmMount*);
   const wchar_t* (PT_CCALL* GetAppId)( PfmMount*);
   const char* (PT_CCALL* GetReport)( PfmMount*, int reportFlags);
};

/*interface PfmFileMountUi*/ typedef struct PfmFileMountUi_vtbl_t PfmFileMountUi_vtbl_t, * PfmFileMountUi; struct PfmFileMountUi_vtbl_t
{
   void (PT_CCALL* Start)( PfmFileMountUi*);
   void (PT_CCALL* Complete)( PfmFileMountUi*, const wchar_t* errorMessage);
   void (PT_CCALL* Status)( PfmFileMountUi*, const wchar_t* data, int/*bool*/ endOfLine);
   void (PT_CCALL* Suspend)( PfmFileMountUi*);
   void (PT_CCALL* Resume)( PfmFileMountUi*);
   const wchar_t* (PT_CCALL* QueryPassword)( PfmFileMountUi*, const wchar_t* prompt, int count);
   void (PT_CCALL* ClearPassword)( PfmFileMountUi*);
};

enum {
   pfmFileMountFlagConsoleUi = 0x0001,
   pfmFileMountFlagInProcess = 0x0002,
   pfmFileMountFlagVerbose = 0x0004,
   pfmFileMountFlagReserved1 = 0x0008,
   pfmFileMountFlagEditOptions = 0x0020,
   pfmFileMountFlagMultiMount = 0x0040,
   pfmFileMountFlagSkipExisting = 0x0080,
};

typedef struct PfmFileMountCreateParams PfmFileMountCreateParams; struct PfmFileMountCreateParams
{
   size_t paramsSize;
   const wchar_t* mountFileName;
   const wchar_t* reserved1;
   const wchar_t* const* reserved_argv;
   int reserved_argc;
   int mountFlags;
   int fileMountFlags;
   int reserved2;
   wchar_t driveLetter;
   wchar_t reserved3[sizeof(int)*2/sizeof(wchar_t)-1];
   const wchar_t* ownerId;
   const wchar_t* formatterName;
   const wchar_t* password;
   PfmFileMountUi* ui;
   void const* reserved4[3];
};

PT_INLINE void PfmFileMountCreateParams_Init(PfmFileMountCreateParams* p)
   { memset( p, 0, sizeof( *p)); p->paramsSize = sizeof( *p);
   p->mountFlags = pfmMountFlagCacheNameSpace|pfmMountFlagUnmountOnDisconnect; }

/*interface PfmFileMount*/ typedef struct PfmFileMount_vtbl_t PfmFileMount_vtbl_t, * PfmFileMount; struct PfmFileMount_vtbl_t
{
   void (PT_CCALL* AddRef)( PfmFileMount*);
   void (PT_CCALL* Release)( PfmFileMount*);
   int /*err*/ (PT_CCALL* GetInterface)( PfmFileMount*, const char* id, void*);
   void (PT_CCALL* Cancel)( PfmFileMount*);
   int /*err*/ (PT_CCALL* Start)( PfmFileMount*, PfmFileMountCreateParams const* params_);
   void (PT_CCALL* Send)( PfmFileMount*, const wchar_t* data, int/*bool*/ endOfLine);
   void (PT_CCALL* Status)( PfmFileMount*, const wchar_t* data, int/*bool*/ endOfLine);
   int /*err*/ (PT_CCALL* WaitReady)( PfmFileMount*);
   PfmMount* (PT_CCALL* GetMount)( PfmFileMount*);
   void (PT_CCALL* Detach)( PfmFileMount*);
};

/*interface PfmMountIterator*/ typedef struct PfmMountIterator_vtbl_t PfmMountIterator_vtbl_t, * PfmMountIterator; struct PfmMountIterator_vtbl_t
{
   void (PT_CCALL* AddRef)( PfmMountIterator*);
   void (PT_CCALL* Release)( PfmMountIterator*);
   int /*mountId*/ (PT_CCALL* Next)( PfmMountIterator*, int64_t* changeInstance);
};

/*interface PfmMountMonitor*/ typedef struct PfmMountMonitor_vtbl_t PfmMountMonitor_vtbl_t, * PfmMountMonitor; struct PfmMountMonitor_vtbl_t
{
   void (PT_CCALL* AddRef)( PfmMountMonitor*);
   void (PT_CCALL* Release)( PfmMountMonitor*);
   int /*err*/ (PT_CCALL* Wait)( PfmMountMonitor*, int64_t nextChangeInstance, int timeoutMsecs);
   void (PT_CCALL* Cancel)( PfmMountMonitor*);
};

enum {
   pfmFastPipeFlagFastMapping = 0x0002,
   pfmFastPipeFlagNamedDevice = 0x0004,
   pfmFastPipeFlagAsyncClient = 0x0008,
   pfmFastPipeFlagAsyncServer = 0x0010,
};

typedef struct PfmFastPipeCreateParams PfmFastPipeCreateParams; struct PfmFastPipeCreateParams
{
   size_t paramsSize;
   const wchar_t* baseDeviceName;
   int fastPipeFlags;
   #if UINT_MAX < SIZE_MAX
   int reserved1[1];
   #endif
   wchar_t* deviceName;
   size_t maxDeviceNameChars;
};

PT_INLINE void PfmFastPipeCreateParams_Init(PfmFastPipeCreateParams* pcp) { memset( pcp, 0, sizeof( *pcp)); pcp->paramsSize = sizeof( *pcp); }

enum {
   pfmFastPipeOpTypeRead = 1,
   pfmFastPipeOpTypeWrite = 2,
   pfmFastPipeOpTypeSend = 3,
};

typedef struct PfmFastPipeOp PfmFastPipeOp; struct PfmFastPipeOp
{
   uint64_t clientId;
   uint64_t offset;
   int opType;
   #if UINT_MAX < SIZE_MAX
   int reserved1[1];
   #endif
   const void* input;
   size_t maxInputSize;
   void* output;
   size_t maxOutputSize;
   uint64_t opaque[(sizeof(void*)*4+48)/8];
};

typedef struct PfmFastPipeSendContext PfmFastPipeSendContext;

typedef void (PT_CCALL* PfmFastPipeSendContext_Complete)( PfmFastPipeSendContext*, int err, size_t inputSize, size_t outputSize);

struct PfmFastPipeSendContext
{
   PfmFastPipeSendContext_Complete complete;
   #if UINT_MAX == SIZE_MAX
   uint8_t align1[4];
   #endif
   uint64_t opaque[(sizeof(void*)*10+40)/8];
};

/*interface PfmFastPipeServer*/ typedef struct PfmFastPipeServer_vtbl_t PfmFastPipeServer_vtbl_t, * PfmFastPipeServer; struct PfmFastPipeServer_vtbl_t
{
   void (PT_CCALL* Release)( PfmFastPipeServer*);
   void (PT_CCALL* InitOp)( PfmFastPipeServer*, PfmFastPipeOp* op);
   void (PT_CCALL* FreeOp)( PfmFastPipeServer*, PfmFastPipeOp* op);
   int /*err*/ (PT_CCALL* Receive)( PfmFastPipeServer*, PfmFastPipeOp* op);
   void (PT_CCALL* Complete)( PfmFastPipeServer*, PfmFastPipeOp* op, int err, size_t inputSize, size_t outputSize);
   void (PT_CCALL* Cancel)( PfmFastPipeServer*);
};

/*interface PfmApi*/ typedef struct PfmApi_vtbl_t PfmApi_vtbl_t, * PfmApi; struct PfmApi_vtbl_t
{
   void (PT_CCALL* AddRef)( PfmApi*);
   void (PT_CCALL* Release)( PfmApi*);
   const char* (PT_CCALL* Version)( PfmApi*);
   int /*err*/ (PT_CCALL* SysStart)( PfmApi*);
   int /*err*/ (PT_CCALL* MountCreate)( PfmApi*, PfmMountCreateParams const* params_, PfmMount** mount);
   int /*err*/ (PT_CCALL* MountSourceNameOpen)( PfmApi*, const wchar_t* mountSourceName, PfmMount** mount);
   int /*err*/ (PT_CCALL* MountPointOpen)( PfmApi*, const wchar_t* mountPoint, PfmMount** mount);
   int /*err*/ (PT_CCALL* MountIdOpen)( PfmApi*, int mountId, PfmMount** mount);
   int /*err*/ (PT_CCALL* MountIterate)( PfmApi*, int64_t startChangeInstance, int64_t* nextChangeInstance, PfmMountIterator** iterator);
   int /*err*/ (PT_CCALL* MountMonitorFactory)( PfmApi*, PfmMountMonitor** monitor);
   int /*err*/ (PT_CCALL* FileMountFactory)( PfmApi*, PfmFileMount** fileMount);
   int /*err*/ (PT_CCALL* FastPipeCreate)( PfmApi*, PfmFastPipeCreateParams const* params_, PT_FD_T* clientFd, PT_FD_T* serverFd);
   int /*err*/ (PT_CCALL* FastPipeCancel)( PfmApi*, PT_FD_T);
   int /*err*/ (PT_CCALL* FastPipeEnableFastMapping)( PfmApi*, PT_FD_T clientFd);
   int /*err*/ (PT_CCALL* FastPipeServerFactory)( PfmApi*, PT_FD_T serverFd, PfmFastPipeServer**);
   int /*err*/ (PT_CCALL* FastPipeClientContext)( PfmApi*, PT_FD_T clientFd, int* clientContext);
   void (PT_CCALL* FastPipeSend)( PfmApi*, PT_FD_T clientFd, int clientContext, uint64_t offset, const void* input, size_t maxInputSize, void* output, size_t maxOutputSize, PfmFastPipeSendContext*);
   int /*err*/ (PT_CCALL* MountEndNameOpen)( PfmApi*, const wchar_t* mountEndName, PfmMount** mount);
};

typedef PfmMountIterator PfmIterator;
typedef PfmMountMonitor PfmMonitor;

   // PfmApi interface version history.
   //    PfmApi1 - 2007.12.31
   //       First public release.
   //    PfmApi2 - 2008.01.25
   //       Auth handles now optional when creating mounts.
   //       WaitReady() added to PfmMount.
   //    PfmApi3 - 2008.02.12
   //       New driveletter parameter in PfmApi::Create.
   //       GetDriveLetter() added to PfmMount.
   //    PfmApi4 - 2008.11.04
   //       Improved handling of process visible mounts (SxS load error fix).
   //    PfmApi5 - 2009.02.26
   //       Fastpipe support.
   //    PfmApi6 - 2010.01.27
   //       Fastpipe and marshaller cancel.
   //    PfmApi7 - 2012.04.12
   //       Version and build installation check.
   //       FileMountCreate() added.
   //       Non-privileged user visible drive letter support.
   //    PfmApi8 - 2012.11.19
   //       Mount.GetMountPoint() added.
   //    PfmApi9 - 2013.12.30
   //       Unmount-on-release support added.
   //    PfmApi10 - 2015.03.25
   //       Dropped virtual mount point support.
   //       Dropped visible process support.
   //       Dropped arbitrary mount point support.
   //       Dropped alerter support.
   //       New device and file versioned name convention.
   //    PfmApi11 - 2015.04.xx
   //       Mount endName and sourceName.
   //       Fake local drive letter mount option.
   //    PfmApi12 - 2016.04.11
   //       Real local drive type mount option.
   //       Mount report diagnostic API.
PTFACTORY1_DECLARE( PfmApi, PFM_PRODIDW, PFM_APIIDW);
PT_INLINE int/*error*/ PfmApiFactory( PfmApi** pfmApi)
   { return PfmApiGetInterface( "PfmApi12", pfmApi); }
// void PfmApiUnload( void);

enum {
   pfmInstInstalled = 0,
   pfmInstOldBuild = 1,
   pfmInstOldVersion = 2,
   pfmInstNotInstalled = 3,
};

PT_INLINE int PfmBuildTagCompare( const char* tag1, const char* tag2)
{
   unsigned i = 0;
   char a, b;
   for (;;)
   {
      a = tag1[i];
      b = tag2[i];
      if (a >= '0' && a <= '9') { if (b < '0' || b > '9') b = 0; }
      else if (a == b) { i ++; continue; }
      else b = 0;
      break;
   }
   for (;;)
   {
      if (a > b) return 1;
      else if (a == b)
      {
         if (a)
         {
            a = tag1[i];
            b = tag2[i];
            i ++;
            continue;
         }
         break;
      }
      return -1;
   }
   return 0;
}

PT_INLINE int PfmInstallCheck( void)
{
   int res = pfmInstNotInstalled;
   PfmApi* api;
   PfmApiUnload();
   if (PfmApiFactory( &api) == 0)
   {
      res = pfmInstInstalled;
      if (PfmBuildTagCompare( PFM_BUILDTAGA, PT_VCAL0( api, Version)) > 0) res = pfmInstOldBuild;
   }
   else if (PfmApiGetInterface( "PfmApi0", &api) == 0 || PfmApiGetInterface( "PfmApi1", &api) == 0)
   {
      res = pfmInstOldVersion;
   }
   if (api) PT_VCAL0(api,Release);
   if (res != pfmInstInstalled) PfmApiUnload();
   return res;
}

typedef struct PfmAttribs PfmAttribs; struct PfmAttribs
{
   int8_t fileType;
   uint8_t fileFlags;
   uint8_t extraFlags;
   uint8_t color;
   unsigned resourceSize;
   int64_t fileId;
   uint64_t fileSize;
   int64_t createTime;
   int64_t accessTime;
   int64_t writeTime;
   int64_t changeTime;
};

typedef struct PfmOpenAttribs PfmOpenAttribs; struct PfmOpenAttribs
{
   int64_t openId;
   int64_t openSequence;
   int8_t accessLevel;
   uint8_t controlFlags;
   uint8_t reserved1[2];
   unsigned touch;
   PfmAttribs attribs;
};

typedef struct PfmNamePart PfmNamePart; struct PfmNamePart
{
   const wchar_t* name;
   size_t len;
   const char* name8;
   size_t len8;
};

typedef struct PfmMediaInfo PfmMediaInfo; struct PfmMediaInfo
{
   PT_UUID mediaUuid;
   uint64_t mediaId64;
   unsigned mediaId32;
   uint8_t mediaFlags;
   uint8_t reserved1[3];
   int64_t createTime;
};

enum { pfmOpFormatterUseSize = 20*sizeof(int)+20*sizeof(void*) };

/*interface PfmFormatterDispatch*/ typedef struct PfmFormatterDispatch_vtbl_t PfmFormatterDispatch_vtbl_t, * PfmFormatterDispatch;

typedef struct PfmMarshallerServeParams PfmMarshallerServeParams; struct PfmMarshallerServeParams
{
   size_t paramsSize;
   PfmFormatterDispatch* dispatch;
   int volumeFlags;
   #if UINT_MAX < SIZE_MAX
   PT_UINT8 align1[sizeof(size_t)-sizeof(int)];
   #endif
   const char* formatterName;
   size_t dataAlign;
   size_t maxDirectMsgSize;
   PT_FD_T toFormatterRead;
   PT_FD_T fromFormatterWrite;
   int volumeFileNameType;
   int reserved1;
};

PT_INLINE void PfmMarshallerServeParams_Init( PfmMarshallerServeParams* params)
   { memset( params, 0, sizeof( *params)); params->paramsSize = sizeof( *params);
   params->toFormatterRead = params->fromFormatterWrite = PT_FD_INVALID; }

/*interface PfmFormatterSerializeOpen*/ typedef struct PfmFormatterSerializeOpen_vtbl_t PfmFormatterSerializeOpen_vtbl_t, * PfmFormatterSerializeOpen; struct PfmFormatterSerializeOpen_vtbl_t
{
   void (PT_CCALL* SerializeOpen)( PfmFormatterSerializeOpen*, int64_t openId, int64_t* openSequence);
};

/*interface PfmMarshallerOpenOp*/ typedef struct PfmMarshallerOpenOp_vtbl_t PfmMarshallerOpenOp_vtbl_t, * PfmMarshallerOpenOp; struct PfmMarshallerOpenOp_vtbl_t
{
   PfmNamePart const* (PT_CCALL* NameParts)( PfmMarshallerOpenOp*);
   size_t (PT_CCALL* NamePartCount)( PfmMarshallerOpenOp*);
   int8_t (PT_CCALL* CreateFileType)( PfmMarshallerOpenOp*);
   uint8_t (PT_CCALL* CreateFileFlags)( PfmMarshallerOpenOp*);
   int64_t (PT_CCALL* WriteTime)( PfmMarshallerOpenOp*);
   int64_t (PT_CCALL* NewCreateOpenId)( PfmMarshallerOpenOp*);
   int8_t (PT_CCALL* ExistingAccessLevel)( PfmMarshallerOpenOp*);
   int64_t (PT_CCALL* NewExistingOpenId)( PfmMarshallerOpenOp*);
   void (PT_CCALL* Complete)( PfmMarshallerOpenOp*, int perr, uint8_t/*bool*/ existed, PfmOpenAttribs const* openAttribs, int64_t parentFileId, const wchar_t* endName, size_t linkNamePartCount, const void* linkData, size_t linkDataSize, PfmFormatterSerializeOpen* serializeOpen);
};

/*interface PfmMarshallerReplaceOp*/ typedef struct PfmMarshallerReplaceOp_vtbl_t PfmMarshallerReplaceOp_vtbl_t, * PfmMarshallerReplaceOp; struct PfmMarshallerReplaceOp_vtbl_t
{
   int64_t (PT_CCALL* TargetOpenId)( PfmMarshallerReplaceOp*);
   int64_t (PT_CCALL* TargetParentFileId)( PfmMarshallerReplaceOp*);
   PfmNamePart const* (PT_CCALL* TargetEndName)( PfmMarshallerReplaceOp*);
   uint8_t (PT_CCALL* CreateFileFlags)( PfmMarshallerReplaceOp*);
   int64_t (PT_CCALL* WriteTime)( PfmMarshallerReplaceOp*);
   int64_t (PT_CCALL* NewCreateOpenId)( PfmMarshallerReplaceOp*);
   void (PT_CCALL* Complete)( PfmMarshallerReplaceOp*, int perr, PfmOpenAttribs const* openAttribs, PfmFormatterSerializeOpen* serializeOpen);
};

/*interface PfmMarshallerMoveOp*/ typedef struct PfmMarshallerMoveOp_vtbl_t PfmMarshallerMoveOp_vtbl_t, * PfmMarshallerMoveOp; struct PfmMarshallerMoveOp_vtbl_t
{
   int64_t (PT_CCALL* SourceOpenId)( PfmMarshallerMoveOp*);
   int64_t (PT_CCALL* SourceParentFileId)( PfmMarshallerMoveOp*);
   PfmNamePart const* (PT_CCALL* SourceEndName)( PfmMarshallerMoveOp*);
   PfmNamePart const* (PT_CCALL* TargetNameParts)( PfmMarshallerMoveOp*);
   size_t (PT_CCALL* TargetNamePartCount)( PfmMarshallerMoveOp*);
   uint8_t/*bool*/ (PT_CCALL* DeleteSource)( PfmMarshallerMoveOp*);
   int64_t (PT_CCALL* WriteTime)( PfmMarshallerMoveOp*);
   int8_t (PT_CCALL* ExistingAccessLevel)( PfmMarshallerMoveOp*);
   int64_t (PT_CCALL* NewExistingOpenId)( PfmMarshallerMoveOp*);
   void (PT_CCALL* Complete)( PfmMarshallerMoveOp*, int perr, uint8_t/*bool*/ existed, PfmOpenAttribs const* openAttribs, int64_t parentFileId, const wchar_t* endName, size_t linkNamePartCount, const void* linkData, size_t linkDataSize, PfmFormatterSerializeOpen* serializeOpen);
};

/*interface PfmMarshallerMoveReplaceOp*/ typedef struct PfmMarshallerMoveReplaceOp_vtbl_t PfmMarshallerMoveReplaceOp_vtbl_t, * PfmMarshallerMoveReplaceOp; struct PfmMarshallerMoveReplaceOp_vtbl_t
{
   int64_t (PT_CCALL* SourceOpenId)( PfmMarshallerMoveReplaceOp*);
   int64_t (PT_CCALL* SourceParentFileId)( PfmMarshallerMoveReplaceOp*);
   PfmNamePart const* (PT_CCALL* SourceEndName)( PfmMarshallerMoveReplaceOp*);
   int64_t (PT_CCALL* TargetOpenId)( PfmMarshallerMoveReplaceOp*);
   int64_t (PT_CCALL* TargetParentFileId)( PfmMarshallerMoveReplaceOp*);
   PfmNamePart const* (PT_CCALL* TargetEndName)( PfmMarshallerMoveReplaceOp*);
   uint8_t/*bool*/ (PT_CCALL* DeleteSource)( PfmMarshallerMoveReplaceOp*);
   int64_t (PT_CCALL* WriteTime)( PfmMarshallerMoveReplaceOp*);
   void (PT_CCALL* Complete)( PfmMarshallerMoveReplaceOp*, int perr);
};

/*interface PfmMarshallerDeleteOp*/ typedef struct PfmMarshallerDeleteOp_vtbl_t PfmMarshallerDeleteOp_vtbl_t, * PfmMarshallerDeleteOp; struct PfmMarshallerDeleteOp_vtbl_t
{
   int64_t (PT_CCALL* OpenId)( PfmMarshallerDeleteOp*);
   int64_t (PT_CCALL* ParentFileId)( PfmMarshallerDeleteOp*);
   PfmNamePart const* (PT_CCALL* EndName)( PfmMarshallerDeleteOp*);
   int64_t (PT_CCALL* WriteTime)( PfmMarshallerDeleteOp*);
   void (PT_CCALL* Complete)( PfmMarshallerDeleteOp*, int perr);
};

/*interface PfmMarshallerCloseOp*/ typedef struct PfmMarshallerCloseOp_vtbl_t PfmMarshallerCloseOp_vtbl_t, * PfmMarshallerCloseOp; struct PfmMarshallerCloseOp_vtbl_t
{
   int64_t (PT_CCALL* OpenId)( PfmMarshallerCloseOp*);
   int64_t (PT_CCALL* OpenSequence)( PfmMarshallerCloseOp*);
   void (PT_CCALL* Complete)( PfmMarshallerCloseOp*, int perr);
};

/*interface PfmMarshallerFlushFileOp*/ typedef struct PfmMarshallerFlushFileOp_vtbl_t PfmMarshallerFlushFileOp_vtbl_t, * PfmMarshallerFlushFileOp; struct PfmMarshallerFlushFileOp_vtbl_t
{
   int64_t (PT_CCALL* OpenId)( PfmMarshallerFlushFileOp*);
   uint8_t (PT_CCALL* FlushFlags)( PfmMarshallerFlushFileOp*);
   uint8_t (PT_CCALL* FileFlags)( PfmMarshallerFlushFileOp*);
   uint8_t (PT_CCALL* Color)( PfmMarshallerFlushFileOp*);
   int64_t (PT_CCALL* CreateTime)( PfmMarshallerFlushFileOp*);
   int64_t (PT_CCALL* AccessTime)( PfmMarshallerFlushFileOp*);
   int64_t (PT_CCALL* WriteTime)( PfmMarshallerFlushFileOp*);
   int64_t (PT_CCALL* ChangeTime)( PfmMarshallerFlushFileOp*);
   void (PT_CCALL* Complete)( PfmMarshallerFlushFileOp*, int perr, PfmOpenAttribs const* openAttribs, PfmFormatterSerializeOpen* serializeOpen);
   const void* (PT_CCALL* LinkData)( PfmMarshallerFlushFileOp*);
   unsigned (PT_CCALL* LinkDataSize)( PfmMarshallerFlushFileOp*);
};

/*interface PfmMarshallerListOp*/ typedef struct PfmMarshallerListOp_vtbl_t PfmMarshallerListOp_vtbl_t, * PfmMarshallerListOp; struct PfmMarshallerListOp_vtbl_t
{
   int64_t (PT_CCALL* OpenId)( PfmMarshallerListOp*);
   int64_t (PT_CCALL* ListId)( PfmMarshallerListOp*);
   uint8_t/*bool*/ (PT_CCALL* Add)( PfmMarshallerListOp*, PfmAttribs const* attribs, const wchar_t* endName);
   uint8_t/*bool*/ (PT_CCALL* Add8)( PfmMarshallerListOp*, PfmAttribs const* attribs, const char* endName);
   void (PT_CCALL* Complete)( PfmMarshallerListOp*, int perr, uint8_t/*bool*/ noMore);
};

/*interface PfmMarshallerListEndOp*/ typedef struct PfmMarshallerListEndOp_vtbl_t PfmMarshallerListEndOp_vtbl_t, * PfmMarshallerListEndOp; struct PfmMarshallerListEndOp_vtbl_t
{
   int64_t (PT_CCALL* OpenId)( PfmMarshallerListEndOp*);
   int64_t (PT_CCALL* ListId)( PfmMarshallerListEndOp*);
   void (PT_CCALL* Complete)( PfmMarshallerListEndOp*, int perr);
};

/*interface PfmMarshallerReadOp*/ typedef struct PfmMarshallerReadOp_vtbl_t PfmMarshallerReadOp_vtbl_t, * PfmMarshallerReadOp; struct PfmMarshallerReadOp_vtbl_t
{
   int64_t (PT_CCALL* OpenId)( PfmMarshallerReadOp*);
   uint64_t (PT_CCALL* FileOffset)( PfmMarshallerReadOp*);
   void* (PT_CCALL* Data)( PfmMarshallerReadOp*);
   size_t (PT_CCALL* RequestedSize)( PfmMarshallerReadOp*);
   void (PT_CCALL* Complete)( PfmMarshallerReadOp*, int perr, size_t actualSize);
};

/*interface PfmMarshallerWriteOp*/ typedef struct PfmMarshallerWriteOp_vtbl_t PfmMarshallerWriteOp_vtbl_t, * PfmMarshallerWriteOp; struct PfmMarshallerWriteOp_vtbl_t
{
   int64_t (PT_CCALL* OpenId)( PfmMarshallerWriteOp*);
   uint64_t (PT_CCALL* FileOffset)( PfmMarshallerWriteOp*);
   const void* (PT_CCALL* Data)( PfmMarshallerWriteOp*);
   size_t (PT_CCALL* RequestedSize)( PfmMarshallerWriteOp*);
   void (PT_CCALL* Complete)( PfmMarshallerWriteOp*, int perr, size_t actualSize);
};

/*interface PfmMarshallerSetSizeOp*/ typedef struct PfmMarshallerSetSizeOp_vtbl_t PfmMarshallerSetSizeOp_vtbl_t, * PfmMarshallerSetSizeOp; struct PfmMarshallerSetSizeOp_vtbl_t
{
   int64_t (PT_CCALL* OpenId)( PfmMarshallerSetSizeOp*);
   uint64_t (PT_CCALL* FileSize)( PfmMarshallerSetSizeOp*);
   void (PT_CCALL* Complete)( PfmMarshallerSetSizeOp*, int perr);
};

/*interface PfmMarshallerCapacityOp*/ typedef struct PfmMarshallerCapacityOp_vtbl_t PfmMarshallerCapacityOp_vtbl_t, * PfmMarshallerCapacityOp; struct PfmMarshallerCapacityOp_vtbl_t
{
   int64_t (PT_CCALL* OpenId)( PfmMarshallerCapacityOp*);
   void (PT_CCALL* Complete)( PfmMarshallerCapacityOp*, int perr, uint64_t totalCapacity, uint64_t availableCapacity);
};

/*interface PfmMarshallerFlushMediaOp*/ typedef struct PfmMarshallerFlushMediaOp_vtbl_t PfmMarshallerFlushMediaOp_vtbl_t, * PfmMarshallerFlushMediaOp; struct PfmMarshallerFlushMediaOp_vtbl_t
{
   void (PT_CCALL* Complete)( PfmMarshallerFlushMediaOp*, int perr, int msecFlushDelay);
};

/*interface PfmMarshallerControlOp*/ typedef struct PfmMarshallerControlOp_vtbl_t PfmMarshallerControlOp_vtbl_t, * PfmMarshallerControlOp; struct PfmMarshallerControlOp_vtbl_t
{
   int64_t (PT_CCALL* OpenId)( PfmMarshallerControlOp*);
   int8_t (PT_CCALL* AccessLevel)( PfmMarshallerControlOp*);
   int (PT_CCALL* ControlCode)( PfmMarshallerControlOp*);
   const void* (PT_CCALL* Input)( PfmMarshallerControlOp*);
   size_t (PT_CCALL* InputSize)( PfmMarshallerControlOp*);
   void* (PT_CCALL* Output)( PfmMarshallerControlOp*);
   size_t (PT_CCALL* MaxOutputSize)( PfmMarshallerControlOp*);
   void (PT_CCALL* Complete)( PfmMarshallerControlOp*, int perr, size_t outputSize);
};

/*interface PfmMarshallerMediaInfoOp*/ typedef struct PfmMarshallerMediaInfoOp_vtbl_t PfmMarshallerMediaInfoOp_vtbl_t, * PfmMarshallerMediaInfoOp; struct PfmMarshallerMediaInfoOp_vtbl_t
{
   int64_t (PT_CCALL* OpenId)( PfmMarshallerMediaInfoOp*);
   void (PT_CCALL* Complete)( PfmMarshallerMediaInfoOp*, int perr, PfmMediaInfo const* mediaInfo, const wchar_t* mediaLabel);
};

/*interface PfmMarshallerAccessOp*/ typedef struct PfmMarshallerAccessOp_vtbl_t PfmMarshallerAccessOp_vtbl_t, * PfmMarshallerAccessOp; struct PfmMarshallerAccessOp_vtbl_t
{
   int64_t (PT_CCALL* OpenId)( PfmMarshallerAccessOp*);
   int8_t (PT_CCALL* AccessLevel)( PfmMarshallerAccessOp*);
   void (PT_CCALL* Complete)( PfmMarshallerAccessOp*, int perr, PfmOpenAttribs const* openAttribs, PfmFormatterSerializeOpen* serializeOpen);
};

/*interface PfmMarshallerReadXattrOp*/ typedef struct PfmMarshallerReadXattrOp_vtbl_t PfmMarshallerReadXattrOp_vtbl_t, * PfmMarshallerReadXattrOp; struct PfmMarshallerReadXattrOp_vtbl_t
{
   int64_t (PT_CCALL* OpenId)( PfmMarshallerReadXattrOp*);
   PfmNamePart const* (PT_CCALL* Name)( PfmMarshallerReadXattrOp*);
   unsigned (PT_CCALL* Offset)( PfmMarshallerReadXattrOp*);
   void* (PT_CCALL* Data)( PfmMarshallerReadXattrOp*);
   size_t (PT_CCALL* RequestedSize)( PfmMarshallerReadXattrOp*);
   void (PT_CCALL* Complete)( PfmMarshallerReadXattrOp*, int perr, unsigned xattrSize, size_t transferredSize);
};

/*interface PfmMarshallerWriteXattrOp*/ typedef struct PfmMarshallerWriteXattrOp_vtbl_t PfmMarshallerWriteXattrOp_vtbl_t, * PfmMarshallerWriteXattrOp; struct PfmMarshallerWriteXattrOp_vtbl_t
{
   int64_t (PT_CCALL* OpenId)( PfmMarshallerWriteXattrOp*);
   PfmNamePart const* (PT_CCALL* Name)( PfmMarshallerWriteXattrOp*);
   unsigned (PT_CCALL* XattrSize)( PfmMarshallerWriteXattrOp*);
   unsigned (PT_CCALL* Offset)( PfmMarshallerWriteXattrOp*);
   const void* (PT_CCALL* Data)( PfmMarshallerWriteXattrOp*);
   size_t (PT_CCALL* RequestedSize)( PfmMarshallerWriteXattrOp*);
   void (PT_CCALL* Complete)( PfmMarshallerWriteXattrOp*, int perr, size_t transferredSize);
};

/*interface PfmFormatterDispatch*/ struct PfmFormatterDispatch_vtbl_t
{
   void (PT_CCALL* Open)( PfmFormatterDispatch*, PfmMarshallerOpenOp* op, void* formatterUse);
   void (PT_CCALL* Replace)( PfmFormatterDispatch*, PfmMarshallerReplaceOp* op, void* formatterUse);
   void (PT_CCALL* Move)( PfmFormatterDispatch*, PfmMarshallerMoveOp* op, void* formatterUse);
   void (PT_CCALL* MoveReplace)( PfmFormatterDispatch*, PfmMarshallerMoveReplaceOp* op, void* formatterUse);
   void (PT_CCALL* Delete)( PfmFormatterDispatch*, PfmMarshallerDeleteOp* op, void* formatterUse);
   void (PT_CCALL* Close)( PfmFormatterDispatch*, PfmMarshallerCloseOp* op, void* formatterUse);
   void (PT_CCALL* FlushFile)( PfmFormatterDispatch*, PfmMarshallerFlushFileOp* op, void* formatterUse);
   void (PT_CCALL* List)( PfmFormatterDispatch*, PfmMarshallerListOp* op, void* formatterUse);
   void (PT_CCALL* ListEnd)( PfmFormatterDispatch*, PfmMarshallerListEndOp* op, void* formatterUse);
   void (PT_CCALL* Read)( PfmFormatterDispatch*, PfmMarshallerReadOp* op, void* formatterUse);
   void (PT_CCALL* Write)( PfmFormatterDispatch*, PfmMarshallerWriteOp* op, void* formatterUse);
   void (PT_CCALL* SetSize)( PfmFormatterDispatch*, PfmMarshallerSetSizeOp* op, void* formatterUse);
   void (PT_CCALL* Capacity)( PfmFormatterDispatch*, PfmMarshallerCapacityOp* op, void* formatterUse);
   void (PT_CCALL* FlushMedia)( PfmFormatterDispatch*, PfmMarshallerFlushMediaOp* op, void* formatterUse);
   void (PT_CCALL* Control)( PfmFormatterDispatch*, PfmMarshallerControlOp* op, void* formatterUse);
   void (PT_CCALL* MediaInfo)( PfmFormatterDispatch*, PfmMarshallerMediaInfoOp* op, void* formatterUse);
   void (PT_CCALL* Access)( PfmFormatterDispatch*, PfmMarshallerAccessOp* op, void* formatterUse);
   void (PT_CCALL* ReadXattr)( PfmFormatterDispatch*, PfmMarshallerReadXattrOp* op, void* formatterUse);
   void (PT_CCALL* WriteXattr)( PfmFormatterDispatch*, PfmMarshallerWriteXattrOp* op, void* formatterUse);
};

/*interface PfmMarshaller*/ typedef struct PfmMarshaller_vtbl_t PfmMarshaller_vtbl_t, * PfmMarshaller; struct PfmMarshaller_vtbl_t
{
   void (PT_CCALL* Release)( PfmMarshaller*);
   void (PT_CCALL* SetTrace)( PfmMarshaller*, const wchar_t* traceChannelName);
   void (PT_CCALL* SetStatus)( PfmMarshaller*, PT_FD_T fd);
   void (PT_CCALL* Line)( PfmMarshaller*, const wchar_t* data, uint8_t/*bool*/ endOfLine);
   void (PT_CCALL* Print)( PfmMarshaller*, const wchar_t* data);
   void (PT_CCALL* Vprintf)( PfmMarshaller*, const wchar_t* format, va_list args);
   void (PT_CCALL* Printf)( PfmMarshaller*, const wchar_t* format, ...);
   int /*perr*/ (PT_CCALL* ConvertSystemError)( PfmMarshaller*, int err);
   int /*err*/ (PT_CCALL* Identify)( PfmMarshaller*, const void* mountFileData, size_t mountFileDataSize, const char* formatterName);
   int /*err*/ (PT_CCALL* GetPassword)( PfmMarshaller*, PT_FD_T read, const wchar_t* prompt, const wchar_t** password);
   void (PT_CCALL* ClearPassword)( PfmMarshaller*);
   void (PT_CCALL* ServeDispatch)( PfmMarshaller*, PfmMarshallerServeParams const* params_);
   void (PT_CCALL* ServeCancel)( PfmMarshaller*);
   int (PT_CCALL* GetClientVersion)( PfmMarshaller*);
   int (PT_CCALL* GetClientFlags)( PfmMarshaller*);
   int (PT_CCALL* GetClientFileNameType)( PfmMarshaller*);
};

#define INTERFACE_NAME PfmMarshallerPreMount
PT_INTERFACE_DEFINE
{
   PT_INTERFACE_FUN1( int/*error*/, PreMount, PfmFormatterDispatch* dispatch);
};
#undef INTERFACE_NAME

#ifdef PFMMARSHALLER_STATIC

PT_EXTERNC int/*error*/ PT_CCALL PfmMarshallerFactory( PfmMarshaller** marshaller);

PT_EXTERNC void PT_CCALL PfmMarshallerSetPreMount( PfmMarshaller* marshaller, PfmMarshallerPreMount* preMount);

#endif

   // Marshaller interface version history.
   //    PfmMarshaller1- 2007.12.31
   //       First public release.
   //    PfmMarshaller2- 2008.02.08
   //       Added FormatterOps::MediaInfo to return media ID and label.
   //    PfmMarshaller3- 2009.03.02
   //       Added fastpipe.
   //    PfmMarshaller4- 2009.06.03
   //       Concurrent marshaller
   //    PfmMarshaller5- 2010.04.22
   //       Move::ExistingAccessLevel
   //    PfmMarshaller6- 2011.01.27
   //       Access operation.
   //       File color attribute.
   //    PfmMarshaller7- 2013.03.11
   //       Xattr support.
   //       Mac alias flag.
   //       Client flags.
   //    PfmMarshaller8- 2013.05
   //       Symlink support.
   //    PfmMarshaller9- 2015.06
   //       Deprecated single-threaded formatter interfaces.
   //    PfmMarshaller10- 2017.06.23
   //       Mac/Windows file name conversion support.

#ifndef PFMMARSHALLER_STATIC

PTFACTORY1_DECLARE( PfmMarshaller, PFM_PRODIDW, PFM_APIIDW);
PT_INLINE int/*error*/ PT_CCALL PfmMarshallerFactory( PfmMarshaller** marshaller)
   { return PfmMarshallerGetInterface( "PfmMarshaller10", marshaller); }
// void PfmMarshallerUnload( void);

#endif

#endif
#endif
