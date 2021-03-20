// CS170Project2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <cmath>


//read in data from file, TO DO: allow user to pass in file name
void load(std::vector<std::vector<float>>& data , char* name);

//intersection function to check that we arent adding a feature twice, returns true if there is no intersection
bool no_intersection(std::vector<int> current_set, int adding);

//clears features not being tested to 0 for forward selection
std::vector<std::vector<float>> clear_Columns(std::vector<std::vector<float>> data, std::vector<int> current_set, int feature_to_be_added);

std::vector<std::vector<float>> clear_Columns_base(std::vector<std::vector<float>> data, std::vector<int> current_set);

//clears features not being tested to 0 for backward elimination
std::vector<std::vector<float>> clear_ColumnsBack(std::vector<std::vector<float>> data, std::vector<int> current_set, int feature_to_be_added);

//cross validation
float leave_one_out_cross_validation(std::vector<std::vector<float>>& data, std::vector<int> current_set, int feature_to_add);

float leave_one_out_cross_validation_base(std::vector<std::vector<float>>& data, std::vector<int> current_set);

//cross validation
float leave_one_out_cross_validationBack(std::vector<std::vector<float>>& data, std::vector<int> current_set, int feature_to_add);

//forward selection
void forward_selection(std::vector<std::vector<float>>& data);

//backward_elimination
void backward_elimination(std::vector<std::vector<float>>& data);

int main()
{
	//srand(time(NULL));

	std::vector<std::vector<float>> data;
	char* file = new char[50];
	int choice;

	
	std::cout << "Samiha Kibria's Feature Selection Algorithm" << std::endl
		<< "Name of test file: ";
	
	std::cin >> file;

	std::cin.ignore(100, '\n');
	
	load(data, file);

	std::cout << std::endl << "Type the algorithm you want to run:" << std::endl
		<< "1. Forward Selection " << std::endl
		<< "2. Backward Elimination" << std::endl;

	std::cin >> choice;

	if (choice == 1)
	{
		forward_selection(data);
	}
	if (choice == 2)
	{
		backward_elimination(data);
	}

	return 0;
}


//read in data from file, TO DO: allow user to pass in file name
void load(std::vector<std::vector<float>>& data , char* name)
{
	std::ifstream file;
	file.open(name);


	do
	{
		std::vector<float> features;

		do
		{
			float num;

			file >> num;

			features.push_back(num);

		} while (file.peek() != '\n' && !file.eof());

		if (features.size() != 1)
		{
			data.push_back(features);
		}

		features.clear();

	} while (!file.eof());


	file.close();
}

//intersection function to check that we arent adding a feature twice, returns true if there is no intersection
bool no_intersection(std::vector<int> current_set, int adding)
{
	for (size_t i = 0; i < current_set.size(); i++)
	{
		if (current_set[i] == adding)
		{
			return false;
		}
	}

	return true;
}

//clears features not being tested to 0
std::vector<std::vector<float>> clear_Columns(std::vector<std::vector<float>> data, std::vector<int> current_set, int feature_to_be_added)
{
	std::vector<std::vector<float>> cleared = data;

	for (size_t i = 0; i < data.size(); i++)
	{
		for (size_t j = 1; j < data[0].size(); j++)
		{
			if (no_intersection(current_set, j) && j != feature_to_be_added)
			{
				cleared[i][j] = 0;
			}
		}
	}

	return cleared;
}

//clears features not being tested to 0
std::vector<std::vector<float>> clear_Columns_base(std::vector<std::vector<float>> data, std::vector<int> current_set)
{
	std::vector<std::vector<float>> cleared = data;

	for (size_t i = 0; i < data.size(); i++)
	{
		for (size_t j = 1; j < data[0].size(); j++)
		{
			if (no_intersection(current_set, j))
			{
				cleared[i][j] = 0;
			}
		}
	}

	return cleared;
}


//clears features not being tested to 0
std::vector<std::vector<float>> clear_ColumnsBack(std::vector<std::vector<float>> data, std::vector<int> current_set, int feature_to_be_added)
{
	std::vector<std::vector<float>> cleared = data;

	for (size_t i = 0; i < data.size(); i++)
	{
		for (size_t j = 1; j < data[0].size(); j++)
		{
			if (j == feature_to_be_added)
			{
				cleared[i][j] = 0;
			}
		}
	}

	return cleared;
}

