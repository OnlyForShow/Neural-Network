#include <vector>
#include <time.h>
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
//#include "raygui.h"


using matrix = std::vector<std::vector<double>>;
using vector = std::vector<double>;


double randomDouble(double min, double max)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<double> dis(min,max);
	return dis(gen);
}

double f(double x)
{
	return 2*x;
}

double g()
{
	return randomDouble(-1000,1000);
}

double ReLu(double x)
{
	return std::max(0.0,x);
}

double d_ReLu(double x)
{
	if(x > 0)return 1;
	else return 0;
}



matrix generateTrainingData(size_t number)
{
	matrix training_set;
	training_set.reserve(2);
	
	vector input(number,0);
	training_set.emplace_back(std::move(input));
	vector output(number,0);
	training_set.emplace_back(std::move(output));



	for(size_t i = 0; i <number; i++)
	{
		double x = g();
		training_set[0][i] = x;
		training_set[1][i] = f(x);
	}
	return training_set;
}



void printTraining(matrix& training)
{
	for(size_t i = 0; i < training[0].size(); i++)
	{
		std::cout<<"["<<training[0][i]<<", "<<training[1][i]<<"]\n";
	}
}



struct Layer
{
		double (*activation_function)(double);
		double (*d_activation_function)(double);
		matrix weight_matrix;
		vector bias;

		size_t output_parameter,input_parameter;

		Layer(size_t output_parameter, size_t input_parameter,double (*activation)(double), double (*d_activation)(double))
		: output_parameter(output_parameter), input_parameter(input_parameter), activation_function(activation), d_activation_function(d_activation)
		{
			weight_matrix.reserve(output_parameter);
			for(int i = 0; i < output_parameter; i++)
			{
				vector temp;
				temp.reserve(input_parameter);
				for(int j = 0; j < input_parameter; j++)
				{
					temp.push_back(randomDouble(-1000,1000));	
				}
				weight_matrix.emplace_back(std::move(temp));
			}

			bias.reserve(output_parameter);
			for(int i = 0; i < output_parameter; i++)
			{
				bias.push_back(0);
			}
		}

		vector feed(const vector& input)
		{
			vector output;		
			
			for(int m = 0; m < output_parameter; m++)
			{
				for(int n = 0; n < input_parameter; n++)
				{
					output[m] += weight_matrix[m][n] * input[n];
				}

			}
			
			return output;
		}

		vector activate(const vector& input)
		{
			vector output;		
			
			for(int m = 0; m < input.size(); m++)
			{
				output[m] = activation_function(input[m]);
			}
			
			return output;

		}

		void train(const matrix& weightUpdate, const vector& biasUpdate, double learning_rate)
		{
			for(int m = 0; m < output_parameter; m++)
			{
				for(int n = 0; n < input_parameter; n++)
				{
					weight_matrix[m][n] = weight_matrix[m][n] -  learning_rate * weightUpdate[m][n];
				}
			}
			for(int m = 0; m < output_parameter; m++)
			{
				
				bias[m] = bias[m] -  learning_rate * biasUpdate[m];
			
			}
		}
	};

	struct NN
	{
		std::vector<Layer> layers;
		std::vector<vector> z;
		std::vector<vector> a;

		size_t input,output;

		void setInput(size_t N)
		{
			input = N;
			vector z_0(N,0.0);	
			z.push_back(z_0);

			vector a_0(N,0.0);	
			a.push_back(a_0);

		}
		void setOutput(size_t M)
		{
			output = M;
		}

		void setLayer(size_t M, size_t N, double (*activation)(double), double (*d_activation)(double))
		{
			layers.emplace_back(M,N,activation,d_activation);

			vector z_i(M,0.0);	
			z.emplace_back(std::move(z_i));

			vector a_i(M,0.0);	
			a.emplace_back(std::move(a_i));	
		}

		vector feed(const vector& input)
		{
			z[0] = input; 
			for(int i = 0; i < layers.size(); i++)	
			{
				z[i+1] = layers[i].feed(z[i]);
				a[i+1] = layers[i].activate(z[i+1]);
			}
			return a[layers.size()];
		}

		void train(const vector& y_nn, const vector& y, double learning_rate)
		{
			
		}
			
	};

	int main()
	{
		SetRandomSeed(time(0));
		matrix training_data = generateTrainingData(10);		
			
		printTraining(training_data);
	/*	
		NN network;

		network.setInput(1);
		network.setLayer(2,1,ReLu, d_ReLu);
		network.setLayer(3,2,ReLu, d_ReLu);
		network.setLayer(2,3,ReLu, d_ReLu);
		network.setLayer(1,2,ReLu, d_ReLu);
		network.setOutput(1);

		double learning_rate = 0.01;
		size_t steps = 100;
		for(int i = 0; i < steps; i++)
		{
			vector y_nn = network.feed(trainingset[0][i]);	
			network.train(y_nn,trainingset[1][i], learning_rate);
		}
	*/

		return 0;
	}
