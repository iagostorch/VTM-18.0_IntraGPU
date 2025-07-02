/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   storchmain.cpp
 * Author: iagostorch
 * 
 * Created on November 3, 2022
 */

#include "storchmain.h"
#include "Picture.h"
#include <math.h>


int storch::sTRACE_estIntraPredLumaQT;
int storch::sEXTRACT_frame;
int storch::sEXTRACT_mipResults;

int storch::sGPU_4x4HadMIP;
int storch::sGPU_alternativeRefMIP;
int storch::sGPU_alternativeRefAngular;
int storch::sGPU_alternativeRefMRL;
string storch::sGPU_alternativeRefType;
string storch::sGPU_filterType;

std::unordered_map<string, KERNEL> storch::filterNameCoeffHash;

int storch::var, storch::numExtractedBlocks;
int storch::extractedFrames[EXT_NUM][500];
int storch::targetBlock;
int storch::targetBlock_multSizes;
int storch::target_availability;
int storch::traceCall;
int storch::bitsMip;
std::ofstream storch::mip_file;


Pel* storch::reconstructedFrame;
Pel* storch::predictedFrame;
int storch::frameWidth, storch::frameHeight;
bool storch::isInitialized;

struct timeval storch::probeBitrateRmdMip1, storch::probeBitrateRmdMip2;
double storch::timeBitrateRmdMip;
double storch::countBitrateRmdMip;


struct timeval storch::rmdGen1, storch::rmdGen2, storch::rmdHevc1, storch::rmdHevc2, storch::rmdVvc1, storch::rmdVvc2, storch::rmdMrl1, storch::rmdMrl2, storch::rmdMip1, storch::rmdMip2, storch::rdoGen1, storch::rdoGen2, storch::rdoIsp1, storch::rdoIsp2; 
double storch::intraRmdGenTime, storch::intraRmd1Time, storch::intraRmd2Time, storch::intraRmdMrlTime, storch::intraRmdMipTime, storch::intraRdoGenTime, storch::intraRdoIspTime;
double storch::intraRmdMipTime_size[NUM_CU_SIZES], storch::intraRmdMipTime_sizeId[3];

double storch::totalEnergy_total, storch::totalEnergy_core, storch::totalEnergy_pkg;

string storch::convVariation;

PelBuf storch::reconstructedPrev;
Picture storch::previousPic;
PelStorage storch::recoBuf, storch::predBuf, storch::convOrig;


