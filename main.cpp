#include "winconsole.h"
#include "texthandler.h"
#include <thread>
#include <chrono>

#define BACKSPACE VK_BACK

#define HEADER "\n\\\\    // ||===>  |-|     //--\\\\  //___  |____| |____| \\\\  //\n"" \\\\__//  ||===>  |-|__  ||    || ||___   _||_    ||    \\\\//\n""  \\\\//   ||===>  |____|  \\\\--//  \\\\___| |____|   ||     ||\n"

/*------- GLOBAL INPUT ENTRY CONTROL FLAG -------*/
static bool OVER_FLAG;


inline float heaviside (float x) {
	return (x > 0) ? x : 0;
}

std::pair<float, float> getTypingStats (std::string input, uint16_t duration, const char* actual, uint16_t totalErrors) {
	uint16_t typedlen = input.length();
	uint16_t tempErrors = 0;

	for (uint16_t itr = 0; itr < typedlen; itr ++) {
		if (input[itr] != actual[itr])
			tempErrors ++;
	}
	float speed = (typedlen / 5.0f - tempErrors) * 60.0f / duration;
	float accuracy = 100.0f - 100.0f*totalErrors / typedlen ;
	return std::make_pair(heaviside(speed), heaviside(accuracy));
}

// Suggestion : make O(1) speed calculation inside Handler to improve efficiency <- needs a lot of rewrite
void DynamicInputHandler (HANDLE opHandle, HANDLE ipHandle, std::string &Text, const char* correct, uint16_t duration, uint16_t &permaErrors) {
	
	clearBG(opHandle);
	COORD pos;
	uint16_t ptr = 0, len = strlen(correct), linebreakptr;
	uint8_t key, correctKey;
	std::string opText;
	std::pair<float, float> stats;
	
	SetColorGold(opHandle);
	WriteFile(opHandle, correct, len, NULL, NULL);
	uint8_t endrow = getCursorPosition(opHandle).Y;

	gotoxy(opHandle, 0, 0);
	while (!OVER_FLAG) {
		if (ptr >= len) {
			OVER_FLAG = true;
			break;
		}
		correctKey = correct[ptr];
		if (correctKey == '\n')
			linebreakptr = pos.X;
		key = getch(ipHandle);
		if ((key == BACKSPACE)) {
			if (ptr > 0) {
				setBgColorBlack(opHandle);
				WriteFile(opHandle, &correctKey, 1, NULL, NULL);
				if (pos.X == 0){

					if (correct[ptr-1] == '\n')
						gotoxy(opHandle, linebreakptr, pos.Y - 1);
					else
						gotoxy(opHandle, getScreenWidth(opHandle) - 1, pos.Y - 1);
				}
				gotoxy(opHandle, pos.X - 1, pos.Y);
				Text.pop_back();
				ptr --;
			}
			
		} else {
			if (key == correct[ptr])
				SetBgColorGreen(opHandle);
			else {
				SetBgColorRed(opHandle);
				permaErrors ++;
			}
			WriteFile(opHandle, &correctKey, 1, NULL, NULL);
			Text.push_back(key);
			ptr ++;
		}
		if (OVER_FLAG)
			break;
		stats = getTypingStats(Text, duration, correct, permaErrors);
		pos = getCursorPosition(opHandle);
		gotoxy(opHandle, 0, endrow + 1);
		opText = "Typing Speed: " + std::to_string(stats.first) + " wpm :: Accuracy: " + std::to_string(stats.second) + " %";
		setBgColorBlack(opHandle);
		WriteFile(opHandle, opText.c_str(), opText.size(), NULL, NULL);
		gotoxy(opHandle, pos.X, pos.Y);
	}
	setBgColorBlack(opHandle);
	gotoxy(opHandle, 0, endrow + 3);
	
	if (ptr >= len)
		WriteFile(opHandle, "\nCompleted..(Press ENTER for Menu)", 35, NULL, NULL);
	else
		WriteFile(opHandle, "\nTime over! (Press ENTER for Menu)", 35, NULL, NULL);
}

uint16_t Timer(uint16_t duration) {
	using namespace std::literals::chrono_literals;
	uint16_t time = 0;
	while (!OVER_FLAG) {
		std::this_thread::sleep_for(1s);
		time ++;
		if (time >= duration)
			break;
	}
	OVER_FLAG = true;
	return time;
}