float leave_one_out_cross_validation_base(std::vector<std::vector<float>>& data, std::vector<int> current_set)
{
	std::vector<std::vector<float>> cleared = clear_Columns_base(data, current_set);

	/*for (size_t i = 0; i < data[0].size(); i++)
	{
		std::cout << cleared[0][i] << " ";
	}

	std::cout << std::endl;*/

	int number_correct = 0;

	int count = 0;

	for (size_t i = 0; i < data.size(); i++)
	{
		std::vector<float> object_to_classify = cleared[i];
		float object_class = data[i][0];

		float nearest_neighbor_distance = std::numeric_limits<float>::infinity();
		float nearest_neighbor_location = std::numeric_limits<float>::max();
		float nearest_class;

		for (size_t k = 0; k < data.size(); k++)
		{
			if (k != i)
			{
				float distance = 0;

				for (size_t n = 1; n < object_to_classify.size(); n++)
				{
					float difference = object_to_classify[n] - data[k][n];

					float square = pow(difference, 2.0);

					distance = distance + square;

				}

				distance = sqrt(distance);

				//std::cout << distance;

				if (distance < nearest_neighbor_distance)
				{
					nearest_neighbor_distance = distance;
					nearest_neighbor_location = k;
					nearest_class = data[nearest_neighbor_location][0];
				}
			}

		}

		if (object_class == nearest_class)
		{
			number_correct++;
		}
	}

	return static_cast<float>(number_correct) / static_cast<float>(data.size());

}

//cross validation
float leave_one_out_cross_validation(std::vector<std::vector<float>>& data, std::vector<int> current_set, int feature_to_add)
{
	/*
	float accuracy = rand() % 101;

	accuracy = accuracy / 100;

	std::cout << accuracy;

	return accuracy;
	*/
	
	std::vector<std::vector<float>> cleared = clear_Columns(data, current_set, feature_to_add);

	/*for (size_t i = 0; i < data[0].size(); i++)
	{
		std::cout << cleared[0][i] << " ";
	}

	std::cout << std::endl;*/

	int number_correct = 0;

	int count = 0;

	for (size_t i = 0; i < data.size(); i++)
	{
		std::vector<float> object_to_classify = cleared[i];
		float object_class = data[i][0];

		float nearest_neighbor_distance = std::numeric_limits<float>::infinity();
		float nearest_neighbor_location = std::numeric_limits<float>::max();
		float nearest_class;

		for (size_t k = 0; k < data.size(); k++)
		{
			if (k != i)
			{
				float distance = 0;

				for (size_t n = 1; n < object_to_classify.size(); n++)
				{
					float difference = object_to_classify[n] - data[k][n];

					float square = pow(difference, 2.0);

					distance = distance + square;

				}

				distance = sqrt(distance);

				//std::cout << distance;

				if (distance < nearest_neighbor_distance)
				{
					nearest_neighbor_distance = distance;
					nearest_neighbor_location = k;
					nearest_class = data[nearest_neighbor_location][0];
				}
			}

		}

		if (object_class == nearest_class)
		{
			number_correct++;
		}
	}

	return static_cast<float>(number_correct) / static_cast<float>(data.size());

}

//cross validation
float leave_one_out_cross_validationBack(std::vector<std::vector<float>>& data, std::vector<int> current_set, int feature_to_add)
{
	/*
	float accuracy = rand() % 101;

	accuracy = accuracy / 100;

	std::cout << accuracy;

	return accuracy;
	*/

	std::vector<std::vector<float>> cleared = clear_ColumnsBack(data, current_set, feature_to_add);

	/*for (size_t i = 0; i < data[0].size(); i++)
	{
		std::cout << cleared[0][i] << " ";
	}

	std::cout << std::endl;*/

	int number_correct = 0;

	int count = 0;

	for (size_t i = 0; i < data.size(); i++)
	{
		std::vector<float> object_to_classify = cleared[i];
		float object_class = data[i][0];

		float nearest_neighbor_distance = std::numeric_limits<float>::infinity();
		float nearest_neighbor_location = std::numeric_limits<float>::max();
		float nearest_class;

		for (size_t k = 0; k < data.size(); k++)
		{
			if (k != i)
			{
				float distance = 0;

				for (size_t n = 1; n < object_to_classify.size(); n++)
				{
					float difference = object_to_classify[n] - data[k][n];

					float square = pow(difference, 2.0);

					distance = distance + square;

				}

				distance = sqrt(distance);

				//std::cout << distance;

				if (distance < nearest_neighbor_distance)
				{
					nearest_neighbor_distance = distance;
					nearest_neighbor_location = k;
					nearest_class = data[nearest_neighbor_location][0];
				}
			}

		}

		if (object_class == nearest_class)
		{
			number_correct++;
		}
	}

	return static_cast<float>(number_correct) / static_cast<float>(data.size());

}

