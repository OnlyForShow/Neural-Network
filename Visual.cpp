#include "Visual.h"
#include <sstream>
#include <iomanip>
#include <functional>
#include <iostream>


Visual::Visual(int width, int height, const char * title, NN& neural)
: network(neural)
{
	screenWidth = width;
	screenHeight= height;

		
	plot_window = {0.0, 0.0, screenWidth * 0.5, screenHeight * 0.5};

	network_window = {0.0,screenHeight * 0.5, screenWidth, screenHeight * 0.5};

	test_window = {screenWidth * 0.5, 0.0, screenWidth * 0.5, screenHeight * 0.5};

	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(screenWidth, screenHeight, title);
	

	SetTargetFPS(120);
		
	
	drawingArea = {plot_window.x + x_achse_abstand, plot_window.y + y_achse_abstand+30 , plot_window.width-15, plot_window.height-10};


}	


void Visual::soft_reset()
{
	error_points.clear();
	max_cost = 0.0;
	network.reset();	
	currentState = State::RESET;
}

void Visual::reset()
{
	error_points.clear();
	test_points.clear();
	max_cost = 0.0;
	network.reset();	
	currentState = State::RESET;
}
	
void Visual::drawCoordinateSystem()
{

	DrawLineEx({x_achse_abstand, y_achse_abstand}, {x_achse_abstand, plot_window.height - y_achse_abstand},2.0 ,BLACK); 
	DrawLineEx({x_achse_abstand, plot_window.height - y_achse_abstand}, {plot_window.width - x_achse_abstand , plot_window.height - y_achse_abstand }, 2.0 ,BLACK); 
	DrawTriangle({x_achse_abstand, y_achse_abstand - 5}, {x_achse_abstand - 7, y_achse_abstand + 20} ,{x_achse_abstand+7, y_achse_abstand + 20}, BLACK);
	DrawTriangle({plot_window.width - x_achse_abstand + 5, plot_window.height - y_achse_abstand }, {plot_window.width - x_achse_abstand - 20 , plot_window.height - y_achse_abstand - 7 },{plot_window.width - x_achse_abstand -20 , plot_window.height - y_achse_abstand +7}, BLACK );

	int abstand = 8;
	for(int i = 0; i < abstand; i++) 		
	{
		int y = drawingArea.y + (drawingArea.height - drawingArea.y)*(double(i)/double(abstand));

		
		std::stringstream stream; 
		stream<<std::fixed<<(max_cost/double(i+1)) << std::endl;
		DrawText(stream.str().c_str(),15,y,15,WHITE);

		DrawLineEx({x_achse_abstand - 4, y},{x_achse_abstand + 4,y} , 2.0,BLACK);	
	}
}

Vector2 Visual::scale(double value, size_t position, size_t max_pos)
{
	double value_ratio = value/max_cost;
	double pos_ratio = double(position)/double(max_pos);	

	int x = drawingArea.x + (drawingArea.width - drawingArea.x)*pos_ratio;
	int y = drawingArea.height + (drawingArea.y - drawingArea.height)*value_ratio;

	Vector2 pos{x,y};

	return pos;

}

void Visual::plotFunction()
{
	if(error_points.empty())return;
	size_t segments = error_points.size();
	
	Vector2 p = scale(error_points[0], 0, segments - 1);
	Vector2 startPoint = {drawingArea.x, p.y};

	for(int i = 0 ; i < segments; i++)
	{
		Vector2 point = scale(error_points[i], i, segments - 1);
		DrawLineEx(startPoint, point, 2.0,RED);
		startPoint = point;
	}
		

}
		




void Visual::draw()
{
	
	BeginDrawing();
		ClearBackground(DARKGRAY);
		drawErrorCurve();
		drawNetwork();
		drawPointSelection();
	EndDrawing();	
}


void Visual::push_back(double value)
{
	if(value > max_cost)max_cost=value;
			
	static int counter = 0;
	counter++;
	error_points.push_back(value);
	if(error_points.size()>max_element)
	{
		error_points.pop_front();
		if(counter > max_element)
		{
			max_cost = error_points.front();
			counter = 0;
		}
	}	

}

void Visual::drawErrorCurve()
{
	
			
	if(error_points.empty())return;	

	std::stringstream stream; 
	stream <<"Avg Error: "<<error_points.back()<<std::endl;

	drawCoordinateSystem();
	plotFunction();

	DrawText(stream.str().c_str(),100,50,20,WHITE);



}

