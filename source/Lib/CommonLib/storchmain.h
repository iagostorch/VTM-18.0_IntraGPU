/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   storchmain.h
 * Author: iagostorch
 *
 * Created on November 3, 2022
 */

#ifndef STORCHMAIN_H
#define STORCHMAIN_H

#include <iostream>
#include <fstream>
#include <time.h>
#include <sys/time.h>
#include <string>

#include "CommonLib/Unit.h"
#include "Slice.h"
#include "convCoeffs.h"
//#include "rapl.h"

// My directives

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
//         MACROS THAT INTERFERE ON THE ENCODING DECISIONS
//
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#define GPU_MIP 0                       // When enabled, the distortion of MIP is computed using HADAMARD 4x4

#define ENABLE_SPLIT_HEURISTICS 1

// When both are disabled we conduct the vanilla intra prediction, with predicted samples from the same frame
#define TEMPORAL_INTRA 0 // When enabled, the references for intra predictin are fetched from the previous reconstructed frame
#define TEMPORAL_PRED 0 // When enabled, the reference for intra prediction are fetched from the previous PREDICTED FRAME (WITHOUT FILTERING)
#define ORIG_SAMPLES_INTRA 0 // When enabled, the references for intra predictin are the original samples of the current frame
#define CONVOLVED_SAMPLES_INTRA 0

// Defines on what encoding stage the alternative references (temporal or orig) will be used
#define ALTERNATIVE_REF_ANGULAR 0
#define ALTERNATIVE_REF_MRL 0
#define ALTERNATIVE_REF_MIP 0


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
//         MACROS USED FOR DEBUGGING OR TRACING
//
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#define TRACE_singleSizeId 0
#define TRACE_targetSizeId 2

#define TRACE_singleCTU 0
#define TRACE_targetCtuX 128
#define TRACE_targetCtuY 128
#define TRACE_singleCuSize 0
#define TRACE_targetCuWidth 64
#define TRACE_targetCuHeight 64

#define TRACE_estIntraPredLumaQT 0      // Print the data in each call of estIntraPredLumaQT()
#define TRACE_boundaries 0      // Complete and reduced boundaries
#define TRACE_predictionProgress 0 // Reduced, horizontal and vertical upsample prediction
#define TRACE_distortion 0  // SAD and SATD distortion
#define TRACE_fineGrainedNeighborAvailability 0  // Fine-grained details of what neighboring units are available for intra references

#define TRACE_energy 0

#define EXTRACT_blockData 0             // Extract block data is NOT FUNCTIONING anymore
#define EXTRACT_frames 0                // Extract the original, true original, predicted and reconstructed frame
#define EXTRACT_distortion 0

using namespace std;


typedef enum
{
  // Pseudo 3x3
  BLUR_3x3_PseudoG_2,
  BLUR_3x3_PseudoG_3,
  BLUR_3x3_PseudoG_4,
  BLUR_3x3_PseudoG_5,
  BLUR_3x3_PseudoG_6,
  BLUR_3x3_PseudoG_7,
  BLUR_3x3_PseudoG_8,
  BLUR_3x3_PseudoG_9,
  BLUR_3x3_PseudoG_10,
  BLUR_3x3_PseudoG_11,
  BLUR_3x3_PseudoG_12,
  BLUR_3x3_PseudoG_13,
  BLUR_3x3_PseudoG_14,
                   
  /// floatGauss 3x3
  BLUR_3x3_floatG_20,
  BLUR_3x3_floatG_10,
  BLUR_3x3_floatG_095,
  BLUR_3x3_floatG_090,
  BLUR_3x3_floatG_085,
  BLUR_3x3_floatG_080,
  BLUR_3x3_floatG_075,
  BLUR_3x3_floatG_070,
  BLUR_3x3_floatG_065,
  BLUR_3x3_floatG_060,
  BLUR_3x3_floatG_055,
  BLUR_3x3_floatG_050,      
  BLUR_3x3_floatG_045,
  BLUR_3x3_floatG_040,
  BLUR_3x3_floatG_035,
  BLUR_3x3_floatG_030,           

  // intGauss 3x3
  BLUR_3x3_intG_20,
  BLUR_3x3_intG_10,
  BLUR_3x3_intG_095,
  BLUR_3x3_intG_090,
  BLUR_3x3_intG_085,
  BLUR_3x3_intG_080,
  BLUR_3x3_intG_075,
  BLUR_3x3_intG_070,
  BLUR_3x3_intG_065,
  BLUR_3x3_intG_060,
  BLUR_3x3_intG_055,
  BLUR_3x3_intG_050,      
  BLUR_3x3_intG_045,
  BLUR_3x3_intG_040,
  BLUR_3x3_intG_035,
  BLUR_3x3_intG_030,  
          
  // Emp 3x3        
  BLUR_3x3_v0,
  BLUR_3x3_v1,
  BLUR_3x3_v2,
  BLUR_3x3_v3,

  // Pseudo 5x5          
  BLUR_5x5_PseudoG_2_3,
  BLUR_5x5_PseudoG_2_4,
  BLUR_5x5_PseudoG_3_8,
          
  // Emp 5x5
  BLUR_5x5_v0,
  BLUR_5x5_v1,
  BLUR_5x5_v2,
  BLUR_5x5_v3,
  BLUR_5x5_v4
} KERNEL;

