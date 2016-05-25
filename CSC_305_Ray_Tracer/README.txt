README.txt
CSC 305, A01, Summer 2016
Assignment 1: Ray Tracer
Author: Keegan van der Laag (jkvander@uvic.ca)

Description: Software ray tracer written in C++. Implements the following features:

- Computes intersection with and renders spheres, planes, and triangles
- Calculates diffuse, reflected, and refracted shading on all objects
- Illumination by multiple point light sources with configurable color and intensity
- Renders from a camera with configurable viewpoint (position, point to look at, up direction vector, and lens radius and focus distance for lens blur)
- Hard shadows cast by all objects from all light sources
- Renders multiple objects specified by an object list
- Renders a cube as an example of a triangle mesh
- Supersampling anti-aliasing (sample number configurable)
- Renders reflective (mirrored) and refractive (glass) surface materials (with some accuracy) with configurable color and reflectiveness values for reflective surfaces, and refraction index for refractive surfaces
- Multi-threaded rendering (number of threads configurable)

This should cover all minimum requirements, as well as most of the advanced requirements, plus some not listed.

Sample renderings are included, in .PNG and.HDR image formats.

To change camera and image size/quality parameters, modify the values in config.h.

One point light is generated at a fixed position, white in color, with a reasonably high intensity. One point light in a fixed position, with random color and intensity properties.