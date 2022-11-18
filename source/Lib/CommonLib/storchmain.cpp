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

int storch::var, storch::numExtractedBlocks;
int storch::extractedFrames[EXT_NUM][500];
int storch::targetBlock;
int storch::target_availability;

storch::storch() {
  for(int i=0; i<EXT_NUM; i++){
    for(int POC=0; POC<500; POC++){
        extractedFrames[EXT_ORIGINAL][POC] = 0;
    }
  }
  var = 0;
  numExtractedBlocks = 0;
  targetBlock=0;
  target_availability=0;
}

void storch::finishEncoding(){
  printf("Finished the encoding\n");
}

// Export the samples of a PU into a CSV file
void storch::exportSamplesBlock_v2(PelBuf samples, SamplesType type, int x, int y){
    int h,w;

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
        name += "_" + std::to_string(x) + "x" + std::to_string(y);
        name += "_" + std::to_string(storch::numExtractedBlocks) + ".csv";
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
  
  std::ofstream fileHandler;
  string name;

  if(type == INTRA_REF_UNFILTERED){
    name = (string) "intra_ref_unfiltered";
    name += "_" + std::to_string(x) + "x" + std::to_string(y) + ".csv";
  }
  else if(type == INTRA_REF_FILTERED){
    name = (string) "intra_ref_filtered";
    name += "_" + std::to_string(x) + "x" + std::to_string(y) + ".csv";
  }
  else{
      printf("ERROR -- Incorrect samples type when exporting intra references buffer\n");
  }

  fileHandler.open(name);

  
  // TODO: This only works for LUMA with MRL=0
  int totalReferences = (cu.lwidth() * cu.lheight())/16 + 1;

  for (int r=0; r<totalReferences; r++){
    fileHandler << buffer[r] << ",";
  }
  fileHandler.close();
}