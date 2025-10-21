#include "Neural.h"
#include "Visual.h"


int main()
{

	Visual visualizer;	

	visualizer.setup(1000,800,"Neural Visualizer");


	size_t training_size = 100;
	training_set training_data = generateTrainingData(training_size);		
		
	printTraining(training_data);
	
	NN network;

	network.setInput(1);
	
	network.setLayer(8,1,tanh, d_tanh);
	network.setLayer(16,8,ReLu, d_ReLu);
	network.setLayer(8,16,sigmoid, d_sigmoid);
	network.setLayer(1,8, ident, d_ident);
	
	network.setOutput(1);
	
	
	
	
		
	double learning_rate = 0.001;
	size_t epoch = 400000;
	
	std::vector<size_t> random_item(training_data[0].size());
	std::generate(random_item.begin(), random_item.end(), [](){static size_t i = 0; return i++;});
	std::random_device rd;
	std::mt19937 gen(rd());
	
	
	//#define VERBOSE
	for(int i = 1; i <= epoch; i++)
	{

		std::ranges::shuffle(random_item, gen);
		for(int k = 0; k < training_data.size(); k++)
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
			vector y_nn = network.feed(training_data[0][random_item[k]] );	
			network.train(y_nn,training_data[1][random_item[k]], learning_rate);
			#endif

		}
		std::cout<<"\repoch ["<<i<<"/"<<epoch<<"  "<<std::fixed<<std::setprecision(0)<<(double(i*100)/double(epoch))<<" %]  error = "<<std::setprecision(10)<<network.currentCost()<<std::flush;
		if(i % 1000 == 0)
		{
			visualizer.drawErrorCurve(network.currentCost());
		}
	}
	std::cout<<"\nFinal network: \n\n\n"<<std::setprecision(6);
	
	network.print();
	
	training_data = generateTrainingData(training_size);		

	std::vector<vector> cases;
	std::cout<<"Test Cases: \n";	
	for(int i = 0; i < training_data[0].size(); i++)
	{
		std::cout<<"---------------------------------\n";
		vector input = training_data[0][i];
		std::cout<<"Input: \n";
		printVector(input);
		std::cout<<"Expected: \n";
		printVector(training_data[1][i]);
		std::cout<<"Prediction: \n";
		vector output = network.feed(input);
		printVector(output);	
		std::cout<<"Error = "<<network.calcCost(output, training_data[1][i])<<"\n";
	}
	
	
	return 0;

}
