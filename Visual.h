#pragma once

#include <queue>
#include <vector>

#include "raylib.h"
#include "Neural.h"

enum color_type
{
	black = 0,white = 1
};


struct point
{
	double x,y;	
	color_type v;	
};

enum State
{
	INIT,RESET,RESET_ALL, RESET_NETWORK
};



class Visual
{
	public:

		Visual(int width, int height, const char * title, NN& network);
		void reset();
		void soft_reset();

		void draw();
		void push_back(double value);
		training_set selectPoints();

		bool state();

	private:
		
		void drawErrorCurve();
			void drawCoordinateSystem();
			Vector2 scale(double value, size_t position, size_t max_pos);
			void plotFunction();

			int x_achse_abstand = 10;		
			int y_achse_abstand = 10;		
			size_t max_element = 200;
			double max_cost = 0.0;	
			Rectangle plot_window;
			Rectangle drawingArea;


			std::deque<double> error_points;


		void drawNetwork();

			Rectangle network_window;


		void drawPointSelection();

			std::vector<point> test_points;
			Rectangle test_window;



		int screenWidth;
		int screenHeight;
				

		NN& network;

		bool resetLoop();
			State currentState = State::INIT;
};


