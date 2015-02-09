#include <iostream>
#include <iomanip>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
#include "util.h"
#include "emsf.h"


using namespace std;
using namespace Eigen;
using namespace util;
using namespace emsf;


Natural card(vec &card_dist)
{
  Natural c = sample_from_dist(card_dist) + 1;
  if (c > 10) c = 10;

  return c;
}


void draw_card(Natural &xc, Natural &x_ace, vec &card_dist)
{
  Natural c = card(card_dist);

  xc += c;

  if (!x_ace && c == 1) {
    xc += 10;
    x_ace = 1;
  }

  if (x_ace && xc > 21) {
    xc -= 10;
    x_ace = 0;
  }
}


Natural transition(Natural &pc, Natural &p_ace, Natural &dc, Natural &d_ace, Natural &r, Natural a, bool &end, vec &card_dist)
{
  Natural stick = 0;
  Natural hit = 1;
  
  if (a == hit) {
    draw_card(pc, p_ace, card_dist);

    if (pc > 21) {
      r = -1;
      end = true;

      return 200;
    }
    else {
      end = false;
    }
  }
  else if (a == stick) {
    while (dc < 17)                                             /* TODO: checar se é < ou <= */
      draw_card(dc, d_ace, card_dist);

    if (dc > 21) {
      r = 1;
      end = true;
      return 202;
    }
    else {
      Natural p_diff = 21 - pc;
      Natural d_diff = 21 - dc;

      if (p_diff < d_diff) {
        r = 1;
        end = true;
        return 202;
      }
      else if (p_diff > d_diff) {
        r = -1;
        end = true;
        return 200;
      }
      else {
        r = 0;
        end = true;
        return 201;
      }
    }

    end = true;
  }
}


Natural get_s(Natural pc, Natural p_ace, Natural dc)
{
  return (p_ace * 100) + ((pc - 12) * 10) + (dc - 2);
}


inline Natural get_a(Natural s, mat &pi)
{
  return sample_from_dist(pi.row(s).transpose());
}


Natural episode(mat &pi, vec &card_dist, const bool save = false, std::vector<Natural> &yv, std::vector<Natural> &av, std::vector<Natural> &rv)
{
  Natural pc = 0, p_ace = 0;
  Natural dc = 0, d_ace = 0;
  Natural s, a, r;
  
  draw_card(pc, p_ace, card_dist);
  while (pc < 12)
    draw_card(pc, p_ace, card_dist);

  s = get_s(pc, p_ace, dc);
  yv.push_back(s);

  draw_card(dc, d_ace, card_dist);

  bool end = false;
  while (!end) {
    s = get_s(pc, p_ace, dc);

    a = get_a(s, pi);
    av.push_back(a);

    transition(pc, p_ace, dc, d_ace, r, a, end, card_dist);

    rv.push_back(r);
  }

  return r;
}


Real evaluation(Natural n_eval, mat &pi, vec &card_dist)
{
  Real E;

  Natural R = 0;
  for (Natural i = 0; i < n_eval; ++i)
    R += episode(pi, card_dist);

  E = (double) R / (double) n_eval;
  
  return E;
}


int main(int argc, char* argv[])
{
  Natural nargs = 3;
  if (argc != nargs) {
    cout << "Usage: blackjack eval_qty eval_size" << endl;
    exit(EXIT_FAILURE);
  }

  const Natural n = 200;
  const Natural na = 2;
  const Natural eval_qty = atoi(argv[1]);
  const Natural eval_size = atoi(argv[2]);

  srand(time(NULL));

  vec card_dist = generate_stochastic_matrix(1, 13, true).transpose();
  stoch_mat pi = generate_stochastic_matrix(n, na, true);

  for (Natural i = 0; i < eval_qty; ++i) {
    Real E = evaluation(eval_size, pi, card_dist);
    cout << E << std::endl;
  }
  
  return 0;
}
