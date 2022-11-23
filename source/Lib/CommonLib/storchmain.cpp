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

int storch::var, storch::numExtractedBlocks;
int storch::extractedFrames[EXT_NUM][500];
int storch::targetBlock;
int storch::targetBlock_multSizes;
int storch::target_availability;

struct timeval storch::rmdGen1, storch::rmdGen2, storch::rmdHevc1, storch::rmdHevc2, storch::rmdVvc1, storch::rmdVvc2, storch::rmdMrl1, storch::rmdMrl2, storch::rmdMip1, storch::rmdMip2, storch::rdoGen1, storch::rdoGen2, storch::rdoIsp1, storch::rdoIsp2; 
double storch::intraRmdGenTime, storch::intraRmd1Time, storch::intraRmd2Time, storch::intraRmdMrlTime, storch::intraRmdMipTime, storch::intraRdoGenTime, storch::intraRdoIspTime;

PelBuf storch::reconstructedPrev;
Picture storch::previousPic;
PelStorage storch::recoBuf;


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
  
  intraRmdGenTime = 0.0;
  intraRmd1Time = 0.0;
  intraRmd2Time = 0.0;
  intraRmdMrlTime = 0.0;
  intraRmdMipTime = 0.0;
  intraRdoGenTime = 0.0;
  intraRdoIspTime = 0.0;
  
  previousPic = Picture();
  recoBuf = PelStorage();
}

void storch::finishEncoding(){
  printf("Finished the encoding\n");
  
  cout << "###      Intraframe prediction time:" << endl;

  cout << "General RMD:  " << (intraRmdGenTime) << endl;
  cout << "  RMD Part 1            " << intraRmd1Time << endl;
  cout << "  RMD Part 2            " << intraRmd2Time << endl;
  cout << "  RMD MRL               " << intraRmdMrlTime << endl;
  cout << "  RMD MIP               " << intraRmdMipTime << endl;
  cout << "  RMD Others            " << intraRmdGenTime - (intraRmd1Time+intraRmd2Time+intraRmdMrlTime+intraRmdMipTime)<< endl;

  cout << "General RDO:            " << intraRdoGenTime << endl;
  cout << "  RDO ISPs              " << intraRdoIspTime << endl;
  cout << "  RDO Normal            " << intraRdoGenTime - intraRdoIspTime << endl;

  cout << "---------------------------------------------------------------------" << endl;
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

void storch::finishIntraRmdMip(){
  gettimeofday(&rmdMip2, NULL);
  intraRmdMipTime += (double) (rmdMip2.tv_usec - rmdMip1.tv_usec)/1000000 + (double) (rmdMip2.tv_sec - rmdMip1.tv_sec);
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

void storch::storeRecBuf(Picture* pic){
//  recoBuf = pic->m_bufs[PIC_RECONSTRUCTION];
//  recoBuf.copyFrom(pic->getRecoBuf(),true );
//  printf("CHAMOU STORE\n");
  recoBuf.createFromBuf(pic->getRecoBuf());
  
//  free(recoBuf.bufs[COMPONENT_Y].buf);
  
  recoBuf.bufs[COMPONENT_Y].buf = (Pel*) malloc(sizeof(Pel) * pic->getRecoBuf(COMPONENT_Y).height * pic->getRecoBuf(COMPONENT_Y).stride);
  
  // Cópia explicita dos pixels pra evitar ponteiros
  // Pel* pixels = pic->getRecoBuf(COMPONENT_Y).buf;
//  int pSize = pic->getRecoBuf(COMPONENT_Y).height * pic->getRecoBuf(COMPONENT_Y).stride;
//  Pel* pixels = (Pel*) malloc(sizeof(Pel) * pSize);
//  memcpy(pixels, pic->getRecoBuf(COMPONENT_Y).buf, pSize);
//  memcpy(recoBuf.bufs[COMPONENT_Y].buf, pixels, pSize);
  
//  printf("Width=%d  Height=%d  Stride=%d\n", pic->getRecoBuf(COMPONENT_Y).width, pic->getRecoBuf(COMPONENT_Y).height, pic->getRecoBuf(COMPONENT_Y).stride);
  
//  memcpy(recoBuf.bufs[COMPONENT_Y].buf, pic->getRecoBuf(COMPONENT_Y).buf, pSize);
  
  int stride = pic->getRecoBuf(COMPONENT_Y).stride;
  //*
  for(int i=0; i<pic->getPicHeightInLumaSamples(); i++){
    for(int j=0; j<pic->getPicWidthInLumaSamples(); j++){
      recoBuf.bufs[COMPONENT_Y].buf[i*stride + j] = pic->getRecoBuf(COMPONENT_Y).buf[i*stride + j];
//      recoBuf.bufs[COMPONENT_Y].buf[i*stride + j] = pixels[i*stride + j];
//      recoBuf.bufs[COMPONENT_Y].buf[i*stride + j] = pixels[i*832 + j];
    }
  }  
  //*/
  
  /*
  for(int i=0; i<pic->getPicHeightInLumaSamples(); i++){
    for(int j=0; j<pic->getPicWidthInLumaSamples(); j++){
      printf("%d,", recoBuf.bufs[COMPONENT_Y].buf[i*stride + j]);
      
//      printf("%d,", recoBuf.bufs[COMPONENT_Y].buf[i*832 + j]);
    }
    printf("\n");
  }
  printf("############################################\n");
  //*/
  
}

PelStorage storch::loadRecBuf(){
  return recoBuf;
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

