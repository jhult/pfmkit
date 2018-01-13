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
// file name:  pfmapi.hxx
// created:    2005.12.02
//----------------------------------------------------------------------------
#ifndef PFMAPI_HXX
#define PFMAPI_HXX
#include "ptfactory1.h"
#ifdef __cplusplus_cli
#pragma managed(push,off)
#endif

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
#define PFM_DATE                    2017.11.16
#define PFM_DATEA                  "2017.11.16"
#define PFM_DATEW                 L"2017.11.16"
#define PFM_BUILDTAG                pfm.1.0.0.190
#define PFM_BUILDTAGA              "pfm.1.0.0.190"
#define PFM_BUILDTAGW             L"pfm.1.0.0.190"
#define PFM_BUILD                   190
#define PFM_BUILDA                 "190"
#define PFM_BUILDW                L"190"
#define PFM_PREFIX                  pfm
#define PFM_PREFIXA                "pfm"
#define PFM_PREFIXW               L"pfm"
#define PFM_APIID                   pfmapi
#define PFM_APIIDA                 "pfmapi"
#define PFM_APIIDW                L"pfmapi"
#define PFM_APIBASENAME             pfmapi_190
#define PFM_APIBASENAMEA           "pfmapi_190"
#define PFM_APIBASENAMEW          L"pfmapi_190"
#define PFM_KERNELBASENAME          pfmfs_190
#define PFM_KERNELBASENAMEA        "pfmfs_190"
#define PFM_KERNELBASENAMEW       L"pfmfs_190"
#define PFM_CMDBASENAME             pfm
#define PFM_CMDBASENAMEA           "pfm"
#define PFM_CMDBASENAMEW          L"pfm"


static const int pfmMountFlagReadOnly = 0x00000001;
static const int pfmMountFlagWorldRead = 0x00000004;
static const int pfmMountFlagWorldWrite = 0x00000008;
static const int pfmMountFlagUncOnly = 0x00000010;
static const int pfmMountFlagVerbose = 0x00000020;
static const int pfmMountFlagForceUnbuffered = 0x00000080;
static const int pfmMountFlagForceBuffered = 0x00000100;
static const int pfmMountFlagGroupRead = 0x00000400;
static const int pfmMountFlagGroupWrite = 0x00000800;
static const int pfmMountFlagGroupOwned = 0x00001000;
static const int pfmMountFlagWorldOwned = 0x00002000;
static const int pfmMountFlagCacheNameSpace = 0x00004000;
static const int pfmMountFlagBrowse = 0x00010000;
static const int pfmMountFlagUnmountOnRelease = 0x00020000;
static const int pfmMountFlagUnmountOnDisconnect = 0x00040000;
static const int pfmMountFlagLocalDriveType = 0x00080000;
static const int pfmMountFlagAccelerate = 0x00200000;
static const int pfmMountFlagForceDriveLetter = 0x00400000;

static const int pfmUnmountFlagBackground = 0x0001;

static const int pfmStatusFlagInitializing = 0x0001;
static const int pfmStatusFlagReady = 0x0002;
static const int pfmStatusFlagDisconnected = 0x0004;
static const int pfmStatusFlagClosed = 0x0008;

static const int pfmErrorSuccess = 0;
static const int pfmErrorDisconnect = 1;
static const int pfmErrorCancelled = 2;
static const int pfmErrorUnsupported = 3;
static const int pfmErrorInvalid = 4;
static const int pfmErrorAccessDenied = 5;
static const int pfmErrorOutOfMemory = 6;
static const int pfmErrorFailed = 7;
static const int pfmErrorNotFound = 8;
static const int pfmErrorParentNotFound = 9;
static const int pfmErrorExists = 10;
static const int pfmErrorNoSpace = 11;
static const int pfmErrorBadName = 12;
static const int pfmErrorNotEmpty = 13;
static const int pfmErrorEndOfData = 14;
static const int pfmErrorNotAFile = 15;
static const int pfmErrorDeleted = 16;
static const int pfmErrorCorruptData = 17;
static const int pfmErrorTimeout = 18;
static const int pfmErrorNotAFolder = 19;

