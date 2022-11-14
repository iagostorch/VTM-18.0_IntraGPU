/* The copyright in this software is being made available under the BSD
* License, included below. This software may be subject to other third party
* and contributor rights, including patent rights, and no such rights are
* granted under this license.
*
* Copyright (c) 2010-2022, ITU/ISO/IEC
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*  * Redistributions of source code must retain the above copyright notice,
*    this list of conditions and the following disclaimer.
*  * Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
*  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*/

/** \file     MipData.h
\brief    weight and bias data for matrix-based intra prediction (MIP)
*/

static const uint8_t MIP_SHIFT_MATRIX  =  6;
static const uint8_t MIP_OFFSET_MATRIX = 32;

// 16 matrices (pred modes)
// Each line of each matrix is used to compute one sample of the predicted block. All 4 coefficients in the same line are used to predict a single sample
// Predicted block has 4x4 samples (16 samples total)
ALIGN_DATA(MEMORY_ALIGN_DEF_SIZE, const uint8_t mipMatrix4x4[16][16][4]) =
{
  {
    {   32,   30,   90,   28},
    {   32,   32,   72,   28},
    {   34,   77,   53,   30},
    {   51,  124,   36,   37},
    {   31,   31,   95,   37},
    {   33,   31,   70,   50},
    {   52,   80,   25,   60},
    {   78,  107,    1,   65},
    {   31,   29,   37,   95},
    {   38,   34,   19,  101},
    {   73,   85,    0,   81},
    {   92,   99,    0,   65},
    {   34,   29,   14,  111},
    {   48,   48,    7,  100},
    {   80,   91,    0,   74},
    {   89,   97,    0,   64}
  },
  {
    {   31,   23,   34,   29},
    {   31,   43,   34,   31},
    {   30,   95,   34,   32},
    {   29,  100,   35,   33},
    {   31,   23,   34,   29},
    {   31,   43,   34,   31},
    {   30,   95,   34,   32},
    {   29,   99,   35,   33},
    {   31,   24,   35,   29},
    {   31,   44,   34,   31},
    {   30,   95,   35,   32},
    {   29,   99,   35,   33},
    {   31,   24,   35,   30},
    {   31,   44,   35,   31},
    {   30,   95,   35,   32},
    {   29,   99,   35,   33}
  },
  {
    {   32,   32,   36,   58},
    {   32,   29,   26,   66},
    {   36,   37,   23,   61},
    {   79,   84,    3,   37},
    {   32,   32,   30,   69},
    {   33,   29,   24,   71},
    {   44,   16,   21,   70},
    {   96,   18,    0,   57},
    {   32,   31,   24,   74},
    {   33,   30,   23,   71},
    {   36,   24,   24,   71},
    {   59,    9,   16,   68},
    {   32,   32,   23,   75},
    {   33,   30,   24,   70},
    {   32,   30,   25,   71},
    {   36,   26,   25,   70}
  },
  {
    {   32,   33,   34,   32},
    {   32,   30,   22,   38},
    {   29,   46,   25,   38},
    {   53,  123,   28,   22},
    {   32,   33,   30,   37},
    {   32,   30,   21,   38},
    {   32,   40,   24,   38},
    {   64,  116,   26,   17},
    {   32,   32,   23,   49},
    {   32,   30,   21,   39},
    {   34,   39,   24,   37},
    {   72,  109,   23,   16},
    {   33,   31,   17,   60},
    {   32,   31,   21,   39},
    {   35,   41,   24,   37},
    {   72,  106,   22,   18}
  },
  {
    {   34,   25,   89,   20},
    {   38,   32,   47,   24},
    {   40,   86,   29,   27},
    {   38,   98,   32,   29},
    {   34,   31,   94,   40},
    {   44,   25,   83,   27},
    {   54,   72,   43,   16},
    {   47,   94,   33,   22},
    {   33,   31,   36,   94},
    {   43,   23,   51,   76},
    {   62,   55,   64,   25},
    {   57,   89,   38,   15},
    {   32,   32,   28,  101},
    {   38,   26,   33,   94},
    {   55,   38,   68,   47},
    {   59,   80,   52,   16}
  },
  {
    {   28,   30,   68,   29},
    {   23,   48,   23,   48},
    {   39,   98,   16,   42},
    {   84,   86,   20,   17},
    {   25,   31,   52,   74},
    {   38,   68,    5,   70},
    {   95,   78,    7,   21},
    {  127,   54,   12,    0},
    {   30,   47,   14,  107},
    {   79,   76,    0,   53},
    {  127,   59,    7,    1},
    {  127,   51,    9,    0},
    {   50,   71,    1,   96},
    {  109,   69,    7,   25},
    {  127,   56,    9,    0},
    {  123,   53,   13,    0}
  },
  {
    {   40,   20,   72,   18},
    {   48,   29,   44,   18},
    {   53,   81,   35,   18},
    {   48,   96,   33,   22},
    {   45,   23,   79,   49},
    {   61,   21,   56,   49},
    {   72,   52,   32,   48},
    {   65,   69,   20,   50},
    {   41,   27,   29,   96},
    {   49,   22,   28,   94},
    {   52,   22,   28,   93},
    {   49,   27,   27,   92},
    {   37,   29,   26,   98},
    {   39,   28,   28,   97},
    {   38,   28,   30,   97},
    {   38,   29,   30,   95}
  },
  {
    {   33,   27,   43,   27},
    {   32,   29,   31,   31},
    {   31,   73,   33,   31},
    {   35,  104,   34,   28},
    {   32,   30,   63,   22},
    {   33,   26,   33,   29},
    {   33,   57,   33,   30},
    {   37,  100,   35,   27},
    {   32,   31,   85,   25},
    {   34,   25,   39,   25},
    {   35,   39,   32,   28},
    {   40,   91,   35,   25},
    {   32,   30,   77,   50},
    {   34,   26,   54,   22},
    {   37,   31,   34,   27},
    {   45,   75,   34,   23}
  },
  {
    {   34,   25,   77,   19},
    {   36,   34,   56,   24},
    {   41,   83,   39,   30},
    {   47,   96,   28,   35},
    {   34,   31,   70,   65},
    {   38,   29,   53,   77},
    {   43,   36,   37,   83},
    {   48,   39,   28,   83},
    {   33,   31,   31,   98},
    {   33,   31,   30,   99},
    {   34,   30,   31,   98},
    {   36,   29,   31,   96},
    {   32,   32,   30,   97},
    {   32,   32,   31,   96},
    {   31,   33,   33,   96},
    {   32,   33,   34,   94}
  },
  {
    {   30,   30,   93,   19},
    {   31,   59,   67,   34},
    {   31,   79,   36,   59},
    {   30,   67,   17,   79},
    {   30,   38,   68,   69},
    {   29,   40,   43,   91},
    {   26,   35,   32,  101},
    {   23,   32,   30,  101},
    {   26,   34,   30,  101},
    {   23,   33,   30,  102},
    {   20,   32,   31,  102},
    {   18,   33,   32,  102},
    {   23,   33,   31,  100},
    {   20,   34,   32,  100},
    {   18,   35,   33,  100},
    {   18,   35,   33,  100}
  },
  {
    {   31,   54,   90,   26},
    {   32,   60,   53,   61},
    {   34,   49,   37,   84},
    {   34,   39,   35,   89},
    {   35,   38,   41,   88},
    {   35,   35,   32,   96},
    {   35,   31,   33,   96},
    {   35,   32,   35,   94},
    {   34,   34,   30,   97},
    {   35,   32,   33,   95},
    {   35,   32,   34,   94},
    {   35,   34,   34,   93},
    {   34,   34,   34,   93},
    {   35,   34,   34,   93},
    {   35,   34,   34,   92},
    {   36,   34,   35,   91}
  },
  {
    {   32,   29,   54,   24},
    {   31,   32,   34,   29},
    {   31,   43,   34,   29},
    {   32,   67,   36,   28},
    {   31,   34,   69,   37},
    {   31,   35,   46,   33},
    {   30,   35,   39,   33},
    {   30,   42,   39,   36},
    {   31,   35,   39,   88},
    {   30,   38,   41,   84},
    {   30,   39,   40,   81},
    {   39,   46,   38,   78},
    {   31,   36,   34,   96},
    {   34,   38,   37,   93},
    {   55,   42,   38,   82},
    {   89,   53,   38,   65}
  },
  {
    {   32,   33,   43,   29},
    {   32,   30,   29,   33},
    {   31,   47,   31,   33},
    {   33,  100,   31,   31},
    {   32,   33,   74,   25},
    {   32,   32,   34,   31},
    {   32,   33,   30,   33},
    {   32,   68,   30,   32},
    {   32,   31,   91,   40},
    {   32,   32,   58,   26},
    {   31,   31,   30,   32},
    {   31,   42,   30,   33},
    {   32,   31,   49,   85},
    {   32,   31,   83,   35},
    {   31,   33,   48,   29},
    {   31,   36,   32,   33}
  },
  {
    {   31,   29,   81,   35},
    {   32,   28,   34,   50},
    {   31,   75,   16,   43},
    {   34,  103,   29,   32},
    {   32,   32,   53,   78},
    {   31,   28,   36,   88},
    {   30,   52,   18,   73},
    {   52,   88,   17,   35},
    {   32,   32,   35,   94},
    {   30,   31,   35,   95},
    {   36,   29,   31,   92},
    {  100,   43,   16,   40},
    {   32,   32,   35,   93},
    {   30,   32,   38,   93},
    {   55,   18,   37,   83},
    {  127,    0,   30,   40}
  },
  {
    {   31,   22,   47,   30},
    {   31,   48,   25,   34},
    {   30,   95,   31,   32},
    {   32,  103,   33,   32},
    {   30,   24,   57,   31},
    {   30,   47,   26,   34},
    {   31,   95,   31,   32},
    {   43,   97,   35,   25},
    {   29,   26,   44,   63},
    {   37,   38,   24,   47},
    {   74,   63,   28,   20},
    {  110,   58,   34,    3},
    {   46,   22,    5,  108},
    {   93,    5,    9,   77},
    {  127,    0,   17,   52},
    {  127,    0,   15,   50}
  },
  {
    {   32,   27,   68,   24},
    {   35,   23,   35,   28},
    {   35,   64,   29,   29},
    {   37,  104,   33,   28},
    {   32,   32,   91,   40},
    {   36,   23,   67,   36},
    {   49,   23,   39,   28},
    {   60,   67,   30,   20},
    {   32,   32,   36,   95},
    {   35,   29,   38,   93},
    {   50,   16,   30,   84},
    {   72,   16,   15,   65},
    {   32,   32,   27,  100},
    {   33,   32,   29,  100},
    {   37,   29,   30,   98},
    {   48,   21,   29,   90}
  }
};


