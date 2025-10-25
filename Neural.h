#pragma once
#include <vector>



//#define NDEBUG

//Uncomment NDEBUG to disable all asserts in this file

#define assertm(exp, msg) assert((void(msg),exp))


#define NORMALISIERUNG 1.0


using matrix = std::vector<std::vector<double>>;
using vector = std::vector<double>;
using training_set = std::vector<std::vector<vector>>; 



training_set generateTrainingData(size_t number);

void printMatrix(const matrix& mat);
void printVector(const vector& vec);



void printTraining(training_set& training);

double ReLu(double x);

double d_ReLu(double x);

double sigmoid(double x);

double d_sigmoid(double x);

double tanhyp(double x);


double d_tanh(double x);

double ident(double x);

double d_ident(double x);



struct Layer
{
		double (*activation_function)(double);
		double (*d_activation_function)(double);
		matrix weight_matrix;
		vector bias;

		size_t output_parameter,input_parameter;

		Layer(size_t output_parameter, size_t input_parameter,double (*activation)(double), double (*d_activation)(double));
		

		vector feed(const vector& input);
		vector activate(const vector& input);
		vector reverse(const vector& input);
		
		void train(const matrix& weightUpdate, const vector& biasUpdate, double learning_rate);

		
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

		void reset();
	};

	struct NN
	{
		std::vector<Layer> layers;
		std::vector<vector> z;
		std::vector<vector> a;

		size_t input,output;
		double J = 0.0;

		void setInput(size_t N);
		void setOutput(size_t M);

		void setLayer(size_t M, size_t N, double (*activation)(double), double (*d_activation)(double));

		Layer& getLayer(size_t t);
		size_t getNumberOfLayer();

		vector feed(const vector& input);

		void print();

		double calcCost(vector y_tt, vector y);

		double currentCost();

		void train(const vector& y_nn, const vector& y, double learning_rate);

		void reset();
			
	};

