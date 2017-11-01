/*****************************************************************************/
/*                NCUP -- Shanghai Huateng Software System Inc.              */
/*****************************************************************************/
/* PROGRAM NAME: BufChg.c                                                  */
/* DESCRIPTIONS:                                                             */
/*****************************************************************************/
/*                             MODIFICATION LOG                              */
/* DATE        PROGRAMMER     DESCRIPTION                                    */
/*                                                                           */
/*****************************************************************************/
static char *Id = "@(#)$Header: /home/ctepub/cvs/ctedev/batch/src/Convert/BufChg.c,v 1.1.1.1 2012/06/08 07:50:32 ctedev Exp $";

#include "BufChg.h"
#include "batch.h"
#include "DbsTbl.h"
extern  char    gLogFile[LOG_NAME_LEN_MAX];
static
int nMFldDspLoad(
    int                          vlFldDspKey,
    fdiMFldDspInfDef              *fdipFldDspInf)
{

    int                               liX;
    int                               llResult;
    Tbl_fld_dsp_Def                     ltTblFldDsp;

    fdipFldDspInf->nFldN = 0;
    ltTblFldDsp.usage_key = 0;
    DbsFLDDSP( DBS_CURSOR, &ltTblFldDsp);

    if(llResult = DbsFLDDSP( DBS_OPEN, &ltTblFldDsp))
        return llResult;

    while(DbsFLDDSP( DBS_FETCH, &ltTblFldDsp) == 0)
    {
        if((liX = fdipFldDspInf->nFldN++) >= NMMaxFldDspN)
        {
            DbsFLDDSP( DBS_CLOSE, &ltTblFldDsp);
            return -1;
        } /* end of if */
        /*		HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__,__LINE__,"[%d][%d]",fdipFldDspInf->nFldN);*/
        fdipFldDspInf->fdeaFldEle[liX].iFldId = ltTblFldDsp.fld_id;
        fdipFldDspInf->fdeaFldEle[liX].nFldL = ltTblFldDsp.fld_l;
        fdipFldDspInf->fdeaFldEle[liX].nFldType = ltTblFldDsp.fld_type;
    } /* end of while */

    if(llResult = DbsFLDDSP( DBS_CLOSE, &ltTblFldDsp))
        return llResult;

    return 0;
} /* end of nMFldDspLoad */

static
int nMFldDspGet(
    int                          viFldId,
    fdiMFldDspInfDef              *fdipFldDspInf,
    fdeMFldDspEleDef             **fdeppFldDspEle)
{

    int                               liX;
    /*HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__,__LINE__,"[%d][%d]",fdipFldDspInf->nFldN,viFldId);*/
    for(liX = 0; liX < fdipFldDspInf->nFldN; liX++)
        if(viFldId == fdipFldDspInf->fdeaFldEle[liX].iFldId)
        {
            *fdeppFldDspEle = &fdipFldDspInf->fdeaFldEle[liX];
            return 0 ;
        } /* end of if */
    HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "22222222[%d][%d]", fdipFldDspInf->nFldN, viFldId);
    return -1;
} /* end of nMFldDspGet */

static
int nMBufDspOffsetGet(
    int                          viFldX,
    bdiMBufDspInfDef              *vbdipBufDspInf,
    bfeMBufFldEleDef             **vbfeppBufFldEle)
{

    int                               liX;

    for(liX = 0; liX < vbdipBufDspInf->nFldN; liX++)
        if(viFldX == vbdipBufDspInf->bfeaBufEle[liX].iFldX)
        {
            *vbfeppBufFldEle = &vbdipBufDspInf->bfeaBufEle[liX];
            return 0;
        } /* end of if */
    return -1;
} /* end of nMBufDspOffsetGet */

