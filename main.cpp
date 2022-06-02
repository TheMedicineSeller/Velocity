#include "winconsole.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#define BACKSPACE VK_BACK

#define HEADER "\n\\\\    // ||===>  |-|     //--\\\\  //___  |____| |____| \\\\  //\n"" \\\\__//  ||===>  |-|__  ||    || ||___   _||_    ||    \\\\//\n""  \\\\//   ||===>  |____|  \\\\--//  \\\\___| |____|   ||     ||\n"

/*------- GLOBAL INPUT ENTRY CONTROL FLAG -------*/
static bool overflag;

/*------- LIST OF TEXTS TO TYPE -------
TODO: Addition to the Text collection will be done along with introducing RANDOM text Generators*/
const char* EvaluationTexts[] = {
	"C makes it easy to shoot yourself in the foot; C++ makes it harder, but when you do, it blows away your whole leg.",

	"The traditional mathematician recognizes and appreciates mathematical elegance when he sees it. I propose to go one step further, and to consider elegance an essential ingredient of mathematics: if it is clumsy, it is not mathematics.",

	"Velocity is one of a kind of typing speed evaluators. Run from the Windows command prompt, it allows for a unique command line experience of practicing and honing your typing speed and skills without the distractions, ads and javascript bloat that you might experience from any browser based evaluator.",

	"If you're always worried about crushing the ants beneath you... you won't be able to walk.",

	"My name is Yoshikage Kira. I'm 33 years old. My house is in the northeast section of Morioh, where all the villas are, and I am not married. I work as an employee for the Kame Yu department stores, and I get home every day by 8 PM at the latest. I don't smoke, but I occasionally drink. I'm in bed by 11 PM, and make sure I get eight hours of sleep, no matter what. After having a glass of warm milk and doing about twenty minutes of stretches before going to bed, I usually have no problems sleeping until morning. Just like a baby, I wake up without any fatigue or stress in the morning.",

	"bucket established circulation maze loss option dribble salvation play examination miscarriage lead trick squash moving motivation snuggle encourage momentum bishop punish aware intelligence harsh reputation race cancer source floor quality weakness crew lake aisle trouble remember modest reverse kick pardon retire scatter unit earwax document arise cheat autonomy"
};

float heaviside (float x) {
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
	clrscr(opHandle);
	COORD pos;
	uint16_t ptr = 0, len = strlen(correct);
	uint8_t key, correctKey;
	std::string opText;
	std::pair<float, float> stats;
	
	SetColorGold(opHandle);
	WriteFile(opHandle, correct, len, NULL, NULL);
	gotoxy(opHandle, 0, 0);
	uint8_t endrow = len / getScreenWidth(opHandle) + 1;
	while (!overflag) {
		if (ptr >= len) {
			overflag = true;
			break;
		}
		correctKey = correct[ptr];
		key = getch(ipHandle);
		if ((key == VK_BACK)) {
			if (ptr > 0) {
				setBgColorBlack(opHandle);
				WriteFile(opHandle, &correctKey, 1, NULL, NULL);
				if (pos.X == 0)
					gotoxy(opHandle, getScreenWidth(opHandle) - 1, pos.Y - 1);
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
		if (overflag)
			break;
		stats = getTypingStats(Text, duration, correct, permaErrors);
		pos = getCursorPosition(opHandle);
		gotoxy(opHandle, 0, endrow + 1);
		opText = "Typing Speed: " + std::to_string(stats.first) + " :: Accuracy: " + std::to_string(stats.second) + " %";
		setBgColorBlack(opHandle);
		WriteFile(opHandle, opText.c_str(), opText.size(), NULL, NULL);
		gotoxy(opHandle, pos.X, pos.Y);
	}
	setBgColorBlack(opHandle);
	gotoxy(opHandle, 0, endrow + 3);
	
	if (ptr >= len)
		WriteFile(opHandle, "\nCompleted..(Press ENTER for Menu)", 36, NULL, NULL);
	else
		WriteFile(opHandle, "\nTime over! (Press ENTER for Menu)", 36, NULL, NULL);
}

uint16_t Timer(uint16_t duration) {
	using namespace std::literals::chrono_literals;
	uint16_t time = 0;
	while (!overflag) {
		std::this_thread::sleep_for(1s);
		time ++;
		if (time >= duration)
			break;
	}
	overflag = true;
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
	
	auto t_now = std::chrono::system_clock::now();
	srand(std::chrono::system_clock::to_time_t(t_now));

	int choice, randidx;
	std::string textbuffer, final_display;
	uint16_t duration, time_took, permaErrors;
	std::pair<float, float> final;
	
	SetColorBlue(opHandle);
	WriteFile(opHandle, HEADER, sizeof(HEADER), NULL, NULL);
	do {
		SetColorAqua(opHandle);
		std::cout<< "\nMenu :: Enter 0 to test your typing speed , 1 to infer performance history, 2 to quit: ";
		std::cin>> choice;
		switch (choice) {
			case 0:
				overflag = false;
				std::cout<< "\nFor what duration in seconds do you want to test (max upto 2 mins<=>120 s) \nTest begins once entered: ";
				std::cin>> duration;
				{
				textbuffer.clear();
				permaErrors = 0;
				randidx = rand() % 5;
				std::thread runner(DynamicInputHandler, opHandle, ipHandle, std::ref(textbuffer), EvaluationTexts[randidx], duration, std::ref(permaErrors));
				time_took = Timer(duration);
				runner.join();
				}

				/* Adjusting the final scores to the time they took to complete text */
				{
				final = getTypingStats(textbuffer, time_took, EvaluationTexts[randidx], permaErrors);
				COORD pos = getCursorPosition(opHandle);
				gotoxy(opHandle, 0, pos.Y - 3);
				final_display = "Typing Speed: " + std::to_string(final.first) + " :: Accuracy: " + std::to_string(final.second) + " %";
				WriteFile(opHandle, final_display.c_str(), final_display.size(), NULL, NULL);
				gotoxy(opHandle, pos.X, pos.Y);
				}
				std::cin.sync();
				std::cin.get();
				/*TODO : Write performance from stats into files and make inferences*/
			break;
			case 1:
			
			break;
			case 2:
				std::cout<< "\nQuitting Velocity...";
				SetColorWhite(opHandle);
				return 0;
			break;
			default : std::cout<< "\nEnter a valid option ...\n";
			break;
		}
	}
	while (true);
	SetColorWhite(opHandle);
	return 0;
}