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

#define GPU_MIP 0                       // When enabled, the distortion of MIP is computed using HADAMARD 4x4

#define ENABLE_SPLIT_HEURISTICS 1

// When both are disabled we conduct the vanilla intra prediction, with predicted samples from the same frame
#define TEMPORAL_INTRA 0 // When enabled, the references for intra predictin are fetched from the previous reconstructed frame
#define ORIG_SAMPLES_INTRA 0 // When enabled, the references for intra predictin are the original samples of the current frame

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

#define TRACE_singleCTU 1
#define TRACE_targetCtuX 128
#define TRACE_targetCtuY 128

#define TRACE_estIntraPredLumaQT 0      // Print the data in each call of estIntraPredLumaQT()
#define TRACE_boundaries 0      // Complete and reduced boundaries
#define TRACE_predictionProgress 0 // Reduced, horizontal and vertical upsample prediction
#define TRACE_distortion 0  // SAD and SATD distortion
#define TRACE_fineGrainedNeighborAvailability 0  // Fine-grained details of what neighboring units are available for intra references

#define TRACE_energy 1

#define EXTRACT_blockData 0             // Extract block data is NOT FUNCTIONING anymore
#define EXTRACT_frames 0                // Extract the original, true original, predicted and reconstructed frame
#define EXTRACT_distortion 0

using namespace std;

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

        static void storeRecBuf(Picture* pic);
        static PelStorage loadRecBuf();
        static void printRecBuf();
        
        static void incEnergy_total(double newVal);
        static void incEnergy_core(double newVal);


        
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
        
        static double totalEnergy_total, totalEnergy_core;
        
};



#endif /* STORCHMAIN_H */

