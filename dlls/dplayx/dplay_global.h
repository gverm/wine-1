#ifndef __WINE_DPLAY_GLOBAL_INCLUDED
#define __WINE_DPLAY_GLOBAL_INCLUDED

#include "dplaysp.h"
#include "dplayx_queue.h"

extern HRESULT DPL_EnumAddress( LPDPENUMADDRESSCALLBACK lpEnumAddressCallback, 
                                LPCVOID lpAddress, DWORD dwAddressSize, 
                                LPVOID lpContext );

extern DWORD DP_CalcSessionDescSize( LPCDPSESSIONDESC2 lpSessDesc, BOOL bAnsi );

/*****************************************************************************
 * Predeclare the interface implementation structures
 */
typedef struct IDirectPlay2Impl IDirectPlay2AImpl;
typedef struct IDirectPlay2Impl IDirectPlay2Impl;
typedef struct IDirectPlay3Impl IDirectPlay3AImpl;
typedef struct IDirectPlay3Impl IDirectPlay3Impl;
typedef struct IDirectPlay4Impl IDirectPlay4AImpl;
typedef struct IDirectPlay4Impl IDirectPlay4Impl;

typedef struct tagDirectPlayIUnknownData
{
  ULONG             ulObjRef;
  CRITICAL_SECTION  DP_lock;
} DirectPlayIUnknownData;

typedef struct tagEnumSessionAsyncCallbackData
{
  LPSPINITDATA lpSpData;
  GUID         requestGuid;
  DWORD        dwEnumSessionFlags;
  DWORD        dwTimeout;
  HANDLE       hSuicideRequest;
} EnumSessionAsyncCallbackData;

struct PlayerData
{
  /* Individual player information */
  DPID dpid;

  DPNAME name;
  HANDLE hEvent;

  /* View of local data */
  LPVOID lpLocalData;
  DWORD  dwLocalDataSize;

  /* View of remote data */
  LPVOID lpRemoteData;
  DWORD  dwRemoteDataSize;

  DWORD  dwFlags; /* Special remarks about the type of player */
};
typedef struct PlayerData* lpPlayerData;

struct PlayerList
{
  DPQ_ENTRY(PlayerList) players;

  lpPlayerData lpPData;
};
typedef struct PlayerList* lpPlayerList;

struct GroupData
{
  /* Internal information */
  DPID parent; /* If parent == 0 it's a top level group */

  DPQ_HEAD(GroupList)  groups;  /* A group has [0..n] groups */
  DPQ_HEAD(PlayerList) players; /* A group has [0..n] players */

  DPID idGroupOwner; /* ID of player who owns the group */

  DWORD dwFlags; /* Flags describing anything special about the group */

  DPID   dpid;
  DPNAME name;

  /* View of local data */
  LPVOID lpLocalData;
  DWORD  dwLocalDataSize;

  /* View of remote data */
  LPVOID lpRemoteData;
  DWORD  dwRemoteDataSize;
};
typedef struct GroupData  GroupData;
typedef struct GroupData* lpGroupData;

struct GroupList
{
  DPQ_ENTRY(GroupList) groups;

  lpGroupData lpGData;
};
typedef struct GroupList* lpGroupList;

struct DPMSG
{
  DPQ_ENTRY( DPMSG ) msgs;
  DPMSG_GENERIC* msg;
};
typedef struct DPMSG* LPDPMSG;

/* Contains all dp1 and dp2 data members */
typedef struct tagDirectPlay2Data
{
  BOOL   bConnectionOpen;

  /* For async EnumSessions requests */
  HANDLE hEnumSessionThread;
  HANDLE hKillEnumSessionThreadEvent;

  LPVOID lpNameServerData; /* DPlay interface doesn't know contents */

  BOOL bHostInterface; /* Did this interface create the session */

#if 0
  DPQ_HEAD(PlayerList) players; /* All players w/ interface */
  DPQ_HEAD(GroupList)  groups;  /* All main groups w/ interface */
#else 
  lpGroupData lpSysGroup; /* System group with _everything_ in it */
#endif

  LPDPSESSIONDESC2 lpSessionDesc;

  /* I/O Msg queues */
  DPQ_HEAD( DPMSG ) receiveMsgs; /* Msg receive queue */
  DPQ_HEAD( DPMSG ) sendMsgs;    /* Msg send pending queue */

  /* Information about the service provider active on this connection */
  SPINITDATA spData;

  /* Our service provider */
  HMODULE hServiceProvider;

  BOOL bConnectionInitialized;
} DirectPlay2Data;

typedef struct tagDirectPlay3Data
{
  BOOL dummy;
} DirectPlay3Data;
typedef struct tagDirectPlay4Data
{
  BOOL dummy;
} DirectPlay4Data;

#define DP_IMPL_FIELDS \
  ULONG ulInterfaceRef; \
  DirectPlayIUnknownData*  unk; \
  DirectPlay2Data*         dp2; \
  DirectPlay3Data*         dp3; \
  DirectPlay4Data*         dp4;

struct IDirectPlay2Impl
{
  ICOM_VFIELD(IDirectPlay2);
  DP_IMPL_FIELDS
};

struct IDirectPlay3Impl
{
  ICOM_VFIELD(IDirectPlay3);
  DP_IMPL_FIELDS
};

struct IDirectPlay4Impl
{
  ICOM_VFIELD(IDirectPlay4);
  DP_IMPL_FIELDS
};

/* Forward declarations of virtual tables */
extern ICOM_VTABLE(IDirectPlay2) directPlay2AVT;
extern ICOM_VTABLE(IDirectPlay3) directPlay3AVT;
extern ICOM_VTABLE(IDirectPlay4) directPlay4AVT;

extern ICOM_VTABLE(IDirectPlay2) directPlay2WVT;
extern ICOM_VTABLE(IDirectPlay3) directPlay3WVT;
extern ICOM_VTABLE(IDirectPlay4) directPlay4WVT;

#endif /* __WINE_DPLAY_GLOBAL_INCLUDED */
