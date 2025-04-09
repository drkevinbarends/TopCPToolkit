#include "Math/Vector4D.h"
#include "Math/Vector3D.h"
#include "Math/GenVector/Boost.h"

using ROOT::Math::PtEtaPhiMVector;
using ROOT::Math::XYZVector;
using ROOT::Math::Boost;

namespace SpinHelpers
{
  
  float cos_theta_helicity(int sign, PtEtaPhiMVector top, PtEtaPhiMVector parent, const PtEtaPhiMVector &ttbar, PtEtaPhiMVector analyser)
  {

    Boost boost_to_ttbar = Boost(ttbar.BoostToCM());

    parent = boost_to_ttbar * parent;
    top = boost_to_ttbar * top;
    analyser = boost_to_ttbar * analyser;

    Boost boost_to_parent = Boost(parent.BoostToCM());

    analyser = boost_to_parent * analyser;

    XYZVector k_vector = top.Vect().Unit();
    k_vector *= sign;
    float theta = analyser.Vect().Unit().Dot(k_vector);

    return theta;
  }

  float cos_theta_helicity_plus(PtEtaPhiMVector &top, PtEtaPhiMVector &tbar, const PtEtaPhiMVector &ttbar, PtEtaPhiMVector &top_analyser, PtEtaPhiMVector &tbar_analyser)
  {
    (void)tbar; // Marking tbar as unused
    (void)tbar_analyser; // Marking tbar_analyser as unused
    return cos_theta_helicity(1, top, top, ttbar, top_analyser);
  }

  float cos_theta_helicity_minus(PtEtaPhiMVector &top, PtEtaPhiMVector &tbar, const PtEtaPhiMVector &ttbar, PtEtaPhiMVector &top_analyser, PtEtaPhiMVector &tbar_analyser)
  {
    (void)top_analyser; // Marking top_analyser as unused
    return cos_theta_helicity(-1, top, tbar, ttbar, tbar_analyser);
  }

  float cos_theta_raxis(int sign, PtEtaPhiMVector top, PtEtaPhiMVector parent, const PtEtaPhiMVector &ttbar, PtEtaPhiMVector analyser)
  {

    Boost boost_to_ttbar = Boost(ttbar.BoostToCM());

    parent = boost_to_ttbar * parent;
    top = boost_to_ttbar * top;
    analyser = boost_to_ttbar * analyser;

    Boost boost_to_parent = Boost(parent.BoostToCM());

    analyser = boost_to_parent * analyser;

    XYZVector k_vector = top.Vect().Unit();
    XYZVector p_vector(0., 0., 1.);

    float y = p_vector.Dot(k_vector);
    float r = pow((1. - y * y), 0.5);

    XYZVector r_vector = (1. / r) * (p_vector - y * k_vector);
    if (sign > 0)
    {
      // a axis
      if (y > 0)
        r_vector *= 1.;
      if (y < 0)
        r_vector *= -1.;
    }
    else if (sign < 0)
    {
      // b axis
      if (y > 0)
        r_vector *= -1.;
      if (y < 0)
        r_vector *= 1.;
    }

    float theta = analyser.Vect().Unit().Dot(r_vector.Unit()); // explicitly take a unit r_vector to make sure .Mag()==1

    return theta;
  }

  float cos_theta_raxis_plus(PtEtaPhiMVector &top, PtEtaPhiMVector &tbar, const PtEtaPhiMVector &ttbar, PtEtaPhiMVector &top_analyser, PtEtaPhiMVector &tbar_analyser)
  {
    (void)tbar; // Marking tbar as unused
    (void)tbar_analyser; // Marking tbar_analyser as unused
    return cos_theta_raxis(1, top, top, ttbar, top_analyser);
  }

  float cos_theta_raxis_minus(PtEtaPhiMVector &top, PtEtaPhiMVector &tbar, const PtEtaPhiMVector &ttbar, PtEtaPhiMVector &top_analyser, PtEtaPhiMVector &tbar_analyser)
  {
    (void)top_analyser; // Marking top_analyser as unused
    return cos_theta_raxis(-1, top, tbar, ttbar, tbar_analyser);
  }

