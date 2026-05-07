#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>

class Encrypter
{
	bool hasKey{ false }; // By default is false, meaning key not loaded yet
	std::string Key; // Encryption/Decryption key
	std::string filename; // Name of the file program work with at the moment
	std::vector<std::string> data; // Data to encrypt or decipher
	enum modes { // Available modes that program can done
		encrypting,
		deciphering
	};
	modes mode; // Saving current mode
	int threadCount; // Number of threads, that will be allocated at function "multiThreadStartWork"
	enum keyCondition { // Check the key status
		good,
		bad
	};
	enum threadMode { // Choose thread mode
		multi,
		single
	};
	threadMode chosedThreadMode;

public:
	

	// For testing saved key
	//std::string getKey() {
	//	return Key;
	//}

	/// <summary>
	/// Input name of file(without ending .data) and load all data from file to vector for encrypting/decrypting 
	/// </summary>
	void loadFile();

	/// <summary>
	/// Save encrypted/decrypted data to the file using before written name and adding to the end ending .data,
	/// also if program was encrypting, at the start of file will be text "#Encrypted"
	/// 
	/// File type "name".data
	/// </summary>
	void saveToFile();

	/// <summary>
	/// Single thread(main) encryptor/decryptor, using written key encrypt or decrypt loaded data
	/// </summary>
	void startWork();
	
	/// <summary>
	/// Do the same work like startWork, 
	/// but using multi threaded optimization with automatically counted max amount of threads for task
	/// </summary>
	void multiThreadStartWork();

	#pragma region thread mode options
	
	void setThreadMode(int i) {
		if (i == 0) {
			chosedThreadMode = multi;
		}
		else
		{
			chosedThreadMode = single;
		}
	}

	int getThreadMode() {
		return chosedThreadMode;
	}

	#pragma endregion

	keyCondition checkKey() {
		return Key.size() > 0 ? good : bad;
	}

	std::string getFileName() {
		return filename + (mode == encrypting ? "_enc" : "_dec") + ".data";
	}

	// Save the encryption key
	friend std::istream& operator>>(std::istream& is, Encrypter& enc) {
		std::string tempStr;
		is >> tempStr;

		// Checking on empty input
		if (tempStr.empty()) {
			std::cerr << "Your input empty" << std::endl;
			return is;
		}
		// Checking on spaces in key
		if (tempStr.find(' ') < 100) {
			std::cerr << "Pls, write key without spaces between them" << std::endl;
			return is;
		}
		// Checking on size > 100
		if (tempStr.size() > 100) {
			std::cerr << "You wrote too long key, maximum lenght is 100 symbols" << std::endl;
			return is;
		}

		enc.hasKey = true; // Key is now loaded
		enc.Key = tempStr;
		return is;
	}
};