// 8 matrices (pred modes)
// Each line of each matrix is used to compute one sample of the predicted block. All 8 coefficients in the same line are used to predict a single sample
// Predicted block has 4x4 samples (16 samples total)
ALIGN_DATA(MEMORY_ALIGN_DEF_SIZE, const uint8_t mipMatrix8x8[8][16][8]) =
{
  {
    {   30,   63,   46,   37,   25,   33,   33,   34},
    {   30,   60,   66,   38,   32,   31,   32,   33},
    {   29,   45,   74,   42,   32,   32,   32,   33},
    {   30,   39,   62,   58,   32,   33,   32,   33},
    {   30,   66,   55,   39,   32,   30,   30,   36},
    {   29,   54,   69,   40,   33,   31,   31,   33},
    {   28,   48,   71,   43,   32,   33,   32,   33},
    {   28,   41,   72,   46,   32,   34,   32,   33},
    {   30,   66,   56,   40,   32,   33,   28,   33},
    {   29,   55,   69,   39,   33,   33,   30,   32},
    {   27,   46,   72,   43,   33,   33,   32,   33},
    {   27,   42,   69,   48,   32,   34,   32,   33},
    {   30,   63,   55,   40,   32,   33,   35,   30},
    {   29,   56,   66,   40,   33,   33,   33,   30},
    {   27,   47,   69,   44,   33,   33,   33,   32},
    {   27,   42,   65,   50,   32,   34,   32,   33}
  },
  {
    {   32,   33,   30,   31,   74,   30,   31,   32},
    {   33,   56,   28,   30,   41,   29,   32,   32},
    {   33,   77,   52,   26,   29,   34,   30,   32},
    {   33,   37,   80,   41,   31,   34,   30,   32},
    {   32,   32,   33,   31,   59,   76,   28,   31},
    {   33,   31,   31,   30,   78,   40,   28,   32},
    {   33,   47,   28,   29,   53,   27,   31,   31},
    {   33,   61,   44,   28,   34,   32,   31,   31},
    {   32,   31,   34,   30,   26,   64,   76,   27},
    {   32,   31,   34,   29,   45,   86,   36,   29},
    {   33,   27,   34,   29,   73,   55,   25,   32},
    {   33,   33,   34,   30,   62,   33,   30,   31},
    {   32,   31,   34,   30,   30,   29,   58,   74},
    {   32,   31,   35,   29,   27,   53,   77,   35},
    {   32,   30,   36,   29,   40,   80,   44,   31},
    {   33,   28,   37,   30,   58,   60,   31,   33}
  },
  {
    {   32,   51,   27,   32,   27,   50,   29,   32},
    {   32,   95,   42,   29,   29,   42,   30,   32},
    {   32,   27,   99,   34,   31,   41,   29,   32},
    {   32,   34,   21,  104,   31,   42,   30,   32},
    {   32,   45,   30,   32,    9,   88,   40,   30},
    {   32,   77,   38,   30,    9,   76,   38,   30},
    {   32,   38,   78,   33,   14,   67,   37,   30},
    {   32,   30,   30,   87,   20,   59,   38,   31},
    {   33,   37,   32,   32,   27,   18,  106,   34},
    {   34,   44,   34,   31,   25,   17,  108,   31},
    {   36,   39,   45,   31,   24,   15,  108,   30},
    {   37,   31,   31,   54,   25,   14,  101,   32},
    {   36,   33,   32,   30,   29,   37,   13,  110},
    {   39,   32,   32,   29,   27,   37,   15,  108},
    {   44,   33,   31,   27,   25,   37,   16,  106},
    {   47,   30,   31,   32,   25,   34,   19,  102}
  },
  {
    {   32,   48,   35,   35,   47,   68,   31,   31},
    {   32,   33,   59,   40,   27,   71,   33,   30},
    {   32,   29,   47,   65,   24,   62,   37,   30},
    {   33,   33,   31,   81,   26,   50,   42,   32},
    {   32,   30,   40,   38,   30,   70,   55,   31},
    {   32,   20,   46,   50,   26,   55,   64,   31},
    {   33,   30,   29,   66,   25,   41,   72,   33},
    {   36,   34,   27,   69,   26,   31,   67,   39},
    {   33,   28,   36,   40,   30,   26,   85,   47},
    {   36,   27,   33,   50,   31,   20,   79,   53},
    {   43,   30,   26,   57,   28,   17,   67,   62},
    {   51,   27,   28,   55,   22,   23,   49,   70},
    {   38,   29,   32,   39,   28,   30,   22,  104},
    {   51,   31,   28,   43,   24,   31,   17,  102},
    {   69,   23,   30,   40,   15,   38,   10,   95},
    {   77,   13,   35,   38,    8,   43,    8,   90}
  },
  {
    {   32,   38,   32,   33,  101,   40,   29,   32},
    {   32,   40,   37,   32,  100,   36,   30,   32},
    {   32,   37,   46,   35,   94,   33,   30,   31},
    {   33,   34,   30,   62,   81,   35,   30,   31},
    {   32,   32,   33,   32,   22,  102,   39,   29},
    {   32,   31,   33,   33,   26,  104,   34,   28},
    {   33,   33,   33,   33,   31,  103,   32,   28},
    {   33,   32,   34,   36,   37,   94,   33,   28},
    {   32,   33,   32,   32,   34,   24,   99,   36},
    {   32,   34,   33,   33,   33,   30,   98,   32},
    {   33,   33,   34,   33,   31,   37,   95,   29},
    {   33,   33,   33,   36,   30,   46,   85,   31},
    {   32,   33,   32,   33,   30,   34,   23,  104},
    {   32,   34,   33,   33,   31,   32,   30,   98},
    {   32,   33,   34,   34,   31,   29,   39,   91},
    {   33,   33,   32,   37,   32,   30,   47,   82}
  },
  {
    {   32,   52,   48,   31,   38,   76,   26,   32},
    {   33,   19,   62,   50,   25,   50,   51,   31},
    {   33,   30,   20,   74,   29,   29,   54,   51},
    {   34,   35,   23,   56,   31,   25,   41,   76},
    {   33,   25,   38,   39,   28,   39,   83,   35},
    {   35,   28,   25,   47,   31,   23,   57,   74},
    {   37,   35,   22,   38,   31,   27,   30,  101},
    {   38,   32,   33,   29,   30,   31,   27,  103},
    {   34,   32,   27,   37,   32,   25,   41,   92},
    {   38,   33,   28,   32,   30,   31,   18,  111},
    {   40,   32,   33,   27,   29,   33,   18,  111},
    {   40,   32,   34,   27,   28,   33,   23,  105},
    {   35,   32,   30,   33,   31,   33,   20,  107},
    {   38,   31,   33,   30,   29,   33,   21,  106},
    {   40,   32,   33,   29,   29,   34,   22,  105},
    {   40,   32,   33,   30,   29,   34,   24,  101}
  },
  {
    {   32,   28,   31,   33,   92,   33,   30,   31},
    {   33,   30,   28,   33,   71,   26,   32,   30},
    {   33,   60,   26,   33,   47,   28,   33,   30},
    {   33,   63,   44,   36,   37,   31,   33,   30},
    {   33,   30,   31,   33,   43,   90,   33,   29},
    {   33,   28,   29,   34,   71,   71,   26,   30},
    {   33,   30,   26,   33,   86,   45,   28,   30},
    {   33,   38,   29,   32,   74,   32,   33,   29},
    {   33,   32,   30,   32,   29,   41,   95,   27},
    {   34,   31,   29,   33,   26,   71,   73,   22},
    {   34,   31,   29,   33,   37,   88,   46,   25},
    {   33,   32,   28,   34,   55,   75,   36,   28},
    {   34,   31,   30,   32,   33,   27,   43,   89},
    {   35,   32,   28,   33,   33,   23,   77,   59},
    {   34,   33,   28,   33,   30,   35,   91,   37},
    {   34,   34,   28,   34,   33,   53,   74,   31}
  },
  {
    {   33,   49,   26,   32,   26,   52,   28,   31},
    {   33,   71,   72,   24,   30,   32,   34,   31},
    {   32,   23,   70,   68,   32,   32,   32,   32},
    {   31,   33,   21,  106,   33,   32,   32,   33},
    {   34,   47,   32,   29,    5,   86,   44,   26},
    {   34,   44,   89,   28,   28,   37,   33,   30},
    {   32,   27,   46,   89,   33,   31,   31,   32},
    {   30,   33,   20,  107,   33,   33,   32,   33},
    {   35,   39,   42,   27,   26,   24,   92,   35},
    {   34,   27,   87,   43,   30,   34,   38,   31},
    {   31,   31,   32,  100,   32,   33,   30,   32},
    {   29,   32,   22,  106,   33,   33,   32,   33},
    {   35,   29,   47,   32,   32,   32,   17,  100},
    {   34,   24,   69,   60,   34,   33,   28,   44},
    {   31,   33,   31,   99,   32,   33,   32,   31},
    {   29,   33,   25,  103,   33,   33,   32,   35}
  }
};

