// __BEGIN_LICENSE__
//  Copyright (c) 2006-2013, United States Government as represented by the
//  Administrator of the National Aeronautics and Space Administration. All
//  rights reserved.
//
//  The NASA Vision Workbench is licensed under the Apache License,
//  Version 2.0 (the "License"); you may not use this file except in
//  compliance with the License. You may obtain a copy of the License at
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
// __END_LICENSE__


#ifndef __VW_CARTOGRAPHY_BRESENHAMLINE_H__
#define __VW_CARTOGRAPHY_BRESENHAMLINE_H__

#include <vw/Core/FundamentalTypes.h>
#include <vw/Math/Vector.h>

// TODO: Move this class up in to the main cartography folder

namespace vw {
namespace cartography {

  /// Class implementing Bresenham line tracing algorithm
  /// - See http://www.cs.helsinki.fi/group/goa/mallinnus/lines/bresenh.html
  /// - This class works somewhat like an iterator down the pixels of the line.
  class BresenhamLine {
    vw::int32 x0, y0, x1, y1; ///< Starting and stopping points of the line
    vw::int32 x, y; ///< Current location of the line
    bool steep;
    vw::int32 deltax, deltay, error, ystep;

    /// Perform precomputations to let us draw the line quickly later
    void setup() {
      steep = abs(y1-y0) > abs(x1-x0);
      if (steep) {
        std::swap(x0,y0);
        std::swap(x1,y1);
      }
      if ( x0 > x1 ) {
        std::swap(x0,x1);
        std::swap(y0,y1);
      }
      deltax = x1 - x0;
      deltay = abs(y1-y0);
      error = deltax / 2;
      ystep = y0 < y1 ? 1 : -1;
      x = x0; y = y0;
    }

  public:
    /// Construct with two points
    /// - The stop point is the point AFTER the last pixel on the line
    BresenhamLine( vw::Vector2i const& start, vw::Vector2i const& stop ) :
      x0(start[0]), y0(start[1]), x1(stop[0]), y1(stop[1]) {
      setup();
    }

    /// Construct with two x,y pairs
    /// - The stop point (mx1,my1) is the point AFTER the last pixel on the line
    BresenhamLine( vw::int32 mx0, vw::int32 my0, vw::int32 mx1, vw::int32 my1 ) :
      x0(mx0), y0(my0), x1(mx1), y1(my1) {
      setup();
    }

    /// Return the current location along the line
    vw::Vector2i operator*() const {
      if (steep)
        return vw::Vector2i(y,x);
      else
        return vw::Vector2i(x,y);
    }

    /// Advance to the next pixel along the line
    void operator++() {
      x++;
      error -= deltay;
      if ( error < 0 ) {
        y     += ystep;
        error += deltax;
      }
    }

    /// Return true if we have not gone past the end of the line
    bool is_good() const { return x < x1; }
  };

}}

#endif//__VW_CARTOGRAPHY_BRESENHAMLINE_H__
