#ifndef GL_HELPERS_H
#define GL_HELPERS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "country.h"

void glad_init();
GLFWwindow* glfw_create_window(int width, int height);
void process_input(GLFWwindow* window);

/*
 * OpenGL only processes coordinates between -1 and 1 in all directions. 
 * Computes necessary transformation to country coordinates so they can be displayed.
 * 
 * Outputs:
 *   s     scale
 *   x     translation in x direction
 *   y     translation in y direction
 */
void fit_to_window(Country* country1, Country* country2, float& s, float& x, float& y);

/*
 * Draw the country outlines in the specified colours. 
 * Each colour is an array containing the RGB values.
 * The i-th country will be drawn in the i-th colour.
 */
void draw(Country** countries, float** colours, int num_countries);

#endif