static
int nMBufDspLoad(
    int                          vlBufDspKey,
    int                          viBufDspX,
    fdiMFldDspInfDef              *vfdipFldDspInf,
    bdiMBufDspInfDef              *vbdipBufDspInf)
{

    int                               liX;
    int                               lnOffset;
    int                               llResult;
    Tbl_buf_dsp_Def                     ltTblBufDsp;
    fdeMFldDspEleDef                   *lfdepFldDspEle;

    vbdipBufDspInf->nFldN = 0;
    ltTblBufDsp.usage_key = vlBufDspKey;
    ltTblBufDsp.buf_dsp_index = viBufDspX;
    DbsBUFDSP( DBS_CURSOR, &ltTblBufDsp);

    if(llResult = DbsBUFDSP( DBS_OPEN, &ltTblBufDsp))
        return llResult;

    lnOffset = 0;
    while(DbsBUFDSP( DBS_FETCH, &ltTblBufDsp) == 0)
    {
        if(M_IS_POSITION(ltTblBufDsp.fld_id))
        {
            lnOffset = M_POSITION(ltTblBufDsp.fld_id);
            continue;
        }
        if(M_IS_SPACE(ltTblBufDsp.fld_id))
        {
            lnOffset += M_SPACE_LEN(ltTblBufDsp.fld_id);
            continue;
        }

        if((liX = vbdipBufDspInf->nFldN++) >= NMMaxBufFldN)
        {
            DbsBUFDSP( DBS_CLOSE, &ltTblBufDsp);
            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "liX[%d]", liX);
            return -1;
        }

        if(llResult = nMFldDspGet(
                          ltTblBufDsp.fld_id,
                          vfdipFldDspInf,
                          &lfdepFldDspEle))
        {
            DbsBUFDSP( DBS_CLOSE, &ltTblBufDsp);
            return llResult;
        }
        /*
        if((lfdepFldDspEle->nFldType == NMFldTypeSint2) ||
           (lfdepFldDspEle->nFldType == NMFldTypeUint2) ||
           (lfdepFldDspEle->nFldType == NMFldTypeSpnt2)) {
            if(lnOffset % 2)
                lnOffset++;
        }
        if((lfdepFldDspEle->nFldType == NMFldTypeSint4) ||
           (lfdepFldDspEle->nFldType == NMFldTypeUint4) ||
           (lfdepFldDspEle->nFldType == NMFldTypeSpnt4)) {
            if(lnOffset % 4)
                lnOffset += (4 - lnOffset % 4);;
        }
        if((lfdepFldDspEle->nFldType == NMFldTypeSint8) ||
           (lfdepFldDspEle->nFldType == NMFldTypeUint8) ||
           (lfdepFldDspEle->nFldType == NMFldTypeSpnt8)) {
            if(lnOffset % 8)
                lnOffset += (8 - lnOffset % 8);;
        }
        */

        vbdipBufDspInf->bfeaBufEle[liX].iFldX = ltTblBufDsp.fld_index;
        vbdipBufDspInf->bfeaBufEle[liX].iFldId = ltTblBufDsp.fld_id;
        vbdipBufDspInf->bfeaBufEle[liX].nOffset = lnOffset;
        lnOffset += lfdepFldDspEle->nFldL;

    } /* end of while */

    if(llResult = DbsBUFDSP( DBS_CLOSE, &ltTblBufDsp))
        return llResult;

    return 0;
} /* end of nMBufDspLoad */

