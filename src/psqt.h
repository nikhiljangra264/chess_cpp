#ifndef PSQT_H
#define PSQT_H

#include "header.h"

namespace Eval {
static u16 PieceValue[] = {100, 320, 330, 500, 900, 20000, 100, 320, 330, 500, 900, 20000, 0};

// most valuable victim - least valuable aggressor
static u16 mvvlva[][6] = {{105, 104, 103, 102, 101, 100}, {205, 204, 203, 202, 201, 100},
                          {305, 304, 303, 302, 301, 300}, {405, 404, 403, 402, 401, 400},
                          {505, 504, 503, 502, 501, 500}, {605, 604, 603, 602, 601, 600}};

static constexpr int psqt_end[][8][8] = {
  // pawn white
  {{0, 0, 0, 0, 0, 0, 0, 0},
   {-15, 0, 0, -20, -20, 0, 0, -15},
   {5, 0, 0, 0, 0, 0, 5, 5},
   {20, 0, 0, 20, 20, 0, 0, 20},
   {20, 20, 20, 20, 20, 20, 20, 20},
   {50, 50, 50, 50, 50, 50, 50, 50},
   {100, 100, 100, 100, 100, 100, 100, 100},
   {0, 0, 0, 0, 0, 0, 0, 0}},
  // knight white
  {{-50, -40, -30, -30, -30, -30, -40, -50},
   {-40, -20, 0, 0, 0, 0, -20, -40},
   {-30, 0, 10, 15, 15, 10, 0, -30},
   {-30, 5, 15, 20, 20, 15, 5, -30},
   {-30, 0, 15, 20, 20, 15, 0, -30},
   {-30, 5, 10, 15, 15, 10, 5, -30},
   {-40, -20, 0, 5, 5, 0, -20, -40},
   {-50, -40, -30, -30, -30, -30, -40, -50}},
  // bihsop white
  {{-20, -10, -10, -10, -10, -10, -10, -20},
   {-10, 12, 0, 0, 0, 0, 12, -10},
   {-10, 10, 10, 10, 10, 10, 10, -10},
   {-10, 0, 10, 10, 10, 10, 0, -10},
   {-10, 5, 5, 10, 10, 5, 5, -10},
   {-10, 0, 5, 10, 10, 5, 0, -10},
   {-10, 0, 0, 0, 0, 0, 0, -10},
   {-20, -10, -10, -10, -10, -10, -10, -20}},
  // rook white
  {{0, 0, 0, 5, 5, 0, 0, 0},
   {-5, 0, 0, 0, 0, 0, 0, -5},
   {-5, 0, 0, 0, 0, 0, 0, -5},
   {-5, 0, 0, 0, 0, 0, 0, -5},
   {-5, 0, 0, 0, 0, 0, 0, -5},
   {-5, 0, 0, 0, 0, 0, 0, -5},
   {5, 10, 10, 10, 10, 10, 10, 5},
   {0, 0, 0, 0, 0, 0, 0, 0}},
  // queen white
  {{-20, -10, -10, -5, -5, -10, -10, -20},
   {-10, 0, 5, 0, 0, 0, 0, -10},
   {-10, 5, 5, 5, 5, 5, 0, -10},
   {0, 0, 5, 5, 5, 5, 0, -5},
   {-5, 0, 5, 5, 5, 5, 0, -5},
   {-10, 0, 5, 5, 5, 5, 0, -10},
   {-10, 0, 0, 0, 0, 0, 0, -10},
   {-20, -10, -10, -5, -5, -10, -10, -20}},
  // king white
  {{-50, -30, -30, -30, -30, -30, -30, -50},
   {-30, -30, 0, 0, 0, 0, -30, -30},
   {-30, -10, 20, 30, 30, 20, -10, -30},
   {-30, -10, 30, 40, 40, 30, -10, -30},
   {-30, -10, 30, 40, 40, 30, -10, -30},
   {-30, -10, 20, 30, 30, 20, -10, -30},
   {-30, -20, -10, 0, 0, -10, -20, -30},
   {-50, -40, -30, -20, -20, -30, -40, -50}},
  // pawn black
  {{0, 0, 0, 0, 0, 0, 0, 0},
   {-100, -100, -100, -100, -100, -100, -100, -100},
   {-50, -50, -50, -50, -50, -50, -50, -50},
   {-20, -20, -20, -20, -20, -20, -20, -20},
   {-20, 0, 0, -20, -20, 0, 0, -20},
   {-5, 0, 0, 0, 0, 0, -5, -5},
   {15, 0, 0, 20, 20, 0, 0, 15},
   {0, 0, 0, 0, 0, 0, 0, 0}},
  // knight black
  {{50, 40, 30, 30, 30, 30, 40, 50},
   {40, 20, 0, 0, 0, 0, 20, 40},
   {30, 0, -10, -15, -15, -10, 0, 30},
   {30, -5, -15, -20, -20, -15, -5, 30},
   {30, 0, -15, -20, -20, -15, 0, 30},
   {30, -5, -10, -15, -15, -10, -5, 30},
   {40, 20, 0, 5, 5, 0, 20, 40},
   {50, 40, 30, 30, 30, 30, 40, 50}},
  // bishop black
  {
    {20, 10, 10, 10, 10, 10, 10, 20},
    {10, 0, 0, 0, 0, 0, 0, 10},
    {10, 0, -5, -10, -10, -5, 0, 10},
    {10, -5, -5, -10, -10, -5, -5, 10},
    {10, 0, -10, -10, -10, -10, 0, 10},
    {10, -10, -10, -10, -10, -10, -10, 10},
    {10, -10, 0, 0, 0, 0, -10, 10},
    {20, 10, 10, 10, 10, 10, 10, 20},
  },
  // rook black
  {{0, 0, 0, 0, 0, 0, 0, 0},
   {-5, -10, -10, -10, -10, -10, -10, -5},
   {5, 0, 0, 0, 0, 0, 0, 5},
   {5, 0, 0, 0, 0, 0, 0, 5},
   {5, 0, 0, 0, 0, 0, 0, 5},
   {5, 0, 0, 0, 0, 0, 0, 5},
   {5, 0, 0, 0, 0, 0, 0, 5},
   {0, 0, 0, -5, -5, 0, 0, 0}},
  // queen black
  {{20, 10, 10, 5, 5, 10, 10, 20},
   {10, 0, 0, 0, 0, 0, 0, 10},
   {10, 0, -5, -5, -5, -5, 0, 10},
   {5, 0, -5, -5, -5, -5, 0, 5},
   {0, 0, -5, -5, -5, -5, 0, 5},
   {10, -5, -5, -5, -5, -5, 0, 10},
   {10, 0, -5, 0, 0, 0, 0, 10},
   {20, 10, 10, 5, 5, 10, 10, 20}},
  // king black
  {{50, 40, 30, 20, 20, 30, 40, 50},
   {30, 20, 10, 0, 0, 10, 20, 30},
   {30, 10, -20, -30, -30, -20, 10, 30},
   {30, 10, -30, -40, -40, -30, 10, 30},
   {30, 10, -30, -40, -40, -30, 10, 30},
   {30, 10, -20, -30, -30, -20, 10, 30},
   {30, 30, 0, 0, 0, 0, 30, 30},
   {50, 30, 30, 30, 30, 30, 30, 50}}};


static constexpr int psqt_middle[][8][8] = {
  // pawn white
  {{0, 0, 0, 0, 0, 0, 0, 0},
   {5, 10, 10, -20, -20, 10, 10, 5},
   {5, -5, -10, 0, 0, -10, -5, 5},
   {0, 0, 0, 20, 20, 0, 0, 0},
   {5, 5, 10, 25, 25, 10, 5, 5},
   {10, 10, 20, 30, 30, 20, 10, 10},
   {50, 50, 50, 50, 50, 50, 50, 50},
   {0, 0, 0, 0, 0, 0, 0, 0}},
  // knight white
  {{-50, -40, -30, -30, -30, -30, -40, -50},
   {-40, -20, 0, 0, 0, 0, -20, -40},
   {-30, 0, 10, 15, 15, 10, 0, -30},
   {-30, 5, 15, 20, 20, 15, 5, -30},
   {-30, 0, 15, 20, 20, 15, 0, -30},
   {-30, 5, 10, 15, 15, 10, 5, -30},
   {-40, -20, 0, 5, 5, 0, -20, -40},
   {-50, -40, -30, -30, -30, -30, -40, -50}},
  // bihsop white
  {{-20, -10, -10, -10, -10, -10, -10, -20},
   {-10, 12, 0, 0, 0, 0, 12, -10},
   {-10, 10, 10, 10, 10, 10, 10, -10},
   {-10, 0, 10, 10, 10, 10, 0, -10},
   {-10, 5, 5, 10, 10, 5, 5, -10},
   {-10, 0, 5, 10, 10, 5, 0, -10},
   {-10, 0, 0, 0, 0, 0, 0, -10},
   {-20, -10, -10, -10, -10, -10, -10, -20}},
  // rook white
  {{0, 0, 0, 5, 5, 0, 0, 0},
   {-5, 0, 0, 0, 0, 0, 0, -5},
   {-5, 0, 0, 0, 0, 0, 0, -5},
   {-5, 0, 0, 0, 0, 0, 0, -5},
   {-5, 0, 0, 0, 0, 0, 0, -5},
   {-5, 0, 0, 0, 0, 0, 0, -5},
   {5, 10, 10, 10, 10, 10, 10, 5},
   {0, 0, 0, 0, 0, 0, 0, 0}},
  // queen white
  {{-20, -10, -10, -5, -5, -10, -10, -20},
   {-10, 0, 5, 0, 0, 0, 0, -10},
   {-10, 5, 5, 5, 5, 5, 0, -10},
   {0, 0, 5, 5, 5, 5, 0, -5},
   {-5, 0, 5, 5, 5, 5, 0, -5},
   {-10, 0, 5, 5, 5, 5, 0, -10},
   {-10, 0, 0, 0, 0, 0, 0, -10},
   {-20, -10, -10, -5, -5, -10, -10, -20}},
  // king white
  {{20, 30, 10, 0, 0, 10, 30, 20},
   {20, 20, 0, 0, 0, 0, 20, 20},
   {-10, -20, -20, -20, -20, -20, -20, -10},
   {-20, -30, -30, -40, -40, -30, -30, -20},
   {-30, -40, -40, -50, -50, -40, -40, -30},
   {-30, -40, -40, -50, -50, -40, -40, -30},
   {-30, -40, -40, -50, -50, -40, -40, -30},
   {-30, -40, -40, -50, -50, -40, -40, -30}},
  // pawn black
  {{-0, -0, -0, -0, -0, -0, -0, -0},
   {-50, -50, -50, -50, -50, -50, -50, -50},
   {-10, -10, -20, -30, -30, -20, -10, -10},
   {-5, -5, -10, -25, -25, -10, -5, -5},
   {0, 0, 0, -20, -20, 0, 0, 0},
   {-5, 5, 10, 0, 0, 10, 5, -5},
   {-5, -10, -10, 20, 20, -10, -10, -5},
   {0, 0, 0, 0, 0, 0, 0, 0}},
  // knight black
  {{50, 40, 30, 30, 30, 30, 40, 50},
   {40, 20, 0, 0, 0, 0, 20, 40},
   {30, 0, -10, -15, -15, -10, 0, 30},
   {30, -5, -15, -20, -20, -15, -5, 30},
   {30, 0, -15, -20, -20, -15, 0, 30},
   {30, -5, -10, -15, -15, -10, -5, 30},
   {40, 20, 0, 5, 5, 0, 20, 40},
   {50, 40, 30, 30, 30, 30, 40, 50}},
  // bishop black
  {
    {20, 10, 10, 10, 10, 10, 10, 20},
    {10, 0, 0, 0, 0, 0, 0, 10},
    {10, 0, -5, -10, -10, -5, 0, 10},
    {10, -5, -5, -10, -10, -5, -5, 10},
    {10, 0, -10, -10, -10, -10, 0, 10},
    {10, -10, -10, -10, -10, -10, -10, 10},
    {10, -10, 0, 0, 0, 0, -10, 10},
    {20, 10, 10, 10, 10, 10, 10, 20},
  },
  // rook black
  {{0, 0, 0, 0, 0, 0, 0, 0},
   {-5, -10, -10, -10, -10, -10, -10, -5},
   {5, 0, 0, 0, 0, 0, 0, 5},
   {5, 0, 0, 0, 0, 0, 0, 5},
   {5, 0, 0, 0, 0, 0, 0, 5},
   {5, 0, 0, 0, 0, 0, 0, 5},
   {5, 0, 0, 0, 0, 0, 0, 5},
   {0, 0, 0, -5, -5, 0, 0, 0}},
  // queen black
  {{20, 10, 10, 5, 5, 10, 10, 20},
   {10, 0, 0, 0, 0, 0, 0, 10},
   {10, 0, -5, -5, -5, -5, 0, 10},
   {5, 0, -5, -5, -5, -5, 0, 5},
   {0, 0, -5, -5, -5, -5, 0, 5},
   {10, -5, -5, -5, -5, -5, 0, 10},
   {10, 0, -5, 0, 0, 0, 0, 10},
   {20, 10, 10, 5, 5, 10, 10, 20}},
  // king black
  {{-30, -40, -40, -50, -50, -40, -40, -30},
   {-30, -40, -40, -50, -50, -40, -40, -30},
   {-30, -40, -40, -50, -50, -40, -40, -30},
   {-30, -40, -40, -50, -50, -40, -40, -30},
   {-20, -30, -30, -40, -40, -30, -30, -20},
   {-10, -20, -20, -20, -20, -20, -20, -10},
   {20, 20, 0, 0, 0, 0, 20, 20},
   {20, 30, 10, 0, 0, 10, 30, 20}}};

}  // namespace Eval

#endif  // PSQT_H