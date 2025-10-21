#pragma once

#include <queue>

#include "raylib.h"





class Visual
{
	public:

		void setup(int width, int height, const char * title);
		void reset();

		void drawErrorCurve(double value);
	private:

		
		void drawCoordinateSystem();
		Vector2 scale(double value, size_t position, size_t max_pos);
		void plotFunction();

		int WINDOW_WIDTH;
		int WINDOW_HEIGHT;
		int screenWidth;
		int screenHeight;
				

		std::deque<double> error_points;
		

		size_t max_element = 200;
		double max_cost = 0.0;	

		Rectangle drawingArea;
};