static
int nMFldTsfLoad(
    int                          vlFldTsfKey,
    int                          viBufChgX,
    fdiMFldDspInfDef              *vfdipFldDspInf,
    bdiMBufDspInfDef              *vbdipSourBufDspInf,
    bdiMBufDspInfDef              *vbdipDestBufDspInf,
    bceMBufChgEleDef              *vbcepBufChgEle)
{

    Tbl_fld_tsf_Def                     ltTblFldTsf;
    int                               llResult;
    int                               liX;
    bfeMBufFldEleDef                   *lbfepBufFldEle;
    fdeMFldDspEleDef                   *lfdepSourFldDspEle;
    fdeMFldDspEleDef                   *lfdepDestFldDspEle;

    vbcepBufChgEle->nFldN = 0;
    vbcepBufChgEle->iBufChgX = viBufChgX;
    ltTblFldTsf.usage_key = vlFldTsfKey;
    ltTblFldTsf.buf_chg_index = viBufChgX;
    DbsFLDTSF( DBS_CURSOR, &ltTblFldTsf);

    if(llResult = DbsFLDTSF( DBS_OPEN, &ltTblFldTsf))
        return llResult;

    while(DbsFLDTSF( DBS_FETCH, &ltTblFldTsf) == 0)
    {
        if((liX = vbcepBufChgEle->nFldN++) >= NMMaxFldTsfN)
        {
            DbsFLDTSF( DBS_CLOSE, &ltTblFldTsf);
            //printf("liX=%d\n", liX);
            return -1;
        } /* end of if */

        if(llResult = nMBufDspOffsetGet(
                          ltTblFldTsf.sour_fld_index,
                          vbdipSourBufDspInf,
                          &lbfepBufFldEle))
        {
            //printf("nMBufDspOffsetGet vbdipSourBufDspInf=%d\n", llResult);
            DbsFLDTSF( DBS_CLOSE, &ltTblFldTsf);
            return llResult;
        } /* end of if */

        vbcepBufChgEle->fceaFldEle[liX].nSourOffset = lbfepBufFldEle->nOffset;
        if(llResult = nMFldDspGet(
                          lbfepBufFldEle->iFldId,
                          vfdipFldDspInf,
                          &lfdepSourFldDspEle))
        {
            DbsFLDTSF( DBS_CLOSE, &ltTblFldTsf);
            return llResult;
        } /* end of if */
        if(llResult = nMBufDspOffsetGet(
                          ltTblFldTsf.dest_fld_index,
                          vbdipDestBufDspInf,
                          &lbfepBufFldEle))
        {
            //printf("nMBufDspOffsetGet vbdipDestBufDspInf=%d\n", llResult);
            DbsFLDTSF( DBS_CLOSE, &ltTblFldTsf);
            return llResult;
        } /* end of if */

        vbcepBufChgEle->fceaFldEle[liX].nDestOffset = lbfepBufFldEle->nOffset;

        if(llResult = nMFldDspGet(
                          lbfepBufFldEle->iFldId,
                          vfdipFldDspInf,
                          &lfdepDestFldDspEle))
        {
            DbsFLDTSF( DBS_CLOSE, &ltTblFldTsf);
            return llResult;
        } /* end of if */

        vbcepBufChgEle->fceaFldEle[liX].nChgType = lfdepSourFldDspEle->nFldType << 4;
        vbcepBufChgEle->fceaFldEle[liX].nChgType |= lfdepDestFldDspEle->nFldType;
        if(lfdepDestFldDspEle->nFldType == NMFldTypeStr)
        {
            vbcepBufChgEle->fceaFldEle[liX].nFldL = lfdepDestFldDspEle->nFldL;
            if((lfdepSourFldDspEle->nFldType == NMFldTypeStr) &&
                    (lfdepSourFldDspEle->nFldL < lfdepDestFldDspEle->nFldL))
                vbcepBufChgEle->fceaFldEle[liX].nFldL = lfdepSourFldDspEle->nFldL;
        }
        else if(lfdepSourFldDspEle->nFldType == NMFldTypeStr)
            vbcepBufChgEle->fceaFldEle[liX].nFldL = lfdepSourFldDspEle->nFldL;
        else
            vbcepBufChgEle->fceaFldEle[liX].nFldL = lfdepDestFldDspEle->nFldL;
        /*		HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__,__LINE__, "nSourOffset[%d]fldl[%d],lix[%d]chgtype[%d]",vbcepBufChgEle->fceaFldEle[liX].nSourOffset,vbcepBufChgEle->fceaFldEle[liX].nFldL,liX,vbcepBufChgEle->fceaFldEle[liX].nChgType);*/
    } /* end of while */

    if(llResult = DbsFLDTSF( DBS_CLOSE, &ltTblFldTsf))
        return llResult;


    return 0;
} /* end of nMFldTsfLoad */

