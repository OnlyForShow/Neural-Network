#include <vector>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <random>
#include <algorithm>
#include <cassert>



//#define NDEBUG

//Uncomment NDEBUG to disable all asserts in this file

#define assertm(exp, msg) assert((void(msg),exp))


#define NORMALISIERUNG 1.0


using matrix = std::vector<std::vector<double>>;
using vector = std::vector<double>;
using training_set = std::vector<std::vector<vector>>; 

double UniformRandomDouble(double min, double max)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<double> dis(min,max);
	return dis(gen);
}

double NormalRandomDouble(double mean, double stddev)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::normal_distribution<double> dis(mean,stddev);
	return dis(gen);
}

double f(double x)
{
	//the neural network should learn this function
	return (2.0 * sin(x/20.0));
}

double g(double min, double max)
{
	//we generate random x values for our training_set
	return UniformRandomDouble(min,max);
}



training_set generateTrainingData(size_t number)
{
	training_set set;
	set.reserve(2);
	
	std::vector<vector> input(number,vector());
	set.emplace_back(std::move(input));
	std::vector<vector> output(number,vector());
	set.emplace_back(std::move(output));

	

	for(size_t i = 0; i <number; i++)
	{
		double x = g(-5.0,5.0);
		set[0][i].push_back(x/NORMALISIERUNG);
		set[1][i].push_back(f(x)/NORMALISIERUNG);
	}

	return set;
}


void printMatrix(const matrix& mat) {	

	size_t N = mat[0].size() - 1;
	for(int m = 0; m < mat.size(); m++)
	{	
		std::cout<<"[ ";
		for(int n = 0; n < N; n++)
		{
			std::cout<<mat[m][n]<<", ";
		}
		std::cout<<mat[m][N]<<" ]\n";
	}
}

void printVector(const vector& vec) 
{	
	std::cout<<"[ ";
	for(int m = 0; m < vec.size(); m++)
	{	
		std::cout<<vec[m]<<", ";
	}
	std::cout<<" ]\n";
}


