#include "Visual.h"




void Visual::setup(int width, int height, const char * title)
{
	WINDOW_HEIGHT = height;
	WINDOW_WIDTH = width;
	screenWidth = WINDOW_WIDTH;
	screenHeight= WINDOW_HEIGHT;
	

	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(screenWidth, screenHeight, title);
	

	SetTargetFPS(120);
	
	
	drawingArea = {10, 40 , WINDOW_WIDTH-15, WINDOW_HEIGHT-10};

}	


void Visual::reset()
{
	error_points.clear();
	max_cost = 0.0;
}
	
void Visual::drawCoordinateSystem()
{
	int x_achse_abstand = 10;		
	int y_achse_abstand = 10;		
	
	DrawLineEx({x_achse_abstand, y_achse_abstand}, {x_achse_abstand, WINDOW_HEIGHT - y_achse_abstand},2.0 ,BLACK); 
	DrawLineEx({x_achse_abstand, WINDOW_HEIGHT - y_achse_abstand}, {WINDOW_WIDTH - x_achse_abstand , WINDOW_HEIGHT - y_achse_abstand }, 2.0 ,BLACK); 
	DrawTriangle({x_achse_abstand, y_achse_abstand - 5}, {x_achse_abstand - 7, y_achse_abstand + 20} ,{x_achse_abstand+7, y_achse_abstand + 20}, BLACK);
	DrawTriangle({WINDOW_WIDTH - x_achse_abstand + 5, WINDOW_HEIGHT - y_achse_abstand }, {WINDOW_WIDTH - x_achse_abstand - 20 , WINDOW_HEIGHT - y_achse_abstand - 7 },{WINDOW_WIDTH - x_achse_abstand -20 , WINDOW_HEIGHT - y_achse_abstand +7}, BLACK );

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
		




void Visual::drawErrorCurve(double value)
{
	if(value > max_cost)max_cost=value;
		
	error_points.push_back(value);
	if(error_points.size()>max_element)
	{
		error_points.pop_front();
	}	


	BeginDrawing();
			
	ClearBackground(DARKGRAY);
		


	drawCoordinateSystem();
	plotFunction();



	EndDrawing();	
}