storch::storch() {
  for(int i=0; i<EXT_NUM; i++){
    for(int POC=0; POC<500; POC++){
        extractedFrames[EXT_ORIGINAL][POC] = 0;
    }
  } 
  var = 0;
  numExtractedBlocks = 0;
  targetBlock=0;
  targetBlock_multSizes=0;
  target_availability=0;
  traceCall = 0;
  bitsMip = 0;
  
  
  timeBitrateRmdMip = 0.0;
  countBitrateRmdMip = 0;
  
  
  intraRmdGenTime = 0.0;
  intraRmd1Time = 0.0;
  intraRmd2Time = 0.0;
  intraRmdMrlTime = 0.0;
  intraRmdMipTime = 0.0;
  intraRdoGenTime = 0.0;
  intraRdoIspTime = 0.0;
  
  intraRmdMipTime_sizeId[0] = 0.0;
  intraRmdMipTime_sizeId[1] = 0.0;
  intraRmdMipTime_sizeId[2] = 0.0;
  
  totalEnergy_total = 0.0;
  totalEnergy_core = 0.0;
   
  isInitialized = 0;
  
  for(int i=0; i<NUM_CU_SIZES; i++){
    intraRmdMipTime_size[i] = 0.0;
  }
  
  previousPic = Picture();
  recoBuf = PelStorage();
  convOrig = PelStorage();
  
  if(sEXTRACT_mipResults){     
      string mipFileName = (string) "mip_costs.csv";
      mip_file.open(mipFileName);
      
      mip_file << "POC,CTU,sizeName,W,H,X,Y,Mode,SAD_PRE_2x,SATD_GPU,SATD_ORIG,minSadHad,bitsMip" << endl;
  }
  

  filterNameCoeffHash.insert( { "BLUR_3x3_PseudoG_2", BLUR_3x3_PseudoG_2 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_PseudoG_3", BLUR_3x3_PseudoG_3 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_PseudoG_4", BLUR_3x3_PseudoG_4 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_PseudoG_5", BLUR_3x3_PseudoG_5 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_PseudoG_6", BLUR_3x3_PseudoG_6 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_PseudoG_7", BLUR_3x3_PseudoG_7 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_PseudoG_8", BLUR_3x3_PseudoG_8 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_PseudoG_9", BLUR_3x3_PseudoG_9 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_PseudoG_10", BLUR_3x3_PseudoG_10 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_PseudoG_11", BLUR_3x3_PseudoG_11 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_PseudoG_12", BLUR_3x3_PseudoG_12 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_PseudoG_13", BLUR_3x3_PseudoG_13 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_PseudoG_14", BLUR_3x3_PseudoG_14 } );

  filterNameCoeffHash.insert( { "BLUR_5x5_PseudoG_2_3", BLUR_5x5_PseudoG_2_3 } );
  filterNameCoeffHash.insert( { "BLUR_5x5_PseudoG_2_4", BLUR_5x5_PseudoG_2_4 } );
  filterNameCoeffHash.insert( { "BLUR_5x5_PseudoG_3_8", BLUR_5x5_PseudoG_3_8 } );

  
  filterNameCoeffHash.insert( { "BLUR_3x3_floatG_20", BLUR_3x3_floatG_20 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_floatG_10", BLUR_3x3_floatG_10 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_floatG_095", BLUR_3x3_floatG_095 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_floatG_090", BLUR_3x3_floatG_090 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_floatG_085", BLUR_3x3_floatG_085 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_floatG_080", BLUR_3x3_floatG_080 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_floatG_075", BLUR_3x3_floatG_075 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_floatG_070", BLUR_3x3_floatG_070 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_floatG_065", BLUR_3x3_floatG_065 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_floatG_060", BLUR_3x3_floatG_060 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_floatG_055", BLUR_3x3_floatG_055 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_floatG_050", BLUR_3x3_floatG_050 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_floatG_045", BLUR_3x3_floatG_045 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_floatG_040", BLUR_3x3_floatG_040 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_floatG_035", BLUR_3x3_floatG_035 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_floatG_030", BLUR_3x3_floatG_030 } );

  filterNameCoeffHash.insert( { "BLUR_3x3_intG_20", BLUR_3x3_intG_20 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_intG_10", BLUR_3x3_intG_10 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_intG_095", BLUR_3x3_intG_095 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_intG_090", BLUR_3x3_intG_090 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_intG_085", BLUR_3x3_intG_085 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_intG_080", BLUR_3x3_intG_080 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_intG_075", BLUR_3x3_intG_075 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_intG_070", BLUR_3x3_intG_070 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_intG_065", BLUR_3x3_intG_065 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_intG_060", BLUR_3x3_intG_060 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_intG_055", BLUR_3x3_intG_055 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_intG_050", BLUR_3x3_intG_050 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_intG_045", BLUR_3x3_intG_045 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_intG_040", BLUR_3x3_intG_040 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_intG_035", BLUR_3x3_intG_035 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_intG_030", BLUR_3x3_intG_030 } );
  
  filterNameCoeffHash.insert( { "BLUR_3x3_v0", BLUR_3x3_v0 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_v1", BLUR_3x3_v1 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_v2", BLUR_3x3_v2 } );
  filterNameCoeffHash.insert( { "BLUR_3x3_v3", BLUR_3x3_v3 } );

  filterNameCoeffHash.insert( { "BLUR_5x5_v0", BLUR_5x5_v0 } );
  filterNameCoeffHash.insert( { "BLUR_5x5_v1", BLUR_5x5_v1 } );
  filterNameCoeffHash.insert( { "BLUR_5x5_v2", BLUR_5x5_v2 } );
  filterNameCoeffHash.insert( { "BLUR_5x5_v3", BLUR_5x5_v3 } );
  filterNameCoeffHash.insert( { "BLUR_5x5_v4", BLUR_5x5_v4 } );  
  
}

void storch::finishEncoding(){  
  printf("\n\n");
  
  printf("-=-=-=-=-= CUSTOM PARAMETERS =-=-=-=-=-\n");
  cout << "sGPU_4x4HadMIP:              " << sGPU_4x4HadMIP << endl;
  cout << "sGPU_alternativeRefMIP:      " << sGPU_alternativeRefMIP << endl;
  cout << "sGPU_alternativeRefAngular:  " << sGPU_alternativeRefAngular << endl;
  cout << "sGPU_alternativeRefMRL:      " << sGPU_alternativeRefMRL << endl;
  cout << "sGPU_alternativeRefType:     " << sGPU_alternativeRefType << endl;
  cout << "sGPU_filterType:             " << sGPU_filterType << endl;
  cout << "sGPU_filterType:       (int) " <<  filterNameCoeffHash[sGPU_filterType] << endl;
  
  
  cout << "sTRACE_estIntraPredLumaQT:   " << sTRACE_estIntraPredLumaQT << endl;
  cout << "sEXTRACT_frame:              " << sEXTRACT_frame << endl;
  cout << "sEXTRACT_mipResults:         " << sEXTRACT_mipResults << endl;
  printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n\n");
  
  printf("-=-=-=-=-=  MACROS  =-=-=-=-=-  \n");
  printf("SIMD_ENABLE:              %d\n", SIMD_ENABLE);
  printf("GPU_MIP:                  %d\n", GPU_MIP);
  printf("ENABLE_SPLIT_HEURISTICS:  %d\n", ENABLE_SPLIT_HEURISTICS);
  printf("TEMPORAL_INTRA:           %d\n", TEMPORAL_INTRA);
  printf("TEMPORAL_PRED :           %d\n", TEMPORAL_PRED);
  printf("CONVOLVED_SAMPLES_INTRA:  %d\n", CONVOLVED_SAMPLES_INTRA);
cout<<   "CONVOLVED VARIATION:      "<< convVariation << endl;  
  printf("ORIG_SAMPLES_INTRA:       %d\n", ORIG_SAMPLES_INTRA);
  printf("ALTERNATIVE_REF_ANGULAR:  %d\n", ALTERNATIVE_REF_ANGULAR);
  printf("ALTERNATIVE_REF_MRL:      %d\n", ALTERNATIVE_REF_MRL);
  printf("ALTERNATIVE_REF_MIP:      %d\n", ALTERNATIVE_REF_MIP);
  printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
  
  printf("-=-=-=-=-=  ENERGY  =-=-=-=-=-  \n");
  printf("Total energy in RMD MIP:  %f\n", totalEnergy_total);
  printf("Package energy in RMD MIP:  %f\n", totalEnergy_pkg);
  printf("Core energy in RMD MIP:   %f\n\n", totalEnergy_core);
          
  printf("-=-=-=-=-=  TIMES  =-=-=-=-=-  \n");
  
  cout << "General RMD:  " << (intraRmdGenTime) << endl;
  cout << "  RMD Part 1            " << intraRmd1Time << endl;
  cout << "  RMD Part 2            " << intraRmd2Time << endl;
  cout << "  RMD MRL               " << intraRmdMrlTime << endl;
  cout << "  RMD MIP               " << intraRmdMipTime << endl;
  cout << "    NumCalls BitrateMIP " << countBitrateRmdMip << endl;
  cout << "    Time BitrateMIP     " << timeBitrateRmdMip << endl;
  cout << "  RMD Others            " << intraRmdGenTime - (intraRmd1Time+intraRmd2Time+intraRmdMrlTime+intraRmdMipTime)<< endl;
  

  cout << "General RDO:            " << intraRdoGenTime << endl;
  cout << "  RDO ISPs              " << intraRdoIspTime << endl;
  cout << "  RDO Normal            " << intraRdoGenTime - intraRdoIspTime << endl;

  cout << "Detailed MIP RMD times according to CU SIZE" << endl;
  for(int size=0; size<NUM_CU_SIZES; size++){
    cout << "  " << translateCuSize(size) << "  " << intraRmdMipTime_size[size] << endl;
  }
  cout << "Detailed MIP RMD times according to SIZE ID" << endl;
  for(int sizeId=0; sizeId<3; sizeId++){
    cout << "  SizeId" << sizeId << "  " << intraRmdMipTime_sizeId[sizeId] << endl;
  }
  
  printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
}

// Export the samples of a PU into a CSV file
void storch::exportSamplesBlock_v2(PelBuf samples, SamplesType type, int x, int y){

    int w = samples.width;
    int h = samples.height;
    
    std::ofstream fileHandler;
    string name;

    if(type == REFERENCE){
        name = (string) "block_reference.csv";
    }
    else if(type == FILTERED_REFERENCE){
        name = (string) "block_filtered.csv";
    }
    else if(type == EXT_PREDICTED){
        name = (string) "block_predicted";
	name += "_" + std::to_string(w) + "x" + std::to_string(h);
        name += "_" + std::to_string(x) + "x" + std::to_string(y);
        name += "_" + std::to_string(storch::numExtractedBlocks) + ".csv";
        storch::numExtractedBlocks++;
    }
    else if(type == EXT_ORIGINAL){
      name = (string) "block_original";
      name += "_" + std::to_string(w) + "x" + std::to_string(h);
      name += "_" + std::to_string(x) + "x" + std::to_string(y);
      name += ".csv";
    }
    else{
        printf("ERROR -- Incorrect samples type when exporting block samples");
    }

    fileHandler.open(name);

    int blockWidth = samples.width;
    int blockHeight = samples.height;

    for (h=0; h<blockHeight; h++){
        for(w=0; w<blockWidth-1; w++){
            fileHandler << samples.at(w,h) << ",";
        }
        fileHandler << samples.at(w,h);
        fileHandler << endl;
    }
    fileHandler.close();
}

// Export the samples of a PU into a CSV file
void storch::exportSamplesBlock_v2(PelBuf samples, SamplesType type, int x, int y, string suffix){
    int h,w;

    std::ofstream fileHandler;
    string name;

    if(type == REFERENCE){
        name = (string) "block_reference";
        name += "_" + std::to_string(x) + "x" + std::to_string(y);
        name += ".csv";
    }
    else if(type == FILTERED_REFERENCE){
        name = (string) "block_filtered";
        name += "_" + std::to_string(x) + "x" + std::to_string(y);
        name += ".csv";
    }
    else if(type == EXT_PREDICTED){
        name = (string) "block_predicted";
        name += "_" + std::to_string(x) + "x" + std::to_string(y);
        name += "_" + suffix;
        name += ".csv";
        storch::numExtractedBlocks++;
    }
    else if(type == EXT_ORIGINAL){
        name = (string) "block_original";
        name += "_" + std::to_string(x) + "x" + std::to_string(y);
        name += ".csv";
    }
    else{
        printf("ERROR -- Incorrect samples type when exporting block samples");
    }

    fileHandler.open(name);

    int blockWidth = samples.width;
    int blockHeight = samples.height;

    for (h=0; h<blockHeight; h++){
        for(w=0; w<blockWidth-1; w++){
            fileHandler << samples.at(w,h) << ",";
        }
        fileHandler << samples.at(w,h);
        fileHandler << endl;
    }
    fileHandler.close();
}

// Export the samples of a frame into a CSV file
void storch::exportSamplesFrame(PelBuf samples, int POC, SamplesType type){
    int h,w;
    if(type == EXT_ORIGINAL){
        if(extractedFrames[EXT_ORIGINAL][POC] == 0){ // If the original frame was not extracted yet...
//            cout << "original " << POC << endl;
            std::ofstream fileHandler;

            string name = (string) "original_" + to_string(POC);

            fileHandler.open(name + ".csv");

            int frameWidth = samples.width;
            int frameHeight = samples.height;

            for (h=0; h<frameHeight; h++){
                for(w=0; w<frameWidth-1; w++){
                    fileHandler << samples.at(w,h) << ",";
                }
                fileHandler << samples.at(w,h);
                fileHandler << endl;
            }
            fileHandler.close();
            extractedFrames[EXT_ORIGINAL][POC] = 1; // Mark the current frame as extracted
        }
        else{
            cout << "ERROR - EXTRACTING THE SAME ORIGINAL FRAME TWICE" << endl;
        }        
    }
    else if(type == EXT_TRUE_ORIGINAL){
        if(extractedFrames[EXT_TRUE_ORIGINAL][POC] == 0){ // If the true original frame was not extracted yet...
//            cout << "reconstructed " << POC << endl;
            std::ofstream fileHandler;

            string name = (string) "true_" + to_string(POC);

            fileHandler.open(name + ".csv");

            int frameWidth = samples.width;
            int frameHeight = samples.height;

            for (h=0; h<frameHeight; h++){
                for(w=0; w<frameWidth-1; w++){
                    fileHandler << samples.at(w,h) << ",";
                }
                fileHandler << samples.at(w,h);
                fileHandler << endl;
            }
            fileHandler.close();
            extractedFrames[EXT_TRUE_ORIGINAL][POC] = 1; // Mark the current frame as extracted
        }
    }
    else if(type == EXT_PREDICTED){
        if(extractedFrames[EXT_PREDICTED][POC] == 0){ // If the reconstructed frame was not extracted yet...
//            cout << "reconstructed " << POC << endl;
            std::ofstream fileHandler;

            string name = (string) "predicted_" + to_string(POC);

            fileHandler.open(name + ".csv");

            int frameWidth = samples.width;
            int frameHeight = samples.height;

            for (h=0; h<frameHeight; h++){
                for(w=0; w<frameWidth-1; w++){
                    fileHandler << samples.at(w,h) << ",";
                }
                fileHandler << samples.at(w,h);
                fileHandler << endl;
            }
            fileHandler.close();
            extractedFrames[EXT_PREDICTED][POC] = 1; // Mark the current frame as extracted
        }
    }
    else if(type == EXT_RECONSTRUCTED){
        if(extractedFrames[EXT_RECONSTRUCTED][POC] == 0){ // If the reconstructed frame was not extracted yet...
//            cout << "reconstructed " << POC << endl;
            std::ofstream fileHandler;

            string name = (string) "reconstructed_" + to_string(POC);

            fileHandler.open(name + ".csv");

            int frameWidth = samples.width;
            int frameHeight = samples.height;

            for (h=0; h<frameHeight; h++){
                for(w=0; w<frameWidth-1; w++){
                    fileHandler << samples.at(w,h) << ",";
                }
                fileHandler << samples.at(w,h);
                fileHandler << endl;
            }
            fileHandler.close();
            extractedFrames[EXT_RECONSTRUCTED][POC] = 1; // Mark the current frame as extracted
            
            
            // Export another "reconstructed" frame kept on the simple Pel* array
            
             name = (string) "reconstructedMalloc_" + to_string(POC);
             fileHandler.open(name + ".csv");
             
             for(h=0; h<storch::frameHeight; h++){
               for(w=0; w<storch::frameWidth-1; w++){
                 fileHandler << storch::reconstructedFrame[h*storch::frameWidth+w] << ",";
               }
               fileHandler << storch::reconstructedFrame[h*storch::frameWidth+w];
               fileHandler << endl;
             }
             fileHandler.close();            
            
        }
    }
    else if(type == EXT_CONVOLVED_KERNEL){
      if(extractedFrames[EXT_CONVOLVED_KERNEL][POC] == 0){ // If the reconstructed frame was not extracted yet...
//            cout << "reconstructed " << POC << endl;
            std::ofstream fileHandler;

            string name = (string) "convolved_" + to_string(POC);

            fileHandler.open(name + ".csv");

            int frameWidth = samples.width;
            int frameHeight = samples.height;

            for (h=0; h<frameHeight; h++){
                for(w=0; w<frameWidth-1; w++){
                    fileHandler << samples.at(w,h) << ",";
                }
                fileHandler << samples.at(w,h);
                fileHandler << endl;
            }
            fileHandler.close();
            extractedFrames[EXT_CONVOLVED_KERNEL][POC] = 1; // Mark the current frame as extracted
        }
    }
    else{
        cout << "ERROR - Incorrect type passes when exporting frame samples" << endl;
    }
}

// Export the samples of a PU into a CSV file
void storch::exportSamplesBlock(CPelBuf samples, SamplesType type){
    int h,w;

    std::ofstream fileHandler;
    string name;

    if(type == REFERENCE){
        name = (string) "block_reference";
    }
    else if(type == FILTERED_REFERENCE){
        name = (string) "block_filtered";
    }
    else if(type == EXT_PREDICTED){
        name = (string) "block_predicted";
    }
    else{
        printf("ERROR -- Incorrect samples type when exporting block samples");
    }

    fileHandler.open(name + ".csv");

    int blockWidth = samples.width;
    int blockHeight = samples.height;

    for (h=0; h<blockHeight; h++){
        for(w=0; w<blockWidth-1; w++){
            fileHandler << samples.at(w,h) << ",";
        }
        fileHandler << samples.at(w,h);
        fileHandler << endl;
    }
    fileHandler.close();
}

// Export the reference buffer used during intra prediction
void storch::exportIntraReferences(Pel *buffer, CodingUnit cu, SamplesType type){
  int x = cu.lx();
  int y = cu.ly();
  
  int w = cu.lwidth();
  int h = cu.lheight();
  
  std::ofstream fileHandler;
  string name;

  printf("Exporting references. MRL=%d MIP=%d ISP=%d\n", -1, cu.mipFlag, cu.ispMode);
  
  if(type == INTRA_REF_UNFILTERED){
    name = (string) "intra_ref_unfiltered";
    name += "_" + std::to_string(w) + "x" + std::to_string(h);
    name += "_" + std::to_string(x) + "x" + std::to_string(y) + ".csv";
  }
  else if(type == INTRA_REF_FILTERED){
    name = (string) "intra_ref_filtered";
    name += "_" + std::to_string(w) + "x" + std::to_string(h);
    name += "_" + std::to_string(x) + "x" + std::to_string(y) + ".csv";
  }
  else{
      printf("ERROR -- Incorrect samples type when exporting intra references buffer\n");
  }

  fileHandler.open(name);

  
  // TODO: This only works for LUMA with MRL=0
  int totalReferences = ( cu.lwidth() + cu.lheight() + 1 )*2;

  for (int r=0; r<totalReferences; r++){
    fileHandler << buffer[r] << ",";
  }
  fileHandler.close();
}

// Used to translate CU dimensions into enum or string
CuSize storch::translateCuSize(int width, int height){
    if(width==64 && height==64){
        return _64x64;
    }
    else if(width==32 && height==32){
        return _32x32;
    }
    else if(width==32 && height==16){
        return _32x16;
    }
    else if(width==16 && height==32){
        return _16x32;
    }
    else if(width==32 && height==8){
        return _32x8;
    }
    else if(width==8 && height==32){
        return _8x32;
    }
    else if(width==32 && height==4){
        return _32x4;
    }
    else if(width==4 && height==32){
        return _4x32;
    }
    else if(width==16 && height==16){
        return _16x16;
    }
    else if(width==16 && height==8){
        return _16x8;
    }
    else if(width==8 && height==16){
        return _8x16;
    }
    else if(width==16 && height==4){
        return _16x4;
    }
    else if(width==4 && height==16){
        return _4x16;
    }
    else if(width==8 && height==8){
        return _8x8;
    }
    else if(width==8 && height==4){
        return _8x4;
    }
    else if(width==4 && height==8){
        return _4x8;
    }
    else if(width==4 && height==4){
        return _4x4;
    }
    else{
        return OTHERS;
    }
}

string storch::translateCuSize(int cuSize){
    if(cuSize==_64x64){
        return "64x64";
    }
    else if(cuSize==_32x32){
        return "32x32";
    }
    else if(cuSize==_32x16){
        return "32x16";
    }
    else if(cuSize==_16x32){
        return "16x32";
    }
    else if(cuSize==_32x8){
        return "32x8";
    }
    else if(cuSize==_8x32){
        return "8x32";
    }
    else if(cuSize==_32x4){
        return "32x4";
    }
    else if(cuSize==_4x32){
        return "4x32";
    }
    else if(cuSize==_16x16){
        return "16x16";
    }
    else if(cuSize==_16x8){
        return "16x8";
    }
    else if(cuSize==_8x16){
        return "8x16";
    }
    else if(cuSize==_16x4){
        return "16x4";
    }
    else if(cuSize==_4x16){
        return "4x16";
    }
    else if(cuSize==_8x8){
        return "8x8";
    }
    else if(cuSize==_8x4){
        return "8x4";
    }
    else if(cuSize==_4x8){
        return "4x8";
    }
    else if(cuSize==_4x4){
        return "4x4";
    }
    else if(cuSize==OTHERS){
        return "OTHERS";
    }
    else{
        return "ERROR";
    }
}

int storch::getSizeId(CuSize cuSize){
  if(cuSize==_64x64){
        return 2;
    }
    else if(cuSize==_32x32){
        return 2;
    }
    else if(cuSize==_32x16){
        return 2;
    }
    else if(cuSize==_16x32){
        return 2;
    }
    else if(cuSize==_32x8){
        return 2;
    }
    else if(cuSize==_8x32){
        return 2;
    }
    else if(cuSize==_32x4){
        return 1;
    }
    else if(cuSize==_4x32){
        return 1;
    }
    else if(cuSize==_16x16){
        return 2;
    }
    else if(cuSize==_16x8){
        return 2;
    }
    else if(cuSize==_8x16){
        return 2;
    }
    else if(cuSize==_16x4){
        return 1;
    }
    else if(cuSize==_4x16){
        return 1;
    }
    else if(cuSize==_8x8){
        return 1;
    }
    else if(cuSize==_8x4){
        return 1;
    }
    else if(cuSize==_4x8){
        return 1;
    }
    else if(cuSize==_4x4){
        return 0;
    }
    else if(cuSize==OTHERS){
        return -1;
    }
    else{
        return -1;
    }
}

// Used to track the execution time

void storch::startIntraRmdPart1(){
  gettimeofday(&rmdHevc1, NULL);
}

void storch::finishIntraRmdPart1(){
  gettimeofday(&rmdHevc2, NULL);
  intraRmd1Time += (double) (rmdHevc2.tv_usec - rmdHevc1.tv_usec)/1000000 + (double) (rmdHevc2.tv_sec - rmdHevc1.tv_sec);
}

void storch::startIntraRmdPart2(){
  gettimeofday(&rmdVvc1, NULL);
}

void storch::finishIntraRmdPart2(){
  gettimeofday(&rmdVvc2, NULL);
  intraRmd2Time += (double) (rmdVvc2.tv_usec - rmdVvc1.tv_usec)/1000000 + (double) (rmdVvc2.tv_sec - rmdVvc1.tv_sec);
}

void storch::startIntraRmdMip(){
  gettimeofday(&rmdMip1, NULL);
}

void storch::finishIntraRmdMip(int width, int height){
  gettimeofday(&rmdMip2, NULL);
  intraRmdMipTime += (double) (rmdMip2.tv_usec - rmdMip1.tv_usec)/1000000 + (double) (rmdMip2.tv_sec - rmdMip1.tv_sec);
 
  CuSize cuSize = storch::translateCuSize(width, height);
  intraRmdMipTime_size[cuSize] += (double) (rmdMip2.tv_usec - rmdMip1.tv_usec)/1000000 + (double) (rmdMip2.tv_sec - rmdMip1.tv_sec);
  
  int sizeId = storch::getSizeId(cuSize);
  intraRmdMipTime_sizeId[sizeId] += (double) (rmdMip2.tv_usec - rmdMip1.tv_usec)/1000000 + (double) (rmdMip2.tv_sec - rmdMip1.tv_sec);
}

void storch::startIntraRmdGeneral(){
  gettimeofday(&rmdGen1, NULL);
}

void storch::finishIntraRmdGeneral(){
  gettimeofday(&rmdGen2, NULL);
  intraRmdGenTime += (double) (rmdGen2.tv_usec - rmdGen1.tv_usec)/1000000 + (double) (rmdGen2.tv_sec - rmdGen1.tv_sec);
}

void storch::startIntraRdoIsp(){
  gettimeofday(&rdoIsp1, NULL);
}

void storch::finishIntraRdoIsp(){
  gettimeofday(&rdoIsp2, NULL);
  intraRdoIspTime += (double) (rdoIsp2.tv_usec - rdoIsp1.tv_usec)/1000000 + (double) (rdoIsp2.tv_sec - rdoIsp1.tv_sec);
}

void storch::startIntraRdoGen(){
  gettimeofday(&rdoGen1, NULL);
}

void storch::finishIntraRdoGen(){
  gettimeofday(&rdoGen2, NULL);
  intraRdoGenTime += (double) (rdoGen2.tv_usec - rdoGen1.tv_usec)/1000000 + (double) (rdoGen2.tv_sec - rdoGen1.tv_sec);
}

void storch::startIntraRmdMrl(){
  gettimeofday(&rmdMrl1, NULL);
}

void storch::finishIntraRmdMrl(){
  gettimeofday(&rmdMrl2, NULL);
  intraRmdMrlTime += (double) (rmdMrl2.tv_usec - rmdMrl1.tv_usec)/1000000 + (double) (rmdMrl2.tv_sec - rmdMrl1.tv_sec);
}


PelStorage storch::loadRecBuf(){
  return recoBuf;
}

void storch::storeRecBuf_2(Picture* pic){

  recoBuf.createFromBuf(pic->getRecoBuf());
  recoBuf.bufs[COMPONENT_Y].buf = (Pel*) malloc(sizeof(Pel) * pic->getRecoBuf(COMPONENT_Y).height * pic->getRecoBuf(COMPONENT_Y).stride);
  
  int stride = pic->getRecoBuf(COMPONENT_Y).stride;
  for(int i=0; i<pic->getPicHeightInLumaSamples(); i++){
    for(int j=0; j<pic->getPicWidthInLumaSamples(); j++){
      recoBuf.bufs[COMPONENT_Y].buf[i*stride + j]        = pic->getRecoBuf(COMPONENT_Y).at(j,i);
      storch::reconstructedFrame[i*storch::frameWidth+j] = pic->getRecoBuf(COMPONENT_Y).at(j,i);
    }
  }  
}

void storch::storePredBuf_2(Picture* pic){

  predBuf.createFromBuf(pic->getRecoBuf());
  predBuf.bufs[COMPONENT_Y].buf = (Pel*) malloc(sizeof(Pel) * pic->getRecoBuf(COMPONENT_Y).height * pic->getRecoBuf(COMPONENT_Y).stride);
  
  
  int stride = pic->getRecoBuf(COMPONENT_Y).stride;
  for(int i=0; i<pic->getPicHeightInLumaSamples(); i++){
    for(int j=0; j<pic->getPicWidthInLumaSamples(); j++){     
      predBuf.bufs[COMPONENT_Y].buf[i*stride + j]    = pic->getRecoBuf(COMPONENT_Y).at(j,i);
      storch::predictedFrame[i*storch::frameWidth+j] = pic->getRecoBuf(COMPONENT_Y).at(j,i);
    }
  }  
}


PelStorage storch::loadPredBuf(){
  return predBuf;
}


void storch::storeConvBuf(Picture* pic, PelBuf buffer){

  if((pic->getPicWidthInLumaSamples() != buffer.width) 
  || (pic->getPicHeightInLumaSamples() != buffer.height)){
    printf("DIMENSIONS BETWEEN pic AND buffer DONT MATCH\n");
    return;
  }
  
  convOrig.createFromBuf(pic->getOrigBuf());
    
  convOrig.bufs[COMPONENT_Y].buf = (Pel*) malloc(sizeof(Pel) * pic->getOrigBuf(COMPONENT_Y).height * pic->getOrigBuf(COMPONENT_Y).stride);
  

  for(int i=0; i<pic->getPicHeightInLumaSamples(); i++){
    for(int j=0; j<pic->getPicWidthInLumaSamples(); j++){
      convOrig.bufs[COMPONENT_Y].at(j,i) =  buffer.at(j,i);

    }
  }   
}

PelStorage storch::loadConvBuf(){
  return convOrig;
}


void storch::printRecBuf(){
  printf("CHAMOU PRINT REC_BUFFER\n");
    Pel* pixels = recoBuf.get(COMPONENT_Y).buf;
    int strid = recoBuf.get(COMPONENT_Y).stride;
    printf("---------------------------\n");
    for(int i=0; i<480; i++){
      for(int j=0; j<832; j++){
        printf("%d,", pixels[i*strid + j] );
      }
      printf("\n");
    }
    printf("---------------------------\n");  
}

void storch::startBitrateRmdMip(){
  gettimeofday(&probeBitrateRmdMip1 , NULL);
}

void storch::finishBitrateRmdMip(){
  gettimeofday(&probeBitrateRmdMip2, NULL);
  timeBitrateRmdMip += (double) (probeBitrateRmdMip2.tv_usec - probeBitrateRmdMip1.tv_usec)/1000000 + (double) (probeBitrateRmdMip2.tv_sec - probeBitrateRmdMip1.tv_sec);
  countBitrateRmdMip += 1;
}


void storch::incEnergy_total(double newVal){
  totalEnergy_total += newVal;
}

void storch::incEnergy_core(double newVal){
  totalEnergy_core += newVal;
}

void storch::incEnergy_pkg(double newVal){
  totalEnergy_pkg += newVal;
}


PelBuf storch::convolveFrameKernel_v2(PelBuf frame, KERNEL kernelSelector){
  Pel *buf = (Pel*) malloc(sizeof(Pel) * frame.width * frame.height);
  
  AreaBuf<Pel> convolved(buf, frame.width, frame.width, frame.height);

  
   
  float temp=0;
    
   // BLUR_3x3
   if(kernelSelector <= BLUR_3x3_v3)
   {
     
    double blur_3x3[3][3];
    
    storch::printKernelName(kernelSelector);

    convVariation = storch::translateKernelName(kernelSelector);

   
    // Pseudo GAUSS 3x3
    if(kernelSelector <= BLUR_3x3_PseudoG_14){
      for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
          blur_3x3[i][j] = blur_3x3_pseudo_gauss[kernelSelector][i][j];
        }
      }
    }
    // Float GAUSS
    else if(kernelSelector <= BLUR_3x3_floatG_030){
      for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            blur_3x3[i][j] = blur_3x3_gauss[kernelSelector-BLUR_3x3_floatG_20][i][j];
        }
      }
    }
    // Int GAUSS
    else if(kernelSelector <= BLUR_3x3_intG_030){
      for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
          blur_3x3[i][j] = blur_3x3_gauss_int[kernelSelector-BLUR_3x3_intG_20][i][j];
        }
      }
    }
    // Empirical 3x3
    else if(kernelSelector <= BLUR_3x3_v3){
      for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
          blur_3x3[i][j] = blur_3x3_emp[kernelSelector-BLUR_3x3_v0][i][j];
        }
      }
    }
    
        
    // Compute divider for the filter
    float div = 0.0, div_corner=0.0, div_firstLast = 0.0;
    
    // Easy case
    for(int y=0; y<3; y++){
       for(int x=0; x<3; x++){
         div += blur_3x3[y][x];
       }
    }
    
    // Corner
    for(int y=1; y<3; y++){
      for(int x=1; x<3; x++){
        div_corner += blur_3x3[y][x];
      }
    }
    
    // First/Last rows/columns without corners
    for(int y=1; y<3; y++){
      for(int x=0; x<3; x++){
        div_firstLast += blur_3x3[y][x];
      }
    }
    
    
     // Easy case without borders
     for(int y=1; y<frame.height-1; y++){
       for(int x=1; x<frame.width-1; x++){
         temp = 0;
         
         temp += blur_3x3[0][0]*frame.at(x-1,y-1);        
         temp += blur_3x3[0][1]*frame.at(x, y-1);
         temp += blur_3x3[0][2]*frame.at(x+1,y-1);
         
         temp += blur_3x3[1][0]*frame.at(x-1,y);
         temp += blur_3x3[1][1]*frame.at(x,y);
         temp += blur_3x3[1][2]*frame.at(x+1,y);
         
         temp += blur_3x3[2][0]*frame.at(x-1,y+1);
         temp += blur_3x3[2][1]*frame.at(x,y+1);
         temp += blur_3x3[2][2]*frame.at(x+1,y+1);
         
//         printf("(y,x)=%d | div=%f | ==%f\n", temp, div, temp/div);
         
         temp = round(temp/div);
//         printf("(y,x)=%d\n", temp);
         convolved.at(x,y) = temp;
       }
     }
     
     // First and last rows without corners
     for(int x=1; x<frame.width-1; x++){
         int y;
         
         y = 0;
         temp = 0;
         temp += blur_3x3[1][0]*frame.at(x-1,y);
         temp += blur_3x3[1][1]*frame.at(x,y);
         temp += blur_3x3[1][2]*frame.at(x+1,y);
         
         temp += blur_3x3[2][0]*frame.at(x-1,y+1);
         temp += blur_3x3[2][1]*frame.at(x,y+1);
         temp += blur_3x3[2][2]*frame.at(x+1,y+1);
         
         temp = round(temp/div_firstLast);
         convolved.at(x,y) = temp;
         
         y = frame.height-1;
         

	 temp = 0;
         temp += blur_3x3[0][0]*frame.at(x-1,y-1);
         temp += blur_3x3[0][1]*frame.at(x,y-1);
         temp += blur_3x3[0][2]*frame.at(x+1,y-1);
         
         temp += blur_3x3[1][0]*frame.at(x-1,y);
         temp += blur_3x3[1][1]*frame.at(x,y);
         temp += blur_3x3[1][2]*frame.at(x+1,y);
         
         temp = round(temp/div_firstLast);
         convolved.at(x,y) = temp;
     }
     
     
     for(int y=1; y<frame.height-1; y++){
        int x;
       
        x = 0;
       
        temp = 0;
        temp += blur_3x3[0][1]*frame.at(x, y-1);
        temp += blur_3x3[0][2]*frame.at(x+1,y-1);

        temp += blur_3x3[1][1]*frame.at(x,y);
        temp += blur_3x3[1][2]*frame.at(x+1,y);

        temp += blur_3x3[2][1]*frame.at(x,y+1);
        temp += blur_3x3[2][2]*frame.at(x+1,y+1);   
        
        temp = round(temp/div_firstLast);
        convolved.at(x,y) = temp;
        
        x = frame.width-1;
  

	temp = 0;
        temp += blur_3x3[0][0]*frame.at(x-1, y-1);
        temp += blur_3x3[0][1]*frame.at(x  , y-1);

        temp += blur_3x3[1][0]*frame.at(x-1,y);
        temp += blur_3x3[1][1]*frame.at(x,y);

        temp += blur_3x3[2][0]*frame.at(x-1,y+1);
        temp += blur_3x3[2][1]*frame.at(x,y+1);   
        
        temp = round(temp/div_firstLast);
        convolved.at(x,y) = temp;
     }
   
     
     // Corner cases
     int x, y;
     // Top-Left
     x = 0;
     y = 0;
     temp = blur_3x3[1][1]*frame.at(x,y) +  blur_3x3[1][2]*frame.at(x+1,y) + blur_3x3[2][1]*frame.at(x,y+1) + blur_3x3[2][2]*frame.at(x+1,y+1);
     temp = round(temp/div_corner);
     convolved.at(x,y) = temp;
     // Top-Right
     x = frame.width-1;
     y = 0;
     temp = blur_3x3[1][0]*frame.at(x-1,y) + blur_3x3[1][1]*frame.at(x,y) + blur_3x3[2][0]*frame.at(x-1,y+1) + blur_3x3[2][1]*frame.at(x,y+1);
     temp = round(temp/div_corner);
     convolved.at(x,y) = temp;
     //Bottom-Left
     x = 0;
     y = frame.height-1;
     temp = blur_3x3[0][1]*frame.at(x, y-1) + blur_3x3[0][2]*frame.at(x+1,y-1) + blur_3x3[1][1]*frame.at(x,y) + blur_3x3[1][2]*frame.at(x+1,y);
     temp = round(temp/div_corner);
     convolved.at(x,y) = temp;
     // Bottom-Right
     x = frame.width-1;
     y = frame.height-1;
     temp = blur_3x3[0][0]*frame.at(x-1,y-1) + blur_3x3[0][1]*frame.at(x, y-1) + blur_3x3[1][0]*frame.at(x-1,y) + blur_3x3[1][1]*frame.at(x,y);
     temp = round(temp/div_corner);
     convolved.at(x,y) = temp;
         
   }
  
   else if(kernelSelector <= BLUR_5x5_v4)
   {
      int blur_5x5[5][5];
    
      // Pseudo Gauss 5x5
      if(kernelSelector <= BLUR_5x5_PseudoG_3_8){
        for(int i=0; i<5; i++){
          for(int j=0; j<5; j++){
            blur_5x5[i][j] = blur_5x5_pseudo_gauss[kernelSelector - BLUR_5x5_PseudoG_2_3][i][j];
          }
        }
      }
      
      else if(kernelSelector <= BLUR_5x5_v4){
        for(int i=0; i<5; i++){
          for(int j=0; j<5; j++){
            blur_5x5[i][j] = blur_5x5_emp[kernelSelector - BLUR_5x5_v0][i][j];
          }
        }
      }
      
      
      // Compute divider for the filter
      float div=0.0, div_outerCorner=0.0, div_innerCorner=0.0, div_firstLast=0.0, div_befFirstLast=0.0;

      // Easy case
      for(int y=0; y<5; y++){
	 for(int x=0; x<5; x++){
	   div += blur_5x5[y][x];
	 }
      }

      // Inner-Corner
      for(int y=1; y<5; y++){
	for(int x=1; x<5; x++){
	  div_innerCorner += blur_5x5[y][x];
	}
      }
      
      // Outer-Corner
      for(int y=2; y<5; y++){
	for(int x=2; x<5; x++){
	  div_outerCorner += blur_5x5[y][x];
	}
      }

      // Before First/Last rows/columns without corners
      for(int y=1; y<5; y++){
	for(int x=0; x<5; x++){
	  div_befFirstLast += blur_5x5[y][x];
	}
      }
      
      // Actual First/Last rows/columns without corners
      for(int y=2; y<5; y++){
	for(int x=0; x<5; x++){
	  div_firstLast += blur_5x5[y][x];
	}
      }


//       printf("CONVOLUTION\n");
       // Easy case without borders
       for(int y=2; y<frame.height-2; y++){
	 for(int x=2; x<frame.width-2; x++){
	   temp = 0;

	   temp += blur_5x5[0][0]*frame.at(x-2,y-2);
	   temp += blur_5x5[0][1]*frame.at(x-1,y-2);
	   temp += blur_5x5[0][2]*frame.at(x,y-2);
	   temp += blur_5x5[0][3]*frame.at(x+1,y-2);
	   temp += blur_5x5[0][4]*frame.at(x+2,y-2);
	   
	   temp += blur_5x5[1][0]*frame.at(x-2,y-1);
	   temp += blur_5x5[1][1]*frame.at(x-1,y-1);
	   temp += blur_5x5[1][2]*frame.at(x,y-1);
	   temp += blur_5x5[1][3]*frame.at(x+1,y-1);
	   temp += blur_5x5[1][4]*frame.at(x+2,y-1);
	   
	   temp += blur_5x5[2][0]*frame.at(x-2,y);
	   temp += blur_5x5[2][1]*frame.at(x-1,y);
	   temp += blur_5x5[2][2]*frame.at(x,y);
	   temp += blur_5x5[2][3]*frame.at(x+1,y);
	   temp += blur_5x5[2][4]*frame.at(x+2,y);
	   
	   temp += blur_5x5[3][0]*frame.at(x-2,y+1);
	   temp += blur_5x5[3][1]*frame.at(x-1,y+1);
	   temp += blur_5x5[3][2]*frame.at(x,y+1);
	   temp += blur_5x5[3][3]*frame.at(x+1,y+1);
	   temp += blur_5x5[3][4]*frame.at(x+2,y+1);
	   
	   temp += blur_5x5[4][0]*frame.at(x-2,y+2);
	   temp += blur_5x5[4][1]*frame.at(x-1,y+2);
	   temp += blur_5x5[4][2]*frame.at(x,y+2);
	   temp += blur_5x5[4][3]*frame.at(x+1,y+2);
	   temp += blur_5x5[4][4]*frame.at(x+2,y+2);	   
	   
  //         printf("(y,x)=%d | div=%f | ==%f\n", temp, div, temp/div);

	   temp = round(temp/div);
  //         printf("(y,x)=%d\n", temp);
	   convolved.at(x,y) = temp;
	 }
       }

       // Border rows
       // 1st and 2nd, last and befLast rows without corners
       for(int x=2; x<frame.width-2; x++){
	   int y;

	   y = 0;
	   temp = 0;
	   
	   temp += blur_5x5[2][0]*frame.at(x-2,y);
	   temp += blur_5x5[2][1]*frame.at(x-1,y);
	   temp += blur_5x5[2][2]*frame.at(x  ,y);
	   temp += blur_5x5[2][3]*frame.at(x+1,y);
	   temp += blur_5x5[2][4]*frame.at(x+2,y);
	   
	   temp += blur_5x5[3][0]*frame.at(x-2,y+1);
	   temp += blur_5x5[3][1]*frame.at(x-1,y+1);
	   temp += blur_5x5[3][2]*frame.at(x  ,y+1);
	   temp += blur_5x5[3][3]*frame.at(x+1,y+1);
	   temp += blur_5x5[3][4]*frame.at(x+2,y+1);
	   
	   temp += blur_5x5[4][0]*frame.at(x-2,y+2);
	   temp += blur_5x5[4][1]*frame.at(x-1,y+2);
	   temp += blur_5x5[4][2]*frame.at(x  ,y+2);
	   temp += blur_5x5[4][3]*frame.at(x+1,y+2);
	   temp += blur_5x5[4][4]*frame.at(x+2,y+2);

	   temp = round(temp/div_firstLast);
	   convolved.at(x,y) = temp;
	   
	   y = 1;
	   temp = 0;
	   
	   temp += blur_5x5[1][0]*frame.at(x-2,y-1);
	   temp += blur_5x5[1][1]*frame.at(x-1,y-1);
	   temp += blur_5x5[1][2]*frame.at(x  ,y-1);
	   temp += blur_5x5[1][3]*frame.at(x+1,y-1);
	   temp += blur_5x5[1][4]*frame.at(x+2,y-1);
	   
	   temp += blur_5x5[2][0]*frame.at(x-2,y);
	   temp += blur_5x5[2][1]*frame.at(x-1,y);
	   temp += blur_5x5[2][2]*frame.at(x  ,y);
	   temp += blur_5x5[2][3]*frame.at(x+1,y);
	   temp += blur_5x5[2][4]*frame.at(x+2,y);
	   
	   temp += blur_5x5[3][0]*frame.at(x-2,y+1);
	   temp += blur_5x5[3][1]*frame.at(x-1,y+1);
	   temp += blur_5x5[3][2]*frame.at(x  ,y+1);
	   temp += blur_5x5[3][3]*frame.at(x+1,y+1);
	   temp += blur_5x5[3][4]*frame.at(x+2,y+1);
	   
	   temp += blur_5x5[4][0]*frame.at(x-2,y+2);
	   temp += blur_5x5[4][1]*frame.at(x-1,y+2);
	   temp += blur_5x5[4][2]*frame.at(x  ,y+2);
	   temp += blur_5x5[4][3]*frame.at(x+1,y+2);
	   temp += blur_5x5[4][4]*frame.at(x+2,y+2);

	   temp = round(temp/div_befFirstLast);
	   convolved.at(x,y) = temp;

	   y = frame.height-2;

	   temp = 0;
	   
	   temp += blur_5x5[0][0]*frame.at(x-2,y-2);
	   temp += blur_5x5[0][1]*frame.at(x-1,y-2);
	   temp += blur_5x5[0][2]*frame.at(x  ,y-2);
	   temp += blur_5x5[0][3]*frame.at(x+1,y-2);
	   temp += blur_5x5[0][4]*frame.at(x+2,y-2);
	   
	   temp += blur_5x5[1][0]*frame.at(x-2,y-1);
	   temp += blur_5x5[1][1]*frame.at(x-1,y-1);
	   temp += blur_5x5[1][2]*frame.at(x  ,y-1);
	   temp += blur_5x5[1][3]*frame.at(x+1,y-1);
	   temp += blur_5x5[1][4]*frame.at(x+2,y-1);
	   
	   temp += blur_5x5[2][0]*frame.at(x-2,y);
	   temp += blur_5x5[2][1]*frame.at(x-1,y);
	   temp += blur_5x5[2][2]*frame.at(x  ,y);
	   temp += blur_5x5[2][3]*frame.at(x+1,y);
	   temp += blur_5x5[2][4]*frame.at(x+2,y);
	   
	   temp += blur_5x5[3][0]*frame.at(x-2,y+1);
	   temp += blur_5x5[3][1]*frame.at(x-1,y+1);
	   temp += blur_5x5[3][2]*frame.at(x  ,y+1);
	   temp += blur_5x5[3][3]*frame.at(x+1,y+1);
	   temp += blur_5x5[3][4]*frame.at(x+2,y+1);  

	   temp = round(temp/div_befFirstLast);
	   convolved.at(x,y) = temp;
	   
	   y = frame.height-1;

	   temp = 0;
	   
	   temp += blur_5x5[0][0]*frame.at(x-2,y-2);
	   temp += blur_5x5[0][1]*frame.at(x-1,y-2);
	   temp += blur_5x5[0][2]*frame.at(x  ,y-2);
	   temp += blur_5x5[0][3]*frame.at(x+1,y-2);
	   temp += blur_5x5[0][4]*frame.at(x+2,y-2);
	   
	   temp += blur_5x5[1][0]*frame.at(x-2,y-1);
	   temp += blur_5x5[1][1]*frame.at(x-1,y-1);
	   temp += blur_5x5[1][2]*frame.at(x  ,y-1);
	   temp += blur_5x5[1][3]*frame.at(x+1,y-1);
	   temp += blur_5x5[1][4]*frame.at(x+2,y-1);
	   
	   temp += blur_5x5[2][0]*frame.at(x-2,y);
	   temp += blur_5x5[2][1]*frame.at(x-1,y);
	   temp += blur_5x5[2][2]*frame.at(x  ,y);
	   temp += blur_5x5[2][3]*frame.at(x+1,y);
	   temp += blur_5x5[2][4]*frame.at(x+2,y);
	   
	   temp = round(temp/div_firstLast);
	   convolved.at(x,y) = temp;
       }
       
       
       // Border columns
       // 1st and 2nd, last and befLast columns without corners
       for(int y=2; y<frame.height-2; y++){
	   int x;

	   x = 0;
	   temp = 0;
	   
	   temp += blur_5x5[0][2]*frame.at(x  ,y-2);
	   temp += blur_5x5[0][3]*frame.at(x+1,y-2);
	   temp += blur_5x5[0][4]*frame.at(x+2,y-2);	   
	   
	   temp += blur_5x5[1][2]*frame.at(x  ,y-1);
	   temp += blur_5x5[1][3]*frame.at(x+1,y-1);
	   temp += blur_5x5[1][4]*frame.at(x+2,y-1);	   

	   temp += blur_5x5[2][2]*frame.at(x  ,y);
	   temp += blur_5x5[2][3]*frame.at(x+1,y);
	   temp += blur_5x5[2][4]*frame.at(x+2,y);	   	   
	   
	   temp += blur_5x5[3][2]*frame.at(x  ,y+1);
	   temp += blur_5x5[3][3]*frame.at(x+1,y+1);
	   temp += blur_5x5[3][4]*frame.at(x+2,y+1);	   	   
	   
	   temp += blur_5x5[4][2]*frame.at(x  ,y+2);
	   temp += blur_5x5[4][3]*frame.at(x+1,y+2);
	   temp += blur_5x5[4][4]*frame.at(x+2,y+2);	 
	   
	   temp = round(temp/div_firstLast);
	   convolved.at(x,y) = temp;
	   
	   x = 1;
	   temp = 0;
	   
	   temp += blur_5x5[0][1]*frame.at(x-1,y-2);
	   temp += blur_5x5[0][2]*frame.at(x  ,y-2);
	   temp += blur_5x5[0][3]*frame.at(x+1,y-2);
	   temp += blur_5x5[0][4]*frame.at(x+2,y-2);	   
	   
	   temp += blur_5x5[1][1]*frame.at(x-1,y-1);
	   temp += blur_5x5[1][2]*frame.at(x  ,y-1);
	   temp += blur_5x5[1][3]*frame.at(x+1,y-1);
	   temp += blur_5x5[1][4]*frame.at(x+2,y-1);	   

	   temp += blur_5x5[2][1]*frame.at(x-1,y);
	   temp += blur_5x5[2][2]*frame.at(x  ,y);
	   temp += blur_5x5[2][3]*frame.at(x+1,y);
	   temp += blur_5x5[2][4]*frame.at(x+2,y);	   	   
	   
	   temp += blur_5x5[3][1]*frame.at(x-1,y+1);
	   temp += blur_5x5[3][2]*frame.at(x  ,y+1);
	   temp += blur_5x5[3][3]*frame.at(x+1,y+1);
	   temp += blur_5x5[3][4]*frame.at(x+2,y+1);	   	   
	   
	   temp += blur_5x5[4][1]*frame.at(x-1,y+2);
	   temp += blur_5x5[4][2]*frame.at(x  ,y+2);
	   temp += blur_5x5[4][3]*frame.at(x+1,y+2);
	   temp += blur_5x5[4][4]*frame.at(x+2,y+2);	 
	   
	   temp = round(temp/div_befFirstLast);
	   convolved.at(x,y) = temp;

	   x = frame.width-2;
	   temp = 0;
	   
	   temp += blur_5x5[0][0]*frame.at(x-2,y-2);	   
	   temp += blur_5x5[0][1]*frame.at(x-1,y-2);
	   temp += blur_5x5[0][2]*frame.at(x  ,y-2);
	   temp += blur_5x5[0][3]*frame.at(x+1,y-2);
	   
	   temp += blur_5x5[1][0]*frame.at(x-2,y-1);	   
	   temp += blur_5x5[1][1]*frame.at(x-1,y-1);
	   temp += blur_5x5[1][2]*frame.at(x  ,y-1);
	   temp += blur_5x5[1][3]*frame.at(x+1,y-1);
	   
	   temp += blur_5x5[2][0]*frame.at(x-2,y);	   
	   temp += blur_5x5[2][1]*frame.at(x-1,y);
	   temp += blur_5x5[2][2]*frame.at(x  ,y);
	   temp += blur_5x5[2][3]*frame.at(x+1,y);
	   
	   temp += blur_5x5[3][0]*frame.at(x-2,y+1);	   
	   temp += blur_5x5[3][1]*frame.at(x-1,y+1);
	   temp += blur_5x5[3][2]*frame.at(x  ,y+1);
	   temp += blur_5x5[3][3]*frame.at(x+1,y+1);
	   
	   temp += blur_5x5[4][0]*frame.at(x-2,y+2);	   
	   temp += blur_5x5[4][1]*frame.at(x-1,y+2);
	   temp += blur_5x5[4][2]*frame.at(x  ,y+2);
	   temp += blur_5x5[4][3]*frame.at(x+1,y+2);
	   
	   temp = round(temp/div_befFirstLast);
	   convolved.at(x,y) = temp;
	   
	   x = frame.width-1;
	   temp = 0;
	   
	   temp += blur_5x5[0][0]*frame.at(x-2,y-2);	   
	   temp += blur_5x5[0][1]*frame.at(x-1,y-2);
	   temp += blur_5x5[0][2]*frame.at(x  ,y-2);
	   
	   temp += blur_5x5[1][0]*frame.at(x-2,y-1);	   
	   temp += blur_5x5[1][1]*frame.at(x-1,y-1);
	   temp += blur_5x5[1][2]*frame.at(x  ,y-1);
	   
	   temp += blur_5x5[2][0]*frame.at(x-2,y);	   
	   temp += blur_5x5[2][1]*frame.at(x-1,y);
	   temp += blur_5x5[2][2]*frame.at(x  ,y);
	   
	   temp += blur_5x5[3][0]*frame.at(x-2,y+1);	   
	   temp += blur_5x5[3][1]*frame.at(x-1,y+1);
	   temp += blur_5x5[3][2]*frame.at(x  ,y+1);
	   
	   temp += blur_5x5[4][0]*frame.at(x-2,y+2);	   
	   temp += blur_5x5[4][1]*frame.at(x-1,y+2);
	   temp += blur_5x5[4][2]*frame.at(x  ,y+2);
	   
	   temp = round(temp/div_firstLast);
	   convolved.at(x,y) = temp;
	   	   
       }
       
       // Corners
       int x, y;
       float currDiv;
       
       
    int kx0, kx1, ky0, ky1;

    // (0,0) Top-Left
    x = 0;
    y = 0;
    kx0 = 0;
    kx1 = 2;
    ky0 = 0;
    ky1 = 2;
    
    currDiv= 0.0;
    temp = 0;
    for(int i=ky0; i<=ky1; i++){
      for(int j=kx0; j<=kx1; j++){
	currDiv += blur_5x5[2+i][2+j];
	temp += blur_5x5[2+i][2+j]*frame.at(x+j, y+i);
      }
    }
    temp = round(temp/currDiv);
    convolved.at(x,y) = temp;
    
    
    // (0,1) Top-Left+1
    x = 1;
    y = 0;
    kx0 = -1;
    kx1 = 2;
    ky0 = 0;
    ky1 = 2;
    
    currDiv= 0;
    temp = 0;
    for(int i=ky0; i<=ky1; i++){
      for(int j=kx0; j<=kx1; j++){
	currDiv += blur_5x5[2+i][2+j];
	temp += blur_5x5[2+i][2+j]*frame.at(x+j, y+i);
      }
    }
    temp = round(temp/currDiv);
    convolved.at(x,y) = temp;    
    
    // (0,w-1) Top-Right-1
    x = frame.width-2;
    y = 0;
    kx0 = -2;
    kx1 = 1;
    ky0 = 0;
    ky1 = 2;
    
    currDiv= 0;
    temp = 0;
    for(int i=ky0; i<=ky1; i++){
      for(int j=kx0; j<=kx1; j++){
	currDiv += blur_5x5[2+i][2+j];
	temp += blur_5x5[2+i][2+j]*frame.at(x+j, y+i);
      }
    }
    temp = round(temp/currDiv);
    convolved.at(x,y) = temp;
    
    
    // (0,w) Top-Right
    x = frame.width-1;
    y = 0;
    kx0 = -2;
    kx1 = 0;
    ky0 = 0;
    ky1 = 2;
    
    currDiv= 0;
    temp = 0;
    for(int i=ky0; i<=ky1; i++){
      for(int j=kx0; j<=kx1; j++){
	currDiv += blur_5x5[2+i][2+j];
	temp += blur_5x5[2+i][2+j]*frame.at(x+j, y+i);
      }
    }
    temp = round(temp/currDiv);
    convolved.at(x,y) = temp;
    
    // (1,0) Top+1-Left
    x = 0;
    y = 1;
    kx0 = 0;
    kx1 = 2;
    ky0 = -1;
    ky1 = 2;
    
    currDiv= 0;
    temp = 0;
    for(int i=ky0; i<=ky1; i++){
      for(int j=kx0; j<=kx1; j++){
	currDiv += blur_5x5[2+i][2+j];
	temp += blur_5x5[2+i][2+j]*frame.at(x+j, y+i);
      }
    }
    temp = round(temp/currDiv);
    convolved.at(x,y) = temp;
    
    // (1,1) Top+1-Left+1
    x = 1;
    y = 1;
    kx0 = -1;
    kx1 = 2;
    ky0 = -1;
    ky1 = 2;
    
    currDiv= 0;
    temp = 0;
    for(int i=ky0; i<=ky1; i++){
      for(int j=kx0; j<=kx1; j++){
	currDiv += blur_5x5[2+i][2+j];
	temp += blur_5x5[2+i][2+j]*frame.at(x+j, y+i);
      }
    }
    temp = round(temp/currDiv);
    convolved.at(x,y) = temp;   
    
    // (1,w-1) Top+1-Right-1
    x = frame.width-2;
    y = 1;
    kx0 = -2;
    kx1 = 1;
    ky0 = -1;
    ky1 = 2;
    
    currDiv= 0;
    temp = 0;
    for(int i=ky0; i<=ky1; i++){
      for(int j=kx0; j<=kx1; j++){
	currDiv += blur_5x5[2+i][2+j];
	temp += blur_5x5[2+i][2+j]*frame.at(x+j, y+i);
      }
    }
    temp = round(temp/currDiv);
    convolved.at(x,y) = temp;  
    
    // (1,w) Top+1-Right
    x = frame.width-1;
    y = 1;
    kx0 = -2;
    kx1 = 0;
    ky0 = -1;
    ky1 = 2;
    
    currDiv= 0;
    temp = 0;
    for(int i=ky0; i<=ky1; i++){
      for(int j=kx0; j<=kx1; j++){
	currDiv += blur_5x5[2+i][2+j];
	temp += blur_5x5[2+i][2+j]*frame.at(x+j, y+i);
      }
    }
    temp = round(temp/currDiv);
    convolved.at(x,y) = temp;
    
    // (h-1,0) Bottom-1-Left
    x = 0;
    y = frame.height-2;
    kx0 = 0;
    kx1 = 2;
    ky0 = -2;
    ky1 = 1;
    
    currDiv= 0;
    temp = 0;
    for(int i=ky0; i<=ky1; i++){
      for(int j=kx0; j<=kx1; j++){
	currDiv += blur_5x5[2+i][2+j];
	temp += blur_5x5[2+i][2+j]*frame.at(x+j, y+i);
      }
    }
    temp = round(temp/currDiv);
    convolved.at(x,y) = temp;
    
    // (h-1,1) Bottom-1-Left+1
    x = 1;
    y = frame.height-2;
    kx0 = -1;
    kx1 = 2;
    ky0 = -2;
    ky1 = 1;
    
    currDiv= 0;
    temp = 0;
    for(int i=ky0; i<=ky1; i++){
      for(int j=kx0; j<=kx1; j++){
	currDiv += blur_5x5[2+i][2+j];
	temp += blur_5x5[2+i][2+j]*frame.at(x+j, y+i);
      }
    }
    temp = round(temp/currDiv);
    convolved.at(x,y) = temp;
    

    // (h-1,w-1) Bottom-1-Right-1
    x = frame.width-2;
    y = frame.height-2;
    kx0 = -2;
    kx1 = 1;
    ky0 = -2;
    ky1 = 1;
    
    currDiv= 0;
    temp = 0;
    for(int i=ky0; i<=ky1; i++){
      for(int j=kx0; j<=kx1; j++){
	currDiv += blur_5x5[2+i][2+j];
	temp += blur_5x5[2+i][2+j]*frame.at(x+j, y+i);
      }
    }
    temp = round(temp/currDiv);
    convolved.at(x,y) = temp;    
    
    
    // (h-1,w) Bottom-1-Right
    x = frame.width-1;
    y = frame.height-2;
    kx0 = -2;
    kx1 = 0;
    ky0 = -2;
    ky1 = 1;
    
    currDiv= 0;
    temp = 0;
    for(int i=ky0; i<=ky1; i++){
      for(int j=kx0; j<=kx1; j++){
	currDiv += blur_5x5[2+i][2+j];
	temp += blur_5x5[2+i][2+j]*frame.at(x+j, y+i);
      }
    }
    temp = round(temp/currDiv);
    convolved.at(x,y) = temp;

    // (h,0) Bottom-Left
    x = 0;
    y = frame.height - 1;
    kx0 = 0;
    kx1 = 2;
    ky0 = -2;
    ky1 = 0;
    
    currDiv= 0;
    temp = 0;
    for(int i=ky0; i<=ky1; i++){
      for(int j=kx0; j<=kx1; j++){
	currDiv += blur_5x5[2+i][2+j];
	temp += blur_5x5[2+i][2+j]*frame.at(x+j, y+i);
      }
    }
    temp = round(temp/currDiv);
    convolved.at(x,y) = temp;      
    
    // (h,1) Bottom-Left+1
    x = 1;
    y = frame.height-1;
    kx0 = -1;
    kx1 = 2;
    ky0 = -2;
    ky1 = 0;
    
    currDiv= 0;
    temp = 0;
    for(int i=ky0; i<=ky1; i++){
      for(int j=kx0; j<=kx1; j++){
	currDiv += blur_5x5[2+i][2+j];
	temp += blur_5x5[2+i][2+j]*frame.at(x+j, y+i);
      }
    }
    temp = round(temp/currDiv);
    convolved.at(x,y) = temp;    
    
    // (h,w-1) Bottom-Right-1
    x = frame.width - 2;
    y = frame.height - 1;
    kx0 = -2;
    kx1 = 1;
    ky0 = -2;
    ky1 = 0;
    
    currDiv= 0;
    temp = 0;
    for(int i=ky0; i<=ky1; i++){
      for(int j=kx0; j<=kx1; j++){
	currDiv += blur_5x5[2+i][2+j];
	temp += blur_5x5[2+i][2+j]*frame.at(x+j, y+i);
      }
    }
    temp = round(temp/currDiv);
    convolved.at(x,y) = temp;         

    // (h,w) Bottom-Right
    x = frame.width - 1;
    y = frame.height - 1;
    kx0 = -2;
    kx1 = 0;
    ky0 = -2;
    ky1 = 0;
    
    currDiv= 0;
    temp = 0;
    for(int i=ky0; i<=ky1; i++){
      for(int j=kx0; j<=kx1; j++){
	currDiv += blur_5x5[2+i][2+j];
	temp += blur_5x5[2+i][2+j]*frame.at(x+j, y+i);
      }
    }
    temp = round(temp/currDiv);
    convolved.at(x,y) = temp;    
	
   }
   return convolved;
 }