static const int8_t pfmFileTypeNone = 0;
static const int8_t pfmFileTypeFile = 1;
static const int8_t pfmFileTypeFolder = 2;
static const int8_t pfmFileTypeSymlink = 3;

static const uint8_t pfmFileFlagsInvalid = 0xFF;

static const uint8_t pfmFileFlagReadOnly = 0x01;
static const uint8_t pfmFileFlagHidden = 0x02;
static const uint8_t pfmFileFlagSystem = 0x04;
static const uint8_t pfmFileFlagExecute = 0x08;
static const uint8_t pfmFileFlagHasIcon = 0x10;
static const uint8_t pfmFileFlagArchive = 0x20;
static const uint8_t pfmFileFlagAlias = 0x40;

static const uint8_t pfmExtraFlagOffline = 0x01;
static const uint8_t pfmExtraFlagReserved1 = 0x02;
static const uint8_t pfmExtraFlagNtfsCompressed = 0x04;
static const uint8_t pfmExtraFlagNtfsEncrypted = 0x08;

static const uint8_t pfmColorInvalid = 0;
static const uint8_t pfmColorDefault = 1;
static const uint8_t pfmColorGray = 2;
static const uint8_t pfmColorGreen = 3;
static const uint8_t pfmColorPurple = 4;
static const uint8_t pfmColorBlue = 5;
static const uint8_t pfmColorYellow = 6;
static const uint8_t pfmColorRed = 7;
static const uint8_t pfmColorOrange = 8;

static const int64_t pfmTimeInvalid = 0;

static const int8_t pfmAccessLevelReadInfo = 1;
static const int8_t pfmAccessLevelReadData = 2;
static const int8_t pfmAccessLevelWriteInfo = 3;
static const int8_t pfmAccessLevelDelete = 4;
static const int8_t pfmAccessLevelWriteData = 5;
static const int8_t pfmAccessLevelOwner = 6;

static const uint8_t pfmControlFlagForceUnbuffered = 1;
static const uint8_t pfmControlFlagForceBuffered = 2;
static const uint8_t pfmControlFlagSlowAppends = 4;

static const int pfmClientFlagXattr = 0x0001;
static const int pfmClientFlagAccelerate = 0x0002;

static const int8_t pfmFileNameTypeAny = 0x00;
static const int8_t pfmFileNameTypeWindows = 0x01;
static const int8_t pfmFileNameTypeUnix = 0x02;
static const int8_t pfmFileNameTypeMac = 0x03;

static const int pfmVolumeFlagReadOnly = 0x0001;
static const int pfmVolumeFlagCompressed = 0x0002;
static const int pfmVolumeFlagEncrypted = 0x0004;
static const int pfmVolumeFlagCaseSensitive = 0x0008;
static const int pfmVolumeFlagTouchMap = 0x0010;
static const int pfmVolumeFlagNoCreateTime = 0x0100;
static const int pfmVolumeFlagNoAccessTime = 0x0200;
static const int pfmVolumeFlagNoWriteTime = 0x0400;
static const int pfmVolumeFlagNoChangeTime = 0x0800;
static const int pfmVolumeFlagXattr = 0x1000;
static const int pfmVolumeFlagSymlinks = 0x2000;
static const int pfmVolumeFlagFakeNamedStreams = 0x4000;
static const int pfmVolumeFlagAccelerate = 0x8000;

static const int pfmFlushFlagOpen = 0x0001;

struct PfmMountCreateParams
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

   PfmMountCreateParams()
      { memset( this, 0, sizeof( *this)); paramsSize = sizeof( *this);
      mountFlags = pfmMountFlagCacheNameSpace|pfmMountFlagUnmountOnDisconnect;
      toFormatterWrite = fromFormatterRead = toAuthWrite = fromAuthRead = PT_FD_INVALID; }
};

