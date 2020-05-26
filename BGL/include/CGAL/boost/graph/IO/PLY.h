// Copyright (c) 2015  GeometryFactory (France).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL$
// $Id$
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s)     : Andreas Fabri
//                 Mael Rouxel-Labbé

#ifndef CGAL_BGL_IO_PLY_H
#define CGAL_BGL_IO_PLY_H

#include <CGAL/IO/PLY.h>
#include <CGAL/boost/graph/IO/Generic_facegraph_builder.h>

#include <CGAL/boost/graph/Named_function_parameters.h>
#include <CGAL/boost/graph/named_params_helper.h>

#include <fstream>
#include <string>

namespace CGAL {

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/// Read

namespace IO {
namespace internal {

// Use CRTP to gain access to the protected members without getters/setters.
template <typename FaceGraph, typename Point>
class PLY_builder
  : public Generic_facegraph_builder<FaceGraph, Point, PLY_builder<FaceGraph, Point> >
{
  typedef PLY_builder<FaceGraph, Point>                                         Self;
  typedef Generic_facegraph_builder<FaceGraph, Point, Self>                     Base;

  typedef typename Base::Point_container                                        Point_container;
  typedef typename Base::Face                                                   Face;
  typedef typename Base::Face_container                                         Face_container;

public:
  PLY_builder(std::istream& is_, bool verbose) : Base(is_, verbose) { }

  template <typename NamedParameters>
  bool read(std::istream& input,
            Point_container& points,
            Face_container& faces,
            const NamedParameters& np,
            bool verbose)
  {
    return read_PLY(input, points, faces, np, verbose);
  }
};

template <typename FaceGraph, typename CGAL_BGL_NP_TEMPLATE_PARAMETERS>
bool read_PLY_BGL(std::istream& in,
                  FaceGraph& g,
                  const CGAL_BGL_NP_CLASS& np,
                  bool verbose = true)
{
  typedef typename CGAL::GetVertexPointMap<FaceGraph, CGAL_BGL_NP_CLASS>::type  VPM;
  typedef typename boost::property_traits<VPM>::value_type                      Point;

  IO::internal::PLY_builder<FaceGraph, Point> builder(in, verbose);
  return builder(g, np);
}

} // namespace internal
} // namespace IO

/*!
  \ingroup PkgBGLIOFct

  reads the graph `g` from data in the PLY format.

  \tparam FaceGraph a model of `MutableFaceGraph`
  \tparam NamedParameters a sequence of \ref bgl_namedparameters "Named Parameters"

  \param in the input stream
  \param g the graph to be built from the input data
  \param verbose whether extra information is printed when an incident occurs during reading
  \param np optional \ref bgl_namedparameters "Named Parameters" described below

  \cgalNamedParamsBegin
    \cgalParamBegin{vertex_point_map}
      the property map with the points associated to the vertices of `g` .
      If this parameter is omitted, an internal property map for
      `CGAL::vertex_point_t` should be available in `FaceGraph`
    \cgalParamEnd
    \cgalParamBegin{vertex_index_map}
      is a property map containing for each vertex of `g` a unique index between `0` and `num_vertices(g)-1`.
    \cgalParamEnd
    \cgalParamBegin{face_color_map} the property map with the colors associated to the faces of `g`.\cgalParamEnd
    \cgalParamBegin{vertex_color_map} the property map with the colors associated to the vertices of `g`.\cgalParamEnd
  \cgalNamedParamsEnd

  \pre The data must represent a 2-manifold

  \sa Overloads of this function for specific models of the concept `FaceGraph`.

  \see \ref IOStreamPLY
*/
template <typename FaceGraph, typename CGAL_BGL_NP_TEMPLATE_PARAMETERS>
bool read_PLY(std::istream& in, FaceGraph& g, const CGAL_BGL_NP_CLASS& np, bool verbose = true)
{
  return IO::internal::read_PLY_BGL(in, g, np, verbose);
}

/*!
  \ingroup PkgBGLIOFct

  reads the graph `g` from a file named `fname`, in the PLY format.

  \tparam FaceGraph a model of `MutableFaceGraph`
  \tparam NamedParameters a sequence of \ref bgl_namedparameters "Named Parameters"

  \param fname the name of the input file
  \param g the graph to be built from the input data
  \param verbose whether extra information is printed when an incident occurs during reading
  \param np optional \ref bgl_namedparameters "Named Parameters" described below

  \cgalNamedParamsBegin
    \cgalParamBegin{vertex_point_map}
      the property map with the points associated to the vertices of `g` .
      If this parameter is omitted, an internal property map for
      `CGAL::vertex_point_t` should be available in `FaceGraph`
    \cgalParamEnd
    \cgalParamBegin{vertex_index_map}
      is a property map containing for each vertex of `g` a unique index between `0` and `num_vertices(g)-1`.
    \cgalParamEnd
    \cgalParamBegin{face_color_map} the property map with the colors associated to the faces of `g`.\cgalParamEnd
    \cgalParamBegin{vertex_color_map} the property map with the colors associated to the vertices of `g`.\cgalParamEnd
  \cgalNamedParamsEnd

  \pre The data must represent a 2-manifold

  \sa Overloads of this function for specific models of the concept `FaceGraph`.

  \see \ref IOStreamPLY
*/
template <typename FaceGraph, typename CGAL_BGL_NP_TEMPLATE_PARAMETERS>
bool read_PLY(const char* fname, FaceGraph& g, const CGAL_BGL_NP_CLASS& np,
              bool verbose = true)
{
  std::ifstream is(fname);
  return IO::internal::read_PLY_BGL(is, g, np, verbose);
}

template <typename FaceGraph, typename CGAL_BGL_NP_TEMPLATE_PARAMETERS>
bool read_PLY(const std::string& fname, FaceGraph& g, const CGAL_BGL_NP_CLASS& np,
              bool verbose = true)
{
  return IO::internal::read_PLY(fname.c_str(), g, np, verbose);
}


template <typename FaceGraph>
bool read_PLY(std::istream& in, FaceGraph& g)
{
  return IO::internal::read_PLY_BGL(in, g, parameters::all_default());
}

template <typename FaceGraph>
bool read_PLY(const char* fname, FaceGraph& g)
{
  std::ifstream is(fname);
  return IO::internal::read_PLY_BGL(is, g, parameters::all_default());
}

template <typename FaceGraph>
bool read_PLY(const std::string& fname, FaceGraph& g)
{
  return IO::internal::read_PLY_BGL(fname.c_str(), g, parameters::all_default());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/// Write

/*!
 \ingroup PkgBGLIOFct

 writes the graph in an output stream in PLY format.

 \tparam FaceGraph a model of `FaceListGraph`
 \tparam NamedParameters a sequence of \ref bgl_namedparameters "Named Parameters"

 \param os the output stream
 \param g the graph to be output
 \param comments a string included line by line in the header of the PLY stream (each line will be precedeed by "comment ")
 \param np optional \ref bgl_namedparameters "Named Parameters" described below

 \cgalNamedParamsBegin
   \cgalParamBegin{vertex_point_map}
     the property map with the points associated to the vertices of `g` .
     If this parameter is omitted, an internal property map for
     `CGAL::vertex_point_t` should be available in `FaceGraph`
   \cgalParamEnd
   \cgalParamBegin{vertex_index_map}
     is a property map containing for each vertex of `g` a unique index between `0` and `num_vertices(g)-1`.
   \cgalParamEnd
   \cgalParamBegin{face_color_map} the property map with the colors associated to the faces of `g`.\cgalParamEnd
   \cgalParamBegin{vertex_color_map} the property map with the colors associated to the vertices of `g`.\cgalParamEnd
 \cgalNamedParamsEnd

 \returns `true` if writing was successful.

 \see \ref IOStreamPLY
*/
template <class FaceGraph, class NamedParameters>
bool write_PLY(std::ostream& os,
               const FaceGraph& g,
               const std::string comments,
               const NamedParameters& np)
{
  typedef typename boost::graph_traits<FaceGraph>::vertex_descriptor                          vertex_descriptor;
  typedef typename boost::graph_traits<FaceGraph>::halfedge_descriptor                        halfedge_descriptor;
  typedef typename boost::graph_traits<FaceGraph>::face_descriptor                            face_descriptor;

  typedef typename CGAL::GetInitializedVertexIndexMap<FaceGraph, NamedParameters>::const_type VIMap;
  typedef typename GetVertexPointMap<FaceGraph, NamedParameters>::const_type                  Vpm;
  typedef typename boost::property_traits<Vpm>::reference                                     Point_3;
  typedef CGAL::Color                                                                         Color;
  typedef typename internal_np::Lookup_named_param_def<
                     internal_np::vertex_color_map_t,
                     NamedParameters,
                     Constant_property_map<vertex_descriptor, Color> >::type                  VCM;
  typedef typename internal_np::Lookup_named_param_def<
                     internal_np::face_color_map_t,
                     NamedParameters,
                     Constant_property_map<face_descriptor, Color> >::type                    FCM;

  using parameters::choose_parameter;
  using parameters::is_default_parameter;
  using parameters::get_parameter;

  VCM vcm = choose_parameter(get_parameter(np, internal_np::vertex_color_map), VCM());
  FCM fcm = choose_parameter(get_parameter(np, internal_np::face_color_map), FCM());

  bool has_vcolor = !is_default_parameter(get_parameter(np, internal_np::vertex_color_map));
  bool has_fcolor = !is_default_parameter(get_parameter(np, internal_np::face_color_map));
  VIMap vim = CGAL::get_initialized_vertex_index_map(g, np);
  Vpm vpm = choose_parameter(get_parameter(np, internal_np::vertex_point),
                             get_const_property_map(boost::vertex_point, g));

  if(!os.good())
  {
    std::cerr << "Error: cannot open file" << std::endl;
    return false;
  }

  // Write header
  os << "ply" << std::endl
      << ((get_mode(os) == IO::BINARY) ? "format binary_little_endian 1.0" : "format ascii 1.0") << std::endl
      << "comment Generated by the CGAL library" << std::endl;

  if(comments != std::string())
  {
    std::istringstream iss(comments);
    std::string line;
    while(getline(iss, line))
    {
      if(line != "Generated by the CGAL library") // Avoid repeating the line if multiple savings
        os << "comment " << line << std::endl;
    }
  }

  os << "element vertex " << num_vertices(g) << std::endl;
  IO::internal::output_property_header(os, make_ply_point_writer (CGAL::Identity_property_map<Point_3>()));
  //if vcm is not default add v:color property
  if(has_vcolor)
  {
    os << "property uchar red" << std::endl
       << "property uchar green" << std::endl
       << "property uchar blue" << std::endl
       << "property uchar alpha" << std::endl;
  }

  os << "element face " << num_faces(g) << std::endl;
  IO::internal::output_property_header(
        os, std::make_pair(CGAL::Identity_property_map<std::vector<std::size_t> >(),
                            PLY_property<std::vector<int> >("vertex_indices")));
  //if fcm is not default add f:color property
  if(has_fcolor)
  {
    os << "property uchar red" << std::endl
       << "property uchar green" << std::endl
       << "property uchar blue" << std::endl
       << "property uchar alpha" << std::endl;
  }
  os << "end_header" << std::endl;

  for(vertex_descriptor vd : vertices(g))
  {
    Point_3 p = get(vpm, vd);
    IO::internal::output_properties(os, &p, make_ply_point_writer (CGAL::Identity_property_map<Point_3>()));
    if(has_vcolor)
    {
      const CGAL::Color& c = get(vcm, vd);
      if(get_mode(os) == CGAL::IO::ASCII)
        os << c << std::endl;
      else
        os.write(reinterpret_cast<char*>(&c), sizeof(c));
    }
  }

  std::vector<std::size_t> polygon;
  for(face_descriptor fd : faces(g))
  {
    polygon.clear();
    for(halfedge_descriptor hd : halfedges_around_face(halfedge(fd, g), g))
      polygon.push_back(get(vim, target(hd,g)));

    IO::internal::output_properties(os, &polygon,
                                    std::make_pair(CGAL::Identity_property_map<std::vector<std::size_t> >(),
                                                   PLY_property<std::vector<int> >("vertex_indices")));
    if(has_fcolor)
    {
      const CGAL::Color& c = get(fcm, fd);
      if(get_mode(os) == CGAL::IO::ASCII)
        os << c << std::endl;
      else
        os.write(reinterpret_cast<char*>(&c), sizeof(c));
    }
  }

  return os.good();
}

template <class FaceGraph>
bool write_PLY(std::ostream& os,
               const FaceGraph& g,
               const std::string comments)
{
  return write_PLY(os, g, comments, parameters::all_default());
}

template <class FaceGraph, typename CGAL_BGL_NP_TEMPLATE_PARAMETERS>
bool write_PLY(std::ostream& os,
               const FaceGraph& g,
               const CGAL_BGL_NP_CLASS& np)
{
  return write_PLY(os, g, "", np);
}

template <class FaceGraph>
bool write_PLY(std::ostream& os,
               const FaceGraph& g)
{
  return write_PLY(os, g, "", parameters::all_default());
}


/*!
  \ingroup PkgBGLIOFct

  writes the graph in the output file `fname` in PLY format.

 \tparam FaceGraph a model of `FaceListGraph`
 \tparam NamedParameters a sequence of \ref bgl_namedparameters "Named Parameters"

 \param fname the name of the output file
 \param g the graph to be output
 \param comments a string included line by line in the header of the PLY stream (each line will be precedeed by "comment ")
 \param np optional \ref bgl_namedparameters "Named Parameters" described below

 \cgalNamedParamsBegin
   \cgalParamBegin{vertex_point_map}
     the property map with the points associated to the vertices of `g` .
     If this parameter is omitted, an internal property map for
     `CGAL::vertex_point_t` should be available in `FaceGraph`
   \cgalParamEnd
   \cgalParamBegin{vertex_index_map}
     is a property map containing for each vertex of `g` a unique index between `0` and `num_vertices(g)-1`.
   \cgalParamEnd
   \cgalParamBegin{face_color_map} the property map with the colors associated to the faces of `g`.\cgalParamEnd
   \cgalParamBegin{vertex_color_map} the property map with the colors associated to the vertices of `g`.\cgalParamEnd
 \cgalNamedParamsEnd

 \returns `true` if writing was successful.

 \see \ref IOStreamPLY
*/
template <class FaceGraph, class NamedParameters>
bool write_PLY(const char* fname,
               const FaceGraph& g,
               const std::string comments,
               const NamedParameters& np)
{
  std::ofstream os(fname);
  return write_PLY(os, g, comments, np);
}

template <class FaceGraph>
bool write_PLY(const char* fname,
               const FaceGraph& g,
               const std::string comments)
{
  return write_PLY(fname, g, comments, parameters::all_default());
}

template <class FaceGraph, typename CGAL_BGL_NP_TEMPLATE_PARAMETERS>
bool write_PLY(const char* fname,
               const FaceGraph& g,
               const CGAL_BGL_NP_CLASS& np)
{
  return write_PLY(fname, g, "", np);
}

template <class FaceGraph>
bool write_PLY(const char* fname,
               const FaceGraph& g)
{
  return write_PLY(fname, g, "", parameters::all_default());
}


template <class FaceGraph, class NamedParameters>
bool write_PLY(const std::string& fname,
               const FaceGraph& g,
               const std::string& comments,
               const NamedParameters& np
               )
{
  return write_PLY(fname.c_str(), g, comments, np);
}

template <class FaceGraph>
bool write_PLY(const std::string& fname,
               const FaceGraph& g,
               const std::string comments)
{
  return write_PLY(fname, g, comments, parameters::all_default());
}

template <class FaceGraph, typename CGAL_BGL_NP_TEMPLATE_PARAMETERS>
bool write_PLY(const std::string& fname,
               const FaceGraph& g,
               const CGAL_BGL_NP_CLASS& np)
{
  return write_PLY(fname, g, "", np);
}

template <class FaceGraph>
bool write_PLY(const std::string& fname,
               const FaceGraph& g)
{
  return write_PLY(fname, g, "", parameters::all_default());
}
} // namespace CGAL

#endif // CGAL_BGL_IO_PLY_H