int main () {
	
	HANDLE opHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (opHandle == NULL) {
		std::cout<< "ERROR::CONSOLE::OUTPUT::NOT FOUND";
		exit(EXIT_FAILURE);
	}
	HANDLE ipHandle = GetStdHandle(STD_INPUT_HANDLE);
	if (ipHandle == NULL) {
		std::cout<< "ERROR::CONSOLE::INPUT::NOT FOUND";
		exit(EXIT_FAILURE);
	}

	std::random_device entropy_seed;
	std::mt19937 randomEngine(entropy_seed());

	LoadParasfromText("paras.txt");
	LoadPermutationsfromText("commonWords5000.txt", randomEngine);

	std::uniform_int_distribution<int> RNG_short (0, EvaluationTexts_short.size()  - 1);
	std::uniform_int_distribution<int> RNG_medium(0, EvaluationTexts_medium.size() - 1);
	std::uniform_int_distribution<int> RNG_long  (0, EvaluationTexts_long.size()   - 1);

	int choice, randidx;
	std::string textbuffer, referenceText, customFilePath;
	uint16_t duration, time_took, permaErrors;
	
	SetColorBlue(opHandle);
	WriteFile(opHandle, HEADER, sizeof(HEADER), NULL, NULL);
	do {
		bool validInput = true;
		SetColorAqua(opHandle);
		std::cout<< "\nMenu :: Enter 0 to test your typing speed, 1 to infer performance history, 2 to quit: ";
		std::cin>> choice;
		switch (choice) {
			case 0:
				OVER_FLAG = false;
				textbuffer.clear();
				permaErrors = 0;

				std::cout<< "\nEnter 0 to practice with random text or 1 to practice with custom input: ";
				std::cin>> choice;
				switch (choice)
				{
					case 0:
						std::cout<< "\nEnter the length of text preferred..\nShort (1)[30 seconds time], Medium (2)[1 minute time], Long (3)[2 minutes time]\nTest begins once entered: ";
						std::cin>> choice;
						
						switch (choice)
						{
							case 1:
								// short text
								randidx = RNG_short(randomEngine);
								referenceText = EvaluationTexts_short[randidx];
								duration = 30;
								
							break;
							case 2:
								// medium text
								randidx = RNG_medium(randomEngine);
								referenceText = EvaluationTexts_medium[randidx];
								duration = 60;
								
							break;
							case 3:
								// long text
								randidx = RNG_long(randomEngine);
								referenceText = EvaluationTexts_long[randidx];
								duration = 120;
								
							break;
							default:
								std::cout<< "Entered invalid option...\n";
								validInput = false;
							break;
						}
					break;
					case 1:
						std::cout<< "Enter the path(absolute or relative to the executable folder) of the text file you want to practice with: ";
						std::cin>> customFilePath;
						LoadCustomText(customFilePath);
						{
						std::uniform_int_distribution<int> RNG_custom(0, EvaluationTexts_custom.size()); 
						randidx = RNG_custom(randomEngine);
						}
						referenceText = EvaluationTexts_custom[randidx];
						duration = 100;

					break;
					default:
						std::cout<< "Entered invalid option...\n";
						validInput = false;
					break;
				}
				
				if (validInput) {
					std::thread runner(DynamicInputHandler, opHandle, ipHandle, std::ref(textbuffer), referenceText.c_str(), duration, std::ref(permaErrors));
					time_took = Timer(duration);
					runner.join();
					
					/* Adjusting the final scores to the time they took to complete text */
					std::pair<float, float> final = getTypingStats(textbuffer, time_took, referenceText.c_str(), permaErrors);

					COORD pos = getCursorPosition(opHandle);
					gotoxy(opHandle, 0, pos.Y - 3);
					std::string final_display = "Typing Speed: " + std::to_string(final.first) + " wpm :: Accuracy: " + std::to_string(final.second) + " %";
					WriteFile(opHandle, final_display.c_str(), final_display.size(), NULL, NULL);
					gotoxy(opHandle, pos.X, pos.Y);
				}

				std::cin.sync();
				std::cin.get();
			break;
			case 1:
				/*TODO : Write performance into file and performance graph?*/
			break;
			case 2:
				std::cout<< "\nQuitting Velocity...";
				SetColorWhite(opHandle);
				return 0;
			break;
			default:
				std::cout<< "\nEnter a valid option ...\n";
				validInput = false;
			break;
		}
	}
	while (true);
	SetColorWhite(opHandle);
	return 0;
}