struct/*interface*/ PfmMount
{
   virtual void PT_CCALL AddRef() = 0;
   virtual void PT_CCALL Release() = 0;
   virtual int /*err*/ PT_CCALL Refresh() = 0;
   virtual int PT_CCALL GetMountId() = 0;
   virtual int PT_CCALL GetMountFlags() = 0;
   virtual int PT_CCALL GetStatusFlags() = 0;
   virtual int PT_CCALL GetVolumeFlags() = 0;
   virtual int64_t PT_CCALL GetChangeInstance() = 0;
   virtual const wchar_t* PT_CCALL GetMountSourceName() = 0;
   virtual const wchar_t* PT_CCALL GetMountPoint() = 0;
   virtual const wchar_t* PT_CCALL GetUncName() = 0;
   virtual const wchar_t* PT_CCALL GetOwnerId() = 0;
   virtual const wchar_t* PT_CCALL GetOwnerName() = 0;
   virtual const wchar_t* PT_CCALL GetFormatterName() = 0;
   virtual wchar_t PT_CCALL GetDriveLetter() = 0;
   virtual int /*err*/ PT_CCALL WaitReady( int timeoutMsecs) = 0;
   virtual int /*err*/ PT_CCALL Unmount( int unmountFlags) = 0;
   virtual int /*err*/ PT_CCALL Flush() = 0;
   virtual int /*err*/ PT_CCALL Control( int controlCode, const void* input, size_t inputSize, void* output, size_t maxOutputSize, size_t* outputSize) = 0;
   virtual const wchar_t* PT_CCALL GetMountEndName() = 0;
   virtual const wchar_t* PT_CCALL GetAppId() = 0;
   virtual const char* PT_CCALL GetReport( int reportFlags) = 0;
};

struct/*interface*/ PfmFileMountUi
{
   virtual void PT_CCALL Start() = 0;
   virtual void PT_CCALL Complete( const wchar_t* errorMessage) = 0;
   virtual void PT_CCALL Status( const wchar_t* data, int/*bool*/ endOfLine) = 0;
   virtual void PT_CCALL Suspend() = 0;
   virtual void PT_CCALL Resume() = 0;
   virtual const wchar_t* PT_CCALL QueryPassword( const wchar_t* prompt, int count) = 0;
   virtual void PT_CCALL ClearPassword() = 0;
};

static const int pfmFileMountFlagConsoleUi = 0x0001;
static const int pfmFileMountFlagInProcess = 0x0002;
static const int pfmFileMountFlagVerbose = 0x0004;
static const int pfmFileMountFlagReserved1 = 0x0008;
static const int pfmFileMountFlagEditOptions = 0x0020;
static const int pfmFileMountFlagMultiMount = 0x0040;
static const int pfmFileMountFlagSkipExisting = 0x0080;

struct PfmFileMountCreateParams
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

   PfmFileMountCreateParams()
      { memset( this, 0, sizeof( *this)); this->paramsSize = sizeof( *this);
      this->mountFlags = pfmMountFlagCacheNameSpace|pfmMountFlagUnmountOnDisconnect; }
};

struct/*interface*/ PfmFileMount
{
   virtual void PT_CCALL AddRef() = 0;
   virtual void PT_CCALL Release() = 0;
   virtual int /*err*/ PT_CCALL GetInterface( const char* id, void*) = 0;
   virtual void PT_CCALL Cancel() = 0;
   virtual int /*err*/ PT_CCALL Start( PfmFileMountCreateParams const* params_) = 0;
   virtual void PT_CCALL Send( const wchar_t* data, int/*bool*/ endOfLine) = 0;
   virtual void PT_CCALL Status( const wchar_t* data, int/*bool*/ endOfLine) = 0;
   virtual int /*err*/ PT_CCALL WaitReady() = 0;
   virtual PfmMount* PT_CCALL GetMount() = 0;
   virtual void PT_CCALL Detach() = 0;
};

