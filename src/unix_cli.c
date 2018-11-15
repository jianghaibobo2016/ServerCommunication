//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <malloc.h>
//#include <sys/types.h>
//#include <errno.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <sys/select.h>
//#include <unistd.h>
//#include <termios.h>
//#include <sys/stat.h>
///**********定时器头文件***************/
//#include <sys/time.h>
//#include <signal.h>
///***********进程间SOCKET通信头文件**********/
//#include <sys/socket.h>
//#include <sys/un.h>
//#include <sys/ioctl.h>
//#include "dp_m2s_prot.h"
//#define VDEC_LOGO_DEFAULT_PATH "./config/DEC_NOVIDEO_1080P.yuv"
////#define VDEC_LOGO_DEFAULT_PATH "./config/VI_NOVIDEOSIGNAL_1080P.yuv"
//int gsConFd;
//
//#pragma pack(1)         //设定为1字节对齐
//static char recv_php_buf[256]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};
//
//static char MenuHelp[] = {
//    "\r\n==========================================="
//        "\r\n===================ctrl menu==============="
//        "\r\n==========================================="
//        "\r\nUsage: ./exe [Option]"
//        "\r\nOption:"
//        //        "\r\n\t debug A         : use to set debug level, "
//        //        "\r\n\t                 example: debug A, A: 0~7, 0:EMERG, 1: ALERT, 2:CRIT, 3:ERROR, 4:WARN, 5:NOTICE, 6:INFO, 7:DEBUG"
//        //        "\r\n\t exit            : use to clean all source and exit."
//        "\r\n\t init            : use to init source."
//        //        "\r\n\t free            : use to free source."
//        "\r\n"
//};
//
//static int print_ai_get_attr(DP_M2S_AI_GET_INFO_S info)
//{
//    printf("#################################################################\n");
//    printf("u32DevId:%d\n", info.u32DevId);
//    printf("bsConn:%d\n", info.bsConn);
//    printf("bSignal:%d\n", info.bSignal);
//    printf("u8Vol:%d\n", info.u8Vol);
//    printf("bSelfAdaptive:%d\n", info.bSelfAdaptive);
//    printf("stCommAttr.enSamplerate:%d\n", info.stCommAttr.enSamplerate);
//    printf("stCommAttr.enBitwidth:%d\n", info.stCommAttr.enBitwidth);
//    printf("stCommAttr.enSoundmode:%d\n", info.stCommAttr.enSoundmode);
//    printf("#################################################################\n");
//    return 0;
//}
//
//static int print_vi_get_attr(DP_M2S_VI_GET_INFO_S info)
//{
//    printf("#################################################################\n");
//    printf("s32DevId:%d\n", info.s32DevId);
//    printf("bConn:%d\n", info.bConn);
//    printf("bSignal:%d\n", info.bSignal);
//    printf("s32FrmRate:%d\n", info.s32FrmRate);
//    printf("stCap.s32X:%d\n", info.stCap.s32X);
//    printf("stCap.s32Y:%d\n", info.stCap.s32Y);
//    printf("stCap.u32Width:%d\n", info.stCap.u32Width);
//    printf("stCap.u32Height:%d\n", info.stCap.u32Height);
//    printf("stDst.u32Width:%d\n", info.stDst.u32Width);
//    printf("stDst.u32Height:%d\n", info.stDst.u32Height);
//    printf("#################################################################\n");
//    return 0;
//}
//
//static int print_avenc_get_attr(DP_M2S_AVENC_SET_INFO_S info)
//{
//    printf("#################################################################\n");
//    printf("task id:%d\n", info.TskId);
//    printf("-------------------------avbind\n");
//    printf("av bind type:%d\n", info.AvBindAttr.enBindType);
//    printf("av bind audio src mode id:%d\n", info.AvBindAttr.stAudio.stIn.ModId);
//    printf("av bind audio src dev id:%d\n", info.AvBindAttr.stAudio.stIn.u32DevId);
//    printf("av bind audio src chn id:%d\n", info.AvBindAttr.stAudio.stIn.u32ChnId);
//    printf("av bind audio dst mode id:%d\n", info.AvBindAttr.stAudio.stOut.ModId);
//    printf("av bind audio dst mode id:%d\n", info.AvBindAttr.stAudio.stOut.u32DevId);
//    printf("av bind audio dst mode id:%d\n", info.AvBindAttr.stAudio.stOut.u32ChnId);
//    printf("av bind video src mode id:%d\n", info.AvBindAttr.stVideo.stIn.ModId);
//    printf("av bind video src dev id:%d\n", info.AvBindAttr.stVideo.stIn.u32DevId);
//    printf("av bind video src chn id:%d\n", info.AvBindAttr.stVideo.stIn.u32ChnId);
//    printf("av bind video dst mode id:%d\n", info.AvBindAttr.stVideo.stOut.ModId);
//    printf("av bind video dst mode id:%d\n", info.AvBindAttr.stVideo.stOut.u32DevId);
//    printf("av bind video dst mode id:%d\n", info.AvBindAttr.stVideo.stOut.u32ChnId);
//    printf("-------------------------stream\n");
//    printf("stream type:%d\n", info.stStream.enType);
//    printf("stream rtsp server open:%d\n", info.stStream._rtsp.stRtspServer.bOpen);
//    printf("stream rtsp server multicast:%d\n", info.stStream._rtsp.stRtspServer.bMulticast);
//    printf("stream rtsp server transtype:%d\n", info.stStream._rtsp.stRtspServer.s32TransType);
//    printf("stream rtsp server conntimeout :%d\n", info.stStream._rtsp.stRtspServer.s32ConnTimeout);
//    printf("stream rtsp server connmax :%d\n", info.stStream._rtsp.stRtspServer.s32ConnMax);
//    printf("stream rtsp server connnums :%d\n", info.stStream._rtsp.stRtspServer.s32ConnNums);
//    printf("stream rtsp server au8url :%s\n", info.stStream._rtsp.stRtspServer.au8Url);
//    printf("-------------------------stAenc\n");
//    printf("aenc alg :%d \n", info.stAenc.enAlg);
//    printf("aenc s32bitrate :%d\n", info.stAenc.s32Bitrate);
//    printf("-------------------------stVenc\n");
//    printf("venc is bCrop :%d\n", info.stVenc.bCrop);
//    printf("venc crop status x:%d\n", info.stVenc.stCrop.s32X);
//    printf("venc crop status y:%d\n", info.stVenc.stCrop.s32Y);
//    printf("venc crop status widget:%d\n", info.stVenc.stCrop.u32Width);
//    printf("venc crop status height:%d\n", info.stVenc.stCrop.u32Height);
//    printf("venc is zoom :%d\n", info.stVenc.bZoom);
//    printf("venc zoom type:%d\n", info.stVenc.stZoom.enType);
//    if (info.stVenc.stZoom.enType == DP_M2S_ZOOM_RECT) {
//        printf("venc zoom rect x:%d\n", info.stVenc.stZoom.stRect.s32X);
//        printf("venc zoom rect y:%d\n", info.stVenc.stZoom.stRect.s32Y);
//        printf("venc zoom rect widget:%d\n", info.stVenc.stZoom.stRect.u32Width);
//        printf("venc zoom rect height:%d\n", info.stVenc.stZoom.stRect.u32Height);
//    } else if (info.stVenc.stZoom.enType == DP_M2S_ZOOM_RATIO) {
//        printf("venc zoom ratio x:%d\n", info.stVenc.stZoom.stRatio.u32XRatio);
//        printf("venc zoom ratio y:%d\n", info.stVenc.stZoom.stRatio.u32YRatio);
//        printf("venc zoom ratio widget:%d\n", info.stVenc.stZoom.stRatio.u32WRatio);
//        printf("venc zoom ratio height:%d\n", info.stVenc.stZoom.stRatio.u32HRatio);
//    }
//    printf("venc is osd:%d\n", info.stVenc.bOsd);
//    printf("venc osd type :%d\n", info.stVenc.stOsd.enType);
//    if (info.stVenc.stOsd.enType == DP_M2S_OSD_PIC) {
//        printf("venc osd pic path :%s\n", info.stVenc.stOsd.au8PicPath);
//    } else if (info.stVenc.stOsd.enType == DP_M2S_OSD_STRING) {
//        printf("venc osd str path :%s\n", info.stVenc.stOsd.stStr.au8Str);
//        printf("venc osd str color :%d\n", info.stVenc.stOsd.stStr.u32Color);
//    }
//    printf("venc osd display mode :%d\n", info.stVenc.stOsd.enDispMode);
//    printf("venc osd point x:%d\n", info.stVenc.stOsd.stPoint.s32X);
//    printf("venc osd point y:%d\n", info.stVenc.stOsd.stPoint.s32Y);
//    printf("venc alg type :%d\n", info.stVenc.stAlg.enAlg);
//    printf("venc alg frmrate:%d\n", info.stVenc.stAlg.stH264Enc.u32FrmRate);
//    printf("venc alg stSize Widget:%d\n", info.stVenc.stAlg.stH264Enc.stSize.u32Width);
//    printf("venc alg stSize Height:%d\n", info.stVenc.stAlg.stH264Enc.stSize.u32Height);
//    printf("venc alg RcMode:%d\n", info.stVenc.stAlg.stH264Enc.enRcMode);
//    printf("venc alg BitRate:%d\n", info.stVenc.stAlg.stH264Enc.u32Bitrate);
//    printf("venc alg Profile:%d\n", info.stVenc.stAlg.stH264Enc.enProfile);
//    printf("venc alg gop:%d\n", info.stVenc.stAlg.stH264Enc.u32Gop);
//    printf("venc alg sf:%d\n", info.stVenc.stAlg.stH264Enc.u16SF);
//    printf("venc alg tf:%d\n", info.stVenc.stAlg.stH264Enc.u16TF);
//    printf("#################################################################\n");
//    return 0;
//}
//
//static int cmd_set_avenc_print(DP_M2S_AVENC_SET_INFO_S info)
//{
//    printf("#################################################################\n");
//    printf("task id:%d\n", info.TskId);
//    printf("-------------------------avbind\n");
//    printf("av bind type:%d\n", info.AvBindAttr.enBindType);
//    printf("av bind audio src mode id:%d\n", info.AvBindAttr.stAudio.stIn.ModId);
//    printf("av bind audio src dev id:%d\n", info.AvBindAttr.stAudio.stIn.u32DevId);
//    printf("av bind audio src chn id:%d\n", info.AvBindAttr.stAudio.stIn.u32ChnId);
//    printf("av bind audio dst mode id:%d\n", info.AvBindAttr.stAudio.stOut.ModId);
//    printf("av bind audio dst mode id:%d\n", info.AvBindAttr.stAudio.stOut.u32DevId);
//    printf("av bind audio dst mode id:%d\n", info.AvBindAttr.stAudio.stOut.u32ChnId);
//    printf("av bind video src mode id:%d\n", info.AvBindAttr.stVideo.stIn.ModId);
//    printf("av bind video src dev id:%d\n", info.AvBindAttr.stVideo.stIn.u32DevId);
//    printf("av bind video src chn id:%d\n", info.AvBindAttr.stVideo.stIn.u32ChnId);
//    printf("av bind video dst mode id:%d\n", info.AvBindAttr.stVideo.stOut.ModId);
//    printf("av bind video dst mode id:%d\n", info.AvBindAttr.stVideo.stOut.u32DevId);
//    printf("av bind video dst mode id:%d\n", info.AvBindAttr.stVideo.stOut.u32ChnId);
//    printf("-------------------------stream\n");
//    printf("stream type:%d\n", info.stStream.enType);
//    printf("stream rtsp server open:%d\n", info.stStream._rtsp.stRtspServer.bOpen);
//    printf("stream rtsp server multicast:%d\n", info.stStream._rtsp.stRtspServer.bMulticast);
//    printf("stream rtsp server transtype:%d\n", info.stStream._rtsp.stRtspServer.s32TransType);
//    printf("stream rtsp server conntimeout :%d\n", info.stStream._rtsp.stRtspServer.s32ConnTimeout);
//    printf("stream rtsp server connmax :%d\n", info.stStream._rtsp.stRtspServer.s32ConnMax);
//    printf("stream rtsp server connnums :%d\n", info.stStream._rtsp.stRtspServer.s32ConnNums);
//    printf("stream rtsp server au8url :%s\n", info.stStream._rtsp.stRtspServer.au8Url);
//    printf("-------------------------stAenc\n");
//    printf("aenc alg :%d \n", info.stAenc.enAlg);
//    printf("aenc s32bitrate :%d\n", info.stAenc.s32Bitrate);
//    printf("-------------------------stVenc\n");
//    printf("venc is bCrop :%d\n", info.stVenc.bCrop);
//    printf("venc crop status x:%d\n", info.stVenc.stCrop.s32X);
//    printf("venc crop status y:%d\n", info.stVenc.stCrop.s32Y);
//    printf("venc crop status widget:%d\n", info.stVenc.stCrop.u32Width);
//    printf("venc crop status height:%d\n", info.stVenc.stCrop.u32Height);
//    printf("venc is zoom :%d\n", info.stVenc.bZoom);
//    printf("venc zoom type:%d\n", info.stVenc.stZoom.enType);
//    if (info.stVenc.stZoom.enType == DP_M2S_ZOOM_RECT) {
//        printf("venc zoom rect x:%d\n", info.stVenc.stZoom.stRect.s32X);
//        printf("venc zoom rect y:%d\n", info.stVenc.stZoom.stRect.s32Y);
//        printf("venc zoom rect widget:%d\n", info.stVenc.stZoom.stRect.u32Width);
//        printf("venc zoom rect height:%d\n", info.stVenc.stZoom.stRect.u32Height);
//    } else if (info.stVenc.stZoom.enType == DP_M2S_ZOOM_RATIO) {
//        printf("venc zoom ratio x:%d\n", info.stVenc.stZoom.stRatio.u32XRatio);
//        printf("venc zoom ratio y:%d\n", info.stVenc.stZoom.stRatio.u32YRatio);
//        printf("venc zoom ratio widget:%d\n", info.stVenc.stZoom.stRatio.u32WRatio);
//        printf("venc zoom ratio height:%d\n", info.stVenc.stZoom.stRatio.u32HRatio);
//    }
//    printf("venc is osd:%d\n", info.stVenc.bOsd);
//    printf("venc osd type :%d\n", info.stVenc.stOsd.enType);
//    if (info.stVenc.stOsd.enType == DP_M2S_OSD_PIC) {
//        printf("venc osd pic path :%s\n", info.stVenc.stOsd.au8PicPath);
//    } else if (info.stVenc.stOsd.enType == DP_M2S_OSD_STRING) {
//        printf("venc osd str path :%s\n", info.stVenc.stOsd.stStr.au8Str);
//        printf("venc osd str color :%d\n", info.stVenc.stOsd.stStr.u32Color);
//    }
//    printf("venc osd display mode :%d\n", info.stVenc.stOsd.enDispMode);
//    printf("venc osd point x:%d\n", info.stVenc.stOsd.stPoint.s32X);
//    printf("venc osd point y:%d\n", info.stVenc.stOsd.stPoint.s32Y);
//    printf("venc alg type :%d\n", info.stVenc.stAlg.enAlg);
//    printf("venc alg frmrate:%d\n", info.stVenc.stAlg.stH264Enc.u32FrmRate);
//    printf("venc alg stSize Widget:%d\n", info.stVenc.stAlg.stH264Enc.stSize.u32Width);
//    printf("venc alg stSize Height:%d\n", info.stVenc.stAlg.stH264Enc.stSize.u32Height);
//    printf("venc alg RcMode:%d\n", info.stVenc.stAlg.stH264Enc.enRcMode);
//    printf("venc alg BitRate:%d\n", info.stVenc.stAlg.stH264Enc.u32Bitrate);
//    printf("venc alg Profile:%d\n", info.stVenc.stAlg.stH264Enc.enProfile);
//    printf("venc alg gop:%d\n", info.stVenc.stAlg.stH264Enc.u32Gop);
//    printf("venc alg sf:%d\n", info.stVenc.stAlg.stH264Enc.u16SF);
//    printf("venc alg tf:%d\n", info.stVenc.stAlg.stH264Enc.u16TF);
//    printf("#################################################################\n");
//    return 0;
//}
//
//static int cmd_set_avenc_default_513(DP_VOID *pPtr)
//{
//    DP_M2S_AVENC_SET_INFO_S *pstAttr = (DP_M2S_AVENC_SET_INFO_S *)pPtr;
//    DP_M2S_AVENC_SET_INFO_S  stAttr;
//
//    memset(&stAttr, 0, sizeof(DP_M2S_AVENC_SET_INFO_S));
//
//    /* s1 TskId*/
//    stAttr.TskId = 513;//512~513
//
//    /* s2 DP_M2S_AVBIND_ATTR_S */
//    stAttr.AvBindAttr.enBindType=DP_M2S_AVBIND_VI2VENC;/* no audio */
//
//    //stAttr.AvBindAttr.stAudio = NULL;
//
//    stAttr.AvBindAttr.stVideo.stIn.ModId = DP_M2S_MOD_VI;
//    stAttr.AvBindAttr.stVideo.stIn.u32DevId = 0;
//    stAttr.AvBindAttr.stVideo.stIn.u32ChnId = 0;//not use
//
//    stAttr.AvBindAttr.stVideo.stOut.ModId = DP_M2S_MOD_VENC;
//    stAttr.AvBindAttr.stVideo.stOut.u32DevId = 0;//not use
//    stAttr.AvBindAttr.stVideo.stOut.u32ChnId = 1;
//
//    /* s3 DP_M2S_AENC_ATTR_S stAenc */
//
//    /* s4 DP_M2S_VENC_ATTR_S stVenc; */
//    stAttr.stVenc.bCrop = DP_FALSE;
//    stAttr.stVenc.bZoom = DP_FALSE;
//    stAttr.stVenc.bOsd = DP_FALSE;
//
//    DP_M2S_CROP_ATTR_S stCrop;
//    stCrop.s32X = 0;
//    stCrop.s32Y = 0;
//    stCrop.u32Width = 1920;
//    stCrop.u32Height = 1080;
//    memcpy(&stAttr.stVenc.stCrop, &stCrop, sizeof(DP_M2S_CROP_ATTR_S));
//
//    DP_M2S_ZOOM_ATTR_S stZoom;
//    stZoom.enType = DP_M2S_ZOOM_RECT;
//    stZoom.stRect.s32X = 0;
//    stZoom.stRect.s32Y = 0;
//    stZoom.stRect.u32Width = 1920;
//    stZoom.stRect.u32Height = 1080;
//    memcpy(&stAttr.stVenc.stZoom, &stZoom, sizeof(DP_M2S_ZOOM_ATTR_S));
//
//    /*
//       DP_M2S_OSD_ATTR_S stOsd;//not use
//       memcpy(&stAttr.stVenc.stOsd, xx, sizeof(DP_M2S_OSD_ATTR_S));
//       */
//
//    DP_M2S_ALG_ATTR_S stAlg;
//    stAlg.enAlg=DP_M2S_ALG_H264_ENC;
//    stAlg.stH264Enc.u32FrmRate = 30;
//    stAlg.stH264Enc.stSize.u32Width = stZoom.stRect.u32Width;
//    stAlg.stH264Enc.stSize.u32Height = stZoom.stRect.u32Height;
//    stAlg.stH264Enc.enRcMode = DP_M2S_RC_MODE_H264CBR;
//    stAlg.stH264Enc.u32Bitrate = 20*1024;
//    stAlg.stH264Enc.enProfile = DP_M2S_ALG_PROFILE_H264_BP;
//    stAlg.stH264Enc.u32Gop = 30;
//    stAlg.stH264Enc.u16SF = 0;
//    stAlg.stH264Enc.u16TF = 0;
//    memcpy(&stAttr.stVenc.stAlg, &stAlg, sizeof(DP_M2S_ALG_ATTR_S));
//
//    /* s5 DP_M2S_STREAM_ATTR_S stStream; */
//    //DP_M2S_STREAM_ATTR_S stStream;
//    stAttr.stStream.enType = DP_M2S_STREAM_RTSP_SERVER;
//    DP_M2S_RTSP_SERVER_ATTR_S stRtspServer;
//    stRtspServer.bOpen = DP_TRUE;
//    stRtspServer.bMulticast = DP_FALSE;
//    stRtspServer.s32TransType = 0;//udp
//    stRtspServer.s32ConnTimeout = 60;
//    stRtspServer.s32ConnMax = 128;
//    stRtspServer.s32ConnNums = 0;//not use
//    //stRtspServer.au8Url[] = NULL;//not use
//    memcpy(&stAttr.stStream._rtsp.stRtspServer, &stRtspServer, sizeof(DP_M2S_RTSP_SERVER_ATTR_S));
//
//    memcpy(pstAttr, &stAttr, sizeof(DP_M2S_AVENC_SET_INFO_S));
//    return 0;
//}
//
//static int cmd_set_avenc_default_512(DP_VOID *pPtr)
//{
//    DP_M2S_AVENC_SET_INFO_S *pstAttr = (DP_M2S_AVENC_SET_INFO_S *)pPtr;
//    DP_M2S_AVENC_SET_INFO_S  stAttr;
//
//    memset(&stAttr, 0, sizeof(DP_M2S_AVENC_SET_INFO_S));
//
//    /* s1 TskId*/
//    stAttr.TskId = 512;//512~513
//
//    /* s2 DP_M2S_AVBIND_ATTR_S */
//    stAttr.AvBindAttr.enBindType=DP_M2S_AVBIND_VI2VENC;/* no audio */
//
//    //stAttr.AvBindAttr.stAudio = NULL;
//
//    stAttr.AvBindAttr.stVideo.stIn.ModId = DP_M2S_MOD_VI;
//    stAttr.AvBindAttr.stVideo.stIn.u32DevId = 0;
//    stAttr.AvBindAttr.stVideo.stIn.u32ChnId = 0;//not use
//
//    stAttr.AvBindAttr.stVideo.stOut.ModId = DP_M2S_MOD_VENC;
//    stAttr.AvBindAttr.stVideo.stOut.u32DevId = 0;//not use
//    stAttr.AvBindAttr.stVideo.stOut.u32ChnId = 0;
//
//    /* s3 DP_M2S_AENC_ATTR_S stAenc */
//
//    /* s4 DP_M2S_VENC_ATTR_S stVenc; */
//    stAttr.stVenc.bCrop = DP_FALSE;
//    stAttr.stVenc.bZoom = DP_FALSE;
//    stAttr.stVenc.bOsd = DP_FALSE;
//
//    DP_M2S_CROP_ATTR_S stCrop;
//    stCrop.s32X = 0;
//    stCrop.s32Y = 0;
//    stCrop.u32Width = 3840;
//    stCrop.u32Height = 2160;
//    memcpy(&stAttr.stVenc.stCrop, &stCrop, sizeof(DP_M2S_CROP_ATTR_S));
//
//    DP_M2S_ZOOM_ATTR_S stZoom;
//    stZoom.enType = DP_M2S_ZOOM_RECT;
//    stZoom.stRect.s32X = 0;
//    stZoom.stRect.s32Y = 0;
//    stZoom.stRect.u32Width = 3840;
//    stZoom.stRect.u32Height = 2160;
//    memcpy(&stAttr.stVenc.stZoom, &stZoom, sizeof(DP_M2S_ZOOM_ATTR_S));
//
//    /*
//       DP_M2S_OSD_ATTR_S stOsd;//not use
//       memcpy(&stAttr.stVenc.stOsd, xx, sizeof(DP_M2S_OSD_ATTR_S));
//       */
//
//    DP_M2S_ALG_ATTR_S stAlg;
//    stAlg.enAlg=DP_M2S_ALG_H264_ENC;
//    stAlg.stH264Enc.u32FrmRate = 30;
//    stAlg.stH264Enc.stSize.u32Width = stZoom.stRect.u32Width;
//    stAlg.stH264Enc.stSize.u32Height = stZoom.stRect.u32Height;
//    stAlg.stH264Enc.enRcMode = DP_M2S_RC_MODE_H264CBR;
//    stAlg.stH264Enc.u32Bitrate = 20*1024;
//    stAlg.stH264Enc.enProfile = DP_M2S_ALG_PROFILE_H264_BP;
//    stAlg.stH264Enc.u32Gop = 30;
//    stAlg.stH264Enc.u16SF = 0;
//    stAlg.stH264Enc.u16TF = 0;
//    memcpy(&stAttr.stVenc.stAlg, &stAlg, sizeof(DP_M2S_ALG_ATTR_S));
//
//    /* s5 DP_M2S_STREAM_ATTR_S stStream; */
//    //DP_M2S_STREAM_ATTR_S stStream;
//    stAttr.stStream.enType = DP_M2S_STREAM_RTSP_SERVER;
//    DP_M2S_RTSP_SERVER_ATTR_S stRtspServer;
//    stRtspServer.bOpen = DP_TRUE;
//    stRtspServer.bMulticast = DP_FALSE;
//    stRtspServer.s32TransType = 0;//udp
//    stRtspServer.s32ConnTimeout = 60;
//    stRtspServer.s32ConnMax = 128;
//    stRtspServer.s32ConnNums = 0;//not use
//    //stRtspServer.au8Url[] = NULL;//not use
//    memcpy(&stAttr.stStream._rtsp.stRtspServer, &stRtspServer, sizeof(DP_M2S_RTSP_SERVER_ATTR_S));
//
//    memcpy(pstAttr, &stAttr, sizeof(DP_M2S_AVENC_SET_INFO_S));
//    return 0;
//}
//
//#if 0
//static int cmd_set_avenc_default_1(DP_VOID *pPtr)
//{
//    DP_M2S_AVENC_SET_INFO_S *pstAttr = (DP_M2S_AVENC_SET_INFO_S *)pPtr;
//    DP_M2S_AVENC_SET_INFO_S  stAttr;
//
//    memset(&stAttr, 0, sizeof(DP_M2S_AVENC_SET_INFO_S));
//
//    /* s1 TskId*/
//    stAttr.TskId = 512;//512~513
//
//    /* s2 DP_M2S_AVBIND_ATTR_S */
//    stAttr.AvBindAttr.enBindType=DP_M2S_AVBIND_VI2VENC;/* no audio */
//
//    //stAttr.AvBindAttr.stAudio = NULL;
//
//    stAttr.AvBindAttr.stVideo.stIn.ModId = DP_M2S_MOD_VI;
//    stAttr.AvBindAttr.stVideo.stIn.u32DevId = 0;
//    stAttr.AvBindAttr.stVideo.stIn.u32ChnId = 0;//not use
//
//    stAttr.AvBindAttr.stVideo.stOut.ModId = DP_M2S_MOD_VENC;
//    stAttr.AvBindAttr.stVideo.stOut.u32DevId = 0;//not use
//    stAttr.AvBindAttr.stVideo.stOut.u32ChnId = 0;
//
//    /* s3 DP_M2S_AENC_ATTR_S stAenc */
//
//    /* s4 DP_M2S_VENC_ATTR_S stVenc; */
//    stAttr.stVenc.bCrop = DP_TRUE;
//    stAttr.stVenc.bZoom = DP_TRUE;
//    stAttr.stVenc.bOsd = DP_FALSE;
//
//    DP_M2S_CROP_ATTR_S stCrop;
//    stCrop.s32X = 100;
//    stCrop.s32Y = 200;
//    stCrop.u32Width = 300;
//    stCrop.u32Height = 400;
//    memcpy(&stAttr.stVenc.stCrop, &stCrop, sizeof(DP_M2S_CROP_ATTR_S));
//
//    DP_M2S_ZOOM_ATTR_S stZoom;
//    stZoom.enType = DP_M2S_ZOOM_RECT;
//    stZoom.stRect.s32X = 500;
//    stZoom.stRect.s32Y = 600;
//    stZoom.stRect.u32Width = 700;
//    stZoom.stRect.u32Height = 800;
//    memcpy(&stAttr.stVenc.stZoom, &stZoom, sizeof(DP_M2S_ZOOM_ATTR_S));
//
//    /*
//       DP_M2S_OSD_ATTR_S stOsd;//not use
//       memcpy(&stAttr.stVenc.stOsd, xx, sizeof(DP_M2S_OSD_ATTR_S));
//       */
//
//    DP_M2S_ALG_ATTR_S stAlg;
//    stAlg.enAlg=DP_M2S_ALG_H264_ENC;
//    stAlg.stH264Enc.u32FrmRate = 55;
//    stAlg.stH264Enc.stSize.u32Width = stZoom.stRect.u32Width;
//    stAlg.stH264Enc.stSize.u32Height = stZoom.stRect.u32Height;
//    stAlg.stH264Enc.enRcMode = DP_M2S_RC_MODE_H264CBR;
//    stAlg.stH264Enc.u32Bitrate = 1111*1024;
//    stAlg.stH264Enc.enProfile = DP_M2S_ALG_PROFILE_H264_BP;
//    stAlg.stH264Enc.u32Gop = 66;
//    stAlg.stH264Enc.u16SF = 77;
//    stAlg.stH264Enc.u16TF = 88;
//    memcpy(&stAttr.stVenc.stAlg, &stAlg, sizeof(DP_M2S_ALG_ATTR_S));
//
//    /* s5 DP_M2S_STREAM_ATTR_S stStream; */
//    //DP_M2S_STREAM_ATTR_S stStream;
//    stAttr.stStream.enType = DP_M2S_STREAM_RTSP_SERVER;
//    DP_M2S_RTSP_SERVER_ATTR_S stRtspServer;
//    stRtspServer.bOpen = DP_TRUE;
//    stRtspServer.bMulticast = DP_FALSE;
//    stRtspServer.s32TransType = 0;//udp
//    stRtspServer.s32ConnTimeout = 99;
//    stRtspServer.s32ConnMax = 111;
//    stRtspServer.s32ConnNums = 222;//not use
//    //stRtspServer.au8Url[] = NULL;//not use
//    memcpy(&stAttr.stStream.stRtspServer, &stRtspServer, sizeof(DP_M2S_RTSP_SERVER_ATTR_S));
//
//    memcpy(pstAttr, &stAttr, sizeof(DP_M2S_AVENC_SET_INFO_S));
//    return 0;
//}
//#endif
//
//static int prot_head_write(void *pPtr, int cmdId, DP_BOOL bAck)
//{
//    DP_U16 u16WrtLen = 0;
//    //printf("======================>cmdId %d\n", cmdId);
//    DP_M2S_INF_PROT_HEAD_S stProtHead;
//    stProtHead.u16HeadTag = DP_M2S_INF_PROT_PKT_HEAD;
//    u16WrtLen = DP_M2S_INF_PROT_PKT_HEAD_LEN;
//    stProtHead.u16PacketLen = (bAck != DP_FALSE) ? (u16WrtLen | 0x8000): (u16WrtLen & 0x7fff);
//    stProtHead.u8CommandID = cmdId;
//
//    memcpy(pPtr, &stProtHead, u16WrtLen);
//    //printf("======================>cmdId2 %d, sizeof %d u16WrtLen %d\n", stProtHead.u8CommandID, sizeof(DP_M2S_INF_PROT_HEAD_S), u16WrtLen);
//
//    return u16WrtLen;
//
//}
//
//int cmd_send_attr_set(char *cmd_type, unsigned char *cmd_buf, int *cmd_len, char *cmd_argv2, char *cmd_argv3, char *cmd_argv4, char *cmd_argv5)
//{
//    printf("cmd: %s\n", cmd_type);
//    //int s32NeedWaitResult = 0, s32StProtHeadSize = 0;
//    int s32NeedWaitResult = 0;
//    //DP_M2S_INF_PROT_HEAD_S stProtHead;
//
//    if (!memcmp(cmd_type, "init", strlen("init")))
//    {
//        /* set pkt head */
//        int cmdId = DP_M2S_CMD_INIT;
//        *cmd_len = prot_head_write(cmd_buf, cmdId, DP_TRUE);
//        s32NeedWaitResult = 1;
//    }
//    else if (!memcmp(cmd_type, "deinit", strlen("deinit")))
//    {
//        /* set pkt head */
//        int cmdId = DP_M2S_CMD_DEINIT;
//        *cmd_len = prot_head_write(cmd_buf, cmdId, DP_TRUE);
//        s32NeedWaitResult = 1;
//    }
//    else if (!memcmp(cmd_type, "getinfo", strlen("getinfo")))
//    {
//        int CmdLen = strlen(cmd_type);
//        int type = atoi(&cmd_type[CmdLen-1]);
//        switch(type)
//        {
//        case DP_M2S_INFO_TYPE_GET_AI:
//            {
//                printf("DP_M2S_INFO_TYPE_GET_AI\n");
//                /* set pkt head */
//                DP_BOOL bAck = DP_TRUE;
//                DP_M2S_CMD_GETINFO_S stCmdGetInfo;
//                DP_U16 u16StCmdGetInfoLen = sizeof(DP_M2S_CMD_GETINFO_S);
//                stCmdGetInfo.stHeader.u16HeadTag = DP_M2S_INF_PROT_PKT_HEAD;
//                stCmdGetInfo.stHeader.u16PacketLen = (bAck != DP_FALSE) ? (u16StCmdGetInfoLen | 0x8000): (u16StCmdGetInfoLen & 0x7fff);
//                stCmdGetInfo.stHeader.u8CommandID = DP_M2S_CMD_GETINFO;
//                stCmdGetInfo.enInfoTYpe = DP_M2S_INFO_TYPE_GET_AI;
//                memcpy(cmd_buf, &stCmdGetInfo, u16StCmdGetInfoLen);
//                *cmd_len = u16StCmdGetInfoLen;
//                printf("[Dbg] getinfo: ai info(%d).\n", *cmd_len);
//                s32NeedWaitResult = 1;
//            }
//            break;
//        case DP_M2S_INFO_TYPE_GET_VI:
//            {
//                printf("DP_M2S_INFO_TYPE_GET_VI\n");
//                /* set pkt head */
//                DP_BOOL bAck = DP_TRUE;
//                DP_M2S_CMD_GETINFO_S stCmdGetInfo;
//                DP_U16 u16StCmdGetInfoLen = sizeof(DP_M2S_CMD_GETINFO_S);
//                stCmdGetInfo.stHeader.u16HeadTag = DP_M2S_INF_PROT_PKT_HEAD;
//                stCmdGetInfo.stHeader.u16PacketLen = (bAck != DP_FALSE) ? (u16StCmdGetInfoLen | 0x8000): (u16StCmdGetInfoLen & 0x7fff);
//                stCmdGetInfo.stHeader.u8CommandID = DP_M2S_CMD_GETINFO;
//                stCmdGetInfo.enInfoTYpe = DP_M2S_INFO_TYPE_GET_VI;
//                memcpy(cmd_buf, &stCmdGetInfo, u16StCmdGetInfoLen);
//                *cmd_len = u16StCmdGetInfoLen;
//                printf("[Dbg] getinfo: vi info(%d).\n", *cmd_len);
//                s32NeedWaitResult = 1;
//            }
//            break;
//        case DP_M2S_INFO_TYPE_GET_AVENC:
//            {
//                printf("DP_M2S_INFO_TYPE_GET_AVENC\n");
//                /* set pkt head */
//                DP_BOOL bAck = DP_TRUE;
//                DP_M2S_CMD_GETINFO_S stCmdGetInfo;
//                DP_U16 u16StCmdGetInfoLen = sizeof(DP_M2S_CMD_GETINFO_S);
//                stCmdGetInfo.stHeader.u16HeadTag = DP_M2S_INF_PROT_PKT_HEAD;
//                stCmdGetInfo.stHeader.u16PacketLen = (bAck != DP_FALSE) ? (u16StCmdGetInfoLen | 0x8000): (u16StCmdGetInfoLen & 0x7fff);
//                stCmdGetInfo.stHeader.u8CommandID = DP_M2S_CMD_GETINFO;
//                stCmdGetInfo.enInfoTYpe = DP_M2S_INFO_TYPE_GET_AVENC;
//                memcpy(cmd_buf, &stCmdGetInfo, u16StCmdGetInfoLen);
//                *cmd_len = u16StCmdGetInfoLen;
//                printf("[Dbg] getinfo: vi info(%d).\n", *cmd_len);
//                s32NeedWaitResult = 1;
//            }
//            break;
//        case DP_M2S_INFO_TYPE_GET_AVDEC:
//        case DP_M2S_INFO_TYPE_GET_AO:
//        case DP_M2S_INFO_TYPE_GET_VO:
//        default:
//            {
//                printf("[err] enInfoTYpe %d Invalid\n", type);
//            }
//            break;
//        }//switch
//
//
//    }
//    else if (!memcmp(cmd_type, "setinfo", strlen("setinfo")))
//    {
//        /* set pkt head */
//        int cmdId = DP_M2S_CMD_SETINFO;
//        int CmdLen = strlen(cmd_type);
//        *cmd_len = prot_head_write(cmd_buf, cmdId, DP_TRUE);
//        //printf("[%s] len %d ===========\n", cmd_type, CmdLen);
//        int type = atoi(&cmd_type[CmdLen-1]);
//
//        switch(type)
//        {
//        case DP_M2S_INFO_TYPE_SET_AI:
//            {
//                printf("DP_M2S_INFO_TYPE_SET_AI\n");
//            }
//            break;
//        case DP_M2S_INFO_TYPE_SET_VI:
//            {
//                printf("DP_M2S_INFO_TYPE_SET_VI\n");
//            }
//            break;
//        case DP_M2S_INFO_TYPE_SET_AVENC:
//            {
//
//                DP_M2S_CMD_SETINFO_S stCmdSetInfo;
//                DP_S32 stCmdSetInfoLen = sizeof(DP_M2S_CMD_SETINFO_S);
//                DP_M2S_AVENC_SET_INFO_S stAVEncInfo;
//                DP_S32 stAVEncInfoLen = sizeof(DP_M2S_AVENC_SET_INFO_S);
//                DP_BOOL bAck = DP_TRUE;
//
//                printf("[Dbg]DP_M2S_CMD_SETINFO_S(id %d, len %d) to DP_M2S_AVENC_SET_INFO_S(id %d, len %d)\n"
//                    , DP_M2S_CMD_SETINFO, stCmdSetInfoLen
//                    , DP_M2S_INFO_TYPE_SET_AVENC, stAVEncInfoLen);
//
//                DP_S32 s32SendDataLen = stCmdSetInfoLen - 4 + stAVEncInfoLen;
//
//                /*s1: DP_M2S_INF_PROT_HEAD_S stHeader; */
//                stCmdSetInfo.stHeader.u16HeadTag = DP_M2S_INF_PROT_PKT_HEAD;
//                stCmdSetInfo.stHeader.u16PacketLen = (bAck != DP_FALSE)?(s32SendDataLen | 0x8000):(s32SendDataLen &0x7fff);
//                stCmdSetInfo.stHeader.u8CommandID = DP_M2S_CMD_SETINFO;
//
//                /* s2 */
//                stCmdSetInfo.enInfoTYpe = DP_M2S_INFO_TYPE_SET_AVENC;
//                /* s3 */
//                stCmdSetInfo.u32InfoLen = stAVEncInfoLen;
//                /* s4 stCmdSetInfo.pu8InfoData */
//                cmd_set_avenc_default_513(&stAVEncInfo);
//
//                cmd_set_avenc_print(stAVEncInfo);
//
//                /* s5 write data */
//                DP_U8 *pBuf = (DP_U8 *)calloc(1, s32SendDataLen);
//                if (NULL == pBuf) {
//                    printf("[Err] calloc %u bytes failed\n", s32SendDataLen);
//                } else {
//                    memcpy(pBuf, &stCmdSetInfo, stCmdSetInfoLen-4);//-pu8InfoData's len
//                    DP_S32 s32Offset = stCmdSetInfoLen - 4;
//                    memcpy(pBuf+s32Offset, &stAVEncInfo, stAVEncInfoLen);
//
//                    memcpy(cmd_buf, pBuf, s32SendDataLen);
//                    *cmd_len = s32SendDataLen;
//
//                    free(pBuf);
//                    pBuf = NULL;
//                }
//            }
//            break;
//        case DP_M2S_INFO_TYPE_SET_AVDEC:
//        case DP_M2S_INFO_TYPE_SET_AO:
//        case DP_M2S_INFO_TYPE_SET_VO:
//        default:
//            {
//                printf("[err] enInfoTYpe %d Invalid\n", type);
//            }
//            break;
//        }//switch
//        s32NeedWaitResult = 1;
//    }
//    else if (!memcmp(cmd_type, "exit", strlen("exit")))
//    {
//        /* set pkt head */
//        int cmdId = DP_M2S_CMD_EXIT;
//        *cmd_len = prot_head_write(cmd_buf, cmdId, DP_TRUE);
//        s32NeedWaitResult = 1;
//    }
//
//    return s32NeedWaitResult;
//}
//
//int cmd_recv_attr_parse(unsigned char *pBuf,  int Len)
//{
//    DP_M2S_INF_PROT_HEAD_S *pstProtHead = (DP_M2S_INF_PROT_HEAD_S *) pBuf;
//    printf("u16HeadTag 0x%x, u16PacketLen %d u8CommandID 0x%x\n"
//        , pstProtHead->u16HeadTag
//        , pstProtHead->u16PacketLen
//        , pstProtHead->u8CommandID);
//#if 1
//    printf("************************************\n");
//    for(int i=0; i<pstProtHead->u16PacketLen; i++) {
//        if (i%16==0 && i!=0) {
//            printf("\n");
//        }
//        printf("0x%x ", pBuf[i]);
//    }
//    printf("\n");
//    printf("************************************\n");
//#endif
//    switch (pstProtHead->u8CommandID)
//    {
//    case DP_M2S_CMD_INIT:
//        {
//            printf("[Dbg] DP_M2S_CMD_INIT %d\n", DP_M2S_CMD_INIT);
//        }
//        break;
//    case DP_M2S_CMD_DEINIT:
//        {
//            printf("[Dbg] DP_M2S_CMD_DEINIT %d\n", DP_M2S_CMD_DEINIT);
//        }
//        break;
//    case DP_M2S_CMD_GETINFO:
//        {
//            printf("[Dbg] DP_M2S_CMD_GETINFO %d\n", DP_M2S_CMD_GETINFO);
//            DP_M2S_CMD_GETINFO_RESPOND_S *pStCmdGetInfoResp = NULL;
//            pStCmdGetInfoResp = (DP_M2S_CMD_GETINFO_RESPOND_S *)pBuf;
//            printf("u32Success %d\n", pStCmdGetInfoResp->u32Success);
//            printf("u32InfoLen %d\n", pStCmdGetInfoResp->u32InfoLen);
//            switch (pStCmdGetInfoResp->enInfoTYpe)
//            {
//            case DP_M2S_INFO_TYPE_GET_AI:
//                {
//                    printf("DP_M2S_INFO_TYPE_GET_AI\n");
//                    DP_S32 s32Offset = pstProtHead->u16PacketLen-pStCmdGetInfoResp->u32InfoLen;
//                    printf("s32Offset %d = %d - %d\n", s32Offset, pstProtHead->u16PacketLen, pStCmdGetInfoResp->u32InfoLen);
//                    DP_M2S_AI_GET_INFO_S stAiGetInfo;
//                    memcpy(&stAiGetInfo, pBuf+s32Offset, sizeof(DP_M2S_AI_GET_INFO_S));
//                    print_ai_get_attr(stAiGetInfo);
//                }
//                break;
//            case DP_M2S_INFO_TYPE_GET_VI:
//                {
//                    printf("DP_M2S_INFO_TYPE_GET_VI\n");
//                    DP_S32 s32Offset = pstProtHead->u16PacketLen-pStCmdGetInfoResp->u32InfoLen;
//                    printf("s32Offset %d = %d - %d\n", s32Offset, pstProtHead->u16PacketLen, pStCmdGetInfoResp->u32InfoLen);
//                    DP_M2S_VI_GET_INFO_S stViGetInfo;
//                    memcpy(&stViGetInfo, pBuf+s32Offset, sizeof(DP_M2S_VI_GET_INFO_S));
//                    print_vi_get_attr(stViGetInfo);
//                }
//                break;
//            case DP_M2S_INFO_TYPE_GET_AVENC:
//                {
//                    printf("DP_M2S_INFO_TYPE_GET_AVENC\n");
//                    DP_S32 s32Offset = pstProtHead->u16PacketLen-pStCmdGetInfoResp->u32InfoLen;
//                    printf("s32Offset %d = %d - %d\n", s32Offset, pstProtHead->u16PacketLen, pStCmdGetInfoResp->u32InfoLen);
//#if 0
//                    DP_M2S_AVENC_GET_INFO_S stAVEncGetInfo;
//                    //memcpy(&stAVEncGetInfo, pBuf+s32Offset, sizeof(DP_M2S_AVENC_GET_INFO_S));
//                    //memcpy(&stAVEncGetInfo, pBuf+s32Offset+sizeof(DP_M2S_AVENC_GET_INFO_S), sizeof(DP_M2S_AVENC_GET_INFO_S));
//                    print_avenc_get_attr(stAVEncGetInfo);
//#else
//
//                    DP_U32 u32InfoLen = pStCmdGetInfoResp->u32InfoLen;
//                    DP_U32 u32InfoCnt = u32InfoLen / sizeof(DP_M2S_AVENC_GET_INFO_S);
//                    printf("[Dbg] u32InfoLen %d u32InfoCnt %d\n", u32InfoLen, u32InfoCnt);
//                    for(int i=0; i<u32InfoCnt; i++)
//                    {
//                        DP_M2S_AVENC_GET_INFO_S stAVEncGetInfo;
//                        memcpy(&stAVEncGetInfo, pBuf+s32Offset+i*sizeof(DP_M2S_AVENC_GET_INFO_S), sizeof(DP_M2S_AVENC_GET_INFO_S));
//                        print_avenc_get_attr(stAVEncGetInfo);
//                    }
//#endif
//                }
//                break;
//            case DP_M2S_INFO_TYPE_GET_AVDEC:
//            case DP_M2S_INFO_TYPE_GET_AO:
//            case DP_M2S_INFO_TYPE_GET_VO:
//            default:
//                {
//                    printf("[err] enInfoTYpe %d Invalid\n", pstProtHead->u8CommandID);
//                }
//                break;
//            }//switch
//        }
//        break;
//    case DP_M2S_CMD_SETINFO:
//        {
//            printf("[Dbg] DP_M2S_CMD_SETINFO %d\n", DP_M2S_CMD_SETINFO);
//        }
//        break;
//    case DP_M2S_CMD_EXIT:
//        {
//            printf("[Dbg] DP_M2S_CMD_EXIT %d\n", DP_M2S_CMD_EXIT);
//        }
//        break;
//    default:
//        break;
//    }
//
//    return DP_TRUE;
//}
//
//int main(int argc, char *argv[])
//{
//    if (argc >= 2) {
//        if (!memcmp(argv[1], "help", sizeof("help"))
//            || !memcmp(argv[1], "-help", sizeof("-help"))
//            || !memcmp(argv[1], "--help", sizeof("--help"))
//            || !memcmp(argv[1], "-h", sizeof("-h"))
//           )
//        {
//            puts(MenuHelp);
//            return -1;
//        }
//    }
//
//    int ret=0;
//    int ReadFlag = 0;
//    static struct sockaddr_un srv_addr;
//    //printf("start ipc\n");
//    gsConFd=socket(AF_UNIX,SOCK_STREAM,0);
//    //printf("gsConFd %d\n",gsConFd);
//    if(gsConFd<0)
//    {
//        perror("cannot create communication socket");
//        printf("%d\n",gsConFd);
//    }
//    else
//    {
//        srv_addr.sun_family=AF_UNIX;
//        strcpy(srv_addr.sun_path,DP_M2S_INF_PROT_UNIX_FIFO);
//
//        //连接服务器
//        ret=connect(gsConFd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
//        if(ret==-1)
//        {
//            close(gsConFd);
//            printf("[Err] connect fail\n");
//        }
//        else
//        {
//            //printf("[Dbg] connect ok\n");
//            memset(recv_php_buf,0,sizeof(recv_php_buf));                             //清空socket_buf
//
//            unsigned char cmd_buf[4096] = {0};
//            int cmd_len = 0;
//            ReadFlag = cmd_send_attr_set(argv[1], cmd_buf, &cmd_len, argv[2], argv[3], argv[4], argv[5]);
//            if (cmd_len <= 0) {
//                printf("[Err] cmd_len <= 0\n");
//                return -1;
//            }
//            ret = write(gsConFd, cmd_buf, cmd_len);//将数据传送到外部应用程序,发送实际长度
//            if (ret != cmd_len) {
//                printf("[Err] write %d Bytes failed\n", cmd_len);
//            } else {
//                for(int i=0; i<cmd_len;i++)
//                {
//                    printf("0x%x ", cmd_buf[i]);
//                    if (i==16) {
//                        printf("\n");
//                    }
//                }
//                printf("\n");
//                printf("[Dbg] write %d Bytes ok\n", cmd_len);
//
//                FILE *fCrcFile = fopen("./send.txt", "w+");
//
//                if (fCrcFile != NULL) {
//                    fseek(fCrcFile, 0L, SEEK_SET);
//                    DP_S32 s32TmpRet = fwrite(cmd_buf, cmd_len, 1, fCrcFile);
//                    if (s32TmpRet != 1) {
//                        printf("write %d Bytes to recv.txt failed\n", cmd_len);
//                    } else {
//                        printf("write %d Bytes to recv.txt\n", cmd_len);
//                    }
//                }
//                if (NULL != fCrcFile) {
//                    fclose(fCrcFile);
//                    fCrcFile=NULL;
//                }
//            }
//
//            if (ReadFlag == 1) {
//                memset(cmd_buf,0,4096);                             //清空socket_buf
//                ret = read(gsConFd, cmd_buf, 4096);
//                if (ret >0 ) {
//                    printf("[Dbg] recv %d Bytes\n", ret);
//                    cmd_recv_attr_parse(cmd_buf, ret);
//                } else {
//                    printf("[Err] recv failed\n");
//                }
//            }
//            usleep(500000);
//        }
//        printf("###############end\n");
//    }
//
//    return 0;
//}

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>
#include <termios.h>
#include <sys/stat.h>
/**********定时器头文件***************/
#include <sys/time.h>
#include <signal.h>
/***********进程间SOCKET通信头文件**********/
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include "dp_m2s_prot.h"
#define VDEC_LOGO_DEFAULT_PATH "./config/DEC_NOVIDEO_1080P.yuv"
//#define VDEC_LOGO_DEFAULT_PATH "./config/VI_NOVIDEOSIGNAL_1080P.yuv"
int gsConFd;