//forward selection
void forward_selection(std::vector<std::vector<float>>& data)
{
	std::vector<int> current_set;

	float true_best = 0;
	std::vector<int> best_set;

	for (size_t i = 1; i < data[0].size(); i++)
	{
		std::cout << "On level " << i << " of the search tree" << std::endl;
		int feature_to_add = 0;
		float best_accuracy = 0;

		for (size_t k = 1; k < data[0].size(); k++)
		{
			if (no_intersection(current_set, k))
			{
				std::cout << " - Considering adding feature " << k << ". ";
				float accuracy = leave_one_out_cross_validation(data, current_set, k);
				std::cout << " Accuracy: " << accuracy << std::endl;
				
				if (accuracy > best_accuracy)
				{
					best_accuracy = accuracy;
					feature_to_add = k;
				}
			}

		}

		current_set.push_back(feature_to_add);

		if (best_accuracy > true_best)
		{
			true_best = best_accuracy;
			best_set = current_set;
		}

		std::cout << "Current set: ";

		for (size_t i = 0; i < current_set.size(); i++)
		{
			std::cout << current_set[i] << " ";
		}

		std::cout << " Accuracy: " << best_accuracy << std::endl;

		std::cout << "On level " << i << " feature " << feature_to_add << " was added to current set" << std::endl;

	}


	std::cout << "Best feature set: {";
	for (size_t i = 0; i < best_set.size(); i++)
	{
		if (i == best_set.size()-1)
		{
			std::cout << best_set[i];
		}
		else
		{
			std::cout << best_set[i] << ", ";
		}
	}

	std::cout << "} with accuracy " << true_best << std::endl;

}

//backward_elimination
void backward_elimination(std::vector<std::vector<float>>& data)
{
	std::vector<int> current_set;

	for (size_t i = 1; i < data[0].size(); i++)
	{
		current_set.push_back(i);
	}

	float true_best = 0;
	std::vector<int> best_set;

	float base_accuracy = leave_one_out_cross_validation_base(data, current_set);

	std::cout << "Level 0 accuracy: " << base_accuracy << std::endl;

	for (size_t i = 1; i < data[0].size(); i++)
	{
		std::cout << "On level " << i << " of the search tree" << std::endl;
		int feature_to_remove = 0;
		float best_accuracy = 0;


		for (size_t k = 1; k < data[0].size(); k++)
		{
			if (!no_intersection(current_set, k))
			{
				std::cout << " - Considering removing feature " << k << ". ";
				float accuracy = leave_one_out_cross_validationBack(data, current_set, k);
				std::cout << " Accuracy: " << accuracy << std::endl;

				if (accuracy > best_accuracy)
				{
					best_accuracy = accuracy;
					feature_to_remove = k;
				}
			}

		}

		for (size_t n = 0; n < current_set.size(); n++)
		{
			if (current_set[n] == feature_to_remove)
			{
				current_set.erase(current_set.begin() + n);
				break;
			}
		}

		if (best_accuracy > true_best)
		{
			true_best = best_accuracy;
			best_set = current_set;
		}

		std::cout << "Current set: ";

		for (size_t i = 0; i < current_set.size(); i++)
		{
			std::cout << current_set[i] << " ";
		}

		std::cout << " Accuracy: " << best_accuracy << std::endl;

		std::cout << "On level " << i << " feature " << feature_to_remove << " was removed from current set" << std::endl;

	}


	std::cout << "Best feature set: {";
	for (size_t i = 0; i < best_set.size(); i++)
	{
		if (i == best_set.size() - 1)
		{
			std::cout << best_set[i];
		}
		else
		{
			std::cout << best_set[i] << ", ";
		}
	}

	std::cout << "} with accuracy " << true_best << std::endl;

}