int BufChgLoad(int  nBufChgKey, bciMBufChgInfDef *gbcipBufChgInf)
{
    int                               lhShmId;
    int                               llFldDspKey;
    int                               llResult;
    int                               llBufDspKey;
    int                               llBufChgKey;
    int                               llFldTsfKey;
    char                             *lspTmp;
    fdiMFldDspInfDef                  lfdiFldDspInf;
    bciMBufChgInfDef                  lbciBufChgInf;
    fdeMFldDspEleDef                 *lfdepSourFldDspEle;
    fdeMFldDspEleDef                 *lfdepDestFldDspEle;
    int                             liX;
    Tbl_buf_chg_Def                   ltTblBufChg;
    bdiMBufDspInfDef                  lbdiSourBufDspInf;
    bdiMBufDspInfDef                  lbdiDestBufDspInf;

    if(llResult = nMFldDspLoad(
                      nBufChgKey,
                      &lfdiFldDspInf))
    {
        gbcipBufChgInf->nBufN = 0;
        //printf("nMFldDspLoad Error\n");
        return llResult;
    } /* end of if */

    lbciBufChgInf.nBufN = 0;
    ltTblBufChg.usage_key = nBufChgKey;
    DbsBUFCHG( DBS_CURSOR, &ltTblBufChg);

    if(llResult = DbsBUFCHG( DBS_OPEN, &ltTblBufChg))
    {
        //printf("DbsBUFCHG Error\n");
        return llResult;
    }

    while(DbsBUFCHG( DBS_FETCH, &ltTblBufChg) == 0)
    {
        if((liX = lbciBufChgInf.nBufN++) >= NMMaxBufChgN)
        {
            DbsBUFCHG( DBS_CLOSE, &ltTblBufChg);
            //printf("NMMaxBufChgN is too smal!\n");
            return -1;
        } /* end of if */
        if(llResult = nMBufDspLoad(
                          nBufChgKey,
                          ltTblBufChg.sour_buf_index,
                          &lfdiFldDspInf,
                          &lbdiSourBufDspInf))
        {
            DbsBUFCHG( DBS_CLOSE, &ltTblBufChg);
            //printf("[%d][%d][%d]nMBufDspLoad Error!\n", __LINE__, liX, llResult);
            return llResult;
        } /* end of if */
        if(llResult = nMBufDspLoad(
                          nBufChgKey,
                          ltTblBufChg.dest_buf_index,
                          &lfdiFldDspInf,
                          &lbdiDestBufDspInf))
        {
            DbsBUFCHG( DBS_CLOSE, &ltTblBufChg);
            //printf("[%d][%d]nMBufDspLoad Error!\n", __LINE__, liX);
            return llResult;
        } /* end of if */
        if(llResult = nMFldTsfLoad(
                          nBufChgKey,
                          ltTblBufChg.buf_chg_index,
                          &lfdiFldDspInf,
                          &lbdiSourBufDspInf,
                          &lbdiDestBufDspInf,
                          &lbciBufChgInf.bceaBufEle[liX]))
        {
            DbsBUFCHG( DBS_CLOSE, &ltTblBufChg);
            //printf("[%d][%d][%d][%d]nMFldTsfLoad Error!\n", __LINE__, liX, ltTblBufChg.buf_chg_index, llResult);
            return llResult;
        } /* end of if */
    } /* end of while */

    if(llResult = DbsBUFCHG( DBS_CLOSE, &ltTblBufChg))
    {
        //printf("DbsBUFCHG LINE[%d] Error\n", __LINE__);
        return llResult;
    }

    HtMemcpy(
        gbcipBufChgInf,
        &lbciBufChgInf,
        sizeof(*gbcipBufChgInf));

    return 0;
} /* end of BufChgLoad */

