#include "Encrypter.h"

void Encrypter::loadFile() {
	do {
		std::cout << "Pls, write name of file: ";
		std::cin >> filename;

		std::string path = filename + ".data"; // Making proper name(path) of the file

		try {
			std::ifstream load(path);
			std::string temp;

			while (std::getline(load, temp)) {
				data.push_back(temp);
			}

			load.close();

			if (data.empty()) {
				throw std::runtime_error("File is empty or not find");
			}

			if (data[0] == "#Encrypted") {
				mode = deciphering; // Setting mode to work with decrypting
			}
			else {
				mode = encrypting; // Setting mode to work with encrypting 
			}

			return;
		}
		catch (const std::runtime_error& error) {
			std::cerr << error.what() << std::endl;
		}
		catch (...) {
			std::cerr << "Unexpected error occurred while loading the file" << std::endl;
		}
	} while (true);
}

void Encrypter::saveToFile() {
	if (filename.empty() || data.empty()) { // Checking to make sure that program has data to save, to avoid accidental save
		std::cerr << "Pls, load file before saving the result" << std::endl;
		return;
	}

	std::string path = filename;
	switch (mode) {
	case encrypting:
		path += "_enc.data"; // Adds an ending to name to understand type of file - encrypted file
		break;
	case deciphering:
		path += "_dec.data"; // Adds an ending to name to understand type of file - decrypted file
		break;
	}
	
	try {
		std::ofstream save(path);

		if (mode == encrypting) {
			save << "#Encrypted" << std::endl; // At the top of the file will be set the mode to work with
		}

		for (int i = mode; i < data.size(); i++) {
			save << data[i] << std::endl; // Saving each row to the file
		}

		save.close();
	}
	catch (std::runtime_error& error) {
		std::cerr << error.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Unexpected error occurred while saving data to the file" << std::endl;
	}
}

void Encrypter::startWork() {
	// "i" for rows and "j" for columns, to be precise each char in row
	for (int i = mode; i < data.size(); i++) {
		for (int j = 0; j < data[i].size(); j++) {
			data[i][j] ^= Key[j % Key.size()]; // Encrypting each symbol in array data by each symbol in key
		}
	}
}

void singleThreadStartWork(const int threadId,const int& threadCount,std::vector<std::string>& data, const std::string& Key) {
	// "i" for rows and "j" for columns, to be precise each char in row, also to make automatically allocation for multi threading 
	// was used threadId to set starting point and threadCount to set lenght of the single move 
	for (int i = threadId; i < data.size(); i += threadCount) {
		for (int j = 0; j < data[i].size(); j++) {
			data[i][j] ^= Key[j % Key.size()]; // Encrypting each symbol in array data by each symbol in key
		}
	}
}

void Encrypter::multiThreadStartWork() {
	// Setting threadCount depending on amount of data (max is 6)
	switch (data.size()) {
	case 0:
		std::cerr << "Your data is empty, pls, load correct data" << std::endl;
		return;
	case 1:
		if (mode == deciphering) {
			std::cerr << "Your data is empty, pls, load correct data" << std::endl;
			return;
		}
		else
		{
			threadCount = 1;
		}
		break;
	case 2:
		threadCount = 1;
		break;
	case 3:
		threadCount = 2;
		break;
	case 4:
		threadCount = 3;
		break;
	case 5:
		threadCount = 4;
		break;
	case 6:
		threadCount = 5;
		break;
	case 7:
		threadCount = 6;
		break;
	default:
		threadCount = 6;
		break;
	}

	std::thread** threads = new std::thread*[threadCount]; // Allocating memory for saving threads in array

	// Creating threads
	for (int i = 0; i < threadCount; i++) {
		threads[i] = new std::thread(singleThreadStartWork, i, std::ref(threadCount), std::ref(data), std::ref(Key));
	}
	// Waiting for threads
	for (int i = 0; i < threadCount; i++) {
		threads[i]->join();
	}

	delete[] threads;
	threads = nullptr;
}