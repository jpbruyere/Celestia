// sensorgeometry.cpp
//
// Copyright (C) 2010, Celestia Development Team
// Original version by Chris Laurel <claurel@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#include "sensorgeometry.h"
#include "rendcontext.h"
#include "texmanager.h"
#include "astro.h"
#include "body.h"
#include <Eigen/Core>
#include <algorithm>
#include <cassert>

using namespace Eigen;
using namespace std;


SensorGeometry::SensorGeometry() :
    m_observer(NULL),
    m_target(NULL),
    m_range(0.0)
{
}


SensorGeometry::~SensorGeometry()
{
}


bool
SensorGeometry::pick(const Ray3d& /* r */, double& /* distance */) const
{
    return false;
}


void
SensorGeometry::render(RenderContext& rc, double tsec)
{
    if (m_target == NULL || m_observer == NULL)
    {
        return;
    }

    double jd = astro::secsToDays(tsec) + astro::J2000;

    UniversalCoord obsPos = m_observer->getPosition(jd);
    UniversalCoord targetPos = m_target->getPosition(jd);

    Vector3d pos = targetPos.offsetFromKm(obsPos);

    if (pos.norm() > m_range)
    {
        pos = pos.normalized() * m_range;
    }

    glDisable(GL_LIGHTING);

    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3dv(pos.data());
    glEnd();

    glEnable(GL_LIGHTING);
}


bool
SensorGeometry::isOpaque() const
{
    return false;
}


bool
SensorGeometry::isNormalized() const
{
    return true;
}
