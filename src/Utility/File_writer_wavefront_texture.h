// Copyright (c) 1997
// Utrecht University (The Netherlands),
// ETH Zurich (Switzerland),
// INRIA Sophia-Antipolis (France),
// Max-Planck-Institute Saarbruecken (Germany),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org);
//
// $URL: https://github.com/CGAL/cgal/blob/v5.5.1/Stream_support/include/CGAL/IO/OBJ/File_writer_wavefront.h $
// $Id: File_writer_wavefront.h 4e519a3 2021-05-05T13:15:37+02:00 Sébastien Loriot
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Lutz Kettner  <kettner@mpi-sb.mpg.de>

#include <CGAL/IO/io.h>

#include <fstream>
#include <iostream>

class File_writer_wavefront_texture
{
  std::ostream* m_os;
  std::size_t m_facets;

public:
  std::ostream& out() const { return *m_os; }

  void write_header(std::ostream& o,
                    std::size_t vertices,
                    std::size_t halfedges,
                    std::size_t facets,
                    const bool /*colors*/ = false,
                    const bool /*normals*/ = false,
                    const bool /*textures*/ = false)
  {
    m_os = &o;
    m_facets = facets;

    // Print header.
    out() << "# file written from a CGAL tool in Wavefront obj format\n";
    out() << "# " << vertices << " vertices\n";
    out() << "# " << halfedges << " halfedges\n";
    out() << "# " << facets << " facets\n\n";

    out() << "\n# " << vertices << " vertices\n";
    out() << "# ------------------------------------------\n\n";
  }

  void write_mtllib(std::string materialFileName){
    out() << "mtllib " << materialFileName << "\n";
  }

  void write_usemtl(){
    out() << "usemtl Material\n";
  }

  void write_footer() const { out() << "\n# End of Wavefront obj format #" << std::endl; }

  void write_vertex(const double x, const double y, const double z) {
    out() << "v " << CGAL::IO::oformat(x) << ' ' << CGAL::IO::oformat(y) << ' ' << CGAL::IO::oformat(z) << '\n';
  }

  void write_vertex_normal(const double x, const double y, const double z) {
    out() << "vn " << CGAL::IO::oformat(x) << ' ' << CGAL::IO::oformat(y) << ' ' << CGAL::IO::oformat(z) << '\n';
  }

  void write_vertex_color(const double, const double, const double) { }
  void write_vertex_texture(const double x, const double y) { 
    out() << "vt " << CGAL::IO::oformat(x) << ' ' << CGAL::IO::oformat(y) << '\n';
  }

  void write_facet_header()
  {
    out() << "\n# " << m_facets << " facets\n";
    out() << "# ------------------------------------------\n\n";
  }
  void write_facet_begin(std::size_t) { out() << "f "; }
  void write_facet_vertex_index(std::size_t idx) { out() << ' ' << idx+1 << '/' << idx+1;}
  void write_face_color(const double, const double, const double) { }
  void write_facet_end() { out() << '\n'; }
};