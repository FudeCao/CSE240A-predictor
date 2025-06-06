//========================================================//
//  predictor.c                                           //
//  Source file for the Branch Predictor                  //
//                                                        //
//  Implement the various branch predictors below as      //
//  described in the README                               //
//========================================================//
#include <stdio.h>
#include "predictor.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
//
// TODO:Student Information
//
const char *studentName = "NAME";
const char *studentID   = "PID";
const char *email       = "EMAIL";

//------------------------------------//
//      Predictor Configuration       //
//------------------------------------//

// Handy Global for use in output routines
const char *bpName[4] = { "Static", "Gshare",
                          "Tournament", "Custom" };

int ghistoryBits; // Number of bits used for Global History
int lhistoryBits; // Number of bits used for Local History
int pcIndexBits;  // Number of bits used for PC index
int bpType;       // Branch Prediction Type
int verbose;

//------------------------------------//
//      Predictor Data Structures     //
//------------------------------------//

//
//TODO: Add your own Branch Predictor data structures here
//
#define COUNTER_MAX 3
#define COUNTER_INIT 2

uint32_t ghr = 0;
uint8_t *gshareTable;

uint32_t *localHistoryTable;
uint8_t  *localPredictionTable;
uint8_t  *globalPredictionTable;
uint8_t  *selectorTable;

uint32_t globalHistory = 0;

#define PERCEPTRON_TABLE_SIZE 1024
#define HISTORY_LENGTH 28
#define WEIGHT_MAX 127
#define WEIGHT_MIN -128
#define THRESHOLD (1.93 * HISTORY_LENGTH + 14)
int8_t history[HISTORY_LENGTH];
int8_t perceptronTable[PERCEPTRON_TABLE_SIZE][HISTORY_LENGTH + 1];

int dot_product(int8_t *weights) {
    int sum = weights[0]; 
    for (int i = 0; i < HISTORY_LENGTH; i++) {
        sum += weights[i + 1] * (history[i] ? 1 : -1);
    }
    return sum;
}

//------------------------------------//
//        Predictor Functions         //
//------------------------------------//

// Initialize the predictor
//
void
init_predictor()
{
  //
  //TODO: Initialize Branch Predictor Data Structures
  //
  if (bpType==GSHARE){
    uint32_t tableSize = 1 << ghistoryBits;
    gshareTable = (uint8_t *)malloc(tableSize * sizeof(uint8_t));
    for (uint32_t i = 0; i < tableSize; i++) {
      gshareTable[i] = COUNTER_INIT;
    }
    ghr = 0;
  } 
  
  else if (bpType == TOURNAMENT) {
      uint32_t lhtSize = 1 << pcIndexBits;
      uint32_t lptSize = 1 << lhistoryBits;
      uint32_t gptSize = 1 << ghistoryBits;

      localHistoryTable = (uint32_t *)malloc(lhtSize * sizeof(uint32_t));
      localPredictionTable = (uint8_t *)malloc(lptSize * sizeof(uint8_t));
      globalPredictionTable = (uint8_t *)malloc(gptSize * sizeof(uint8_t));
      selectorTable = (uint8_t *)malloc(gptSize * sizeof(uint8_t));

      for (uint32_t i = 0; i < lhtSize; i++) localHistoryTable[i] = 0;
      for (uint32_t i = 0; i < lptSize; i++) localPredictionTable[i] = COUNTER_INIT;
      for (uint32_t i = 0; i < gptSize; i++) globalPredictionTable[i] = COUNTER_INIT;
      for (uint32_t i = 0; i < gptSize; i++) selectorTable[i] = COUNTER_INIT;

      globalHistory = 0;
  } else if (bpType==CUSTOM){
    memset(history, 0, sizeof(history));
    memset(perceptronTable, 0, sizeof(perceptronTable));
  }
}

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken
//
uint8_t
make_prediction(uint32_t pc)
{
  //
  //TODO: Implement prediction scheme
  //

  // Make a prediction based on the bpType
  switch (bpType) {
    case STATIC:
      return TAKEN;
    case GSHARE:{
      uint32_t index = (pc ^ ghr) & ((1 << ghistoryBits) - 1);
      return gshareTable[index] >= 2 ? TAKEN : NOTTAKEN;
    }
    case TOURNAMENT:{
      uint32_t pcIndex = pc & ((1 << pcIndexBits) - 1);
      uint32_t localHist = localHistoryTable[pcIndex] & ((1 << lhistoryBits) - 1);
      uint32_t globalIdx = globalHistory & ((1 << ghistoryBits) - 1);

      int localPred = localPredictionTable[localHist] >= 2 ? TAKEN : NOTTAKEN;
      int globalPred = globalPredictionTable[globalIdx] >= 2 ? TAKEN : NOTTAKEN;

      return selectorTable[globalIdx] >= 2 ? globalPred : localPred;
    }
    case CUSTOM:{
      uint32_t index = pc % PERCEPTRON_TABLE_SIZE;
      int sum = dot_product(perceptronTable[index]);
      return sum >= 0 ? TAKEN : NOTTAKEN;
    }
    default:
      break;
  }

  // If there is not a compatable bpType then return NOTTAKEN
  return NOTTAKEN;
}

