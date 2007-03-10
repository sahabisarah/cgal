#include <CGAL/basic.h>
#include <utility>
#include <CGAL/Cartesian.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Algebraic_kernel_for_circles_2_2.h>
#include <CGAL/Lazy_circular_kernel_2.h>
#include <CGAL/intersections.h>
#include <CGAL/Circular_kernel_2.h>
#include <CGAL/Timer.h>
#include <CGAL/Arr_circular_arc_traits.h>
#include <CGAL/Arr_circular_arc_traits_tracer.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arr_naive_point_location.h>
#include <CGAL/Random.h>

typedef CGAL::Quotient<CGAL::MP_Float>                       NT1;
typedef CGAL::Cartesian<NT1>                                 Linear_k1;
typedef CGAL::Algebraic_kernel_for_circles_2_2<NT1>          Algebraic_k1;
typedef CGAL::Circular_kernel_2<Linear_k1, Algebraic_k1>     CK1_;

typedef CGAL::Interval_nt_advanced                           NT2;
typedef CGAL::Cartesian<NT2>                                 Linear_k2;
typedef CGAL::Algebraic_kernel_for_circles_2_2<NT2>          Algebraic_k2;
typedef CGAL::Circular_kernel_2<Linear_k2,Algebraic_k2>      CK2_;

typedef CGAL::Lazy_circular_kernel_2<CK1_,CK2_>                  Circular_k;

typedef Circular_k::Circular_arc_2                            Arc1;
typedef std::vector<Arc1>                                    ArcContainer;

typedef CK1_::Circular_arc_2                            Arc2;
typedef std::vector<Arc2>                                    ArcContainer2;

#ifndef CGAL_CURVED_KERNEL_DEBUG
typedef CGAL::Arr_circular_arc_traits<Circular_k>                  Traits;
typedef CGAL::Arr_circular_arc_traits<CK1_>                      Traits2;
#else
typedef CGAL::Arr_circular_arc_traits<Circular_k>                  Traits0;
typedef CGAL::Circular_arc_traits_tracer<Traits0>            Traits;

typedef CGAL::Arr_circular_arc_traits<CK1_>                      Traits02;
typedef CGAL::Circular_arc_traits_tracer<Traits02>           Traits2;
#endif

//typedef Traits::Point_2                             Point_2;
typedef Traits::Curve_2                             Conic_arc_2;
typedef CGAL::Arrangement_2<Traits>                 Pmwx;
typedef CGAL::Arr_naive_point_location<Pmwx>        Point_location;

//typedef Traits::Point_2                             Point_2_2;
typedef Traits2::Curve_2                             Conic_arc_2_2;
typedef CGAL::Arrangement_2<Traits2>                 Pmwx2;
typedef CGAL::Arr_naive_point_location<Pmwx2>        Point_location2;

typedef Traits2::X_monotone_curve_2                          X_monotone_curve_2_2;
typedef CK1_::Point_2                     Point_2_2;
typedef CK1_::Circle_2                    Circle_2_2;
typedef CK1_::Circular_arc_2              Circular_arc_2_2;

typedef Traits::X_monotone_curve_2            X_monotone_curve_2;
typedef Circular_k::Point_2                     Point_2;
typedef Circular_k::Circle_2                    Circle_2;
typedef Circular_k::Circular_arc_2              Circular_arc_2;

int main(){

  CGAL::Timer clck1,clck2;
  double t1,t2,t3,t4;
  CGAL::Random generatorOfgenerator;
  int random_seed = generatorOfgenerator.get_int(0, 123456);
  std::cout << "random_seed = " << random_seed << std::endl;
  CGAL::Random theRandom(random_seed);
  int random_max = 128;
  int random_min = -128;
  ArcContainer ac;
  ArcContainer2 ac2;
  int x;
  int y;
  for(int i = 0; i < 30; i++){
    x = theRandom.get_int(random_min,random_max);
    y = theRandom.get_int(random_min,random_max);
    ac.push_back( Circle_2( Point_2(x,y), x*x + y*y));
    ac2.push_back( Circle_2_2( Point_2_2(x,y), x*x + y*y));
  }

  Pmwx _pm;
  Point_location _pl(_pm);

  clck1.start();
  t1=clck1.time();
  for (ArcContainer::const_iterator it=ac.begin();
       it != ac.end(); ++it) {
    //insert(_pm,_pl,*it);
    insert_curve(_pm,*it,_pl);
      };
  t2=clck1.time();
  clck1.stop();


  Pmwx2 _pm2;
  Point_location2 _pl2(_pm2);
  clck2.start();
  t3=clck2.time();
  for (ArcContainer2::const_iterator it2=ac2.begin();
       it2 != ac2.end(); ++it2) {
    //insert(_pm2,_pl2,*it2);
    insert_curve(_pm2,*it2,_pl2);
      };
  t4=clck2.time();
  clck2.stop();

std::cout<<"Lazy Circular_k ="<<(t2-t1)<<std::endl;
std::cout<<"Exact Circular_k ="<<(t4-t3)<<std::endl;


  return 0;
};