int BufChgOpr(
    int                          viBufChgX,
    void                          *vvpSourBuf,
    void                          *vvpDestBuf,
    bciMBufChgInfDef				*gbcipBufChgInf)
{

    bceMBufChgEleDef                 *lbcepFldChgInf;
    char                             *lspSourBuf;
    char                             *lspDestBuf;
    int                             liX;
    int                             lnSTmp;
    int                               llSTmp;
    int                               llResult;
    char		tmp[100];

    for(liX = 0; liX < gbcipBufChgInf->nBufN; liX++)
    {
        if(viBufChgX == gbcipBufChgInf->bceaBufEle[liX].iBufChgX)
        {
            viBufChgX = liX;
            break;
        } /* end of if */
    } /* end of for */
    if(liX >= gbcipBufChgInf->nBufN)

        return -1;
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "vvpSourBuf=%s, liX=%d, gbcipBufChgInf->nBufN[%d]", vvpSourBuf, liX, gbcipBufChgInf->nBufN);
    lbcepFldChgInf = &gbcipBufChgInf->bceaBufEle[viBufChgX];

    lspSourBuf = vvpSourBuf;
    lspDestBuf = vvpDestBuf;
    for(liX = 0; liX < lbcepFldChgInf->nFldN; liX++)
    {
        /*
        			memset(tmp,0,sizeof(tmp));
        			HtMemcpy(tmp,&lspSourBuf[lbcepFldChgInf->fceaFldEle[liX].nSourOffset],lbcepFldChgInf->fceaFldEle[liX].nFldL);
        		HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__,__LINE__, "[%d][%d][%d][%s][%d]",M_SOUR_FLD_TYPE(lbcepFldChgInf->fceaFldEle[liX].nChgType), M_DEST_FLD_TYPE(lbcepFldChgInf->fceaFldEle[liX].nChgType),lbcepFldChgInf->nFldN,tmp,liX);
        */
        if(M_SOUR_FLD_TYPE(lbcepFldChgInf->fceaFldEle[liX].nChgType) == M_DEST_FLD_TYPE(lbcepFldChgInf->fceaFldEle[liX].nChgType))
        {
            HtMemcpy(
                &lspDestBuf[lbcepFldChgInf->fceaFldEle[liX].nDestOffset],
                &lspSourBuf[lbcepFldChgInf->fceaFldEle[liX].nSourOffset],
                lbcepFldChgInf->fceaFldEle[liX].nFldL);
            memset(&lspDestBuf[lbcepFldChgInf->fceaFldEle[liX].nDestOffset + lbcepFldChgInf->fceaFldEle[liX].nFldL], 0x00,
                   1);
            //HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "lspDestBuf[%d],    &lspSourBuf[%d],   lbcepFldChgInf->fceaFldEle[liX].nFldL[%d]", lbcepFldChgInf->fceaFldEle[liX].nDestOffset, lbcepFldChgInf->fceaFldEle[liX].nSourOffset, lbcepFldChgInf->fceaFldEle[liX].nFldL);
            continue;
        }
        else
            return -1;
    } /* end of for */
    return 0;
} /* end of BufChgOpr */