// This typedef is used to control what type of samples are being exported from the encoder
typedef enum
{
  EXT_RECONSTRUCTED,
  EXT_ORIGINAL,
  REFERENCE,
  FILTERED_REFERENCE,
  EXT_PREDICTED,
  EXT_TRUE_ORIGINAL,
  INTRA_REF_UNFILTERED,
  INTRA_REF_FILTERED,
  EXT_BEFORE_FILTER,
  EXT_CONVOLVED_KERNEL,
  EXT_NUM          
  
} SamplesType;

// This typedef is used to reference different CU sizes.
// Currently, only supports square CUs with SizeId=2
typedef enum
{
    _64x64,
    _32x32,
    _32x16,
    _16x32,
    _32x8,
    _8x32,
    _32x4,
    _4x32,
    _16x16,
    _16x8,
    _8x16,
    _16x4,
    _4x16,
    _8x8,
    _8x4,
    _4x8,
    _4x4,
    OTHERS,
    NUM_CU_SIZES
} CuSize;


class storch{
    public:
        storch();
        
        // custom parameters: These parameters are parsed by EncAppCfg and used to define what trace information is extracted and what GPU-based simplifications are conducted
        // TRACE information into the encoding report (sTRACE preffix)
        static int sTRACE_estIntraPredLumaQT; // TRACE_estIntraPredLumaQT: When ebabled: print all calls to estIntraPredLumaQT, including the block dimension, position, current split series, and encoding modes to be tested

        // TRACE information into distinct files (sEXTRACT preffix)
        static int sEXTRACT_frame; // EXTRACT_frames: Extract the original, true original, predicted and reconstructed frame
        static int sEXTRACT_mipResults; // EXTRACT_distortion: Extract the distortion for each MIP mode in each block

        // GPU Parallelization parameters. These interfere with the encoding process to expose parallelism
        static int sGPU_4x4HadMIP; // GPU_MIP: When enabled, the distortion during MIP in RMD is computed using only 4x4 hadamard. SAD is computed as usual
        static int sGPU_alternativeRefMIP; // ALTERNATIVE_REF_MIP: When enabled, the prediction using MIP during RMD uses alternative references (see sGPU_alternativeRefType)
        static int sGPU_alternativeRefAngular; // ALTERNATIVE_REF_ANGULAR: When enabled, the prediction using angular modes during RMD uses alternative references (see sGPU_alternativeRefType)
        static int sGPU_alternativeRefMRL; // ALTERNATIVE_REF_MRL: When enabled, the prediction using MRL during RMD uses alternative references (see sGPU_alternativeRefType)
        static string sGPU_alternativeRefType; // Receives a string with the type of alternative reference. Only allowed when at least one tool uses alternative references
                    // Possibilities:  ORIGINAL, PREV_REC, PREV_FILT, CONV (CONV requires an additional parameters to set the filter type)
        static string sGPU_filterType; // Receives the code for one of the predefined low-pass filters
                    // Possibilities on enum "KERNEL" and convCoeffs.h file
        
