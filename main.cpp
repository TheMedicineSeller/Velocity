#include "winconsole.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#define BACKSPACE VK_BACK

#define HEADER "\n\\\\    // ||===>  |-|     //--\\\\  //___  |____| |____| \\\\  //\n"" \\\\__//  ||===>  |-|__  ||    || ||___   _||_    ||    \\\\//\n""  \\\\//   ||===>  |____|  \\\\--//  \\\\___| |____|   ||     ||\n"

/*------- GLOBAL INPUT ENTRY CONTROL FLAG -------*/
static bool OVER_FLAG;

/*------- LIST OF TEXTS TO TYPE -------
TODO: Addition to the Text collection will be done along with introducing RANDOM text Generators*/
const char* EvaluationTexts_short[] = {
	"C makes it easy to shoot yourself in the foot; C++ makes it harder, but when you do, it blows away your whole leg.",

	"The traditional mathematician recognizes and appreciates mathematical elegance when he sees it. I propose to go one step further, and to consider elegance an essential ingredient of mathematics: if it is clumsy, it is not mathematics.",

	"If you're always worried about crushing the ants beneath you... you won't be able to walk.",

	"#include <stdio.h>\nint main()\n{\nint x = 10;\nfor (int i = 0; i < x; i ++)\nprintf('Hello World ');\nreturn 0;\n}",

	"You'll hunt me. You'll condemn me, set the dogs on me. Because that's what needs to happen. Because sometimes the truth isn't good enough. Sometimes people deserve to have their faith rewarded."
};

const char* EvaluationTexts_medium[] = {
	"Velocity is one of a kind of typing speed evaluators. Run from the Windows command prompt, it allows for a unique command line experience of practicing and honing your typing speed and skills without the distractions, ads and javascript bloat that you might experience from any browser based evaluator.",

	"bucket established circulation maze loss option dribble salvation play examination miscarriage lead trick squash moving motivation punish aware intelligence harsh reputation race cancer source floor quality weakness crew lake aisle trouble remember modest reverse kick pardon retire scatter unit earwax document arise cheat autonomy",

	"Everything assumes such an immense quality when you are a child. You are a seed amongst the trees - waiting, waiting, waiting - for the soil and the sun to open you up, to release you from the feeling of smallness. And in that smallness rests another seed, one of fear or something like it - a lack of agency and hopelessness. ",
	
	"Written over half a century ago, Business Adventures remains relevant in its examination of the pitfalls and successes of the world of Wall Street. The book is a collection of stories written over John Brooks' career. Brooks excelled at writing insightful commentary about complicated business phenomenon in a way that was accessible to the everyday reader.",

	"Blood-red snakeclouds gathered in the western sky, and the rubber trees glowed as if on fire. Lu Beiping counted the pits he'd dug that day on the recently denuded hillside, picked up the squad leader's notebook, recorded his number, and stood scanning the list of names for Fong's mark. Nothing. She'd vanished."

};

const char* EvaluationTexts_long[] = {
	"My name is Yoshikage Kira. I'm 33 years old. My house is in the northeast section of Morioh, where all the villas are, and I am not married. I work as an employee for the Kame Yu department stores, and I get home every day by 8 PM at the latest. I don't smoke, but I occasionally drink. I'm in bed by 11 PM, and make sure I get eight hours of sleep, no matter what. After having a glass of warm milk and doing about twenty minutes of stretches before going to bed, I usually have no problems sleeping until morning. Just like a baby, I wake up without any fatigue or stress in the morning.",

	"I remember being reborn a thousand times.\nIn the between-times, and at the very start and end of a life, the memories are strong. I remember the warmth of the sun on my crumpled wings as they unfold for one brilliant bright day of flight. I relive the elation of the chase and the sinking of my teeth into the throat of my prey- what a joy it is to satisfy physical hunger. I recall the agony of being prey, because of course, all things must balance out in the end. My heart has pumped cold blood through sinuous coils and thumped its warm-blooded rhythm from ocean depths.\nLike you, I am the sum of a million experiences.\nIt's true that the messy, bloody rebirth into the world as a human stuns me every time. The shock of union as soul re-joins body. The violent expulsion through the birth canal. The startling sensation of air on skin, after an age of ethereal floating. The instinctive gasp, that first mortal wail. And so it begins again, and I'm here at the start of a new life.",

	"The Long Flight Home draws on the historical importance of carrier pigeons to craft a masterful novel about strength and hope in the midst of wartime crisis. The story focuses on Susan Shepherd, a young British woman who was orphaned as a child. Susan was raised by her grandfather Bertie, who taught her the art of raising homing pigeons. As the war gets closer, threatening everything she's ever known, Susan becomes involved in a covert mission to air-drop hundreds of homing pigeons in German-occupied France.",

	"Once, a hare saw a tortoise walking slowly with a heavy shell on his back. The hare was very proud of himself and he asked the tortoise. 'Shall we have a race?'. The tortoise agreed. They started the running race. The hare ran very fast. But the tortoise walked very slowly. The proud hair rested under a tree and soon slept off. But the tortoise walked very fast, slowly and steadily and reached the goal. At last, the tortoise won the race.\nMoral : Pride goes before a fall.",

	"equal invent necessary courageous sofa sticky hate macho produce adorable spiritual clap temper mellow fact apparatus group unequaled modern imperfect trick challenge coat chivalrous snatch present hanging sassy peaceful funny meek curious perpetual teaching face receipt small judge meal idea fall pickle abiding glue acrid filthy annoyed nod like guarantee puffy anxious excite juvenile sea stimulating tire peep knowing swing sleep heavy lackadaisical suit drab bewildered satisfy tart current adhesive join brick gigantic scared boundless"
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
	/*clrscr(opHandle);*/
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
	
	auto t_now = std::chrono::system_clock::now();
	srand(std::chrono::system_clock::to_time_t(t_now));

	int choice, randidx;
	std::string textbuffer, referenceText;
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

				std::cout<< "\nEnter 0 to practice with random predeclared text or 1 to practice with custom input: ";
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
								randidx = rand() % 5;
								referenceText = EvaluationTexts_short[randidx];
								duration = 30;
							break;
							case 2:
								// medium text
								randidx = rand() % 5;
								referenceText = EvaluationTexts_medium[randidx];
								duration = 60;
							break;
							case 3:
								// long text
								randidx = rand() % 5;
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
						// file read
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
				/*TODO : Write performance from stats into files and make inferences*/

				// std::cout<< "\nFor what duration in seconds do you want to test (max upto 2 mins<=>120 s) \nTest begins once entered: ";
				// std::cin>> duration;
			break;
			case 1:
				// inference
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