int nMGetIpcDft(
    int		vlUsageKey,
    int		vlIpcDftIndex,
    int		*vipIpcDftLen,
    void		*vvpIpcDft)
{

    int				nPos, i;
    Tbl_ipc_dft_Def		vtpTblIpcDft;
    char			sEnvValue[31];

    memset(&vtpTblIpcDft, 0, sizeof(vtpTblIpcDft));

    vtpTblIpcDft.usage_key = vlUsageKey;
    vtpTblIpcDft.ipc_dft_index = vlIpcDftIndex;

    DbsIPCDFT(DBS_CURSOR, &vtpTblIpcDft);

    if(DbsIPCDFT(DBS_OPEN, &vtpTblIpcDft)) return -1;

    nPos = 0;
    while(DbsIPCDFT(DBS_FETCH, &vtpTblIpcDft) == 0)
    {
        /*
        		HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__,__LINE__, "sunyudong test valuse  pos_index[%d]fld_len[%d][%s]",vtpTblIpcDft.pos_index,vtpTblIpcDft.fld_len,vtpTblIpcDft.fld_val);
        */
        if(vtpTblIpcDft.fld_val[0] == '$')
        {
            memset(sEnvValue, 0, sizeof(sEnvValue));
            HtMemcpy(sEnvValue, &vtpTblIpcDft.fld_val[1], 30);
            for(i = 0; i < 30; i++)
            {
                if(sEnvValue[i] == ' ')
                {
                    sEnvValue[i] = 0;
                    break;
                }
            }
            HtMemcpy((char *)vvpIpcDft + nPos,
                     (char *)getenv(sEnvValue),
                     vtpTblIpcDft.fld_len);
        }
        else if (vtpTblIpcDft.fld_val[0] == ' ')
        {
            memset((char *)vvpIpcDft + nPos,
                   ' ',
                   vtpTblIpcDft.fld_len);
        }
        else
        {
            HtMemcpy((char *)vvpIpcDft + nPos,
                     vtpTblIpcDft.fld_val,
                     vtpTblIpcDft.fld_len);
        }
        nPos += vtpTblIpcDft.fld_len;
    } /* end of while */

    if(DbsIPCDFT(DBS_CLOSE, &vtpTblIpcDft)) return -2;

    *vipIpcDftLen = nPos;

    return 0;
} /* end of nMGetIpcDft */

int IpcDftLoad(
    int				vlUsageKey,
    stIpcDftRuleDef   *vfripIpcDftRule)
{

    int   			liFldX;
    int  			nReturn;
    Tbl_ipc_dft_dsp_Def	vtpTblIpcDftDsp;

    memset(&vtpTblIpcDftDsp, 0, sizeof(Tbl_ipc_dft_dsp_Def));
    vtpTblIpcDftDsp.usage_key = vlUsageKey;
    DbsIPCDFTDSP(DBS_CURSOR, &vtpTblIpcDftDsp);

    nReturn=DbsIPCDFTDSP(DBS_OPEN, &vtpTblIpcDftDsp);
    if(nReturn) 
    {
    	HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "DBS_OPEN sqlcode[%d]", nReturn);
	return -1;
    } 
    memset(vfripIpcDftRule, 0, sizeof(stIpcDftRuleDef));
    liFldX = 0;
    while(1)
    {
        nReturn = DbsIPCDFTDSP(DBS_FETCH, &vtpTblIpcDftDsp);
        if(nReturn)
        {
            if(nReturn == DBS_NOTFOUND)
            {
                break;
            }

            HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "fetch sqlcode[%d]", nReturn);
            return -1;
        }
        HtMemcpy(vfripIpcDftRule->stIpcDft[liFldX].saCompKey, vtpTblIpcDftDsp.comp_key, 10);
        vfripIpcDftRule->stIpcDft[liFldX].nCompKeyLen = vtpTblIpcDftDsp.comp_key_len;
        vfripIpcDftRule->stIpcDft[liFldX].nIpcDftIndex = vtpTblIpcDftDsp.ipc_dft_index;
        vfripIpcDftRule->stIpcDft[liFldX].nBufDspIndex = vtpTblIpcDftDsp.buf_dsp_index;
        HtMemcpy(vfripIpcDftRule->stIpcDft[liFldX].saTxnNum, vtpTblIpcDftDsp.txn_num, 4);
        if( nMGetIpcDft(vlUsageKey, vfripIpcDftRule->stIpcDft[liFldX].nIpcDftIndex, &vfripIpcDftRule->stIpcDft[liFldX].nIpcDftLen, vfripIpcDftRule->stIpcDft[liFldX].saIpcDft) )
        {
            return -2;
        }

        liFldX++;
    } /* end of while */

    if(DbsIPCDFTDSP(DBS_CLOSE, &vtpTblIpcDftDsp)) return -3;

    vfripIpcDftRule->nIpcDftN = liFldX;

    return 0;
} /* end of IpcDftLoad */