struct/*interface*/ PfmMountIterator
{
   virtual void PT_CCALL AddRef() = 0;
   virtual void PT_CCALL Release() = 0;
   virtual int /*mountId*/ PT_CCALL Next( int64_t* changeInstance) = 0;
};

struct/*interface*/ PfmMountMonitor
{
   virtual void PT_CCALL AddRef() = 0;
   virtual void PT_CCALL Release() = 0;
   virtual int /*err*/ PT_CCALL Wait( int64_t nextChangeInstance, int timeoutMsecs) = 0;
   virtual void PT_CCALL Cancel() = 0;
};

static const int pfmFastPipeFlagFastMapping = 0x0002;
static const int pfmFastPipeFlagNamedDevice = 0x0004;
static const int pfmFastPipeFlagAsyncClient = 0x0008;
static const int pfmFastPipeFlagAsyncServer = 0x0010;

struct PfmFastPipeCreateParams
{
   size_t paramsSize;
   const wchar_t* baseDeviceName;
   int fastPipeFlags;
   #if UINT_MAX < SIZE_MAX
   int reserved1[1];
   #endif
   wchar_t* deviceName;
   size_t maxDeviceNameChars;

   PfmFastPipeCreateParams() { memset( this, 0, sizeof( *this)); this->paramsSize = sizeof( *this); }
};

static const int pfmFastPipeOpTypeRead = 1;
static const int pfmFastPipeOpTypeWrite = 2;
static const int pfmFastPipeOpTypeSend = 3;

struct PfmFastPipeOp
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

struct PfmFastPipeSendContext;

typedef void (PT_CCALL* PfmFastPipeSendContext_Complete)( PfmFastPipeSendContext*, int err, size_t inputSize, size_t outputSize);

struct PfmFastPipeSendContext
{
   PfmFastPipeSendContext_Complete complete;
   #if UINT_MAX == SIZE_MAX
   uint8_t align1[4];
   #endif
   uint64_t opaque[(sizeof(void*)*10+40)/8];
};

struct/*interface*/ PfmFastPipeServer
{
   virtual void PT_CCALL Release() = 0;
   virtual void PT_CCALL InitOp( PfmFastPipeOp* op) = 0;
   virtual void PT_CCALL FreeOp( PfmFastPipeOp* op) = 0;
   virtual int /*err*/ PT_CCALL Receive( PfmFastPipeOp* op) = 0;
   virtual void PT_CCALL Complete( PfmFastPipeOp* op, int err, size_t inputSize, size_t outputSize) = 0;
   virtual void PT_CCALL Cancel() = 0;
};