void storch::initializeFrameArray(Picture* pcPic){
  storch::frameWidth = pcPic->getPicWidthInLumaSamples();
  storch::frameHeight = pcPic->getPicHeightInLumaSamples();
  
  storch::reconstructedFrame = (Pel*) malloc(sizeof(Pel) * storch::frameWidth*storch::frameHeight);
  storch::predictedFrame = (Pel*) malloc(sizeof(Pel) * storch::frameWidth*storch::frameHeight);
  storch::isInitialized = 1;
}

void storch::printKernelName(KERNEL kernelSelector){
  switch(kernelSelector){
    case(BLUR_3x3_PseudoG_2):
      cout << "BLUR_3x3_PseudoG_2" << endl;
      break;
    case(BLUR_3x3_PseudoG_3):
      cout << "BLUR_3x3_PseudoG_3" << endl;
      break;
    case(BLUR_3x3_PseudoG_4):
      cout << "BLUR_3x3_PseudoG_4" << endl;
      break;
    case(BLUR_3x3_PseudoG_5):
      cout << "BLUR_3x3_PseudoG_5" << endl;
      break;
    case(BLUR_3x3_PseudoG_6):
      cout << "BLUR_3x3_PseudoG_6" << endl;
      break;
    case(BLUR_3x3_PseudoG_7):
      cout << "BLUR_3x3_PseudoG_7" << endl;
      break;
    case(BLUR_3x3_PseudoG_8):
      cout << "BLUR_3x3_PseudoG_8" << endl;
      break;
    case(BLUR_3x3_PseudoG_9):
      cout << "BLUR_3x3_PseudoG_9" << endl;
      break;
    case(BLUR_3x3_PseudoG_10):
      cout << "BLUR_3x3_PseudoG_10" << endl;
      break;
    case(BLUR_3x3_PseudoG_11):
      cout << "BLUR_3x3_PseudoG_11" << endl;
      break;
    case(BLUR_3x3_PseudoG_12):
      cout << "BLUR_3x3_PseudoG_12" << endl;
      break;
    case(BLUR_3x3_PseudoG_13):
      cout << "BLUR_3x3_PseudoG_13" << endl;
      break;
    case(BLUR_3x3_PseudoG_14):
      cout << "BLUR_3x3_PseudoG_14" << endl;
      break;
    
  
    case(BLUR_5x5_PseudoG_2_3):
      cout << "BLUR_5x5_PseudoG_2_3" << endl;
      break;
    case(BLUR_5x5_PseudoG_2_4):
      cout << "BLUR_5x5_PseudoG_2_4" << endl;
      break;
    case(BLUR_5x5_PseudoG_3_8):
      cout << "BLUR_5x5_PseudoG_3_8" << endl;
      break;

    
    case(BLUR_3x3_floatG_20):
      cout << "BLUR_3x3_floatG_20" << endl;
      break;
    case(BLUR_3x3_floatG_10):
      cout << "BLUR_3x3_floatG_10" << endl;
      break;
    case(BLUR_3x3_floatG_095):
      cout << "BLUR_3x3_floatG_095" << endl;
      break;
    case(BLUR_3x3_floatG_090):
      cout << "BLUR_3x3_floatG_090" << endl;
      break;
    case(BLUR_3x3_floatG_085):
      cout << "BLUR_3x3_floatG_085" << endl;
      break;
    case(BLUR_3x3_floatG_080):
      cout << "BLUR_3x3_floatG_080" << endl;
      break;
    case(BLUR_3x3_floatG_075):
      cout << "BLUR_3x3_floatG_075" << endl;
      break;
    case(BLUR_3x3_floatG_070):
      cout << "BLUR_3x3_floatG_070" << endl;
      break;
    case(BLUR_3x3_floatG_065):
      cout << "BLUR_3x3_floatG_065" << endl;
      break;
    case(BLUR_3x3_floatG_060):
      cout << "BLUR_3x3_floatG_060" << endl;
      break;
    case(BLUR_3x3_floatG_055):
      cout << "BLUR_3x3_floatG_055" << endl;
      break;
    case(BLUR_3x3_floatG_050):
      cout << "BLUR_3x3_floatG_050" << endl;
      break;
    case(BLUR_3x3_floatG_045):
      cout << "BLUR_3x3_floatG_045" << endl;
      break;
    case(BLUR_3x3_floatG_040):
      cout << "BLUR_3x3_floatG_040" << endl;
      break;
    case(BLUR_3x3_floatG_035):
      cout << "BLUR_3x3_floatG_035" << endl;
      break;
    case(BLUR_3x3_floatG_030)     :
      cout << "BLUR_3x3_floatG_030" << endl;
      break;
  
    case(BLUR_3x3_intG_20):
      cout << "BLUR_3x3_intG_20" << endl;
      break;
    case(BLUR_3x3_intG_10):
      cout << "BLUR_3x3_intG_10" << endl;
      break;
    case(BLUR_3x3_intG_095):
      cout << "BLUR_3x3_intG_095" << endl;
      break;
    case(BLUR_3x3_intG_090):
      cout << "BLUR_3x3_intG_090" << endl;
      break;
    case(BLUR_3x3_intG_085):
      cout << "BLUR_3x3_intG_085" << endl;
      break;
    case(BLUR_3x3_intG_080):
      cout << "BLUR_3x3_intG_080" << endl;
      break;
    case(BLUR_3x3_intG_075):
      cout << "BLUR_3x3_intG_075" << endl;
      break;
    case(BLUR_3x3_intG_070):
      cout << "BLUR_3x3_intG_070" << endl;
      break;
    case(BLUR_3x3_intG_065):
      cout << "BLUR_3x3_intG_065" << endl;
      break;
    case(BLUR_3x3_intG_060):
      cout << "BLUR_3x3_intG_060" << endl;
      break;
    case(BLUR_3x3_intG_055):
      cout << "BLUR_3x3_intG_055" << endl;
      break;
    case(BLUR_3x3_intG_050):
      cout << "BLUR_3x3_intG_050" << endl;
      break;
    case(BLUR_3x3_intG_045):
      cout << "BLUR_3x3_intG_045" << endl;
      break;
    case(BLUR_3x3_intG_040):
      cout << "BLUR_3x3_intG_040" << endl;
      break;
    case(BLUR_3x3_intG_035):
      cout << "BLUR_3x3_intG_035" << endl;
      break;
    case(BLUR_3x3_intG_030)     :
      cout << "BLUR_3x3_intG_030" << endl;
      break;
  
    case(BLUR_3x3_v0):
      cout << "BLUR_3x3_v0" << endl;
      break;
    case(BLUR_3x3_v1):
      cout << "BLUR_3x3_v1" << endl;
      break;
    case(BLUR_3x3_v2):
      cout << "BLUR_3x3_v2" << endl;
      break;
    case(BLUR_3x3_v3):
      cout << "BLUR_3x3_v3" << endl;
      break;

    case(BLUR_5x5_v0):
      cout << "BLUR_5x5_v0" << endl;
      break;
    case(BLUR_5x5_v1):
      cout << "BLUR_5x5_v1" << endl;
      break;
    case(BLUR_5x5_v2):
      cout << "BLUR_5x5_v2" << endl;
      break;
    case(BLUR_5x5_v3):
      cout << "BLUR_5x5_v3" << endl;
      break;
    case(BLUR_5x5_v4):
      cout << "BLUR_5x5_v4" << endl;
      break;
    default:
      cout << "ERROR IDENTIFYING KERNEL" << endl;
      break;
  }
  
}