// 6 matrices (pred modes)
// Each line of each matrix is used to compute one sample of the predicted block. All 8**** coefficients in the same line are used to predict a single sample
// Predicted block has 8x8 samples (64 samples total)
// **** IMPORTANT: The first coefficient of all lines is zero, therefore only 7 (seven) coefficients per line are kept in the matrix
ALIGN_DATA(MEMORY_ALIGN_DEF_SIZE, const uint8_t mipMatrix16x16[6][64][7]) =
{
  {
    {   42,   37,   33,   27,   44,   33,   35},
    {   71,   39,   34,   24,   36,   35,   36},
    {   77,   46,   35,   33,   30,   34,   36},
    {   64,   60,   35,   33,   31,   32,   36},
    {   49,   71,   38,   32,   32,   31,   36},
    {   42,   66,   50,   33,   31,   32,   36},
    {   40,   52,   67,   33,   31,   32,   35},
    {   38,   43,   75,   33,   32,   32,   35},
    {   56,   40,   33,   26,   43,   38,   36},
    {   70,   49,   34,   30,   28,   38,   38},
    {   65,   57,   36,   34,   28,   33,   39},
    {   59,   60,   39,   33,   30,   31,   38},
    {   55,   60,   43,   33,   30,   31,   38},
    {   51,   61,   47,   33,   30,   32,   37},
    {   46,   62,   51,   34,   30,   32,   37},
    {   42,   60,   55,   33,   31,   32,   37},
    {   60,   42,   34,   30,   37,   43,   38},
    {   68,   52,   35,   35,   22,   37,   40},
    {   62,   58,   37,   34,   28,   31,   40},
    {   58,   59,   41,   33,   30,   30,   39},
    {   56,   59,   44,   34,   30,   31,   38},
    {   53,   60,   45,   33,   30,   31,   38},
    {   49,   65,   45,   33,   30,   31,   38},
    {   45,   64,   47,   33,   31,   32,   38},
    {   59,   44,   35,   31,   34,   43,   41},
    {   66,   53,   36,   35,   25,   31,   43},
    {   61,   58,   38,   34,   29,   30,   40},
    {   59,   57,   41,   33,   30,   31,   39},
    {   57,   58,   43,   33,   30,   31,   39},
    {   54,   61,   43,   33,   31,   31,   39},
    {   51,   64,   43,   33,   31,   31,   39},
    {   48,   64,   45,   33,   32,   31,   39},
    {   57,   45,   35,   30,   35,   40,   44},
    {   65,   54,   37,   33,   33,   24,   44},
    {   63,   56,   38,   34,   30,   29,   39},
    {   61,   56,   41,   34,   30,   32,   39},
    {   58,   58,   42,   33,   31,   31,   39},
    {   54,   62,   41,   33,   31,   31,   39},
    {   51,   65,   42,   33,   31,   31,   39},
    {   48,   63,   43,   33,   32,   31,   39},
    {   55,   46,   35,   30,   36,   38,   47},
    {   65,   53,   37,   32,   36,   26,   40},
    {   65,   54,   38,   33,   31,   30,   38},
    {   63,   55,   39,   33,   30,   32,   38},
    {   59,   58,   40,   33,   31,   31,   39},
    {   54,   64,   40,   33,   31,   30,   40},
    {   49,   66,   40,   32,   32,   30,   41},
    {   48,   64,   42,   32,   32,   30,   41},
    {   54,   46,   35,   30,   34,   39,   49},
    {   64,   52,   36,   32,   34,   34,   35},
    {   65,   53,   37,   33,   32,   32,   37},
    {   63,   55,   38,   33,   31,   31,   39},
    {   59,   60,   38,   33,   31,   31,   40},
    {   54,   64,   38,   33,   32,   30,   40},
    {   49,   66,   39,   33,   32,   29,   41},
    {   47,   64,   42,   32,   33,   29,   42},
    {   51,   46,   35,   31,   33,   37,   54},
    {   61,   51,   36,   32,   33,   38,   36},
    {   63,   53,   37,   32,   32,   34,   37},
    {   62,   55,   37,   33,   32,   32,   39},
    {   58,   59,   37,   33,   32,   31,   40},
    {   53,   63,   38,   33,   32,   31,   40},
    {   49,   64,   40,   33,   33,   30,   41},
    {   46,   62,   42,   33,   33,   30,   42}
  },
  {
    {   39,   34,   33,   58,   44,   31,   32},
    {   60,   38,   32,   40,   51,   30,   31},
    {   73,   49,   31,   39,   48,   32,   31},
    {   60,   73,   30,   39,   46,   33,   32},
    {   43,   87,   35,   38,   45,   33,   32},
    {   35,   78,   54,   36,   45,   33,   32},
    {   33,   47,   86,   35,   44,   33,   32},
    {   31,   17,  114,   34,   44,   34,   33},
    {   43,   37,   32,   53,   70,   30,   31},
    {   53,   50,   30,   42,   72,   31,   30},
    {   52,   66,   30,   39,   70,   32,   30},
    {   46,   78,   35,   37,   68,   34,   30},
    {   43,   75,   48,   37,   66,   34,   30},
    {   40,   62,   68,   35,   65,   35,   30},
    {   33,   37,   97,   33,   62,   37,   31},
    {   26,   14,  122,   32,   59,   38,   33},
    {   40,   39,   33,   34,   87,   37,   30},
    {   45,   54,   32,   34,   84,   41,   29},
    {   41,   70,   35,   33,   83,   40,   29},
    {   37,   73,   44,   32,   82,   40,   30},
    {   37,   65,   60,   31,   81,   41,   29},
    {   35,   48,   82,   30,   79,   43,   29},
    {   28,   27,  108,   28,   76,   45,   30},
    {   19,   11,  127,   27,   70,   46,   32},
    {   38,   40,   34,   27,   73,   62,   28},
    {   39,   54,   35,   30,   73,   62,   28},
    {   33,   65,   41,   29,   75,   59,   28},
    {   30,   65,   53,   27,   76,   58,   29},
    {   29,   53,   72,   26,   77,   58,   29},
    {   27,   35,   95,   24,   77,   60,   28},
    {   19,   19,  117,   23,   74,   61,   30},
    {    9,   16,  127,   23,   68,   60,   34},
    {   35,   40,   35,   29,   44,   89,   30},
    {   33,   51,   39,   29,   49,   86,   30},
    {   28,   57,   49,   28,   53,   83,   30},
    {   24,   52,   65,   26,   56,   82,   30},
    {   22,   39,   86,   24,   58,   82,   30},
    {   18,   22,  108,   23,   59,   82,   31},
    {   10,   13,  125,   22,   58,   80,   33},
    {    0,   19,  127,   22,   56,   74,   40},
    {   33,   40,   36,   31,   28,   90,   45},
    {   29,   46,   44,   29,   31,   92,   43},
    {   24,   45,   58,   28,   34,   91,   43},
    {   19,   37,   78,   26,   37,   91,   43},
    {   15,   22,   99,   25,   38,   91,   42},
    {   11,   11,  118,   24,   39,   90,   44},
    {    2,   11,  127,   23,   41,   85,   48},
    {    0,   17,  127,   23,   43,   75,   55},
    {   31,   37,   39,   30,   28,   54,   82},
    {   27,   37,   52,   28,   30,   58,   79},
    {   22,   30,   70,   27,   32,   58,   79},
    {   15,   19,   91,   26,   33,   58,   79},
    {   10,    8,  111,   25,   34,   58,   79},
    {    5,    2,  125,   25,   35,   57,   80},
    {    0,    9,  127,   25,   36,   53,   84},
    {    0,   13,  127,   25,   39,   47,   88},
    {   28,   29,   46,   28,   39,    2,  123},
    {   24,   24,   62,   27,   41,    1,  125},
    {   19,   14,   81,   25,   43,    0,  126},
    {   13,    4,  101,   24,   44,    0,  127},
    {    6,    0,  116,   23,   45,    0,  127},
    {    0,    0,  126,   23,   45,    1,  127},
    {    0,    4,  127,   25,   44,    2,  127},
    {    0,    9,  127,   25,   44,    3,  127}
  },
  {
    {   30,   32,   32,   42,   34,   32,   32},
    {   63,   26,   34,   16,   38,   32,   32},
    {   98,   26,   34,   25,   34,   33,   32},
    {   75,   61,   30,   31,   32,   33,   32},
    {   36,   94,   32,   30,   33,   32,   32},
    {   26,   76,   58,   30,   33,   32,   32},
    {   30,   39,   91,   31,   32,   33,   31},
    {   32,   23,  105,   32,   32,   32,   32},
    {   34,   30,   33,   31,   52,   29,   32},
    {   66,   24,   34,   11,   41,   33,   32},
    {   97,   28,   34,   24,   34,   33,   32},
    {   71,   65,   30,   30,   32,   33,   32},
    {   34,   92,   35,   30,   33,   32,   32},
    {   26,   70,   64,   29,   34,   32,   32},
    {   30,   37,   94,   30,   33,   32,   31},
    {   32,   23,  105,   31,   33,   33,   31},
    {   37,   29,   33,    8,   79,   27,   32},
    {   71,   22,   35,    5,   50,   32,   32},
    {   98,   29,   34,   23,   34,   34,   32},
    {   66,   70,   30,   31,   31,   33,   32},
    {   31,   92,   38,   30,   33,   32,   32},
    {   26,   66,   68,   29,   34,   32,   31},
    {   30,   34,   97,   30,   34,   33,   31},
    {   31,   22,  106,   30,   34,   33,   31},
    {   40,   28,   34,    0,   76,   46,   28},
    {   76,   21,   35,    0,   55,   35,   32},
    {   97,   32,   34,   21,   37,   33,   33},
    {   61,   75,   29,   30,   32,   32,   32},
    {   29,   92,   40,   29,   33,   32,   32},
    {   26,   62,   73,   29,   34,   32,   31},
    {   29,   32,   99,   30,   34,   33,   30},
    {   31,   22,  107,   30,   34,   33,   31},
    {   42,   27,   34,    1,   48,   79,   25},
    {   80,   20,   35,    0,   48,   47,   31},
    {   94,   36,   32,   17,   40,   33,   33},
    {   55,   80,   29,   27,   35,   31,   32},
    {   27,   90,   43,   28,   34,   32,   31},
    {   26,   58,   76,   29,   33,   33,   30},
    {   29,   30,  101,   29,   34,   34,   30},
    {   31,   21,  108,   29,   35,   34,   30},
    {   44,   26,   34,    6,   30,   80,   40},
    {   81,   21,   35,    0,   41,   52,   35},
    {   90,   41,   31,   14,   41,   35,   33},
    {   51,   82,   29,   24,   37,   32,   32},
    {   27,   87,   47,   27,   35,   32,   31},
    {   26,   54,   79,   29,   34,   33,   30},
    {   29,   29,  102,   28,   34,   33,   30},
    {   31,   21,  108,   28,   35,   33,   31},
    {   47,   26,   34,    7,   34,   44,   75},
    {   80,   24,   34,    0,   41,   41,   50},
    {   84,   45,   31,   12,   40,   36,   36},
    {   49,   81,   31,   22,   37,   33,   32},
    {   28,   81,   51,   26,   35,   33,   31},
    {   28,   51,   81,   28,   34,   33,   30},
    {   29,   30,  101,   28,   35,   33,   31},
    {   31,   22,  107,   28,   35,   33,   32},
    {   48,   27,   34,   10,   40,   16,   97},
    {   75,   27,   34,    3,   42,   26,   66},
    {   77,   47,   33,   12,   40,   32,   43},
    {   49,   75,   36,   21,   37,   33,   35},
    {   32,   72,   55,   25,   36,   33,   32},
    {   30,   49,   81,   27,   35,   33,   31},
    {   30,   32,   98,   28,   35,   32,   32},
    {   31,   24,  104,   28,   35,   32,   33}
  },
  {
    {   36,   29,   33,   43,   47,   29,   31},
    {   74,   20,   35,   19,   47,   34,   32},
    {   92,   35,   32,   29,   31,   40,   34},
    {   53,   80,   26,   33,   28,   36,   37},
    {   24,   91,   41,   31,   31,   31,   38},
    {   25,   57,   74,   31,   32,   30,   37},
    {   32,   28,   99,   32,   32,   29,   36},
    {   34,   20,  105,   33,   32,   30,   35},
    {   50,   26,   34,   33,   74,   30,   31},
    {   75,   28,   33,   23,   46,   47,   33},
    {   64,   58,   29,   30,   26,   46,   40},
    {   31,   85,   37,   31,   27,   33,   44},
    {   22,   67,   64,   30,   31,   28,   42},
    {   29,   35,   93,   31,   32,   27,   40},
    {   33,   20,  105,   32,   33,   27,   37},
    {   34,   19,  106,   33,   32,   29,   36},
    {   51,   29,   33,   25,   72,   51,   30},
    {   61,   42,   31,   30,   31,   60,   39},
    {   40,   70,   34,   32,   24,   41,   50},
    {   22,   72,   54,   30,   31,   27,   50},
    {   25,   44,   83,   30,   33,   25,   44},
    {   32,   23,  102,   32,   33,   26,   40},
    {   34,   18,  107,   32,   33,   28,   37},
    {   34,   19,  105,   33,   32,   30,   35},
    {   45,   35,   32,   30,   39,   79,   33},
    {   43,   53,   33,   35,   24,   53,   55},
    {   27,   67,   45,   32,   29,   27,   61},
    {   22,   53,   72,   30,   33,   22,   52},
    {   28,   31,   95,   31,   33,   25,   43},
    {   32,   20,  105,   32,   33,   27,   38},
    {   34,   18,  107,   32,   32,   29,   36},
    {   34,   20,  105,   33,   31,   31,   35},
    {   38,   40,   32,   35,   23,   72,   54},
    {   31,   55,   39,   34,   29,   32,   73},
    {   22,   57,   60,   31,   35,   18,   64},
    {   25,   39,   86,   31,   35,   22,   49},
    {   30,   24,  101,   32,   33,   27,   40},
    {   33,   19,  106,   32,   32,   30,   36},
    {   34,   18,  107,   33,   31,   31,   35},
    {   34,   20,  104,   33,   31,   32,   34},
    {   33,   42,   35,   34,   28,   39,   82},
    {   26,   51,   50,   33,   34,   18,   80},
    {   23,   46,   74,   31,   35,   20,   59},
    {   27,   32,   93,   32,   34,   26,   44},
    {   31,   22,  103,   32,   32,   30,   37},
    {   33,   19,  106,   33,   31,   31,   35},
    {   34,   19,  106,   33,   31,   32,   34},
    {   35,   21,  103,   34,   31,   32,   34},
    {   29,   41,   41,   33,   34,   20,   92},
    {   24,   44,   62,   34,   35,   18,   73},
    {   24,   37,   83,   34,   33,   25,   52},
    {   28,   28,   97,   33,   32,   30,   40},
    {   32,   23,  103,   33,   31,   32,   36},
    {   34,   20,  105,   34,   30,   33,   34},
    {   35,   20,  104,   34,   30,   33,   33},
    {   35,   22,  102,   34,   30,   33,   34},
    {   27,   38,   51,   34,   34,   20,   86},
    {   26,   37,   71,   35,   34,   24,   64},
    {   27,   33,   87,   35,   32,   30,   47},
    {   30,   28,   96,   34,   31,   32,   39},
    {   32,   24,  100,   35,   30,   32,   36},
    {   34,   23,  101,   34,   30,   33,   34},
    {   35,   23,  101,   34,   30,   32,   34},
    {   34,   24,   99,   35,   30,   33,   34}
  },
  {
    {   39,   30,   31,   67,   33,   34,   31},
    {   72,   21,   32,   43,   39,   33,   31},
    {  100,   23,   32,   35,   39,   34,   31},
    {   75,   63,   24,   32,   38,   34,   32},
    {   32,   98,   26,   29,   37,   35,   32},
    {   22,   77,   55,   29,   36,   35,   31},
    {   31,   37,   90,   31,   35,   35,   32},
    {   35,   22,  100,   33,   33,   36,   33},
    {   47,   29,   32,   74,   54,   32,   31},
    {   71,   24,   32,   60,   50,   36,   30},
    {   86,   31,   30,   46,   48,   37,   30},
    {   65,   63,   25,   34,   46,   39,   30},
    {   33,   85,   32,   28,   43,   40,   30},
    {   26,   64,   60,   27,   39,   41,   30},
    {   33,   33,   87,   29,   35,   41,   31},
    {   37,   23,   93,   32,   33,   41,   32},
    {   41,   32,   32,   45,   84,   32,   32},
    {   55,   31,   32,   50,   70,   40,   30},
    {   62,   37,   31,   45,   61,   45,   29},
    {   53,   55,   31,   36,   55,   48,   29},
    {   38,   63,   40,   29,   48,   50,   28},
    {   34,   49,   60,   27,   43,   51,   29},
    {   38,   30,   78,   28,   38,   50,   31},
    {   40,   24,   83,   30,   36,   48,   33},
    {   35,   33,   33,   29,   75,   58,   29},
    {   39,   35,   33,   34,   68,   59,   29},
    {   41,   39,   34,   36,   61,   62,   29},
    {   41,   43,   37,   33,   54,   64,   28},
    {   41,   43,   45,   30,   48,   65,   29},
    {   42,   36,   56,   27,   44,   63,   30},
    {   42,   30,   65,   27,   41,   60,   33},
    {   42,   28,   68,   28,   37,   56,   36},
    {   33,   34,   33,   31,   42,   88,   30},
    {   31,   36,   34,   31,   44,   84,   31},
    {   31,   37,   35,   32,   43,   83,   31},
    {   35,   35,   39,   32,   40,   82,   31},
    {   40,   32,   44,   31,   38,   81,   31},
    {   44,   30,   48,   30,   37,   78,   33},
    {   44,   30,   52,   28,   37,   72,   36},
    {   43,   30,   55,   29,   35,   66,   40},
    {   32,   33,   33,   34,   25,   85,   48},
    {   30,   34,   34,   33,   25,   88,   44},
    {   30,   34,   36,   34,   25,   90,   41},
    {   33,   32,   38,   34,   25,   90,   40},
    {   38,   29,   41,   34,   26,   88,   40},
    {   42,   29,   41,   33,   27,   85,   41},
    {   43,   30,   42,   31,   28,   80,   43},
    {   42,   31,   45,   31,   30,   72,   47},
    {   32,   33,   33,   33,   26,   54,   79},
    {   31,   32,   34,   35,   20,   68,   68},
    {   32,   32,   35,   36,   17,   76,   62},
    {   34,   31,   36,   36,   17,   79,   59},
    {   37,   29,   37,   36,   18,   78,   58},
    {   39,   29,   37,   35,   20,   77,   58},
    {   41,   30,   37,   34,   22,   74,   58},
    {   40,   31,   40,   32,   26,   68,   59},
    {   33,   31,   34,   33,   29,   31,   98},
    {   34,   30,   34,   35,   23,   45,   88},
    {   34,   31,   34,   36,   20,   54,   82},
    {   35,   31,   34,   36,   18,   59,   78},
    {   36,   31,   34,   37,   19,   60,   76},
    {   38,   30,   34,   36,   20,   61,   74},
    {   39,   31,   35,   35,   22,   60,   73},
    {   39,   31,   37,   34,   24,   59,   71}
  },
  {
    {   30,   33,   32,   55,   32,   32,   32},
    {   47,   30,   31,   29,   36,   32,   32},
    {   81,   28,   32,   28,   34,   32,   32},
    {   85,   46,   29,   32,   32,   33,   32},
    {   54,   82,   26,   32,   32,   33,   32},
    {   30,   90,   38,   31,   32,   33,   32},
    {   30,   56,   73,   31,   33,   32,   32},
    {   37,   21,  102,   32,   32,   32,   32},
    {   33,   32,   31,   68,   39,   31,   31},
    {   38,   32,   31,   43,   34,   33,   31},
    {   63,   30,   31,   29,   34,   32,   32},
    {   82,   37,   30,   29,   33,   32,   32},
    {   71,   63,   27,   31,   32,   33,   32},
    {   44,   86,   30,   30,   33,   33,   32},
    {   33,   72,   55,   30,   32,   32,   31},
    {   37,   37,   86,   31,   32,   33,   31},
    {   34,   33,   32,   60,   61,   29,   32},
    {   36,   33,   31,   56,   38,   32,   31},
    {   51,   30,   31,   38,   33,   33,   32},
    {   75,   31,   31,   30,   33,   33,   32},
    {   80,   47,   29,   30,   32,   33,   31},
    {   60,   73,   27,   30,   33,   33,   31},
    {   41,   78,   41,   30,   33,   32,   31},
    {   38,   53,   68,   30,   32,   33,   31},
    {   33,   33,   32,   43,   77,   35,   30},
    {   35,   33,   31,   55,   54,   29,   32},
    {   43,   32,   31,   46,   39,   31,   32},
    {   64,   30,   31,   35,   34,   33,   32},
    {   79,   37,   30,   31,   32,   33,   31},
    {   73,   57,   28,   30,   32,   33,   31},
    {   54,   73,   33,   30,   32,   33,   31},
    {   43,   64,   52,   30,   32,   33,   31},
    {   33,   33,   32,   34,   68,   58,   28},
    {   34,   33,   31,   45,   70,   33,   31},
    {   38,   33,   31,   48,   52,   29,   32},
    {   54,   31,   31,   40,   39,   31,   32},
    {   73,   32,   31,   34,   34,   33,   31},
    {   77,   45,   29,   31,   32,   32,   32},
    {   65,   63,   30,   31,   31,   33,   31},
    {   51,   66,   42,   30,   32,   33,   31},
    {   33,   32,   32,   34,   44,   81,   31},
    {   34,   33,   31,   38,   66,   52,   28},
    {   36,   33,   30,   44,   62,   34,   31},
    {   47,   31,   31,   43,   48,   30,   32},
    {   64,   31,   31,   38,   38,   32,   32},
    {   75,   38,   30,   33,   34,   32,   32},
    {   71,   53,   30,   31,   32,   33,   32},
    {   59,   61,   37,   30,   32,   33,   32},
    {   33,   32,   31,   35,   31,   71,   54},
    {   34,   33,   31,   37,   49,   70,   33},
    {   36,   33,   31,   41,   60,   48,   30},
    {   43,   32,   31,   43,   54,   35,   31},
    {   56,   31,   31,   40,   44,   32,   32},
    {   68,   35,   30,   36,   37,   32,   32},
    {   70,   45,   30,   33,   34,   33,   32},
    {   63,   55,   35,   31,   33,   33,   32},
    {   33,   32,   31,   33,   34,   36,   87},
    {   34,   32,   31,   36,   38,   62,   52},
    {   36,   33,   31,   39,   50,   57,   36},
    {   41,   33,   31,   41,   53,   43,   33},
    {   50,   33,   31,   41,   48,   36,   32},
    {   59,   35,   31,   37,   41,   34,   32},
    {   65,   42,   31,   35,   36,   33,   32},
    {   62,   49,   35,   33,   34,   34,   33}
  }
};