struct/*interface*/ PfmApi
{
   virtual void PT_CCALL AddRef() = 0;
   virtual void PT_CCALL Release() = 0;
   virtual const char* PT_CCALL Version() = 0;
   virtual int /*err*/ PT_CCALL SysStart() = 0;
   virtual int /*err*/ PT_CCALL MountCreate( PfmMountCreateParams const* params_, PfmMount** mount) = 0;
   virtual int /*err*/ PT_CCALL MountSourceNameOpen( const wchar_t* mountSourceName, PfmMount** mount) = 0;
   virtual int /*err*/ PT_CCALL MountPointOpen( const wchar_t* mountPoint, PfmMount** mount) = 0;
   virtual int /*err*/ PT_CCALL MountIdOpen( int mountId, PfmMount** mount) = 0;
   virtual int /*err*/ PT_CCALL MountIterate( int64_t startChangeInstance, int64_t* nextChangeInstance, PfmMountIterator** iterator) = 0;
   virtual int /*err*/ PT_CCALL MountMonitorFactory( PfmMountMonitor** monitor) = 0;
   virtual int /*err*/ PT_CCALL FileMountFactory( PfmFileMount** fileMount) = 0;
   virtual int /*err*/ PT_CCALL FastPipeCreate( PfmFastPipeCreateParams const* params_, PT_FD_T* clientFd, PT_FD_T* serverFd) = 0;
   virtual int /*err*/ PT_CCALL FastPipeCancel( PT_FD_T) = 0;
   virtual int /*err*/ PT_CCALL FastPipeEnableFastMapping( PT_FD_T clientFd) = 0;
   virtual int /*err*/ PT_CCALL FastPipeServerFactory( PT_FD_T serverFd, PfmFastPipeServer**) = 0;
   virtual int /*err*/ PT_CCALL FastPipeClientContext( PT_FD_T clientFd, int* clientContext) = 0;
   virtual void PT_CCALL FastPipeSend( PT_FD_T clientFd, int clientContext, uint64_t offset, const void* input, size_t maxInputSize, void* output, size_t maxOutputSize, PfmFastPipeSendContext*) = 0;
   virtual int /*err*/ PT_CCALL MountEndNameOpen( const wchar_t* mountEndName, PfmMount** mount) = 0;
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

static const int pfmInstInstalled = 0;
static const int pfmInstOldBuild = 1;
static const int pfmInstOldVersion = 2;
static const int pfmInstNotInstalled = 3;

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

struct PfmAttribs
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

struct PfmOpenAttribs
{
   int64_t openId;
   int64_t openSequence;
   int8_t accessLevel;
   uint8_t controlFlags;
   uint8_t reserved1[2];
   unsigned touch;
   PfmAttribs attribs;
};

struct PfmNamePart
{
   const wchar_t* name;
   size_t len;
   const char* name8;
   size_t len8;
};

struct PfmMediaInfo
{
   PT_UUID mediaUuid;
   uint64_t mediaId64;
   unsigned mediaId32;
   uint8_t mediaFlags;
   uint8_t reserved1[3];
   int64_t createTime;
};

static const unsigned pfmOpFormatterUseSize = 20*sizeof(int)+20*sizeof(void*);

struct/*interface*/ PfmFormatterDispatch;

struct PfmMarshallerServeParams
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

   PfmMarshallerServeParams()
      { memset( this, 0, sizeof( *this)); paramsSize = sizeof( *this);
      toFormatterRead = fromFormatterWrite = PT_FD_INVALID; }
};

struct/*interface*/ PfmFormatterSerializeOpen
{
   virtual void PT_CCALL SerializeOpen( int64_t openId, int64_t* openSequence) = 0;
};

struct/*interface*/ PfmMarshallerOpenOp
{
   virtual PfmNamePart const* PT_CCALL NameParts() = 0;
   virtual size_t PT_CCALL NamePartCount() = 0;
   virtual int8_t PT_CCALL CreateFileType() = 0;
   virtual uint8_t PT_CCALL CreateFileFlags() = 0;
   virtual int64_t PT_CCALL WriteTime() = 0;
   virtual int64_t PT_CCALL NewCreateOpenId() = 0;
   virtual int8_t PT_CCALL ExistingAccessLevel() = 0;
   virtual int64_t PT_CCALL NewExistingOpenId() = 0;
   virtual void PT_CCALL Complete( int perr, uint8_t/*bool*/ existed, PfmOpenAttribs const* openAttribs, int64_t parentFileId, const wchar_t* endName, size_t linkNamePartCount, const void* linkData, size_t linkDataSize, PfmFormatterSerializeOpen* serializeOpen) = 0;
};

struct/*interface*/ PfmMarshallerReplaceOp
{
   virtual int64_t PT_CCALL TargetOpenId() = 0;
   virtual int64_t PT_CCALL TargetParentFileId() = 0;
   virtual PfmNamePart const* PT_CCALL TargetEndName() = 0;
   virtual uint8_t PT_CCALL CreateFileFlags() = 0;
   virtual int64_t PT_CCALL WriteTime() = 0;
   virtual int64_t PT_CCALL NewCreateOpenId() = 0;
   virtual void PT_CCALL Complete( int perr, PfmOpenAttribs const* openAttribs, PfmFormatterSerializeOpen* serializeOpen) = 0;
};

