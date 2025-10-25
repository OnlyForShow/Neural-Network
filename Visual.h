#pragma once

#include <queue>
#include <vector>

#include "raylib.h"
#include "Neural.h"

enum color_type
{
	black,white
};


struct point
{
	double x,y;	
	color_type v;	
};



class Visual
{
	public:

		Visual(int width, int height, const char * title, NN& network);
		void reset();

		void draw(double value);
	private:

		
		void drawErrorCurve(double value);
		void drawNetwork();
		void drawPointSelection();

		void drawCoordinateSystem();
		Vector2 scale(double value, size_t position, size_t max_pos);
		void plotFunction();



		Rectangle plot_window;
		Rectangle network_window;
		Rectangle test_window;


		int screenWidth;
		int screenHeight;
				

		std::deque<double> error_points;
		std::vector<point> test_points;
		

		size_t max_element = 200;
		double max_cost = 0.0;	

		int x_achse_abstand = 10;		
		int y_achse_abstand = 10;		
	
		Rectangle drawingArea;

		NN& network;
};