void printTraining(training_set& training)
{
	for(size_t i = 0; i < training[0].size(); i++)
	{
		std::cout<<"[ ";
		for(size_t k = 0; k < training[0][0].size() - 1; k++)
		{
			std::cout<<training[0][i][k]<<", ";	
		}
		std::cout<<training[0][i][training[0][0].size() - 1]<<" ] = [ ";	

		for(size_t k = 0; k < training[1][0].size() - 1; k++)
		{
			std::cout<<training[1][i][k]<<", ";	
		}
		std::cout<<training[1][i][training[1][0].size() - 1]<<" ]\n";	

	}
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

double sigmoid(double x)
{
	return 1.0/(1+std::exp(-x));
}

double d_sigmoid(double x)
{
	return sigmoid(x)*(1.0 - sigmoid(x));
}

double tanhyp(double x)
{
	return std::tanh(x);
}

double d_tanh(double x)
{
	return 1.0 - (std::tanh(x)*std::tanh(x));
}

double ident(double x)
{
	return x;
}

double d_ident(double x)
{
	return 1;
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
					temp.push_back(NormalRandomDouble(0,2.0/input_parameter));	
				}
				weight_matrix.emplace_back(std::move(temp));
			}
			
			bias = vector(output_parameter,0.0);
			
		}


		vector feed(const vector& input)
		{
			vector output(output_parameter, 0.0);		

			assertm(bias.size()==output_parameter, "unexpected bias dimension");
			assertm(weight_matrix.size()==output_parameter, "unexpected output weight dimension");
			assertm(weight_matrix[0].size()==input_parameter, "unexpected input weight dimension");

			
			for(int m = 0; m < output_parameter; m++)
			{
				for(int n = 0; n < input_parameter; n++)
				{
					output[m] += weight_matrix[m][n] * input[n];
				}

				output[m] = output[m] + bias[m];

			}
			
			return output;
		}

		vector activate(const vector& input)
		{
			vector output(input.size(), 0);		

			
			for(int m = 0; m < input.size(); m++)
			{

				output[m] = activation_function(input[m]);

			}
			
			return output;

		}

		vector reverse(const vector& input)
		{
			vector output(input.size(), 0);		
		
			for(int m = 0; m < input.size(); m++)
			{
				output[m] = d_activation_function(input[m]);
			}
			
			return output;

		}

		void train(const matrix& weightUpdate, const vector& biasUpdate, double learning_rate)
		{
			assertm(biasUpdate.size()==bias.size(), "biasUpdate dimension does not match bias dimension");
			assertm(weight_matrix.size()== weightUpdate.size(), "weightUpdadte output dimension does not match weight_matrix output dimension");
			assertm(weight_matrix[0].size()== weightUpdate[0].size(), "weightUpdadte input dimension does not match weight_matrix input dimension");


			for(int m = 0; m < output_parameter; m++)
			{
				for(int n = 0; n < input_parameter; n++)
				{
					weight_matrix[m][n] = weight_matrix[m][n] - learning_rate * weightUpdate[m][n];
				}
			}

			for(int m = 0; m < output_parameter; m++)
			{
				
				bias[m] = bias[m] -  learning_rate * biasUpdate[m];
			
			}
		}

		matrix& getWeights()
		{
			return weight_matrix;
		}

		vector& getBias()
		{
			return bias;
		}

		void printWeights()
		{
			printMatrix(weight_matrix);
		}

		void printBias()
		{
			printVector(bias);
		}
	};

	struct NN
	{
		std::vector<Layer> layers;
		std::vector<vector> z;
		std::vector<vector> a;

		size_t input,output;
		double J = 0.0;

		void setInput(size_t N)
		{
			input = N;
			vector z_0(N,1.0);	
			z.emplace_back(z_0);

			vector a_0(N,0.0);	
			a.emplace_back(a_0);

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

		Layer& getLayer(size_t t)
		{
			assertm(t < layers.size(), "There are not that many layers!");
			if(t<layers.size())
			{
				return layers[t];	
			}


		}

		vector feed(const vector& input)
		{
			assertm(input.size()==this->input, "Input vector dimension do not match!");
			z[0] = input;
			a[0] = input;
			
			for(int i = 0; i < layers.size(); i++)	
			{
				z[i+1] = layers[i].feed(a[i]);
				a[i+1] = layers[i].activate(z[i+1]);
			}
			
			return a[layers.size()];
		
		}

		void print()
		{
			for(int i = 0; i < layers.size(); i++)
			{
				std::cout<<"Weights_"<<(i+1)<<" : \n";
				layers[i].printWeights();
				std::cout<<"Bias_"<<(i+1)<<" : \n";
				layers[i].printBias();
			}
		}

		double calcCost(vector y_tt, vector y)
		{
			assertm(y_tt.size() == y.size(), "both y vectors must have the same dimension");
			double cost = 0.0;	
			for(int i = 0; i < y_tt.size(); i++)
			{
				double d = y_tt[i] - y[i];
				cost += d*d;

				
			}
			cost/=2;
			
			return cost;	
		}

		double currentCost()
		{
			return J;
		}

		void train(const vector& y_nn, const vector& y, double learning_rate)
		{

			J = 0.0;	
			vector derivative = layers[layers.size()-1].reverse(z[layers.size()]);
			vector delta(y_nn.size(),0);				
			for(int i = 0; i < delta.size(); i++)
			{
				delta[i] = y_nn[i] - y[i];
				J += delta[i]*delta[i];
				delta[i] = delta[i] * derivative[i];

				
			}
			J/=2;
			
			{
				matrix d_J_d_W; 

				//dJ/dW^[k] = delta[k] * x^T;
				for(int m = 0; m < delta.size(); m++)	
				{
					vector tmp;
					for(int n = 0; n < a[layers.size()-1].size(); n++)
					{
						tmp.push_back(delta[m]*a[layers.size()-1][n]);	
					}
					d_J_d_W.emplace_back(std::move(tmp));
				} 

				//train(const matrix& weightUpdate, const vector& biasUpdate, double learning_rate)
				layers[layers.size()-1].train(d_J_d_W, delta, learning_rate);
			}


			for(int i = layers.size()-1; i > 0; i--)	
			{
				const matrix& weight = layers[i].getWeights();
				vector delta_new(weight[0].size(),0.0);
				
				assertm(weight.size() == delta.size(), "Unmatched dimension matrix input and vector");
				assertm(weight[0].size() == delta_new.size(), "Unmatched dimension matrix input and vector");

				// delta[k] = W^T[k+1] * delta[k+1]
				for(int n = 0; n < weight[0].size(); n++)
				{
					for(int m = 0; m < weight.size(); m++)
					{
						delta_new[n] += weight[m][n] * delta[m];
					}
				}
				
				vector derivative = layers[i-1].reverse(z[i]);

				// delta[k] = delta[k] *(element-wise) derivative(z[k])	
				for(int m = 0; m < delta_new.size(); m++)	
				{
					delta_new[m] = delta_new[m] * derivative[m];	
				}


				matrix d_J_d_W; 

				//dJ/dW^[k] = delta[k] * a^T[k-1];
				for(int m = 0; m < delta_new.size(); m++)	
				{
					vector tmp;
					for(int n = 0; n < a[i-1].size(); n++)
					{
						tmp.push_back(delta_new[m] * a[i-1][n]);	
					}
					d_J_d_W.emplace_back(std::move(tmp));
				} 

				//train(const matrix& weightUpdate, const vector& biasUpdate, double learning_rate)
				layers[i-1].train(d_J_d_W, delta_new, learning_rate);
				delta = delta_new;
			}

		}
			
	};