double xPos( size_t max, double x1, double x2, double r, double abstand)
{
	double middle = 0.5 * (x2 - x1) + x1;
	double L = abstand + 2.0 * r;
	
	double ret = middle - 0.5 * L * (max - 1);

	return ret;

}

double yPos( size_t max, double y1, double y2, double r, double abstand)
{
	double middle = 0.5 * (y2 - y1) + y1;
	double L = abstand + 2.0 * r;
	
	double ret = middle - 0.5 * L * (max - 1);

	return ret;

}

Color BiasColor(double value)
{
	Color c1 = BLUE;
	Color c2 = RED;
	double t = sigmoid(value*100.0);

	unsigned char red = c1.r + ( c2.r - c1.r) * t;
	unsigned char green = c1.g + ( c2.g - c1.g) * t;
	unsigned char blue = c1.b + ( c2.b - c1.b) * t;

	return (Color){red,green,blue,255}; 
}


Color WeightColor(double value)
{
	Color c1 = BLUE;
	Color c2 = RED;
	double t = sigmoid(value*100.0);

	unsigned char red = c1.r + ( c2.r - c1.r) * t;
	unsigned char green = c1.g + ( c2.g - c1.g) * t;
	unsigned char blue = c1.b + ( c2.b - c1.b) * t;

	return (Color){red,green,blue,255}; 
}

void Visual::drawNetwork()
{
	using namespace std::placeholders;	
	//graphics settings
	int neuron_radius = 10;
	int neuron_distance = 10;
	int layer_distance = 100;

		

	auto y_p = std::bind(yPos,_1,network_window.y, network_window.y + network_window.height, neuron_radius, neuron_distance);
	auto x_p = std::bind(xPos,_1, network_window.x, network_window.x + network_window.width, neuron_radius, layer_distance);

	double _x1 = x_p(network.getNumberOfLayer() + 1); 
	double dY = neuron_distance + 2 * neuron_radius;		
	double dX = layer_distance + 2 * neuron_radius;		
	double _x2 = _x1 + dX; 

	
	for(int i = 0; i < network.getNumberOfLayer(); i++)
	{
		size_t input = network.getLayer(i).input_parameter;
		size_t output = network.getLayer(i).output_parameter;
		matrix &weight = network.getLayer(i).getWeights();
		double y1 = y_p(input); 
		double x1 = _x1 + dX * i; 
		double y2 = y_p(output); 
		double x2 = _x2 + dX * i; 


		for(int j = 0; j < input; j++) 
		{
			for(int k = 0; k < output; k++) 
			{
				DrawLineEx({x1,y1 + dY * j},{x2, y2 + dY * k}, 3.0, WeightColor(weight[k][j]));	
			}
		}
	}

	size_t input = network.getLayer(0).input_parameter;
	double y1 = y_p(input);
	for(int j = 0; j < input; j++)
	{
		DrawCircle(_x1, y1 + dY * j, neuron_radius, BLACK);
		
	}


	for(int i = 0; i < network.getNumberOfLayer(); i++)	
	{
		size_t output = network.getLayer(i).output_parameter;
		vector bias = network.getLayer(i).getBias();
		double y2 = y_p(output); 
		double x2 = _x2 + dX * i; 

		for(int k = 0; k < output; k++)
		{
			DrawCircle(x2, y2 + dY * k, neuron_radius, BiasColor(bias[k]));
		}
	}	
}


Color getTypeColor(color_type t)
{
	switch(t)
	{
		case color_type::black:
			return BLACK;
		case color_type::white:
			return WHITE;
	}
	return WHITE;
}


void Visual::drawPointSelection()
{
	
	
	for(int y = 0; y < test_window.height; y+=test_window.height/100)
	{
		for(int x = 0; x < test_window.width; x+=test_window.width/100)
		{
			vector output = network.feed({x/test_window.width,y/test_window.height});
			double value = output[0];
			DrawRectangle(x+test_window.x, 
				      y+test_window.y, 
				      test_window.width/100,
				      test_window.height/100,
				      (Color){value*255,value*255,value*255,255}
				      );
		}
	}

	for(int i = 0; i < test_points.size(); i++) 
	{
		point p = test_points[i];
		DrawCircle(p.x + test_window.x ,p.y + test_window.y, 8, GRAY);
		DrawCircle(p.x + test_window.x ,p.y + test_window.y, 5, getTypeColor(p.v));
	}
}


