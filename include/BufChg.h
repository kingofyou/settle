#ifndef TL_HDR_FILE_BUFCHG
#define TL_HDR_FILE_BUFCHG

#include <string.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>

/***
#include "DbsDef.h"
#include "DbsTbl.h"
#include "HtLog.h"
****/

#define NMMaxIpcDftLen					3000
#define NMMaxIpcDftN					200

#define NMMaxFldDspN                    2000
#define NMMaxBufFldN                    500
#define NMMaxBufDspN                    500
#define NMMaxFldTsfN                    500
#define NMMaxBufChgN                    500

#define NMFldTypeStr                    0

#define M_SOUR_FLD_TYPE(x)              ((x & 0xFFF0) >> 4)
#define M_DEST_FLD_TYPE(x)              (x & 0x000F)
#define M_IS_POSITION(x)                (x <= -5000)
#define M_POSITION(x)                   (-5000 - x)
#define M_IS_SPACE(x)                   ((x < 0) && \
                                         (x > -5000))
#define M_SPACE_LEN(x)                  (0 - x)

typedef struct
{
    int                               iFldId;
    int                               nFldL;
    int                               nFldType;
} fdeMFldDspEleDef;

typedef struct
{
    int                               nFldN;
    fdeMFldDspEleDef                    fdeaFldEle[NMMaxFldDspN];
} fdiMFldDspInfDef;

typedef struct
{
    int                               iFldId;
    int                               iFldX;
    int                               nOffset;
} bfeMBufFldEleDef;

typedef struct
{
    int                               nFldN;
    bfeMBufFldEleDef                    bfeaBufEle[NMMaxBufFldN];
} bdiMBufDspInfDef;

typedef struct
{
    int                               nDestOffset;
    int                               nSourOffset;
    int                               nFldL;
    int                               nChgType;
} fceMFldChgEleDef;

typedef struct
{
    int                               nFldN;
    int                               iBufChgX;
    fceMFldChgEleDef                    fceaFldEle[NMMaxFldTsfN];
} bceMBufChgEleDef;

typedef struct
{
    int                               nBufN;
    bceMBufChgEleDef                    bceaBufEle[NMMaxBufChgN];
} bciMBufChgInfDef;

typedef struct
{
    char	saCompKey[11];
    int	nCompKeyLen;
    int	nIpcDftIndex;
    int	nBufDspIndex;
    char	saTxnNum[5];
    char	saIpcDft[NMMaxIpcDftLen];
    int	nIpcDftLen;
} stIpcDftDef;

typedef struct
{
    int txn_num;
    char    val[36];
} IPCTblConInf;

typedef struct
{
    int          nCount;
    IPCTblConInf ipcTblConInf[1000];
} IPCRuleInf;

typedef struct
{
    int		nIpcDftN;
    stIpcDftDef	stIpcDft[NMMaxIpcDftN];
} stIpcDftRuleDef;


int IpcDftLoad(int nIpcDftKey,
               stIpcDftRuleDef *stIpcDftRule);

int IpcDftOpr(void *vvpInBuf,
              int *vnpOutBufLen,
              void *vvpOutBuf,
              void *vvpTxnNum,
              int *vnpBufChgIndex,
              stIpcDftRuleDef *stIpcDftRule);

int BufChgLoad(int nBufChgKey,
               bciMBufChgInfDef *gbcipBufChgInf);

int BufChgOpr(int viBufChgX,
              void *vvpSourBuf,
              void *vvpDestBuf,
              bciMBufChgInfDef *gbcipBufChgInf);

IPCRuleInf   ipcRuleInf;

#endif
