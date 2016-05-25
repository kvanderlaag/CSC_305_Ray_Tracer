#pragma once

#include "Vector3.h"

/* Configuration settings

Changing these settings will adjust image and scene parameters.

*/

// Image dimensions. Will affect camera FOV and aspect ratio.
const int width = 400;
const int height = 300;

// Number of rendering threads. Adjust this to match the number of cores.
const int NUM_THREADS = 2;

// Number of bounces to compute.
const int BOUNCES = 10;

// Number of samples to compute.
const int SAMPLES = 16;

// Camera settings
const Vector3 lookfrom(-1, 4, 6); // Position of camera in world space.
const Vector3 lookat(0, 1.5, 1); // Point for camera to focus on in world space. Affects focus distance.
const Vector3 cameraup(0.4, 0.9, 0); // Up vector for camera in world space.
const double fov = 70; // Vertical field of view in degrees.
const double lensRadius = 0.1; // Lens radius in arbitrary units. Higher results in more blur.
const double dvignette = 0.9; // Factor by which to dim colors at the edge of the image. 1 = complete falloff, 0 = no falloff.
const double vignetteradius = 0.9; // Radius, as a fraction of the image diagnonal, on which to vignette.