int IpcDftOpr(
    void		*vvpInBuf,
    int		*vnpOutBufLen,
    void		*vvpOutBuf,
    void		*vvpTxnNum,
    int		*vnpBufChgIndex,
    stIpcDftRuleDef	*stIpcDftRule)
{
    char	*lspInBuf;
    char	*lspOutBuf;
    char	*lspTxnNum;
    int		liX;

    lspInBuf = vvpInBuf;
    lspOutBuf = vvpOutBuf;
    lspTxnNum = vvpTxnNum;
    HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "stIpcDftRule->nIpcDftN=%d", stIpcDftRule->nIpcDftN);
    for(liX = 0; liX < stIpcDftRule->nIpcDftN; liX++)
    {
        HtLog (gLogFile, HT_LOG_MODE_DEBUG, __FILE__, __LINE__, "lspInBuf[%s]=stIpcDftRule->stIpcDft[%d].saCompKey[%s]", lspInBuf, liX, stIpcDftRule->stIpcDft[liX].saCompKey);
        if(memcmp(lspInBuf,
                  stIpcDftRule->stIpcDft[liX].saCompKey,
                  stIpcDftRule->stIpcDft[liX].nCompKeyLen) == 0)
        {
            if(stIpcDftRule->stIpcDft[liX].nIpcDftIndex > -5000)
            {
                HtMemcpy(lspOutBuf,
                         stIpcDftRule->stIpcDft[liX].saIpcDft,
                         stIpcDftRule->stIpcDft[liX].nIpcDftLen);
                *vnpOutBufLen = stIpcDftRule->stIpcDft[liX].nIpcDftLen;

            }
            else
            {
                memset(lspOutBuf, 0x00,
                       -(stIpcDftRule->stIpcDft[liX].nIpcDftIndex + 5000));
                *vnpOutBufLen = -(stIpcDftRule->stIpcDft[liX].nIpcDftIndex + 5000);
            }
            HtMemcpy(lspTxnNum,
                     stIpcDftRule->stIpcDft[liX].saTxnNum, 4);
            *vnpBufChgIndex = stIpcDftRule->stIpcDft[liX].nBufDspIndex;
            return 0;
        }

    } /* end of for */
    return -1;
} /* end of IpcDftOpr */

int LoadRuleInf(IPCRuleInf *pipcRuleInf)
{
    int ret;
    Tbl_con_inf_Def dbtblconif;
    memset(&dbtblconif, 0x00, sizeof(dbtblconif));
    dbtblconif.usage_key = 0;
    DbsCONINF(DBS_CURSOR, &dbtblconif);
    ret = DbsCONINF(DBS_OPEN, &dbtblconif);
    if(ret)
    {
        HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "open error:%d", ret);
        return -1;
    }
    pipcRuleInf->nCount = 0;
    while(1)
    {
        memset(&dbtblconif, 0x00, sizeof(dbtblconif));
        ret = DbsCONINF(DBS_FETCH, &dbtblconif);
        if(ret)
        {
            if(ret == DBS_NOTFOUND)
                break;
            HtLog(gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "open error:%d", ret);
            return -1;
        }

        HtMemcpy(&pipcRuleInf->ipcTblConInf[pipcRuleInf->nCount].val, dbtblconif.val, sizeof(dbtblconif.val));
        pipcRuleInf->ipcTblConInf[pipcRuleInf->nCount].txn_num = dbtblconif.txn_num;
        HtLog (gLogFile, HT_LOG_MODE_ERROR, __FILE__, __LINE__, "txn_num[%d],val[%s]", pipcRuleInf->ipcTblConInf[pipcRuleInf->nCount].txn_num, pipcRuleInf->ipcTblConInf[pipcRuleInf->nCount].val);
        pipcRuleInf->nCount++;
    }
    DbsCONINF(DBS_CLOSE, &dbtblconif);
    return 0;
}