struct/*interface*/ PfmMarshallerMoveOp
{
   virtual int64_t PT_CCALL SourceOpenId() = 0;
   virtual int64_t PT_CCALL SourceParentFileId() = 0;
   virtual PfmNamePart const* PT_CCALL SourceEndName() = 0;
   virtual PfmNamePart const* PT_CCALL TargetNameParts() = 0;
   virtual size_t PT_CCALL TargetNamePartCount() = 0;
   virtual uint8_t/*bool*/ PT_CCALL DeleteSource() = 0;
   virtual int64_t PT_CCALL WriteTime() = 0;
   virtual int8_t PT_CCALL ExistingAccessLevel() = 0;
   virtual int64_t PT_CCALL NewExistingOpenId() = 0;
   virtual void PT_CCALL Complete( int perr, uint8_t/*bool*/ existed, PfmOpenAttribs const* openAttribs, int64_t parentFileId, const wchar_t* endName, size_t linkNamePartCount, const void* linkData, size_t linkDataSize, PfmFormatterSerializeOpen* serializeOpen) = 0;
};

struct/*interface*/ PfmMarshallerMoveReplaceOp
{
   virtual int64_t PT_CCALL SourceOpenId() = 0;
   virtual int64_t PT_CCALL SourceParentFileId() = 0;
   virtual PfmNamePart const* PT_CCALL SourceEndName() = 0;
   virtual int64_t PT_CCALL TargetOpenId() = 0;
   virtual int64_t PT_CCALL TargetParentFileId() = 0;
   virtual PfmNamePart const* PT_CCALL TargetEndName() = 0;
   virtual uint8_t/*bool*/ PT_CCALL DeleteSource() = 0;
   virtual int64_t PT_CCALL WriteTime() = 0;
   virtual void PT_CCALL Complete( int perr) = 0;
};

struct/*interface*/ PfmMarshallerDeleteOp
{
   virtual int64_t PT_CCALL OpenId() = 0;
   virtual int64_t PT_CCALL ParentFileId() = 0;
   virtual PfmNamePart const* PT_CCALL EndName() = 0;
   virtual int64_t PT_CCALL WriteTime() = 0;
   virtual void PT_CCALL Complete( int perr) = 0;
};

struct/*interface*/ PfmMarshallerCloseOp
{
   virtual int64_t PT_CCALL OpenId() = 0;
   virtual int64_t PT_CCALL OpenSequence() = 0;
   virtual void PT_CCALL Complete( int perr) = 0;
};

struct/*interface*/ PfmMarshallerFlushFileOp
{
   virtual int64_t PT_CCALL OpenId() = 0;
   virtual uint8_t PT_CCALL FlushFlags() = 0;
   virtual uint8_t PT_CCALL FileFlags() = 0;
   virtual uint8_t PT_CCALL Color() = 0;
   virtual int64_t PT_CCALL CreateTime() = 0;
   virtual int64_t PT_CCALL AccessTime() = 0;
   virtual int64_t PT_CCALL WriteTime() = 0;
   virtual int64_t PT_CCALL ChangeTime() = 0;
   virtual void PT_CCALL Complete( int perr, PfmOpenAttribs const* openAttribs, PfmFormatterSerializeOpen* serializeOpen) = 0;
   virtual const void* PT_CCALL LinkData() = 0;
   virtual unsigned PT_CCALL LinkDataSize() = 0;
};

struct/*interface*/ PfmMarshallerListOp
{
   virtual int64_t PT_CCALL OpenId() = 0;
   virtual int64_t PT_CCALL ListId() = 0;
   virtual uint8_t/*bool*/ PT_CCALL Add( PfmAttribs const* attribs, const wchar_t* endName) = 0;
   virtual uint8_t/*bool*/ PT_CCALL Add8( PfmAttribs const* attribs, const char* endName) = 0;
   virtual void PT_CCALL Complete( int perr, uint8_t/*bool*/ noMore) = 0;
};

