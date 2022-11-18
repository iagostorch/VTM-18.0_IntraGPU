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

// My directives

#define TRACE_neighborAvailability 0
#define TRACE_estIntraPredLumaQT 1      // Print the data in each call of estIntraPredLumaQT()
#define TRACE_predefinedSize 1          // Only trace a predefined size of CU
#define TRACE_predefinedWidth 64
#define TRACE_predefinedHeight 64
#define TRACE_predefinedPosition 1      // Only trace CUs in a predefined position
#define TRACE_predefinedX 128
#define TRACE_predefinedY 128
#define TRACE_innerResults 1            // Trace some inner results of the prediction, such as reduced boundaries, subsampled and complete prediction signal for MIP
#define TRACE_fineGrainedNeighborAvailability 1  // Fine-grained details of what neighboring units are available for intra references

#define EXTRACT_blockData 1
#define EXTRACT_frames 1                // Extract the original, true original, predicted and reconstructed frame

#define ENABLE_SPLIT_HEURISTICS 1

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
        static int target_availability;
        
    private:
        static int var;
        static int extractedFrames[EXT_NUM][500];
};



#endif /* STORCHMAIN_H */