        static void finishEncoding();
        static void exportSamplesFrame(PelBuf samples, int POC, SamplesType type);
        static void exportSamplesBlock(CPelBuf samples, SamplesType type);
        static void exportSamplesBlock_v2(PelBuf samples, SamplesType type, int x, int y);
        static void exportSamplesBlock_v2(PelBuf samples, SamplesType type, int x, int y, string suffix);
        static void exportIntraReferences(Pel *buffer, CodingUnit cu, SamplesType type);
        
        
        static void checkConvKernel();
        static Pel* convolveFrameKernel(PelBuf frame, KERNEL kernelSelector);
        static PelBuf convolveFrameKernel_v2(PelBuf frame, KERNEL kernelSelector);        
        static int numExtractedBlocks;
        static int targetBlock;
        static int targetBlock_multSizes; // used to trace multiple CU sizes on the same CTU
        static int target_availability;
        static std::ofstream mip_file;
        static int traceCall;
        static int bitsMip;
                        
        // Translating CU dimensions into enum or strings
        static CuSize translateCuSize(int width, int height);
        static string translateCuSize(int cuSize);
        static int getSizeId(CuSize size);

        // Tracking intra prediction time

        static void startBitrateRmdMip();
        static void finishBitrateRmdMip();

        static void startIntraRmdPart1();
        static void finishIntraRmdPart1();

        static void startIntraRmdPart2();
        static void finishIntraRmdPart2();

        static void startIntraRmdMrl();
        static void finishIntraRmdMrl();

        static void startIntraRmdMip();
        static void finishIntraRmdMip(int width, int height);

        static void startIntraRmdGeneral();
        static void finishIntraRmdGeneral();

        static void startIntraRdoIsp();
        static void finishIntraRdoIsp();

        static void startIntraRdoGen();
        static void finishIntraRdoGen();

        static void storeRecBuf_2(Picture* pic);
        static void storePredBuf_2(Picture* pic);
        static PelStorage loadRecBuf();
        static PelStorage loadPredBuf();
        static void storeConvBuf(Picture* pic, PelBuf buffer);
        static PelStorage loadConvBuf();
        static void printRecBuf();
        
        static void incEnergy_total(double newVal);
        static void incEnergy_core(double newVal);
        static void incEnergy_pkg(double newVal);
        
        static void initializeFrameArray(Picture* pcPic);
        
        static void printKernelName(KERNEL kernelSelector);
        static string translateKernelName(KERNEL kernelSelector);

        static bool isInitialized;

        static Pel* reconstructedFrame;
        static Pel* predictedFrame;
        static int frameWidth, frameHeight;
        
        static std::unordered_map<string, KERNEL>filterNameCoeffHash;
        
    private:
        static int var;
        static int extractedFrames[EXT_NUM][500];
        
        static struct timeval rmdGen1, rmdGen2, rmdHevc1, rmdHevc2, rmdVvc1, rmdVvc2, rmdMrl1, rmdMrl2, rmdMip1, rmdMip2, rdoGen1, rdoGen2, rdoIsp1, rdoIsp2; 
        static double intraRmdGenTime, intraRmd1Time, intraRmd2Time, intraRmdMrlTime, intraRmdMipTime, intraRdoGenTime, intraRdoIspTime;
        static double intraRmdMipTime_size[NUM_CU_SIZES], intraRmdMipTime_sizeId[3];
        
        static struct timeval probeBitrateRmdMip1, probeBitrateRmdMip2;
        static double timeBitrateRmdMip;
        static double countBitrateRmdMip;
        

        
        
        static PelBuf reconstructedPrev;
        static Picture previousPic;
        static PelStorage recoBuf;
        static PelStorage predBuf;
        static PelStorage convOrig;
        
        static double totalEnergy_total, totalEnergy_core, totalEnergy_pkg;
        
        static string convVariation;
        
        
        
};



#endif /* STORCHMAIN_H */