struct/*interface*/ PfmMarshallerListEndOp
{
   virtual int64_t PT_CCALL OpenId() = 0;
   virtual int64_t PT_CCALL ListId() = 0;
   virtual void PT_CCALL Complete( int perr) = 0;
};

struct/*interface*/ PfmMarshallerReadOp
{
   virtual int64_t PT_CCALL OpenId() = 0;
   virtual uint64_t PT_CCALL FileOffset() = 0;
   virtual void* PT_CCALL Data() = 0;
   virtual size_t PT_CCALL RequestedSize() = 0;
   virtual void PT_CCALL Complete( int perr, size_t actualSize) = 0;
};

struct/*interface*/ PfmMarshallerWriteOp
{
   virtual int64_t PT_CCALL OpenId() = 0;
   virtual uint64_t PT_CCALL FileOffset() = 0;
   virtual const void* PT_CCALL Data() = 0;
   virtual size_t PT_CCALL RequestedSize() = 0;
   virtual void PT_CCALL Complete( int perr, size_t actualSize) = 0;
};

struct/*interface*/ PfmMarshallerSetSizeOp
{
   virtual int64_t PT_CCALL OpenId() = 0;
   virtual uint64_t PT_CCALL FileSize() = 0;
   virtual void PT_CCALL Complete( int perr) = 0;
};

struct/*interface*/ PfmMarshallerCapacityOp
{
   virtual int64_t PT_CCALL OpenId() = 0;
   virtual void PT_CCALL Complete( int perr, uint64_t totalCapacity, uint64_t availableCapacity) = 0;
};

struct/*interface*/ PfmMarshallerFlushMediaOp
{
   virtual void PT_CCALL Complete( int perr, int msecFlushDelay) = 0;
};

struct/*interface*/ PfmMarshallerControlOp
{
   virtual int64_t PT_CCALL OpenId() = 0;
   virtual int8_t PT_CCALL AccessLevel() = 0;
   virtual int PT_CCALL ControlCode() = 0;
   virtual const void* PT_CCALL Input() = 0;
   virtual size_t PT_CCALL InputSize() = 0;
   virtual void* PT_CCALL Output() = 0;
   virtual size_t PT_CCALL MaxOutputSize() = 0;
   virtual void PT_CCALL Complete( int perr, size_t outputSize) = 0;
};

struct/*interface*/ PfmMarshallerMediaInfoOp
{
   virtual int64_t PT_CCALL OpenId() = 0;
   virtual void PT_CCALL Complete( int perr, PfmMediaInfo const* mediaInfo, const wchar_t* mediaLabel) = 0;
};

struct/*interface*/ PfmMarshallerAccessOp
{
   virtual int64_t PT_CCALL OpenId() = 0;
   virtual int8_t PT_CCALL AccessLevel() = 0;
   virtual void PT_CCALL Complete( int perr, PfmOpenAttribs const* openAttribs, PfmFormatterSerializeOpen* serializeOpen) = 0;
};

struct/*interface*/ PfmMarshallerReadXattrOp
{
   virtual int64_t PT_CCALL OpenId() = 0;
   virtual PfmNamePart const* PT_CCALL Name() = 0;
   virtual unsigned PT_CCALL Offset() = 0;
   virtual void* PT_CCALL Data() = 0;
   virtual size_t PT_CCALL RequestedSize() = 0;
   virtual void PT_CCALL Complete( int perr, unsigned xattrSize, size_t transferredSize) = 0;
};

struct/*interface*/ PfmMarshallerWriteXattrOp
{
   virtual int64_t PT_CCALL OpenId() = 0;
   virtual PfmNamePart const* PT_CCALL Name() = 0;
   virtual unsigned PT_CCALL XattrSize() = 0;
   virtual unsigned PT_CCALL Offset() = 0;
   virtual const void* PT_CCALL Data() = 0;
   virtual size_t PT_CCALL RequestedSize() = 0;
   virtual void PT_CCALL Complete( int perr, size_t transferredSize) = 0;
};