// Train the predictor the last executed branch at PC 'pc' and with
// outcome 'outcome' (true indicates that the branch was taken, false
// indicates that the branch was not taken)
//
void
train_predictor(uint32_t pc, uint8_t outcome)
{
  //
  //TODO: Implement Predictor training
  //
  if (bpType == GSHARE) {
    uint32_t index = (pc ^ ghr) & ((1 << ghistoryBits) - 1);

    if (outcome == TAKEN && gshareTable[index] < COUNTER_MAX)
      gshareTable[index]++;
    else if (outcome == NOTTAKEN && gshareTable[index] > 0)
      gshareTable[index]--;

    ghr = ((ghr << 1) | outcome) & ((1 << ghistoryBits) - 1);
  } 
  
  else if (bpType == TOURNAMENT) {
    uint32_t pcIndex = pc & ((1 << pcIndexBits) - 1);
    uint32_t localHist = localHistoryTable[pcIndex] & ((1 << lhistoryBits) - 1);
    uint32_t globalIdx = globalHistory & ((1 << ghistoryBits) - 1);

    int localPred = localPredictionTable[localHist] >= 2 ? TAKEN : NOTTAKEN;
    int globalPred = globalPredictionTable[globalIdx] >= 2 ? TAKEN : NOTTAKEN;

    if (outcome == TAKEN && localPredictionTable[localHist] < COUNTER_MAX)
      localPredictionTable[localHist]++;
    else if (outcome == NOTTAKEN && localPredictionTable[localHist] > 0)
      localPredictionTable[localHist]--;

    if (outcome == TAKEN && globalPredictionTable[globalIdx] < COUNTER_MAX)
      globalPredictionTable[globalIdx]++;
    else if (outcome == NOTTAKEN && globalPredictionTable[globalIdx] > 0)
      globalPredictionTable[globalIdx]--;

    if (localPred != globalPred) {
      if (globalPred == outcome && selectorTable[globalIdx] < COUNTER_MAX)
        selectorTable[globalIdx]++;
      else if (localPred == outcome && selectorTable[globalIdx] > 0)
        selectorTable[globalIdx]--;
    }

    localHistoryTable[pcIndex] = ((localHistoryTable[pcIndex] << 1) | outcome) & ((1 << lhistoryBits) - 1);

    globalHistory = ((globalHistory << 1) | outcome) & ((1 << ghistoryBits) - 1);
  } 
  
  else if (bpType==CUSTOM){
    uint32_t index = pc % PERCEPTRON_TABLE_SIZE;
    int8_t *weights = perceptronTable[index];
    int y = outcome ? 1 : -1;

    int sum = dot_product(weights);

    if ((sum >= 0) != outcome || abs(sum) <= THRESHOLD) {
        weights[0] += y;
        weights[0] = weights[0] > WEIGHT_MAX ? WEIGHT_MAX : (weights[0] < WEIGHT_MIN ? WEIGHT_MIN : weights[0]);

        for (int i = 0; i < HISTORY_LENGTH; i++) {
            int x = history[i] ? 1 : -1;
            weights[i + 1] += y * x;
            if (weights[i + 1] > WEIGHT_MAX) weights[i + 1] = WEIGHT_MAX;
            if (weights[i + 1] < WEIGHT_MIN) weights[i + 1] = WEIGHT_MIN;
        }
    }

    memmove(&history[1], &history[0], (HISTORY_LENGTH - 1) * sizeof(int8_t));
    history[0] = outcome ? 1 : 0;
  }
}