string storch::translateKernelName(KERNEL kernelSelector){
  switch(kernelSelector){
    
    case(BLUR_3x3_PseudoG_2):
      return "BLUR_3x3_PseudoG_2";
      break;
    case(BLUR_3x3_PseudoG_3):
      return  "BLUR_3x3_PseudoG_3";
      break;
    case(BLUR_3x3_PseudoG_4):
      return  "BLUR_3x3_PseudoG_4" ;
      break;
    case(BLUR_3x3_PseudoG_5):
      return  "BLUR_3x3_PseudoG_5" ;
      break;
    case(BLUR_3x3_PseudoG_6):
      return  "BLUR_3x3_PseudoG_6" ;
      break;
    case(BLUR_3x3_PseudoG_7):
      return  "BLUR_3x3_PseudoG_7" ;
      break;
    case(BLUR_3x3_PseudoG_8):
      return  "BLUR_3x3_PseudoG_8" ;
      break;
    case(BLUR_3x3_PseudoG_9):
      return  "BLUR_3x3_PseudoG_9" ;
      break;
    case(BLUR_3x3_PseudoG_10):
      return  "BLUR_3x3_PseudoG_10";
      break;
    case(BLUR_3x3_PseudoG_11):
      return  "BLUR_3x3_PseudoG_11" ;
      break;
    case(BLUR_3x3_PseudoG_12):
      return  "BLUR_3x3_PseudoG_12" ;
      break;
    case(BLUR_3x3_PseudoG_13):
      return  "BLUR_3x3_PseudoG_13" ;
      break;
    case(BLUR_3x3_PseudoG_14):
      return  "BLUR_3x3_PseudoG_14" ;
      break;
    
  
  
    case(BLUR_5x5_PseudoG_2_3):
      return  "BLUR_5x5_PseudoG_2_3" ;
      break;
    case(BLUR_5x5_PseudoG_2_4):
      return  "BLUR_5x5_PseudoG_2_4" ;
      break;
    case(BLUR_5x5_PseudoG_3_8):
      return  "BLUR_5x5_PseudoG_3_8" ;
      break;

    
    case(BLUR_3x3_floatG_20):
      return "BLUR_3x3_floatG_20";
      break;
    case(BLUR_3x3_floatG_10):
      return "BLUR_3x3_floatG_10";
      break;
    case(BLUR_3x3_floatG_095):
      return "BLUR_3x3_floatG_095";
      break;
    case(BLUR_3x3_floatG_090):
      return "BLUR_3x3_floatG_090";
      break;
    case(BLUR_3x3_floatG_085):
      return "BLUR_3x3_floatG_085";
      break;
    case(BLUR_3x3_floatG_080):
      return "BLUR_3x3_floatG_080";
      break;
    case(BLUR_3x3_floatG_075):
      return "BLUR_3x3_floatG_075";
      break;
    case(BLUR_3x3_floatG_070):
      return "BLUR_3x3_floatG_070";
      break;
    case(BLUR_3x3_floatG_065):
      return "BLUR_3x3_floatG_065";
      break;
    case(BLUR_3x3_floatG_060):
      return "BLUR_3x3_floatG_060";
      break;
    case(BLUR_3x3_floatG_055):
      return "BLUR_3x3_floatG_055";
      break;
    case(BLUR_3x3_floatG_050):
      return "BLUR_3x3_floatG_050";
      break;
    case(BLUR_3x3_floatG_045):
      return "BLUR_3x3_floatG_045";
      break;
    case(BLUR_3x3_floatG_040):
      return "BLUR_3x3_floatG_040";
      break;
    case(BLUR_3x3_floatG_035):
      return "BLUR_3x3_floatG_035";
      break;
    case(BLUR_3x3_floatG_030)     :
      return "BLUR_3x3_floatG_030";
      break;

    case(BLUR_3x3_intG_20):
      return "BLUR_3x3_intG_20";
      break;
    case(BLUR_3x3_intG_10):
      return "BLUR_3x3_intG_10";
      break;
    case(BLUR_3x3_intG_095):
      return "BLUR_3x3_intG_095";
      break;
    case(BLUR_3x3_intG_090):
      return "BLUR_3x3_intG_090";
      break;
    case(BLUR_3x3_intG_085):
      return "BLUR_3x3_intG_085";
      break;
    case(BLUR_3x3_intG_080):
      return "BLUR_3x3_intG_080";
      break;
    case(BLUR_3x3_intG_075):
      return "BLUR_3x3_intG_075";
      break;
    case(BLUR_3x3_intG_070):
      return "BLUR_3x3_intG_070";
      break;
    case(BLUR_3x3_intG_065):
      return "BLUR_3x3_intG_065";
      break;
    case(BLUR_3x3_intG_060):
      return "BLUR_3x3_intG_060";
      break;
    case(BLUR_3x3_intG_055):
      return "BLUR_3x3_intG_055";
      break;
    case(BLUR_3x3_intG_050):
      return "BLUR_3x3_intG_050";
      break;
    case(BLUR_3x3_intG_045):
      return "BLUR_3x3_intG_045";
      break;
    case(BLUR_3x3_intG_040):
      return "BLUR_3x3_intG_040";
      break;
    case(BLUR_3x3_intG_035):
      return "BLUR_3x3_intG_035";
      break;
    case(BLUR_3x3_intG_030)     :
      return "BLUR_3x3_intG_030";
      break;


    case(BLUR_3x3_v0):
      return "BLUR_3x3_v0";
      break;
    case(BLUR_3x3_v1):
      return "BLUR_3x3_v1";
      break;
    case(BLUR_3x3_v2):
      return "BLUR_3x3_v2";
      break;
    case(BLUR_3x3_v3):
      return "BLUR_3x3_v3";
      break;
      
      
    case(BLUR_5x5_v0):
      return "BLUR_5x5_v0";
      break;
    case(BLUR_5x5_v1):
      return "BLUR_5x5_v1";
      break;
    case(BLUR_5x5_v2):
      return "BLUR_5x5_v2";
      break;
    case(BLUR_5x5_v3):
      return "BLUR_5x5_v3";
      break;
    case(BLUR_5x5_v4):
      return "BLUR_5x5_v4";
      break;
    default:
      cout <<  "ERROR IDENTIFYING KERNEL" << endl;
      return " ";
      break;
  }
  
}

