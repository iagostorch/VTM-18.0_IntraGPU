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
//#include "rapl.h"

// My directives

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
//         MACROS THAT INTERFERE ON THE ENCODING DECISIONS
//
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#define GPU_MIP 1                       // When enabled, the distortion of MIP is computed using HADAMARD 4x4

#define ENABLE_SPLIT_HEURISTICS 1

// When both are disabled we conduct the vanilla intra prediction, with predicted samples from the same frame
#define TEMPORAL_INTRA 0 // When enabled, the references for intra predictin are fetched from the previous reconstructed frame
#define TEMPORAL_PRED 0 // When enabled, the reference for intra prediction are fetched from the previous PREDICTED FRAME (WITHOUT FILTERING)
#define ORIG_SAMPLES_INTRA 0 // When enabled, the references for intra predictin are the original samples of the current frame
#define CONVOLVED_SAMPLES_INTRA 1

// Defines on what encoding stage the alternative references (temporal or orig) will be used
#define ALTERNATIVE_REF_ANGULAR 0
#define ALTERNATIVE_REF_MRL 0
#define ALTERNATIVE_REF_MIP 1


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
#define EXTRACT_frames 1                // Extract the original, true original, predicted and reconstructed frame
#define EXTRACT_distortion 1

using namespace std;

const int blur_3x3_v0[3][3] = {
  {1,1,1},
  {1,1,1},
  {1,1,1}
};

const int blur_3x3_v1[3][3] = {
  {1,1,1},
  {1,3,1},
  {1,1,1}
};

const int blur_3x3_v2[3][3] = {
  {1,1,1},
  {1,5,1},
  {1,1,1}
};
const int blur_3x3_v3[3][3] = {
  {1,1,1},
  {1,2,1},
  {1,1,1}
};
const int blur_3x3_v4[3][3] = {
  {1,2,1},
  {2,3,2},
  {1,2,1}
};
const int blur_3x3_v5[3][3] = {
  {1,2,1},
  {2,12,2},
  {1,2,1}
};
const int blur_3x3_v6[3][3] = {
  {1,1,1},
  {1,8,1},
  {1,1,1}
};

const int blur_5x5_v0[5][5] = {
  {1,1,1,1,1},
  {1,1,1,1,1},
  {1,1,1,1,1},
  {1,1,1,1,1},
  {1,1,1,1,1}
};

const int blur_5x5_v1[5][5] = {
  {1,1,1,1,1},
  {1,1,1,1,1},
  {1,1,5,1,1},
  {1,1,1,1,1},
  {1,1,1,1,1}
};

const int blur_5x5_v2[5][5] = {
  {1,1,1,1,1},
  {1,2,2,2,1},
  {1,2,3,2,1},
  {1,2,2,2,1},
  {1,1,1,1,1}
};

const int blur_5x5_v3[5][5] = {
  {1,1,1,1,1},
  {1,2,2,2,1},
  {1,2,5,2,1},
  {1,2,2,2,1},
  {1,1,1,1,1}
};

const int blur_5x5_v4[5][5] = {
  {1,1,1,1,1},
  {1,3,3,3,1},
  {1,3,5,3,1},
  {1,3,3,3,1},
  {1,1,1,1,1}
};

typedef enum
{
  BLUR_3x3_v0,
  BLUR_3x3_v1,
  BLUR_3x3_v2,
  BLUR_3x3_v3,
  BLUR_3x3_v4,
  BLUR_3x3_v5,
  BLUR_3x3_v6,        
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
        static void finishEncoding();
        static void exportSamplesFrame(PelBuf samples, int POC, SamplesType type);
        static void exportSamplesBlock(CPelBuf samples, SamplesType type);
        static void exportSamplesBlock_v2(PelBuf samples, SamplesType type, int x, int y);
        static void exportSamplesBlock_v2(PelBuf samples, SamplesType type, int x, int y, string suffix);
        static void exportIntraReferences(Pel *buffer, CodingUnit cu, SamplesType type);
        
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
        
        static bool isInitialized;

        static Pel* reconstructedFrame;
        static Pel* predictedFrame;
        static int frameWidth, frameHeight;
        
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
        
};



#endif /* STORCHMAIN_H */

