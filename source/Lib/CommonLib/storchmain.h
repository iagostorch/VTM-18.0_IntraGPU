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

#define TRACE_estIntraOredLumaQT 1
#define TRACE_neighborAvailability 1
#define TRACE_estIntraPredLumaQT 1
#define TRACE_predefinedSize 1
#define TRACE_predefinedWidth 32
#define TRACE_predefinedHeight 32
#define TRACE_predefinedPosition 1
#define TRACE_predefinedX 64
#define TRACE_predefinedY 768

#define EXTRACT_predictedBlock 1
#define EXTRACT_predictedReducedBlock 1
#define EXTRACT_frames 1

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
        
    private:
        static int var;
        static int extractedFrames[EXT_NUM][500];
};



#endif /* STORCHMAIN_H */