  float cos_theta_transverse(int sign, PtEtaPhiMVector top, PtEtaPhiMVector parent, const PtEtaPhiMVector &ttbar, PtEtaPhiMVector analyser)
  {

    Boost boost_to_ttbar = Boost(ttbar.BoostToCM());

    parent = boost_to_ttbar * parent;
    top = boost_to_ttbar * top;
    analyser = boost_to_ttbar * analyser;

    Boost boost_to_parent = Boost(parent.BoostToCM());

    analyser = boost_to_parent * analyser;

    XYZVector k_vector = top.Vect().Unit();
    XYZVector p_vector(0., 0., 1.);

    float y = p_vector.Dot(k_vector);
    float r = pow((1. - y * y), 0.5);

    XYZVector n_vector = (1. / r) * (p_vector.Cross(k_vector));
    if (sign > 0)
    {
      // a axis
      if (y > 0)
        n_vector *= 1.;
      if (y < 0)
        n_vector *= -1.;
    }
    else if (sign < 0)
    {
      // b axis
      if (y > 0)
        n_vector *= -1.;
      if (y < 0)
        n_vector *= 1.;
    }

    float theta = analyser.Vect().Unit().Dot(n_vector.Unit()); // explicitly take a unit n_vector to make sure .Mag()==1

    return theta;
  }

  float cos_theta_transverse_plus(PtEtaPhiMVector &top, PtEtaPhiMVector &tbar, const PtEtaPhiMVector &ttbar, PtEtaPhiMVector &top_analyser, PtEtaPhiMVector &tbar_analyser)
  {
    (void)tbar; // Marking tbar as unused
    (void)tbar_analyser; // Marking tbar_analyser as unused
    return cos_theta_transverse(1, top, top, ttbar, top_analyser);
  }

  float cos_theta_transverse_minus(PtEtaPhiMVector &top, PtEtaPhiMVector &tbar, const PtEtaPhiMVector &ttbar, PtEtaPhiMVector &top_analyser, PtEtaPhiMVector &tbar_analyser)
  {
    (void)top_analyser; // Marking top_analyser as unused
    return cos_theta_transverse(-1, top, tbar, ttbar, tbar_analyser);
  }

  float cos_phi(PtEtaPhiMVector top, PtEtaPhiMVector tbar, const PtEtaPhiMVector &ttbar, PtEtaPhiMVector top_analyser, PtEtaPhiMVector tbar_analyser)
  {

    Boost boost_to_ttbar = Boost(ttbar.BoostToCM());

    top = boost_to_ttbar * top;
    tbar = boost_to_ttbar * tbar;
    top_analyser = boost_to_ttbar * top_analyser;
    tbar_analyser = boost_to_ttbar * tbar_analyser;

    Boost boost_to_top = Boost(top.BoostToCM());

    top_analyser = boost_to_top * top_analyser;

    Boost boost_to_tbar = Boost(tbar.BoostToCM());

    tbar_analyser = boost_to_tbar * tbar_analyser;

    float theta = top_analyser.Vect().Unit().Dot(tbar_analyser.Vect().Unit());

    return theta;
  }

  float cos_theta_x(const PtEtaPhiMVector &ttbar, PtEtaPhiMVector parent, PtEtaPhiMVector analyser)
  {

    XYZVector x_vector(1., 0., 0.);

    Boost boost_to_ttbar = Boost(ttbar.BoostToCM());

    parent = boost_to_ttbar * parent;
    analyser = boost_to_ttbar * analyser;

    Boost boost_to_parent = Boost(parent.BoostToCM());

    analyser = boost_to_parent * analyser;

    float theta = analyser.Vect().Unit().Dot(x_vector);

    return theta;
  }

  float cos_theta_y(const PtEtaPhiMVector &ttbar, PtEtaPhiMVector parent, PtEtaPhiMVector analyser)
  {

    XYZVector y_vector(0., 1., 0.);

    Boost boost_to_ttbar = Boost(ttbar.BoostToCM());

    parent = boost_to_ttbar * parent;
    analyser = boost_to_ttbar * analyser;

    Boost boost_to_parent = Boost(parent.BoostToCM());

    analyser = boost_to_parent * analyser;

    float theta = analyser.Vect().Unit().Dot(y_vector);

    return theta;
  }

  float cos_theta_z(const PtEtaPhiMVector &ttbar, PtEtaPhiMVector parent, PtEtaPhiMVector analyser)
  {

    XYZVector z_vector(0., 0., 1.);

    Boost boost_to_ttbar = Boost(ttbar.BoostToCM());

    parent = boost_to_ttbar * parent;
    analyser = boost_to_ttbar * analyser;

    Boost boost_to_parent = Boost(parent.BoostToCM());

    analyser = boost_to_parent * analyser;

    float theta = analyser.Vect().Unit().Dot(z_vector);

    return theta;
  }


} // namespace SpinHelpers
