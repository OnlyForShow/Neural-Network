#include "Neural.h"
#include "Visual.h"
#include <csignal>
#include <functional>
#include <iostream>
#include <iomanip>
#include <random>
#include <algorithm>


bool running = true;

int main()
{


	NN network;

	Visual visualizer(1000,800,"Neural Visualizer", network);
	
	


	network.setInput(2);
	
	network.setLayer(10,2, tanh, d_tanh);
	network.setLayer(10,10, ReLu, d_ReLu);
	network.setLayer(1,10, sigmoid, d_sigmoid);
	
	network.setOutput(1);
	
	while(visualizer.state())
	{
		running = true;
		training_set training_data = visualizer.selectPoints();
			
		if(training_data[0].empty())continue;
		printTraining(training_data);

			
		double learning_rate = 0.01;
		size_t epoch = 1000000;

		size_t label_number = 0;

		std::vector<size_t> random_item(training_data[0].size());
		std::generate(random_item.begin(), random_item.end(), [&label_number](){ return label_number++;});
		std::random_device rd;
		std::mt19937 gen(rd());
		
		double J_AVG = 0.0;	
		
		std::signal(SIGINT, [](int a){running = false;});	
		
		//#define VERBOSE
		for(int i = 1; i <= epoch && running; i++)
		{

			std::ranges::shuffle(random_item, gen);

			for(int k = 0; k < training_data[0].size(); k++)
			{
				#ifdef VERBOSE
				std::cout<<"\r-------------------------------------------\n";
				vector y_nn = network.feed(training_data[0][random_item[k]] );	
				std::cout<<"Soll: ";
				printVector(training_data[1][random_item[k]]);
				std::cout<<"Ist: ";
				printVector(y_nn);
				network.train(y_nn,training_data[1][random_item[k]], learning_rate);
				network.print();
				#else
				vector y_nn = network.feed(training_data[0][random_item[k]]);	
				network.train(y_nn,training_data[1][random_item[k]], learning_rate);
				#endif

				J_AVG += network.currentCost();
			}

			std::cout<<"\repoch ["<<i<<"/"<<epoch<<"  "<<std::fixed<<std::setprecision(0)<<(double(i*100)/double(epoch))<<" %]  error = "<<std::setprecision(10)<<J_AVG/(training_data.size())<<std::flush;
			if(i % 100 == 0)
			{
				J_AVG/=(training_data.size()*100.0);
				visualizer.push_back(J_AVG);
				J_AVG = 0.0;
				visualizer.draw();
			}
			if(IsKeyPressed(KEY_Q))
			{
				running = false;	
			}
		}
			
		std::cout<<"\nFinal network: \n\n\n"<<std::setprecision(6);
		
		network.print();
		
	}
	

	return 0;

}
