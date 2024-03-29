#ifndef EMSF_H
#define EMSF_H


#include "util.h"

using namespace util;

namespace emsf
{
  typedef mat stoch_mat;
  typedef std::vector<stoch_mat> v_stoch_mat;

  class model
  {
    public:
      v_stoch_mat P;
      vec mu;
      stoch_mat pi;
  };

  class data
  {
    public:
      vecn y;
      vecn a;
  };

  typedef std::vector<data> v_data;
  typedef std::vector<mat> v_mat;
}


#endif
