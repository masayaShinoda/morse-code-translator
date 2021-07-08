#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <string.h>
#include <fstream> // to open and read morse code dictionary txt
#include <thread>  // std::this_thread::sleep_for
#include <chrono>  // std::chrono::seconds
#include <SFML/Audio.hpp>
using namespace std;
using std::this_thread::sleep_for;

string concatDirectory(string directory, char soundName) {
	string s(1, soundName);
	directory.append(s);
	directory.append(".wav");
	return directory;
}

void playSound(string dir, char character)
{
	// if there's space, skip function iteration
	if (dir == "./sound/ .wav" || dir == ".\\sound\\ .wav" || character == ' ') {
		constexpr int TIME_TO_SLEEP = 500;
		for (int i = 0; i < 10; ++i) {
			if (i == 4) {
				sleep_for(std::chrono::milliseconds(TIME_TO_SLEEP));
			}
		}
		return;
	}
		// Load a sound buffer from a wav file
		sf::SoundBuffer buffer;
		if (!buffer.loadFromFile(dir))
			return;
		// Create a sound instance and play it
		sf::Sound sound(buffer);
		sound.play();
		// Loop while the sound is playing
		while (sound.getStatus() == sf::Sound::Playing)
		{
			// Leave some CPU time for other processes
			sf::sleep(sf::milliseconds(100));
			// Display the playing position
			std::cout << "\rPlaying " << character << " ";
			std::cout << std::flush;
		}
}

void englishToMorse() {
	//variables
	string userInput;
	string directory;
	string baseDir = ".\\sound\\";
	cout << "Enter your message and it will be converted into morse code sounds! (alphabetical characters only please)" << endl;
	cin.ignore();
	getline(cin, userInput);

	//check userInput length
	int userInputLength = 0;
	while (userInput[userInputLength])
		userInputLength++;

	//check for invalid inputs
	for (int i = 0; i < userInputLength; i++) {
		if (!isalpha(userInput[i])) {
			// if it's space then it's okay
			if (userInput[i] == ' ') {
				continue;
			}
			else {
				cout << "Invalid input, only alphabets allowed!" << endl;
				cout << endl << "Press enter to exit. line 71";
				getchar();
				return;
			}
		}
	}

	//convert userInput string to message char array
	vector<char> message(userInputLength);

	//declare array of audios to execute
	vector<string> audios(userInputLength);
	for (int i = 0; i < userInputLength; i++) {
		message[i] = userInput[i];
	}

	// format directory
	for (int i = 0; i < size(message); i++) {
		string dir = concatDirectory(baseDir, message[i]);
		audios[i] = dir;
	}

	// notify user to turn on audio
	if (userInput != "") {
		// don't print if user input is empty
		cout << "Sound on! :)" << endl;
	}

	//play a sound for each character
	for (int i = 0; i < size(audios); i++) {
		playSound(audios[i], message[i]);
		constexpr int TIME_TO_SLEEP = 1000;
		for (int i = 0; i < 10; ++i) {
			if (i == 4) {
				sleep_for(std::chrono::milliseconds(TIME_TO_SLEEP));
			}
		}
	}
}

string morseDictionary[26][26] = {
	{"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"},
	{
		".-",
		"-...",
		"-.-.",
		"-..",
		".",
		"..-.",
		"--.",
		"....",
		"..",
		".---",
		"-.-",
		".-..",
		"--",
		"-.",
		"---",
		".--.",
		"--.-",
		".-.",
		"...",
		"-",
		"..-",
		"...-",
		".--",
		"-..-",
		"-.--",
		"--..",
	}
};

class Translation {
	public:
		string character;
		string morseCode;
};

void morseToEnglish() {
	string userInput;
	Translation translate; // generate Translation object
	string morse[26]; // array of translate.morseCodes
	string alphabet[26]; // array of translate.character
	// print to user morsecode guide
	cout << "Refer to this translation guide:" << endl;
	for (int i = 0; i < 26; i++) {
		cout << morseDictionary[0][i] << " | " << morseDictionary[1][i] << endl;
		translate.character = morseDictionary[0][i];
		translate.morseCode =  morseDictionary[1][i];
		morse[i] = translate.morseCode;
		alphabet[i] = translate.character;
	}
	cout << "Enter morse code character to translate to English: " << endl;
	cout << "Example: .-" << endl;
	//write to file
	ofstream outputFile;
	// File Open
	outputFile.open("message.txt");
	// Write to the file
	outputFile << "Translated message: " << endl;
	
	while (true) {
		cout << ">> ";
		cin.ignore();
		cin >> userInput;
		cout << "Successfully written to message.txt | Type x to exit" << endl;
		//exit upon receiving x input
		if (userInput == "x" || userInput == "X") {
			// File Close
			outputFile.close();
			return;
		}

		//check userInput length
		int userInputLength = 0;
		while (userInput[userInputLength])
			userInputLength++;
		//check for invalid user input
		if (userInputLength > 4 || userInputLength < 1) {
			cout << "Invalid input." << endl;
			break;
		}

		//loop through morse array to find what character it is
		for (int i = 0; i < 26; i++) {
			if (userInput == morse[i]) {
				cout << "Translation: " << alphabet[i] << endl;
				// Write to the file
				outputFile << alphabet[i] << endl;					
			}
		}
	}
	
	cout << endl << "Press enter to exit.";
	getchar();
	return;
}


int main() {
	int choice;
	cout << "Choose between:" << endl;
	cout << "1. Translate English to morse code sound." << endl;
	cout << "2. Translate morse code to English." << endl;
	cin >> choice;

	if (choice == 1) {
		englishToMorse();
	} else if (choice == 2) {
		morseToEnglish();
	} else {
		cout << "Invalid input." << endl;
		return 0;

	}
	cout << endl << "Press any key to exit.";
	getchar();
	return 0;
}