#pragma pack(1)         //设定为1字节对齐
static char recv_php_buf[256] =
{	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

static char MenuHelp[] =
{	"\r\n==========================================="
	"\r\n===================ctrl menu==============="
	"\r\n==========================================="
	"\r\nUsage: ./exe [Option]"
	"\r\nOption:"
	//        "\r\n\t debug A         : use to set debug level, "
	//        "\r\n\t                 example: debug A, A: 0~7, 0:EMERG, 1: ALERT, 2:CRIT, 3:ERROR, 4:WARN, 5:NOTICE, 6:INFO, 7:DEBUG"
	//        "\r\n\t exit            : use to clean all source and exit."
	"\r\n\t init            : use to init source."
	"\r\n\t exit            : use to clean all source and exit."
	"\r\n\t getinfo0        : use to get ai attr"
	"\r\n\t getinfo2        : use to get vi attr"
	"\r\n\t getinfo4        : use to get (all)avenc attr"
	"\r\n\t setinfo5 [0-255] [Width] [Height] [BitRate] [AencChn]: use to avenc attr."
	"\r\n\t setinfo5 [256-511] [Width] [Height] [BitRate] [VencChn]: use to avenc attr."
	"\r\n\t setinfo5 [512-768] [Width] [Height] [BitRate] [AencChn] [VencChn]: use to avenc attr."
	//        "\r\n\t free            : use to free source."
	"\r\n"};

static int print_ai_get_attr(DP_M2S_AI_GET_INFO_S info) {
	printf(
			"#################################################################\n");
	printf("u32DevId:%d\n", info.u32DevId);
	printf("bsConn:%d\n", info.bsConn);
	printf("bSignal:%d\n", info.bSignal);
	printf("u8Vol:%d\n", info.u8Vol);
	printf("bSelfAdaptive:%d\n", info.bSelfAdaptive);
	printf("stCommAttr.enSamplerate:%d\n", info.stCommAttr.enSamplerate);
	printf("stCommAttr.enBitwidth:%d\n", info.stCommAttr.enBitwidth);
	printf("stCommAttr.enSoundmode:%d\n", info.stCommAttr.enSoundmode);
	printf(
			"#################################################################\n");
	return 0;
}

static int print_vi_get_attr(DP_M2S_VI_GET_INFO_S info) {
	printf(
			"#################################################################\n");
	printf("s32DevId:%d\n", info.s32DevId);
	printf("bConn:%d\n", info.bConn);
	printf("bSignal:%d\n", info.bSignal);
	printf("s32FrmRate:%d\n", info.s32FrmRate);
	printf("stCap.s32X:%d\n", info.stCap.s32X);
	printf("stCap.s32Y:%d\n", info.stCap.s32Y);
	printf("stCap.u32Width:%d\n", info.stCap.u32Width);
	printf("stCap.u32Height:%d\n", info.stCap.u32Height);
	printf("stDst.u32Width:%d\n", info.stDst.u32Width);
	printf("stDst.u32Height:%d\n", info.stDst.u32Height);
	printf(
			"#################################################################\n");
	return 0;
}

static int print_avenc_get_attr(DP_M2S_AVENC_SET_INFO_S info) {
	printf(
			"#################################################################\n");
	printf("task id:%d\n", info.TskId);
	printf("-------------------------avbind\n");
	printf("av bind type:%d\n", info.AvBindAttr.enBindType);
	printf("av bind audio src mode id:%d\n",
			info.AvBindAttr.stAudio.stIn.ModId);
	printf("av bind audio src dev id:%d\n",
			info.AvBindAttr.stAudio.stIn.u32DevId);
	printf("av bind audio src chn id:%d\n",
			info.AvBindAttr.stAudio.stIn.u32ChnId);
	printf("av bind audio dst mode id:%d\n",
			info.AvBindAttr.stAudio.stOut.ModId);
	printf("av bind audio dst mode id:%d\n",
			info.AvBindAttr.stAudio.stOut.u32DevId);
	printf("av bind audio dst mode id:%d\n",
			info.AvBindAttr.stAudio.stOut.u32ChnId);
	printf("av bind video src mode id:%d\n",
			info.AvBindAttr.stVideo.stIn.ModId);
	printf("av bind video src dev id:%d\n",
			info.AvBindAttr.stVideo.stIn.u32DevId);
	printf("av bind video src chn id:%d\n",
			info.AvBindAttr.stVideo.stIn.u32ChnId);
	printf("av bind video dst mode id:%d\n",
			info.AvBindAttr.stVideo.stOut.ModId);
	printf("av bind video dst mode id:%d\n",
			info.AvBindAttr.stVideo.stOut.u32DevId);
	printf("av bind video dst mode id:%d\n",
			info.AvBindAttr.stVideo.stOut.u32ChnId);
	printf("-------------------------stream\n");
	printf("stream type:%d\n", info.stStream.enType);
	printf("stream rtsp server open:%d\n", info.stStream.stRtspServer.bOpen);
	printf("stream rtsp server multicast:%d\n",
			info.stStream.stRtspServer.bMulticast);
	printf("stream rtsp server transtype:%d\n",
			info.stStream.stRtspServer.s32TransType);
	printf("stream rtsp server conntimeout :%d\n",
			info.stStream.stRtspServer.s32ConnTimeout);
	printf("stream rtsp server connmax :%d\n",
			info.stStream.stRtspServer.s32ConnMax);
	printf("stream rtsp server connnums :%d\n",
			info.stStream.stRtspServer.s32ConnNums);
	printf("stream rtsp server au8url :%s\n",
			info.stStream.stRtspServer.au8Url);
	printf("-------------------------stAenc\n");
	printf("aenc alg :%d \n", info.stAenc.enAlg);
	printf("aenc s32bitrate :%d\n", info.stAenc.s32Bitrate);
	printf("-------------------------stVenc\n");
	printf("venc is bCrop :%d\n", info.stVenc.bCrop);
	printf("venc crop status x:%d\n", info.stVenc.stCrop.s32X);
	printf("venc crop status y:%d\n", info.stVenc.stCrop.s32Y);
	printf("venc crop status widget:%d\n", info.stVenc.stCrop.u32Width);
	printf("venc crop status height:%d\n", info.stVenc.stCrop.u32Height);
	printf("venc is zoom :%d\n", info.stVenc.bZoom);
	printf("venc zoom type:%d\n", info.stVenc.stZoom.enType);
	if (info.stVenc.stZoom.enType == DP_M2S_ZOOM_RECT) {
		printf("venc zoom rect x:%d\n", info.stVenc.stZoom.stRect.s32X);
		printf("venc zoom rect y:%d\n", info.stVenc.stZoom.stRect.s32Y);
		printf("venc zoom rect widget:%d\n",
				info.stVenc.stZoom.stRect.u32Width);
		printf("venc zoom rect height:%d\n",
				info.stVenc.stZoom.stRect.u32Height);
	} else if (info.stVenc.stZoom.enType == DP_M2S_ZOOM_RATIO) {
		printf("venc zoom ratio x:%d\n", info.stVenc.stZoom.stRatio.u32XRatio);
		printf("venc zoom ratio y:%d\n", info.stVenc.stZoom.stRatio.u32YRatio);
		printf("venc zoom ratio widget:%d\n",
				info.stVenc.stZoom.stRatio.u32WRatio);
		printf("venc zoom ratio height:%d\n",
				info.stVenc.stZoom.stRatio.u32HRatio);
	}
	printf("venc is osd:%d\n", info.stVenc.bOsd);
	printf("venc osd type :%d\n", info.stVenc.stOsd.enType);
	if (info.stVenc.stOsd.enType == DP_M2S_OSD_PIC) {
		printf("venc osd pic path :%s\n", info.stVenc.stOsd.au8PicPath);
	} else if (info.stVenc.stOsd.enType == DP_M2S_OSD_STRING) {
		printf("venc osd str path :%s\n", info.stVenc.stOsd.stStr.au8Str);
		printf("venc osd str color :%d\n", info.stVenc.stOsd.stStr.u32Color);
	}
	printf("venc osd display mode :%d\n", info.stVenc.stOsd.enDispMode);
	printf("venc osd point x:%d\n", info.stVenc.stOsd.stPoint.s32X);
	printf("venc osd point y:%d\n", info.stVenc.stOsd.stPoint.s32Y);
	printf("venc alg type :%d\n", info.stVenc.stAlg.enAlg);
	printf("venc alg frmrate:%d\n", info.stVenc.stAlg.stH264Enc.u32FrmRate);
	printf("venc alg stSize Widget:%d\n",
			info.stVenc.stAlg.stH264Enc.stSize.u32Width);
	printf("venc alg stSize Height:%d\n",
			info.stVenc.stAlg.stH264Enc.stSize.u32Height);
	printf("venc alg RcMode:%d\n", info.stVenc.stAlg.stH264Enc.enRcMode);
	printf("venc alg BitRate:%d\n", info.stVenc.stAlg.stH264Enc.u32Bitrate);
	printf("venc alg Profile:%d\n", info.stVenc.stAlg.stH264Enc.enProfile);
	printf("venc alg gop:%d\n", info.stVenc.stAlg.stH264Enc.u32Gop);
	printf("venc alg sf:%d\n", info.stVenc.stAlg.stH264Enc.u16SF);
	printf("venc alg tf:%d\n", info.stVenc.stAlg.stH264Enc.u16TF);
	printf(
			"#################################################################\n");
	return 0;
}

static int cmd_set_avenc_print(DP_M2S_AVENC_SET_INFO_S info) {
	printf(
			"#################################################################\n");
	printf("task id:%d\n", info.TskId);
	printf("-------------------------avbind\n");
	printf("av bind type:%d\n", info.AvBindAttr.enBindType);
	printf("av bind audio src mode id:%d\n",
			info.AvBindAttr.stAudio.stIn.ModId);
	printf("av bind audio src dev id:%d\n",
			info.AvBindAttr.stAudio.stIn.u32DevId);
	printf("av bind audio src chn id:%d\n",
			info.AvBindAttr.stAudio.stIn.u32ChnId);
	printf("av bind audio dst mode id:%d\n",
			info.AvBindAttr.stAudio.stOut.ModId);
	printf("av bind audio dst mode id:%d\n",
			info.AvBindAttr.stAudio.stOut.u32DevId);
	printf("av bind audio dst mode id:%d\n",
			info.AvBindAttr.stAudio.stOut.u32ChnId);
	printf("av bind video src mode id:%d\n",
			info.AvBindAttr.stVideo.stIn.ModId);
	printf("av bind video src dev id:%d\n",
			info.AvBindAttr.stVideo.stIn.u32DevId);
	printf("av bind video src chn id:%d\n",
			info.AvBindAttr.stVideo.stIn.u32ChnId);
	printf("av bind video dst mode id:%d\n",
			info.AvBindAttr.stVideo.stOut.ModId);
	printf("av bind video dst mode id:%d\n",
			info.AvBindAttr.stVideo.stOut.u32DevId);
	printf("av bind video dst mode id:%d\n",
			info.AvBindAttr.stVideo.stOut.u32ChnId);
	printf("-------------------------stream\n");
	printf("stream type:%d\n", info.stStream.enType);
	printf("stream rtsp server open:%d\n", info.stStream.stRtspServer.bOpen);
	printf("stream rtsp server multicast:%d\n",
			info.stStream.stRtspServer.bMulticast);
	printf("stream rtsp server transtype:%d\n",
			info.stStream.stRtspServer.s32TransType);
	printf("stream rtsp server conntimeout :%d\n",
			info.stStream.stRtspServer.s32ConnTimeout);
	printf("stream rtsp server connmax :%d\n",
			info.stStream.stRtspServer.s32ConnMax);
	printf("stream rtsp server connnums :%d\n",
			info.stStream.stRtspServer.s32ConnNums);
	printf("stream rtsp server au8url :%s\n",
			info.stStream.stRtspServer.au8Url);
	printf("-------------------------stAenc\n");
	printf("aenc alg :%d \n", info.stAenc.enAlg);
	printf("aenc s32bitrate :%d\n", info.stAenc.s32Bitrate);
	printf("-------------------------stVenc\n");
	printf("venc is bCrop :%d\n", info.stVenc.bCrop);
	printf("venc crop status x:%d\n", info.stVenc.stCrop.s32X);
	printf("venc crop status y:%d\n", info.stVenc.stCrop.s32Y);
	printf("venc crop status widget:%d\n", info.stVenc.stCrop.u32Width);
	printf("venc crop status height:%d\n", info.stVenc.stCrop.u32Height);
	printf("venc is zoom :%d\n", info.stVenc.bZoom);
	printf("venc zoom type:%d\n", info.stVenc.stZoom.enType);
	if (info.stVenc.stZoom.enType == DP_M2S_ZOOM_RECT) {
		printf("venc zoom rect x:%d\n", info.stVenc.stZoom.stRect.s32X);
		printf("venc zoom rect y:%d\n", info.stVenc.stZoom.stRect.s32Y);
		printf("venc zoom rect widget:%d\n",
				info.stVenc.stZoom.stRect.u32Width);
		printf("venc zoom rect height:%d\n",
				info.stVenc.stZoom.stRect.u32Height);
	} else if (info.stVenc.stZoom.enType == DP_M2S_ZOOM_RATIO) {
		printf("venc zoom ratio x:%d\n", info.stVenc.stZoom.stRatio.u32XRatio);
		printf("venc zoom ratio y:%d\n", info.stVenc.stZoom.stRatio.u32YRatio);
		printf("venc zoom ratio widget:%d\n",
				info.stVenc.stZoom.stRatio.u32WRatio);
		printf("venc zoom ratio height:%d\n",
				info.stVenc.stZoom.stRatio.u32HRatio);
	}
	printf("venc is osd:%d\n", info.stVenc.bOsd);
	printf("venc osd type :%d\n", info.stVenc.stOsd.enType);
	if (info.stVenc.stOsd.enType == DP_M2S_OSD_PIC) {
		printf("venc osd pic path :%s\n", info.stVenc.stOsd.au8PicPath);
	} else if (info.stVenc.stOsd.enType == DP_M2S_OSD_STRING) {
		printf("venc osd str path :%s\n", info.stVenc.stOsd.stStr.au8Str);
		printf("venc osd str color :%d\n", info.stVenc.stOsd.stStr.u32Color);
	}
	printf("venc osd display mode :%d\n", info.stVenc.stOsd.enDispMode);
	printf("venc osd point x:%d\n", info.stVenc.stOsd.stPoint.s32X);
	printf("venc osd point y:%d\n", info.stVenc.stOsd.stPoint.s32Y);
	printf("venc alg type :%d\n", info.stVenc.stAlg.enAlg);
	printf("venc alg frmrate:%d\n", info.stVenc.stAlg.stH264Enc.u32FrmRate);
	printf("venc alg stSize Widget:%d\n",
			info.stVenc.stAlg.stH264Enc.stSize.u32Width);
	printf("venc alg stSize Height:%d\n",
			info.stVenc.stAlg.stH264Enc.stSize.u32Height);
	printf("venc alg RcMode:%d\n", info.stVenc.stAlg.stH264Enc.enRcMode);
	printf("venc alg BitRate:%d\n", info.stVenc.stAlg.stH264Enc.u32Bitrate);
	printf("venc alg Profile:%d\n", info.stVenc.stAlg.stH264Enc.enProfile);
	printf("venc alg gop:%d\n", info.stVenc.stAlg.stH264Enc.u32Gop);
	printf("venc alg sf:%d\n", info.stVenc.stAlg.stH264Enc.u16SF);
	printf("venc alg tf:%d\n", info.stVenc.stAlg.stH264Enc.u16TF);
	printf(
			"#################################################################\n");
	return 0;
}

#if 0
static int cmd_set_avenc_default_513(DP_VOID *pPtr)
{
	DP_M2S_AVENC_SET_INFO_S *pstAttr = (DP_M2S_AVENC_SET_INFO_S *)pPtr;
	DP_M2S_AVENC_SET_INFO_S stAttr;

	memset(&stAttr, 0, sizeof(DP_M2S_AVENC_SET_INFO_S));

	/* s1 TskId*/
	stAttr.TskId = 513;        //512~513

	/* s2 DP_M2S_AVBIND_ATTR_S */
	stAttr.AvBindAttr.enBindType=DP_M2S_AVBIND_VI2VENC;/* no audio */

	//stAttr.AvBindAttr.stAudio = NULL;
	stAttr.AvBindAttr.stVideo.stIn.ModId = DP_M2S_MOD_VI;
	stAttr.AvBindAttr.stVideo.stIn.u32DevId = 0;
	stAttr.AvBindAttr.stVideo.stIn.u32ChnId = 0;//not use

	stAttr.AvBindAttr.stVideo.stOut.ModId = DP_M2S_MOD_VENC;
	stAttr.AvBindAttr.stVideo.stOut.u32DevId = 0;//not use
	stAttr.AvBindAttr.stVideo.stOut.u32ChnId = 1;

	/* s3 DP_M2S_AENC_ATTR_S stAenc */

	/* s4 DP_M2S_VENC_ATTR_S stVenc; */
	stAttr.stVenc.bCrop = DP_FALSE;
	stAttr.stVenc.bZoom = DP_FALSE;
	stAttr.stVenc.bOsd = DP_FALSE;

	DP_M2S_CROP_ATTR_S stCrop;
	stCrop.s32X = 0;
	stCrop.s32Y = 0;
	stCrop.u32Width = 1920;
	stCrop.u32Height = 1080;
	memcpy(&stAttr.stVenc.stCrop, &stCrop, sizeof(DP_M2S_CROP_ATTR_S));

	DP_M2S_ZOOM_ATTR_S stZoom;
	stZoom.enType = DP_M2S_ZOOM_RECT;
	stZoom.stRect.s32X = 0;
	stZoom.stRect.s32Y = 0;
	stZoom.stRect.u32Width = 1920;
	stZoom.stRect.u32Height = 1080;
	memcpy(&stAttr.stVenc.stZoom, &stZoom, sizeof(DP_M2S_ZOOM_ATTR_S));

	/*
	 DP_M2S_OSD_ATTR_S stOsd;//not use
	 memcpy(&stAttr.stVenc.stOsd, xx, sizeof(DP_M2S_OSD_ATTR_S));
	 */

	DP_M2S_ALG_ATTR_S stAlg;
	stAlg.enAlg=DP_M2S_ALG_H264_ENC;
	stAlg.stH264Enc.u32FrmRate = 30;
	stAlg.stH264Enc.stSize.u32Width = stZoom.stRect.u32Width;
	stAlg.stH264Enc.stSize.u32Height = stZoom.stRect.u32Height;
	stAlg.stH264Enc.enRcMode = DP_M2S_RC_MODE_H264CBR;
	stAlg.stH264Enc.u32Bitrate = 20*1024;
	stAlg.stH264Enc.enProfile = DP_M2S_ALG_PROFILE_H264_BP;
	stAlg.stH264Enc.u32Gop = 30;
	stAlg.stH264Enc.u16SF = 0;
	stAlg.stH264Enc.u16TF = 0;
	memcpy(&stAttr.stVenc.stAlg, &stAlg, sizeof(DP_M2S_ALG_ATTR_S));

	/* s5 DP_M2S_STREAM_ATTR_S stStream; */
	//DP_M2S_STREAM_ATTR_S stStream;
	stAttr.stStream.enType = DP_M2S_STREAM_RTSP_SERVER;
	DP_M2S_RTSP_SERVER_ATTR_S stRtspServer;
	stRtspServer.bOpen = DP_TRUE;
	stRtspServer.bMulticast = DP_FALSE;
	stRtspServer.s32TransType = 0;//udp
	stRtspServer.s32ConnTimeout = 60;
	stRtspServer.s32ConnMax = 128;
	stRtspServer.s32ConnNums = 0;//not use
	//stRtspServer.au8Url[] = NULL;//not use
	memcpy(&stAttr.stStream.stRtspServer, &stRtspServer, sizeof(DP_M2S_RTSP_SERVER_ATTR_S));

	memcpy(pstAttr, &stAttr, sizeof(DP_M2S_AVENC_SET_INFO_S));
	return 0;
}

static int cmd_set_avenc_default_512(DP_VOID *pPtr)
{
	DP_M2S_AVENC_SET_INFO_S *pstAttr = (DP_M2S_AVENC_SET_INFO_S *)pPtr;
	DP_M2S_AVENC_SET_INFO_S stAttr;

	memset(&stAttr, 0, sizeof(DP_M2S_AVENC_SET_INFO_S));

	/* s1 TskId*/
	stAttr.TskId = 512;    //512~513

	/* s2 DP_M2S_AVBIND_ATTR_S */
	stAttr.AvBindAttr.enBindType=DP_M2S_AVBIND_VI2VENC;/* no audio */

	//stAttr.AvBindAttr.stAudio = NULL;
	stAttr.AvBindAttr.stVideo.stIn.ModId = DP_M2S_MOD_VI;
	stAttr.AvBindAttr.stVideo.stIn.u32DevId = 0;
	stAttr.AvBindAttr.stVideo.stIn.u32ChnId = 0;//not use

	stAttr.AvBindAttr.stVideo.stOut.ModId = DP_M2S_MOD_VENC;
	stAttr.AvBindAttr.stVideo.stOut.u32DevId = 0;//not use
	stAttr.AvBindAttr.stVideo.stOut.u32ChnId = 0;

	/* s3 DP_M2S_AENC_ATTR_S stAenc */

	/* s4 DP_M2S_VENC_ATTR_S stVenc; */
	stAttr.stVenc.bCrop = DP_FALSE;
	stAttr.stVenc.bZoom = DP_FALSE;
	stAttr.stVenc.bOsd = DP_FALSE;

	DP_M2S_CROP_ATTR_S stCrop;
	stCrop.s32X = 0;
	stCrop.s32Y = 0;
	stCrop.u32Width = 3840;
	stCrop.u32Height = 2160;
	memcpy(&stAttr.stVenc.stCrop, &stCrop, sizeof(DP_M2S_CROP_ATTR_S));

	DP_M2S_ZOOM_ATTR_S stZoom;
	stZoom.enType = DP_M2S_ZOOM_RECT;
	stZoom.stRect.s32X = 0;
	stZoom.stRect.s32Y = 0;
	stZoom.stRect.u32Width = 3840;
	stZoom.stRect.u32Height = 2160;
	memcpy(&stAttr.stVenc.stZoom, &stZoom, sizeof(DP_M2S_ZOOM_ATTR_S));

	/*
	 DP_M2S_OSD_ATTR_S stOsd;//not use
	 memcpy(&stAttr.stVenc.stOsd, xx, sizeof(DP_M2S_OSD_ATTR_S));
	 */

	DP_M2S_ALG_ATTR_S stAlg;
	stAlg.enAlg=DP_M2S_ALG_H264_ENC;
	stAlg.stH264Enc.u32FrmRate = 30;
	stAlg.stH264Enc.stSize.u32Width = stZoom.stRect.u32Width;
	stAlg.stH264Enc.stSize.u32Height = stZoom.stRect.u32Height;
	stAlg.stH264Enc.enRcMode = DP_M2S_RC_MODE_H264CBR;
	stAlg.stH264Enc.u32Bitrate = 20*1024;
	stAlg.stH264Enc.enProfile = DP_M2S_ALG_PROFILE_H264_BP;
	stAlg.stH264Enc.u32Gop = 30;
	stAlg.stH264Enc.u16SF = 0;
	stAlg.stH264Enc.u16TF = 0;
	memcpy(&stAttr.stVenc.stAlg, &stAlg, sizeof(DP_M2S_ALG_ATTR_S));

	/* s5 DP_M2S_STREAM_ATTR_S stStream; */
	//DP_M2S_STREAM_ATTR_S stStream;
	stAttr.stStream.enType = DP_M2S_STREAM_RTSP_SERVER;
	DP_M2S_RTSP_SERVER_ATTR_S stRtspServer;
	stRtspServer.bOpen = DP_TRUE;
	stRtspServer.bMulticast = DP_FALSE;
	stRtspServer.s32TransType = 0;//udp
	stRtspServer.s32ConnTimeout = 60;
	stRtspServer.s32ConnMax = 128;
	stRtspServer.s32ConnNums = 0;//not use
	//stRtspServer.au8Url[] = NULL;//not use
	memcpy(&stAttr.stStream.stRtspServer, &stRtspServer, sizeof(DP_M2S_RTSP_SERVER_ATTR_S));

	memcpy(pstAttr, &stAttr, sizeof(DP_M2S_AVENC_SET_INFO_S));
	return 0;
}
#endif

static int cmd_set_aenc_default(DP_VOID *pPtr, DP_S32 s32TskId, DP_U32 u32Width,
		DP_U32 u32Height, DP_U32 u32Bitrate, DP_S32 AencChn)    //0 0 0 0 0
{
	DP_M2S_AVENC_SET_INFO_S *pstAttr = (DP_M2S_AVENC_SET_INFO_S *) pPtr;
	DP_M2S_AVENC_SET_INFO_S stAttr;

	memset(&stAttr, 0, sizeof(DP_M2S_AVENC_SET_INFO_S));

	/* s1 TskId*/
	stAttr.TskId = s32TskId;    //0

	/* s2 DP_M2S_AVBIND_ATTR_S */
	stAttr.AvBindAttr.enBindType = DP_M2S_AVBIND_AI2AENC;/* no video */

	stAttr.AvBindAttr.stAudio.stIn.ModId = DP_M2S_MOD_AI;
	stAttr.AvBindAttr.stAudio.stIn.u32DevId = 0;
	stAttr.AvBindAttr.stAudio.stIn.u32ChnId = 0;    //not use

	stAttr.AvBindAttr.stAudio.stOut.ModId = DP_M2S_MOD_AENC;
	stAttr.AvBindAttr.stAudio.stOut.u32DevId = 0;//not use
	stAttr.AvBindAttr.stAudio.stOut.u32ChnId = AencChn;

	//stAttr.AvBindAttr.stVideo = NULL;

	/* s3 DP_M2S_AENC_ATTR_S stAenc */
	stAttr.stAenc.enAlg = DP_M2S_ALG_AAC_ENC;
	stAttr.stAenc.s32Bitrate = 48000;

	/* s4 DP_M2S_VENC_ATTR_S stVenc; */
	stAttr.stVenc.bCrop = DP_FALSE;
	stAttr.stVenc.bZoom = DP_FALSE;
	stAttr.stVenc.bOsd = DP_FALSE;

	DP_M2S_CROP_ATTR_S stCrop;
	stCrop.s32X = 0;
	stCrop.s32Y = 0;
	stCrop.u32Width = u32Width;
	stCrop.u32Height = u32Height;
	memcpy(&stAttr.stVenc.stCrop, &stCrop, sizeof(DP_M2S_CROP_ATTR_S));

	DP_M2S_ZOOM_ATTR_S stZoom;
	stZoom.enType = DP_M2S_ZOOM_RECT;
	stZoom.stRect.s32X = 0;
	stZoom.stRect.s32Y = 0;
	stZoom.stRect.u32Width = u32Width;
	stZoom.stRect.u32Height = u32Height;
	memcpy(&stAttr.stVenc.stZoom, &stZoom, sizeof(DP_M2S_ZOOM_ATTR_S));

	DP_M2S_OSD_ATTR_S stOsd;
	stOsd.enType = DP_M2S_OSD_BUTT;
	memcpy(&stAttr.stVenc.stOsd, &stOsd, sizeof(DP_M2S_OSD_ATTR_S));

	DP_M2S_ALG_ATTR_S stAlg;
	stAlg.enAlg = DP_M2S_ALG_H264_ENC;
	stAlg.stH264Enc.u32FrmRate = 30;
	stAlg.stH264Enc.stSize.u32Width = stZoom.stRect.u32Width;
	stAlg.stH264Enc.stSize.u32Height = stZoom.stRect.u32Height;
	stAlg.stH264Enc.enRcMode = DP_M2S_RC_MODE_H264CBR;
	stAlg.stH264Enc.u32Bitrate = u32Bitrate;
	stAlg.stH264Enc.enProfile = DP_M2S_ALG_PROFILE_H264_BP;
	stAlg.stH264Enc.u32Gop = 30;
	stAlg.stH264Enc.u16SF = 0;
	stAlg.stH264Enc.u16TF = 0;
	memcpy(&stAttr.stVenc.stAlg, &stAlg, sizeof(DP_M2S_ALG_ATTR_S));

	/* s5 DP_M2S_STREAM_ATTR_S stStream; */
	//DP_M2S_STREAM_ATTR_S stStream;
	stAttr.stStream.enType = DP_M2S_STREAM_RTSP_SERVER;
	DP_M2S_RTSP_SERVER_ATTR_S stRtspServer;
	stRtspServer.bOpen = DP_TRUE;
	stRtspServer.bMulticast = DP_FALSE;
	stRtspServer.s32TransType = 0;//udp
	stRtspServer.s32ConnTimeout = 60;
	stRtspServer.s32ConnMax = 128;
	stRtspServer.s32ConnNums = 0;//not use
	//stRtspServer.au8Url[] = NULL;//not use
	memcpy(&stAttr.stStream.stRtspServer, &stRtspServer,
			sizeof(DP_M2S_RTSP_SERVER_ATTR_S));

	memcpy(pstAttr, &stAttr, sizeof(DP_M2S_AVENC_SET_INFO_S));
	return 0;
}

static int cmd_set_venc_default(DP_VOID *pPtr, DP_S32 s32TskId, DP_U32 u32Width,
		DP_U32 u32Height, DP_U32 u32Bitrate, DP_S32 VencChn) //256 1920 1080 4000 0
{														//257 1920 1080 4000 1
	DP_M2S_AVENC_SET_INFO_S *pstAttr = (DP_M2S_AVENC_SET_INFO_S *) pPtr;
	DP_M2S_AVENC_SET_INFO_S stAttr;

	memset(&stAttr, 0, sizeof(DP_M2S_AVENC_SET_INFO_S));

	/* s1 TskId*/
	stAttr.TskId = s32TskId;										//256~257

	/* s2 DP_M2S_AVBIND_ATTR_S */
	stAttr.AvBindAttr.enBindType = DP_M2S_AVBIND_VI2VENC;/* no audio */

	//stAttr.AvBindAttr.stAudio = NULL;
	stAttr.AvBindAttr.stVideo.stIn.ModId = DP_M2S_MOD_VI;
	stAttr.AvBindAttr.stVideo.stIn.u32DevId = 0;
	stAttr.AvBindAttr.stVideo.stIn.u32ChnId = 0;//not use

	stAttr.AvBindAttr.stVideo.stOut.ModId = DP_M2S_MOD_VENC;
	stAttr.AvBindAttr.stVideo.stOut.u32DevId = 0;//not use
	stAttr.AvBindAttr.stVideo.stOut.u32ChnId = VencChn;

	/* s3 DP_M2S_AENC_ATTR_S stAenc */

	/* s4 DP_M2S_VENC_ATTR_S stVenc; */
	stAttr.stVenc.bCrop = DP_FALSE;
	stAttr.stVenc.bZoom = DP_FALSE;
	stAttr.stVenc.bOsd = DP_FALSE;

	DP_M2S_CROP_ATTR_S stCrop;
	stCrop.s32X = 0;
	stCrop.s32Y = 0;
	stCrop.u32Width = u32Width;
	stCrop.u32Height = u32Height;
	memcpy(&stAttr.stVenc.stCrop, &stCrop, sizeof(DP_M2S_CROP_ATTR_S));

	DP_M2S_ZOOM_ATTR_S stZoom;
	stZoom.enType = DP_M2S_ZOOM_RECT;
	stZoom.stRect.s32X = 0;
	stZoom.stRect.s32Y = 0;
	stZoom.stRect.u32Width = u32Width;
	stZoom.stRect.u32Height = u32Height;
	memcpy(&stAttr.stVenc.stZoom, &stZoom, sizeof(DP_M2S_ZOOM_ATTR_S));

	DP_M2S_OSD_ATTR_S stOsd;
	stOsd.enType = DP_M2S_OSD_BUTT;
	memcpy(&stAttr.stVenc.stOsd, &stOsd, sizeof(DP_M2S_OSD_ATTR_S));

	DP_M2S_ALG_ATTR_S stAlg;
	stAlg.enAlg = DP_M2S_ALG_H264_ENC;
	stAlg.stH264Enc.u32FrmRate = 30;
	stAlg.stH264Enc.stSize.u32Width = stZoom.stRect.u32Width;
	stAlg.stH264Enc.stSize.u32Height = stZoom.stRect.u32Height;
	stAlg.stH264Enc.enRcMode = DP_M2S_RC_MODE_H264CBR;
	stAlg.stH264Enc.u32Bitrate = u32Bitrate;
	stAlg.stH264Enc.enProfile = DP_M2S_ALG_PROFILE_H264_BP;
	stAlg.stH264Enc.u32Gop = 30;
	stAlg.stH264Enc.u16SF = 0;
	stAlg.stH264Enc.u16TF = 0;
	memcpy(&stAttr.stVenc.stAlg, &stAlg, sizeof(DP_M2S_ALG_ATTR_S));

	/* s5 DP_M2S_STREAM_ATTR_S stStream; */
	//DP_M2S_STREAM_ATTR_S stStream;
	stAttr.stStream.enType = DP_M2S_STREAM_RTSP_SERVER;
	DP_M2S_RTSP_SERVER_ATTR_S stRtspServer;
	stRtspServer.bOpen = DP_TRUE;
	stRtspServer.bMulticast = DP_FALSE;
	stRtspServer.s32TransType = 0;//udp
	stRtspServer.s32ConnTimeout = 60;
	stRtspServer.s32ConnMax = 128;
	stRtspServer.s32ConnNums = 0;//not use
	//stRtspServer.au8Url[] = NULL;//not use
	memcpy(&stAttr.stStream.stRtspServer, &stRtspServer,
			sizeof(DP_M2S_RTSP_SERVER_ATTR_S));

	memcpy(pstAttr, &stAttr, sizeof(DP_M2S_AVENC_SET_INFO_S));
	return 0;
}

static int cmd_set_avenc_default(DP_VOID *pPtr, DP_S32 s32TskId,
		DP_U32 u32Width, DP_U32 u32Height, DP_U32 u32Bitrate, DP_S32 AencChn,
		DP_S32 VencChn)    //512 1920 1080 4000 1 2
{														//513 1920 1080 4000 2 3
	DP_M2S_AVENC_SET_INFO_S *pstAttr = (DP_M2S_AVENC_SET_INFO_S *) pPtr;
	DP_M2S_AVENC_SET_INFO_S stAttr;

	memset(&stAttr, 0, sizeof(DP_M2S_AVENC_SET_INFO_S));

	/* s1 TskId*/
	stAttr.TskId = s32TskId;											//513

	/* s2 DP_M2S_AVBIND_ATTR_S */
	stAttr.AvBindAttr.enBindType = DP_M2S_AVBIND_AI2AENC_VI2VENC;/* AV */

	stAttr.AvBindAttr.stAudio.stIn.ModId = DP_M2S_MOD_AI;
	stAttr.AvBindAttr.stAudio.stIn.u32DevId = 0;
	stAttr.AvBindAttr.stAudio.stIn.u32ChnId = 0;					//not use

	stAttr.AvBindAttr.stAudio.stOut.ModId = DP_M2S_MOD_AENC;
	stAttr.AvBindAttr.stAudio.stOut.u32DevId = 0;//not use
	stAttr.AvBindAttr.stAudio.stOut.u32ChnId = AencChn;

	stAttr.AvBindAttr.stVideo.stIn.ModId = DP_M2S_MOD_VI;
	stAttr.AvBindAttr.stVideo.stIn.u32DevId = 0;
	stAttr.AvBindAttr.stVideo.stIn.u32ChnId = 0;//not use

	stAttr.AvBindAttr.stVideo.stOut.ModId = DP_M2S_MOD_VENC;
	stAttr.AvBindAttr.stVideo.stOut.u32DevId = 0;//not use
	stAttr.AvBindAttr.stVideo.stOut.u32ChnId = VencChn;

	/* s3 DP_M2S_AENC_ATTR_S stAenc */
	stAttr.stAenc.enAlg = DP_M2S_ALG_AAC_ENC;
	stAttr.stAenc.s32Bitrate = 48000;

	/* s4 DP_M2S_VENC_ATTR_S stVenc; */
	stAttr.stVenc.bCrop = DP_FALSE;
	stAttr.stVenc.bZoom = DP_FALSE;
	stAttr.stVenc.bOsd = DP_FALSE;

	DP_M2S_CROP_ATTR_S stCrop;
	stCrop.s32X = 0;
	stCrop.s32Y = 0;
	stCrop.u32Width = u32Width;
	stCrop.u32Height = u32Height;
	memcpy(&stAttr.stVenc.stCrop, &stCrop, sizeof(DP_M2S_CROP_ATTR_S));

	DP_M2S_ZOOM_ATTR_S stZoom;
	stZoom.enType = DP_M2S_ZOOM_RECT;
	stZoom.stRect.s32X = 0;
	stZoom.stRect.s32Y = 0;
	stZoom.stRect.u32Width = u32Width;
	stZoom.stRect.u32Height = u32Height;
	memcpy(&stAttr.stVenc.stZoom, &stZoom, sizeof(DP_M2S_ZOOM_ATTR_S));

	DP_M2S_OSD_ATTR_S stOsd;
	stOsd.enType = DP_M2S_OSD_BUTT;
	memcpy(&stAttr.stVenc.stOsd, &stOsd, sizeof(DP_M2S_OSD_ATTR_S));

	DP_M2S_ALG_ATTR_S stAlg;
	stAlg.enAlg = DP_M2S_ALG_H264_ENC;
	stAlg.stH264Enc.u32FrmRate = 30;
	stAlg.stH264Enc.stSize.u32Width = stZoom.stRect.u32Width;
	stAlg.stH264Enc.stSize.u32Height = stZoom.stRect.u32Height;
	stAlg.stH264Enc.enRcMode = DP_M2S_RC_MODE_H264CBR;
	stAlg.stH264Enc.u32Bitrate = u32Bitrate;
	stAlg.stH264Enc.enProfile = DP_M2S_ALG_PROFILE_H264_BP;
	stAlg.stH264Enc.u32Gop = 30;
	stAlg.stH264Enc.u16SF = 0;
	stAlg.stH264Enc.u16TF = 0;
	memcpy(&stAttr.stVenc.stAlg, &stAlg, sizeof(DP_M2S_ALG_ATTR_S));

	/* s5 DP_M2S_STREAM_ATTR_S stStream; */
	//DP_M2S_STREAM_ATTR_S stStream;
	stAttr.stStream.enType = DP_M2S_STREAM_RTSP_SERVER;
	DP_M2S_RTSP_SERVER_ATTR_S stRtspServer;
	stRtspServer.bOpen = DP_TRUE;
	stRtspServer.bMulticast = DP_FALSE;
	stRtspServer.s32TransType = 0;//udp
	stRtspServer.s32ConnTimeout = 60;
	stRtspServer.s32ConnMax = 128;
	stRtspServer.s32ConnNums = 0;//not use
	//stRtspServer.au8Url[] = NULL;//not use
	memcpy(&stAttr.stStream.stRtspServer, &stRtspServer,
			sizeof(DP_M2S_RTSP_SERVER_ATTR_S));

	memcpy(pstAttr, &stAttr, sizeof(DP_M2S_AVENC_SET_INFO_S));
	return 0;
}

static int prot_head_write(void *pPtr, int cmdId, DP_BOOL bAck) {
	DP_U16 u16WrtLen = 0;
	//printf("======================>cmdId %d\n", cmdId);
	DP_M2S_INF_PROT_HEAD_S stProtHead;
	stProtHead.u16HeadTag = DP_M2S_INF_PROT_PKT_HEAD;
	u16WrtLen = DP_M2S_INF_PROT_PKT_HEAD_LEN;
	stProtHead.u16PacketLen =
	(bAck != DP_FALSE) ? (u16WrtLen | 0x8000) : (u16WrtLen & 0x7fff);
	stProtHead.u8CommandID = cmdId;

	memcpy(pPtr, &stProtHead, u16WrtLen);
	//printf("======================>cmdId2 %d, sizeof %d u16WrtLen %d\n", stProtHead.u8CommandID, sizeof(DP_M2S_INF_PROT_HEAD_S), u16WrtLen);

	return u16WrtLen;

}

int cmd_send_attr_set(char *cmd_type, unsigned char *cmd_buf, int *cmd_len,
		char *cmd_argv2, char *cmd_argv3, char *cmd_argv4, char *cmd_argv5,
		char *cmd_argv6, char *cmd_argv7) {
	printf("cmd: %s\n", cmd_type);
	//int s32NeedWaitResult = 0, s32StProtHeadSize = 0;
	int s32NeedWaitResult = 0;
	//DP_M2S_INF_PROT_HEAD_S stProtHead;

	if (!memcmp(cmd_type, "init", strlen("init"))) {
		/* set pkt head */
		int cmdId = DP_M2S_CMD_INIT;
		*cmd_len = prot_head_write((DP_VOID *) cmd_buf, cmdId, DP_TRUE);
		s32NeedWaitResult = 1;
	} else if (!memcmp(cmd_type, "deinit", strlen("deinit"))) {
		/* set pkt head */
		int cmdId = DP_M2S_CMD_DEINIT;
		*cmd_len = prot_head_write(cmd_buf, cmdId, DP_TRUE);
		s32NeedWaitResult = 1;
	} else if (!memcmp(cmd_type, "getinfo", strlen("getinfo"))) {
		int CmdLen = strlen(cmd_type);
		int type = atoi(&cmd_type[CmdLen - 1]);
		switch (type) {
			case DP_M2S_INFO_TYPE_GET_AI: {
				printf("DP_M2S_INFO_TYPE_GET_AI\n");
				/* set pkt head */
				DP_BOOL bAck = DP_TRUE;
				DP_M2S_CMD_GETINFO_S stCmdGetInfo;
				DP_U16 u16StCmdGetInfoLen = sizeof(DP_M2S_CMD_GETINFO_S);
				stCmdGetInfo.stHeader.u16HeadTag = DP_M2S_INF_PROT_PKT_HEAD;
				stCmdGetInfo.stHeader.u16PacketLen =
				(bAck != DP_FALSE) ?
				(u16StCmdGetInfoLen | 0x8000) :
				(u16StCmdGetInfoLen & 0x7fff);
				stCmdGetInfo.stHeader.u8CommandID = DP_M2S_CMD_GETINFO;
				stCmdGetInfo.enInfoTYpe = DP_M2S_INFO_TYPE_GET_AI;
				memcpy(cmd_buf, &stCmdGetInfo, u16StCmdGetInfoLen);
				*cmd_len = u16StCmdGetInfoLen;
				printf("[Dbg] getinfo: ai info(%d).\n", *cmd_len);
				s32NeedWaitResult = 1;
			}
			break;
			case DP_M2S_INFO_TYPE_GET_VI: {
				printf("DP_M2S_INFO_TYPE_GET_VI\n");
				/* set pkt head */
				DP_BOOL bAck = DP_TRUE;
				DP_M2S_CMD_GETINFO_S stCmdGetInfo;
				DP_U16 u16StCmdGetInfoLen = sizeof(DP_M2S_CMD_GETINFO_S);
				stCmdGetInfo.stHeader.u16HeadTag = DP_M2S_INF_PROT_PKT_HEAD;
				stCmdGetInfo.stHeader.u16PacketLen =
				(bAck != DP_FALSE) ?
				(u16StCmdGetInfoLen | 0x8000) :
				(u16StCmdGetInfoLen & 0x7fff);
				stCmdGetInfo.stHeader.u8CommandID = DP_M2S_CMD_GETINFO;
				stCmdGetInfo.enInfoTYpe = DP_M2S_INFO_TYPE_GET_VI;
				memcpy(cmd_buf, &stCmdGetInfo, u16StCmdGetInfoLen);
				*cmd_len = u16StCmdGetInfoLen;
				printf("[Dbg] getinfo: vi info(%d).\n", *cmd_len);
				s32NeedWaitResult = 1;
			}
			break;
			case DP_M2S_INFO_TYPE_GET_AVENC: {
				printf("DP_M2S_INFO_TYPE_GET_AVENC\n");
				/* set pkt head */
				DP_BOOL bAck = DP_TRUE;
				DP_M2S_CMD_GETINFO_S stCmdGetInfo;
				DP_U16 u16StCmdGetInfoLen = sizeof(DP_M2S_CMD_GETINFO_S);
				stCmdGetInfo.stHeader.u16HeadTag = DP_M2S_INF_PROT_PKT_HEAD;
				stCmdGetInfo.stHeader.u16PacketLen =
				(bAck != DP_FALSE) ?
				(u16StCmdGetInfoLen | 0x8000) :
				(u16StCmdGetInfoLen & 0x7fff);
				stCmdGetInfo.stHeader.u8CommandID = DP_M2S_CMD_GETINFO;
				stCmdGetInfo.enInfoTYpe = DP_M2S_INFO_TYPE_GET_AVENC;
				memcpy(cmd_buf, &stCmdGetInfo, u16StCmdGetInfoLen);
				*cmd_len = u16StCmdGetInfoLen;
				printf("[Dbg] getinfo: vi info(%d).\n", *cmd_len);
				s32NeedWaitResult = 1;
			}
			break;
			case DP_M2S_INFO_TYPE_GET_AVDEC:
			case DP_M2S_INFO_TYPE_GET_AO:
			case DP_M2S_INFO_TYPE_GET_VO:
			default: {
				printf("[err] enInfoTYpe %d Invalid\n", type);
			}
			break;
		}    //switch

	} else if (!memcmp(cmd_type, "setinfo", strlen("setinfo"))) {
		/* set pkt head */
		int cmdId = DP_M2S_CMD_SETINFO;
		int CmdLen = strlen(cmd_type);
		*cmd_len = prot_head_write(cmd_buf, cmdId, DP_TRUE);
		//printf("[%s] len %d ===========\n", cmd_type, CmdLen);
		int type = atoi(&cmd_type[CmdLen - 1]);

		switch (type) {
			case DP_M2S_INFO_TYPE_SET_AI: {
				printf("DP_M2S_INFO_TYPE_SET_AI\n");
			}
			break;
			case DP_M2S_INFO_TYPE_SET_VI: {
				printf("DP_M2S_INFO_TYPE_SET_VI\n");
			}
			break;
			case DP_M2S_INFO_TYPE_SET_AVENC: {

				DP_M2S_CMD_SETINFO_S stCmdSetInfo;
				DP_S32 stCmdSetInfoLen = sizeof(DP_M2S_CMD_SETINFO_S);
				DP_M2S_AVENC_SET_INFO_S stAVEncInfo;
				DP_S32 stAVEncInfoLen = sizeof(DP_M2S_AVENC_SET_INFO_S);
				DP_BOOL bAck = DP_TRUE;

				printf(
						"[Dbg]DP_M2S_CMD_SETINFO_S(id %d, len %d) to DP_M2S_AVENC_SET_INFO_S(id %d, len %d)\n",
						DP_M2S_CMD_SETINFO, stCmdSetInfoLen,
						DP_M2S_INFO_TYPE_SET_AVENC, stAVEncInfoLen);

				DP_S32 s32SendDataLen = stCmdSetInfoLen - 4 + stAVEncInfoLen;

				/*s1: DP_M2S_INF_PROT_HEAD_S stHeader; */
				stCmdSetInfo.stHeader.u16HeadTag = DP_M2S_INF_PROT_PKT_HEAD;
				stCmdSetInfo.stHeader.u16PacketLen =
				(bAck != DP_FALSE) ?
				(s32SendDataLen | 0x8000) :
				(s32SendDataLen & 0x7fff);
				stCmdSetInfo.stHeader.u8CommandID = DP_M2S_CMD_SETINFO;

				/* s2 */
				stCmdSetInfo.enInfoTYpe = DP_M2S_INFO_TYPE_SET_AVENC;
				/* s3 */
				stCmdSetInfo.u32InfoLen = stAVEncInfoLen;
				/* s4 stCmdSetInfo.pu8InfoData */
				DP_S32 s32TskId = -1;
				DP_U32 u32Width = 0;
				DP_U32 u32Height = 0;
				DP_U32 u32Bitrate = 0;
				DP_U32 s32AencChn = -1;
				DP_U32 s32VencChn = -1;
				s32TskId = atoi(cmd_argv2);
				u32Width = atoi(cmd_argv3);
				u32Height = atoi(cmd_argv4);
				u32Bitrate = atoi(cmd_argv5);
				s32AencChn = atoi(cmd_argv6);
				s32VencChn = atoi(cmd_argv7);
				if ((s32TskId >= 0) && (s32TskId <= 255)) {
					cmd_set_aenc_default(&stAVEncInfo, s32TskId, u32Width,
							u32Height, u32Bitrate, s32AencChn);
				} else if ((s32TskId >= 256) && (s32TskId <= 511)) {
					cmd_set_venc_default(&stAVEncInfo, s32TskId, u32Width,
							u32Height, u32Bitrate, s32VencChn);
				} else {
					cmd_set_avenc_default(&stAVEncInfo, s32TskId, u32Width,
							u32Height, u32Bitrate, s32AencChn, s32VencChn);
				}
				cmd_set_avenc_print(stAVEncInfo);

				/* s5 write data */
				DP_U8 *pBuf = (DP_U8 *) calloc(1, s32SendDataLen);
				if (NULL == pBuf) {
					printf("[Err] calloc %u bytes failed\n", s32SendDataLen);
				} else {
					memcpy(pBuf, &stCmdSetInfo, stCmdSetInfoLen - 4); //-pu8InfoData's len
					DP_S32 s32Offset = stCmdSetInfoLen - 4;
					memcpy(pBuf + s32Offset, &stAVEncInfo, stAVEncInfoLen);

					memcpy(cmd_buf, pBuf, s32SendDataLen);
					*cmd_len = s32SendDataLen;

					free(pBuf);
					pBuf = NULL;
				}
			}
			break;
			case DP_M2S_INFO_TYPE_SET_AVDEC:
			case DP_M2S_INFO_TYPE_SET_AO:
			case DP_M2S_INFO_TYPE_SET_VO:
			default: {
				printf("[err] enInfoTYpe %d Invalid\n", type);
			}
			break;
		}    //switch
		s32NeedWaitResult = 1;
	} else if (!memcmp(cmd_type, "exit", strlen("exit"))) {
		/* set pkt head */
		int cmdId = DP_M2S_CMD_EXIT;
		*cmd_len = prot_head_write(cmd_buf, cmdId, DP_TRUE);
		s32NeedWaitResult = 1;
	}

	return s32NeedWaitResult;
}

int cmd_recv_attr_parse(unsigned char *pBuf, int Len) {
	DP_M2S_INF_PROT_HEAD_S *pstProtHead = (DP_M2S_INF_PROT_HEAD_S *) pBuf;
	printf("u16HeadTag 0x%x, u16PacketLen %d u8CommandID 0x%x\n",
			pstProtHead->u16HeadTag, pstProtHead->u16PacketLen,
			pstProtHead->u8CommandID);
#if 1
	printf("************************************\n");
	for (int i = 0; i < pstProtHead->u16PacketLen; i++) {
		if (i % 16 == 0 && i != 0) {
			printf("\n");
		}
		printf("0x%x ", pBuf[i]);
	}
	printf("\n");
	printf("************************************\n");
#endif
	switch (pstProtHead->u8CommandID) {
		case DP_M2S_CMD_INIT: {
			printf("[Dbg] DP_M2S_CMD_INIT %d\n", DP_M2S_CMD_INIT);
		}
		break;
		case DP_M2S_CMD_DEINIT: {
			printf("[Dbg] DP_M2S_CMD_DEINIT %d\n", DP_M2S_CMD_DEINIT);
		}
		break;
		case DP_M2S_CMD_GETINFO: {
			printf("[Dbg] DP_M2S_CMD_GETINFO %d\n", DP_M2S_CMD_GETINFO);
			DP_M2S_CMD_GETINFO_RESPOND_S *pStCmdGetInfoResp = NULL;
			pStCmdGetInfoResp = (DP_M2S_CMD_GETINFO_RESPOND_S *) pBuf;
			printf("u32Success %d\n", pStCmdGetInfoResp->u32Success);
			printf("u32InfoLen %d\n", pStCmdGetInfoResp->u32InfoLen);
			switch (pStCmdGetInfoResp->enInfoTYpe) {
				case DP_M2S_INFO_TYPE_GET_AI: {
					printf("DP_M2S_INFO_TYPE_GET_AI\n");
					DP_S32 s32Offset = pstProtHead->u16PacketLen
					- pStCmdGetInfoResp->u32InfoLen;
					printf("s32Offset %d = %d - %d\n", s32Offset,
							pstProtHead->u16PacketLen, pStCmdGetInfoResp->u32InfoLen);
					DP_M2S_AI_GET_INFO_S stAiGetInfo;
					memcpy(&stAiGetInfo, pBuf + s32Offset,
							sizeof(DP_M2S_AI_GET_INFO_S));
					print_ai_get_attr(stAiGetInfo);
				}
				break;
				case DP_M2S_INFO_TYPE_GET_VI: {
					printf("DP_M2S_INFO_TYPE_GET_VI\n");
					DP_S32 s32Offset = pstProtHead->u16PacketLen
					- pStCmdGetInfoResp->u32InfoLen;
					printf("s32Offset %d = %d - %d\n", s32Offset,
							pstProtHead->u16PacketLen, pStCmdGetInfoResp->u32InfoLen);
					DP_M2S_VI_GET_INFO_S stViGetInfo;
					memcpy(&stViGetInfo, pBuf + s32Offset,
							sizeof(DP_M2S_VI_GET_INFO_S));
					print_vi_get_attr(stViGetInfo);
				}
				break;
				case DP_M2S_INFO_TYPE_GET_AVENC: {
					printf("DP_M2S_INFO_TYPE_GET_AVENC\n");
					DP_S32 s32Offset = pstProtHead->u16PacketLen
					- pStCmdGetInfoResp->u32InfoLen;
					printf("s32Offset %d = %d - %d\n", s32Offset,
							pstProtHead->u16PacketLen, pStCmdGetInfoResp->u32InfoLen);
#if 0
					DP_M2S_AVENC_GET_INFO_S stAVEncGetInfo;
					//memcpy(&stAVEncGetInfo, pBuf+s32Offset, sizeof(DP_M2S_AVENC_GET_INFO_S));
					//memcpy(&stAVEncGetInfo, pBuf+s32Offset+sizeof(DP_M2S_AVENC_GET_INFO_S), sizeof(DP_M2S_AVENC_GET_INFO_S));
					print_avenc_get_attr(stAVEncGetInfo);
#else

					DP_U32 u32InfoLen = pStCmdGetInfoResp->u32InfoLen;
					DP_U32 u32InfoCnt = u32InfoLen / sizeof(DP_M2S_AVENC_GET_INFO_S);
					printf("[Dbg] u32InfoLen %d u32InfoCnt %d\n", u32InfoLen,
							u32InfoCnt);
					for (int i = 0; i < u32InfoCnt; i++) {
						DP_M2S_AVENC_GET_INFO_S stAVEncGetInfo;
						memcpy(&stAVEncGetInfo,
								pBuf + s32Offset + i * sizeof(DP_M2S_AVENC_GET_INFO_S),
								sizeof(DP_M2S_AVENC_GET_INFO_S));
						print_avenc_get_attr(stAVEncGetInfo);
					}
#endif
				}
				break;
				case DP_M2S_INFO_TYPE_GET_AVDEC:
				case DP_M2S_INFO_TYPE_GET_AO:
				case DP_M2S_INFO_TYPE_GET_VO:
				default: {
					printf("[err] enInfoTYpe %d Invalid\n", pstProtHead->u8CommandID);
				}
				break;
			}                    //switch
		}
		break;
		case DP_M2S_CMD_SETINFO: {
			printf("[Dbg] DP_M2S_CMD_SETINFO %d\n", DP_M2S_CMD_SETINFO);
		}
		break;
		case DP_M2S_CMD_EXIT: {
			printf("[Dbg] DP_M2S_CMD_EXIT %d\n", DP_M2S_CMD_EXIT);
		}
		break;
		default:
		break;
	}

	return DP_TRUE;
}

int main(int argc, char *argv[]) {
	if (argc >= 2) {
		if (!memcmp(argv[1], "help", sizeof("help"))
				|| !memcmp(argv[1], "-help", sizeof("-help"))
				|| !memcmp(argv[1], "--help", sizeof("--help"))
				|| !memcmp(argv[1], "-h", sizeof("-h"))) {
			puts(MenuHelp);
			return -1;
		}
	}

	int ret = 0;
	int ReadFlag = 0;
	static struct sockaddr_un srv_addr;
	//printf("start ipc\n");
	gsConFd = socket(AF_UNIX, SOCK_STREAM, 0);
	//printf("gsConFd %d\n",gsConFd);
	if (gsConFd < 0) {
		perror("cannot create communication socket");
		printf("%d\n", gsConFd);
	} else {
		srv_addr.sun_family = AF_UNIX;
		strcpy(srv_addr.sun_path, DP_M2S_INF_PROT_UNIX_FIFO);

		//连接服务器
		ret = connect(gsConFd, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
		if (ret == -1) {
			close(gsConFd);
			printf("[Err] connect fail\n");
		} else {
			//printf("[Dbg] connect ok\n");
			memset(recv_php_buf, 0, sizeof(recv_php_buf));//清空socket_buf

			unsigned char cmd_buf[4096] = {0};
			int cmd_len = 0;
			ReadFlag = cmd_send_attr_set(argv[1], cmd_buf, &cmd_len, argv[2],
					argv[3], argv[4], argv[5], argv[6], argv[7]);
			if (cmd_len <= 0) {
				printf("[Err] cmd_len <= 0\n");
				return -1;
			}
			ret = write(gsConFd, cmd_buf, cmd_len);        //将数据传送到外部应用程序,发送实际长度
			if (ret != cmd_len) {
				printf("[Err] write %d Bytes failed\n", cmd_len);
			} else {
				for (int i = 0; i < cmd_len; i++) {
					printf("0x%x ", cmd_buf[i]);
					if (i == 16) {
						printf("\n");
					}
				}
				printf("\n");
				printf("[Dbg] write %d Bytes ok\n", cmd_len);

				FILE *fCrcFile = fopen("./send.txt", "w+");

				if (fCrcFile != NULL) {
					fseek(fCrcFile, 0L, SEEK_SET);
					DP_S32 s32TmpRet = fwrite(cmd_buf, cmd_len, 1, fCrcFile);
					if (s32TmpRet != 1) {
						printf("write %d Bytes to recv.txt failed\n", cmd_len);
					} else {
						printf("write %d Bytes to recv.txt\n", cmd_len);
					}
				}
				if (NULL != fCrcFile) {
					fclose(fCrcFile);
					fCrcFile = NULL;
				}
			}

			if (ReadFlag == 1) {
				memset(cmd_buf, 0, 4096);                        //清空socket_buf
				ret = read(gsConFd, cmd_buf, 4096);
				if (ret > 0) {
					printf("[Dbg] recv %d Bytes\n", ret);
					cmd_recv_attr_parse(cmd_buf, ret);
				} else {
					printf("[Err] recv failed\n");
				}
			}
			usleep(500000);
		}
		printf("###############end\n");
	}

	return 0;
}

#endif