training_set Visual::selectPoints()
{
	while(true)
	{
		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			Vector2 pos = GetMousePosition();
			test_points.push_back((point){pos.x - test_window.x, pos.y - test_window.y, color_type(0)});
		}else if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			Vector2 pos = GetMousePosition();
			
			test_points.push_back((point){pos.x - test_window.x, pos.y - test_window.y, color_type(1)});

		}

		if(IsKeyPressed(KEY_ENTER))
		{
			training_set set;
			std::vector<vector> input(test_points.size(), vector());
			std::vector<vector> output(test_points.size(), vector());
			set.emplace_back(std::move(input));
			set.emplace_back(std::move(output));

			for(size_t i = 0; i < test_points.size(); i++)
			{
				point p_test = test_points[i];
				set[0][i].push_back(p_test.x/test_window.width);
				set[0][i].push_back(p_test.y/test_window.height);
				set[1][i].push_back((double)p_test.v);
			}
			return set;
		}
		

		BeginDrawing();

			
			ClearBackground(DARKGRAY);
			drawErrorCurve();
			drawNetwork();
			drawPointSelection();

			int w_width = 400;
			int w_height = 100;
			DrawRectangle((screenWidth - w_width)/2 ,
				      (screenHeight - w_height)/2,
				      w_width,
				      w_height,
				      Fade(BLUE,0.6));
				      
			DrawText("Press ENTER to start the training!",
				 (screenWidth - w_width)/2 + 20,
				 (screenHeight - w_height)/2 + 40,
				 20,
				 Fade(WHITE,0.9)
				 );

						

		EndDrawing();

	}
}


bool Visual::resetLoop()
{
	while(true)
	{
		int w_width = 400;
		int w_height = 100;

		switch(currentState)
		{
			
			case State::RESET:
				if(IsKeyPressed(KEY_Z))
				{
					currentState = State::RESET_NETWORK;
					break;
				}

				if(IsKeyPressed(KEY_N))
				{
					return false;
				}

				BeginDrawing();
					DrawRectangle((screenWidth - w_width)/2 ,
						      (screenHeight - w_height)/2,
						      w_width,
						      w_height,
						      Fade(BLUE,0.6));
						      
					DrawText("Do you want a new run ? (Y/N)",
						 (screenWidth - w_width)/2 + 50,
						 (screenHeight - w_height)/2 + 40,
						 20,
						 Fade(WHITE,0.9)
						 );

				EndDrawing();
				break;
			case State::RESET_NETWORK:
				if(IsKeyPressed(KEY_K))
				{
					soft_reset();
					return true;
				}

				if(IsKeyPressed(KEY_D))
				{
					currentState = State::RESET_ALL;
					break;
				}



				BeginDrawing();
					DrawRectangle((screenWidth - w_width)/2 ,
						      (screenHeight - w_height)/2,
						      w_width,
						      w_height,
						      Fade(BLUE,0.6));
						      
					DrawText("Do you want to (K)eep the ",
						 (screenWidth - w_width)/2 + 50,
						 (screenHeight - w_height)/2 + 40,
						 20,
						 Fade(WHITE,0.9)
						 );
					DrawText("data points or (D)iscard them? (K/D)",
						 (screenWidth - w_width)/2 + 50,
						 (screenHeight - w_height)/2 + 70,
						 20,
						 Fade(WHITE,0.9)
						 );


				EndDrawing();
				break;
			case State::RESET_ALL:
				if(IsKeyPressed(KEY_E))
				{
					reset();
					return true;
				}

				if(IsKeyPressed(KEY_Q))
				{
					return false;
				}



				BeginDrawing();
					DrawRectangle((screenWidth - w_width)/2 ,
						      (screenHeight - w_height)/2,
						      w_width,
						      w_height,
						      Fade(BLUE,0.6));
						      
					DrawText("Do you want to reset",
						 (screenWidth - w_width)/2 + 50,
						 (screenHeight - w_height)/2 + 40,
						 20,
						 Fade(WHITE,0.9)
						 );
					DrawText("(E)verthing or (Q)uit ? (E/Q)",
						 (screenWidth - w_width)/2 + 50,
						 (screenHeight - w_height)/2 + 70,
						 20,
						 Fade(WHITE,0.9)
						 );
				EndDrawing();
				break;
		}
			
	}
}

bool Visual::state()
{
	switch(currentState)
	{
		case State::INIT:
			currentState = State::RESET;
			return true;
		case State::RESET:
			return resetLoop();	
	}
}