struct/*interface*/ PfmFormatterDispatch
{
   virtual void PT_CCALL Open( PfmMarshallerOpenOp* op, void* formatterUse) = 0;
   virtual void PT_CCALL Replace( PfmMarshallerReplaceOp* op, void* formatterUse) = 0;
   virtual void PT_CCALL Move( PfmMarshallerMoveOp* op, void* formatterUse) = 0;
   virtual void PT_CCALL MoveReplace( PfmMarshallerMoveReplaceOp* op, void* formatterUse) = 0;
   virtual void PT_CCALL Delete( PfmMarshallerDeleteOp* op, void* formatterUse) = 0;
   virtual void PT_CCALL Close( PfmMarshallerCloseOp* op, void* formatterUse) = 0;
   virtual void PT_CCALL FlushFile( PfmMarshallerFlushFileOp* op, void* formatterUse) = 0;
   virtual void PT_CCALL List( PfmMarshallerListOp* op, void* formatterUse) = 0;
   virtual void PT_CCALL ListEnd( PfmMarshallerListEndOp* op, void* formatterUse) = 0;
   virtual void PT_CCALL Read( PfmMarshallerReadOp* op, void* formatterUse) = 0;
   virtual void PT_CCALL Write( PfmMarshallerWriteOp* op, void* formatterUse) = 0;
   virtual void PT_CCALL SetSize( PfmMarshallerSetSizeOp* op, void* formatterUse) = 0;
   virtual void PT_CCALL Capacity( PfmMarshallerCapacityOp* op, void* formatterUse) = 0;
   virtual void PT_CCALL FlushMedia( PfmMarshallerFlushMediaOp* op, void* formatterUse) = 0;
   virtual void PT_CCALL Control( PfmMarshallerControlOp* op, void* formatterUse) = 0;
   virtual void PT_CCALL MediaInfo( PfmMarshallerMediaInfoOp* op, void* formatterUse) = 0;
   virtual void PT_CCALL Access( PfmMarshallerAccessOp* op, void* formatterUse) = 0;
   virtual void PT_CCALL ReadXattr( PfmMarshallerReadXattrOp* op, void* formatterUse) = 0;
   virtual void PT_CCALL WriteXattr( PfmMarshallerWriteXattrOp* op, void* formatterUse) = 0;
};

struct/*interface*/ PfmMarshaller
{
   virtual void PT_CCALL Release() = 0;
   virtual void PT_CCALL SetTrace( const wchar_t* traceChannelName) = 0;
   virtual void PT_CCALL SetStatus( PT_FD_T fd) = 0;
   virtual void PT_CCALL Line( const wchar_t* data, uint8_t/*bool*/ endOfLine) = 0;
   virtual void PT_CCALL Print( const wchar_t* data) = 0;
   virtual void PT_CCALL Vprintf( const wchar_t* format, va_list args) = 0;
   virtual void PT_CCALL Printf( const wchar_t* format, ...) = 0;
   virtual int /*perr*/ PT_CCALL ConvertSystemError( int err) = 0;
   virtual int /*err*/ PT_CCALL Identify( const void* mountFileData, size_t mountFileDataSize, const char* formatterName) = 0;
   virtual int /*err*/ PT_CCALL GetPassword( PT_FD_T read, const wchar_t* prompt, const wchar_t** password) = 0;
   virtual void PT_CCALL ClearPassword() = 0;
   virtual void PT_CCALL ServeDispatch( PfmMarshallerServeParams const* params_) = 0;
   virtual void PT_CCALL ServeCancel() = 0;
   virtual int PT_CCALL GetClientVersion() = 0;
   virtual int PT_CCALL GetClientFlags() = 0;
   virtual int PT_CCALL GetClientFileNameType() = 0;
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

#ifdef __cplusplus_cli
#pragma managed(pop)
#endif
#endif
