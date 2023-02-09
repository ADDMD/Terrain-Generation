// Copyright (c) 2015-2020  GeometryFactory (France).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL: https://github.com/CGAL/cgal/blob/v5.5.1/BGL/include/CGAL/boost/graph/IO/OBJ.h $
// $Id: OBJ.h 10b0af3 2022-01-13T14:43:34+01:00 Sébastien Loriot
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s)     : Andreas Fabri
//                 Mael Rouxel-Labbé

#include "./File_writer_wavefront_texture.h"
#include "./Generic_facegraph_printer_obj.h"

#include <CGAL/IO/OBJ.h>
#include <CGAL/boost/graph/IO/Generic_facegraph_builder.h>

#include <CGAL/assertions.h>
#include <CGAL/boost/graph/Euler_operations.h>
#include <CGAL/Named_function_parameters.h>
#include <CGAL/boost/graph/named_params_helper.h>

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename Graph,
          typename CGAL_NP_TEMPLATE_PARAMETERS>
bool write_OBJ_texture(std::ostream& os,
               const Graph& g,
               const CGAL_NP_CLASS& np = CGAL::parameters::default_values()
#ifndef DOXYGEN_RUNNING
               , typename boost::disable_if<CGAL::IO::internal::is_Point_set_or_Range_or_Iterator<Graph> >::type* = nullptr
#endif
               )
{
  Generic_facegraph_printer_obj<std::ostream, Graph, File_writer_wavefront_texture> printer(os);
  return printer(g, np);
}

template <typename Graph,
          typename CGAL_NP_TEMPLATE_PARAMETERS>
bool write_OBJ_texture(const std::string& fname,
               const Graph& g,
               const CGAL_NP_CLASS& np = CGAL::parameters::default_values()
#ifndef DOXYGEN_RUNNING
               , typename boost::disable_if<CGAL::IO::internal::is_Point_set_or_Range_or_Iterator<Graph> >::type* = nullptr
#endif
               )
{
  std::ofstream os(fname);
  CGAL::IO::set_mode(os, CGAL::IO::ASCII);
  return write_OBJ_texture(os, g, np);
}