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

// My directives

#define GPU_MIP 1                       // When enabled, the distortion of MIP is computed using HADAMARD 4x4

#define TRACE_neighborAvailability 0
#define TRACE_estIntraPredLumaQT 1      // Print the data in each call of estIntraPredLumaQT()
#define TRACE_multipleCusInCtu 0        // When enabled, trace multiple CU sizes inside the same CTU. The CTU position is defined by the predefined X and Y positions
//     IMPORTANT                        // When multipleCusInCtu is enabled, predefinedSize must be enabled with a wrong predefinedWidth and height
#define TRACE_predefinedSize 1          // Only trace a predefined size of CU
#define TRACE_predefinedWidth 32
#define TRACE_predefinedHeight 32
#define TRACE_predefinedPosition 1      // Only trace CUs in a predefined position
#define TRACE_predefinedX 192
#define TRACE_predefinedY 192
#define TRACE_innerResults 1            // Trace some inner results of the prediction, such as reduced boundaries, subsampled and complete prediction signal for MIP
#define TRACE_fineGrainedNeighborAvailability 0  // Fine-grained details of what neighboring units are available for intra references

#define EXTRACT_blockData 0
#define EXTRACT_frames 0                // Extract the original, true original, predicted and reconstructed frame

#define ENABLE_SPLIT_HEURISTICS 1

// When both are disabled we conduct the vanilla intra prediction, with predicted samples from the same frame
#define TEMPORAL_INTRA 0 // When enabled, the references for intra predictin are fetched from the previous reconstructed frame
#define ORIG_SAMPLES_INTRA 1 // When enabled, the references for intra predictin are the original samples of the current frame

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
        
        // Tracking intra prediction time
        static void startIntraRmdPart1();
        static void finishIntraRmdPart1();

        static void startIntraRmdPart2();
        static void finishIntraRmdPart2();

        static void startIntraRmdMrl();
        static void finishIntraRmdMrl();

        static void startIntraRmdMip();
        static void finishIntraRmdMip();

        static void startIntraRmdGeneral();
        static void finishIntraRmdGeneral();

        static void startIntraRdoIsp();
        static void finishIntraRdoIsp();

        static void startIntraRdoGen();
        static void finishIntraRdoGen();

        static void storeRecBuf(Picture* pic);
        static PelStorage loadRecBuf();
        static void printRecBuf();


        
    private:
        static int var;
        static int extractedFrames[EXT_NUM][500];
        
        static struct timeval rmdGen1, rmdGen2, rmdHevc1, rmdHevc2, rmdVvc1, rmdVvc2, rmdMrl1, rmdMrl2, rmdMip1, rmdMip2, rdoGen1, rdoGen2, rdoIsp1, rdoIsp2; 
        static double intraRmdGenTime, intraRmd1Time, intraRmd2Time, intraRmdMrlTime, intraRmdMipTime, intraRdoGenTime, intraRdoIspTime;
    
        static PelBuf reconstructedPrev;
        static Picture previousPic;
        static PelStorage recoBuf;
    
        
};



#endif /* STORCHMAIN_H */

